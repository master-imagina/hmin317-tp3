#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "3rdparty/entityx/System.h"


class ParticleSystem : public entityx::System<ParticleSystem>
{
public:
    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override;
};

#endif // PARTICLESYSTEM_H
