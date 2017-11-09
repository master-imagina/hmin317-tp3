#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include "core/aliases_int.h"

#include "../geometry/geometry.h"
#include "../material/material.h"

#include "../transform.h"

#include "glbuffer.h"


struct DrawCommand
{
    uint32 shaderProgramId;
    uint32 vaoId;

    Geometry &geometry;
    Material &material;
    Transform &transform;

    GLBuffer *vertexGLBuffer;
    GLBuffer *indexGLBuffer;
};

#endif // DRAWCOMMAND_H
