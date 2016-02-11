#include "luaSub.hpp"
#include "GDPlay.hpp"
#include "GPlayers.h"


//メモリリーク検出用
#include <crtdbg.h>  
#ifdef _DEBUG 
#ifdef __MEMLEAKCHECK 
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__) 
#define malloc(p1) _malloc_dbg((p1),_NORMAL_BLOCK,__FILE__,__LINE__) 
#define __MEMLEAKCHECK
#endif
#endif 
//--メモリリーク検出用




float luaL2dx, luaL2dy;
void Line(GVector &p1, GVector &p2, unsigned int col);
void Line2D(GFloat x0, GFloat y0, GFloat x1, GFloat y1, int col);

int luaGetSMouseX(lua_State *L)
{
	static int m = 0;
	if (CtrlKey == 0 && g_World->Stop == 0) lua_pushnumber(L, m);
	else {
		lua_pushnumber(L, MouseX);
		m = MouseX;
	}
	return 1;
}
int luaGetSMouseY(lua_State *L)
{
	static int m = 0;
	if (CtrlKey == 0 && g_World->Stop == 0) lua_pushnumber(L, m);
	else {
		lua_pushnumber(L, MouseY);
		m = MouseY;
	}
	return 1;
}
int luaGetSMouseL(lua_State *L)
{
	if (CtrlKey == 0 && g_World->Stop == 0) lua_pushnumber(L, 0);
	else lua_pushnumber(L, MouseL);
	return 1;
}
int luaGetSMouseR(lua_State *L)
{
	if (CtrlKey == 0 && g_World->Stop == 0) lua_pushnumber(L, 0);
	else lua_pushnumber(L, MouseR);
	return 1;
}
int luaGetSMouseM(lua_State *L)
{
	if (CtrlKey == 0 && g_World->Stop == 0) lua_pushnumber(L, 0);
	else lua_pushnumber(L, MouseM);
	return 1;
}
int luaGetMouseX(lua_State *L)
{
	static int m = 0;
	if (CtrlKey == 1) lua_pushnumber(L, m);
	else {
		lua_pushnumber(L, MouseX);
		m = MouseX;
	}
	return 1;
}
int luaGetMouseY(lua_State *L)
{
	static int m = 0;
	if (CtrlKey == 1) lua_pushnumber(L, m);
	else {
		lua_pushnumber(L, MouseY);
		m = MouseY;
	}
	return 1;
}
int luaGetMouseL(lua_State *L)
{
	if (CtrlKey == 1) lua_pushnumber(L, 0);
	else lua_pushnumber(L, MouseL);
	return 1;
}
int luaGetMouseR(lua_State *L)
{
	if (CtrlKey == 1) lua_pushnumber(L, 0);
	else lua_pushnumber(L, MouseR);
	return 1;
}
int luaGetMouseM(lua_State *L)
{
	if (CtrlKey == 1) lua_pushnumber(L, 0);
	else lua_pushnumber(L, MouseM);
	return 1;
}
int luaGetTickCount(lua_State *L)
{
	lua_pushnumber(L, g_TickCount);
	return 1;
}
int luaSetTicks(lua_State *L)
{
	g_TickCount = (int)lua_tonumber(L, 1);
	lua_pushnumber(L, g_TickCount);
	return 1;
}
int luaSetCCDZoom(lua_State *L)
{
	float v = (float)lua_tonumber(L, 1);
	if (v >= 1 && v <= 100) CCDZoom = v;
	lua_pushnumber(L, CCDZoom);
	return 1;
}

int luaGetCCD(lua_State *L)
{
	lua_pushnumber(L, (double)CCDImage[(int)lua_tonumber(L, 2)][(int)lua_tonumber(L, 1)]);
	return 1;
}
int luaGetCCDRed(lua_State *L)
{
	lua_pushnumber(L, (CCDImage[(int)lua_tonumber(L, 2)][(int)lua_tonumber(L, 1)] >> 10) / 32.0);
	return 1;
}
int luaGetCCDGreen(lua_State *L)
{
	lua_pushnumber(L, ((CCDImage[(int)lua_tonumber(L, 2)][(int)lua_tonumber(L, 1)] & 0x3e0) >> 5) / 32.0);
	return 1;
}
int luaGetCCDBlue(lua_State *L)
{
	lua_pushnumber(L, (CCDImage[(int)lua_tonumber(L, 2)][(int)lua_tonumber(L, 1)] & 0x1f) / 32.0);
	return 1;
}

