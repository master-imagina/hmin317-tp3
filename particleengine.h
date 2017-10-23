#ifndef PARTICLEENGINE_H
#define PARTICLEENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <vector>
#include "particle.h"

#define MAX_PARTICULES 2

class ParticleEngine : QOpenGLFunctions
{
public:
    ParticleEngine();
    ~ParticleEngine();

    void drawParticles(QOpenGLShaderProgram *program);

private:
    Particle particleContainer[MAX_PARTICULES];
    QOpenGLBuffer arrayBuffer;
    //QVector3D *particlePosData;
    std::vector<QVector3D> particlePosData;
    int countParticules;

    void initParticles();

};

#endif // PARTICLEENGINE_H
