#include "renderer.h"

#include <assert.h>
#include <iostream>

#include <QDebug>

#include "3rdparty/entityx/Entity.h"

#include "render/geometry/geometry.h"
#include "render/geometry/vertexattrib.h"
#include "render/geometry/vertexlayout.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"
#include "render/material/shaderprogram.h"
#include "render/material/shaderutils.h"
#include "render/material/texture.h"

#include "render/camera.h"
#include "render/transform.h"

#include "render/renderer/drawcommand.h"


Renderer::Renderer() :
    m_bufferManager(),
    m_shaderManager(),
    m_vaoManager(),
    m_textureManager(),
    m_gl(nullptr),
    m_glWrapper(),
    m_drawCommands(),
    m_currentPassParams(),
    m_activeTextures()
{}

Renderer::~Renderer()
{
    cleanup();
}

void Renderer::initialize()
{
    QOpenGLContext *currentGLContext = QOpenGLContext::currentContext();

    m_gl = currentGLContext->versionFunctions<OpenGLFuncs>();
    const bool initialized = m_gl->initializeOpenGLFunctions();
    Q_ASSERT_X (initialized,
                "Renderer::initialize()", "OpenGL 3.3 failed to initialize");

    m_glWrapper.initialize(currentGLContext);

    m_gl->glClearColor(0.f, 0.f, 0.f, 1.f);

    m_gl->glEnable(GL_DEPTH_TEST);
    m_gl->glDepthFunc(GL_LESS);

//    m_gl->glEnable(GL_BLEND);
//    m_gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_activeTextures.reserve(m_glWrapper.maxTextureUnits());

    m_glWrapper.checkForErrors();
}

void Renderer::startNewFrame()
{
    m_gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::prepareDrawCommand(Geometry &geometry,
                                  Material &material,
                                  Transform &transform)
{
    const DrawCommand cmd = createDrawCommand(geometry,
                                              material,
                                              transform);

    m_drawCommands.emplace_back(cmd);
}

void Renderer::render(Camera &camera, float dt)
{
    for (DrawCommand &drawCmd : m_drawCommands) {
        // Allocate resources if necessary
        if (drawCmd.glVao == 0) {
            createGLResources(drawCmd.geometry, drawCmd.material, drawCmd);
        }

        updateDirtyBuffers(drawCmd);
        updatePassParameters(camera, drawCmd);
    }

    m_glWrapper.draw(m_drawCommands);

    m_drawCommands.clear();


    m_glWrapper.checkForErrors();
}

void Renderer::cleanup()
{
    if (!m_glWrapper.isInitialized()) {
        return;
    }

    m_bufferManager.cleanup(m_glWrapper);
    m_textureManager.cleanup(m_glWrapper);
    m_shaderManager.cleanup(m_glWrapper);
    m_vaoManager.cleanup(m_glWrapper);


    m_glWrapper.checkForErrors();
}

DrawCommand Renderer::createDrawCommand(Geometry &geometry,
                                        Material &material,
                                        Transform &transform) const
{
    Geometry *geomPtr = &geometry;

    std::pair<GLBuffer *, GLBuffer *> gpuBuffers =
            m_bufferManager.buffersForGeometry(geomPtr);

    //FIXME Handle other passes
    GLShaderProgram *glProgram =
            m_shaderManager.get(material.renderPasses()[0]->shaderProgram());

    // Create draw command
    const DrawCommand ret {
        glProgram, m_vaoManager.get(geomPtr),
        geometry, material, transform,
        gpuBuffers.first, gpuBuffers.second
    };

    return ret;
}

void Renderer::createGLResources(Geometry &geom, Material &material, DrawCommand &drawCmd)
{
    //TODO Use clearer way to check if the resources are already allocated ?
    if (m_vaoManager.isAllocated(&geom)) {
        return;
    }

    // Generate a vao
    GLVao *glVao = m_vaoManager.addGeometry(&geom, m_glWrapper);

    // Generate GPU buffers
    std::pair<GLBuffer *, GLBuffer *> gpuBuffers =
            m_bufferManager.addGeometry(&geom, m_glWrapper);
    assert (gpuBuffers.first);

    // Generate shader program
    ShaderProgram *shaderProgram = material.renderPasses()[0]->shaderProgram();
    GLShaderProgram *glProgram =
            m_shaderManager.addShaderProgram(shaderProgram, m_glWrapper);

    m_glWrapper.createShaderProgram(*glProgram, *shaderProgram);


    m_glWrapper.setupVaoForBufferAndShader(*glProgram, *glVao,
                                           geom.vertexLayout,
                                           *gpuBuffers.first,
                                           gpuBuffers.second);

    drawCmd.glVao = glVao;
    drawCmd.glProgram = glProgram;
    drawCmd.glVertexBuffer = gpuBuffers.first;
    drawCmd.glIndexBuffer = gpuBuffers.second;
}

void Renderer::updateDirtyBuffers(DrawCommand &drawCmd)
{
    Geometry &geom = drawCmd.geometry;

    if (geom.isDirty) {
        // Upload vertices
        GLBuffer *vertexGLBuffer = drawCmd.glVertexBuffer;

        const std::vector<Vertex> &vertices = geom.vertices;

        m_glWrapper.allocateBuffer(*vertexGLBuffer,
                                   vertices.size() * Geometry::vertexSize,
                                   vertices.data());

        // Upload indices, if any
        GLBuffer *indexGLBuffer = drawCmd.glIndexBuffer;

        if (indexGLBuffer) {
            const std::vector<uint32> &indices = geom.indices;

            m_glWrapper.allocateBuffer(*indexGLBuffer,
                                       indices.size() * Geometry::indexSize,
                                       indices.data());
        }

        geom.isDirty = false;
    }


    m_glWrapper.checkForErrors();
}

void Renderer::updatePassParameters(Camera &camera, const DrawCommand &drawCmd)
{
    m_activeTextures.clear();
    m_currentPassParams.clear();

    // Note that material parameters override pass parameters
    Material &material = drawCmd.material;

    for (const uptr<ShaderParam> &materialParam : material.params()) {
        m_currentPassParams.push_back(materialParam.get());
    }

    const uptr_vector<RenderPass> &passes = material.renderPasses();

    for (const uptr<RenderPass> &pass : passes) {
        assert (pass);

        GLShaderProgram *glProgram = m_shaderManager.get(pass->shaderProgram());

        for (const uptr<ShaderParam> &passParam : pass->params()) {
            ShaderParam *overridingParam = material.param(passParam->name);

            if (!overridingParam) {
                m_currentPassParams.push_back(passParam.get());
            }
        }

        int textureUnitCounter = 0;
        for (ShaderParam *param : m_currentPassParams) {
            if (param->value.userType() == qMetaTypeId<Texture2D>()) {
                auto texture = param->value.value<Texture2D>();
                if (!texture.path.empty()) {
                    m_activeTextures.push_back(param);
                }

                textureUnitCounter++;
            }
        }

        m_glWrapper.sendTextureUniforms(*glProgram, m_activeTextures, m_textureManager);

        m_glWrapper.sendActiveCameraUniforms(*glProgram,
                                             camera.worldMatrix(),
                                             camera.viewMatrix(),
                                             camera.projectionMatrix());

        m_glWrapper.sendTransformUniform(*glProgram, drawCmd.transform.matrix());

        m_glWrapper.sendUniforms(*glProgram, m_currentPassParams);
    }


    m_glWrapper.checkForErrors();
}
