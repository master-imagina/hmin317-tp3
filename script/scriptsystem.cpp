#include "scriptsystem.h"

#include <QDebug>

#include "core/scene.h"

#include "script/lua_includes.h"
#include "script/luaqvariant_bridge.h"
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
                [&entities, this, dt] (entityx::Entity entity, Script &script) {
        if (!script.path.empty()) {
            m_luaServer.evaluateScript(script);

            // Pass script properties to LUA
            luabridge::LuaRef propsTable = m_luaServer.getPropertiesTable();

            for (const Param &param : script.params()) {
                commitParamValueToLua(param.name, param.value, propsTable, entities);
            }

            // Call update
            luabridge::LuaRef updateHandler = m_luaServer.getUpdateFunc();
            updateHandler(entity, (float) dt);
        }
    });
}

LuaServer &ScriptSystem::luaServer()
{
    return m_luaServer;
}
