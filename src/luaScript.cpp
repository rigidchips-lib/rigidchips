#pragma warning(disable : 4996)
#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <D3DX9.h>
#include <DXErr9.h>
#include <tchar.h>
#include <dinput.h>
#include "Consts.hpp"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "D3DEnumeration.h"
#include "D3DSettings.h"
#include "D3DApp.h"
#include "D3DFont.h"
#include "D3DFile.h"
#include "DIUtil.h"
#include "DMUtil.h"
#include "DSUtil.h"
#include "D3Dtypes.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include "string.h"
#include "ctype.h"
#include "Globals.hpp"
#include "readData.hpp"
#include "luaScript.hpp"
#include "luaSub.hpp"

//���������[�N���o�p
#include <crtdbg.h>  
#ifdef _DEBUG 
#ifdef __MEMLEAKCHECK 
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__) 
#define malloc(p1) _malloc_dbg((p1),_NORMAL_BLOCK,__FILE__,__LINE__) 
#define __MEMLEAKCHECK
#endif
#endif 
//--���������[�N���o�p



// Lua�ɓo�^����C�̊֐��́Aint��Ԃ��Alua_State*���󂯎����̂ł���K�v������B
// �Ԃ��l�͖߂�l�̐��B���̏ꍇ���l��1�Ԃ��i�X�^�b�N�ɐςށj�̂�1�B

