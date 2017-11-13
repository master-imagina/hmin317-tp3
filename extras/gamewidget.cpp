#include "gamewidget.h"

#include <QApplication>

#include "3rdparty/entityx/deps/Dependencies.h"

#include "core/scene.h"

#include "extras/cameracontroller.h"

#include "extras/particles/particleeffect.h"
#include "extras/particles/particlesystem.h"

#include "render/rendersystem.h"
#include "render/transform.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"
#include "render/material/shaderutils.h"

#include "render/renderer/renderer.h"


GameWidget::GameWidget(Scene &scene, QWidget *parent) :
    RenderWidget(parent),
    m_systemEngine(scene),
    m_gameLoop(new GameLoop(60)),
    m_cameraController(new CameraController(this))
{
    installEventFilter(m_cameraController);

    initSystems();

    m_gameLoop->setCallback([this] (float dt) { iterateGameLoop(dt); });
    m_gameLoop->run();
}

GameLoop *GameWidget::gameLoop() const
{
    return m_gameLoop;
}

void GameWidget::initSystems()
{
    // Add system dependencies
    m_systemEngine.registerSystem<entityx::deps::Dependency<Geometry, Transform>>();
    m_systemEngine.registerSystem<entityx::deps::Dependency<ParticleEffect, Geometry, Material>>();

    // Add systems
    m_systemEngine.registerSystem<ParticleSystem>();
    m_systemEngine.registerSystem<RenderSystem>(this);
    m_systemEngine.initialize();
}

void GameWidget::iterateGameLoop(float dt)
{
    m_cameraController->updateCamera(m_camera, dt);

    // Render
    m_systemEngine.update<ParticleSystem>(dt);
    m_systemEngine.update<RenderSystem>(dt);

    //WARNING Comment from ingame build
    //TODO Implement multithread rendering to remove that
    qApp->processEvents();
}
