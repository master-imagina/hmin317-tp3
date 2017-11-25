#include "mainwindow.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDataStream>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFormLayout>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>

#include "core/assetmanager.h"

#include "extras/gamewidget.h"
#include "extras/serialization.h"

#include "editor/gui/fpswidgets.h"

#include "editor/assetmanagerview.h"
#include "editor/componentuihandlers.h"
#include "editor/componentview.h"
#include "editor/hooksystems.h"
#include "editor/panemanager.h"
#include "editor/particleeditor.h"
#include "editor/projectmanager.h"
#include "editor/sceneview.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_paneManager(nullptr),
    m_openViewPaneMenu(nullptr),
    m_assetManagerView(nullptr),
    m_projectManager(nullptr),
    m_scene(),
    m_camera(),
    m_gameWidget(nullptr)
{
    m_projectManager = new ProjectManager(this);

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
        m_projectManager->load(action->data().toString());

        const QUrl projectFileUrl =
                QUrl(m_projectManager->currentProjectPath() + "/")
                .resolved(QUrl(m_projectManager->currentProjectName()));

        QFile projectFile(projectFileUrl.toString());

        if (!projectFile.open(QIODevice::ReadOnly)) {
            std::cerr << "[EDITOR - ERROR] << can't load project file"
                      << std::endl;
        }

        QDataStream dataStream(&projectFile);
        dataStream >> m_scene;
    });

    QAction *saveProjectAction = fileMenu->addAction(tr("Save project"));
    saveProjectAction->setShortcut(QKeySequence("Ctrl+S"));

    connect(saveProjectAction, &QAction::triggered,
            [this] {
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
    });

    // Project menu
    QMenu *projectMenu = menuBar->addMenu(tr("Project"));

    QAction *buildProjectAction = projectMenu->addAction(tr("Build"));

    connect(buildProjectAction, &QAction::triggered,
            m_projectManager, &ProjectManager::build);

    // View menu
    QMenu *viewMenu = menuBar->addMenu(tr("View"));
    m_openViewPaneMenu = viewMenu->addMenu(tr("Open view pane"));
}

void MainWindow::initPanes()
{
    auto *sceneView = new SceneView(m_scene, this);
    auto *componentView = new ComponentView(sceneView, m_projectManager, this);

    auto *assetManagerPane = new QDockWidget(tr("Asset Manager"), this);
    m_assetManagerView = new AssetManagerView(assetManagerPane);
    m_assetManagerView->setWindowFlags(m_assetManagerView->windowFlags() & ~Qt::Window);
    assetManagerPane->setWidget(m_assetManagerView);

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
}
