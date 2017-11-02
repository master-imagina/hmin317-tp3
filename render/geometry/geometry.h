#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

#include <QVector3D>

#include "core/aliases_int.h"

#include "vertexlayout.h"


class Geometry
{
public:
    enum PrimitiveType
    {
        Points = 0x0000,        // GL_POINTS
        Triangles = 0x0004      // GL_TRIANGLES
    };

    std::vector<QVector3D> vertices;
    std::vector<uint32> indices;

    std::size_t primitiveCount = 0;
    PrimitiveType primitiveType = Points;

    static const std::size_t vertexSize = sizeof(QVector3D);
    static const std::size_t indexSize = sizeof(uint32);

    VertexLayout vertexLayout;

    bool isDirty = false;
};

#endif // GEOMETRY_H
