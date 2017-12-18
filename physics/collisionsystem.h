#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "3rdparty/entityx/System.h"

#include "core/aliases_memory.h"

class btBroadphaseInterface;
class btCollisionDispatcher;
class btCollisionShape;
class btDefaultCollisionConfiguration;
class btDefaultMotionState;
class btDiscreteDynamicsWorld;
class btRigidBody;
class btSequentialImpulseConstraintSolver;
class btStridingMeshInterface;

class Collider;
class RigidBody;


class CollisionSystem : public entityx::System<CollisionSystem>
{
public:
    CollisionSystem();
    ~CollisionSystem();

    void configure(entityx::EntityManager &entities,
                   entityx::EventManager &events) override;

    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override;

    void clear(entityx::EntityManager &entities);

private:
    void createBtRigidBody(const Collider &collider, RigidBody &rigidBody);

private:
    uptr<btBroadphaseInterface> m_broadPhase;
    uptr<btDefaultCollisionConfiguration> m_collisionConfig;
    uptr<btCollisionDispatcher> m_dispatcher;
    uptr<btSequentialImpulseConstraintSolver> m_solver;
    uptr<btDiscreteDynamicsWorld> m_dynamicsWorld;

    uptr_vector<btCollisionShape> m_collisionShapes;
    uptr_vector<btRigidBody> m_rigidBodies;
    uptr_vector<btDefaultMotionState> m_motionStates;
    uptr_vector<btStridingMeshInterface> m_meshInterfaces;
};

#endif // COLLISIONSYSTEM_H