int luaPrint(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 1) return 0;
	int a = (int)lua_tonumber(L, 1);
	int i;
	char str[1000];
	if(a >= GOUTPUTMAX || a < 0) return 0;
	ScriptOutput[a][0] = '\0';
	for (i = 2; i <= n; i++) {
		if (lua_isnumber(L, i)) {
			sprintf(str, "%.2f", lua_tonumber(L, i));
		}
		else if (lua_isstring(L, i)) {
			sprintf(str, "%s", lua_tostring(L, i));
		}
		else if (lua_isnil(L, i)) {
			sprintf(str, "%s", "nil");
		}
		else if (lua_isboolean(L, i)) {
			sprintf(str, "%s", lua_toboolean(L, i) ? "true" : "false");
		}
		else {
			sprintf(str, "%s:%p", lua_typename(L, lua_type(L, i)), lua_topointer(L, i));
		}
		strcat(ScriptOutput[a], str);
	}
	return 0;
}
lua_State *luaScriptInit(char *buff) {
	ScriptErrorStr[0] = '\0';
	ScriptErrorCode = 0;
	setlocale(LC_ALL, "");
	//
	lua_State *L = lua_open();  /* create state */
	// �֐���o�^����
	lua_register(L, "out", luaPrint);

	lua_register(L, "_MX", luaGetMouseX);
	lua_register(L, "_MY", luaGetMouseY);
	lua_register(L, "_ML", luaGetMouseL);
	lua_register(L, "_MR", luaGetMouseR);
	lua_register(L, "_MM", luaGetMouseM);

	lua_register(L, "_DT", luaGetDt);
	lua_register(L, "_FPS", luaGetFps);
	lua_register(L, "_BASE", luaGetBase);
	lua_register(L, "_TICKS", luaGetTickCount);
	lua_register(L, "_SETTICKS", luaSetTicks);
	lua_register(L, "_KEY", luaKey);
	lua_register(L, "_KEYDOWN", luaKeyDown);
	lua_register(L, "_KEYUP", luaKeyUp);
	lua_register(L, "_ANALOG", luaAnalog);
	lua_register(L, "_HAT", luaHat);
	lua_register(L, "_CHIPS", luaChips);
	lua_register(L, "_WEIGHT", luaWeight);
	lua_register(L, "_WIDTH", luaGetWidth);
	lua_register(L, "_HEIGHT", luaGetHeight);
	lua_register(L, "_FACE", luaGetFaces);
	lua_register(L, "_ZOOM", luaSetCCDZoom);
	lua_register(L, "_OX", luaObjPosx);
	lua_register(L, "_OY", luaObjPosy);
	lua_register(L, "_OZ", luaObjPosz);
	lua_register(L, "_X", luaPosx);
	lua_register(L, "_Y", luaPosy);
	lua_register(L, "_Z", luaPosz);
	lua_register(L, "_H", luaGetY);
	lua_register(L, "_AX", luaAx);
	lua_register(L, "_AY", luaAy);
	lua_register(L, "_AZ", luaAz);
	lua_register(L, "_EX", luaEx);
	lua_register(L, "_EY", luaEy);
	lua_register(L, "_EZ", luaEz);
	lua_register(L, "_GX", luaGx);
	lua_register(L, "_GY", luaGy);
	lua_register(L, "_GZ", luaGz);
	lua_register(L, "_XX", luaXx);
	lua_register(L, "_XY", luaXy);
	lua_register(L, "_XZ", luaXz);
	lua_register(L, "_YX", luaYx);
	lua_register(L, "_YY", luaYy);
	lua_register(L, "_YZ", luaYz);
	lua_register(L, "_ZX", luaZx);
	lua_register(L, "_ZY", luaZy);
	lua_register(L, "_ZZ", luaZz);
	lua_register(L, "_QX", luaQx);
	lua_register(L, "_QY", luaQy);
	lua_register(L, "_QZ", luaQz);
	lua_register(L, "_QW", luaQw);
	lua_register(L, "_RX", luaRx);
	lua_register(L, "_RY", luaRy);
	lua_register(L, "_RZ", luaRz);
	lua_register(L, "_LX", luaLx);
	lua_register(L, "_LY", luaLy);
	lua_register(L, "_LZ", luaLz);
	lua_register(L, "_VX", luaVx);
	lua_register(L, "_VY", luaVy);
	lua_register(L, "_VZ", luaVz);
	lua_register(L, "_FX", luaFx);
	lua_register(L, "_FY", luaFy);
	lua_register(L, "_FZ", luaFz);
	lua_register(L, "_WX", luaWx);
	lua_register(L, "_WY", luaWy);
	lua_register(L, "_WZ", luaWz);
	lua_register(L, "_CCD", luaGetCCD);
	lua_register(L, "_RED", luaGetCCDRed);
	lua_register(L, "_GREEN", luaGetCCDGreen);
	lua_register(L, "_BLUE", luaGetCCDBlue);
	lua_register(L, "_BYE", luaUnLinkBye);
	lua_register(L, "_SPLIT", luaUnLink);
	lua_register(L, "_RND", luaRnd);
	lua_register(L, "_TODEG", luaToDeg);
	lua_register(L, "_TORAD", luaToRad);
	lua_register(L, "_TYPE", luaGetType);
	lua_register(L, "_OPTION", luaGetOption);
	lua_register(L, "_EFFECT", luaGetEffect);
	lua_register(L, "_USER1", luaGetUserEffect);
	lua_register(L, "_USER2", luaGetUserOption);
	lua_register(L, "_DIR", luaGetDir);
	lua_register(L, "_ANGLE", luaGetAngle);
	lua_register(L, "_POWER", luaGetPower);
	lua_register(L, "_SPRING", luaGetSpring);
	lua_register(L, "_DAMPER", luaGetDamper);
	lua_register(L, "_BRAKE", luaGetBrake);
	lua_register(L, "_COLOR", luaGetColor);
	lua_register(L, "_PARENT", luaGetParent);
	lua_register(L, "_TOP", luaGetTop);
	lua_register(L, "_M", luaGetM);
	lua_register(L, "_I", luaGetI);
	lua_register(L, "_MOBJ", luaGetObjM);
	lua_register(L, "_IOBJ", luaGetObjI);
	lua_register(L, "_E", luaGetEnergy);
	lua_register(L, "_T", luaGetTolerant);
	lua_register(L, "_MOVE3D", luaMove3D);
	lua_register(L, "_LINE3D", luaLine3D);
	lua_register(L, "_MOVE2D", luaMove2D);
	lua_register(L, "_LINE2D", luaLine2D);
	lua_register(L, "_SETCOLOR", luaSetColor);
	lua_register(L, "_PLAYERS", luaGetPlayers);
	lua_register(L, "_PLAYERHOSTID", luaGetPlayerHostID);
	lua_register(L, "_PLAYERMYID", luaGetPlayerMyID);
	lua_register(L, "_PLAYERID", luaGetPlayerID);
	lua_register(L, "_PLAYERCHIPS", luaGetPlayerChips);
	lua_register(L, "_PLAYERCRUSHES", luaGetPlayerCrushes);
	lua_register(L, "_PLAYERRESETS", luaGetPlayerResets);
	lua_register(L, "_PLAYERINITS", luaGetPlayerInits);
	lua_register(L, "_PLAYERCOLOR", luaGetPlayerColor);
	lua_register(L, "_PLAYERX", luaGetPlayerX);
	lua_register(L, "_PLAYERY", luaGetPlayerY);
	lua_register(L, "_PLAYERZ", luaGetPlayerZ);
	lua_register(L, "_PLAYERARMS", luaGetPlayerArms);
	lua_register(L, "_PLAYERYFORCES", luaGetPlayerYForces);
	lua_register(L, "_PLAYERNAME", luaGetPlayerName);
	lua_register(L, "_FUEL", luaGetFuel);
	lua_register(L, "_FUELMAX", luaGetFuelMax);

	luaL3dx = luaL3dy = luaL3dz = 0.0f;
	luaGraColor = 0xffffff;
	//�O���[�o���ϐ��̓o�^
	for (int i = 0;i < g_VarCount;i++) {
		lua_pushnumber(L, g_ValList[i].Val);
		lua_setglobal(L, g_ValList[i].Name);
	}
	//�X�N���v�g���Z�b�g����
	luaopen_string(L);
	luaopen_base(L);
	luaopen_table(L);
	luaopen_math(L);
	if (UseLuaExternal)
	{
		luaopen_io(L);
		luaopen_loadlib(L);
	}

	int e = lua_dobuffer(L, buff, strlen(buff), szUpdateFileName0);
	if (e != 0) {
		ScriptErrorCode = -1;
		sprintf(ScriptErrorStr, "%s\n", lua_tostring(L, 0));
		lua_close(L);
		return NULL;
	}
	return L;
}
void luaScriptEnd(lua_State *L) {
	if (L == NULL) return;
	lua_close(L);  /* create state */
	return;
}

