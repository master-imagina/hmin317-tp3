#ifndef PARTICULESENGINE_H
#define PARTICULESENGINE_H

#include <QVector3D>
#include <QOpenGLWidget>

class particulesEngine
{
public:
    /*const*/ int maxParticle = 1000000;

    QVector3D pos, speed;
    float r, g, b, a;
    float size, angle, weight;
    float life;
    float cameraDistance;

    bool operator<(particulesEngine const &p) const;

    particulesEngine();
    int findUnusedParticle(std::vector<particulesEngine> &particleContainer, int lastUsedParticle = 0);
    void sortParticles(std::vector<particulesEngine> &particleContainer);
};

#endif // PARTICULESENGINE_H
