#include "renderer.h"

#include <QColor>

#include "geometry.h"
#include "particleeffect.h"


Renderer::Renderer() :
    gl(nullptr),
    m_vaos(),
    m_terrainShaderProgram(nullptr),
    m_particlesShaderProgram(nullptr),
    m_particlesVbo(0),
    m_vertexVbo(0),
    m_indexVbo(0),
    m_shaderPrograms()
{}

Renderer::~Renderer()
{
    cleanup();
}

void Renderer::initialize()
{
    gl = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    const bool initialized = gl->initializeOpenGLFunctions();
    Q_ASSERT_X (initialized,
                "Renderer::initialize()", "OpenGL 3.3 failed to initialize");

    gl->glClearColor(0.f, 0.f, 0.f, 1.f);

    gl->glEnable(GL_DEPTH_TEST);
    gl->glDepthFunc(GL_LESS);

//    gl->glEnable(GL_BLEND);
//    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // VBO
    gl->glGenBuffers(1, &m_vertexVbo);
    gl->glBindBuffer(GL_ARRAY_BUFFER, m_vertexVbo);
    gl->glGenBuffers(1, &m_indexVbo);
    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVbo);

    gl->glGenBuffers(1, &m_particlesVbo);
    gl->glBindBuffer(GL_ARRAY_BUFFER, m_particlesVbo);

    // Shader and vao
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

    // Terrain shader
    m_terrainShaderProgram.bind();
    gl->glBindVertexArray(m_vaos[0]);

    gl->glBindBuffer(GL_ARRAY_BUFFER, m_vertexVbo);
    int location = m_terrainShaderProgram.attributeLocation("vertexPos");
    gl->glEnableVertexAttribArray(location);
    gl->glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVbo);

    // Particles shader
    m_particlesShaderProgram.bind();
    gl->glBindVertexArray(m_vaos[1]);

    gl->glBindBuffer(GL_ARRAY_BUFFER, m_particlesVbo);
    location = m_particlesShaderProgram.attributeLocation("particleWorldPos");
    gl->glEnableVertexAttribArray(location);
    gl->glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    cglPrintAnyError();

    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);

    gl->glBindVertexArray(0);
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

    gl->glBindBuffer(GL_ARRAY_BUFFER, m_vertexVbo);
    gl->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVector3D), vertices.data(), GL_STATIC_DRAW);

    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVbo);
    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Update particles buffers
    const std::vector<QVector3D> &particles = particleEffect->worldPositions();

    gl->glBindBuffer(GL_ARRAY_BUFFER, m_particlesVbo);
    gl->glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(QVector3D), particles.data(), GL_STREAM_DRAW);

    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);

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

void Renderer::render()
{
    cglPrintAnyError();

    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_terrainShaderProgram.bind();
    gl->glBindVertexArray(m_vaos[0]);

    // Draw terrain
    int sizeVertexVbo = 0;
    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVbo);
    gl->glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &sizeVertexVbo);

    gl->glDrawElements(GL_TRIANGLES, sizeVertexVbo / 4, GL_UNSIGNED_INT, 0);

    // Draw particles
    //TODO get rid of glGetBufferParameter
    m_particlesShaderProgram.bind();
    gl->glBindVertexArray(m_vaos[1]);

    int sizeParticlesVbo = 0;
    gl->glBindBuffer(GL_ARRAY_BUFFER, m_particlesVbo);
    gl->glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &sizeParticlesVbo);

    gl->glDrawArrays(GL_POINTS, 0, sizeParticlesVbo / sizeof(QVector3D));

    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);

    gl->glBindVertexArray(0);
}

void Renderer::cleanup()
{
    gl->glDeleteVertexArrays(2, m_vaos.data());

    gl->glDeleteBuffers(1, &m_vertexVbo);
    gl->glDeleteBuffers(1, &m_indexVbo);

    gl->glDeleteBuffers(1, &m_particlesVbo);
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
