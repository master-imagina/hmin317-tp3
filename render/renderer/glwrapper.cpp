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

#include "managers/texturemanager.h"

#include "render/geometry/vertexlayout.h"

#include "render/material/shaderparam.h"
#include "render/material/shaderprogram.h"
#include "render/material/texture.h"

#include "drawcommand.h"


using namespace std::literals;


////////////////////// Helpers //////////////////////

namespace {

void assertUniformFound(uint32 location, const char *name)
{
    C_ASSERT (location != -1, "Required uniform "s + name + " not found in shader");
}

void checkUniformFound(uint32 location, const char *name)
{
    if (location == -1) {
        std::cout << "Required uniform " << name << " not found in shader" << std::endl;
    }
}

} // anon namespace


////////////////////// GLWrapper //////////////////////

GLWrapper::GLWrapper()
{}

GLWrapper::~GLWrapper()
{}

bool GLWrapper::isInitialized() const
{
    return m_gl;
}

void GLWrapper::initialize(QOpenGLContext *glContext)
{
    m_gl = glContext->versionFunctions<OpenGLFuncs>();

    const bool initialized = m_gl->initializeOpenGLFunctions();
    Q_ASSERT_X (initialized,
                "GLWrapper::initialize()", "OpenGL 3.3 failed to initialize");
}

void GLWrapper::createVao(GLVao &glVao)
{
    m_gl->glGenVertexArrays(1, &glVao.id);
}

void GLWrapper::destroyVao(const GLVao &glVao)
{
    m_gl->glDeleteVertexArrays(1, &glVao.id);
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
    bindBuffer(buffer);

    // Orphan allocation
    m_gl->glBufferData(buffer.type, size, nullptr, buffer.usage);
    m_gl->glBufferData(buffer.type, size, data, buffer.usage);

    releaseBuffer(buffer);
}

void GLWrapper::createShaderProgram(GLShaderProgram &glProgram, const ShaderProgram &program)
{
    glProgram.id = m_gl->glCreateProgram();

    uint32 shaderId = 0;

    if (!program.vertexShaderSource.empty()) {
        shaderId = m_gl->glCreateShader(GL_VERTEX_SHADER);
        compileShader(glProgram, shaderId, program.vertexShaderSource);
    }

    if (!program.geometryShaderSource.empty()) {
        shaderId = m_gl->glCreateShader(GL_GEOMETRY_SHADER);
        compileShader(glProgram, shaderId, program.geometryShaderSource);
    }

    if (!program.fragmentShaderSource.empty()) {
        shaderId = m_gl->glCreateShader(GL_FRAGMENT_SHADER);
        compileShader(glProgram, shaderId, program.fragmentShaderSource);
    }

    linkShaderProgram(glProgram);
}

void GLWrapper::destroyShaderProgram(const GLShaderProgram &glProgram)
{
    m_gl->glDeleteProgram(glProgram.id);
}

void GLWrapper::bindShaderProgram(const GLShaderProgram &glProgram)
{
    m_gl->glUseProgram(glProgram.id);
}

void GLWrapper::releaseShaderProgram(const GLShaderProgram &glProgram)
{
    m_gl->glUseProgram(0);
}

void GLWrapper::sendUniforms(const GLShaderProgram &glProgram,
                             const std::vector<ShaderParam*> &params)
{
    bindShaderProgram(glProgram);

    // /!\ Assume a shader program is bound to the current context
    for (const ShaderParam *param : params) {
        const char *rawName = param->name.c_str();
        const QVariant value = param->value;
        const int valueType = value.userType();

        //FIXME ugly trick
        if (valueType == qMetaTypeId<Texture2D>()) {
            continue;
        }

        switch (valueType) {
        case QMetaType::Int:
            setUniform(glProgram.id, rawName, value.toInt());
            break;
        case QMetaType::Float:
            setUniform(glProgram.id, rawName, value.toFloat());
            break;
        case QMetaType::QVector2D:
            setUniform(glProgram.id, rawName, value.value<QVector2D>());
            break;
        case QMetaType::QVector3D:
            setUniform(glProgram.id, rawName, value.value<QVector3D>());
            break;
        case QMetaType::QVector4D:
            setUniform(glProgram.id, rawName, value.value<QVector4D>());
            break;
        case QMetaType::QColor:
            setUniform(glProgram.id, rawName, value.value<QColor>());
            break;
        case QMetaType::QMatrix4x4:
            setUniform(glProgram.id, rawName, value.value<QMatrix4x4>());
            break;
        default:
            std::cerr << "GLWrapper: unsupported uniform type :"
                      << QMetaType::typeName(valueType) << " "
                      << "(" << valueType << ")"
                      << std::endl;
            break;
        }
    }

    releaseShaderProgram(glProgram);
}

