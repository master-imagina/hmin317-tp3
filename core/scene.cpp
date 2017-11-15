#include "scene.h"


Scene::Scene() :
    entities(events)
{}

entityx::Entity Scene::createEntity()
{
    entityx::Entity ret = entities.create();

    Q_EMIT entityAdded(ret);

    return ret;
}
