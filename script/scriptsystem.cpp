#include "scriptsystem.h"

extern "C" {
#include <lua5.3.4/lua.h>
#include <lua5.3.4/lauxlib.h>
#include <lua5.3.4/lualib.h>
}

#include "3rdparty/luabridge/luabridge/LuaBridge.h"

#include "script/api.h"
#include "script/script.h"


namespace {

void luaReportErrors(lua_State *L, int status)
{
    if (status != 0) {
        std::cerr << "[ERROR] ScriptSystem: "
                  << lua_tostring(L, -1);
        lua_pop(L, 1);
    }
}

} // anon namespace


void ScriptSystem::configure(entityx::EntityManager &entities,
                             entityx::EventManager &events)
{
    if (!m_luaState) {
        m_luaState = luaL_newstate();

        luaL_openlibs(m_luaState);

        exposeEngineAPI(m_luaState);
    }
}

void ScriptSystem::update(entityx::EntityManager &entities,
                          entityx::EventManager &events,
                          double dt)
{
    entities.each<Script>(
                [this, dt] (entityx::Entity entity, Script &script) {
        const int lscript = luaL_dostring(m_luaState, script.sourceCode.constData());

        luaReportErrors(m_luaState, lscript);

        luabridge::LuaRef updateHandler = luabridge::getGlobal(m_luaState, "update");
        updateHandler(entity, (float) dt);
    });
}