void GLWrapper::sendActiveCameraUniforms(const GLShaderProgram &glProgram,
                                         const QMatrix4x4 &worldMatrix,
                                         const QMatrix4x4 &viewMatrix,
                                         const QMatrix4x4 &projectionMatrix)
{
    bindShaderProgram(glProgram);

    // Send world matrix
    int location = m_gl->glGetUniformLocation(glProgram.id, "worldMatrix");

    if (location != -1) {
        m_gl->glUniformMatrix4fv(location, 1, false, worldMatrix.constData());
    }

    // Send view matrix
    location = m_gl->glGetUniformLocation(glProgram.id, "viewMatrix");

    if (location != -1) {
        m_gl->glUniformMatrix4fv(location, 1, false, viewMatrix.constData());
    }

    // Send projection matrix
    location = m_gl->glGetUniformLocation(glProgram.id, "projectionMatrix");

    if (location != -1) {
        m_gl->glUniformMatrix4fv(location, 1, false, projectionMatrix.constData());
    }

    releaseShaderProgram(glProgram);
}

void GLWrapper::sendTransformUniform(const GLShaderProgram &glProgram,
                                     const QMatrix4x4 &modelMatrix)
{
    bindShaderProgram(glProgram);

    int location = m_gl->glGetUniformLocation(glProgram.id, "modelMatrix");

    if (location != -1) {
        m_gl->glUniformMatrix4fv(location, 1, false, modelMatrix.constData());
    }

    releaseShaderProgram(glProgram);
}

void GLWrapper::sendTextureUniforms(const GLShaderProgram &glProgram,
                                    const std::vector<ShaderParam *> &textureParams,
                                    TextureManager &textureManager)
{
    bindShaderProgram(glProgram);

    for (int texUnit = 0; texUnit < textureParams.size(); texUnit++) {
        ShaderParam *param = textureParams[texUnit];
        auto texture = param->value.value<Texture2D>();

         GLTexture *glTexture = textureManager.get(texture);

        if (glTexture == 0) {
            glTexture = textureManager.addTexture(texture, *this);
        }

        C_ASSERT (glTexture,
                  "GLWrapper::sendTextureUniforms(): texture must exist");

        activeTexture2D(*glTexture, texUnit);

        setUniform(glProgram.id, param->name.c_str(), texUnit);
    }

    releaseShaderProgram(glProgram);
}

void GLWrapper::compileShader(const GLShaderProgram &glProgram,
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

    m_gl->glAttachShader(glProgram.id, shaderId);

    checkForErrors();
}

void GLWrapper::linkShaderProgram(const GLShaderProgram &glProgram)
{
    m_gl->glLinkProgram(glProgram.id);

    int linked = 0;
    m_gl->glGetProgramiv(glProgram.id, GL_LINK_STATUS, &linked);

    if (!linked) {
        int logSize = 0;
        m_gl->glGetProgramiv(glProgram.id, GL_INFO_LOG_LENGTH, &logSize);

        if (logSize > 1) {  // ignore the null termination character
            std::vector<char> logBuffer(logSize);

            m_gl->glGetProgramInfoLog(glProgram.id, logSize, nullptr,
                                      logBuffer.data());

            std::cerr << "Shader linking error :" << std::endl
                      << std::string(logBuffer.data())
                      << std::endl;
        }
    }

    checkForErrors();
}

