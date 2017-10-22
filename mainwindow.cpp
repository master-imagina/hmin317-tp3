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
#include "cameracontroller.h"
#include "gameloop.h"
#include "gamewidget.h"
#include "geometry.h"
#include "heightmap.h"
#include "particleeffect.h"


namespace {

const std::map<std::pair<int, int>, Season> DATE_TO_SEASON {
    {{9, 22}, Season::Autumn},
    {{12, 21}, Season::Winter},
    {{3, 20}, Season::Spring},
    {{6, 21}, Season::Summer}
};

} // anon namespace



MainWindow::MainWindow(GameLoop *gameLoop) :
    m_theGameLoop(gameLoop),
    m_terrainGeometry(std::make_unique<Geometry>()),
    m_particleEffect(std::make_unique<ParticleEffect>(QVector3D(0, 400, 0), 50, 100)),
    m_gameWidgets(),
    m_fpsLabels(),
    m_camera(std::make_unique<Camera>()),
    m_computeFpsTimer(new QTimer(this)),
    m_cameraController(nullptr),
    m_seasonTimer(new QTimer(this)),
    m_gameWidgetsDates()
{
    m_camera->setEyePos({8, 20, 8});
    m_particleEffect->setDirection({0, -1, 0});

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

    m_computeFpsTimer->setInterval(1000);

    for (int i = 0; i < m_gameWidgets.size(); i++) {
        auto gameWidget = new GameWidget(centralWidget);

        gameWidget->setGeometry(m_terrainGeometry.get());
        gameWidget->setParticleEffect(m_particleEffect.get());
        gameWidget->setCamera(m_camera.get());

        m_gameWidgets[i] = gameWidget;

        viewportsLayout->addWidget(gameWidget, i / 2, i % 2);

        auto fpsLabel = new QLabel(gameWidget);
        fpsLabel->setStyleSheet("QLabel { background-color : red }");
        fpsLabel->move(20, 20);

        connect(m_computeFpsTimer, &QTimer::timeout,
                [this, fpsLabel] {
            const unsigned int fps = m_theGameLoop->effectiveFramerate();
            fpsLabel->setText(QString::number(fps) + " fps");
        });

        m_fpsLabels[i] = fpsLabel;
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

    initSeasons();

    centralLayout->addLayout(fpsControlsLayout);
    centralLayout->addLayout(viewportsLayout);

    setCentralWidget(centralWidget);

    createActions();

    centralWidget->setFocus();

    gameLoop->setCallback([this] (float dt) { iterateGameLoop(dt); });
    m_computeFpsTimer->start();
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

    if (heightmap.isNull() || !heightmap.isGrayscale()) {
        return;
    }

    *m_terrainGeometry = heightmapToGeometry(heightmap);

    for (GameWidget *gameWidget : m_gameWidgets) {
        gameWidget->setRendererDirty();
    }

    pointCameraToTerrainCenter();
}

void MainWindow::pointCameraToTerrainCenter()
{
    AABoundingBox terrainAABB(m_terrainGeometry->vertices);

    const QVector3D terrainCenter = terrainAABB.center();
    const QVector3D flatCenter(terrainCenter.x(), 0.f, terrainCenter.z());

    const QVector3D newEye(flatCenter.x() + 50,
                           terrainAABB.yBounds().second + 50,
                           flatCenter.z() + 50);

    m_camera->setEyePos(newEye);
    m_camera->setTargetPos(flatCenter);
}

void MainWindow::iterateGameLoop(float dt)
{
    // Update scene
    m_particleEffect->live(dt);
    m_cameraController->updateCamera(m_camera.get(), dt);

    for (GameWidget *gameWidget : m_gameWidgets) {
        gameWidget->setRendererDirty();

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

void MainWindow::initSeasons()
{
    // Init season for each game widget
    for (auto it = DATE_TO_SEASON.begin(); it != DATE_TO_SEASON.end(); it++) {
        const int i = DATE_TO_SEASON.size() - std::distance(it, DATE_TO_SEASON.end());

        m_gameWidgetsDates[i] = QDate(2017, it->first.first, it->first.second - 1);
        m_gameWidgets[i]->setSeason(it->second);
    }

    // Init and start timer
    m_seasonTimer->setInterval(75);

    connect(m_seasonTimer, &QTimer::timeout, [this] {
        for (int i = 0; i < m_gameWidgetsDates.size(); i++) {
            QDate &date = m_gameWidgetsDates[i];
            date = date.addDays(1);

            GameWidget *gameWidget = m_gameWidgets[i];

            const std::pair<int, int> dayAndMonth {date.month(), date.day()};

            auto seasonChangedIt = DATE_TO_SEASON.find(dayAndMonth);

            if (seasonChangedIt != DATE_TO_SEASON.end()) {
                const Season newSeason = seasonChangedIt->second;

                gameWidget->setSeason(newSeason);
            }
        }
    });

    m_seasonTimer->start();
}
