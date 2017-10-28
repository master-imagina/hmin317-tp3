#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

#include <QVector3D>


class Geometry
{
public:
    enum PrimitiveType
    {
        Points = 0x0000,        // GL_POINTS
        Triangles = 0x0004      // GL_TRIANGLES
    };

    std::vector<QVector3D> vertices;
    std::vector<unsigned int> indices;

    std::size_t primitiveCount = 0;
    PrimitiveType primitiveType = Points;

    static const std::size_t vertexSize = sizeof(QVector3D);
    static const std::size_t indexSize = sizeof(unsigned int);

    bool isDirty = false;
};

#endif // GEOMETRY_H
