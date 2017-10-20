#include "particlesystem.h"

ParticleSystem::ParticleSystem()
{
    timeActualParticles = 0;
    timeActualDuration = 0;

    enable = true;
    emission = true;
}

float getRand(float n){
    return ((float)(rand()%1000)) / 1000.0 * n;
}

float getRand(float min,float max){
    return ((float)(rand()%1000)) / 1000.0 * (max-min) + min;
}

void ParticleSystem::Run(int t_ms){

    if(!enable)
        return;

    Draw::pushMatrix();

    timeActualParticles += t_ms;
    timeActualDuration += t_ms;
    timeBetweenTwoParticles = duration / nb_particles;

    for(int i=0;i<(int)particulesPosition.size();i++){
        particulesPosition[i].second -= t_ms;
        particulesPosition[i].first -= QVector3D(0,gravity,0) * (float)t_ms / 1000.0;

        if(particulesPosition[i].second <= 0){
            particulesPosition.erase(particulesPosition.begin()+i);
            i--;
        } else {

            cameraDirection = particulesPosition[i].first - cameraPosition;

            //Draw_3D::Cube(particulesPosition[i].first,0.1);
            Draw_3D::Particle(particulesPosition[i].first,cameraDirection ,0.1);
            //Draw_3D::Particle(QVector3D(0,0,0),QVector3D(0,1,0),1);
        }
    }

    if(emission && timeActualParticles >= timeBetweenTwoParticles && (timeActualDuration < duration || loop) ){
        timeActualParticles -= timeBetweenTwoParticles;

        particulesPosition.push_back( pair<QVector3D,int>(
                                          position + QVector3D(
                                            getRand(-emissionSphere.x(),emissionSphere.x()),
                                            getRand(-emissionSphere.y(),emissionSphere.y()),
                                            getRand(-emissionSphere.z(),emissionSphere.z())),
                                          lifetime_ms) );
    }

    Draw::popMatrix();

}
