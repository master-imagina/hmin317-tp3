#include "particleeffect.h"
#include <QDebug>


namespace {

const int m_countDefaultValue = 20;
const uint32 m_maxLifeDefaultValue = 50;
const float m_radiusDefaultValue = 50.f;
const QVector3D m_directionDefaultValue = {0, 1, 0};
const float m_speedDefaultValue = 0.4f;
const float m_particlesSizeDefaultValue = 4.f;

} // anon namespace


ParticleEffect::ParticleEffect() :
    m_randDevice(),
    m_randEngine(m_randDevice()),
    m_radiusXRandDistrib(),
    m_radiusZRandDistrib(),
    m_count(m_countDefaultValue),
    m_maxLife(m_maxLifeDefaultValue),
    m_worldPos(),
    m_radius(m_radiusDefaultValue),
    m_direction(m_directionDefaultValue),
    m_speed(m_speedDefaultValue),
    m_particlesSize(m_particlesSizeDefaultValue),
    m_geometry(std::make_unique<Geometry>()),
    m_lifes(m_count, m_maxLife)
{
    resetRadiusRandDistribs();

    m_geometry->vertices.resize(m_count);

    std::fill(m_geometry->vertices.begin(),
              m_geometry->vertices.end(),
              m_worldPos);
}

ParticleEffect::ParticleEffect(const QVector3D &pos, int count, uint32 maxLife) :
    m_randDevice(),
    m_randEngine(m_randDevice()),
    m_radiusXRandDistrib(),
    m_radiusZRandDistrib(),
    m_count(count),
    m_maxLife(maxLife),
    m_worldPos(pos),
    m_radius(m_radiusDefaultValue),
    m_direction(m_directionDefaultValue),
    m_speed(m_speedDefaultValue),
    m_particlesSize(m_particlesSizeDefaultValue),
    m_geometry(std::make_unique<Geometry>()),
    m_lifes(m_count, m_maxLife)
{
    resetRadiusRandDistribs();

    m_geometry->vertices.resize(m_count);

    std::fill(m_geometry->vertices.begin(),
              m_geometry->vertices.end(),
              m_worldPos);

    m_geometry->primitiveCount = m_count;
}

QVector3D ParticleEffect::worldPos() const
{
    return m_worldPos;
}

void ParticleEffect::setWorldPos(const QVector3D &pos)
{
    m_worldPos = pos;
}

float ParticleEffect::radius() const
{
    return m_radius;
}

void ParticleEffect::setRadius(float radius)
{
    if (m_radius != radius) {
        m_radius = radius;

        resetRadiusRandDistribs();
    }
}

QVector3D ParticleEffect::direction() const
{
    return m_direction;
}

void ParticleEffect::setDirection(const QVector3D &direction)
{
    m_direction = direction.normalized();
}

float ParticleEffect::speed() const
{
    return m_speed;
}

void ParticleEffect::setSpeed(float speed)
{
    m_speed = speed;
}

float ParticleEffect::particlesSize() const
{
    return m_particlesSize;
}

void ParticleEffect::setParticlesSize(float particlesSize)
{
    m_particlesSize = particlesSize;
}

void ParticleEffect::live(float dt)
{
    for (int i = 0; i < m_lifes.size(); i++) {
        uint32 &life = m_lifes[i];
        QVector3D &particlePos = m_geometry->vertices[i];

        // Recycle particle
        if (life == 0) {
            life = m_maxLife;
            particlePos = m_worldPos;
        }
        // Sets the particle's starting position
        else if (life == m_maxLife) {
            particlePos.setX(m_radiusXRandDistrib(m_randEngine));
            particlePos.setZ(m_radiusZRandDistrib(m_randEngine));

            life--;
        }
        // or animate and decrase life
        else {
            life--;

            //TODO parametrize that by adding a std::function member attribute ?
            particlePos += m_speed * direction() * dt;
        }
    }

    m_geometry->isDirty = true;
}

Geometry *ParticleEffect::geometry() const
{
    return m_geometry.get();
}

void ParticleEffect::resetRadiusRandDistribs()
{
    const float worldPosX = m_worldPos.x();
    const float worldPosZ = m_worldPos.z();

    m_radiusXRandDistrib = std::uniform_real_distribution<float>
            (worldPosX - m_radius, worldPosX + m_radius);

    m_radiusZRandDistrib = std::uniform_real_distribution<float>
            (worldPosZ - m_radius, worldPosZ + m_radius);
}
