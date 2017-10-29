#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>
#include <QVector4D>
#include <QtMath>
#include <QTime>

class Particle
{
public:
    Particle();
    Particle(QVector3D pos, QVector4D color, float theta, float phi, float speed, int life = 5000, float size = 5.0f);
    void update();
    QVector3D getPosition();
    QVector4D getPosSize();
    QVector4D getColor();
    bool isAlive();
private:
    // _position: position of particle
    // _color : color of particle
    // _velocity :  velocity of the particle
    QVector3D _position, _velocity;
    QVector4D _color;
    float _life, _size, _originalLife, _originalSize;
    double lastTime;
};

#endif // PARTICLE_H
