#include "systemengine.h"

#include "scene.h"


SystemEngine::SystemEngine(Scene &scene) :
    m_scene(scene),
    m_systemManager(scene.entities, scene.events)
{}

void SystemEngine::configure()
{
    m_systemManager.configure();
}
