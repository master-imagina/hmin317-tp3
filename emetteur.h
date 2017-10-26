#ifndef EMETTEUR_H
#define EMETTEUR_H

#include "particle.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLWidget>

class Particle;

class Emetteur : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Emetteur(int nb_particles);
    virtual ~Emetteur();

    void drawParticles(QOpenGLShaderProgram *program);

    bool active;

private :
    QList <Particle *> particles;

    int nb_particles;
};

#endif // EMETTEUR_H
