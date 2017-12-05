#ifndef ENTITYXHOOK_H
#define ENTITYXHOOK_H

#include <QObject>

#include "3rdparty/entityx/System.h"

class SystemEngine;


class EntityxHook : public QObject
{
    Q_OBJECT

public:
    EntityxHook(SystemEngine &systemEngine, QObject *parent = nullptr);
    ~EntityxHook() = default;

Q_SIGNALS:
    void entityComponentAdded(entityx::Entity entity);

private:
    SystemEngine &m_systemEngine;
};


template <typename C>
class ComponentAddedHook :
        public entityx::System<ComponentAddedHook<C>>,
        public entityx::Receiver<ComponentAddedHook<C>>
{
public:
    ComponentAddedHook(EntityxHook &entityxHook) :
        m_entityxHook(entityxHook)
    {}

    void receive(const entityx::ComponentAddedEvent<C> &event)
    {
        Q_EMIT m_entityxHook.entityComponentAdded(event.entity);
    }

    void configure(entityx::EventManager &events) override
    {
        events.subscribe<entityx::ComponentAddedEvent<C>>(*this);
    }

    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override {}
private:
    EntityxHook &m_entityxHook;
};

#endif // ENTITYXHOOK_H
