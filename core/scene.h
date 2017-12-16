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
    void removeEntity(entityx::Entity entity);

    std::vector<entityx::Entity> entityCache() const;
    std::size_t count() const;

    void clear();

    /* consider private */
    entityx::EventManager m_eventManager;
    entityx::EntityManager m_entityManager;

Q_SIGNALS:
    void entityAdded(entityx::Entity entity);
    void entityRemoved(entityx::Entity entity);
    void cleared();

private:
    std::vector<entityx::Entity> m_entityCache;
};


Q_DECLARE_METATYPE(entityx::Entity)
Q_DECLARE_METATYPE(entityx::Entity::Id)

QDataStream &operator<<(QDataStream &os, const entityx::Entity::Id &entityId);
QDataStream &operator>>(QDataStream &os, entityx::Entity::Id &entityId);


#endif // SCENE_H
