#include "particleemitter.h"
#include <iostream>
ParticleEmitter::ParticleEmitter():
    position(QVector3D(0,100,0))
{

}

ParticleEmitter::ParticleEmitter(QVector3D position, int mapSize):
    position(position),
    mapSize(mapSize)
{
    heightMap = QImage(":/heightmap-3.png");
}

void ParticleEmitter::updateParticle(){

    std::vector<Particle>::size_type i = 0;
    glPointSize(3);
    glBegin(GL_POINTS);
    while ( i < particles.size() ) {
        particles[i].update();
        glVertex3f(particles[i].getPosition().x(), particles[i].getPosition().y(),particles[i].getPosition().z());
        if ( particles[i].getLifeTime() == 0 || collisionTest(particles[i]))  {
            particles.erase( particles.begin() + i );
        } else {
            ++i;
        }

    }
    glEnd();

}

bool ParticleEmitter::collisionTest(Particle &p){

    int i = p.getPosition().x() / 0.5 + 597/2;
    int j = p.getPosition().z() / 0.5 + 597/2;
    QRgb rgb = heightMap.pixel(i,j);
    rgb = qGray(rgb);
    float y = 35.0 * ((float)rgb/255);

    if(p.getPosition().y() < y){
        return true;
    }
    return false;
}
void ParticleEmitter::update(){
    if(particles.size() < 500000){
        int currentSize = particles.size() + 50;
        for(int i=particles.size() ; i < currentSize; ++i ){
            float x =  rand() % (mapSize + 1) - mapSize/2;
            float z =  rand() % (mapSize + 1) - mapSize/2;

            Particle p(QVector3D(x,position.y(),z),QVector3D(0,1,0),QVector4D(1,1,1,0),1000,1,1);
            particles.push_back(p);
        }
    }



}
