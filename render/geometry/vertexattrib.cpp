#include "vertexattrib.h"


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
