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
    void initSeasons();

private:
    static constexpr int viewportCount = 4;

    std::unique_ptr<Geometry> m_terrainGeometry;
    std::unique_ptr<ParticleEffect> m_particleEffect;

    std::array<GameWidget *, viewportCount> m_gameWidgets;
    std::array<QLabel *, viewportCount> m_fpsLabels;
    std::unique_ptr<Camera> m_camera;

    CameraController *m_cameraController;

    QTimer *m_seasonTimer;
    std::array<QDate, viewportCount> m_gameWidgetsDates;
};

#endif // MAINWINDOW_H
