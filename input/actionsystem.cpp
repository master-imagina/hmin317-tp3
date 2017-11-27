#include "actionsystem.h"

#include "input/frameaction.h"


void ActionSystem::update(entityx::EntityManager &entities,
                          entityx::EventManager &events,
                          double dt)
{
    entities.each<FrameAction>(
                [dt] (entityx::Entity entity,
                      FrameAction &frameAction) {
        frameAction(entity, dt);
    });
}
