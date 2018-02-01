#include "mainwindow.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDataStream>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFormLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>

#include "core/assetmanager.h"

#include "extras/serialization.h"

#include "editor/gui/fpswidgets.h"
#include "editor/gui/gamewidget.h"

#include "editor/assetmanagerview.h"
#include "editor/cameracontrols.h"
#include "editor/componentuihandlers.h"
#include "editor/componentview.h"
#include "editor/entityxhook.h"
#include "editor/entitylistmodel.h"
#include "editor/panemanager.h"
#include "editor/particleeditor.h"
#include "editor/projectmanager.h"
#include "editor/sceneview.h"

#include "physics/collisionsystem.h"

#include "render/camera.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_paneManager(nullptr),
    m_openViewPaneMenu(nullptr),
    m_assetManagerView(nullptr),
    m_projectManager(nullptr),
    m_freeCamera(),
    m_scene(),
    m_luaServer(),
    m_centralWidget(nullptr),
    m_gameWidget(nullptr),
    m_entityxHook(nullptr),
    m_cameraControls(nullptr),
    m_inPlayMode(false),
    m_playModeSceneDump(),
    m_entityListModel(nullptr)
{
    m_entityListModel = new EntityListModel(m_scene, this);

    m_projectManager = new ProjectManager(this);

    m_centralWidget = new QWidget(this);
    m_centralWidget->setObjectName("centralWidget");
    m_centralWidget->setFocusPolicy(Qt::StrongFocus);

    setCentralWidget(m_centralWidget);

    m_gameWidget = new GameWidget(m_scene, &m_luaServer, m_centralWidget);
    m_gameWidget->setObjectName("Editor Main Viewport");
    m_gameWidget->setFocusPolicy(Qt::StrongFocus);
    m_gameWidget->systemEngine().configure();
    m_gameWidget->disablePlayMode();

    m_gameWidget->setCamera(&m_freeCamera);

    auto centralLayout = new QVBoxLayout(m_centralWidget);
    centralLayout->setMargin(1);
    centralLayout->addWidget(m_gameWidget);

    // Add controls to game widget
    createFpsLabel(m_gameWidget->gameLoop(), m_gameWidget);

    m_cameraControls = new CameraControls(m_gameWidget->camera(), m_scene,
                                          m_gameWidget);

    connect(m_cameraControls, &CameraControls::cameraChanged,
            [this] (Camera *camera) {
        if (!camera) {
            m_gameWidget->setCamera(&m_freeCamera);
            m_cameraControls->setCamera(&m_freeCamera);
        }
        else {
            m_gameWidget->setCamera(camera);
        }
    });

    auto *gameWidgetTopBar = new QWidget(m_gameWidget);

    auto *gameWidgetTopBarLayout = new QHBoxLayout(gameWidgetTopBar);
    gameWidgetTopBarLayout->addStretch();
    gameWidgetTopBarLayout->addWidget(m_cameraControls);

    auto *gameWidgetLayout = new QVBoxLayout(m_gameWidget);
    gameWidgetLayout->addWidget(m_cameraControls);
    gameWidgetLayout->addStretch();

    // Finishing UI creation
    createMenus();
    initPanes();

    createConnections();
}

