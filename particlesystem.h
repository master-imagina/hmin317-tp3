#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "global.h"
#include "drawing.h"

class ParticleSystem
{
public:
    ParticleSystem();

    void Run(int);

    QVector3D position;
    QVector3D emissionSphere;
    QVector3D cameraDirection;
    QVector3D cameraPosition;

    int lifetime_ms;
    float gravity;

    float duration;
    int nb_particles;
    bool loop;

    bool enable;
    bool emission;

private:
    vector<pair<QVector3D,int> > particulesPosition;

    int timeBetweenTwoParticles;
    int timeActualParticles;
    int timeActualDuration;
};

#endif // PARTICLESYSTEM_H
