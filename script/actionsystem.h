#ifndef FRAMESYSTEM_H
#define FRAMESYSTEM_H

#include "3rdparty/entityx/System.h"


class ActionSystem : public entityx::System<ActionSystem>
{
public:
    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override;
};

#endif // FRAMESYSTEM_H
