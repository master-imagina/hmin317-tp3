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
