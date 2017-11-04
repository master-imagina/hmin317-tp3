#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <array>
#include <memory>

#include <QDate>
#include <QMainWindow>

#include "core/aliases_memory.h"

#include "render/aabb.h"

class Camera;
class CameraController;
class GameLoop;
class GameWidget;
class Geometry;
class Material;
class ParticleEffect;
class ParticleMaterial;
class Scene;
class SeasonController;
class ShaderParam;

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
    void gatherShadersParams();

private:
    GameLoop *m_theGameLoop;

    uptr<Scene> m_scene;
    uptr<Geometry> m_terrain;
    AABoundingBox m_terrainBoundingBox;
    uptr<ParticleEffect> m_particleEffect;
    uptr<Material> m_terrainMaterial;
    ShaderParam *m_terrainMatWorldMatParam;
    ShaderParam *m_terrainMinHeightParam;
    ShaderParam *m_terrainMaxHeightParam;
    ShaderParam *m_terrainColorParam;
    uptr<ParticleMaterial> m_particleMaterial;

    GameWidget *m_gameWidget;
    QLabel *m_fpsLabel;
    uptr<Camera> m_camera;

    QTimer *m_estimateFpsTimer;

    CameraController *m_cameraController;

    SeasonController *m_seasonController;
};

#endif // MAINWINDOW_H
