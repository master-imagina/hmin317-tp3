#include "glwrapper.h"

#include <assert.h>
#include <iostream>
#include <string>

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QVariant>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

#include "core/log.h"

#include "../geometry/vertexlayout.h"

#include "../material/shaderparam.h"
#include "../material/shaderprogram.h"

#include "drawcommand.h"


using namespace std::literals;


////////////////////// Helpers //////////////////////

namespace {

void assertUniformFound(uint32 location, const char *name)
{
    C_ASSERT (location != -1, "Required Uniform "s + name + " not found in shader");
}

} // anon namespace


////////////////////// GLWrapper //////////////////////

GLWrapper::GLWrapper()
{}

GLWrapper::~GLWrapper()
{}

void GLWrapper::initialize(QOpenGLContext *glContext)
{
    m_gl = glContext->versionFunctions<OpenGLFuncs>();

    const bool initialized = m_gl->initializeOpenGLFunctions();
    Q_ASSERT_X (initialized,
                "GLWrapper::initialize()", "OpenGL 3.3 failed to initialize");
}

uint32 GLWrapper::createVao()
{
    uint32 ret = 0;

    m_gl->glGenVertexArrays(1, &ret);

    assert (ret > 0);

    return ret;
}

uint32 GLWrapper::destroyVao(uint32 &vaoId)
{
    m_gl->glDeleteVertexArrays(1, &vaoId);
}

void GLWrapper::createBuffer(GLBuffer &buffer,
                             GLBuffer::Type type,
                             GLBuffer::Usage usage)
{
    m_gl->glGenBuffers(1, &buffer.id);

    buffer.type = type;
    buffer.usage = usage;

    Q_ASSERT(buffer.id > 0);
}

void GLWrapper::destroyBuffer(GLBuffer &buffer)
{
    m_gl->glDeleteBuffers(1, &buffer.id);
}

void GLWrapper::bindBuffer(const GLBuffer &buffer)
{
    m_gl->glBindBuffer(buffer.type, buffer.id);
}

void GLWrapper::releaseBuffer(const GLBuffer &buffer)
{
    m_gl->glBindBuffer(buffer.type, 0);
}

void GLWrapper::allocateBuffer(const GLBuffer &buffer,
                               uint32 size, const void *data)
{
    // Orphan allocation
    m_gl->glBufferData(buffer.type, size, nullptr, buffer.usage);
    m_gl->glBufferData(buffer.type, size, data, buffer.usage);
}

uint32 GLWrapper::buildShaderProgram(const ShaderProgram *program)
{
    const uint32 programId = m_gl->glCreateProgram();
    uint32 shaderId = 0;

    if (!program->vertexShaderSource.empty()) {
        shaderId = m_gl->glCreateShader(GL_VERTEX_SHADER);
        compileShader(programId, shaderId, program->vertexShaderSource);
    }

    if (!program->geometryShaderSource.empty()) {
        shaderId = m_gl->glCreateShader(GL_GEOMETRY_SHADER);
        compileShader(programId, shaderId, program->geometryShaderSource);
    }

    if (!program->fragmentShaderSource.empty()) {
        shaderId = m_gl->glCreateShader(GL_FRAGMENT_SHADER);
        compileShader(programId, shaderId, program->fragmentShaderSource);
    }

    linkShaderProgram(programId);

    return programId;
}

void GLWrapper::bindShaderProgram(uint32 programId)
{
    m_gl->glUseProgram(programId);
}

void GLWrapper::releaseShaderProgram(uint32 programId)
{
    m_gl->glUseProgram(0);
}

void GLWrapper::destroyShaderProgram(uint32 programId)
{
    m_gl->glDeleteProgram(programId);
}

