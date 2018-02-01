#ifndef SCRIPTASSETS_H
#define SCRIPTASSETS_H

#include "script/script.h"

class LuaServer;


Script scriptFromFile(const std::string &path, LuaServer &luaServer);

#endif // SCRIPTASSETS_H
