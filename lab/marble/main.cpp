#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QSurfaceFormat>

#include "core/assetmanager.h"
#include "core/param.h"
#include "core/scene.h"

#include "editor/gui/gamewidget.h"
#include "editor/gui/fpswidgets.h"

#include "extras/cameraactions.h"
#include "extras/grid.h"
#include "extras/heightmap.h"
#include "extras/serialization.h"

#include "input/keyboard.h"

#include "physics/collider.h"
#include "physics/rigidbody.h"

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
    auto terrainMesh = terrainEntity.assign<Mesh>("meshes/map1.obj");
    AABoundingBox terrainBB = meshAABB(*terrainMesh.get());

//    terrainEntity.assign<Collider>(Collider::Type::Box, terrainBB.radius());
    terrainEntity.assign<Collider>(Collider::Type::Mesh);

    auto terrainRigidBody = terrainEntity.assign<RigidBody>();
    terrainRigidBody->restitution = 0.7f;

    //  Terrain material
    auto terrainMaterial = terrainEntity.assign<Material>();
    RenderPass &terrainPass = terrainMaterial->addRenderPass("base");
    ShaderProgram terrainShader =
            shaderProgramFromFile("shaders/terrain_heightmap.vert",
                                  "shaders/terrain_wireframe.frag");

    terrainPass.setShaderProgram(terrainShader);

    // Create camera
    entityx::Entity mainCameraEntity = scene.createEntity();
    auto camera = mainCameraEntity.assign<Camera>();

    centerCameraOnBBox(*camera.get(), terrainBB);

    // Create player
    entityx::Entity playerEntity = scene.createEntity();
    auto playerMesh = playerEntity.assign<Mesh>("meshes/sphere.obj");

    auto playerTransform = playerEntity.component<Transform>();
    playerTransform->setTranslate({-9, 5, 0});

    AABoundingBox playerBB = meshAABB(*playerMesh.get());

    playerEntity.assign<Collider>(Collider::Type::Sphere, playerBB.radius());

    auto playerRigidBody = playerEntity.assign<RigidBody>();
    playerRigidBody->mass = 1.f;
    playerRigidBody->linearDamping = 0.1f;
    playerRigidBody->friction = 0.2f;

    playerEntity.assign<Keyboard>();
    auto playerScript = playerEntity.assign<Script>(scriptFromFile("scripts/player.lua", luaServer));
    playerScript->setParam("cameraEntity", QVariant::fromValue(mainCameraEntity.id()));

    // Create light
    entityx::Entity lightEntity = scene.createEntity();
    auto light = lightEntity.assign<Light>();
    light->direction = {0, -1, 0};
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

//    gameWidget.show();
    gameWidget.showMaximized();

    QFile projectFile("test_project");
    if (!projectFile.open(QIODevice::WriteOnly)) {
        std::cerr << "[EDITOR - ERROR] << can't save project file"
                  << std::endl;
    }

    QDataStream dataStream(&projectFile);
    dataStream << scene;

    return app.exec();
}
