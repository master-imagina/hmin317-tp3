#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include <vector>

#include <QVector3D>


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

    bool isDirty() const;
    void setDirty();
    void unsetDirty();

private:
    void resetCount();

private:
    bool m_isDirty;

    int m_count;
    int m_maxLife;

    QVector3D m_worldPos;
    float m_radius;
    QVector3D m_direction;
    float m_speed;
    float m_particlesSize;

public:
    //TODO preallocate
    std::vector<int> m_lifes;
};

#endif // PARTICLEEFFECT_H
