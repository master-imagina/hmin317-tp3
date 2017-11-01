#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include "core/aliases_int.h"

#include "../geometry/geometry.h"

#include "glbuffer.h"


struct DrawCommand
{
    uint32 shaderId;
    uint32 vaoId;

    Geometry &geometry;
    GLBuffer &vertexGLBuffer;
    GLBuffer *indexGLBuffer;
};

#endif // DRAWCOMMAND_H
