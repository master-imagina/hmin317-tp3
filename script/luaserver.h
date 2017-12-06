#ifndef LUASERVER_H
#define LUASERVER_H

#include <unordered_map>

struct lua_State;

class QString;

namespace luabridge {
class LuaRef;
} // namespace luabridge

class Script;


using LuaKeyValueMap = std::unordered_map<std::string, luabridge::LuaRef>;


class LuaServer
{
public:
    LuaServer();
    ~LuaServer() = default;

    void evaluateScript(const Script &script, QString *errorMsg = nullptr);

    luabridge::LuaRef getUpdateFunc() const;
    luabridge::LuaRef getPropertiesTable() const;
    LuaKeyValueMap getKeyValueMap(const luabridge::LuaRef &table);

private:
    lua_State *m_luaState;
};

#endif // LUASERVER_H
