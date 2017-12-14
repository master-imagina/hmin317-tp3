#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "3rdparty/entityx/System.h"

#include "core/aliases_memory.h"

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;


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

private:
    uptr<btBroadphaseInterface> m_broadPhase;
    uptr<btDefaultCollisionConfiguration> m_collisionConfig;
    uptr<btCollisionDispatcher> m_dispatcher;
    uptr<btSequentialImpulseConstraintSolver> m_solver;
    uptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
};

#endif // COLLISIONSYSTEM_H
