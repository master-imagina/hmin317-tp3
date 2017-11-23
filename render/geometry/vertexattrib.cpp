#include "vertexattrib.h"

#include "render/geometry/geometry.h"


bool operator==(const VertexAttrib &lhs, const VertexAttrib &rhs)
{
    if (lhs.name != rhs.name) {
        return false;
    }

    if (lhs.size != rhs.size) {
        return false;
    }

    if (lhs.type != rhs.type) {
        return false;
    }

    if (lhs.normalized != rhs.normalized) {
        return false;
    }

    if (lhs.stride != rhs.stride) {
        return false;
    }

    return true;
}

bool operator!=(const VertexAttrib &lhs, const VertexAttrib &rhs)
{
    return !operator==(lhs, rhs);
}


std::string defaultPositionAttribName()
{
    return "vertexPos";
}

std::string defaultNormalAttribName()
{
    return "normal";
}

std::string defaultTexCoordsAttribName()
{
    return "texCoords";
}

VertexAttrib defaultPositionAttrib()
{
    return {
        defaultPositionAttribName(),
        3,
        VertexAttrib::Type::Float,
        false,
        Geometry::vertexSize
    };
}

VertexAttrib defaultNormalAttrib()
{
    return {
        defaultNormalAttribName(),
        3,
        VertexAttrib::Type::Float,
        false,
        Geometry::vertexSize,
        offsetof(Vertex, normal)
    };
}

VertexAttrib defaultTexCoordsAttrib()
{
    return {
        defaultTexCoordsAttribName(),
        2,
        VertexAttrib::Type::Float,
        false,
        Geometry::vertexSize,
        offsetof(Vertex, texCoords)
    };
}
