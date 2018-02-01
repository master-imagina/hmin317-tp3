#include "collider.h"

#include <QDataStream>
#include <QVector3D>


////////////////////// Collider //////////////////////

Collider::Collider() :
    type(Type::Box),
    origin(),
    dimensions(),
    shape(nullptr),
    meshInterface(nullptr)
{}

Collider::Collider(Type _type) :
    type(_type),
    origin(),
    dimensions(),
    shape(nullptr),
    meshInterface(nullptr)
{}

Collider::Collider(Type _type, const QVector3D &dims, const QVector3D &orig) :
    type(_type),
    origin(orig),
    dimensions(dims),
    shape(nullptr),
    meshInterface(nullptr)
{
    if (type == Type::Sphere) {
        Q_ASSERT (qFuzzyCompare(dims.x(), dims.y()) && qFuzzyCompare(dims.y(), dims.z()));
    }
}

Collider::~Collider()
{}


////////////////////// Functions //////////////////////

QDataStream &operator<<(QDataStream &os, const Collider &collider)
{
    os << (quint32) collider.type
       << collider.origin
       << collider.dimensions;
}

QDataStream &operator>>(QDataStream &os, Collider &collider)
{
    int rawType;
    os >> rawType;
    collider.type = (Collider::Type) rawType;

    os >> collider.origin;
    os >> collider.dimensions;
}
