#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <map>

#include "editor/gui/pane.h"

#include "3rdparty/entityx/Entity.h"

class QTreeWidget;
class QTreeWidgetItem;

class Scene;


class SceneView : public Pane
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
    void onEntityItemActivated(entityx::Entity entity);

private:
    Scene &m_scene;

    QTreeWidget *m_entityTreeView;

    std::map<QTreeWidgetItem *, entityx::Entity> m_itemToEntity;
};

#endif // SCENEVIEW_H