int luaKey(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= GKEYMAX) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, (double)g_KeyList[(int)no].Pressed);
	return 1;
}
int luaKeyDown(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= GKEYMAX) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, (double)g_KeyList[(int)no].Down);
	return 1;
}
int luaKeyUp(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= GKEYMAX) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, (double)g_KeyList[(int)no].Up);
	return 1;
}
int luaKey2(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= GKEYMAX) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, (double)g_KeyList[(int)no].SPressed);
	return 1;
}
int luaKeyDown2(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= GKEYMAX) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, (double)g_KeyList[(int)no].SDown);
	return 1;
}
int luaKeyUp2(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= GKEYMAX) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, (double)g_KeyList[(int)no].SUp);
	return 1;
}
int luaAnalog(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= 6) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, Analog[(int)no]);
	return 1;
}
int luaHat(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= 1) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, Hat[(int)no]);
	return 1;
}

int luaGetType(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->ChipType);
	return 1;
}
int luaGetDir(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->Dir);
	return 1;
}
int luaGetOption(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->Option);
	return 1;
}
int luaGetEffect(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->Effect);
	return 1;
}
int luaGetUserOption(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->UserOption);
	return 1;
}
int luaGetUserEffect(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->UserEffect);
	return 1;
}
int luaGetPower(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->Power);
	return 1;
}
int luaGetAngle(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	if (g_Chip[(int)no]->LinkInfo) lua_pushnumber(L, g_Chip[(int)no]->LinkInfo->Angle);
	else lua_pushnumber(L, -1);
	return 1;
}
int luaGetSpring(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	if (g_Chip[(int)no]->LinkInfo) lua_pushnumber(L, g_Chip[(int)no]->LinkInfo->SpringK);
	else lua_pushnumber(L, -1);
	return 1;
}
int luaGetDamper(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	if (g_Chip[(int)no]->LinkInfo) lua_pushnumber(L, g_Chip[(int)no]->LinkInfo->DamperK);
	else lua_pushnumber(L, -1);
	return 1;
}
int luaGetBrake(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	if (g_Chip[(int)no]->LinkInfo) lua_pushnumber(L, g_Chip[(int)no]->LinkInfo->FrictionK);
	else lua_pushnumber(L, -1);
	return 1;
}
int luaGetParent(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, -1);return 1; }
	if (g_Chip[(int)no]->Parent) lua_pushnumber(L, g_Chip[(int)no]->Parent->ID);
	else lua_pushnumber(L, -1);
	return 1;
}
int luaGetTop(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, -1);return 1; }
	if (g_Chip[(int)no]->Top) lua_pushnumber(L, g_Chip[(int)no]->Top->ID);
	else lua_pushnumber(L, -1);
	return 1;
}
int luaGetColor(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->Color);
	return 1;
}
int luaGetEnergy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	double e = g_Chip[(int)no]->Energy;
	if (e <= 0) e = 0;
	lua_pushnumber(L, e);
	return 1;
}
int luaGetM(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->M);
	return 1;
}
int luaGetObjM(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= GOBJMAX || g_World->Object[(int)no] == NULL) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_World->Object[(int)no]->M);
	return 1;
}
int luaGetObjI(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	if (no < 0 || no >= GOBJMAX || g_World->Object[(int)no] == NULL) lua_pushnumber(L, 0);
	else {
		if (x < 0 || x >= 3) { lua_pushnumber(L, 0);return 1; }
		if (y < 0 || y >= 3) { lua_pushnumber(L, 0);return 1; }
		lua_pushnumber(L, g_World->Object[(int)no]->I.elem[x][y]);
	}
	return 1;
}
int luaGetI(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	if (x < 0 || x >= 3) { lua_pushnumber(L, 0);return 1; }
	if (y < 0 || y >= 3) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->I.elem[x][y]);
	return 1;
}
int luaPosx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->X.x);
	return 1;
}
int luaPosy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->X.y);
	return 1;
}
int luaPosz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->X.z);
	return 1;
}
int luaGetY(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	double h = g_World->Land->GetY2(g_Chip[(int)no]->X.x, g_Chip[(int)no]->X.y, g_Chip[(int)no]->X.z);
	if (h < 0) lua_pushnumber(L, h);
	else  lua_pushnumber(L, g_Chip[(int)no]->X.y - h);
	return 1;
}
int luaUnLinkBye(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (g_Chip[(int)no]->Parent == NULL) { lua_pushnumber(L, 0);return 1; }
	g_World->DeleteLink(g_Chip[(int)no]);
	g_Chip[(int)no]->ByeFlag = 1;
	if (ViewType == 5 || ViewType == 6) LastBye = (int)no;
	lua_pushnumber(L, 1);
	return 1;
}
int luaUnLink(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (g_Chip[(int)no]->Parent == NULL) { lua_pushnumber(L, 0);return 1; }
	g_World->DeleteLink(g_Chip[(int)no]);
	g_Chip[(int)no]->ByeFlag = 0;
	if (ViewType == 5 || ViewType == 6) LastBye = (int)no;
	lua_pushnumber(L, 1);
	return 1;
}
int luaLx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->L*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.x);
	return 1;
}
int luaLy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->L*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.y);
	return 1;
}
int luaLz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->L*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.z);
	return 1;
}
int luaWx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->W*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.x);
	return 1;
}
int luaWy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->W*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.y);
	return 1;
}
int luaWz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->W*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.z);
	return 1;
}
int luaVx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->V*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.x);
	return 1;
}
int luaVy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->V*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.y);
	return 1;
}
int luaVz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->V*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.z);
	return 1;
}
int luaFx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->P*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.x);
	return 1;
}
int luaFy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->P*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.y);
	return 1;
}
int luaFz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector l = g_Chip[(int)no]->P*g_Chip[(int)no]->Rt;
	lua_pushnumber(L, l.z);
	return 1;
}
int luaAx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector vx = GVector(1, 0, 0)*g_Chip[(int)no]->R;
	GVector vy = GVector(0, 1, 0);
	GVector vz = GVector(0, 0, 1)*g_Chip[(int)no]->R;
	double a = (vy).angle(vz) - M_PI / 2.0;
	lua_pushnumber(L, a);
	return 1;
}
int luaAy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	double a = -(GVector(0, 0, 1)*g_Chip[(int)no]->R).Cut2(GVector(0, 1, 0)).angle2(GVector(0, 0, 1), GVector(0, 1, 0));
	lua_pushnumber(L, a);
	return 1;
}
int luaAz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector vx = GVector(1, 0, 0)*g_Chip[(int)no]->R;
	GVector vy = GVector(0, 1, 0);
	GVector vz = GVector(0, 0, 1)*g_Chip[(int)no]->R;
	double a = (vy).angle(vx) - M_PI / 2.0;
	lua_pushnumber(L, a);
	return 1;
}
int luaEx(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector  y_axis, z_axis;
	z_axis.x = g_Chip[n]->R.elem[2][0];//m[2];
	z_axis.y = g_Chip[n]->R.elem[2][1];//m[5];
	z_axis.z = g_Chip[n]->R.elem[2][2];//m[8];

	double yaw = atan2(z_axis.x, z_axis.z);

	double  cos_yaw = cos(yaw);
	lua_pushnumber(L, atan2((double)(cos_yaw * z_axis.y), (double)fabs(z_axis.z)));
	return 1;
}
int luaEy(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector  y_axis, z_axis;
	z_axis.x = g_Chip[n]->R.elem[2][0];//m[2];
	z_axis.z = g_Chip[n]->R.elem[2][2];//m[8];

	lua_pushnumber(L, atan2(z_axis.x, z_axis.z));
	return 1;
}
int luaEz(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector  y_axis, z_axis;
	z_axis.x = g_Chip[n]->R.elem[2][0];//m[2];
	z_axis.y = g_Chip[n]->R.elem[2][1];//m[5];
	z_axis.z = g_Chip[n]->R.elem[2][2];//m[8];
	y_axis.x = g_Chip[n]->R.elem[1][0];//m[1];
	y_axis.y = g_Chip[n]->R.elem[1][1];//m[4];
	y_axis.z = g_Chip[n]->R.elem[1][2];//m[7];

	double yaw = atan2(z_axis.x, z_axis.z);

	double  cos_yaw = cos(yaw);
	double pitch = atan2((double)(cos_yaw * z_axis.y), (double)fabs(z_axis.z));

	double  sin_yaw = sin(yaw);
	double  cos_pitch = cos(pitch);
	lua_pushnumber(L, atan2((double)(cos_pitch * (sin_yaw * y_axis.z - cos_yaw * y_axis.x)), (double)y_axis.y));
	return 1;
}

