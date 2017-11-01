#include "aabb.h"


AABoundingBox::AABoundingBox() :
    m_center(),
    m_radius()
{}

AABoundingBox::AABoundingBox(const std::vector<QVector3D> vertices) :
    m_center(),
    m_radius()
{
    processVertices(vertices);
}

QVector3D AABoundingBox::center() const
{
    return m_center;
}

QVector3D AABoundingBox::radius() const
{
    return m_radius;
}

std::array<QVector3D, 8> AABoundingBox::getCorners()
{
    std::array<QVector3D, 8> ret;

    const float cx = m_center.x();
    const float cy = m_center.y();
    const float cz = m_center.z();

    const float rx = m_radius.x();
    const float ry = m_radius.y();
    const float rz = m_radius.z();

    ret.at(0) = {cx - rx, cy - ry, cz + rz};
    ret.at(1) = {cx + rx, cy - ry, cz + rz};
    ret.at(2) = {cx + rx, cy + ry, cz + rz};
    ret.at(3) = {cx - rx, cy + ry, cz + rz};

    ret.at(4) = {cx - rx, cy - ry, cz - rz};
    ret.at(5) = {cx + rx, cy - ry, cz - rz};
    ret.at(6) = {cx + rx, cy + ry, cz - rz};
    ret.at(7) = {cx - rx, cy + ry, cz - rz};

    return ret;
}

void AABoundingBox::processVertices(const std::vector<QVector3D> vertices)
{
    if (vertices.empty()) {
        m_center = QVector3D();
        m_radius = QVector3D();

        return;
    }

    QVector3D min = vertices[0];
    QVector3D max = vertices[0];

    for (int i = 1; i < vertices.size(); ++i) {
        const QVector3D &p = vertices[i];

        const float px = p.x();
        const float py = p.y();
        const float pz = p.z();

        if (px < min.x()) {
            min.setX(px);
        }

        if (py < min.y()) {
            min.setY(py);
        }

        if (pz < min.z()) {
            min.setZ(pz);
        }

        if (px > max.x()) {
            max.setX(px);
        }

        if (py > max.y()) {
            max.setY(py);
        }

        if (pz > max.z()) {
            max.setZ(pz);
        }
    }

    m_center = 0.5 * (min + max);
    m_radius = 0.5 * (max - min);
}
