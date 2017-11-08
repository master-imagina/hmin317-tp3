#ifndef PARTICLE_H
#define PARTICLE_H
#include <QVector3D>
#include <QVector4D>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QTimer>

class Particle
{
public:
    Particle(QVector3D position, QVector3D velocity,
             QVector4D color, short lifeTime, float size, float gravity);
    void update();
    ~Particle();
    short getLifeTime();
    QVector3D getPosition()const;
private :

    QVector3D position;
    QVector3D velocity;
    QVector4D color;
    float size;
    short lifeTime;
    float gravity;

};

#endif // PARTICLE_H
