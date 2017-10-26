#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>
#include <QVector2D>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

struct ParticleData
{
    QVector3D position;
    QVector2D texCoord;
};

class Particle :  protected QOpenGLFunctions
{
public:
    Particle(int id);
    virtual ~Particle();

    void initParticles();

    void drawParticle(QOpenGLShaderProgram *program);

    int id;

    float lifeTime;

    bool active;

private:
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

    ParticleData *particleData;
};

#endif // PARTICLE_H
