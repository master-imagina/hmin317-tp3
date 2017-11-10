#ifndef PARTICULESENGINE_H
#define PARTICULESENGINE_H

#include <QVector3D>
#include <QOpenGLWidget>

class particulesEngine
{
public:
    int maxParticle = 10000;

    QVector3D pos, speed;

    particulesEngine();
    int findUnusedParticle(std::vector<particulesEngine> &particleContainer, int lastUsedParticle = 0);
    void sortParticles(std::vector<particulesEngine> &particleContainer);
};

#endif // PARTICULESENGINE_H
