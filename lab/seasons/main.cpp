#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QSurfaceFormat>

#include "core/assetmanager.h"
#include "core/scene.h"

#include "editor/gui/gamewidget.h"
#include "editor/gui/fpswidgets.h"

#include "extras/cameraactions.h"
#include "extras/grid.h"
#include "extras/heightmap.h"

#include "extras/particles/particleeffect.h"
#include "extras/particles/quick.h"

#include "input/keyboard.h"

#include "render/aabb.h"
#include "render/renderassets.h"
#include "render/camera.h"
#include "render/light.h"
#include "render/mesh.h"
#include "render/transform.h"

#include "render/geometry/geometry.h"

#include "render/material/builtins.h"
#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"
#include "render/material/texture.h"

#include "script/luaserver.h"
#include "script/script.h"
#include "script/scriptassets.h"

#include "seasoncontroller.h"


// Global attributes
AABoundingBox terrainBoundingBox;

ShaderParam *terrainColorParam;

entityx::ComponentHandle<Camera> camera;
entityx::ComponentHandle<ParticleEffect> particleEffect;
entityx::ComponentHandle<Material> particleMaterial;


void fetchBigFile()
{
    const std::string bigFileName = "data_seasons.pak";

    createBigFile("data_seasons", bigFileName);

    AssetManager::self()->loadBigFile(bigFileName);
}

void initScene(Scene &scene)
{
    entityx::Entity mainCameraEntity = scene.createEntity();
    camera = mainCameraEntity.assign<Camera>();
    mainCameraEntity.assign<Script>(scriptFromFile("builtins/scripts/free_camera.lua"));
    mainCameraEntity.assign<Keyboard>();

    // Create terrain
    entityx::Entity terrainEntity = scene.createEntity();

    //  Terrain geometry
    auto terrainGeom = terrainEntity.assign<Geometry>();
    *terrainGeom.get() = heightmapToGeometry(imageFromFile("images/dunes.jpg"));

    terrainBoundingBox.processVertices(terrainGeom->vertices);

    terrainGeom->vertexLayout.addAttribute(defaultPositionAttrib());

    //  Terrain material
    auto terrainMaterial = terrainEntity.assign<Material>();
    RenderPass &terrainPass = terrainMaterial->addRenderPass("base");
    ShaderProgram terrainShader =
            shaderProgramFromFile("shaders/terrain_heightmap.vert",
                                  "shaders/terrain_heightmap.frag");

    terrainPass.setShaderProgram(terrainShader);

    const QVector3D terrainAABBCenter = terrainBoundingBox.center();
    const QVector3D terrainAABBRadius = terrainBoundingBox.radius();

    const float minHeight = terrainAABBCenter.y() - terrainAABBRadius.y();
    const float maxHeight = terrainAABBCenter.y() + terrainAABBRadius.y();

    terrainMaterial->setParam("minHeight", minHeight);
    terrainMaterial->setParam("maxHeight", maxHeight);
    terrainColorParam = &terrainMaterial->addParam("terrainColor", QColor());

    // Create particle effect
    entityx::Entity particleEntity = scene.createEntity();

    createParticleEffect(particleEntity, {0, -1, 0},
                         240, 100, terrainBoundingBox.radius().z(),
                         0.3f, 4.f);
    particleEntity.component<Transform>()->setTranslate({0, maxHeight * 2, 0});

    particleEffect = particleEntity.component<ParticleEffect>();
    particleEffect->setSpawnRate(2);

    particleMaterial = particleEntity.component<Material>();

    // Create tree
    entityx::Entity treeEntity = scene.createEntity();
    treeEntity.assign<Mesh>("meshes/wintertree.ply");
    auto treeTransform = treeEntity.component<Transform>();
    treeTransform->setTranslate(terrainAABBCenter);
    treeTransform->setScale({100, 100, 100});
    treeTransform->setRotation({-90, 0, 0});

    entityx::Entity treeEntity2 = scene.createEntity();
    treeEntity2.assign<Mesh>("meshes/springtree.ply");
    auto treeTransform2 = treeEntity2.component<Transform>();
    treeTransform2->setTranslate(terrainAABBCenter + QVector3D(100, 0, 200));
    treeTransform2->setScale({80, 80, 80});
    treeTransform2->setRotation({-85, 0, 0});

    entityx::Entity treeEntity3 = scene.createEntity();
    treeEntity3.assign<Mesh>("meshes/autumntree.ply");
    auto treeTransform3 = treeEntity3.component<Transform>();
    treeTransform3->setTranslate(terrainAABBCenter +  QVector3D(140, 0, 50));
    treeTransform3->setScale({90, 90, 90});
    treeTransform3->setRotation({-74, 5, 3});

//    treeEntity.assign<Material>(phongMaterial());

    // Create light
    entityx::Entity lightEntity = scene.createEntity();
    lightEntity.assign<Light>();
    lightEntity.component<Light>()->color = QColor(Qt::red);

    // Center camera above terrain
    AABoundingBox a(terrainGeom->vertices);
    centerCameraOnBBox(*camera.get(), a);
}

void onSeasonChanged(Season season)
{
    QColor seasonColor;

    Texture2D particleTexture;
    float textureFlag = 1.f;
    float particleSize = 1.f;

    switch (season) {
    case Season::Autumn:
        seasonColor = QColor(244, 183, 51);
        particleTexture.path = "images/autumn_leaf.png";
        particleSize = 15.f;
        break;
    case Season::Winter:
        seasonColor = Qt::white;
        particleTexture.path = "images/winter_flake.png";
        particleSize = 10.f;
        break;
    case Season::Spring:
        seasonColor = Qt::green;
        particleTexture.path = "images/spring_leaf.png";
        particleSize = 8.f;
        break;
    case Season::Summer:
        seasonColor = Qt::yellow;
        textureFlag = 0.f;
        break;
    default:
        break;
    }

    terrainColorParam->value = seasonColor;

    particleMaterial->setParam("particleColor", seasonColor);
    particleMaterial->setParam("particleTexture", QVariant::fromValue(particleTexture));
    particleMaterial->setParam("textureFlag", textureFlag);
    particleMaterial->setParam("particleSize", particleSize);
}


int main(int argc, char *argv[])
{
    fetchBigFile();

    // Set OpenGL version
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(format);

    // Create app
    QApplication app(argc, argv);
    app.setApplicationName("Seasons");

    // Create game widget and scene
    Scene scene;
    LuaServer luaServer;
    GameWidget gameWidget(scene, &luaServer);

    initScene(scene);
    gameWidget.systemEngine().configure();

    // Show game widget
    gameWidget.setMinimumSize(640, 400);
    createFpsLabel(gameWidget.gameLoop(), &gameWidget);

    gameWidget.show();

    // Glue the seasons logic
    SeasonController seasonController;

    QObject::connect(&seasonController, &SeasonController::seasonChanged,
                     &onSeasonChanged);

    seasonController.start();

    return app.exec();
}
