#ifndef GLWRAPPER_H
#define GLWRAPPER_H

#include "core/aliases_int.h"
#include "core/aliases_memory.h"

#include "glbuffer.h"
#include "openglversion.h"

class DrawCommand;
class GLBuffer;
class ShaderParam;
class ShaderProgram;
class Texture2D;
class Texture2DParams;
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

    void initialize(QOpenGLContext *glContext);

    // VAOs
    uint32 createVao();
    uint32 destroyVao(uint32 &vaoId);

    // Buffers
    void createBuffer(GLBuffer &buffer, GLBuffer::Type type, GLBuffer::Usage usage);
    void destroyBuffer(GLBuffer &buffer);
    void bindBuffer(const GLBuffer &buffer);
    void releaseBuffer(const GLBuffer &buffer);
    void allocateBuffer(const GLBuffer &buffer, uint32 size, const void *data);

    // Shaders and shader programs
    uint32 buildShaderProgram(const ShaderProgram *program);
    void bindShaderProgram(uint32 programId);
    void releaseShaderProgram(uint32 programId);
    void destroyShaderProgram(uint32 programId);
    void sendUniforms(uint32 programId, const std::vector<ShaderParam *> &params);
    void sendActiveCameraUniforms(uint32 programId,
                                  const QMatrix4x4 &worldMatrix,
                                  const QMatrix4x4 &viewMatrix,
                                  const QMatrix4x4 &projectionMatrix);
    void sendTransformUniform(uint32 programId, const QMatrix4x4 &modelMatrix);
    void sendTextureUniforms(uint32 programId,
                             const std::vector<ShaderParam *> &textures,
                             TextureManager &textureManager);

    void setupVaoForBufferAndShader(GLuint programId,
                                    GLuint vaoId,
                                    const VertexLayout &geometryLayout,
                                    GLBuffer &vertexBuffer,
                                    GLBuffer *indexBuffer = nullptr);

    // Textures
    uint32 createTexture2D();
    void destroyTexture2D(uint32 &textureId);
    void allocateTexture2D(uint32 textureId,
                           const Texture2DParams &params,
                           const ubyte *data);
    void bindTexture2D(uint32 textureId);
    void activeTexture2D(uint32 textureId, int i);
    void releaseTexture2D();

    int maxTextureUnits();

    // UBO
    uint32 uniformBlockIndex(uint32 programId, const char *name);
    void setUniformBlockForUBO(uint32 programId, uint32 blockIndex, uint32 bindingPoint);

    // Drawing
    void draw(const std::vector<DrawCommand> &commands);

    void checkForErrors();

private:
    void compileShader(uint32 programId, uint32 shaderId,
                       const std::string &shaderSource);
    void linkShaderProgram(uint32 programId);

    std::vector<std::string> activeUniforms(uint32 programId) const;

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
