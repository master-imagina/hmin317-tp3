#ifndef INPUTSYSTEM_h
#define INPUTSYSTEM_h

#include <entityx/System.h>

class QObject;

class EventHook;


class InputSystem : public entityx::System<InputSystem>
{
public:
    explicit InputSystem(QObject *target);

    void configure(entityx::EntityManager &entities,
                   entityx::EventManager &events) override;

    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override;

private:
    QObject *m_target;
    EventHook *m_eventHook;
};

#endif // INPUTSYSTEM_h