int luaRx(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	int rn = (int)lua_tonumber(L, 2);
	if (n < 0 || n >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	if (rn < 0 || rn >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector  y_axis, z_axis;
	GMatrix R = g_Chip[n]->R*g_Chip[rn]->R.transpose();
	z_axis.x = R.elem[2][0];//m[2];
	z_axis.y = R.elem[2][1];//m[5];
	z_axis.z = R.elem[2][2];//m[8];

	double yaw = atan2(z_axis.x, z_axis.z);

	double  cos_yaw = cos(yaw);
	lua_pushnumber(L, atan2((double)(cos_yaw * z_axis.y), (double)fabs(z_axis.z)));
	return 1;
}
int luaRy(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	int rn = (int)lua_tonumber(L, 2);
	if (n < 0 || n >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	if (rn < 0 || rn >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GVector  y_axis, z_axis;
	GMatrix R = g_Chip[n]->R*g_Chip[rn]->R.inverse();
	z_axis.x = R.elem[2][0];//m[2];
	z_axis.z = R.elem[2][2];//m[8];

	lua_pushnumber(L, atan2(z_axis.x, z_axis.z));
	return 1;
}
int luaRz(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	int rn = (int)lua_tonumber(L, 2);
	if (n < 0 || n >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	if (rn < 0 || rn >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	GMatrix R = g_Chip[n]->R*g_Chip[rn]->R.transpose();
	GVector  y_axis, z_axis;
	z_axis.x = R.elem[2][0];//m[2];
	z_axis.y = R.elem[2][1];//m[5];
	z_axis.z = R.elem[2][2];//m[8];
	y_axis.x = R.elem[1][0];//m[1];
	y_axis.y = R.elem[1][1];//m[4];
	y_axis.z = R.elem[1][2];//m[7];

	double yaw = atan2(z_axis.x, z_axis.z);

	double  cos_yaw = cos(yaw);
	double pitch = atan2((double)(cos_yaw * z_axis.y), (double)fabs(z_axis.z));

	double  sin_yaw = sin(yaw);
	double  cos_pitch = cos(pitch);
	lua_pushnumber(L, atan2((double)(cos_pitch * (sin_yaw * y_axis.z - cos_yaw * y_axis.x)), (double)y_axis.y));
	return 1;
}
int luaGx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->Top->TotalCenterOfGravity.x);
	return 1;
}
int luaGy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->Top->TotalCenterOfGravity.y);
	return 1;
}
int luaGz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->Top->TotalCenterOfGravity.z);
	return 1;
}
int luaXx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->R.elem[0][0]);
	return 1;
}
int luaXy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->R.elem[0][1]);
	return 1;
}
int luaXz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->R.elem[0][2]);
	return 1;
}
int luaYx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->R.elem[1][0]);
	return 1;
}
int luaYy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->R.elem[1][1]);
	return 1;
}
int luaYz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->R.elem[1][2]);
	return 1;
}
int luaZx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->R.elem[2][0]);
	return 1;
}
int luaZy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->R.elem[2][1]);
	return 1;
}
int luaZz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->R.elem[2][2]);
	return 1;
}
int luaQx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->Q.x);
	return 1;
}
int luaQy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->Q.y);
	return 1;
}
int luaQz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->Q.z);
	return 1;
}
int luaQw(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->Q.w);
	return 1;
}
int luaObjPosx(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= GOBJMAX || g_World->Object[(int)no] == NULL) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_World->Object[(int)no]->X.x);
	return 1;
}
int luaObjPosy(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= GOBJMAX || g_World->Object[(int)no] == NULL) lua_pushnumber(L, -100000.0);
	else lua_pushnumber(L, g_World->Object[(int)no]->X.y);
	return 1;
}
int luaObjPosz(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= GOBJMAX || g_World->Object[(int)no] == NULL) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_World->Object[(int)no]->X.z);
	return 1;
}
int luaVel(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, 0);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->V.abs());
	return 1;
}
int luaChips(lua_State *L)
{
	lua_pushnumber(L, (double)g_ChipCount);
	return 1;
}
int luaWeight(lua_State *L)
{
	lua_pushnumber(L, (double)g_Chip[0]->TotalMass);
	return 1;
}
int luaRnd(lua_State *L)
{
	lua_pushnumber(L, (double)rand() / (double)RAND_MAX);
	return 1;
}

