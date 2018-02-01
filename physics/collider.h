#ifndef COLLIDER_H
#define COLLIDER_H

#include <QVector3D>

class btCollisionShape;
class btStridingMeshInterface;

struct Collider
{
    enum class Type
    {
        Box,
        Sphere,
        Mesh
    };

    Collider();
    Collider(Type _type);
    Collider(Type _type, const QVector3D &dims, const QVector3D &orig = QVector3D());
    ~Collider();

    Type type;
    QVector3D origin;
    QVector3D dimensions;

    btCollisionShape *shape;
    btStridingMeshInterface *meshInterface;
};


QDataStream &operator<<(QDataStream &os, const Collider &collider);
QDataStream &operator>>(QDataStream &os, Collider &collider);

#endif // COLLIDER_H
