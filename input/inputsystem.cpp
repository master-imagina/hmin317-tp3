#include "inputsystem.h"

#include "input/eventhook.h"
#include "input/keyboard.h"


InputSystem::InputSystem(QObject *target) :
    m_target(target),
    m_eventHook(nullptr)
{}

void InputSystem::configure(entityx::EntityManager &,
                            entityx::EventManager &)
{
    m_eventHook = new EventHook(m_target);

    m_target->installEventFilter(m_eventHook);
}

void InputSystem::update(entityx::EntityManager &entities,
                         entityx::EventManager &events,
                         double dt)
{
    entities.each<Keyboard>(
                [this] (entityx::Entity entity, Keyboard &keyboard) {
        if (!keyboard.m_theEventHook) {
            keyboard.m_theEventHook = m_eventHook;
        }
    });
}
