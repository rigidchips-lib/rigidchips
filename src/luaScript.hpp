#ifndef G_LUASCRIPT_H
#define G_LUASCRIPT_H
#include "Globals.hpp"
#include "Consts.hpp"
#include "GVector.hpp"
#include "GRigid.hpp"
#include "readData.hpp"

#define lua_c

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}





lua_State *luaScriptInit(char *buff);
void luaScriptEnd(lua_State *L);
int luaScriptRun(lua_State *L, char *funcName);
#endif
