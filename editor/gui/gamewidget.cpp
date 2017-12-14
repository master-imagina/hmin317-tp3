#include "gamewidget.h"

#include "core/scene.h"

#include "extras/particles/particleeffect.h"
#include "extras/particles/particlesystem.h"

#include "input/inputsystem.h"

#include "render/light.h"
#include "render/mesh.h"
#include "render/transform.h"
#include "render/rendersystem.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"

#include "render/renderer/renderer.h"

#include "script/scriptsystem.h"


GameWidget::GameWidget(Scene &scene, LuaServer *luaServer, QWidget *parent) :
    RenderWidget(parent),
    m_initialized(false),
    m_systemEngine(scene),
    m_theLuaServer(luaServer),
    m_gameLoop(new GameLoop(60, this)),
    m_playModeEnabled(true)
{
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

void GameWidget::enablePlayMode()
{
    m_playModeEnabled = true;
}

void GameWidget::disablePlayMode()
{
    m_playModeEnabled = false;
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
    m_systemEngine.registerDependency<Geometry, Transform>();
    m_systemEngine.registerDependency<ParticleEffect, Geometry, Material>();
    m_systemEngine.registerDependency<Mesh, Transform>();
    m_systemEngine.registerDependency<Light, Transform>();

    // Add systems
    m_systemEngine.registerSystem<InputSystem>(this);

    if (m_theLuaServer) {
        m_systemEngine.registerSystem<ScriptSystem>();
    }

    m_systemEngine.registerSystem<ParticleSystem>();
    m_systemEngine.registerSystem<RenderSystem>(this);

    m_systemEngine.configure();
}

void GameWidget::iterateGameLoop(float dt)
{
    m_systemEngine.update<InputSystem>(dt);

    if (m_theLuaServer && m_playModeEnabled) {
        m_systemEngine.update<ScriptSystem>(dt);
    }

    m_systemEngine.update<ParticleSystem>(dt);
    m_systemEngine.update<RenderSystem>(dt);
}
