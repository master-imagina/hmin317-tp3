#include "renderer.h"

#include <QColor>

#include "geometry.h"
#include "particleeffect.h"

#include "renderer/geometrydesc.h"
#include "renderer/vertexattrib.h"


Renderer::Renderer() :
    gl(nullptr),
    m_vaos(),
    m_terrainShaderProgram(nullptr),
    m_particlesShaderProgram(nullptr),
    m_terrainArrayVbo(Buffer::Type::ArrayBuffer),
    m_terrainIndexVbo(Buffer::Type::IndexBuffer),
    m_particlesVbo(Buffer::Type::ArrayBuffer),
    m_isDirty(false),
    m_shaderPrograms()
{}

Renderer::~Renderer()
{
    cleanup();
}

void Renderer::setupVaoForBufferAndShader(GLuint vao,
                                          const GeometryDesc &geometryDesc,
                                          Buffer &arrayBuffer,
                                          const QOpenGLShaderProgram &program,
                                          Buffer *indexBuffer)
{
    gl->glUseProgram(program.programId());
    gl->glBindVertexArray(vao);

    arrayBuffer.bind();

    const std::vector<VertexAttrib> &attributes = geometryDesc.attributes();

    for (const VertexAttrib &attrib : attributes) {
        const int location = program.attributeLocation(attrib.name);

        gl->glEnableVertexAttribArray(location);
        gl->glVertexAttribPointer(location,
                                  attrib.size,
                                  static_cast<unsigned int>(attrib.type),
                                  attrib.normalized, attrib.stride,
                                  nullptr);
    }

    if (indexBuffer) {
        indexBuffer->bind();
    }

    arrayBuffer.release();

    gl->glBindVertexArray(0);
    gl->glUseProgram(0);

    if (indexBuffer) {
        indexBuffer->release();
    }
}

void Renderer::initialize()
{
    gl = QOpenGLContext::currentContext()->versionFunctions<OpenGLFuncs>();
    const bool initialized = gl->initializeOpenGLFunctions();
    Q_ASSERT_X (initialized,
                "Renderer::initialize()", "OpenGL 3.3 failed to initialize");

    gl->glClearColor(0.f, 0.f, 0.f, 1.f);

    gl->glEnable(GL_DEPTH_TEST);
    gl->glDepthFunc(GL_LESS);

    //    gl->glEnable(GL_BLEND);
    //    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create VBOs
    m_terrainArrayVbo.create();
    m_terrainIndexVbo.create();

    m_particlesVbo.create();

    // Create shaders and vaos
    m_terrainShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                   "://res/shaders/geom_textured.vert");

    m_terrainShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                   "://res/shaders/terrain_heightmap.frag");

    m_particlesShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                     "://res/shaders/particle.vert");
    m_particlesShaderProgram.addShaderFromSourceFile(QOpenGLShader::Geometry,
                                                     "://res/shaders/particle.geom");
    m_particlesShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                     "://res/shaders/particle.frag");

    m_shaderPrograms.push_back(&m_terrainShaderProgram);
    m_shaderPrograms.push_back(&m_particlesShaderProgram);

    m_terrainShaderProgram.link();
    m_particlesShaderProgram.link();

    gl->glGenVertexArrays(2, m_vaos.data());

    // Setup terrain geometry
    GeometryDesc terrainGeomDesc;
    VertexAttrib terrainGeomVertexAttrib {"vertexPos", 3, VertexAttrib::Type::Float, false, 0};
    terrainGeomDesc.addAttribute(terrainGeomVertexAttrib);

    setupVaoForBufferAndShader(m_vaos[0], terrainGeomDesc,
                               m_terrainArrayVbo, m_terrainShaderProgram,
                               &m_terrainIndexVbo);

    // Setup particles geometry
    GeometryDesc particlesGeomDesc;
    VertexAttrib particlesGeomVertexAttrib {"particleWorldPos", 3, VertexAttrib::Type::Float, false, 0};
    particlesGeomDesc.addAttribute(particlesGeomVertexAttrib);

    setupVaoForBufferAndShader(m_vaos[1], particlesGeomDesc,
                               m_particlesVbo, m_particlesShaderProgram);


    cglPrintAnyError();
}