int luaGetDt(lua_State *L)
{
	lua_pushnumber(L, (double)g_World->Dt);
	return 1;
}
int luaGetWidth(lua_State *L)
{
	lua_pushnumber(L, (double)Width);
	return 1;
}
int luaGetHeight(lua_State *L)
{
	lua_pushnumber(L, (double)Height);
	return 1;
}
int luaGetFps(lua_State *L)
{
	lua_pushnumber(L, (double)g_FPS);
	return 1;
}
int luaGetBase(lua_State *L)
{
	lua_pushnumber(L, (double)g_LimitFPS);
	return 1;
}
int luaGetFaces(lua_State *L)
{
	lua_pushnumber(L, (double)NumFace);
	return 1;
}
int luaGetTolerant(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) lua_pushnumber(L, 0);
	else lua_pushnumber(L, g_Chip[(int)no]->Tolerant);
	return 1;
}
int luaMove3D(lua_State *L)
{
	luaL3dx = (float)lua_tonumber(L, 1);
	luaL3dy = (float)lua_tonumber(L, 2);
	luaL3dz = (float)lua_tonumber(L, 3);
	lua_pushnumber(L, 0);
	return 1;
}
int luaLine3D(lua_State *L)
{
	float x = (float)lua_tonumber(L, 1);
	float y = (float)lua_tonumber(L, 2);
	float z = (float)lua_tonumber(L, 3);
	if (ViewUpdate) {
		g_pApp->ViewSet();
		ViewUpdate = 0;
	}
	Line(GVector(luaL3dx, luaL3dy, luaL3dz), GVector(x, y, z), luaGraColor);
	luaL3dx = (float)x;
	luaL3dy = (float)y;
	luaL3dz = (float)z;
	lua_pushnumber(L, 0);
	return 1;
}
int luaMove2D(lua_State *L)
{
	luaL2dx = (float)lua_tonumber(L, 1);
	luaL2dy = (float)lua_tonumber(L, 2);
	lua_pushnumber(L, 0);
	return 1;
}
int luaLine2D(lua_State *L)
{
	float x = (float)lua_tonumber(L, 1);
	float y = (float)lua_tonumber(L, 2);
	if (ViewUpdate) {
		g_pApp->ViewSet();
		ViewUpdate = 0;
	}
	Line2D(luaL2dx, luaL2dy, x, y, luaGraColor);
	luaL2dx = (float)x;
	luaL2dy = (float)y;
	lua_pushnumber(L, 0);

	return 1;
}
int luaSetColor(lua_State *L)
{
	luaGraColor = (int)lua_tonumber(L, 1);
	lua_pushnumber(L, (double)luaGraColor);
	return 1;
}
int luaRound(lua_State *L)
{
	double a = lua_tonumber(L, 1);
	if (a >= 0.0)lua_pushnumber(L, (int)(fabs(a) + 0.5));
	else lua_pushnumber(L, -(int)(fabs(a) + 0.5));
	return 1;
}
int luaLen3(lua_State *L)
{
	double a = lua_tonumber(L, 1);
	double b = lua_tonumber(L, 2);
	double c = lua_tonumber(L, 3);
	lua_pushnumber(L, sqrt(a*a + b*b + c*c));
	return 1;
}
int luaLen2(lua_State *L)
{
	double a = lua_tonumber(L, 1);
	double b = lua_tonumber(L, 2);
	lua_pushnumber(L, sqrt(a*a + b*b));
	return 1;
}
int luaPI(lua_State *L)
{
	lua_pushnumber(L, M_PI);
	return 1;
}
int luaToDeg(lua_State *L)
{
	double a = lua_tonumber(L, 1);
	lua_pushnumber(L, a*180.0 / M_PI);
	return 1;
}
int luaToRad(lua_State *L)
{
	double a = lua_tonumber(L, 1);
	lua_pushnumber(L, a*M_PI / 180.0);
	return 1;
}

