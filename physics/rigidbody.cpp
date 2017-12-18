#include "rigidbody.h"

#include <QDataStream>

#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "LinearMath/btDefaultMotionState.h"

#include "physics/bullet_engine_bridge.h"


////////////////////// RigidBody //////////////////////

RigidBody::RigidBody() :
    mass(0.f),
    fallInertia(),
    restitution(1.f),
    friction(1.f),
    linearDamping(0.f),
    rollingFriction(0.f),
    bulletRigidBody(nullptr),
    motionState(nullptr)
{}

RigidBody::~RigidBody()
{}

void RigidBody::applyCentralImpulse(const QVector3D &impulse)
{
    Q_ASSERT (bulletRigidBody);

    bulletRigidBody->activate();
    bulletRigidBody->applyCentralImpulse(vec3ToBtVec3(impulse));
}

void RigidBody::applyForce(const QVector3D &force, const QVector3D &relPos)
{
    Q_ASSERT (bulletRigidBody);

    bulletRigidBody->activate();
    bulletRigidBody->applyForce(vec3ToBtVec3(force), vec3ToBtVec3(relPos));
}


////////////////////// Functions //////////////////////

QDataStream &operator<<(QDataStream &os, const RigidBody &rigidBody)
{
    os << rigidBody.mass
       << rigidBody.fallInertia
       << rigidBody.restitution
       << rigidBody.friction
       << rigidBody.linearDamping
       << rigidBody.rollingFriction;
}

QDataStream &operator>>(QDataStream &os, RigidBody &rigidBody)
{
    os >> rigidBody.mass;
    os >> rigidBody.fallInertia;
    os >> rigidBody.restitution;
    os >> rigidBody.friction;
    os >> rigidBody.linearDamping;
    os >> rigidBody.rollingFriction;
}
