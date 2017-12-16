#ifndef AABB_H
#define AABB_H

#include <vector>

#include <QVector3D>

struct Vertex;

class Geometry;
class Mesh;


class AABoundingBox
{
public:
    AABoundingBox();
    explicit AABoundingBox(const std::vector<Vertex> vertices);

    QVector3D center() const;
    QVector3D radius() const;

    std::array<QVector3D, 8> getCorners();

    void processVertices(const std::vector<Vertex> vertices);

    void expand(const AABoundingBox &other);

private:
    QVector3D m_center;
    QVector3D m_radius;
};


AABoundingBox meshAABB(const Geometry &geom);
AABoundingBox meshAABB(Mesh &mesh);

#endif // AABB_H