void MainWindow::createMenus()
{
    auto menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // File menu
    QMenu *fileMenu = menuBar->addMenu(tr("File"));
    QAction *newProjectAction = fileMenu->addAction(tr("New project"));
    newProjectAction->setShortcut(QKeySequence("Ctrl+N"));

    connect(newProjectAction, &QAction::triggered,
            [this] {
        auto *createProjectDialog = new QDialog(this);
        createProjectDialog->setWindowTitle(tr("New project"));
        createProjectDialog->setModal(true);

        auto *projectNameLineEdit = new QLineEdit(createProjectDialog);
        projectNameLineEdit->setText("test_project");
        //TODO validate line edit content (no slashs or forbidden chars)

        auto *acceptBtn = new QPushButton(createProjectDialog);
        acceptBtn->setText(tr("Create"));

        connect(acceptBtn, &QPushButton::clicked,
                createProjectDialog, &QDialog::accept);

        auto *createProjectDialogLayout = new QFormLayout(createProjectDialog);
        createProjectDialogLayout->addRow(tr("Project Name"), projectNameLineEdit);
        createProjectDialogLayout->addRow(acceptBtn);

        connect(createProjectDialog, &QDialog::accepted,
                [this, projectNameLineEdit] {
            m_scene.clear();
            m_projectManager->create(projectNameLineEdit->text());
        });

        createProjectDialog->exec();
    });

    QMenu *recentProjectsMenu = fileMenu->addMenu(tr("Recent projects"));

    for (const QString &projectPath : m_projectManager->recentProjects()) {
        const QString projectName =
                projectPath.mid(m_projectManager->projectsPath().lastIndexOf(QDir::separator()) + 1);

        QAction *openRecentAction = recentProjectsMenu->addAction(projectName);
        openRecentAction->setData(projectPath);
    }

    connect(recentProjectsMenu, &QMenu::triggered,
            [this] (QAction *action) {
        loadProject(action->data().toString());
    });

    QAction *saveProjectAction = fileMenu->addAction(tr("Save project"));
    saveProjectAction->setShortcut(QKeySequence("Ctrl+S"));

    connect(saveProjectAction, &QAction::triggered,
            this, &MainWindow::saveProject);

    // Project menu
    QMenu *projectMenu = menuBar->addMenu(tr("Project"));

    QAction *buildProjectAction = projectMenu->addAction(tr("Build"));

    connect(buildProjectAction, &QAction::triggered,
            m_projectManager, &ProjectManager::build);

    QAction *playAction = projectMenu->addAction(tr("Run"));
    playAction->setShortcut(QKeySequence("Ctrl+R"));

    connect(playAction, &QAction::triggered,
            this, &MainWindow::enterPlayMode);

    auto *leavePlayAction = new QAction(tr("Play"), m_gameWidget);
    leavePlayAction->setShortcut(QKeySequence("Escape"));

    m_gameWidget->addAction(leavePlayAction);

    connect(leavePlayAction, &QAction::triggered,
            this, &MainWindow::leavePlayMode);

    // View menu
    QMenu *viewMenu = menuBar->addMenu(tr("View"));
    m_openViewPaneMenu = viewMenu->addMenu(tr("Open view pane"));
}

void MainWindow::initPanes()
{
    auto *sceneView = new SceneView(m_scene, this);
    auto *componentView = new ComponentView(sceneView, m_entityListModel, m_projectManager, this);

    auto *assetManagerPane = new QDockWidget(tr("Asset Manager"), this);
    m_assetManagerView = new AssetManagerView(assetManagerPane);
    m_assetManagerView->setWindowFlags(m_assetManagerView->windowFlags() & ~Qt::Window);
    assetManagerPane->setWidget(m_assetManagerView);

    auto *particleEditorPane = new QDockWidget(tr("Particle Editor"), this);
    auto *particleEditor = new ParticleEditor(particleEditorPane);
    particleEditorPane->setWidget(particleEditor);

    m_entityxHook = new EntityxHook(m_gameWidget->systemEngine(), this);

    connect(m_entityxHook, &EntityxHook::entityComponentAdded,
            componentView, &ComponentView::onComponentAdded,
            Qt::QueuedConnection);

    connect(m_entityxHook, &EntityxHook::entityComponentRemoved,
            componentView, &ComponentView::onComponentAdded,
            Qt::QueuedConnection);

    connect(m_entityxHook, &EntityxHook::entityComponentAdded,
            m_cameraControls, &CameraControls::onEntityComponentAdded);

    connect(m_entityxHook, &EntityxHook::entityComponentRemoved,
            m_cameraControls, &CameraControls::onEntityComponentRemoved);

    m_paneManager = std::make_unique<PaneManager>(this, m_openViewPaneMenu);
    m_paneManager->registerPane(Qt::RightDockWidgetArea, sceneView);
    m_paneManager->registerPane(Qt::RightDockWidgetArea, componentView);
    m_paneManager->registerPane(Qt::BottomDockWidgetArea, assetManagerPane, false);
    m_paneManager->registerPane(Qt::LeftDockWidgetArea, particleEditorPane, false);

    createDefaultComponentEditorCreators(componentView);
}

