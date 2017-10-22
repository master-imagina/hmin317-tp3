#include "buffer.h"

#include <QDebug>
#include <QOpenGLContext>

Buffer::Buffer(Type type) :
    m_id(0),
    m_type(type),
    m_size(0),
    gl(nullptr)
{}

void Buffer::create()
{
    QOpenGLContext *currentGLContext = QOpenGLContext::currentContext();
    Q_ASSERT (currentGLContext);

    gl = currentGLContext->versionFunctions<OpenGLFuncs>();
    Q_ASSERT (gl);

    gl->glGenBuffers(1, &m_id);

    Q_ASSERT(m_id > 0);
}

void Buffer::destroy()
{
    if (m_id == 0) {
        qCritical() << "Buffer::destroy(): can't destroy an invalid buffer.";
        return;
    }

    gl->glDeleteBuffers(1, &m_id);
}

void Buffer::bind()
{
    gl->glBindBuffer(static_cast<unsigned int>(m_type), m_id);
}

void Buffer::release()
{
    gl->glBindBuffer(static_cast<unsigned int>(m_type), 0);
}

unsigned int Buffer::id() const
{
    return m_id;
}

void Buffer::allocate(int size, const void *data, Usage usage)
{
    m_size = size;

    gl->glBufferData(static_cast<unsigned int>(m_type),
                     size, data,
                     static_cast<unsigned int>(usage));
}

int Buffer::size() const
{
    return m_size;
}
