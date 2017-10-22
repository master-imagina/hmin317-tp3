#ifndef VERTEXATTRIB_H
#define VERTEXATTRIB_H

#include <QString>


struct VertexAttrib
{
public:
    enum class Type : unsigned int
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
