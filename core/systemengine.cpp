#include "systemengine.h"

#include "scene.h"


SystemEngine::SystemEngine(Scene &scene) :
    m_scene(scene),
    m_systemManager(scene.m_entityManager, scene.m_eventManager)
{}

void SystemEngine::configure()
{
    m_systemManager.configure();
}
