#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>
#include <QVector4D>

typedef enum class ParticleType {
    Snow,
    Rain
} ParticleType;

class Particle
{
public:
    int life;
    QVector3D pos, velocity;
    QVector4D color;
    float speed, size, oriLife, oriSize;

    Particle();
    Particle(QVector3D pos, QVector4D color, float theta, float phi, );
    void update();
    bool isAlive();
    static Particle generateSeasonParticle(ParticleType season);
};

#endif // PARTICLE_H