int luaScriptRun(lua_State *L, char *funcName) {
	//	int status;
	//  struct Smain s;
		//�O���[�o���ϐ��̓o�^
	for (int i = 0;i < g_VarCount;i++) {
		lua_pushnumber(L, g_ValList[i].Val);
		lua_setglobal(L, g_ValList[i].Name);
	}
	// �O���[�o���e�[�u������main�֐����E���ăX�^�b�N�ɐς�
	lua_pushstring(L, funcName);
	lua_gettable(L, LUA_GLOBALSINDEX);
	if (lua_topointer(L, -1) == NULL) {//�����֐����Ȃ��Ȃ�
		lua_pop(L, 1);//�X�^�b�N��߂�
		if (strcmp(funcName, "OnFrame") == 0) {
			lua_pushstring(L, "main");
			lua_gettable(L, LUA_GLOBALSINDEX);
		}
		else return 0;
	}
	// �֐����ĂԁBlua_call�̑�2�����͓n�������̐��A��3�����͖߂�l�̐��B
	// �֐��Ƃ��̈����̓X�^�b�N�����菜����A�߂�l���X�^�b�N�Ɏc��B
	ScriptErrorCode = lua_pcall(L, 0, 0, 0);
	if (ScriptErrorCode)sprintf(ScriptErrorStr, "%s\n", lua_tostring(L, -1));
	for (int i = 0;i < g_VarCount;i++) {
		lua_pushstring(L, g_ValList[i].Name); // (1) Lua�̕ϐ���toC���w��
		lua_gettable(L, LUA_GLOBALSINDEX); // (2)��(3)�̓���
		double v = lua_tonumber(L, -1); // (4) ���z�X�^�b�N�̃g�b�v���e(toC�̒��g)�𐔒l�^�Ŏ��o��
		if (g_ValList[i].Val != v) g_ValList[i].Updated = true;
		g_ValList[i].Val = (GFloat)v;
		lua_pop(L, 1); // (5) ���o�����牼�z�X�^�b�N��1��pop����
		if (g_ValList[i].Val > g_ValList[i].Max) g_ValList[i].Val = g_ValList[i].Max;
		if (g_ValList[i].Val < g_ValList[i].Min) g_ValList[i].Val = g_ValList[i].Min;
	}
	luaUpdateVal();
	g_RandTime += rand() % 3 + 1;
	if (g_RandTime > 20000000) g_RandTime = 0;
	return 0;
}

