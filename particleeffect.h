#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include <memory>
#include <random>
#include <vector>

#include <QVector3D>

#include "geometry.h"


class ParticleEffect
{
public:
    ParticleEffect();

    int count() const;
    void setCount(int count);

    int maxLife() const;
    void setMaxLife(int maxLife);

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

    Geometry *geometry() const;

private:
    void resetCount();
    void resetRadiusRandDistribs();

private:
    std::random_device m_randDevice;
    std::default_random_engine m_randEngine;

    std::uniform_real_distribution<float> m_radiusXRandDistrib;
    std::uniform_real_distribution<float> m_radiusZRandDistrib;

    int m_count;
    int m_maxLife;

    QVector3D m_worldPos;
    float m_radius;
    QVector3D m_direction;
    float m_speed;
    float m_particlesSize;

    std::unique_ptr<Geometry> m_geometry;
    std::vector<int> m_lifes;
};

#endif // PARTICLEEFFECT_H
