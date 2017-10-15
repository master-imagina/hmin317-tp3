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

class QTimer;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
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

    GameLoop *m_gameLoop;
    std::unique_ptr<Geometry> m_terrainGeometry;
    std::array<GameWidget *, viewportCount> m_gameWidgets;
    std::unique_ptr<Camera> m_camera;

    CameraController *m_cameraController;

    QTimer *m_seasonTimer;
    std::array<QDate, viewportCount> m_gameWidgetsDates;
};

#endif // MAINWINDOW_H
