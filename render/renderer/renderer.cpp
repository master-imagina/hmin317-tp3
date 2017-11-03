#include "renderer.h"

#include <assert.h>
#include <iostream>

#include <QDebug>

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
    m_vaos()
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

    auto onRenderableAdded = [this] (Geometry *geom, Material *material) {
        //TODO OMG optimize that
        // Generate a vao
        m_vaos.reserve(m_vaos.capacity() + 1);

        uint32 newVaoId = 0;
        m_gl->glGenVertexArrays(1, &newVaoId);
        m_vaos.emplace_back(newVaoId);

        // Generate GPU buffers
        std::pair<GLBuffer *, GLBuffer *> gpuBuffers =
                m_bufferManager.addGeometry(geom, m_glWrapper);
        assert (gpuBuffers.first);

        // Generate shader program
        const uint32 programId =
                m_glWrapper.buildShaderProgram(material->renderPasses()[0]->shaderProgram());

        m_shaderManager.addShaderProgram(material->renderPasses()[0]->shaderProgram(),
                programId);

        m_glWrapper.setupVaoForBufferAndShader(programId, m_vaos.back(),
                                               geom->vertexLayout,
                                               *gpuBuffers.first,
                                               gpuBuffers.second);
    };

    scene->connectRenderableAdded(onRenderableAdded);

    // Allocate the scene in GPU, if exists
    for (int i = 0; i < scene->geometries().size(); i++) {
        onRenderableAdded(scene->geometries()[i], scene->materials()[i]);
    }


    m_glWrapper.checkForErrors();
}

void Renderer::render(Scene *scene, float dt)
{
    Q_UNUSED (dt);

    updateDirtyBuffers(scene->geometries());
    updatePassParameters(scene->materials());

    const std::vector<DrawCommand> commands = prepareDrawCommands(scene);

    m_gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_glWrapper.draw(commands);


    m_glWrapper.checkForErrors();
}

void Renderer::cleanup()
{
    m_gl->glDeleteVertexArrays(m_vaos.size(), m_vaos.data());

    m_bufferManager.cleanup(m_glWrapper);
    m_shaderManager.cleanup(m_glWrapper);


    m_glWrapper.checkForErrors();
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


    m_glWrapper.checkForErrors();
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


    m_glWrapper.checkForErrors();
}

std::vector<DrawCommand> Renderer::prepareDrawCommands(Scene *scene)
{
    const std::vector<Geometry *> geometries = scene->geometries();
    const std::vector<Material *> materials = scene->materials();

    const int32 maxCommandCount = geometries.size();

    std::vector<DrawCommand> ret;
    ret.reserve(maxCommandCount);

    for (int i = 0; i < maxCommandCount; i++) {
        Geometry *geometry = geometries[i];

        std::pair<GLBuffer *, GLBuffer *> gpuBuffers =
                m_bufferManager.buffersForGeometry(geometry);

        //FIXME Handle other passes
        const uint32 shaderProgramId = m_shaderManager.shaderIdForShaderProgram(materials[i]->renderPasses()[0]->shaderProgram());

        const DrawCommand cmd {
            shaderProgramId, m_vaos[i],
            *geometry, *gpuBuffers.first, gpuBuffers.second
        };

        ret.emplace_back(cmd);
    }

    return ret;
}
