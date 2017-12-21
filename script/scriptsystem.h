#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include <entityx/System.h>

#include "script/luaserver.h"

struct lua_State;


class ScriptSystem : public entityx::System<ScriptSystem>
{
public:
    ScriptSystem();

    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override;

    LuaServer &luaServer();

private:
    LuaServer m_luaServer;
};

#endif // SCRIPTSYSTEM_H
