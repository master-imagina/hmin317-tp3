#include "particleeffect.h"

#include <QDataStream>


ParticleEffect::ParticleEffect() :
    m_isDirty(true),
    m_count(50),
    m_maxLife(100),
    m_radius(50.f),
    m_direction({0.f, 1.f, 0.f}),
    m_speed(0.4f),
    m_particleSize(4.f),
    m_spawnRate(100),
    m_lifes()
{
    resetCount();
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

float ParticleEffect::radius() const
{
    return m_radius;
}

void ParticleEffect::setRadius(float radius)
{
    if (m_radius != radius) {
        m_radius = radius;

        setDirty();
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

float ParticleEffect::particleSize() const
{
    return m_particleSize;
}

void ParticleEffect::setParticleSize(float particleSize)
{
    if (m_particleSize != particleSize) {
        m_particleSize = particleSize;
    }
}

int ParticleEffect::spawnRate() const
{
    return m_spawnRate;
}

void ParticleEffect::setSpawnRate(int rate)
{
    if (m_spawnRate != rate) {
        m_spawnRate = rate;

        setDirty();
    }
}

bool ParticleEffect::isDirty() const
{
    return m_isDirty;
}

void ParticleEffect::setDirty()
{
    m_isDirty = true;
}

void ParticleEffect::unsetDirty()
{
    m_isDirty = false;
}

void ParticleEffect::resetCount()
{
    m_lifes.resize(m_count, m_maxLife);

    setDirty();
}


QDataStream &operator<<(QDataStream &os, const ParticleEffect &effect)
{
    os << effect.count()
       << effect.maxLife()
       << effect.radius()
       << effect.direction()
       << effect.speed()
       << effect.particleSize()
       << effect.spawnRate();
}

QDataStream &operator>>(QDataStream &os, ParticleEffect &effect)
{
    int count = 0;
    int maxLife = 0;
    float radius = 0.f;
    QVector3D direction;
    float speed = 0.f;
    float particleSize = 0.f;
    int spawnRate = 0;

    os >> count;
    os >> maxLife;
    os >> radius;
    os >> direction;
    os >> speed;
    os >> particleSize;
    os >> spawnRate;

    effect.setCount(count);
    effect.setMaxLife(maxLife);
    effect.setRadius(radius);
    effect.setDirection(direction);
    effect.setSpeed(speed);
    effect.setParticleSize(particleSize);
    effect.setSpawnRate(spawnRate);
}
