#ifndef COMPONENTVIEW_H
#define COMPONENTVIEW_H

#include "core/aliases_memory.h"

#include "editor/gui/pane.h"

#include "editor/icomponentuihandler.h"

#include "3rdparty/entityx/Entity.h"
#include "3rdparty/entityx/System.h"

class QMenu;
class QVBoxLayout;

class ProjectManager;
class SceneView;


class ComponentView : public ScrollablePane
{
    Q_OBJECT

public:
    ComponentView(SceneView *sceneView, ProjectManager *projectManager,
                  QWidget *parent = nullptr);
    ~ComponentView() = default;

    template <class Handler>
    void registerComponentUiHandler()
    {
        m_componentUiHandlers.emplace_back(std::make_unique<Handler>());
    }

    void setCurrentEntity(entityx::Entity entity);

private:
    void createConnections();

    void updateComponentMenuForEntity();

private:
    SceneView *m_theSceneView;
    ProjectManager *m_theProjectManager;

    QWidget *m_mainWidget;
    QVBoxLayout *m_mainLayout;

    QMenu *m_addComponentMenu;

    entityx::Entity m_currentEntity;

    std::vector<uptr<IComponentUiHandler>> m_componentUiHandlers;
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
