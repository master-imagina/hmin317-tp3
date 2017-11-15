#ifndef COMPONENTVIEW_H
#define COMPONENTVIEW_H

#include <functional>
#include <map>

#include "editor/gui/pane.h"

#include "3rdparty/entityx/Entity.h"

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

private:
    void createConnections();

private:
    void onEntityItemSelected(entityx::Entity entity);

private:
    SceneView *m_theSceneView;

    QWidget *m_mainWidget;
    QVBoxLayout *m_mainLayout;

    std::vector<ComponentEditorCreator> m_componentEditorCreators;
};

#endif // COMPONENTVIEW_H
