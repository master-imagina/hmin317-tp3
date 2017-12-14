#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <QVector3D>

#include "core/aliases_memory.h"

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

    uptr<btRigidBody> bulletRigidBody;
    uptr<btDefaultMotionState> motionState;
};

#endif // RIGIDBODY_H
