#ifndef G_LUASYSTEM_H
#define G_LUASYSTEM_H
//#include <dinput.h>
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
};



HRESULT LoadLand(LPDIRECT3DDEVICE9 Device, char *fname);
char *SearchFolder(const TCHAR *path, char *filename, char *result);
void ResetChip2(int n, GFloat a);
void setOption(GRigid *rigid, GFloat value);

int LoadSystem(char *fileName);
int luaSystemInit();
void luaSystemEnd();
int luaSystemRun(char *funcName);
#endif
