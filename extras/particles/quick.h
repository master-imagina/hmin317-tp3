#ifndef PARTICLEEFFECT_QUICK_H
#define PARTICLEEFFECT_QUICK_H

#include "3rdparty/entityx/Entity.h"

class QVector3D;

class Scene;


entityx::Entity createParticleEffect(Scene &scene,
                                     const QVector3D &worldPos,
                                     const QVector3D &direction,
                                     int count = 50,
                                     int maxLife = 100,
                                     float radius = 50.f,
                                     float speed = 0.4f,
                                     float size = 4.f);

#endif // PARTICLEEFFECT_QUICK_H
