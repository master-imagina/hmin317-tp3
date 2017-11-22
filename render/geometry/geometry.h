#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

#include <QVector2D>
#include <QVector3D>

#include "core/aliases_int.h"

#include "vertexlayout.h"


struct Vertex
{
    QVector3D position;
    QVector3D normal;
    QVector2D texCoords;
};


class Geometry
{
public:
    enum PrimitiveType
    {
        Points = 0x0000,        // GL_POINTS
        Triangles = 0x0004      // GL_TRIANGLES
    };

    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    std::size_t primitiveCount = 0;
    PrimitiveType primitiveType = Points;

    static const std::size_t vertexSize = sizeof(Vertex);
    static const std::size_t indexSize = sizeof(uint32);

    VertexLayout vertexLayout;

    bool isDynamic = false;
    bool isIndexed = false;
    bool isDirty = false;
};

#endif // GEOMETRY_H
