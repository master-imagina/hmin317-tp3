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


////////////////////// EntityxHook //////////////////////

EntityxHook::EntityxHook(SystemEngine &systemEngine, QObject *parent) :
    QObject(parent),
    m_systemEngine(systemEngine)
{
    m_systemEngine.registerSystem<ComponentAddedHook<Transform>>(*this);
    m_systemEngine.registerSystem<ComponentAddedHook<Geometry>>(*this);
    m_systemEngine.registerSystem<ComponentAddedHook<Material>>(*this);
    m_systemEngine.registerSystem<ComponentAddedHook<ParticleEffect>>(*this);
    m_systemEngine.registerSystem<ComponentAddedHook<Mesh>>(*this);
    m_systemEngine.registerSystem<ComponentAddedHook<Light>>(*this);
    m_systemEngine.registerSystem<ComponentAddedHook<Camera>>(*this);
    m_systemEngine.registerSystem<ComponentAddedHook<Keyboard>>(*this);
    m_systemEngine.registerSystem<ComponentAddedHook<Script>>(*this);

    m_systemEngine.configure();
}
