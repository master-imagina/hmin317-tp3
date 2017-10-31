#ifndef GLWRAPPER_H
#define GLWRAPPER_H

#include "openglversion.h"

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
    void allocateBuffer(const GLBuffer &buffer, unsigned int size, const void *data);

    unsigned int buildShaderProgram(const ShaderProgram *program);
    void bindShaderProgram(unsigned int programId);
    void releaseShaderProgram(unsigned int programId);
    void destroyShaderProgram(unsigned int programId);
    void sendUniforms(unsigned int programId,
                      const std::vector<ShaderParam> &params);

    void setupVaoForBufferAndShader(GLuint programId,
                                    GLuint vaoId,
                                    const VertexLayout &geometryLayout,
                                    GLBuffer &vertexBuffer,
                                    GLBuffer *indexBuffer = nullptr);

    void draw(const std::vector<DrawCommand> &commands);

    void printAnyError();

private:
    void compileShader(unsigned int programId, unsigned int shaderId,
                       const QByteArray &shaderSource);
    void linkShaderProgram(unsigned int programId);

    std::vector<std::string> activeUniforms(unsigned int programId) const;

private:
    void setUniform(unsigned int programId, const char *name, int value);
    void setUniform(unsigned int programId, const char *name, float value);
    void setUniform(unsigned int programId, const char *name, const QVector2D &value);
    void setUniform(unsigned int programId, const char *name, const QVector3D &value);
    void setUniform(unsigned int programId, const char *name, const QVector4D &value);
    void setUniform(unsigned int programId, const char *name, const QColor &value);
    void setUniform(unsigned int programId, const char *name, const QMatrix4x4 &value);

private:
    OpenGLFuncs *m_gl;
};

#endif // GLWRAPPER_H
