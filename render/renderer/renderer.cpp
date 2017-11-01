#include "renderer.h"

#include <assert.h>
#include <iostream>

#include <QColor>

#include "../geometry/geometry.h"
#include "../geometry/vertexattrib.h"
#include "../geometry/vertexlayout.h"

#include "../material/material.h"
#include "../material/renderpass.h"
#include "../material/shaderparam.h"
#include "../material/shaderprogram.h"
#include "../material/shaderutils.h"

#include "../scene.h"

#include "drawcommand.h"


Renderer::Renderer() :
    m_bufferManager(),
    m_shaderManager(),
    m_gl(nullptr),
    m_glWrapper(),
    m_shaderIds(),
    m_vaos(),
    m_arrayVbos(),
    m_indexVbos()
{}

Renderer::~Renderer()
{
    cleanup();
}

void Renderer::initialize(Scene *scene)
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

    // Create VBOs
    m_arrayVbos[0].type = GLBuffer::Type::ArrayBuffer;
    m_arrayVbos[0].usage = GLBuffer::Usage::StaticDraw;
    m_glWrapper.createBuffer(m_arrayVbos[0]);

    m_indexVbos[0].type = GLBuffer::Type::IndexBuffer;
    m_indexVbos[0].usage = GLBuffer::Usage::StaticDraw;
    m_glWrapper.createBuffer(m_indexVbos[0]);

    m_arrayVbos[1].type = GLBuffer::Type::ArrayBuffer;
    m_arrayVbos[1].usage = GLBuffer::Usage::StreamDraw;
    m_glWrapper.createBuffer(m_arrayVbos[1]);

    // Create shaders and VAOs
    m_shaderIds[0] = m_glWrapper.buildShaderProgram(scene->materials[0]->renderPasses()[0]->shaderProgram());
    m_shaderIds[1] = m_glWrapper.buildShaderProgram(scene->materials[1]->renderPasses()[0]->shaderProgram());

    m_gl->glGenVertexArrays(2, m_vaos.data());

    // Setup terrain geometry
    VertexLayout terrainVertexLayout;
    VertexAttrib standardVertexAttrib {"vertexPos", 3, VertexAttrib::Type::Float, false, 0};
    terrainVertexLayout.addAttribute(standardVertexAttrib);

    m_glWrapper.setupVaoForBufferAndShader(m_shaderIds[0], m_vaos[0],
                                           terrainVertexLayout,
                                           m_arrayVbos[0],
                                           &m_indexVbos[0]);

    // Setup particles geometry
    VertexLayout particlesVertexLayout;
    particlesVertexLayout.addAttribute(standardVertexAttrib);

    m_glWrapper.setupVaoForBufferAndShader(m_shaderIds[1], m_vaos[1],
                                           particlesVertexLayout,
                                           m_arrayVbos[1]);

    m_glWrapper.printAnyError();
}

void Renderer::render(Scene *scene, float dt)
{
    Q_UNUSED (dt);

    m_bufferManager.addGeometry(scene->geometries[0], &m_arrayVbos[0], &m_indexVbos[0]);
    m_bufferManager.addGeometry(scene->geometries[1], &m_arrayVbos[1], nullptr);

    m_shaderManager.addShaderProgram(scene->materials[0]->renderPasses()[0]->shaderProgram(), m_shaderIds[0]);
    m_shaderManager.addShaderProgram(scene->materials[1]->renderPasses()[0]->shaderProgram(), m_shaderIds[1]);

    updateDirtyBuffers(scene->geometries);

    updatePassParameters(scene->materials);

    const std::vector<DrawCommand> commands = prepareDrawCommands(scene);

    m_gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_glWrapper.draw(commands);

    m_glWrapper.printAnyError();
}

void Renderer::cleanup()
{
    m_gl->glDeleteVertexArrays(2, m_vaos.data());

    m_glWrapper.destroyShaderProgram(m_shaderIds[0]);
    m_glWrapper.destroyShaderProgram(m_shaderIds[1]);

    m_glWrapper.destroyBuffer(m_arrayVbos[0]);
    m_glWrapper.destroyBuffer(m_arrayVbos[1]);
    m_glWrapper.destroyBuffer(m_indexVbos[0]);

    m_glWrapper.printAnyError();
}

void Renderer::updateDirtyBuffers(const std::vector<Geometry *> &geoms)
{
    for (Geometry *geom : geoms) {
        if (geom->isDirty) {
            std::pair<GLBuffer *, GLBuffer *> buffers =
                    m_bufferManager.buffersForGeometry(geom);

            // Upload vertices
            const GLBuffer &vertexGLBuffer = *buffers.first;

            const std::vector<QVector3D> &vertices = geom->vertices;

            m_glWrapper.bindBuffer(vertexGLBuffer);
            m_glWrapper.allocateBuffer(vertexGLBuffer,
                                       vertices.size() * Geometry::vertexSize,
                                       vertices.data());
            m_glWrapper.releaseBuffer(vertexGLBuffer);

            // Upload indices, if any
            GLBuffer *indexGLBuffer = buffers.second;

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
    }

    m_glWrapper.printAnyError();
}

void Renderer::updatePassParameters(const std::vector<Material *> &materials)
{
    for (const Material *mat : materials) {
        const uptr_vector<RenderPass> &passes = mat->renderPasses();

        for (const uptr<RenderPass> &pass : passes) {
            assert (pass);

            const uint32 programId =
                    m_shaderManager.shaderIdForShaderProgram(pass->shaderProgram());

            m_glWrapper.sendUniforms(programId, pass->params());
        }
    }
}

std::vector<DrawCommand> Renderer::prepareDrawCommands(Scene *scene)
{
    std::vector<DrawCommand> ret;
    ret.reserve(scene->geometries.size());

    for (int i = 0; i < scene->geometries.size(); i++) {
        Geometry *geometry = scene->geometries[i];

        std::pair<GLBuffer *, GLBuffer *> gpuBuffers =
                m_bufferManager.buffersForGeometry(geometry);

        const DrawCommand cmd {
            m_shaderIds[i], m_vaos[i],
            *geometry, *gpuBuffers.first, gpuBuffers.second
        };

        ret.emplace_back(cmd);
    }

    return ret;
}
