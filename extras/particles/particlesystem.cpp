#include "particlesystem.h"

#include <random>

#include "render/geometry/geometry.h"

#include "particleeffect.h"


void ParticleSystem::update(entityx::EntityManager &entities,
                            entityx::EventManager &events,
                            double dt)
{
    entities.each<ParticleEffect, Geometry>([dt] (entityx::Entity entity,
                                                  ParticleEffect &particleEffect,
                                                  Geometry &geom) {
        liveParticles(entity, particleEffect, geom, dt);
    });
}

void ParticleSystem::liveParticles(entityx::Entity entity,
                                   ParticleEffect &particleEffect,
                                   Geometry &geom, double dt)
{
    const QVector3D worldPos = particleEffect.worldPos();
    const float worldPosX = worldPos.x();
    const float worldPosZ = worldPos.z();

    const float radius = particleEffect.radius();

    static std::random_device m_randDevice;
    static std::default_random_engine m_randEngine(m_randDevice());

    static std::uniform_real_distribution<float> m_radiusXRandDistrib;
    static std::uniform_real_distribution<float> m_radiusZRandDistrib;

    // If the particle effect's primary attributes were changed, change the
    // random settings accordingly
    if (particleEffect.isDirty()) {
        m_radiusXRandDistrib = std::uniform_real_distribution<float>(worldPosX - radius, worldPosX + radius);
        m_radiusZRandDistrib = std::uniform_real_distribution<float>(worldPosZ - radius, worldPosZ + radius);

        const int count = particleEffect.count();

        geom.vertices.resize(count);
        geom.primitiveCount = count;

        std::fill(geom.vertices.begin(), geom.vertices.end(), worldPos);

        particleEffect.unsetDirty();
    }

    // Compute particles positions
    const int maxLife = particleEffect.maxLife();

    for (int i = 0; i < particleEffect.m_lifes.size(); i++) {
        int &life = particleEffect.m_lifes[i];
        QVector3D &particlePos = geom.vertices[i];

        // Recycle particle
        if (life == 0) {
            life = maxLife;
            particlePos = worldPos;
        }
        // Sets the particle's starting position
        else if (life == maxLife) {
            particlePos.setX(m_radiusXRandDistrib(m_randEngine));
            particlePos.setZ(m_radiusZRandDistrib(m_randEngine));

            life--;
        }
        // or animate and decrase life
        else {
            life--;

            //TODO parametrize that by adding a std::function member attribute ?
            particlePos += particleEffect.speed() * particleEffect.direction() * dt;
        }
    }

    geom.isDirty = true;
}
