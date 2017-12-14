#include "rigidbody.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "LinearMath/btDefaultMotionState.h"

#include "physics/bullet_engine_bridge.h"


RigidBody::RigidBody() :
    mass(0.f),
    fallInertia(),
    restitution(1.f),
    friction(1.f),
    linearDamping(0.f),
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
