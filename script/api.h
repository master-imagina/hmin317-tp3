#ifndef SCRIPT_API_H
#define SCRIPT_API_H


struct lua_State;


void exposeEngineAPI(lua_State *lState);

#endif // SCRIPT_API_H
