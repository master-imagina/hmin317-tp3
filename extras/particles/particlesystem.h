#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "3rdparty/entityx/System.h"

class Geometry;
class ParticleEffect;
class Transform;


class ParticleSystem : public entityx::System<ParticleSystem>
{
public:
    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override;

private:
    static void liveParticles(entityx::Entity entity,
                              ParticleEffect &particleEffect,
                              Geometry &geom,
                              Transform &transform,
                              double dt);
};

#endif // PARTICLESYSTEM_H
