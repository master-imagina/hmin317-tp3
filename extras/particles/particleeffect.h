#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include <vector>

#include <QVector3D>

class QDataStream;


class ParticleEffect
{
public:
    ParticleEffect();

    int count() const;
    void setCount(int count);

    int maxLife() const;
    void setMaxLife(int maxLife);

    float radius() const;
    void setRadius(float radius);

    QVector3D direction() const;
    void setDirection(const QVector3D &direction);

    float speed() const;
    void setSpeed(float speed);

    float particleSize() const;
    void setParticleSize(float particleSize);

    int spawnRate() const;
    void setSpawnRate(int rate);

    bool isDirty() const;
    void setDirty();
    void unsetDirty();

private:
    void resetCount();

private:
    bool m_isDirty;

    int m_count;
    int m_maxLife;

    float m_radius;
    QVector3D m_direction;
    float m_speed;
    float m_particleSize;
    int m_spawnRate;

public:
    //TODO preallocate
    std::vector<int> m_lifes;
};


QDataStream &operator<<(QDataStream &os, const ParticleEffect &effect);
QDataStream &operator>>(QDataStream &os, ParticleEffect &effect);

#endif // PARTICLEEFFECT_H
