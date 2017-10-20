#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include <random>
#include <vector>

#include <QVector3D>


class ParticleEffect
{
public:
    ParticleEffect();
    ParticleEffect(const QVector3D &worldPos, int count, unsigned int maxLife);

    QVector3D worldPos() const;
    void setWorldPos(const QVector3D &worldPos);

    float radius() const;
    void setRadius(float radius);

    QVector3D direction() const;
    void setDirection(const QVector3D &direction);

    float speed() const;
    void setSpeed(float speed);

    float particlesSize() const;
    void setParticlesSize(float particlesSize);

    void live(float dt);

    const std::vector<QVector3D> &worldPositions() const;

private:
    void resetRadiusRandDistribs();

private:
    std::random_device m_randDevice;
    std::default_random_engine m_randEngine;

    std::uniform_real_distribution<float> m_radiusXRandDistrib;
    std::uniform_real_distribution<float> m_radiusZRandDistrib;

    int m_count;
    unsigned int m_maxLife;

    QVector3D m_worldPos;
    float m_radius;
    QVector3D m_direction;
    float m_speed;
    //TODO float m_count; (adapt m_worldPositions and m_lifes)
    float m_particlesSize;

    std::vector<QVector3D> m_worldPositions;
    std::vector<unsigned int> m_lifes;
};

#endif // PARTICLEEFFECT_H
