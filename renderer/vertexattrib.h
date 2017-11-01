#ifndef VERTEXATTRIB_H
#define VERTEXATTRIB_H

#include <QString>

#include "../aliases_int.h"


struct VertexAttrib
{
public:
    enum class Type : uint32
    {
        Int = 0x1404,       // GL_INT
        UInt = 0x1405,      // GL_UNSIGNED_INT
        Float = 0x1406      // GL_FLOAT
    };

    friend bool operator==(const VertexAttrib &lhs, const VertexAttrib &rhs);
    friend bool operator!=(const VertexAttrib &lhs, const VertexAttrib &rhs);

public:
    QString name;
    int size;
    Type type;
    bool normalized = false;
    int stride;
};

#endif // VERTEXATTRIB_H
