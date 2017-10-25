#include "particle.h"
#include <iostream>



Particle::Particle(QVector3D position,QVector3D velocity,QVector4D color,short lifeTime,float size,float gravity) :
    position(position),
    velocity(velocity),
    color(color),
    lifeTime(lifeTime),
    size(size),
    gravity(gravity)
{
}

Particle::~Particle(){

}



short Particle::getLifeTime(){
    return lifeTime;
}

QVector3D Particle::getPosition()const{
    return position;
}

void Particle::update(){
    position -= velocity*gravity;
    lifeTime--;
}
