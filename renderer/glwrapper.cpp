#include "glwrapper.h"

#include "drawcommand.h"
#include "glbuffer.h"
#include "vertexlayout.h"

#include <QOpenGLContext>
#include <QOpenGLShader>


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

void GLWrapper::createGLBuffer(GLBuffer &buffer)
{
    m_gl->glGenBuffers(1, &buffer.id);

    Q_ASSERT(buffer.id > 0);
}

void GLWrapper::destroyGLBuffer(GLBuffer &buffer)
{
    m_gl->glDeleteBuffers(1, &buffer.id);
}

void GLWrapper::bindGLBuffer(const GLBuffer &buffer)
{
    m_gl->glBindBuffer(buffer.type, buffer.id);
}

void GLWrapper::releaseGLBuffer(const GLBuffer &buffer)
{
    m_gl->glBindBuffer(buffer.type, 0);
}

void GLWrapper::allocateGLBuffer(const GLBuffer &buffer,
                                   unsigned int size, const void *data)
{
    // Orphan allocation
    m_gl->glBufferData(buffer.type, size, nullptr, buffer.usage);
    m_gl->glBufferData(buffer.type, size, data, buffer.usage);
}

void GLWrapper::setupVaoForBufferAndShader(const QOpenGLShaderProgram &program,
                                             GLuint vao,
                                             const VertexLayout &vertexLayout,
                                             GLBuffer &arrayBuffer,
                                             GLBuffer *indexBuffer)
{
    m_gl->glUseProgram(program.programId());
    m_gl->glBindVertexArray(vao);

    bindGLBuffer(arrayBuffer);

    for (const VertexAttrib &attrib : vertexLayout.attributes()) {
        const int location = program.attributeLocation(attrib.name);

        m_gl->glEnableVertexAttribArray(location);
        m_gl->glVertexAttribPointer(location,
                                    attrib.size,
                                    static_cast<unsigned int>(attrib.type),
                                    attrib.normalized, attrib.stride,
                                    nullptr);
    }

    if (indexBuffer) {
        bindGLBuffer(*indexBuffer);
    }

    releaseGLBuffer(arrayBuffer);

    m_gl->glBindVertexArray(0);
    m_gl->glUseProgram(0);

    if (indexBuffer) {
        releaseGLBuffer(*indexBuffer);
    }
}

void GLWrapper::draw(const std::vector<DrawCommand> &commands)
{
    for (const DrawCommand &cmd : commands) {
        draw(cmd);
    }
}

void GLWrapper::draw(const DrawCommand &command)
{
    m_gl->glUseProgram(command.shaderId);
    m_gl->glBindVertexArray(command.vaoId);

    if (command.indexGLBuffer) {
        m_gl->glDrawElements(command.geometry.primitiveType,
                             command.geometry.primitiveCount,
                             GL_UNSIGNED_INT,
                             nullptr);
    }
    else {
        m_gl->glDrawArrays(command.geometry.primitiveType, 0,
                           command.geometry.primitiveCount);
    }

    m_gl->glBindVertexArray(0);
    m_gl->glUseProgram(0);
}
