#include "renderer.h"

#include <assert.h>
#include <iostream>

#include <QDebug>

#include "3rdparty/entityx/Entity.h"

#include "../geometry/geometry.h"
#include "../geometry/vertexattrib.h"
#include "../geometry/vertexlayout.h"

#include "../material/material.h"
#include "../material/renderpass.h"
#include "../material/shaderparam.h"
#include "../material/shaderprogram.h"
#include "../material/shaderutils.h"

#include "drawcommand.h"


Renderer::Renderer() :
    m_bufferManager(),
    m_shaderManager(),
    m_vaoManager(),
    m_gl(nullptr),
    m_glWrapper(),
    m_drawCommands()
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

    //    gl->glEnable(GL_BLEND);
    //    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_glWrapper.checkForErrors();
}

void Renderer::startNewFrame()
{
    m_gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::prepareDrawCommand(entityx::Entity entity)
{
    auto geometry = entity.component<Geometry>();
    auto material = entity.component<Material>();

    assert (geometry && material);

    const DrawCommand cmd = createDrawCommand(*geometry.get(), *material.get());

    m_drawCommands.emplace_back(cmd);
}

void Renderer::render(float dt)
{
    for (DrawCommand &drawCmd : m_drawCommands) {
        // Allocate resources if necessary
        if (drawCmd.vaoId == 0) {
            createGLResources(*drawCmd.geometry, *drawCmd.material, drawCmd);
        }

        updateDirtyBuffers(drawCmd);
        updatePassParameters(drawCmd);
    }

    m_glWrapper.draw(m_drawCommands);

    m_drawCommands.clear();

    m_glWrapper.checkForErrors();
}

void Renderer::cleanup()
{
    m_bufferManager.cleanup(m_glWrapper);
    m_shaderManager.cleanup(m_glWrapper);
    m_vaoManager.cleanup(m_glWrapper);


    m_glWrapper.checkForErrors();
}

DrawCommand Renderer::createDrawCommand(Geometry &geometry,
                                        Material &material) const
{
    Geometry *geomPtr = &geometry;
    Material *materialPtr = &material;

    std::pair<GLBuffer *, GLBuffer *> gpuBuffers =
            m_bufferManager.buffersForGeometry(geomPtr);

    //FIXME Handle other passes
    const uint32 shaderProgramId =
            m_shaderManager.shaderIdForShaderProgram(material.renderPasses()[0]->shaderProgram());

    // Create draw command
    const DrawCommand ret {
        shaderProgramId, m_vaoManager.vaoForGeometry(geomPtr),
        geomPtr, materialPtr,
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
    const uint32 vaoId = m_vaoManager.addGeometry(&geom, m_glWrapper);

    // Generate GPU buffers
    std::pair<GLBuffer *, GLBuffer *> gpuBuffers =
            m_bufferManager.addGeometry(&geom, m_glWrapper);
    assert (gpuBuffers.first);

    // Generate shader program
    const uint32 programId =
            m_glWrapper.buildShaderProgram(material.renderPasses()[0]->shaderProgram());

    m_shaderManager.addShaderProgram(material.renderPasses()[0]->shaderProgram(),
            programId);

    m_glWrapper.setupVaoForBufferAndShader(programId, vaoId,
                                           geom.vertexLayout,
                                           *gpuBuffers.first,
                                           gpuBuffers.second);

    drawCmd.vaoId = vaoId;
    drawCmd.shaderProgramId = programId;
    drawCmd.vertexGLBuffer = gpuBuffers.first;
    drawCmd.indexGLBuffer = gpuBuffers.second;
}

void Renderer::updateDirtyBuffers(DrawCommand &drawCmd)
{
    Geometry *geom = drawCmd.geometry;

    if (geom->isDirty) {
        // Upload vertices
        GLBuffer *vertexGLBuffer = drawCmd.vertexGLBuffer;

        const std::vector<QVector3D> &vertices = geom->vertices;

        m_glWrapper.bindBuffer(*vertexGLBuffer);
        m_glWrapper.allocateBuffer(*vertexGLBuffer,
                                   vertices.size() * Geometry::vertexSize,
                                   vertices.data());
        m_glWrapper.releaseBuffer(*vertexGLBuffer);

        // Upload indices, if any
        GLBuffer *indexGLBuffer = drawCmd.indexGLBuffer;

        if (indexGLBuffer) {
            const std::vector<uint32> &indices = geom->indices;

            m_glWrapper.bindBuffer(*indexGLBuffer);
            m_glWrapper.allocateBuffer(*indexGLBuffer,
                                       indices.size() * Geometry::indexSize,
                                       indices.data());
            m_glWrapper.releaseBuffer(*indexGLBuffer);
        }

        geom->isDirty = false;
    }


    m_glWrapper.checkForErrors();
}

void Renderer::updatePassParameters(const DrawCommand &drawCmd)
{
    const uptr_vector<RenderPass> &passes = drawCmd.material->renderPasses();

    for (const uptr<RenderPass> &pass : passes) {
        assert (pass);

        const uint32 programId =
                m_shaderManager.shaderIdForShaderProgram(pass->shaderProgram());

        m_glWrapper.sendUniforms(programId, pass->params());
    }


    m_glWrapper.checkForErrors();
}
