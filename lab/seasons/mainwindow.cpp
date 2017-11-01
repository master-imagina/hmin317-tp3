#include "mainwindow.h"

#include <algorithm>

#include <QApplication>
#include <QBoxLayout>
#include <QColor>
#include <QDate>
#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QSlider>
#include <QtMath>
#include <QTimer>

#include "core/gameloop.h"

#include "editor/cameracontrols.h"
#include "editor/cameracontrollercontrols.h"
#include "editor/gamewidget.h"
#include "extras/heightmap.h"
#include "extras/particleeffect.h"
#include "extras/particlematerial.h"

#include "extras/cameraactions.h"
#include "extras/cameracontroller.h"

#include "render/aabb.h"
#include "render/camera.h"
#include "render/geometry.h"
#include "render/material.h"
#include "render/renderpass.h"
#include "render/scene.h"
#include "render/shaderutils.h"

#include "seasoncontroller.h"


MainWindow::MainWindow(GameLoop *gameLoop) :
    m_theGameLoop(gameLoop),
    m_scene(nullptr),
    m_terrain(nullptr),
    m_particleEffect(nullptr),
    m_terrainMaterial(nullptr),
    m_particleMaterial(nullptr),
    m_terrainMatWorldMatParam(nullptr),
    m_terrainMinHeightParam(nullptr),
    m_terrainMaxHeightParam(nullptr),
    m_terrainColorParam(nullptr),
    m_gameWidget(nullptr),
    m_fpsLabel(nullptr),
    m_camera(nullptr),
    m_estimateFpsTimer(new QTimer(this)),
    m_cameraController(nullptr),
    m_seasonController(new SeasonController(this))
{
    // Scene creation
    initScene();

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

    // Viewport
    auto viewportsLayout = new QGridLayout;

    m_estimateFpsTimer->setInterval(500);

    // Create game widget
    m_gameWidget = new GameWidget(m_scene.get(), centralWidget);
    m_gameWidget->setCamera(m_camera.get());

    viewportsLayout->addWidget(m_gameWidget);

    // Create fps label
    m_fpsLabel = new QLabel(m_gameWidget);
    m_fpsLabel->setStyleSheet("QLabel { background-color : red }");
    m_fpsLabel->move(20, 20);

    connect(m_estimateFpsTimer, &QTimer::timeout, [this] {
        const uint32 fps = m_theGameLoop->effectiveFramerate();
        m_fpsLabel->setText(QString::number(fps) + " fps");
    });

    // Add controls to game widget
    auto *gameWidgetLayout = new QVBoxLayout(m_gameWidget);
    auto *gameWidgetTopLayout = new QHBoxLayout;
    gameWidgetTopLayout->addWidget(m_fpsLabel);
    gameWidgetTopLayout->addStretch();
    gameWidgetTopLayout->addWidget(new CameraControls(m_camera.get(), m_gameWidget));
    gameWidgetLayout->addLayout(gameWidgetTopLayout);
    gameWidgetLayout->addStretch();

    connect(fpsSlider, &QSlider::valueChanged,
            [this, gameLoop, fpsIndicator] (int fps) {
        gameLoop->setFramerate(fps);

        const QString fpsText = QString::number(fps);

        m_fpsLabel->setText(fpsText + " fps");

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

    m_seasonController->start();

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

    gatherShadersParams();

    m_gameWidget->startNewFrame(dt);

    //FIXME Avoid file dialogs freezing. Implement threaded rendering instead
//    qApp->processEvents();
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
    m_scene = std::make_unique<Scene>();

    m_camera = std::make_unique<Camera>();
    m_camera->setEyePos({8, 20, 8});

    m_terrain = std::make_unique<Geometry>();

    m_particleEffect = std::make_unique<ParticleEffect>();
    m_particleEffect->setWorldPos({0, 400, 0});
    m_particleEffect->setCount(50);
    m_particleEffect->setMaxLife(100);
    m_particleEffect->setDirection({0, -1, 0});

    m_scene->geometries.emplace_back(m_terrain.get());
    m_scene->geometries.emplace_back(m_particleEffect->geometry());

    m_terrainMaterial = std::make_unique<Material>();
    RenderPass *terrainPass = m_terrainMaterial->addRenderPass("base");
    uptr<ShaderProgram> terrainShader = shaderProgramFromFile("://res/shaders/terrain_heightmap.vert",
                                                              "",
                                                              "://res/shaders/terrain_heightmap.frag");
    terrainPass->setShaderProgram(std::move(terrainShader));

    m_terrainMatWorldMatParam = terrainPass->addParam("worldMatrix", QMatrix4x4());
    m_terrainMinHeightParam = terrainPass->addParam("minHeight", 0.f);
    m_terrainMaxHeightParam = terrainPass->addParam("maxHeight", 1.f);
    m_terrainColorParam = terrainPass->addParam("terrainColor", QColor());

    m_particleMaterial = std::make_unique<ParticleMaterial>();

    m_scene->materials.emplace_back(m_terrainMaterial.get());
    m_scene->materials.emplace_back(m_particleMaterial.get());
}

void MainWindow::gatherShadersParams()
{
    const QMatrix4x4 viewMatrix = m_camera->viewMatrix();
    const QMatrix4x4 projectionMatrix = m_camera->projectionMatrix();
    const QMatrix4x4 worldMatrix = projectionMatrix * viewMatrix;

    const QVector3D terrainAABBCenter = m_scene->terrainBoundingBox.center();
    const QVector3D terrainAABBRadius = m_scene->terrainBoundingBox.radius();

    const float minHeight = terrainAABBCenter.y() - terrainAABBRadius.y();
    const float maxHeight = terrainAABBCenter.y() + terrainAABBRadius.y();

    const QColor drawColor = m_seasonController->colorFromSeason();

    // Update terrain material parameters
    m_terrainMatWorldMatParam->value = worldMatrix;
    m_terrainMinHeightParam->value = minHeight;
    m_terrainMaxHeightParam->value = maxHeight;
    m_terrainColorParam->value = drawColor;

    // Update particle material parameters
    m_particleMaterial->setViewMatrix(viewMatrix);
    m_particleMaterial->setProjectionMatrix(projectionMatrix);
    m_particleMaterial->setColor(drawColor);
    m_particleMaterial->setSize(4.f);
}
