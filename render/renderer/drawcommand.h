#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include "core/aliases_int.h"

#include "../geometry/geometry.h"
#include "../material/material.h"

#include "../transform.h"

class GLBuffer;
class GLShaderProgram;
class GLVao;


struct DrawCommand
{
    GLShaderProgram *glProgram;
    GLVao *glVao;

    Geometry &geometry;
    Material &material;
    Transform &transform;

    GLBuffer *glVertexBuffer;
    GLBuffer *glIndexBuffer;
};

#endif // DRAWCOMMAND_H
