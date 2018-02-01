#ifndef LUAQVARIANT_BRIDGE
#define LUAQVARIANT_BRIDGE

#include <QVariant>

#include "script/lua_includes.h"

namespace entityx {
class EntityManager;
} // namespace name


QVariant luaRefToVariant(const luabridge::LuaRef &luaValue);

void commitParamValueToLua(const std::string &name,
                           const QVariant &qtValue,
                           luabridge::LuaRef &luaPropsTable,
                           entityx::EntityManager &entityManager);

#endif // LUAQVARIANT_BRIDGE
