#include "mainwindow.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>

#include "extras/gamewidget.h"

#include "editor/gui/fpswidgets.h"

#include "editor/assetmanagerview.h"
#include "editor/componentuihandlers.h"
#include "editor/componentview.h"
#include "editor/hooksystems.h"
#include "editor/panemanager.h"
#include "editor/particleeditor.h"
#include "editor/sceneview.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_paneManager(nullptr),
    m_openViewPaneMenu(nullptr),
    m_scene(),
    m_camera(),
    m_gameWidget(nullptr)
{
    auto centralWidget = new QWidget(this);
    centralWidget->setFocusPolicy(Qt::StrongFocus);

    setCentralWidget(centralWidget);

    m_gameWidget = new GameWidget(m_scene, centralWidget);
    m_gameWidget->setObjectName("Editor Main Viewport");
    m_gameWidget->setFocusPolicy(Qt::StrongFocus);
    m_gameWidget->setCamera(&m_camera);

    auto centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setMargin(0);
    centralLayout->addWidget(m_gameWidget);

    // Add controls to game widget
    createFpsLabel(m_gameWidget->gameLoop(), m_gameWidget);

    createMenus();
    initPanes();
}

void MainWindow::createMenus()
{
    auto menuBar = new QMenuBar(this);

    // View menu
    auto viewMenu = new QMenu(tr("View"), menuBar);
    m_openViewPaneMenu = new QMenu(tr("Open view pane"), viewMenu);

    viewMenu->addMenu(m_openViewPaneMenu);

    menuBar->addMenu(viewMenu);

    setMenuBar(menuBar);
}

void MainWindow::initPanes()
{
    auto *sceneView = new SceneView(m_scene, this);
    auto *componentView = new ComponentView(sceneView, this);

    auto *assetManagerPane = new QDockWidget(tr("Asset Manager"), this);
    auto *assetManagerView = new AssetManagerView(assetManagerPane);
    assetManagerView->setWindowFlags(assetManagerView->windowFlags() & ~Qt::Window);
    assetManagerPane->setWidget(assetManagerView);

    auto *particleEditorPane = new QDockWidget(tr("Particle Editor"), this);
    auto *particleEditor = new ParticleEditor(particleEditorPane);
    particleEditorPane->setWidget(particleEditor);

    registerHookSystems(m_gameWidget->systemEngine(), *componentView);

    m_paneManager = std::make_unique<PaneManager>(this, m_openViewPaneMenu);
    m_paneManager->registerPane(Qt::RightDockWidgetArea, sceneView);
    m_paneManager->registerPane(Qt::RightDockWidgetArea, componentView);
    m_paneManager->registerPane(Qt::BottomDockWidgetArea, assetManagerPane/*, false*/);
    m_paneManager->registerPane(Qt::LeftDockWidgetArea, particleEditorPane, false);

    createDefaultComponentEditorCreators(componentView);
}

void MainWindow::createDefaultComponentEditorCreators(ComponentView *componentView)
{
    componentView->registerComponentUiHandler<TransformCompUiHandler>();
    componentView->registerComponentUiHandler<ParticleEffectCompUiHandler>();
}
