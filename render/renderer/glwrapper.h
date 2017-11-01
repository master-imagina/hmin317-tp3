#ifndef GLWRAPPER_H
#define GLWRAPPER_H

#include "openglversion.h"
#include "core/aliases_int.h"

class DrawCommand;
class GLBuffer;
class ShaderParam;
class ShaderProgram;
class VertexLayout;

class QOpenGLContext;
class QVector2D;
class QVector3D;
class QVector4D;
class QMatrix4x4;
class QColor;


class GLWrapper
{
public:
    GLWrapper();
    ~GLWrapper();

    void initialize(QOpenGLContext *glContext);

    void createBuffer(GLBuffer &buffer);
    void destroyBuffer(GLBuffer &buffer);
    void bindBuffer(const GLBuffer &buffer);
    void releaseBuffer(const GLBuffer &buffer);
    void allocateBuffer(const GLBuffer &buffer, uint32 size, const void *data);

    uint32 buildShaderProgram(const ShaderProgram *program);
    void bindShaderProgram(uint32 programId);
    void releaseShaderProgram(uint32 programId);
    void destroyShaderProgram(uint32 programId);
    void sendUniforms(uint32 programId,
                      const std::vector<ShaderParam> &params);

    void setupVaoForBufferAndShader(GLuint programId,
                                    GLuint vaoId,
                                    const VertexLayout &geometryLayout,
                                    GLBuffer &vertexBuffer,
                                    GLBuffer *indexBuffer = nullptr);

    void draw(const std::vector<DrawCommand> &commands);

    void printAnyError();

private:
    void compileShader(uint32 programId, uint32 shaderId,
                       const QByteArray &shaderSource);
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
