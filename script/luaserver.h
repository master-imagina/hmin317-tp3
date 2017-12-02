#ifndef LUASERVER_H
#define LUASERVER_H

struct lua_State;

class QString;

namespace luabridge {
class LuaRef;
} // namespace luabridge

class Script;


class LuaServer
{
public:
    LuaServer();
    ~LuaServer() = default;

    void evaluateScript(const Script &script, QString *errorMsg = nullptr);

    luabridge::LuaRef getUpdateFunc() const;

private:
    lua_State *m_luaState;
};

#endif // LUASERVER_H
