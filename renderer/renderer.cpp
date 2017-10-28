#include "renderer.h"

#include <QColor>

#include "drawcommand.h"
#include "vertexattrib.h"
#include "vertexlayout.h"

#include "../geometry.h"
#include "../scene.h"


Renderer::Renderer() :
    m_bufferManager(),
    gl(nullptr),
    m_glWrapper(),
    m_shaderPrograms(),
    m_vaos(),
    m_arrayVbos(),
    m_indexVbos()
{}

Renderer::~Renderer()
{
    cleanup();
}

void Renderer::initialize()
{
    QOpenGLContext *currentGLContext = QOpenGLContext::currentContext();

    gl = currentGLContext->versionFunctions<OpenGLFuncs>();
    const bool initialized = gl->initializeOpenGLFunctions();
    Q_ASSERT_X (initialized,
                "Renderer::initialize()", "OpenGL 3.3 failed to initialize");

    m_glWrapper.initialize(currentGLContext);

    gl->glClearColor(0.f, 0.f, 0.f, 1.f);

    gl->glEnable(GL_DEPTH_TEST);
    gl->glDepthFunc(GL_LESS);

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
    m_shaderPrograms[0].addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                "://res/shaders/terrain_heightmap.vert");

    m_shaderPrograms[0].addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                "://res/shaders/terrain_heightmap.frag");

    m_shaderPrograms[1].addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                "://res/shaders/particle.vert");
    m_shaderPrograms[1].addShaderFromSourceFile(QOpenGLShader::Geometry,
                                                "://res/shaders/particle.geom");
    m_shaderPrograms[1].addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                "://res/shaders/particle.frag");

    m_shaderPrograms[0].link();
    m_shaderPrograms[1].link();

    gl->glGenVertexArrays(2, m_vaos.data());

    // Setup terrain geometry
    VertexLayout terrainVertexLayout;
    VertexAttrib terrainVertexAttrib {"vertexPos", 3, VertexAttrib::Type::Float, false, 0};
    terrainVertexLayout.addAttribute(terrainVertexAttrib);

    m_glWrapper.setupVaoForBufferAndShader(m_shaderPrograms[0].programId(), m_vaos[0],
                                           terrainVertexLayout,
                                           m_arrayVbos[0],
                                           &m_indexVbos[0]);

    // Setup particles geometry
    VertexLayout particlesVertexLayout;
    VertexAttrib particlesVertexAttrib {"particleWorldPos", 3, VertexAttrib::Type::Float, false, 0};
    particlesVertexLayout.addAttribute(particlesVertexAttrib);

    m_glWrapper.setupVaoForBufferAndShader(m_shaderPrograms[1].programId(), m_vaos[1],
                                           particlesVertexLayout,
                                           m_arrayVbos[1]);


    cglPrintAnyError();
}

void Renderer::updateBuffers(const std::vector<Geometry *> &geoms)
{
    for (Geometry *geom : geoms) {
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
            const std::vector<unsigned int> &indices = geom->indices;

            m_glWrapper.bindBuffer(*indexGLBuffer);
            m_glWrapper.allocateBuffer(*indexGLBuffer,
                                         indices.size() * Geometry::indexSize,
                                         indices.data());
            m_glWrapper.releaseBuffer(*indexGLBuffer);
        }

//        geom->isDirty = false;
    }

    cglPrintAnyError();
}

void Renderer::updateUniforms(const QVariantMap &uniforms)
{
    for (QOpenGLShaderProgram &shaderProgram : m_shaderPrograms) {
        shaderProgram.bind();

        for (auto it = uniforms.begin(); it != uniforms.end(); it++) {
            const QString uniformName = it.key();
            const QVariant uniformValue = it.value();

            const char *rawName = uniformName.toLatin1().constData();
            const int valueType = uniformValue.type();

            switch (valueType) {
            case QMetaType::Int:
                shaderProgram.setUniformValue(rawName, uniformValue.toInt());
                break;
            case QMetaType::Float:
                shaderProgram.setUniformValue(rawName, uniformValue.toFloat());
                break;
            case QMetaType::QVector2D:
                shaderProgram.setUniformValue(rawName, uniformValue.value<QVector2D>());
                break;
            case QMetaType::QVector3D:
                shaderProgram.setUniformValue(rawName, uniformValue.value<QVector3D>());
                break;
            case QMetaType::QVector4D:
                shaderProgram.setUniformValue(rawName, uniformValue.value<QVector4D>());
                break;
            case QMetaType::QColor:
                shaderProgram.setUniformValue(rawName, uniformValue.value<QColor>());
                break;
            case QMetaType::QMatrix4x4:
                shaderProgram.setUniformValue(rawName, uniformValue.value<QMatrix4x4>());
                break;
            default:
                qCritical() << "Renderer: unsupported uniform type :"
                            << QMetaType::typeName(valueType);
                break;
            }
        }

        shaderProgram.release();
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
            m_shaderPrograms[i].programId(), m_vaos[i],
            *geometry, *gpuBuffers.first, gpuBuffers.second
        };

        ret.push_back(cmd);
    }

    return ret;
}

void Renderer::render(Scene *scene, float dt)
{
    Q_UNUSED (dt);

    m_bufferManager.addGeometry(scene->geometries[0], &m_arrayVbos[0], &m_indexVbos[0]);
    m_bufferManager.addGeometry(scene->geometries[1], &m_arrayVbos[1], nullptr);

    std::vector<Geometry *> dirtyGeoms;
    dirtyGeoms.reserve(scene->geometries.size());
    for (Geometry *geom : scene->geometries) {
        if (geom->isDirty) {
            dirtyGeoms.push_back(geom);
        }
    }
    updateBuffers(dirtyGeoms);

    const std::vector<DrawCommand> commands = prepareDrawCommands(scene);

    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_glWrapper.draw(commands);

    cglPrintAnyError();
}

void Renderer::cleanup()
{
    gl->glDeleteVertexArrays(2, m_vaos.data());

    m_glWrapper.destroyBuffer(m_arrayVbos[0]);
    m_glWrapper.destroyBuffer(m_arrayVbos[1]);
    m_glWrapper.destroyBuffer(m_indexVbos[0]);

    cglPrintAnyError();
}

void Renderer::cglPrintAnyError()
{
    const GLenum err = gl->glGetError();

    if (err != 0) {
        qCritical() << "OpenGL error:" << err;
    }
}
