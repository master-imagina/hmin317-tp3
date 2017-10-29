#include "mainwindow.h"

#include <algorithm>

#include <QApplication>
#include <QBoxLayout>
#include <QColor>
#include <QDate>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QSlider>
#include <QtMath>
#include <QTimer>

#include "aabb.h"
#include "camera.h"
#include "cameraactions.h"
#include "cameracontroller.h"
#include "cameracontrols.h"
#include "cameracontrollercontrols.h"
#include "gameloop.h"
#include "gamewidget.h"
#include "geometry.h"
#include "heightmap.h"
#include "particleeffect.h"
#include "seasoncontroller.h"
#include "scene.h"


MainWindow::MainWindow(GameLoop *gameLoop) :
    m_theGameLoop(gameLoop),
    m_scene(std::make_unique<Scene>()),
    m_terrain(std::make_unique<Geometry>()),
    m_particleEffect(std::make_unique<ParticleEffect>(QVector3D(0, 400, 0), 50, 100)),
    m_gameWidgets(),
    m_fpsLabels(),
    m_camera(std::make_unique<Camera>()),
    m_estimateFpsTimer(new QTimer(this)),
    m_cameraController(nullptr)
{
    // Scene creation
    initScene();
    auto *seasons = new SeasonController(this);

    // Build UI
    auto centralWidget = new QWidget(this);
    centralWidget->setFocusPolicy(Qt::StrongFocus);

    m_cameraController = new CameraController(centralWidget);
    centralWidget->installEventFilter(m_cameraController);

    auto centralLayout = new QVBoxLayout(centralWidget);

    // FPS controls
    auto fpsIndicator = new QLabel(centralWidget);

    auto fpsSlider = new QSlider(centralWidget);
    fpsSlider->setOrientation(Qt::Horizontal);
    fpsSlider->setMaximum(GameLoop::MAX_FPS);

    auto fpsControlsLayout = new QHBoxLayout;
    fpsControlsLayout->addWidget(fpsIndicator);
    fpsControlsLayout->addWidget(fpsSlider);

    // Viewports
    auto viewportsLayout = new QGridLayout;

    m_estimateFpsTimer->setInterval(500);

    for (int i = 0; i < m_gameWidgets.size(); i++) {
        // Create game widget
        auto gameWidget = new GameWidget(m_scene.get(), seasons, centralWidget);
        gameWidget->setObjectName(QString::number(i));
        gameWidget->setCamera(m_camera.get());

        m_gameWidgets[i] = gameWidget;

        viewportsLayout->addWidget(gameWidget, i / 2, i % 2);

        // Create fps label
        auto fpsLabel = new QLabel(gameWidget);
        fpsLabel->setStyleSheet("QLabel { background-color : red }");
        fpsLabel->move(20, 20);

        connect(m_estimateFpsTimer, &QTimer::timeout,
                [this, fpsLabel] {
            const unsigned int fps = m_theGameLoop->effectiveFramerate();
            fpsLabel->setText(QString::number(fps) + " fps");
        });

        m_fpsLabels[i] = fpsLabel;

        // Add controls to game widget
        auto *gameWidgetLayout = new QVBoxLayout(gameWidget);
        auto *gameWidgetTopLayout = new QHBoxLayout;
        gameWidgetTopLayout->addWidget(fpsLabel);
        gameWidgetTopLayout->addStretch();
        gameWidgetTopLayout->addWidget(new CameraControls(m_camera.get(), gameWidget));
        gameWidgetLayout->addLayout(gameWidgetTopLayout);
        gameWidgetLayout->addStretch();
    }

    connect(fpsSlider, &QSlider::valueChanged,
            [this, gameLoop, fpsIndicator] (int fps) {
        gameLoop->setFramerate(fps);

        const QString fpsText = QString::number(fps);

        for (QLabel *fpsLabel : m_fpsLabels) {
            fpsLabel->setText(fpsText + " fps");
        }

        fpsIndicator->setText("fps :" + fpsText);
    });

    fpsSlider->setValue(gameLoop->framerate());

    auto cameraControllerControls = new CameraControllerControls(m_cameraController,
                                                                 centralWidget);

    centralLayout->addLayout(fpsControlsLayout);
    centralLayout->addLayout(viewportsLayout, 1);
    centralLayout->addWidget(cameraControllerControls);

    setCentralWidget(centralWidget);

    createActions();

    centralWidget->setFocus();

    seasons->start();

    gameLoop->setCallback([this] (float dt) { iterateGameLoop(dt); });
    m_estimateFpsTimer->start();
}

MainWindow::~MainWindow()
{}

void MainWindow::openLoadHeightMapDialog()
{
    const QString filePath = QFileDialog::getOpenFileName(this,
                                                          QString(),
                                                          tr("Open HeightMap"),
                                                          tr("Image Files (*.png)"));

    if (filePath.isNull()) {
        return;
    }

    loadHeightMap(filePath);
}

void MainWindow::loadHeightMap(const QString &filePath)
{
    QImage heightmap(filePath);

    if (heightmap.isNull() || !heightmap.isGrayscale() ||
            heightmap.width() != heightmap.height()) {
        qCritical() << "Invalid heightmap";
        return;
    }

    *m_terrain = heightmapToGeometry(heightmap);


    m_terrain->isDirty = true;

    pointCameraToTerrainCenter();

    m_particleEffect->setRadius(heightmap.width());
}

void MainWindow::pointCameraToTerrainCenter()
{
    m_scene->terrainBoundingBox.processVertices(m_terrain->vertices);

    centerCameraOnBBox(m_camera.get(), m_scene->terrainBoundingBox);
}

void MainWindow::iterateGameLoop(float dt)
{
    // Update scene
    m_particleEffect->live(dt);
    m_cameraController->updateCamera(m_camera.get(), dt);

    for (GameWidget *gameWidget : m_gameWidgets) {
        gameWidget->startNewFrame(dt);
    }

    //FIXME Avoid file dialogs freezing. Implement threaded rendering instead
    qApp->processEvents();
}

void MainWindow::createActions()
{
    auto menuBar = new QMenuBar(this);

    // File menu
    auto fileMenu = new QMenu("&File", menuBar);
    QAction *openHeightMapAction = fileMenu->addAction("Load heightmap");
    openHeightMapAction->setShortcut(QKeySequence("Ctrl+I"));
    connect(openHeightMapAction, &QAction::triggered,
            this, &MainWindow::openLoadHeightMapDialog);

    // Camera menu
    auto cameraMenu = new QMenu("&Camera", menuBar);
    QAction *targetTerrainCenterAction = cameraMenu->addAction("Target Terrain Center");
    targetTerrainCenterAction->setShortcut(QKeySequence("C"));
    connect(targetTerrainCenterAction, &QAction::triggered,
            this, &MainWindow::pointCameraToTerrainCenter);

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(cameraMenu);

    setMenuBar(menuBar);
}

void MainWindow::initScene()
{
    m_camera->setEyePos({8, 20, 8});
    m_particleEffect->setDirection({0, -1, 0});

    m_scene->geometries.push_back(m_terrain.get());
    m_scene->geometries.push_back(m_particleEffect->geometry());
}
