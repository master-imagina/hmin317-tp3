#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <vector>
#include "particle.h"
#include <QImage>

class ParticleEmitter
{



public:
    ParticleEmitter();
    ParticleEmitter(QVector3D position, int mapSize);
    void updateParticle();
    void update();
    bool collisionTest(Particle &p);

private:
    QVector3D position;
    std::vector<Particle> particles;
    QImage heightMap;
    int mapSize;
};

#endif // PARTICLEEMITTER_H
