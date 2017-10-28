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

    void createBuffer(GLBuffer &buffer);
    void destroyBuffer(GLBuffer &buffer);
    void bindBuffer(const GLBuffer &buffer);
    void releaseBuffer(const GLBuffer &buffer);
    void allocateBuffer(const GLBuffer &buffer, unsigned int size, const void *data);

    void setupVaoForBufferAndShader(GLuint programId,
                                    GLuint vaoId,
                                    const VertexLayout &geometryLayout,
                                    GLBuffer &vertexBuffer,
                                    GLBuffer *indexBuffer = nullptr);

    void draw(const std::vector<DrawCommand> &commands);

private:
    OpenGLFuncs *m_gl;
};

#endif // GLWRAPPER_H
