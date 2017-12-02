#include "scriptsystem.h"

extern "C" {
#include <lua5.3.4/lua.h>
#include <lua5.3.4/lauxlib.h>
#include <lua5.3.4/lualib.h>
}

#include "3rdparty/luabridge/luabridge/LuaBridge.h"

#include "script/script.h"


ScriptSystem::ScriptSystem() :
    entityx::System<ScriptSystem>(),
    m_luaServer()
{}

void ScriptSystem::update(entityx::EntityManager &entities,
                          entityx::EventManager &events,
                          double dt)
{
    entities.each<Script>(
                [this, dt] (entityx::Entity entity, Script &script) {
        if (!script.path.empty()) {
            m_luaServer.evaluateScript(script);

            luabridge::LuaRef updateHandler = m_luaServer.getUpdateFunc();
            updateHandler(entity, (float) dt);
        }
    });
}

LuaServer &ScriptSystem::luaServer()
{
    return m_luaServer;
}
