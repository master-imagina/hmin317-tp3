#include "particle.h"
#include <iostream>

Particle::Particle()
    : _position(QVector3D(0.0f, 0.0f, 0.0f))
    , _color(QVector4D(1.0f, 1.0f, 1.0f, 1.0f))
    , _life(-1000.0f)
    , _velocity(QVector3D(0.0f, 0.0f, 0.0f))
    , lastTime(0)
    , _size(5.0f)
{
    _originalSize = _size;
    _originalLife = _life;
}

Particle::Particle(QVector3D pos, QVector4D color, float theta, float phi, float speed, int life, float size)
    : _position(pos)
    , _color(color)
    , _life(life)
    , lastTime(0)
    , _size(size)
{
    float thetaR = qDegreesToRadians(theta);
    float phiR = qDegreesToRadians(phi);
    float x = speed * sin(phiR) * cos(thetaR);
    float z = speed * sin(phiR) * sin(thetaR);
    float y = speed * cos(phiR);
    _velocity = QVector3D(x, y, z);
    _originalSize = _size;
    _originalLife = _life;
}

void Particle::update() {
    double delta = 0;
    if(lastTime == 0) {
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    } else {
        delta = QTime::currentTime().msecsSinceStartOfDay() - lastTime ;
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    }
    _life -= delta;
    if(_life > 0) {
        _position += (_velocity * (delta / 1000.0f));
        float ageRatio = _life / _originalLife;
        if(_size > 1.5f) {
            _size = _originalSize * ageRatio;
        }
       // _color.setW(ageRatio + 0.2f);
    }
}

QVector3D Particle::getPosition() {
    return _position;
}

QVector4D Particle::getPosSize() {
    return QVector4D(_position.x(), _position.y(), _position.z(), _size);
}

QVector4D Particle::getColor() {
    return _color;
}

bool Particle::isAlive() {
    return _life > 0.0f;
}
