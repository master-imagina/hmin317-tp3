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

private Q_SLOTS:
    void openLoadHeightMapDialog();
    void loadHeightMap(const QString &filePath);
    void pointCameraToTerrainCenter();

private:
    void iterateGameLoop(float dt);
    void createActions();

private:
    void initSeasons();

private:
    GameLoop *m_gameLoop;
    std::unique_ptr<Geometry> m_terrainGeometry;
    std::array<GameWidget *, 4> m_gameWidgets;
    std::unique_ptr<Camera> m_camera;

    CameraController *m_cameraController;

    QTimer *m_seasonTimer;
    std::array<QDate, 4> m_gameWidgetsDates;
};

#endif // MAINWINDOW_H
