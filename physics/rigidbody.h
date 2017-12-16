#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <QVector3D>

class btDefaultMotionState;
class btRigidBody;


struct RigidBody
{
    RigidBody();
    ~RigidBody();

    void applyCentralImpulse(const QVector3D &impulse);

    float mass;
    QVector3D fallInertia;
    float restitution;
    float friction;
    float linearDamping;

    btRigidBody *bulletRigidBody;
    btDefaultMotionState *motionState;
};


QDataStream &operator<<(QDataStream &os, const RigidBody &rigidBody);
QDataStream &operator>>(QDataStream &os, RigidBody &rigidBody);

#endif // RIGIDBODY_H
