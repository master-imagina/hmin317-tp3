#ifndef BULLETENGINEBRIDGE_H
#define BULLETENGINEBRIDGE_H

#include <QVector3D>

#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

class Transform;


inline btVector3 vec3ToBtVec3(const QVector3D &v)
{
    return btVector3(v.x(), v.y(), v.z());
}

inline QVector3D btVec3ToVec3(const btVector3 &v)
{
    return QVector3D(v.x(), v.y(), v.z());
}

QVector3D btQuatToVec3(const btQuaternion &quat);

btTransform transformToBtTransform(const Transform &transform);

#endif // BULLETENGINEBRIDGE_H
