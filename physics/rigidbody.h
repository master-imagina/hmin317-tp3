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
    void applyForce(const QVector3D &force, const QVector3D &relPos);

    float mass;
    QVector3D fallInertia;
    float restitution;
    float friction;
    float linearDamping;
    float rollingFriction;

    btRigidBody *bulletRigidBody;
    btDefaultMotionState *motionState;
};


QDataStream &operator<<(QDataStream &os, const RigidBody &rigidBody);
QDataStream &operator>>(QDataStream &os, RigidBody &rigidBody);

#endif // RIGIDBODY_H