std::vector<std::string> GLWrapper::activeUniforms(const GLShaderProgram &glProgram) const
{
    std::vector<std::string> ret;
    ret.reserve(16);

    int count = 0;
    m_gl->glGetProgramiv(glProgram.id, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; i++) {
        std::array<char, 128> name;

        m_gl->glGetActiveUniformName(glProgram.id, i, sizeof(name), nullptr, name.data());

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

void GLWrapper::bindVao(const GLVao &glVao)
{
    m_gl->glBindVertexArray(glVao.id);

}

void GLWrapper::releaseVao(const GLVao &glVao)
{
    m_gl->glBindVertexArray(0);
}

void GLWrapper::setupVaoForBufferAndShader(const GLShaderProgram &glProgram,
                                           const GLVao &glVao,
                                           const VertexLayout &vertexLayout,
                                           GLBuffer &arrayBuffer,
                                           GLBuffer *indexBuffer)
{
    bindShaderProgram(glProgram);
    bindVao(glVao);

    bindBuffer(arrayBuffer);

    for (const VertexAttrib &attrib : vertexLayout.attributes()) {
        const int location =
                m_gl->glGetAttribLocation(glProgram.id, attrib.name.c_str());

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

    releaseVao(glVao);
    releaseShaderProgram(glProgram);

    if (indexBuffer) {
        releaseBuffer(*indexBuffer);
    }

    checkForErrors();
}

void GLWrapper::createTexture2D(GLTexture &texture)
{
    uint32 ret = 0;

    m_gl->glGenTextures(1, &texture.id);
}

void GLWrapper::destroyTexture2D(GLTexture &texture)
{
    m_gl->glDeleteTextures(1, &texture.id);
}

void GLWrapper::allocateTexture2D(const GLTexture &texture,
                                  const GLTexture::Params &params,
                                  const ubyte *data)
{
    bindTexture2D(texture);

    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);

    m_gl->glTexImage2D(GL_TEXTURE_2D, 0, params.format,
                       params.width, params.height, 0, params.format,
                       GL_UNSIGNED_BYTE, data);

    if (params.genMipMap) {
        m_gl->glGenerateMipmap(GL_TEXTURE_2D);
    }

    releaseTexture2D();
}

void GLWrapper::bindTexture2D(const GLTexture &texture)
{
    m_gl->glBindTexture(GL_TEXTURE_2D, texture.id);
}

void GLWrapper::activeTexture2D(const GLTexture &texture, int i)
{
    const int maxTextureUnitsCount = maxTextureUnits();

    C_ASSERT (i >= 0 && i <= maxTextureUnitsCount,
              "GLWrapper: supports only up to "s +
              std::to_string(maxTextureUnitsCount) + " active texture points");

    m_gl->glActiveTexture(GL_TEXTURE0 + i);
    bindTexture2D(texture);
//    releaseTexture2D();
}

void GLWrapper::releaseTexture2D()
{
    m_gl->glBindTexture(GL_TEXTURE_2D, 0);
}

int GLWrapper::maxTextureUnits()
{
    int ret = 0;

    m_gl->glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &ret);

    return ret;
}

void GLWrapper::draw(const std::vector<DrawCommand> &commands)
{
    for (const DrawCommand &cmd : commands) {
        bindShaderProgram(*cmd.glProgram);
        bindVao(*cmd.glVao);

        if (cmd.glIndexBuffer) {
            m_gl->glDrawElements(cmd.geometry.primitiveType,
                                 cmd.geometry.primitiveCount,
                                 GL_UNSIGNED_INT,
                                 nullptr);
        }
        else {
            m_gl->glDrawArrays(cmd.geometry.primitiveType, 0,
                               cmd.geometry.primitiveCount);
        }

        releaseVao(*cmd.glVao);
        releaseShaderProgram(*cmd.glProgram);
    }
}

void GLWrapper::setUniform(uint32 programId, const char *name, int value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    checkUniformFound(location, name);

    m_gl->glUniform1i(location, value);
}

void GLWrapper::setUniform(uint32 programId, const char *name, float value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    checkUniformFound(location, name);

    m_gl->glUniform1fv(location, 1, &value);
}

void GLWrapper::setUniform(uint32 programId, const char *name, const QVector2D &value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    checkUniformFound(location, name);

    m_gl->glUniform2fv(location, 1, reinterpret_cast<const float *>(&value));
}

void GLWrapper::setUniform(uint32 programId, const char *name, const QVector3D &value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    checkUniformFound(location, name);

    m_gl->glUniform3fv(location, 1, reinterpret_cast<const float *>(&value));
}

void GLWrapper::setUniform(uint32 programId, const char *name, const QVector4D &value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    checkUniformFound(location, name);

    m_gl->glUniform4fv(location, 1, reinterpret_cast<const float *>(&value));
}

void GLWrapper::setUniform(uint32 programId, const char *name, const QColor &value)
{
    const int location = m_gl->glGetUniformLocation(programId, name);
    checkUniformFound(location, name);

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
    checkUniformFound(location, name);

    m_gl->glUniformMatrix4fv(location, 1, false, value.constData());
}
