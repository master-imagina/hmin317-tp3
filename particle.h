#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>
#include <QVector4D>
#include <QtMath>
#include <QTime>
#include <random>

typedef enum class ParticleType {
    Snow,
    Rain
} ParticleType;

class Particle
{
public:

    Particle();
    Particle(QVector3D pos, QVector4D color, float theta, float phi, float speed, int life = 5000, float size = 5.0f);
    void update();
    bool isAlive();
    QVector3D getPosition();
    QVector4D getPositionAndSize();
    QVector4D getColor();
    static Particle generateSeasonParticle(ParticleType type);

private:

    QVector3D pos, velocity;
    QVector4D color;
    float life, speed, size, originalLife, originalSize;
    double lastTime;
    static Particle generateSnowParticle();
    static Particle generateRainParticle();

};

#endif // PARTICLE_H
