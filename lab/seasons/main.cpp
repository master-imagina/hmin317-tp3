#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>

#include "core/scene.h"

#include "editor/fpswidgets.h"

#include "extras/cameraactions.h"
#include "extras/gamewidget.h"
#include "extras/heightmap.h"

#include "extras/particles/particleeffect.h"
#include "extras/particles/quick.h"

#include "render/aabb.h"
#include "render/camera.h"
#include "render/transform.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderutils.h"

#include "seasoncontroller.h"


// Global attributes
Camera camera;

AABoundingBox terrainBoundingBox;

ShaderParam *terrainColorParam;

entityx::ComponentHandle<ParticleEffect> particleEffect;
entityx::ComponentHandle<Material> particleMaterial;


void initScene(Scene &scene)
{
    camera.setEyePos({8, 20, 8});

    // Create terrain
    entityx::Entity terrainEntity = scene.createEntity();

    //  Terrain geometry
    auto terrainGeom = terrainEntity.assign<Geometry>();
    *terrainGeom.get() = heightmapToGeometry(QImage("images/heightmap-1.png"));

    terrainBoundingBox.processVertices(terrainGeom->vertices);

    VertexAttrib standardVertexAttrib {"vertexPos", 3, VertexAttrib::Type::Float, false, 0};
    terrainGeom->vertexLayout.addAttribute(standardVertexAttrib);

    //  Terrain material
    auto terrainMaterial = terrainEntity.assign<Material>();
    RenderPass *terrainPass = terrainMaterial->addRenderPass("base");
    uptr<ShaderProgram> terrainShader = shaderProgramFromFile("://res/shaders/terrain_heightmap.vert",
                                                              "",
                                                              "://res/shaders/terrain_heightmap.frag");
    terrainPass->setShaderProgram(std::move(terrainShader));

    const QVector3D terrainAABBCenter = terrainBoundingBox.center();
    const QVector3D terrainAABBRadius = terrainBoundingBox.radius();

    const float minHeight = terrainAABBCenter.y() - terrainAABBRadius.y();
    const float maxHeight = terrainAABBCenter.y() + terrainAABBRadius.y();

    terrainPass->setParam("minHeight", minHeight);
    terrainPass->setParam("maxHeight", maxHeight);
    terrainColorParam = terrainPass->addParam("terrainColor", QColor());


    // Create particle effect
    entityx::Entity particleEntity =
            createParticleEffect(scene, {0, 400, 0}, {0, -1, 0},
                                 50, 100, terrainBoundingBox.radius().z(),
                                 0.4f, 4.f);

    particleEffect = particleEntity.component<ParticleEffect>();
    particleMaterial = particleEntity.component<Material>();


    // Center camera above terrain
    centerCameraOnBBox(camera, terrainBoundingBox);
}

void onSeasonChanged(const QColor &seasonColor)
{
    // Update terrain material parameters
    terrainColorParam->value = seasonColor;

    // Update particle material parameters
    RenderPass *particleBasePass = particleMaterial->renderPasses()[0].get();

    particleBasePass->setParam("particleColor", seasonColor);
    particleBasePass->setParam("particleSize", particleEffect->particleSize());
}

int main(int argc, char *argv[])
{
    // Set OpenGL version
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(format);

    // Create app
    QApplication app(argc, argv);
    app.setApplicationName("Game Engines - PW3");

    // Create scene
    Scene scene;

    // Create game widget
    auto *gameWidget = new GameWidget(scene);
    gameWidget->setMinimumSize(640, 400);
    createFpsLabel(gameWidget->gameLoop(), gameWidget);

    // Populate scene
    initScene(scene);

    // Show game widget
    gameWidget->setCamera(&camera);
    gameWidget->show();

    // Glue the seasons logic
    SeasonController seasonController;

    QObject::connect(&seasonController, &SeasonController::seasonChanged,
                     &onSeasonChanged);

    seasonController.start();

    return app.exec();
}
