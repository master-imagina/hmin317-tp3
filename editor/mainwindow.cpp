#include "mainwindow.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>

#include "extras/gamewidget.h"

#include "editor/gui/fpswidgets.h"

#include "editor/panemanager.h"
#include "editor/sceneview.h"
#include "editor/componenteditors.h"
#include "editor/componentview.h"


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

    m_paneManager = std::make_unique<PaneManager>(this, m_openViewPaneMenu);
    m_paneManager->registerPane(Qt::RightDockWidgetArea, sceneView);
    m_paneManager->registerPane(Qt::RightDockWidgetArea, componentView);

    createDefaultComponentEditorCreators(componentView);
}

void MainWindow::createDefaultComponentEditorCreators(ComponentView *componentView)
{
    componentView->addComponentEditorCreator(&createTransformEditor);
}
