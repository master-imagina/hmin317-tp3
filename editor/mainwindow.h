#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "core/aliases_memory.h"
#include "core/scene.h"

#include "editor/panemanager.h"

#include "render/camera.h"

class AssetManagerView;
class ComponentView;
class GameWidget;
class PaneManager;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private Q_SLOTS:
    void buildCurrentProject();

private:
    void createMenus();
    void initPanes();

    void createDefaultComponentEditorCreators(ComponentView *componentView);

private:
    uptr<PaneManager> m_paneManager;
    QMenu *m_openViewPaneMenu;

    AssetManagerView *m_assetManagerView;

    Scene m_scene;
    Camera m_camera;
    GameWidget *m_gameWidget;
};

#endif // MAINWINDOW_H