void MainWindow::createConnections()
{
    connect(m_projectManager, &ProjectManager::projectCreated,
            m_assetManagerView, &AssetManagerView::setProjectPath);

    connect(m_projectManager, &ProjectManager::projectLoaded,
            m_assetManagerView, &AssetManagerView::setProjectPath);
}

void MainWindow::createDefaultComponentEditorCreators(ComponentView *componentView)
{
    componentView->registerComponentUiHandler<TransformCompUiHandler>();
    componentView->registerComponentUiHandler<ParticleEffectCompUiHandler>();
    componentView->registerComponentUiHandler<MeshCompUiHandler>();
    componentView->registerComponentUiHandler<LightCompUiHandler>();
    componentView->registerComponentUiHandler<MaterialCompUiHandler>();
    componentView->registerComponentUiHandler<CameraCompUiHandler>();
    componentView->registerComponentUiHandler<KeyboardCompUiHandler>();
    componentView->registerComponentUiHandler<ScriptCompUiHandler>(m_luaServer);
    componentView->registerComponentUiHandler<ColliderCompUiHandler>();
    componentView->registerComponentUiHandler<RigidBodyCompUiHandler>();
}

void MainWindow::loadProject(const QString &filePath)
{
    m_projectManager->load(filePath);

    const QUrl projectFileUrl =
            QUrl(m_projectManager->currentProjectPath() + "/")
            .resolved(QUrl(m_projectManager->currentProjectName()));

    QFile projectFile(projectFileUrl.toString());

    if (!projectFile.open(QIODevice::ReadOnly)) {
        std::cerr << "[EDITOR - ERROR] << can't load project file"
                  << std::endl;
    }

    m_scene.clear();

    QDataStream dataStream(&projectFile);
    deserializeScene(dataStream, m_scene, m_luaServer);
}

void MainWindow::saveProject() const
{
    if (!m_projectManager->hasOpenedProject()) {
        return;
    }

    const QUrl projectFileUrl =
            QUrl(m_projectManager->currentProjectPath() + "/")
            .resolved(QUrl(m_projectManager->currentProjectName()));

    QFile projectFile(projectFileUrl.toString());
    if (!projectFile.open(QIODevice::WriteOnly)) {
        std::cerr << "[EDITOR - ERROR] << can't save project file"
                  << std::endl;
    }

    QDataStream dataStream(&projectFile);
    dataStream << m_scene;
}

void MainWindow::enterPlayMode()
{
    if (m_inPlayMode) {
        return;
    }

    QDataStream playModeSceneDumpStream(&m_playModeSceneDump, QIODevice::WriteOnly);
    playModeSceneDumpStream << m_scene;

    m_gameWidget->setFocus();
    m_gameWidget->enablePlayMode();
    m_gameWidget->systemEngine().configure();

    m_centralWidget->setStyleSheet("#centralWidget { border: 2px solid red; }");

    setCursor(Qt::BlankCursor);

    m_inPlayMode = true;
}

void MainWindow::leavePlayMode()
{
    if (!m_inPlayMode) {
        return;
    }

    m_gameWidget->disablePlayMode();
    m_gameWidget->setCamera(m_cameraControls->getCamera());

    m_centralWidget->setStyleSheet(QString());

    setCursor(Qt::ArrowCursor);

    m_gameWidget->systemEngine().system<CollisionSystem>()->clear(m_scene.m_entityManager);

    //TODO Optimize
    m_scene.clear();

    QDataStream playModeSceneDumpStream(&m_playModeSceneDump, QIODevice::ReadOnly);
    deserializeScene(playModeSceneDumpStream, m_scene, m_luaServer);

    m_inPlayMode = false;
}
