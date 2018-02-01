#include "scene.h"

#include <QDataStream>


Scene::Scene() :
    m_eventManager(),
    m_entityManager(m_eventManager),
    m_entityCache()
{}

entityx::Entity Scene::createEntity()
{
    entityx::Entity ret = m_entityManager.create();

    m_entityCache.emplace_back(ret);

    Q_EMIT entityAdded(ret);

    return ret;
}

void Scene::removeEntity(entityx::Entity entity)
{
    Q_EMIT entityRemoved(entity);

    auto cacheIt = std::find(m_entityCache.begin(), m_entityCache.end(), entity);
    m_entityCache.erase(cacheIt);

    m_entityManager.destroy(entity.id());
}

std::vector<entityx::Entity> Scene::entityCache() const
{
    return m_entityCache;
}

std::size_t Scene::count() const
{
    return m_entityManager.size();
}

void Scene::clear()
{
    m_entityManager.reset();
    m_entityCache.clear();

    Q_EMIT cleared();
}


////////////////////// Functions //////////////////////

QDataStream &operator<<(QDataStream &os, const entityx::Entity::Id &entityId)
{
    os << (quint64) entityId.id();
}

QDataStream &operator>>(QDataStream &os, entityx::Entity::Id &entityId)
{
    quint64 rawEntityId = 0;
    os >> rawEntityId;

    entityId = entityx::Entity::Id(rawEntityId);
}