bool Renderer::isDirty() const
{
    return m_isDirty;
}

void Renderer::setDirty()
{
    m_isDirty = true;
}

void Renderer::unsetDirty()
{
    m_isDirty = false;
}

void Renderer::updateBuffers(Geometry *geom, ParticleEffect *particleEffect)
{
    // Update geometry buffers
    const std::vector<QVector3D> &vertices = geom->vertices;
    const std::vector<unsigned int> &indices = geom->indices;

    m_terrainArrayVbo.bind();
    m_terrainArrayVbo.allocate(vertices.size() * sizeof(QVector3D),
                                vertices.data(),
                                Buffer::Usage::StaticDraw);

    m_terrainIndexVbo.bind();
    m_terrainIndexVbo.allocate(indices.size() * sizeof(unsigned int),
                               indices.data(),
                               Buffer::Usage::StaticDraw);
    m_terrainIndexVbo.release();

    // Update particles buffers
    const std::vector<QVector3D> &particles = particleEffect->worldPositions();

    m_particlesVbo.bind();
    m_particlesVbo.allocate(particles.size() * sizeof(QVector3D),
                            particles.data(),
                            Buffer::Usage::StreamDraw);

    m_particlesVbo.release();

    cglPrintAnyError();
}

void Renderer::updateUniforms(const QVariantMap &uniforms)
{
    for (QOpenGLShaderProgram *shaderProgram : m_shaderPrograms) {
        shaderProgram->bind();

        for (auto it = uniforms.begin(); it != uniforms.end(); it++) {
            const QString uniformName = it.key();
            const QVariant uniformValue = it.value();

            sendVariantUniform(*shaderProgram, uniformName, uniformValue);
        }

        shaderProgram->release();
    }
}

void Renderer::render(float dt)
{
    Q_UNUSED (dt);

    cglPrintAnyError();

    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw terrain
    m_terrainShaderProgram.bind();
    gl->glBindVertexArray(m_vaos[0]);

    gl->glDrawElements(GL_TRIANGLES, m_terrainIndexVbo.size() / 4, GL_UNSIGNED_INT, 0);

    // Draw particles
    m_particlesShaderProgram.bind();
    gl->glBindVertexArray(m_vaos[1]);

    gl->glDrawArrays(GL_POINTS, 0, m_particlesVbo.size() / sizeof(QVector3D));


    gl->glBindVertexArray(0);
}

void Renderer::cleanup()
{
    gl->glDeleteVertexArrays(2, m_vaos.data());

    m_terrainArrayVbo.destroy();
    m_terrainIndexVbo.destroy();

    m_particlesVbo.destroy();

    cglPrintAnyError();
}

void Renderer::sendVariantUniform(QOpenGLShaderProgram &program,
                                  const QString &name, const QVariant &value)
{
    const char *rawName = name.toLatin1().constData();
    const int valueType = value.type();

    switch (valueType) {
    case QMetaType::Int:
        program.setUniformValue(rawName, value.toInt());
        break;
    case QMetaType::Float:
        program.setUniformValue(rawName, value.toFloat());
        break;
    case QMetaType::QVector2D:
        program.setUniformValue(rawName, value.value<QVector2D>());
        break;
    case QMetaType::QVector3D:
        program.setUniformValue(rawName, value.value<QVector3D>());
        break;
    case QMetaType::QVector4D:
        program.setUniformValue(rawName, value.value<QVector4D>());
        break;
    case QMetaType::QColor:
        program.setUniformValue(rawName, value.value<QColor>());
        break;
    case QMetaType::QMatrix4x4:
        program.setUniformValue(rawName, value.value<QMatrix4x4>());
        break;
    default:
        qCritical() << "Renderer: unsupported uniform type :"
                    << QMetaType::typeName(valueType);
        break;
    }
}

void Renderer::cglPrintAnyError()
{
    const GLenum err = gl->glGetError();

    if (err != 0) {
        qCritical() << err;
    }
}