void GLWrapper::sendUniforms(uint32 programId,
                             const std::vector<ShaderParam*> &params)
{
    // /!\ Assume a shader program is bound to the current context
    for (const ShaderParam *param : params) {
        const char *rawName = param->name.c_str();
        const QVariant value = param->value;
        const int valueType = value.type();

        switch (valueType) {
        case QMetaType::Int:
            setUniform(programId, rawName, value.toInt());
            break;
        case QMetaType::Float:
            setUniform(programId, rawName, value.toFloat());
            break;
        case QMetaType::QVector2D:
            setUniform(programId, rawName, value.value<QVector2D>());
            break;
        case QMetaType::QVector3D:
            setUniform(programId, rawName, value.value<QVector3D>());
            break;
        case QMetaType::QVector4D:
            setUniform(programId, rawName, value.value<QVector4D>());
            break;
        case QMetaType::QColor:
            setUniform(programId, rawName, value.value<QColor>());
            break;
        case QMetaType::QMatrix4x4:
            setUniform(programId, rawName, value.value<QMatrix4x4>());
            break;
        default:
            std::cerr << "GLWrapper: unsupported uniform type :"
                      << QMetaType::typeName(valueType)
                      << std::endl;
            break;
        }
    }
}

void GLWrapper::sendActiveCameraUniforms(uint32 programId,
                                         const QMatrix4x4 &worldMatrix,
                                         const QMatrix4x4 &viewMatrix,
                                         const QMatrix4x4 &projectionMatrix)
{
    // Send world matrix
    int location = m_gl->glGetUniformLocation(programId, "worldMatrix");

    if (location != -1) {
        m_gl->glUniformMatrix4fv(location, 1, false, worldMatrix.constData());
    }

    // Send view matrix
    location = m_gl->glGetUniformLocation(programId, "viewMatrix");

    if (location != -1) {
        m_gl->glUniformMatrix4fv(location, 1, false, viewMatrix.constData());
    }

    // Send projection matrix
    location = m_gl->glGetUniformLocation(programId, "projectionMatrix");

    if (location != -1) {
        m_gl->glUniformMatrix4fv(location, 1, false, projectionMatrix.constData());
    }
}

void GLWrapper::sendTransformUniform(uint32 programId,
                                     const QMatrix4x4 &modelMatrix)
{
    const int location = m_gl->glGetUniformLocation(programId, "modelMatrix");

    if (location != -1) {
        m_gl->glUniformMatrix4fv(location, 1, false, modelMatrix.constData());
    }
}

void GLWrapper::compileShader(uint32 programId,
                              uint32 shaderId,
                              const std::string &shaderSource)
{
    const char *rawSource = shaderSource.c_str();
    int sourceLength = shaderSource.size();
    m_gl->glShaderSource(shaderId, 1, &rawSource, &sourceLength);
    m_gl->glCompileShader(shaderId);

    int compiled = 0;
    m_gl->glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        int logSize = 0;
        m_gl->glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

        if (logSize > 1) {  // ignore the null termination character
            std::vector<char> logBuffer(logSize);

            m_gl->glGetShaderInfoLog(shaderId, logSize, nullptr,
                                     logBuffer.data());

            std::cerr << "Shader compilation error :" << std::endl
                      << std::string(logBuffer.data()) << "in :" << std::endl
                      << "--------------------------" << std::endl
                      << shaderSource
                      << std::endl;
        }
    }

    m_gl->glAttachShader(programId, shaderId);

    checkForErrors();
}

void GLWrapper::linkShaderProgram(uint32 programId)
{
    m_gl->glLinkProgram(programId);

    int linked = 0;
    m_gl->glGetProgramiv(programId, GL_LINK_STATUS, &linked);

    if (!linked) {
        int logSize = 0;
        m_gl->glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);

        if (logSize > 1) {  // ignore the null termination character
            std::vector<char> logBuffer(logSize);

            m_gl->glGetProgramInfoLog(programId, logSize, nullptr,
                                      logBuffer.data());

            std::cerr << "Shader linking error :" << std::endl
                      << std::string(logBuffer.data())
                      << std::endl;
        }
    }

    checkForErrors();
}

std::vector<std::string> GLWrapper::activeUniforms(uint32 programId) const
{
    std::vector<std::string> ret;
    ret.reserve(16);

    int count = 0;
    m_gl->glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; i++) {
        std::array<char, 128> name;

        m_gl->glGetActiveUniformName(programId, i, sizeof(name), nullptr, name.data());

        ret.push_back(std::string(name.data()));
    }

    return ret;
}

