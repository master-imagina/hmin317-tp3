#include "scene.h"


Scene::Scene() :
    events(),
    entities(events),
    m_entityCache()
{}

entityx::Entity Scene::createEntity()
{
    entityx::Entity ret = entities.create();

    m_entityCache.emplace_back(ret);

    Q_EMIT entityAdded(ret);

    return ret;
}

std::vector<entityx::Entity> Scene::entityCache() const
{
    return m_entityCache;
}

std::size_t Scene::count() const
{
    return entities.size();
}

void Scene::clear()
{
    entities.reset();
    m_entityCache.clear();

    Q_EMIT cleared();
}
