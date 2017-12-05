#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "core/aliases_memory.h"
#include "core/scene.h"

#include "editor/panemanager.h"

#include "render/camera.h"

class AssetManagerView;
class CameraControls;
class ComponentView;
class EntityxHook;
class GameWidget;
class PaneManager;
class ProjectManager;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    void createMenus();
    void initPanes();
    void createConnections();

    void createDefaultComponentEditorCreators(ComponentView *componentView);

    void enterPlayMode();
    void leavePlayMode();

private:
    uptr<PaneManager> m_paneManager;
    QMenu *m_openViewPaneMenu;

    AssetManagerView *m_assetManagerView;
    ProjectManager *m_projectManager;

    Camera m_freeCamera;
    Scene m_scene;

    QWidget *m_centralWidget;
    GameWidget *m_gameWidget;

    EntityxHook *m_entityxHook;

    CameraControls *m_cameraControls;

    bool m_inPlayMode;
};

#endif // MAINWINDOW_H