void GLWrapper::checkForErrors()
{
    const GLenum err = m_gl->glGetError();

    if (err != 0) {
        std::cerr << "OpenGL error: " << err << std::endl;
        assert (false);
    }
}

void GLWrapper::setupVaoForBufferAndShader(GLuint programId,
                                           GLuint vaoId,
                                           const VertexLayout &vertexLayout,
                                           GLBuffer &arrayBuffer,
                                           GLBuffer *indexBuffer)
{
    m_gl->glUseProgram(programId);
    m_gl->glBindVertexArray(vaoId);

    bindBuffer(arrayBuffer);

    for (const VertexAttrib &attrib : vertexLayout.attributes()) {
        const int location =
                m_gl->glGetAttribLocation(programId, attrib.name.c_str());

        m_gl->glEnableVertexAttribArray(location);
        m_gl->glVertexAttribPointer(location,
                                    attrib.size,
                                    static_cast<uint32>(attrib.type),
                                    attrib.normalized, attrib.stride,
                                    nullptr);
    }

    if (indexBuffer) {
        bindBuffer(*indexBuffer);
    }

    releaseBuffer(arrayBuffer);

    m_gl->glBindVertexArray(0);
    m_gl->glUseProgram(0);

    if (indexBuffer) {
        releaseBuffer(*indexBuffer);
    }

    checkForErrors();
}

uint32 GLWrapper::uniformBlockIndex(uint32 programId, const char *name)
{
    return m_gl->glGetUniformBlockIndex(programId, name);
}

void GLWrapper::setUniformBlockForUBO(uint32 programId,
                                      uint32 blockIndex,
                                      uint32 bindingPoint)
{
    m_gl->glUniformBlockBinding(programId, blockIndex, bindingPoint);
}

void GLWrapper::draw(const std::vector<DrawCommand> &commands)
{
    for (const DrawCommand &cmd : commands) {
        m_gl->glUseProgram(cmd.shaderProgramId);
        m_gl->glBindVertexArray(cmd.vaoId);

        if (cmd.indexGLBuffer) {
            m_gl->glDrawElements(cmd.geometry.primitiveType,
                                 cmd.geometry.primitiveCount,
                                 GL_UNSIGNED_INT,
                                 nullptr);
        }
        else {
            m_gl->glDrawArrays(cmd.geometry.primitiveType, 0,
                               cmd.geometry.primitiveCount);
        }

        m_gl->glBindVertexArray(0);
        m_gl->glUseProgram(0);
    }
}

void GLWrapper::setUniform(uint32 programId, const char *name, int value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    assertUniformFound(location, name);

    m_gl->glUniform1i(location, value);
}

void GLWrapper::setUniform(uint32 programId, const char *name, float value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    assertUniformFound(location, name);

    m_gl->glUniform1fv(location, 1, &value);
}

void GLWrapper::setUniform(uint32 programId, const char *name, const QVector2D &value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    assertUniformFound(location, name);

    m_gl->glUniform2fv(location, 1, reinterpret_cast<const float *>(&value));
}

void GLWrapper::setUniform(uint32 programId, const char *name, const QVector3D &value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    assertUniformFound(location, name);

    m_gl->glUniform3fv(location, 1, reinterpret_cast<const float *>(&value));
}

void GLWrapper::setUniform(uint32 programId, const char *name, const QVector4D &value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    assertUniformFound(location, name);

    m_gl->glUniform4fv(location, 1, reinterpret_cast<const float *>(&value));
}

void GLWrapper::setUniform(uint32 programId, const char *name, const QColor &value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    assertUniformFound(location, name);

    const float rawValue[4] {
        static_cast<float>(value.redF()),
                static_cast<float>(value.greenF()),
                static_cast<float>(value.blueF()),
                static_cast<float>(value.alphaF())
    };

    m_gl->glUniform4fv(location, 1, rawValue);
}

void GLWrapper::setUniform(uint32 programId, const char *name, const QMatrix4x4 &value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    assertUniformFound(location, name);

    m_gl->glUniformMatrix4fv(location, 1, false, value.constData());
}
