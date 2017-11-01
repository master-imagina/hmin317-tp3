#ifndef AABB_H
#define AABB_H

#include <vector>

#include <QVector3D>


class AABoundingBox
{
public:
    AABoundingBox();
    explicit AABoundingBox(const std::vector<QVector3D> vertices);

    QVector3D center() const;
    QVector3D radius() const;

    std::array<QVector3D, 8> getCorners();

    void processVertices(const std::vector<QVector3D> vertices);

private:
    QVector3D m_center;
    QVector3D m_radius;
};

#endif // AABB_H
