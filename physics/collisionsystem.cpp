#include "collisionsystem.h"

#include "btBulletDynamicsCommon.h"

#include "render/transform.h"

#include "physics/bullet_engine_bridge.h"
#include "physics/collider.h"
#include "physics/rigidbody.h"


namespace {

void createBtRigidBody(const Collider &collider, RigidBody &rigidBody)
{
    btVector3 bulletFallIntertia(rigidBody.fallInertia.x(),
                                 rigidBody.fallInertia.y(),
                                 rigidBody.fallInertia.z());

    if (rigidBody.mass > 0.f) {
        collider.shape->calculateLocalInertia(rigidBody.mass, bulletFallIntertia);
    }

    btRigidBody::btRigidBodyConstructionInfo
            rigidBodyCI(rigidBody.mass,
                        rigidBody.motionState.get(),
                        collider.shape.get(),
                        bulletFallIntertia);

    rigidBodyCI.m_restitution = rigidBody.restitution;
    rigidBodyCI.m_friction = rigidBody.friction;
    rigidBodyCI.m_linearDamping = rigidBody.linearDamping;

    rigidBody.bulletRigidBody = std::make_unique<btRigidBody>(rigidBodyCI);
}

} // anon namespace


CollisionSystem::CollisionSystem() :
    m_broadPhase(std::make_unique<btDbvtBroadphase>()),
    m_collisionConfig(std::make_unique<btDefaultCollisionConfiguration>()),
    m_dispatcher(std::make_unique<btCollisionDispatcher>(m_collisionConfig.get())),
    m_solver(std::make_unique<btSequentialImpulseConstraintSolver>()),
    m_dynamicsWorld(std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_broadPhase.get(), m_solver.get(), m_collisionConfig.get()))
{
    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

CollisionSystem::~CollisionSystem()
{}

void CollisionSystem::configure(entityx::EntityManager &entities,
                                entityx::EventManager &events)
{

}

void CollisionSystem::update(entityx::EntityManager &entities,
                             entityx::EventManager &events,
                             double dt)
{
    m_dynamicsWorld->stepSimulation(dt, 10);

    entities.each<Collider, RigidBody>(
                [this] (entityx::Entity entity,
                        Collider &collider, RigidBody &rigidBody) {
        auto transform = entity.component<Transform>();

        if (!rigidBody.bulletRigidBody) {
            rigidBody.motionState = std::make_unique<btDefaultMotionState>();
            btTransform startTransform = transformToBtTransform(*transform.get());

            if (rigidBody.mass > 0.f) {
                startTransform.setOrigin(vec3ToBtVec3(transform->translate()));
            }
            else {
                startTransform.setOrigin(vec3ToBtVec3(collider.origin));
            }

            rigidBody.motionState->setWorldTransform(startTransform);

            createBtRigidBody(collider, rigidBody);

            m_dynamicsWorld->addRigidBody(rigidBody.bulletRigidBody.get());
        }

        btTransform bulletTransform;
        rigidBody.bulletRigidBody->getMotionState()->getWorldTransform(bulletTransform);

        const btVector3 &origin = bulletTransform.getOrigin();

        if (rigidBody.mass > 0.f) {
            transform->setTranslate({origin.x(), origin.y(), origin.z()});
        }
        //TODO update rotation too
    });
}
