#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "3rdparty/entityx/System.h"

struct lua_State;


class ScriptSystem : public entityx::System<ScriptSystem>
{
public:
    void configure(entityx::EntityManager &entities,
                   entityx::EventManager &events) override;

    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override;

private:
    lua_State *m_luaState = nullptr;
};

#endif // SCRIPTSYSTEM_H
