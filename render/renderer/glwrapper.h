#ifndef GLWRAPPER_H
#define GLWRAPPER_H

#include "core/aliases_int.h"
#include "core/aliases_memory.h"

#include "glbuffer.h"
#include "gltexture.h"
#include "glshaderprogram.h"
#include "glvao.h"
#include "openglversion.h"

class DrawCommand;
class ShaderParam;
class ShaderProgram;
class Texture2D;
class TextureManager;
class VertexLayout;

class QColor;
class QOpenGLContext;
class QVector2D;
class QVector3D;
class QVector4D;
class QMatrix4x4;


class GLWrapper
{
public:
    GLWrapper();
    ~GLWrapper();

    bool isInitialized() const;
    void initialize(QOpenGLContext *glContext);

    // VAOs
    void createVao(GLVao &glVao);
    void destroyVao(const GLVao &glVao);

    // Buffers
    void createBuffer(GLBuffer &buffer, GLBuffer::Type type, GLBuffer::Usage usage);
    void destroyBuffer(GLBuffer &buffer);
    void allocateBuffer(const GLBuffer &buffer, uint32 size, const void *data);

    // Shaders and shader programs
    void createShaderProgram(GLShaderProgram &glProgram, const ShaderProgram &program);
    void destroyShaderProgram(const GLShaderProgram &glProgram);

    void sendUniforms(const GLShaderProgram &glProgram, const std::vector<ShaderParam *> &params);
    void sendActiveCameraUniforms(const GLShaderProgram &glProgram,
                                  const QMatrix4x4 &worldMatrix,
                                  const QMatrix4x4 &viewMatrix,
                                  const QMatrix4x4 &projectionMatrix, const QVector3D &cameraPos);
    void sendTransformUniform(const GLShaderProgram &glProgram,
                              const QMatrix4x4 &modelMatrix);
    void sendTextureUniforms(const GLShaderProgram &glProgram,
                             const std::vector<ShaderParam *> &textures,
                             TextureManager &textureManager);

    void setupVaoForBufferAndShader(const GLShaderProgram &glProgram,
                                    const GLVao &vaoId,
                                    const VertexLayout &geometryLayout,
                                    GLBuffer &vertexBuffer,
                                    GLBuffer *indexBuffer = nullptr);

    // Textures
    void createTexture2D(GLTexture &texture);
    void destroyTexture2D(GLTexture &texture);
    void allocateTexture2D(const GLTexture &texture,
                           const GLTexture::Params &params,
                           const ubyte *data);

    int maxTextureUnits();

    // Drawing
    void draw(const std::vector<DrawCommand> &commands);

    void checkForErrors();

private:
    void bindVao(const GLVao &buffer);
    void releaseVao(const GLVao &glVao);

    void bindBuffer(const GLBuffer &buffer);
    void releaseBuffer(const GLBuffer &buffer);

    void bindShaderProgram(const GLShaderProgram &glProgram);
    void releaseShaderProgram(const GLShaderProgram &glProgram);

    void bindTexture2D(const GLTexture &texture);
    void activeTexture2D(const GLTexture &texture, int i);
    void releaseTexture2D();

private:
    void compileShader(const GLShaderProgram &glProgram, uint32 shaderId,
                       const std::string &shaderSource);
    void linkShaderProgram(const GLShaderProgram &glProgram);

    std::vector<std::string> activeUniforms(const GLShaderProgram &glProgram) const;

private:
    void setUniform(uint32 programId, const char *name, int value);
    void setUniform(uint32 programId, const char *name, float value);
    void setUniform(uint32 programId, const char *name, const QVector2D &value);
    void setUniform(uint32 programId, const char *name, const QVector3D &value);
    void setUniform(uint32 programId, const char *name, const QVector4D &value);
    void setUniform(uint32 programId, const char *name, const QColor &value);
    void setUniform(uint32 programId, const char *name, const QMatrix4x4 &value);

private:
    OpenGLFuncs *m_gl;
};

#endif // GLWRAPPER_H
