#ifndef SCENE_H
#define SCENE_H

#include "3rdparty/entityx/Event.h"
#include "3rdparty/entityx/Entity.h"


class Scene
{
public:
    Scene();
    ~Scene() = default;

    entityx::Entity createEntity();

/* consider private */
    entityx::EventManager events;
    entityx::EntityManager entities;
};

#endif // SCENE_H
