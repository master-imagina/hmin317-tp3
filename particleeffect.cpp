#include "particleeffect.h"


ParticleEffect::ParticleEffect(const QVector3D &pos, int count, unsigned int life) :
    m_count(count),
    m_life(life),
    m_worldPos(pos),
    m_direction(0, 1, 0),
    m_speed(4.f),
    m_worldPositions(count),
    m_lifes(count)
{}

QVector3D ParticleEffect::direction() const
{
    return m_direction;
}

void ParticleEffect::setDirection(const QVector3D &direction)
{
    m_direction = direction.normalized();
}

void ParticleEffect::live(float dt)
{
    for (int i = 0; i < m_lifes.size(); i++) {
        unsigned int &life = m_lifes[i];
        QVector3D &particlePos = m_worldPositions[i];

        // Recycle particle
        if (life == 0) {
            life = m_life;
            particlePos = m_worldPos;
        }
        // or animate and decrase life
        else if (life < 0) {
            life--;

            particlePos += m_speed * direction() * dt;
        }
    }
}

float ParticleEffect::speed() const
{
    return m_speed;
}

void ParticleEffect::setSpeed(float speed)
{
    m_speed = speed;
}

QVector3D ParticleEffect::worldPos() const
{
    return m_worldPos;
}

void ParticleEffect::setWorldPos(const QVector3D &pos)
{
    m_worldPos = pos;
}
