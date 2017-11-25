#include "gamewidget.h"

#include "3rdparty/entityx/deps/Dependencies.h"

#include "core/scene.h"

#include "extras/cameracontroller.h"

#include "extras/particles/particleeffect.h"
#include "extras/particles/particlesystem.h"

#include "render/light.h"
#include "render/mesh.h"
#include "render/transform.h"
#include "render/rendersystem.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"

#include "render/renderer/renderer.h"


GameWidget::GameWidget(Scene &scene, QWidget *parent) :
    RenderWidget(parent),
    m_initialized(false),
    m_systemEngine(scene),
    m_gameLoop(new GameLoop(60, this)),
    m_cameraController(new CameraController(this))
{
    installEventFilter(m_cameraController);

    initSystems();
}

GameLoop *GameWidget::gameLoop() const
{
    return m_gameLoop;
}

SystemEngine &GameWidget::systemEngine()
{
    return m_systemEngine;
}

void GameWidget::showEvent(QShowEvent *)
{
    if (!m_initialized) {
        m_gameLoop->setCallback([this] (float dt) { iterateGameLoop(dt); });
        m_gameLoop->run();

        m_initialized = true;
    }
}

void GameWidget::initSystems()
{
    // Add system dependencies
    m_systemEngine.registerSystem<entityx::deps::Dependency<Geometry, Transform>>();
    m_systemEngine.registerSystem<entityx::deps::Dependency<ParticleEffect, Geometry, Material>>();
    m_systemEngine.registerSystem<entityx::deps::Dependency<Mesh, Transform>>();
    m_systemEngine.registerSystem<entityx::deps::Dependency<Light, Transform>>();

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
}
