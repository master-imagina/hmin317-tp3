#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

#include <QVector2D>
#include <QVector3D>


class Geometry
{
public:
    Geometry();
    ~Geometry();

    std::vector<QVector3D> vertices;
    std::vector<unsigned int> indices;
};

#endif // GEOMETRY_H
