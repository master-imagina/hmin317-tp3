#ifndef AABB_H
#define AABB_H

#include <vector>

#include <QVector3D>

struct Vertex;


class AABoundingBox
{
public:
    AABoundingBox();
    explicit AABoundingBox(const std::vector<Vertex> vertices);

    QVector3D center() const;
    QVector3D radius() const;

    std::array<QVector3D, 8> getCorners();

    void processVertices(const std::vector<Vertex> vertices);

private:
    QVector3D m_center;
    QVector3D m_radius;
};

#endif // AABB_H
