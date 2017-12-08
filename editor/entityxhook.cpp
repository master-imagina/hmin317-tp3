#include "entityxhook.h"

#include "core/systemengine.h"

#include "editor/componentview.h"

#include "extras/particles/particleeffect.h"

#include "input/keyboard.h"

#include "render/camera.h"
#include "render/light.h"
#include "render/mesh.h"
#include "render/transform.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"

#include "script/script.h"


////////////////////// Helpers //////////////////////

template <typename Component>
QString compName() = delete;

template <>
QString compName<Transform>()
{
    return "transform";
}

template <>
QString compName<Geometry>()
{
    return "geometry";
}

template <>
QString compName<Material>()
{
    return "material";
}

template <>
QString compName<ParticleEffect>()
{
    return "particleeffect";
}

template <>
QString compName<Mesh>()
{
    return "mesh";
}

template <>
QString compName<Light>()
{
    return "light";
}

template <>
QString compName<Camera>()
{
    return "camera";
}

template <>
QString compName<Keyboard>()
{
    return "keyboard";
}

template <>
QString compName<Script>()
{
    return "script";
}


////////////////////// ComponentAddedHook //////////////////////

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
        Q_EMIT m_entityxHook.entityComponentAdded(event.entity,
                                                  compName<C>());
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


////////////////////// ComponentRemovedHook //////////////////////

template <typename C>
class ComponentRemovedHook :
        public entityx::System<ComponentRemovedHook<C>>,
        public entityx::Receiver<ComponentRemovedHook<C>>
{
public:
    ComponentRemovedHook(EntityxHook &entityxHook) :
        m_entityxHook(entityxHook)
    {}

    void receive(const entityx::ComponentRemovedEvent<C> &event)
    {
        Q_EMIT m_entityxHook.entityComponentRemoved(event.entity,
                                                    compName<C>());
    }

    void configure(entityx::EventManager &events) override
    {
        events.subscribe<entityx::ComponentRemovedEvent<C>>(*this);
    }

    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override {}
private:
    EntityxHook &m_entityxHook;
};


////////////////////// Helpers //////////////////////

template <typename Component>
void registerHookSystemsForComponent(EntityxHook &entityHook, SystemEngine &systemEngine)
{
    systemEngine.registerSystem<ComponentAddedHook<Component>>(entityHook);
    systemEngine.registerSystem<ComponentRemovedHook<Component>>(entityHook);
}


////////////////////// EntityxHook //////////////////////

EntityxHook::EntityxHook(SystemEngine &systemEngine, QObject *parent) :
    QObject(parent),
    m_systemEngine(systemEngine)
{
    registerHookSystemsForComponent<Transform>(*this, m_systemEngine);
    registerHookSystemsForComponent<Geometry>(*this, m_systemEngine);
    registerHookSystemsForComponent<Material>(*this, m_systemEngine);
    registerHookSystemsForComponent<ParticleEffect>(*this, m_systemEngine);
    registerHookSystemsForComponent<Mesh>(*this, m_systemEngine);
    registerHookSystemsForComponent<Light>(*this, m_systemEngine);
    registerHookSystemsForComponent<Camera>(*this, m_systemEngine);
    registerHookSystemsForComponent<Keyboard>(*this, m_systemEngine);
    registerHookSystemsForComponent<Script>(*this, m_systemEngine);

    m_systemEngine.configure();
}
