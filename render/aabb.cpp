#include "aabb.h"

#include "render/geometry/geometry.h"

#include "render/mesh.h"


////////////////////// AABoundingBox //////////////////////

AABoundingBox::AABoundingBox() :
    m_center(),
    m_radius()
{}

AABoundingBox::AABoundingBox(const std::vector<Vertex> vertices) :
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

void AABoundingBox::processVertices(const std::vector<Vertex> vertices)
{
    if (vertices.empty()) {
        m_center = QVector3D();
        m_radius = QVector3D();

        return;
    }

    QVector3D min = vertices[0].position;
    QVector3D max = vertices[0].position;

    for (int i = 1; i < vertices.size(); ++i) {
        const QVector3D &p = vertices[i].position;

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

void AABoundingBox::expand(const AABoundingBox &other)
{
    const QVector3D otherRadius = other.radius();
    const float otherRadiusX = otherRadius.x();
    const float otherRadiusY = otherRadius.y();
    const float otherRadiusZ = otherRadius.z();

    if (m_radius.x() < otherRadiusX) {
        m_radius.setX(otherRadiusX);
    }
    if (m_radius.y() < otherRadiusY) {
        m_radius.setY(otherRadiusY);
    }
    if (m_radius.z() < otherRadiusZ) {
        m_radius.setZ(otherRadiusZ);
    }

    m_center.setX(m_center.x() + other.center().x() * 0.5);
    m_center.setY(m_center.y() + other.center().y() * 0.5);
    m_center.setZ(m_center.z() + other.center().z() * 0.5);
}


////////////////////// Functions //////////////////////

AABoundingBox meshAABB(const Geometry &geom)
{
    return AABoundingBox(geom.vertices);
}

AABoundingBox meshAABB(Mesh &mesh)
{
    AABoundingBox ret;

    for (int meshIdx = 0; meshIdx < mesh.count(); meshIdx++) {
        const Geometry &geom = mesh.geometry(meshIdx);

        ret.expand(meshAABB(geom));
    }

    return ret;
}
