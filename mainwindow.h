#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <array>
#include <memory>

#include <QDate>
#include <QMainWindow>

class Camera;
class CameraController;
class GameLoop;
class GameWidget;
class Geometry;
class ParticleEffect;
class Scene;

class QLabel;
class QTimer;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(GameLoop *gameLoop);
    ~MainWindow();

public Q_SLOTS:
    void loadHeightMap(const QString &filePath);

private Q_SLOTS:
    void openLoadHeightMapDialog();
    void pointCameraToTerrainCenter();

private:
    void iterateGameLoop(float dt);
    void createActions();

private:
    void initScene();
    void initSeasons();

private:
    static constexpr int viewportCount = 4;

    GameLoop *m_theGameLoop;

    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<Geometry> m_terrain;
    std::unique_ptr<ParticleEffect> m_particleEffect;

    std::array<GameWidget *, viewportCount> m_gameWidgets;
    std::array<QLabel *, viewportCount> m_fpsLabels;
    std::unique_ptr<Camera> m_camera;

    QTimer *m_computeFpsTimer;

    CameraController *m_cameraController;

    QTimer *m_seasonTimer;
    std::array<QDate, viewportCount> m_gameWidgetsDates;
};

#endif // MAINWINDOW_H
