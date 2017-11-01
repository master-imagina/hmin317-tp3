#ifndef GLBUFFER_H
#define GLBUFFER_H

#include "core/aliases_int.h"


struct GLBuffer
{
    enum Type : uint32
    {
        ArrayBuffer = 0x8892,       // GL_ARRAY_BUFFER
        IndexBuffer = 0x8893        // GL_ELEMENT_ARRAY_BUFFER
    };

    enum Usage : uint32
    {
        StaticDraw = 0x88E4,        // GL_STATIC_DRAW
        DynamicDraw = 0x88E8,       // GL_DYNAMIC_DRAW
        StreamDraw = 0x88E0         // GL_STREAM_DRAW
    };

    uint32 id = 0;
    Type type = Type::ArrayBuffer;
    Usage usage = Usage::StaticDraw;
};

#endif // GLBUFFER_H
