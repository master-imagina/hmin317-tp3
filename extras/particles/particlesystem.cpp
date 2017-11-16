#include "particlesystem.h"

#include <random>

#include "render/geometry/geometry.h"

#include "render/transform.h"

#include "particleeffect.h"


void ParticleSystem::update(entityx::EntityManager &entities,
                            entityx::EventManager &events,
                            double dt)
{
    entities.each<ParticleEffect, Geometry, Transform>(
                [dt] (entityx::Entity entity,
                ParticleEffect &particleEffect,
                Geometry &geom,
                Transform &transform) {
        liveParticles(entity, particleEffect, geom, transform, dt);
    });
}

void ParticleSystem::liveParticles(entityx::Entity entity,
                                   ParticleEffect &particleEffect,
                                   Geometry &geom,
                                   Transform &transform,
                                   double dt)
{
    const QVector3D worldPos = transform.translate();
    const float worldPosX = worldPos.x();
    const float worldPosZ = worldPos.z();

    const float radius = particleEffect.radius();

    static std::random_device m_randDevice;
    static std::default_random_engine m_randEngine(m_randDevice());

    std::uniform_real_distribution<float> m_radiusXRandDistrib(worldPosX - radius, worldPosX + radius);
    std::uniform_real_distribution<float> m_radiusZRandDistrib(worldPosZ - radius, worldPosZ + radius);

    if (particleEffect.isDirty()) {
        const int count = particleEffect.count();

        geom.vertices.resize(count);
        geom.primitiveCount = count;

        std::fill(geom.vertices.begin(), geom.vertices.end(), worldPos);
        std::fill(particleEffect.m_lifes.begin(), particleEffect.m_lifes.end(), particleEffect.maxLife());

        particleEffect.unsetDirty();
    }

    // Compute particles positions
    const int maxLife = particleEffect.maxLife();

    const int spawnMaxCount = particleEffect.count() * ((float)particleEffect.spawnRate() / 100);
    int spawnCounter = 0;

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

            if (spawnCounter < spawnMaxCount) {
                life--;

                spawnCounter++;
            }
        }
        // or animate and decrase life
        else {
            life--;

            //TODO parametrize that by adding a std::function member attribute ?
            particlePos += particleEffect.speed() * particleEffect.direction() * dt;
        }
    }

    spawnCounter = 0;

    geom.isDirty = true;
}
