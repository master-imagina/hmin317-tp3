#ifndef GLWRAPPER_H
#define GLWRAPPER_H

#include "openglversion.h"

class DrawCommand;
class GLBuffer;
class VertexLayout;

class QOpenGLContext;
class QOpenGLShaderProgram;


class GLWrapper
{
public:
    GLWrapper();
    ~GLWrapper();

    void initialize(QOpenGLContext *glContext);

    void createGLBuffer(GLBuffer &buffer);
    void destroyGLBuffer(GLBuffer &buffer);
    void bindGLBuffer(const GLBuffer &buffer);
    void releaseGLBuffer(const GLBuffer &buffer);
    void allocateGLBuffer(const GLBuffer &buffer, unsigned int size, const void *data);

    void setupVaoForBufferAndShader(const QOpenGLShaderProgram &program,
                                    GLuint vao,
                                    const VertexLayout &geometryLayout,
                                    GLBuffer &vertexBuffer,
                                    GLBuffer *indexBuffer = nullptr);

    void draw(const std::vector<DrawCommand> &commands);

private:
    void draw(const DrawCommand &command);

private:
    OpenGLFuncs *m_gl;
};

#endif // GLWRAPPER_H
