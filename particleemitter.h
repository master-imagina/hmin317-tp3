#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <QVector>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "particle.h"

class ParticleEmitter : protected QOpenGLFunctions
{
public:
    ParticleEmitter(float power);
    virtual ~ParticleEmitter();
    void Update(float DT);
    void Render(QOpenGLShaderProgram *);

private:
    QVector<Particle *> Particles;
    float currentTime;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    float power;
};

#endif // PARTICLEEMITTER_H
