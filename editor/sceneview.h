#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <unordered_map>

#include <entityx/Entity.h>

#include "editor/gui/pane.h"

class QTreeWidget;
class QTreeWidgetItem;

class Scene;


class SceneView : public ScrollablePane
{
    Q_OBJECT

public:
    SceneView(Scene &scene, QWidget *parent = nullptr);
    ~SceneView() = default;

Q_SIGNALS:
    void entityItemSelected(entityx::Entity entity);

private:
    void createToolbar();
    void createConnections();

private:
    void onEntityAdded(entityx::Entity entity);
    void onEntityRemoved(entityx::Entity entity);
    void onSceneCleared();

private:
    Scene &m_scene;

    QTreeWidget *m_entityTreeView;

    std::unordered_map<QTreeWidgetItem *, entityx::Entity> m_itemToEntity;
};

#endif // SCENEVIEW_H
