#include "scriptsystem.h"

#include <QDebug>
#include <QVector3D>

extern "C" {
#include <lua5.3.4/lua.h>
#include <lua5.3.4/lauxlib.h>
#include <lua5.3.4/lualib.h>
}

#include "3rdparty/luabridge/luabridge/LuaBridge.h"

#include "script/script.h"


namespace {

void commitParamValueToLua(const std::string &name,
                           const QVariant &qtValue,
                           luabridge::LuaRef &luaPropsTable)
{
    const int type = qtValue.userType();

    if (type == QMetaType::Int) {
        luaPropsTable[name] = qtValue.toInt();
    }
    else if (type == QMetaType::Float) {
        luaPropsTable[name] = qtValue.toFloat();
    }
    else if (type == QMetaType::QVector3D) {
        luaPropsTable[name] = qtValue.value<QVector3D>();
    }
}

} // anon namespace


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

            // Pass script properties to LUA
            luabridge::LuaRef propsTable = m_luaServer.getPropertiesTable();

            for (const Param &param : script.params()) {
                commitParamValueToLua(param.name, param.value, propsTable);
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
