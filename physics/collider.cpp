#include "collider.h"

#include <QVector3D>

#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"


Collider::Collider() :
    type(Type::Box),
    origin(),
    shape(std::make_unique<btBoxShape>(btVector3(1, 1, 1)))
{}

Collider::Collider(Type _type, const QVector3D &dims, const QVector3D &orig) :
    type(_type),
    origin(orig),
    shape(nullptr)
{
    Q_ASSERT (type == Type::Box || type == Type::Sphere);

    if (type == Type::Sphere) {
        Q_ASSERT (qFuzzyCompare(dims.x(), dims.y()) && qFuzzyCompare(dims.y(), dims.z()));
    }

    switch (type) {
    case Type::Box:
        shape = std::make_unique<btBoxShape>(btVector3(dims.x(), dims.y(), dims.z()));
        break;
    case Type::Sphere:
        shape = std::make_unique<btSphereShape>(dims.x());
        break;
    default:
        break;
    }
}

Collider::~Collider()
{}
