#ifndef PARTICLEENGINE_H
#define PARTICLEENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <vector>
#include <random>
#include "particle.h"

#define MAX_PARTICULES 10000

class ParticleEngine : QOpenGLFunctions
{
public:
    ParticleEngine();
    ~ParticleEngine();

    void drawParticles(QOpenGLShaderProgram *program);
    void updateParticles();
    void generateParticles();
    int findUnusedParticles();
    Particle generateSnowParticle();

private:
    Particle particleContainer[MAX_PARTICULES];
    QOpenGLBuffer arrayBuffer;
    std::vector<QVector4D> particlePosSizeData;
    int lastUsedParticles;
    double lastTime;
    std::random_device rd;
    std::mt19937 gen;
    void initParticles();

};

#endif // PARTICLEENGINE_H
