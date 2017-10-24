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
    void updateParticles();

private:
    Particle particleContainer[MAX_PARTICULES];
    QOpenGLBuffer arrayBuffer;
    std::vector<QVector4D> particlePosSizeData;

    void initParticles();

};

#endif // PARTICLEENGINE_H
