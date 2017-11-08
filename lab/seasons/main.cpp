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
#include "extras/particles/particlesystem.h"
#include "extras/particles/quick.h"

#include "render/aabb.h"
#include "render/camera.h"
#include "render/rendersystem.h"

#include "render/renderer/renderer.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderutils.h"

#include "seasoncontroller.h"


Scene scene;

// System engine
SystemEngine systemEngine(scene);
RenderWidget *renderWidget;

// Scene description
uptr<Camera> camera;

entityx::Entity terrainEntity;
entityx::ComponentHandle<Geometry> terrainGeom;
entityx::ComponentHandle<Material> terrainMaterial;

ShaderParam *terrainMatWorldMatParam;
ShaderParam *terrainMinHeightParam;
ShaderParam *terrainMaxHeightParam;
ShaderParam *terrainColorParam;


entityx::Entity particleEntity;
entityx::ComponentHandle<ParticleEffect> particleEffect;
entityx::ComponentHandle<Geometry> particleGeom;
entityx::ComponentHandle<Material> particleMaterial;

ShaderParam *particleViewMatrixParam;
ShaderParam *particleProjMatrixParam;
ShaderParam *particleColorParam;
ShaderParam *particleSizeParam;

// Others
AABoundingBox terrainBoundingBox;

CameraController cameraController;
SeasonController seasonController;


void initScene()
{
    camera = std::make_unique<Camera>();
    camera->setEyePos({8, 20, 8});

    // Create terrain
    terrainEntity = scene.createEntity();
    terrainGeom = terrainEntity.assign<Geometry>();
    *terrainGeom.get() = heightmapToGeometry(QImage("images/heightmap-1.png"));

    terrainBoundingBox.processVertices(terrainGeom->vertices);

    VertexAttrib standardVertexAttrib {"vertexPos", 3, VertexAttrib::Type::Float, false, 0};
    terrainGeom->vertexLayout.addAttribute(standardVertexAttrib);

    terrainMaterial = terrainEntity.assign<Material>();
    RenderPass *terrainPass = terrainMaterial->addRenderPass("base");
    uptr<ShaderProgram> terrainShader = shaderProgramFromFile("://res/shaders/terrain_heightmap.vert",
                                                              "",
                                                              "://res/shaders/terrain_heightmap.frag");
    terrainPass->setShaderProgram(std::move(terrainShader));

    terrainMatWorldMatParam   =   terrainPass->addParam("worldMatrix", QMatrix4x4());
    terrainMinHeightParam     =   terrainPass->addParam("minHeight", 0.f);
    terrainMaxHeightParam     =   terrainPass->addParam("maxHeight", 1.f);
    terrainColorParam         =   terrainPass->addParam("terrainColor", QColor());



    // Create particle effect
    particleEntity = createParticleEffect(scene, {0, 400, 0}, {0, -1, 0},
                                          50, 100, terrainBoundingBox.radius().z(),
                                          0.4f, 4.f);
    particleEffect = particleEntity.component<ParticleEffect>();
    particleMaterial = particleEntity.component<Material>();
}

void gatherShadersParams()
{
    const QMatrix4x4 viewMatrix = camera->viewMatrix();
    const QMatrix4x4 projectionMatrix = camera->projectionMatrix();
    const QMatrix4x4 worldMatrix = projectionMatrix * viewMatrix;

    const QVector3D terrainAABBCenter = terrainBoundingBox.center();
    const QVector3D terrainAABBRadius = terrainBoundingBox.radius();

    const float minHeight = terrainAABBCenter.y() - terrainAABBRadius.y();
    const float maxHeight = terrainAABBCenter.y() + terrainAABBRadius.y();

    const QColor drawColor = seasonController.colorFromSeason();

    // Update terrain material parameters
    terrainMatWorldMatParam->value = worldMatrix;
    terrainMinHeightParam->value = minHeight;
    terrainMaxHeightParam->value = maxHeight;
    terrainColorParam->value = drawColor;

    // Update particle material parameters
    RenderPass *particleBasePass = particleMaterial->renderPasses()[0].get();
    particleBasePass->setParam("viewMatrix", viewMatrix);
    particleBasePass->setParam("projectionMatrix", projectionMatrix);
    particleBasePass->setParam("particleColor", drawColor);
    particleBasePass->setParam("particleSize", particleEffect->particleSize());
}

void iterateGameLoop(float dt)
{
    // Update camera
    cameraController.updateCamera(camera.get(), dt);

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

    renderWidget = new RenderWidget;
    renderWidget->installEventFilter(&cameraController);
    renderWidget->setMinimumSize(640, 400);
    createFpsLabel(&gameLoop, renderWidget);

    systemEngine.registerSystem<entityx::deps::Dependency<ParticleEffect, Geometry, Material>>();
    systemEngine.registerSystem<ParticleSystem>();
    systemEngine.registerSystem<RenderSystem>(renderWidget);
    systemEngine.initialize();

    initScene();

    renderWidget->setCamera(camera.get());
    renderWidget->show();

    centerCameraOnBBox(camera.get(), terrainBoundingBox);

    gameLoop.run();
    seasonController.start();

    return app.exec();
}
