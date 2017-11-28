#include "actionsystem.h"

#include "script/frameaction.h"


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
