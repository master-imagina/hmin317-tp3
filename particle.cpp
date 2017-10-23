#include "particle.h"

Particle::Particle()
    : _position(QVector3D(0.0f, 0.0f, 0.0f))
    , _color(QVector3D(1.0f, 1.0f, 1.0f))
    , _life(3000.0f)
    , _velocity(QVector3D(0.0f, 0.0f, 0.0f))
    , lastTime(0)
{

}

Particle::Particle(QVector3D pos, QVector3D color, float theta, float phi, float speed, int life)
    : _position(pos)
    , _color(color)
    , _life(life)
    , lastTime(0)
{
    float thetaR = qDegreesToRadians(theta);
    float phiR = qDegreesToRadians(phi);
    float x = speed * sin(phiR) * cos(thetaR);
    float y = speed * sin(phiR) * sin(thetaR);
    float z = speed * cos(phiR);
    _velocity = QVector3D(x, y, z);
}

void Particle::update() {
    double delta = 0;
    if(lastTime == 0) {
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    } else {
        delta = QTime::currentTime().msecsSinceStartOfDay() - lastTime ;
    }
    _life -= delta;
    if(_life > 0) {
        _position += (_velocity * (delta / 1000.0f));
    }
}

QVector3D Particle::getPosition() {
    return _position;
}

