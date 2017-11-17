#ifndef PARTICLEENGINE_H
#define PARTICLEENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <vector>
#include <random>
#include "particle.h"

#define MAX_PARTICULES 10000

struct ParticleData {
    QVector4D posSize, color;
    ParticleData(QVector4D ps, QVector4D c)
        : posSize(ps)
        , color(c)
    {}
};

class ParticleEngine : QOpenGLFunctions
{
public:
    ParticleEngine(ParticleType t);
    ~ParticleEngine();

    void drawParticles(QOpenGLShaderProgram *program);
    void updateParticles();
    void generateParticles(float seuilGeneration);
    int findUnusedParticles();
    void setParticleType(ParticleType t);

private:
    Particle particleContainer[MAX_PARTICULES];
    QOpenGLBuffer arrayBuffer;
    std::vector<ParticleData> particlesData;
    int lastUsedParticles;
    double lastTime;
    ParticleType type;

    void initParticles();
};

#endif // PARTICLEENGINE_H
