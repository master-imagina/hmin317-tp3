#include "particleeffect.h"

#include "render/geometry/geometry.h"


ParticleEffect::ParticleEffect() :
    m_randDevice(),
    m_randEngine(m_randDevice()),
    m_radiusXRandDistrib(),
    m_radiusZRandDistrib(),
    m_count(50),
    m_maxLife(100),
    m_worldPos({0.f, 0.f, 0.f}),
    m_radius(50.f),
    m_direction({0.f, 1.f, 0.f}),
    m_speed(0.4f),
    m_particlesSize(4.f),
    m_geometry(std::make_unique<Geometry>()),
    m_lifes()
{
    resetCount();
    resetRadiusRandDistribs();
}

int ParticleEffect::count() const
{
    return m_count;
}

void ParticleEffect::setCount(int count)
{
    if (m_count != count) {
        m_count = count;

        resetCount();
    }
}

int ParticleEffect::maxLife() const
{
    return m_maxLife;
}

void ParticleEffect::setMaxLife(int maxLife)
{
    if (m_maxLife != maxLife) {
        m_maxLife = maxLife;
    }
}

QVector3D ParticleEffect::worldPos() const
{
    return m_worldPos;
}

void ParticleEffect::setWorldPos(const QVector3D &pos)
{
    if (m_worldPos != pos) {
        m_worldPos = pos;

        resetRadiusRandDistribs();
    }
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
    if (m_direction != direction) {
        m_direction = direction.normalized();
    }
}

float ParticleEffect::speed() const
{
    return m_speed;
}

void ParticleEffect::setSpeed(float speed)
{
    if (m_speed != speed) {
        m_speed = speed;
    }
}

float ParticleEffect::particlesSize() const
{
    return m_particlesSize;
}

void ParticleEffect::setParticlesSize(float particlesSize)
{
    if (m_particlesSize != particlesSize) {
        m_particlesSize = particlesSize;
    }
}

void ParticleEffect::live(float dt)
{
    for (int i = 0; i < m_lifes.size(); i++) {
        int &life = m_lifes[i];
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
            particlePos += m_speed * m_direction * dt;
        }
    }

    m_geometry->isDirty = true;
}

Geometry *ParticleEffect::geometry() const
{
    return m_geometry.get();
}

void ParticleEffect::resetCount()
{
    m_lifes.resize(m_count, m_maxLife);

    m_geometry->vertices.resize(m_count);
    m_geometry->primitiveCount = m_count;

    std::fill(m_geometry->vertices.begin(),
              m_geometry->vertices.end(),
              m_worldPos);
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