int luaGetPlayers(lua_State *L)
{
	lua_pushnumber(L, (double)g_DPlay->GetNumPlayers());
	return 1;
}
int luaGetPlayerHostID(lua_State *L)
{
	lua_pushnumber(L, (double)g_DPlay->GetHostPlayerDPNID());
	return 1;
}
int luaGetPlayerMyID(lua_State *L)
{
	lua_pushnumber(L, (double)g_DPlay->GetLocalPlayerDPNID());
	return 1;
}
int luaGetPlayerID(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	lua_pushnumber(L, (double)g_PlayerData[n].ReceiveData.info.dpnidPlayer);
	return 1;
}
int luaGetPlayerCrushes(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (g_DPlay->GetLocalPlayerDPNID() == g_PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)g_MyPlayerData.crush);
		return 1;
	}
	lua_pushnumber(L, (double)g_PlayerData[n].crush);
	return 1;
}
int luaGetPlayerResets(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (g_DPlay->GetLocalPlayerDPNID() == g_PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)g_MyPlayerData.reset);
		return 1;
	}
	lua_pushnumber(L, (double)g_PlayerData[n].reset);
	return 1;
}
int luaGetPlayerInits(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (g_DPlay->GetLocalPlayerDPNID() == g_PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)g_MyPlayerData.init);
		return 1;
	}
	lua_pushnumber(L, (double)g_PlayerData[n].init);
	return 1;
}
int luaGetPlayerColor(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	lua_pushnumber(L, (double)g_PlayerData[n].ReceiveData.info.color);
	return 1;
}
int luaGetPlayerChips(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (g_DPlay->GetLocalPlayerDPNID() == g_PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)g_ChipCount);
		return 1;
	}
	lua_pushnumber(L, (double)g_PlayerData[n].g_ChipCount);
	return 1;
}
int luaGetPlayerX(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	double a = pow((double)g_PlayerData[n].g_ChipCount, 1.0 / 3.0) / 2.0;
	if (g_DPlay->GetLocalPlayerDPNID() == g_PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)g_Chip[0]->X.x);
		return 1;
	}
	double v = g_PlayerData[n].x + (rand() % 10000 - 5000) / 1000.0*a + sin(g_RandTime / 150.0)*a - sin(g_RandTime / 350.0)*a;
	lua_pushnumber(L, v);
	return 1;
}
int luaGetPlayerY(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushnumber(L, -100000.0);
		return 1;
	}
	double a = pow((double)g_PlayerData[n].g_ChipCount, 1.0 / 3.0) / 2.0;
	if (g_DPlay->GetLocalPlayerDPNID() == g_PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)g_Chip[0]->X.y);
		return 1;
	}
	double v = g_PlayerData[n].y + (rand() % 10000 - 5000) / 1000.0*a + sin(g_RandTime / 160.0)*a - sin(g_RandTime / 360.0)*a;
	lua_pushnumber(L, v);
	return 1;
}
int luaGetPlayerZ(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	double a = pow((double)g_PlayerData[n].g_ChipCount, 1.0 / 3.0) / 2.0;
	if (g_DPlay->GetLocalPlayerDPNID() == g_PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)g_Chip[0]->X.z);
		return 1;
	}
	double v = g_PlayerData[n].z + (rand() % 10000 - 5000) / 1000.0*a + sin(g_RandTime / 140.0)*a - sin(g_RandTime / 340.0)*a;
	lua_pushnumber(L, v);
	return 1;
}
int luaGetPlayerArms(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (g_DPlay->GetLocalPlayerDPNID() == g_PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)g_MyPlayerData.haveArm);
		return 1;
	}
	lua_pushnumber(L, (double)g_PlayerData[n].haveArm);
	return 1;
}
int luaGetPlayerYForces(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (g_DPlay->GetLocalPlayerDPNID() == g_PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)g_MyPlayerData.yforce);
		return 1;
	}
	lua_pushnumber(L, (double)g_PlayerData[n].yforce);
	return 1;
}
int luaGetPlayerName(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n < 0 || n >= g_DPlay->GetNumPlayers()) {
		lua_pushstring(L, "");
		return 1;
	}
	lua_pushstring(L, g_PlayerData[n].ReceiveData.info.strPlayerName);
	return 1;
}
int luaGetFuel(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, -1);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->Top->TotalFuel);
	return 1;
}
int luaGetFuelMax(lua_State *L)
{
	int no = (int)lua_tonumber(L, 1);
	if (no < 0 || no >= g_ChipCount) { lua_pushnumber(L, -1);return 1; }
	lua_pushnumber(L, g_Chip[(int)no]->Top->TotalFuelMax);
	return 1;
}
void luaUpdateVal() {
	int i, k;
	for (i = 0;i < GVALMAX;i++) {
		for (k = 0;k < g_ValList[i].RefCount;k++) {
			if (g_ValList[i].Flag[k])
				*(g_ValList[i].Ref[k]) = -g_ValList[i].Val;
			else *(g_ValList[i].Ref[k]) = g_ValList[i].Val;
		}
	}
}
