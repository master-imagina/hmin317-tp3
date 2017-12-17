#include "collisionsystem.h"

#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"

#include "btBulletDynamicsCommon.h"

#include "physics/bullet_engine_bridge.h"
#include "physics/collider.h"
#include "physics/rigidbody.h"

#include "render/transform.h"


CollisionSystem::CollisionSystem() :
    m_broadPhase(std::make_unique<btDbvtBroadphase>()),
    m_collisionConfig(std::make_unique<btDefaultCollisionConfiguration>()),
    m_dispatcher(std::make_unique<btCollisionDispatcher>(m_collisionConfig.get())),
    m_solver(std::make_unique<btSequentialImpulseConstraintSolver>()),
    m_dynamicsWorld(std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_broadPhase.get(), m_solver.get(), m_collisionConfig.get())),
    m_collisionShapes(),
    m_rigidBodies(),
    m_motionStates()
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

        if (!collider.shape) {
            std::unique_ptr<btCollisionShape> shape;

            switch (collider.type) {
            case Collider::Type::Box:
                shape = std::make_unique<btBoxShape>(vec3ToBtVec3(collider.dimensions));
                break;
            case Collider::Type::Sphere:
                shape = std::make_unique<btSphereShape>(collider.dimensions.x());
                break;
            default:
                break;
            }

            collider.shape = shape.get();

            m_collisionShapes.emplace_back(std::move(shape));
        }

        if (!rigidBody.bulletRigidBody && !rigidBody.motionState) {
            auto motionState = std::make_unique<btDefaultMotionState>();
            rigidBody.motionState = motionState.get();
            m_motionStates.emplace_back(std::move(motionState));

            btTransform startTransform = transformToBtTransform(*transform.get());

            startTransform.setOrigin(vec3ToBtVec3(transform->translate() + collider.origin));

            rigidBody.motionState->setWorldTransform(startTransform);

            createBtRigidBody(collider, rigidBody);

            m_dynamicsWorld->addRigidBody(rigidBody.bulletRigidBody);
        }

        if (rigidBody.mass > 0.f) {
            btTransform bulletTransform;
            rigidBody.bulletRigidBody->getMotionState()->getWorldTransform(bulletTransform);

            transform->setTranslate(btVec3ToVec3(bulletTransform.getOrigin()));
        }
        //TODO update rotation too
    });
}

void CollisionSystem::createBtRigidBody(const Collider &collider, RigidBody &rigidBody)
{
    btVector3 bulletFallIntertia(rigidBody.fallInertia.x(),
                                 rigidBody.fallInertia.y(),
                                 rigidBody.fallInertia.z());

    if (rigidBody.mass > 0.f) {
        collider.shape->calculateLocalInertia(rigidBody.mass, bulletFallIntertia);
    }

    btRigidBody::btRigidBodyConstructionInfo
            rigidBodyCI(rigidBody.mass,
                        rigidBody.motionState,
                        collider.shape,
                        bulletFallIntertia);

    rigidBodyCI.m_restitution = rigidBody.restitution;
    rigidBodyCI.m_friction = rigidBody.friction;
    rigidBodyCI.m_linearDamping = rigidBody.linearDamping;

    auto bulletRigidBody = std::make_unique<btRigidBody>(rigidBodyCI);

    rigidBody.bulletRigidBody = bulletRigidBody.get();

    m_rigidBodies.emplace_back(std::move(bulletRigidBody));
}
