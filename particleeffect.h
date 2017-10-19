#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include <vector>

#include <QVector3D>


class ParticleEffect
{
public:
    ParticleEffect(const QVector3D &worldPos, int count, unsigned int life);

    QVector3D worldPos() const;
    void setWorldPos(const QVector3D &worldPos);

    QVector3D direction() const;
    void setDirection(const QVector3D &direction);

    float speed() const;
    void setSpeed(float speed);

    void live(float dt);

private:
    int m_count;
    unsigned int m_life;

    QVector3D m_worldPos;
    QVector3D m_direction;
    float m_speed;

    std::vector<QVector3D> m_worldPositions;
    std::vector<unsigned int> m_lifes;
};

#endif // PARTICLEEFFECT_H
