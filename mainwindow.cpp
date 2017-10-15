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
#include <QtMath>
#include <QTimer>

#include "aabb.h"
#include "camera.h"
#include "cameracontroller.h"
#include "gameloop.h"
#include "gamewidget.h"
#include "geometry.h"
#include "heightmap.h"


namespace {

const int FPS = 60;

const std::map<std::pair<int, int>, Season> DATE_TO_SEASON {
    {{9, 22}, Season::Autumn},
    {{12, 21}, Season::Winter},
    {{3, 20}, Season::Spring},
    {{6, 21}, Season::Summer}
};

} // anon namespace



MainWindow::MainWindow() :
    m_gameLoop(new GameLoop(FPS, this)),
    m_terrainGeometry(std::make_unique<Geometry>()),
    m_gameWidgets(),
    m_camera(std::make_unique<Camera>()),
    m_cameraController(nullptr),
    m_seasonTimer(new QTimer(this)),
    m_gameWidgetsDates()
{
    m_camera->setEyePos({8, 20, 8});

    auto centralWidget = new QWidget(this);
    m_cameraController = new CameraController(centralWidget);
    centralWidget->installEventFilter(m_cameraController);

    auto centralLayout = new QGridLayout(centralWidget);

    for (int i = 0; i < m_gameWidgets.size(); i++) {
        auto gameWidget = new GameWidget(this);

        gameWidget->setObjectName("GameWidget" + QString::number(i));
        gameWidget->setGeometry(m_terrainGeometry.get());
        gameWidget->setCamera(m_camera.get());

        m_gameWidgets[i] = gameWidget;

        centralLayout->addWidget(gameWidget, i / 2, i % 2);

        auto fpsLabel = new QLabel(QString::number(FPS) + " fps", gameWidget);
        fpsLabel->setStyleSheet("QLabel { background-color : red }");
        fpsLabel->move(20, 20);
    }

    initSeasons();

    setCentralWidget(centralWidget);

    createActions();

    m_gameLoop->setCallback([this] (float dt) { iterateGameLoop(dt); });
    m_gameLoop->run();

    centralWidget->setFocus();
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

void MainWindow::initSeasons()
{
    // Init season for each game widget
    for (auto it = DATE_TO_SEASON.begin(); it != DATE_TO_SEASON.end(); it++) {
        const int i = DATE_TO_SEASON.size() - std::distance(it, DATE_TO_SEASON.end());

        m_gameWidgetsDates[i] = QDate(2017, it->first.first, it->first.second-1);
        m_gameWidgets[i]->setSeason(it->second);
    }

    // Init and start timer
    m_seasonTimer->setInterval(125);

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
