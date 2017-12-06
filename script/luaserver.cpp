#include "luaserver.h"

#include <iostream>

#include <QString>

extern "C" {
#include <lua5.3.4/lua.h>
#include <lua5.3.4/lauxlib.h>
#include <lua5.3.4/lualib.h>
}

#include "3rdparty/luabridge/luabridge/LuaBridge.h"

#include "script/api.h"
#include "script/script.h"


LuaServer::LuaServer() :
    m_luaState(nullptr)
{
    m_luaState = luaL_newstate();

    luaL_openlibs(m_luaState);

    exposeEngineAPI(m_luaState);
}

void LuaServer::evaluateScript(const Script &script, QString *errorMsg)
{
    const int status = luaL_dostring(m_luaState, script.sourceCode.constData());

    if (status != 0) {
        QString rawErrorMsg = lua_tostring(m_luaState, -1);
        rawErrorMsg.remove(0, rawErrorMsg.indexOf(']') + 2);

        std::cerr << "[ERROR] LuaServer: in " << script.path << ": "
                  << rawErrorMsg.toStdString()
                  << std::endl;

        if (errorMsg) {
            *errorMsg = rawErrorMsg;
        }

        lua_pop(m_luaState, 1);
    }
}

luabridge::LuaRef LuaServer::getUpdateFunc() const
{
    return luabridge::getGlobal(m_luaState, "update");
}

LuaKeyValueMap LuaServer::getKeyValueMap(const luabridge::LuaRef &table)
{
    LuaKeyValueMap ret;

    if (table.isNil()) {
        return ret;
    }

    push(m_luaState, table); // push table

    lua_pushnil(m_luaState);  // push nil, so lua_next removes it from stack and puts (k, v) on stack

    while (lua_next(m_luaState, -2) != 0) { // -2, because we have table at -1
        if (lua_isstring(m_luaState, -2)) { // only store stuff with string keys
            ret.emplace(lua_tostring(m_luaState, -2), luabridge::LuaRef::fromStack(m_luaState, -1));
        }

        lua_pop(m_luaState, 1); // remove value, keep key for lua_next
    }

    lua_pop(m_luaState, 1); // pop table

    return ret;
}

luabridge::LuaRef LuaServer::getPropertiesTable() const
{
    return luabridge::getGlobal(m_luaState, "properties");
}
