#include "scene.h"


Scene::Scene() :
    entities(events)
{}

entityx::Entity Scene::createEntity()
{
    return entities.create();
}
