#include "renderer.h"

#include <QColor>

#include "geometry.h"


Renderer::Renderer() :
    m_vao(nullptr),
    m_shaderProgram(nullptr),
    m_vertexVbo(0),
    m_indexVbo(0)
{}

Renderer::~Renderer()
{
    cleanup();
}

void Renderer::initialize()
{
    Q_ASSERT_X (initializeOpenGLFunctions(),
                "Renderer::initialize()", "OpenGL 3.3 failed to initialize");

    glClearColor(0.f, 0.f, 0.f, 1.f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // VBO
    glGenBuffers(1, &m_vertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVbo);
    glGenBuffers(1, &m_indexVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVbo);

    // Shader and vao
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                            "://res/shaders/geom_textured.vert");

    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                            ":/res/shaders/terrain_heightmap.frag");

    m_vao.create();
    m_vao.bind();

    m_shaderProgram.enableAttributeArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVbo);
    m_shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVbo);

    m_shaderProgram.link();

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

void Renderer::updateBuffers(Geometry *geom)
{
    const std::vector<VertexData> &vertices = geom->vertices;
    const std::vector<unsigned int> &indices = geom->indices;

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    cglPrintAnyError();
}

void Renderer::updateUniforms(const QVariantMap &uniforms)
{
    m_shaderProgram.bind();

    for (auto it = uniforms.begin(); it != uniforms.end(); it++) {
        const QString uniformName = it.key();
        const QVariant uniformValue = it.value();

        sendVariantUniform(m_shaderProgram, uniformName, uniformValue);
    }

    m_shaderProgram.release();
}

void Renderer::render()
{
    cglPrintAnyError();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shaderProgram.bind();
    m_vao.bind();

    int sizeVertexVbo = 0;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVbo);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &sizeVertexVbo);

    glDrawElements(GL_TRIANGLES, sizeVertexVbo / 4, GL_UNSIGNED_INT, 0);
}

void Renderer::cleanup()
{
    if (m_vao.isCreated()) {
        m_vao.destroy();
    }

    glDeleteBuffers(1, &m_vertexVbo);
    glDeleteBuffers(1, &m_indexVbo);
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
    const GLenum err = glGetError();

    if (err != 0) {
        qCritical() << err;
    }
}
