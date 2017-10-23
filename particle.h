#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>
#include <QtMath>
#include <QTime>

class Particle
{
public:
    Particle();
    Particle(QVector3D pos, QVector3D color, float theta, float phi, float speed, int life = 3000.0f);
    void update();
    QVector3D getPosition();
private:
    // _position: position of particle
    // _color : color of particle
    // _velocity :  velocity of the particle
    QVector3D _position, _color, _velocity;
    float _life;
    double lastTime;
};

#endif // PARTICLE_H
