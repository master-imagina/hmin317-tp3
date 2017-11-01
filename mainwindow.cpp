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
#include "scene.h"
#include "seasoncontroller.h"
#include "shaderutils.h"

#include "renderer/material.h"
#include "renderer/renderpass.h"


MainWindow::MainWindow(GameLoop *gameLoop) :
    m_theGameLoop(gameLoop),
    m_scene(std::make_unique<Scene>()),
    m_terrain(std::make_unique<Geometry>()),
    m_particleEffect(std::make_unique<ParticleEffect>()),
    m_gameWidget(),
    m_fpsLabel(),
    m_camera(std::make_unique<Camera>()),
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

    m_particleEffect->setWorldPos({0, 400, 0});
    m_particleEffect->setCount(50);
    m_particleEffect->setMaxLife(100);
    m_particleEffect->setDirection({0, -1, 0});

    m_scene->geometries.emplace_back(m_terrain.get());
    m_scene->geometries.emplace_back(m_particleEffect->geometry());

    Material *terrainMaterial = new Material;
    RenderPass *terrainPass = terrainMaterial->addRenderPass("base");
    uptr<ShaderProgram> terrainShader = shaderProgramFromFile("://res/shaders/terrain_heightmap.vert",
                                                              "",
                                                              "://res/shaders/terrain_heightmap.frag");
    terrainPass->setShaderProgram(std::move(terrainShader));

    Material *particleMaterial = new Material;
    RenderPass *particlePass = particleMaterial->addRenderPass("base");
    uptr<ShaderProgram> particleShader = shaderProgramFromFile("://res/shaders/particle.vert",
                                                               "://res/shaders/particle.geom",
                                                               "://res/shaders/particle.frag");
    particlePass->setShaderProgram(std::move(particleShader));

    m_scene->materials.emplace_back(terrainMaterial);
    m_scene->materials.emplace_back(particleMaterial);
}

void MainWindow::gatherShadersParams()
{
    // Update terrain shader parameters
    RenderPass *terrainPass = m_scene->materials[0]->renderPasses()[0].get();
    terrainPass->clearParams();

    const QMatrix4x4 viewMatrix = m_camera->viewMatrix();
    const QMatrix4x4 projectionMatrix = m_camera->projectionMatrix();
    const QMatrix4x4 worldMatrix = projectionMatrix * viewMatrix;

    const QVector3D terrainAABBCenter = m_scene->terrainBoundingBox.center();
    const QVector3D terrainAABBRadius = m_scene->terrainBoundingBox.radius();

    const float minHeight = terrainAABBCenter.y() - terrainAABBRadius.y();
    const float maxHeight = terrainAABBCenter.y() + terrainAABBRadius.y();

    const QColor drawColor = m_seasonController->colorFromSeason();

    terrainPass->addParam({"worldMatrix", worldMatrix});
    terrainPass->addParam({"minHeight", minHeight});
    terrainPass->addParam({"maxHeight", maxHeight});
    terrainPass->addParam({"terrainColor", drawColor});

    // Update particle shader parameters
    RenderPass *particlePass = m_scene->materials[1]->renderPasses()[0].get();
    particlePass->clearParams();

    particlePass->addParam({"viewMatrix", viewMatrix});
    particlePass->addParam({"projectionMatrix", projectionMatrix});
    particlePass->addParam({"particleColor", drawColor});
    particlePass->addParam({"particlesSize", 4.f});
}
