#ifndef LUAQVARIANT_BRIDGE
#define LUAQVARIANT_BRIDGE

#include <QVariant>

#include "script/lua_includes.h"


QVariant luaRefToVariant(const luabridge::LuaRef &luaValue);

void commitParamValueToLua(const std::string &name,
                           const QVariant &qtValue,
                           luabridge::LuaRef &luaPropsTable);

#endif // LUAQVARIANT_BRIDGE
