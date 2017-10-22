#ifndef BUFFER_H
#define BUFFER_H

#include "openglversion.h"
#include "vertexattrib.h"


class Buffer
{
public:
    enum class Type : unsigned int
    {
        ArrayBuffer = 0x8892,       // GL_ARRAY_BUFFER
        IndexBuffer = 0x8893        // GL_ELEMENT_ARRAY_BUFFER
    };

    enum class Usage : unsigned int
    {
        StaticDraw = 0x88E4,        // GL_STATIC_DRAW
        DynamicDraw = 0x88E8,       // GL_DYNAMIC_DRAW
        StreamDraw = 0x88E0         // GL_STREAM_DRAW
    };

    Buffer(Type type);

    void create();
    void destroy();

    void bind();
    void release();

    void allocate(int size, const void *data, Usage usage);

    int size() const;

    unsigned int id() const;

private:
    unsigned int m_id;
    Type m_type;
    int m_size;

    OpenGLFuncs *gl;
};

#endif // BUFFER_H
