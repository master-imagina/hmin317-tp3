#ifndef PARTICLEEFFECT_QUICK_H
#define PARTICLEEFFECT_QUICK_H

#include <QVector3D>

#include "3rdparty/entityx/Entity.h"

class Scene;


void createParticleEffect(entityx::Entity entity,
                          const QVector3D &direction = QVector3D(0, 1, 0),
                          int count = 50,
                          int maxLife = 100,
                          float radius = 50.f,
                          float speed = 0.4f,
                          float size = 4.f,
                          int spawnRate = 100);

#endif // PARTICLEEFFECT_QUICK_H
