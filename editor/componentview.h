#ifndef COMPONENTVIEW_H
#define COMPONENTVIEW_H

#include <functional>
#include <map>

#include "editor/gui/pane.h"

#include "3rdparty/entityx/Entity.h"
#include "3rdparty/entityx/System.h"

class QVBoxLayout;

class SceneView;


class ComponentView : public Pane
{
    Q_OBJECT

public:
    using ComponentEditorCreator = std::function<void (entityx::Entity, QWidget *, QVBoxLayout *)>;

    ComponentView(SceneView *sceneView, QWidget *parent = nullptr);
    ~ComponentView() = default;

    void addComponentEditorCreator(ComponentEditorCreator creator);

    void setCurrentEntity(entityx::Entity entity);

private:
    void createConnections();

private:
    SceneView *m_theSceneView;

    QWidget *m_mainWidget;
    QVBoxLayout *m_mainLayout;

    entityx::Entity m_currentEntity;

    std::vector<ComponentEditorCreator> m_componentEditorCreators;
};


template <typename C>
class ComponentAddedHook : public entityx::System<ComponentAddedHook<C>>, public entityx::Receiver<ComponentAddedHook<C>>
{
public:
    ComponentAddedHook(ComponentView &componentView) :
        m_componentView(componentView)
    {}

    void receive(const entityx::ComponentAddedEvent<C> &event)
    {
        m_componentView.setCurrentEntity(event.entity);
    }

    void configure(entityx::EventManager &events) override
    {
        events.subscribe<entityx::ComponentAddedEvent<C>>(*this);
    }

    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override {}
private:
    ComponentView &m_componentView;
};

#endif // COMPONENTVIEW_H
