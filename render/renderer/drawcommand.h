#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include <cstdint>

#include "glbuffer.h"

#include "../geometry.h"


struct DrawCommand
{
    std::uint32_t shaderId;
    std::uint32_t vaoId;

    Geometry &geometry;
    GLBuffer &vertexGLBuffer;
    GLBuffer *indexGLBuffer;
};


#endif // DRAWCOMMAND_H
