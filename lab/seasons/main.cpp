#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>

#include "3rdparty/entityx/deps/Dependencies.h"

#include "core/gameloop.h"
#include "core/scene.h"
#include "core/systemengine.h"

#include "editor/fpswidgets.h"
#include "editor/renderwidget.h"

#include "extras/cameraactions.h"
#include "extras/cameracontroller.h"
#include "extras/heightmap.h"
#include "extras/particles/particleeffect.h"
#include "extras/particles/particlematerial.h"
#include "extras/particles/particlesystem.h"

#include "render/aabb.h"
#include "render/camera.h"
#include "render/rendersystem.h"

#include "render/renderer/renderer.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderutils.h"

#include "seasoncontroller.h"



Scene m_scene;
SystemEngine systemEngine(m_scene);

RenderWidget *m_renderWidget;

uptr<Camera> m_camera;

entityx::Entity terrainEntity;
entityx::ComponentHandle<Geometry> m_terrainGeom;
entityx::ComponentHandle<Material> m_terrainMaterial;

ShaderParam *m_terrainMatWorldMatParam;
ShaderParam *m_terrainMinHeightParam;
ShaderParam *m_terrainMaxHeightParam;
ShaderParam *m_terrainColorParam;
AABoundingBox m_terrainBoundingBox;

CameraController m_cameraController;
SeasonController m_seasonController;


void initScene()
{
    m_camera = std::make_unique<Camera>();
    m_camera->setEyePos({8, 20, 8});

    // Create terrain
    terrainEntity = m_scene.createEntity();
    m_terrainGeom = terrainEntity.assign<Geometry>();
    *m_terrainGeom.get() = heightmapToGeometry(QImage("images/heightmap-1.png"));

    VertexAttrib standardVertexAttrib {"vertexPos", 3, VertexAttrib::Type::Float, false, 0};
    m_terrainGeom->vertexLayout.addAttribute(standardVertexAttrib);

    m_terrainMaterial = terrainEntity.assign<Material>();
    RenderPass *terrainPass = m_terrainMaterial->addRenderPass("base");
    uptr<ShaderProgram> terrainShader = shaderProgramFromFile("://res/shaders/terrain_heightmap.vert",
                                                              "",
                                                              "://res/shaders/terrain_heightmap.frag");
    terrainPass->setShaderProgram(std::move(terrainShader));

    m_terrainMatWorldMatParam   =   terrainPass->addParam("worldMatrix", QMatrix4x4());
    m_terrainMinHeightParam     =   terrainPass->addParam("minHeight", 0.f);
    m_terrainMaxHeightParam     =   terrainPass->addParam("maxHeight", 1.f);
    m_terrainColorParam         =   terrainPass->addParam("terrainColor", QColor());

    // Create particle effect
//    entityx::Entity particleEntity = m_scene.createEntity();
//    auto particleEffect = particleEntity.assign<ParticleEffect>();
//    *particleEntity.component<Geometry>().get() = particleEffect->geometry();
//    m_particleEffect->geometry()->vertexLayout.addAttribute(standardVertexAttrib);

//    m_particleEffect = particleEntity;
//    m_particleEffect->setWorldPos({0, 400, 0});
//    m_particleEffect->setCount(50);
//    m_particleEffect->setMaxLife(100);
//    m_particleEffect->setDirection({0, -1, 0});

//    m_particleMaterial = particleEntity.assign<ParticleMaterial>();
}

void gatherShadersParams()
{
    const QMatrix4x4 viewMatrix = m_camera->viewMatrix();
    const QMatrix4x4 projectionMatrix = m_camera->projectionMatrix();
    const QMatrix4x4 worldMatrix = projectionMatrix * viewMatrix;

    const QVector3D terrainAABBCenter = m_terrainBoundingBox.center();
    const QVector3D terrainAABBRadius = m_terrainBoundingBox.radius();

    const float minHeight = terrainAABBCenter.y() - terrainAABBRadius.y();
    const float maxHeight = terrainAABBCenter.y() + terrainAABBRadius.y();

    const QColor drawColor = m_seasonController.colorFromSeason();

    // Update terrain material parameters
    m_terrainMatWorldMatParam->value = worldMatrix;
    m_terrainMinHeightParam->value = minHeight;
    m_terrainMaxHeightParam->value = maxHeight;
    m_terrainColorParam->value = drawColor;

    // Update particle material parameters
//    m_particleMaterial->setViewMatrix(viewMatrix);
//    m_particleMaterial->setProjectionMatrix(projectionMatrix);
//    m_particleMaterial->setColor(drawColor);
//    m_particleMaterial->setSize(4.f);
}

void iterateGameLoop(float dt)
{
    // Update camera
    m_cameraController.updateCamera(m_camera.get(), dt);

    // Update scene
    gatherShadersParams();

    // Render
    systemEngine.update<ParticleSystem>(dt);
    systemEngine.update<RenderSystem>(dt);
}


int main(int argc, char *argv[])
{
    // Set OpenGL version
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);
    app.setApplicationName("Game Engines - PW3");

    GameLoop gameLoop(60);
    gameLoop.setCallback([] (float dt) { iterateGameLoop(dt); });

    m_renderWidget = new RenderWidget;
    m_renderWidget->installEventFilter(&m_cameraController);
    m_renderWidget->setMinimumSize(640, 400);

    initScene();

    systemEngine.registerSystem<RenderSystem>(m_renderWidget);
    systemEngine.registerSystem<ParticleSystem>();
    systemEngine.initialize();

    m_renderWidget->setCamera(m_camera.get());
    m_renderWidget->show();

    m_terrainBoundingBox.processVertices(m_terrainGeom->vertices);
    centerCameraOnBBox(m_camera.get(), m_terrainBoundingBox);

    gameLoop.run();
    m_seasonController.start();

    return app.exec();
}
