#ifndef COLLIDER_H
#define COLLIDER_H

#include <QVector3D>

#include "core/aliases_memory.h"

class btCollisionShape;


struct Collider
{
    enum class Type
    {
        Box,
        Sphere,
        Plane
    };

    Collider();
    Collider(Type type, const QVector3D &dims, const QVector3D &orig = QVector3D());
    ~Collider();

    Type type;
    QVector3D origin;

    uptr<btCollisionShape> shape;
};

#endif // COLLIDER_H
