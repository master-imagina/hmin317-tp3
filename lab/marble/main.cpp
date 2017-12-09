#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QSurfaceFormat>
#include <QWindow>

#include "core/assetmanager.h"
#include "core/param.h"
#include "core/scene.h"

#include "editor/gui/gamewidget.h"
#include "editor/gui/fpswidgets.h"

#include "extras/cameraactions.h"
#include "extras/grid.h"
#include "extras/heightmap.h"

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
#include "render/material/texture.h"

#include "script/luaserver.h"
#include "script/script.h"
#include "script/scriptassets.h"


void fetchBigFile()
{
    const std::string bigFileName = "data_marble.pak";

    createBigFile("data_marble", bigFileName);

    AssetManager::self()->loadBigFile(bigFileName);
}

void initScene(Scene &scene, LuaServer &luaServer)
{
    // Create terrain
    entityx::Entity terrainEntity = scene.createEntity();

    //  Terrain geometry
    auto terrainGeom = terrainEntity.assign<Geometry>(grid(32));

    terrainGeom->vertexLayout.addAttribute(defaultPositionAttrib());

    //  Terrain material
    auto terrainMaterial = terrainEntity.assign<Material>();
    RenderPass &terrainPass = terrainMaterial->addRenderPass("base");
    ShaderProgram terrainShader =
            shaderProgramFromFile("shaders/terrain_heightmap.vert",
                                  "shaders/terrain_wireframe.frag");

    terrainPass.setShaderProgram(terrainShader);

    // Create camera
    AABoundingBox terrainBB(terrainGeom->vertices);
    entityx::Entity mainCameraEntity = scene.createEntity();
    auto camera = mainCameraEntity.assign<Camera>();

    centerCameraOnBBox(*camera.get(), terrainBB);

    // Create player
    entityx::Entity playerEntity = scene.createEntity();
    playerEntity.assign<Mesh>("meshes/sphere.ply");

    auto playerTransform = playerEntity.component<Transform>();
    playerTransform->setTranslate(terrainBB.center());

    playerEntity.assign<Keyboard>();
    auto playerScript = playerEntity.assign<Script>(scriptFromFile("scripts/player.lua", luaServer));
    playerScript->setParam("cameraEntity", QVariant::fromValue(mainCameraEntity));

    // Create light
    entityx::Entity lightEntity = scene.createEntity();
    entityx::ComponentHandle<Light> light = lightEntity.assign<Light>();
    light->color = QColor(Qt::red);
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
    app.setApplicationName("Marble");

    // Create game widget and scene
    Scene scene;
    LuaServer luaServer;
    GameWidget gameWidget(scene, &luaServer);

    initScene(scene, luaServer);
    gameWidget.systemEngine().configure();

    // Show game widget
    gameWidget.setMinimumSize(640, 400);
    createFpsLabel(gameWidget.gameLoop(), &gameWidget);

    gameWidget.show();

    return app.exec();
}
