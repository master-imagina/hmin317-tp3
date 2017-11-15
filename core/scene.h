#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include "3rdparty/entityx/Event.h"
#include "3rdparty/entityx/Entity.h"


class Scene : public QObject
{
    Q_OBJECT

public:
    Scene();
    ~Scene() = default;

    entityx::Entity createEntity();

/* consider private */
    entityx::EventManager events;
    entityx::EntityManager entities;

Q_SIGNALS:
    void entityAdded(entityx::Entity entity);
};

#endif // SCENE_H
