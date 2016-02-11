#pragma warning(disable : 4996)
#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <D3DX8.h>
#include <DXErr8.h>
#include <tchar.h>
#include <dinput.h>
#include "D3DApp.h"
#include "D3DFont.h"
#include "D3DFile.h"
#include "D3DUtil.h"
#include "DIUtil.h"
#include "DMUtil.h"
#include "DSUtil.h"
#include "DXUtil.h"
#include "D3Dtypes.h"

#include <stdio.h>
//#include <stdlib.h>
#include <math.h>
#include "string.h"
#include "ctype.h"

#include "Globals.hpp"
#include "GVector.hpp"
#include "GRigid.hpp"
#include "readData.hpp"
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

#define IDENTSYM 1
#define NUMSYM 2
#define STRSYM 3
#define MAXNUM 32766
#define MAXVARIABLE 256
#define MAXNAMESIZE 256
#define MAXSOURCESIZE 32766
#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif


static float l3dx, l3dy, l3dz;
static float l2dx, l2dy;
static int graColor;
void Line(GVector &p1, GVector &p2, unsigned int col);
void Line2D(GFloat x0, GFloat y0, GFloat x1, GFloat y1, int col);

//変数の内容を更新

static long myrandval = 1L;
int myrand() {
	return(((myrandval = myrandval * 214013L + 2531011L) >> 16) & 0x7fffL);
}

void updateVal() {
	int i, k;
	for (i = 0;i < GVALMAX;i++) {
		for (k = 0;k < ValList[i].RefCount;k++) {
			if (ValList[i].Flag[k])
				*(ValList[i].Ref[k]) = -ValList[i].Val;
			else *(ValList[i].Ref[k]) = ValList[i].Val;
		}
	}
}
void output(int no, char *str)
{
	if (no < 0) return;
	if (no >= GOUTPUTMAX) return;
	lstrcpy(ScriptOutput[no], str);

}
double  mouseX(void)
{
	return (double)MouseX;
}
double  mouseY(void)
{
	return (double)MouseY;
}
double  mouseL(void)
{
	return (double)MouseL;
}
double  mouseR(void)
{
	return (double)MouseR;
}
double  mouseM(void)
{
	return (double)MouseM;
}
double  ticks(void)
{
	return (double)g_TickCount;
}
double  systemTicks(void)
{
	return (double)g_SystemTickCount;
}
double  setTicks(double v)
{
	g_TickCount = (int)v;
	return v;
}
double  setCCDZoom(double v)
{
	if (v >= 1 && v <= 100) CCDZoom = (float)v;
	return CCDZoom;
}

double  getCCD(double x, double y)
{
	return (double)CCDImage[(int)x][(int)y];
}
double  getCCDRed(double x, double y)
{
	return (CCDImage[(int)x][(int)y] >> 10) / 32.0;
}
double  getCCDGreen(double x, double y)
{
	return ((CCDImage[(int)x][(int)y] & 0x3e0) >> 5) / 32.0;
}
double  getCCDBlue(double x, double y)
{
	return (CCDImage[(int)x][(int)y] & 0x1f) / 32.0;
}

double  key(double no)
{
	if (no < 0 || no >= GKEYMAX) return 0.0;
	return (double)KeyList[(int)no].Pressed;
}
double  keyDown(double no)
{
	if (no < 0 || no >= GKEYMAX) return 0.0;
	return (double)KeyList[(int)no].Down;
}
double  keyUp(double no)
{
	if (no < 0 || no >= GKEYMAX) return 0.0;
	return (double)KeyList[(int)no].Up;
}
double  analog(double no)
{
	if (no < 0 || no >= 6) return 0.0;
	return Analog[(int)no];
}
double  hat(double no)
{
	if (no < 0 || no >= 1) return 0.0;
	return Hat[(int)no];
}

double  ObjPosx(double no)
{
	if (no < 0 || no >= World->ObjectCount) return 0.0;
	return World->Object[(int)no]->X.x;
}
double  ObjPosy(double no)
{
	if (no < 0 || no >= World->ObjectCount) return -100000.0;
	return World->Object[(int)no]->X.y;
}
double  ObjPosz(double no)
{
	if (no < 0 || no >= World->ObjectCount) return 0.0;
	return World->Object[(int)no]->X.z;
}
double  getType(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return Chip[(int)no]->ChipType;
}
double  getOption(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return Chip[(int)no]->Option;
}
double  getEffect(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return Chip[(int)no]->Effect;
}
double  getDir(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return Chip[(int)no]->Dir;
}
double  getPower(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return Chip[(int)no]->Power;
}
double  getAngle(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	if (Chip[(int)no]->LinkInfo) return Chip[(int)no]->LinkInfo->Angle;
	return -1;
}
double  getSpring(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	if (Chip[(int)no]->LinkInfo) return Chip[(int)no]->LinkInfo->SpringK;
	return -1;
}
double  getDamper(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	if (Chip[(int)no]->LinkInfo) return Chip[(int)no]->LinkInfo->DamperK;
	return -1;
}
double  getBrake(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	if (Chip[(int)no]->LinkInfo) return Chip[(int)no]->LinkInfo->FrictionK;
	return -1;
}
double  getParent(double no)
{
	if (no < 0 || no >= ChipCount) return -1;
	if (Chip[(int)no]->Parent) return Chip[(int)no]->Parent->ID;
	return -1;
}
double  getTop(double no)
{
	if (no < 0 || no >= ChipCount) return -1;
	if (Chip[(int)no]->Top) return Chip[(int)no]->Top->ID;
	return -1;
}
double  getFuel(double no)
{
	if (no < 0 || no >= ChipCount) return -1;
	if (Chip[(int)no]->Top) return Chip[(int)no]->Top->TotalFuel;
	return -1;
}
double  getFuelMax(double no)
{
	if (no < 0 || no >= ChipCount) return -1;
	if (Chip[(int)no]->Top) return Chip[(int)no]->Top->TotalFuelMax;
	return -1;
}
double  getColor(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return Chip[(int)no]->Color;
}
double  getEnergy(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	if (Chip[(int)no]->Energy <= 0) return 0.0;
	return Chip[(int)no]->Energy;
}
double  getM(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return (Chip[(int)no]->M);
}
double  getI(double no, double x, double y)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	if (x < 0 || x >= 3) return 0.0;
	if (y < 0 || y >= 3) return 0.0;
	return (Chip[(int)no]->I.elem[(int)x][(int)y]);
}
double  getObjM(double no)
{
	if (no < 0 || no >= World->ObjectCount) return 0.0;
	if (World->Object[(int)no] == NULL) return 0.0;
	return (World->Object[(int)no]->M);
}
double  getObjI(double no, double x, double y)
{
	if (no < 0 || no >= World->ObjectCount) return 0.0;
	if (World->Object[(int)no] == NULL) return 0.0;
	if (x < 0 || x >= 3) return 0.0;
	if (y < 0 || y >= 3) return 0.0;
	return (World->Object[(int)no]->I.elem[(int)x][(int)y]);
}
double  posx(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return Chip[(int)no]->X.x;
}
double  posy(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return Chip[(int)no]->X.y;
}
double  posz(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return Chip[(int)no]->X.z;
}
double  gx(double no)
{
	return Chip[(int)no]->Top->TotalCenterOfGravity.x;
}
double  gy(double no)
{
	return Chip[(int)no]->Top->TotalCenterOfGravity.y;
}
double  gz(double no)
{
	return Chip[(int)no]->Top->TotalCenterOfGravity.z;
}
double  xx(double no)
{
	return Chip[(int)no]->R.elem[0][0];
}
double  xy(double no)
{
	return Chip[(int)no]->R.elem[0][1];
}
double  xz(double no)
{
	return Chip[(int)no]->R.elem[0][2];
}
double  yx(double no)
{
	return Chip[(int)no]->R.elem[1][0];
}
double  yy(double no)
{
	return Chip[(int)no]->R.elem[1][1];
}
double  yz(double no)
{
	return Chip[(int)no]->R.elem[1][2];
}
double  zx(double no)
{
	return Chip[(int)no]->R.elem[2][0];
}
double  zy(double no)
{
	return Chip[(int)no]->R.elem[2][1];
}
double  zz(double no)
{
	return Chip[(int)no]->R.elem[2][2];
}
double  qx(double no)
{
	return Chip[(int)no]->Q.x;
}
double  qy(double no)
{
	return Chip[(int)no]->Q.y;
}
double  qz(double no)
{
	return Chip[(int)no]->Q.z;
}
double  qw(double no)
{
	return Chip[(int)no]->Q.w;
}
double  getY(double no)
{
	double h = World->Land->GetY2(Chip[(int)no]->X.x, Chip[(int)no]->X.y, Chip[(int)no]->X.z);
	if (h < 0) return h;

	return Chip[(int)no]->X.y - h;
}
double  unLinkBye(double no)
{
	if (Chip[(int)no]->Parent == NULL) return 0;
	World->DeleteLink(Chip[(int)no]);
	Chip[(int)no]->ByeFlag = 1;
	if (ViewType == 5 || ViewType == 6) LastBye = (int)no;
	return 1;
}
double  unLink(double no)
{
	if (Chip[(int)no]->Parent == NULL) return 0;
	World->DeleteLink(Chip[(int)no]);
	Chip[(int)no]->ByeFlag = 0;
	if (ViewType == 5 || ViewType == 6) LastBye = (int)no;

	return 1;
}
double  lx(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->L*Chip[(int)no]->Rt;
	return l.x;
}
double  ly(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->L*Chip[(int)no]->Rt;
	return l.y;
}
double  lz(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->L*Chip[(int)no]->Rt;
	return l.z;
}
double  wx(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->W*Chip[(int)no]->Rt;
	return l.x;
}
double  wy(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->W*Chip[(int)no]->Rt;
	return l.y;
}
double  wz(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->W*Chip[(int)no]->Rt;
	return l.z;
}
double  vx(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->V*Chip[(int)no]->Rt;
	return l.x;
}
double  vy(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->V*Chip[(int)no]->Rt;
	return l.y;
}
double  vz(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->V*Chip[(int)no]->Rt;
	return l.z;
}
double  fx(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->P*Chip[(int)no]->Rt;
	return l.x;
}
double  fy(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->P*Chip[(int)no]->Rt;
	return l.y;
}
double  fz(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector l = Chip[(int)no]->P*Chip[(int)no]->Rt;
	return l.z;
}
double  ax(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector vx = GVector(1, 0, 0)*Chip[(int)no]->R;
	GVector vy = GVector(0, 1, 0);
	GVector vz = GVector(0, 0, 1)*Chip[(int)no]->R;
	double a = (vy).angle(vz) - M_PI / 2.0;
	return a;
}
double  ay(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	double a = -(GVector(0, 0, 1)*Chip[(int)no]->R).Cut2(GVector(0, 1, 0)).angle2(GVector(0, 0, 1), GVector(0, 1, 0));
	return a;
}

double  az(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	GVector vx = GVector(1, 0, 0)*Chip[(int)no]->R;
	GVector vy = GVector(0, 1, 0);
	GVector vz = GVector(0, 0, 1)*Chip[(int)no]->R;
	double a = (vy).angle(vx) - M_PI / 2.0;
	return a;
}
double  ex(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	int n = (int)no;
	GVector  y_axis, z_axis;
	z_axis.x = Chip[n]->R.elem[2][0];//m[2];
	z_axis.y = Chip[n]->R.elem[2][1];//m[5];
	z_axis.z = Chip[n]->R.elem[2][2];//m[8];

	double yaw = atan2(z_axis.x, z_axis.z);

	double  cos_yaw = cos(yaw);
	return atan2((double)(cos_yaw * z_axis.y), (double)fabs(z_axis.z));
}
double  ey(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	int n = (int)no;
	GVector  y_axis, z_axis;
	z_axis.x = Chip[n]->R.elem[2][0];//m[2];
	z_axis.z = Chip[n]->R.elem[2][2];//m[8];

	return  atan2(z_axis.x, z_axis.z);
}
double  ez(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	int n = (int)no;
	GVector  y_axis, z_axis;
	z_axis.x = Chip[n]->R.elem[2][0];//m[2];
	z_axis.y = Chip[n]->R.elem[2][1];//m[5];
	z_axis.z = Chip[n]->R.elem[2][2];//m[8];
	y_axis.x = Chip[n]->R.elem[1][0];//m[1];
	y_axis.y = Chip[n]->R.elem[1][1];//m[4];
	y_axis.z = Chip[n]->R.elem[1][2];//m[7];

	double yaw = atan2(z_axis.x, z_axis.z);

	double  cos_yaw = cos(yaw);
	double pitch = atan2((double)(cos_yaw * z_axis.y), (double)fabs(z_axis.z));

	double  sin_yaw = sin(yaw);
	double  cos_pitch = cos(pitch);
	return  atan2((double)(cos_pitch * (sin_yaw * y_axis.z - cos_yaw * y_axis.x)), (double)y_axis.y);
}

double  rx(double rno, double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	if (rno < 0 || rno >= ChipCount) return 0.0;
	int rn = (int)rno;
	int n = (int)no;
	GVector  y_axis, z_axis;
	GMatrix R = Chip[n]->R*Chip[rn]->R.transpose();
	z_axis.x = R.elem[2][0];//m[2];
	z_axis.y = R.elem[2][1];//m[5];
	z_axis.z = R.elem[2][2];//m[8];

	double yaw = atan2(z_axis.x, z_axis.z);

	double  cos_yaw = cos(yaw);
	return atan2((double)(cos_yaw * z_axis.y), (double)fabs(z_axis.z));
}
double  ry(double rno, double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	if (rno < 0 || rno >= ChipCount) return 0.0;
	int rn = (int)rno;
	int n = (int)no;
	GVector  y_axis, z_axis;
	GMatrix R = Chip[n]->R*Chip[rn]->R.transpose();
	z_axis.x = R.elem[2][0];//m[2];
	z_axis.z = R.elem[2][2];//m[8];

	return  atan2(z_axis.x, z_axis.z);
}
double  rz(double rno, double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	if (rno < 0 || rno >= ChipCount) return 0.0;
	int rn = (int)rno;
	int n = (int)no;
	GMatrix R = Chip[n]->R*Chip[rn]->R.transpose();
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
	return  atan2((double)(cos_pitch * (sin_yaw * y_axis.z - cos_yaw * y_axis.x)), (double)y_axis.y);
}


double  vel(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return Chip[(int)no]->V.abs();
}
double  chips(void)
{
	return ChipCount;
}
double  weight(void)
{
	return Chip[0]->TotalMass;
}
double move3D(double x, double y, double z)
{
	l3dx = (float)x;
	l3dy = (float)y;
	l3dz = (float)z;
	return 0.0;
}
double line3D(double x, double y, double z)
{
	Line(GVector(l3dx, l3dy, l3dz), GVector((float)x, (float)y, (float)z), graColor);
	l3dx = (float)x;
	l3dy = (float)y;
	l3dz = (float)z;
	return 0.0;
}
double move2D(double x, double y)
{
	l2dx = (float)x;
	l2dy = (float)y;
	return 0.0;
}
double line2D(double x, double y)
{
	Line2D(l2dx, l2dy, (GFloat)x, (GFloat)y, graColor);
	l2dx = (float)x;
	l2dy = (float)y;
	return 0.0;
}
double setColor(double color)
{
	graColor = (int)color;
	return (double)graColor;
}
double rnd(double a)
{
	return myrand() / (double)RAND_MAX;
}

double getDt()
{
	return (double)World->Dt;
}
double getWidth()
{
	return (double)Width;
}
double getHeight()
{
	return (double)Height;
}
double getFps()
{
	return (double)FPS;
}
double getBase()
{
	return (double)g_LimitFPS;
}
double getFaces()
{
	return (double)NumFace;
}
double sign(double a)
{
	if (a == 0.0) return 0.0;
	return a > 0.0 ? 1.0 : -1.0;
}
double integer(double a)
{
	return (double)((int)a);
}
double fix(double a)
{
	return sign(a) * integer(fabs(a));
}
double round(double a)
{
	return sign(a) * (int)(fabs(a) + 0.5);
}
double len3(double a, double b, double c)
{
	return sqrt(a*a + b*b + c*c);
}
double len2(double a, double b)
{
	return sqrt(a*a + b*b);
}
double pi()
{
	return M_PI;
}
double todeg(double a)
{
	return a*180.0 / M_PI;
}
double torad(double a)
{
	return a*M_PI / 180.0;
}
double getTolerant(double no)
{
	if (no < 0 || no >= ChipCount) return 0.0;
	return Chip[(int)no]->Tolerant;
}

double getPlayers()
{
	return (double)DPlay->GetNumPlayers();
}
double getPlayerHostID()
{
	return (double)DPlay->GetHostPlayerDPNID();
}
double getPlayerMyID()
{
	return (double)DPlay->GetLocalPlayerDPNID();
}
double getPlayerID(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	return (double)PlayerData[n].ReceiveData.info.dpnidPlayer;
}
double getPlayerCrushes(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		return (double)MyPlayerData.crush;
	}
	return (double)PlayerData[n].crush;
}
double getPlayerResets(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		return (double)MyPlayerData.reset;
	}
	return (double)PlayerData[n].reset;
}
double getPlayerInits(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		return (double)MyPlayerData.init;
	}
	return (double)PlayerData[n].init;
}
double getPlayerColor(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	return (double)PlayerData[n].ReceiveData.info.color;
}
double getPlayerChips(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		return (double)ChipCount;
	}
	return (double)PlayerData[n].ChipCount;
}
double getPlayerX(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	double a = pow((double)PlayerData[n].ChipCount, 1.0 / 3.0) / 2;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		return (double)Chip[0]->X.x;
	}
	double v = PlayerData[n].x + (myrand() % 10000 - 5000) / 1000.0*a + sin(randTime / 150.0)*a - sin(randTime / 350.0)*a;
	return v;
}
double getPlayerY(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return -100000.0;
	double a = pow((double)PlayerData[n].ChipCount, 1.0 / 3.0) / 2.0;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		return (double)Chip[0]->X.y;
	}
	double v = PlayerData[n].y + (myrand() % 10000 - 5000) / 1000.0*a + sin(randTime / 160.0)*a - sin(randTime / 360.0)*a;
	return v;
}
double getPlayerZ(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	double a = pow((double)PlayerData[n].ChipCount, 1.0 / 3.0) / 2.0;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		return (double)Chip[0]->X.z;
	}
	double v = PlayerData[n].z + (myrand() % 10000 - 5000) / 1000.0*a + sin(randTime / 140.0)*a - sin(randTime / 340.0)*a;
	return v;
}
double getPlayerArms(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		return (double)MyPlayerData.haveArm;
	}
	return (double)PlayerData[n].haveArm;
}
double getPlayerYForces(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		return (double)MyPlayerData.yforce;
	}
	return (double)PlayerData[n].yforce;
}
double getPlayerName(double no)
{
	int n = (int)no;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	char ss[5];
	strncpy(ss, PlayerData[n].ReceiveData.info.strPlayerName, 4);
	unsigned char *s = (unsigned char*)ss;
	bool f = false;
	for (int j = 0;j < 4;j++) {
		if (f) s[j] = '\0';
		if (s[j] == '\0') f = true;
	}
	double d = ((unsigned long)s[3] << 24) + ((unsigned long)s[2] << 16) + ((unsigned long)s[1] << 8) + s[0];
	return d;
}
double getPlayerName2(double no, double sn)
{
	int n = (int)no;
	int i = (int)sn;
	if (n < 0 || n >= DPlay->GetNumPlayers()) return 0.0;
	if (i<0 || i>(int)(strlen(PlayerData[n].ReceiveData.info.strPlayerName) / 4)) return 0.0;
	char ss[5];
	strncpy(ss, &(PlayerData[n].ReceiveData.info.strPlayerName[i * 4]), 4);
	unsigned char *s = (unsigned char*)ss;
	bool f = false;
	for (int j = 0;j < 4;j++) {
		if (f) s[j] = '\0';
		if (s[j] == '\0') f = true;
	}
	double d = ((unsigned long)s[3] << 24) + ((unsigned long)s[2] << 16) + ((unsigned long)s[1] << 8) + s[0];
	return d;
}

struct varrec {
	char name[MAXNAMESIZE];
	double value;
	int type;
} variable[MAXVARIABLE];

struct funrec {
	char name[MAXNAMESIZE];
	double(*func0)(void);
	double(*func1)(double a);
	double(*func2)(double a, double b);
	double(*func3)(double a, double b, double c);
	int argc;
} function[] = {
	{"_MX",mouseX,NULL,NULL,NULL,0},
	{"_MY",mouseY,NULL,NULL,NULL,0},
	{"_ML",mouseL,NULL,NULL,NULL,0},
	{"_MR",mouseR,NULL,NULL,NULL,0},
	{"_MM",mouseM,NULL,NULL,NULL,0},
	{"_PI",pi,NULL,NULL,NULL,0},
	{"_DT",getDt,NULL,NULL,NULL,0},
	{"_FPS",getFps,NULL,NULL,NULL,0},
	{"_BASE",getBase,NULL,NULL,NULL,0},
	{"_TICKS",ticks,NULL,NULL,NULL,0},
	{"_STICKS",systemTicks,NULL,NULL,NULL,0},
	{"_SETTICKS",NULL,setTicks,NULL,NULL,1},
	{"_KEY",NULL,key,NULL,NULL,1},
	{"_KEYDOWN",NULL,keyDown,NULL,NULL,1},
	{"_KEYUP",NULL,keyUp,NULL,NULL,1},
	{"_ANALOG",NULL,analog,NULL,NULL,1},
	{"_HAT",NULL,hat,NULL,NULL,1},
	{"_VEL",NULL,vel,NULL,NULL,1},
	{"_CHIPS",chips,NULL,NULL,NULL,0},
	{"_WEIGHT",weight,NULL,NULL,NULL,0},
	{"_WIDTH",getWidth,NULL,NULL,NULL,0},
	{"_HEIGHT",getHeight,NULL,NULL,NULL,0},
	{"_FACE",getFaces,NULL,NULL,NULL,0},	//地形データのポリゴン数
	{"_ZOOM",NULL,setCCDZoom,NULL,NULL,1},	//位置
	{"_X",NULL,posx,NULL,NULL,1},	//位置
	{"_Y",NULL,posy,NULL,NULL,1},
	{"_Z",NULL,posz,NULL,NULL,1},
	{"_OX",NULL,ObjPosx,NULL,NULL,1},	//オブジェクトの位置
	{"_OY",NULL,ObjPosy,NULL,NULL,1},
	{"_OZ",NULL,ObjPosz,NULL,NULL,1},
	{"_AX",NULL,ax,NULL,NULL,1},	//角度（バグ有り）
	{"_AY",NULL,ay,NULL,NULL,1},
	{"_AZ",NULL,az,NULL,NULL,1},
	{"_EX",NULL,ex,NULL,NULL,1},	//絶対オイラー角
	{"_EY",NULL,ey,NULL,NULL,1},
	{"_EZ",NULL,ez,NULL,NULL,1},
	{"_RX",NULL,NULL,rx,NULL,2},	//相対オイラー角
	{"_RY",NULL,NULL,ry,NULL,2},
	{"_RZ",NULL,NULL,rz,NULL,2},
	{"_LX",NULL,lx,NULL,NULL,1},	//角速度
	{"_LY",NULL,ly,NULL,NULL,1},
	{"_LZ",NULL,lz,NULL,NULL,1},
	{"_VX",NULL,vx,NULL,NULL,1},	//並進速度
	{"_VY",NULL,vy,NULL,NULL,1},
	{"_VZ",NULL,vz,NULL,NULL,1},
	{"_FX",NULL,fx,NULL,NULL,1},	//並進運動量
	{"_FY",NULL,fy,NULL,NULL,1},
	{"_FZ",NULL,fz,NULL,NULL,1},
	{"_WX",NULL,wx,NULL,NULL,1},	//角運動量
	{"_WY",NULL,wy,NULL,NULL,1},
	{"_WZ",NULL,wz,NULL,NULL,1},
	{"_ABS",NULL,fabs,NULL,NULL,1},
	{"_MOD",NULL,NULL,fmod,NULL,2},
	{"_CCD",NULL,NULL,getCCD,NULL,2},	//CCDカメラの色情報(x,y)
	{"_RED",NULL,NULL,getCCDRed,NULL,2},
	{"_GREEN",NULL,NULL,getCCDGreen,NULL,2},
	{"_BLUE",NULL,NULL,getCCDBlue,NULL,2},
	{"_SQRT",NULL,sqrt,NULL,NULL,1},
	{"_SIN",NULL,sin,NULL,NULL,1},
	{"_COS",NULL,cos,NULL,NULL,1},
	{"_TAN",NULL,tan,NULL,NULL,1},
	{"_H",NULL,getY,NULL,NULL,1},
	{"_BYE",NULL,unLinkBye,NULL,NULL,1},
	{"_SPLIT",NULL,unLink,NULL,NULL,1},
	{"_SGN",NULL,sign,NULL,NULL,1},
	{"_RND",NULL,rnd,NULL,NULL,1},
	{"_TODEG",NULL,todeg,NULL,NULL,1},
	{"_TORAD",NULL,torad,NULL,NULL,1},
	{"_ASIN",NULL,asin,NULL,NULL,1},
	{"_ACOS",NULL,acos,NULL,NULL,1},
	{"_ATAN",NULL,atan,NULL,NULL,1},
	{"_ATAN2",NULL,NULL,atan2,NULL,2},
	{"_POW",NULL,NULL,pow,NULL,2},
	{"_EXP",NULL,exp,NULL,NULL,1},
	{"_INT",NULL,integer,NULL,NULL,1},
	{"_ROUND",NULL,round,NULL,NULL,1},
	{"_FIX",NULL,fix,NULL,NULL,1},
	{"_FLOOR",NULL,floor,NULL,NULL,1},
	{"_CEIL",NULL,ceil,NULL,NULL,1},
	{"_LOG",NULL,log,NULL,NULL,1},
	{"_LOG10",NULL,log10,NULL,NULL,1},
	{"_LEN3",NULL,NULL,NULL,len3,3},
	{"_LEN2",NULL,NULL,len2,NULL,2},
	{"_GX",NULL,gx,NULL,NULL,1},
	{"_GY",NULL,gy,NULL,NULL,1},
	{"_GZ",NULL,gz,NULL,NULL,1},
	{"_XX",NULL,xx,NULL,NULL,1},
	{"_XY",NULL,xy,NULL,NULL,1},
	{"_XZ",NULL,xz,NULL,NULL,1},
	{"_YX",NULL,yx,NULL,NULL,1},
	{"_YY",NULL,yy,NULL,NULL,1},
	{"_YZ",NULL,yz,NULL,NULL,1},
	{"_ZX",NULL,zx,NULL,NULL,1},
	{"_ZY",NULL,zy,NULL,NULL,1},
	{"_ZZ",NULL,zz,NULL,NULL,1},
	{"_QX",NULL,qx,NULL,NULL,1},
	{"_QY",NULL,qy,NULL,NULL,1},
	{"_QZ",NULL,qz,NULL,NULL,1},
	{"_QW",NULL,qw,NULL,NULL,1},
	{"_TYPE",NULL,getType,NULL,NULL,1},
	{"_OPTION",NULL,getOption,NULL,NULL,1},
	{"_EFFECT",NULL,getEffect,NULL,NULL,1},
	{"_DIR",NULL,getDir,NULL,NULL,1},
	{"_POWER",NULL,getPower,NULL,NULL,1},
	{"_COLOR",NULL,getColor,NULL,NULL,1},
	{"_ANGLE",NULL,getAngle,NULL,NULL,1},
	{"_DAMPER",NULL,getDamper,NULL,NULL,1},
	{"_SPRING",NULL,getSpring,NULL,NULL,1},
	{"_BRAKE",NULL,getBrake,NULL,NULL,1},
	{"_TOP",NULL,getTop,NULL,NULL,1},
	{"_PARENT",NULL,getParent,NULL,NULL,1},
	{"_M",NULL,getM,NULL,NULL,1},
	{"_I",NULL,NULL,NULL,getI,3},
	{"_MOBJ",NULL,getObjM,NULL,NULL,1},
	{"_IOBJ",NULL,NULL,NULL,getObjI,3},
	{"_E",NULL,getEnergy,NULL,NULL,1},
	{"_FUEL",NULL,getFuel,NULL,NULL,1},
	{"_FUELMAX",NULL,getFuelMax,NULL,NULL,1},
	{"_MOVE3D",NULL,NULL,NULL,move3D,3},
	{"_LINE3D",NULL,NULL,NULL,line3D,3},
	{"_MOVE2D",NULL,NULL,move2D,NULL,2},
	{"_LINE2D",NULL,NULL,line2D,NULL,2},
	{"_SETCOLOR",NULL,setColor,NULL,NULL,1},
	{"_T",NULL,getTolerant,NULL,NULL,1},
	{"_PLAYERS",getPlayers,NULL,NULL,NULL,0},
	{"_PLAYERHOSTID",getPlayerHostID,NULL,NULL,NULL,0},
	{"_PLAYERMYID",getPlayerMyID,NULL,NULL,NULL,0},
	{"_PLAYERID",NULL,getPlayerID,NULL,NULL,1},
	{"_PLAYERCHIPS",NULL,getPlayerChips,NULL,NULL,1},
	{"_PLAYERCOLOR",NULL,getPlayerColor,NULL,NULL,1},
	{"_PLAYERCRUSHES",NULL,getPlayerCrushes,NULL,NULL,1},
	{"_PLAYERRESETS",NULL,getPlayerResets,NULL,NULL,1},
	{"_PLAYERINITS",NULL,getPlayerInits,NULL,NULL,1},
	{"_PLAYERX",NULL,getPlayerX,NULL,NULL,1},
	{"_PLAYERY",NULL,getPlayerY,NULL,NULL,1},
	{"_PLAYERZ",NULL,getPlayerZ,NULL,NULL,1},
	{"_PLAYERARMS",NULL,getPlayerArms,NULL,NULL,1},
	{"_PLAYERYFORCES",NULL,getPlayerYForces,NULL,NULL,1},
	{"_PLAYERNAME",NULL,getPlayerName,NULL,NULL,1},
	{"_PLAYERNAME2",NULL,NULL,getPlayerName2,NULL,2},
	{"???",NULL,NULL,NULL,NULL,0} };

//char *ScriptSource = NULL;
//int ScriptType = 0;
char ScriptStr[MAXNAMESIZE], ScriptChar, ScriptSym;
//int ScriptErrorCode;
char ScriptErrorStr[512];
//int ScriptErrorPc;
int ScriptPc;
int ScriptSymPC;
int ScriptSymPrePC;
int ScriptLine;
int ScriptMode;
double ScriptNum;

void ScriptError(int err);
void getCh(void);
void getSym(void);
double expression(void);
void compare(void);
void addsub(void);
void muldiv(void);
void factor(void);
void labelRead(void);
void statement(void);
int getVariable(char name[], int createFlag);
int getFunction(char *name);


int RunScript()
{
	int i;
	ScriptErrorCode = 0;
	ScriptErrorPc = 0;
	ScriptSymPC = 0;
	ScriptSymPrePC = 0;
	l3dx = l3dy = l3dz = 0.0f;
	graColor = 0xffffff;
	for (i = 0;i < VarCount;i++) {
		lstrcpy(variable[i].name, ValList[i].Name);
		int j = 0;
		while (variable[i].name[j] != '\0') {
			variable[i].name[j] = toupper(variable[i].name[j]);
			j++;
		}
		variable[i].value = ValList[i].Val;
		variable[i].type = 2;
	}
	for (i = VarCount;i < MAXVARIABLE;i++) {
		variable[i].name[0] = '\0';
		variable[i].value = 0;
		variable[i].type = 0;
	}
	ScriptLine = 0;
	ScriptChar = ' ';ScriptPc = 0;getSym();
	labelRead();
	ScriptLine = 0;
	ScriptChar = ' ';ScriptPc = 0;getSym();
	statement();
	randTime += myrand() % 3 + 1;
	if (randTime > 20000000) randTime = 0;
	return ScriptErrorCode;
}

void ScriptError(int err) {
	if (ScriptErrorCode == 0) {
		switch (err) {
		case 1:lstrcpy(ScriptErrorStr, TEXT("Cant divide by 0"));break;
		case 2:lstrcpy(ScriptErrorStr, TEXT("missing '('"));break;
		case 3:lstrcpy(ScriptErrorStr, TEXT("missing ')'"));break;
		case 4:lstrcpy(ScriptErrorStr, TEXT("Expression syntax ScriptError"));break;
		case 5:sprintf(ScriptErrorStr, TEXT("Label not found"));break;
		case 6:sprintf(ScriptErrorStr, TEXT("Variable or Label memory ScriptError( %s ))"), ScriptStr);break;
		case 7:lstrcpy(ScriptErrorStr, TEXT("Syntax ScriptError"));break;
		case 8:lstrcpy(ScriptErrorStr, TEXT("Non ','"));break;
		default:lstrcpy(ScriptErrorStr, TEXT("Syntax Error"));
		}
		ScriptErrorCode = err;
		if (ScriptPc > 1) ScriptErrorPc = ScriptSymPC;
	}
}
void getCh(void)
{
	ScriptChar = ScriptSource[ScriptPc++];
	ScriptChar = toupper(ScriptChar);
	if ((0 <= ScriptChar) && (ScriptChar < 20) && (ScriptChar != '\0') && (ScriptChar != '\n'))ScriptChar = ' ';
}

void getSym(void)
{
	ScriptSymPrePC = ScriptSymPC;
	ScriptSymPC = ScriptPc - 1;
	int k = 0;
	ScriptStr[0] = '\0';
	while (ScriptChar == ' ')getCh();
	if (isalpha(ScriptChar) || ScriptChar == '_') {	//名前
		ScriptSym = IDENTSYM;
		while (isalnum(ScriptChar) || ScriptChar == '_') {
			if (k < MAXNAMESIZE - 2)ScriptStr[k++] = ScriptChar;getCh();
		}
		ScriptStr[k] = '\0';
	}
	else if (isdigit(ScriptChar)) {	//数値
		ScriptSym = NUMSYM;
		while (isdigit(ScriptChar) || ScriptChar == '.') {
			if (k < MAXNAMESIZE - 2)ScriptStr[k++] = ScriptChar;getCh();
		}
		ScriptStr[k] = '\0';
		ScriptNum = 0.0f;
		sscanf(ScriptStr, "%lf", &ScriptNum);
	}
	else if (ScriptChar == '#') {	//数値16進
		ScriptSym = NUMSYM;
		getCh();
		while (isdigit(ScriptChar) || (toupper(ScriptChar) >= 'A' && toupper(ScriptChar) <= 'F')) {
			if (k < MAXNAMESIZE - 2)ScriptStr[k++] = ScriptChar;getCh();
		}
		ScriptStr[k] = '\0';
		int xx;
		sscanf(ScriptStr, "%x", &xx);
		ScriptNum = xx;
	}
	else if (ScriptChar == '"') {	//文字列
		ScriptSym = STRSYM;
		ScriptChar = ScriptSource[ScriptPc++];
		while (ScriptChar != '"'&& ScriptChar != '\n') {
			if (k < MAXNAMESIZE - 2) {
				ScriptStr[k++] = ScriptChar;
				ScriptChar = ScriptSource[ScriptPc++];
			}
		}
		ScriptStr[k] = '\0';
		ScriptNum = 0.0f;
		getCh();
	}
	else {	//記号
		ScriptSym = ScriptChar;
		ScriptStr[0] = ScriptChar;ScriptStr[1] = '\0';
		getCh();
	}
}
double expression(void) {
	double savenum;
	compare();
	savenum = ScriptNum;
	while (ScriptSym == '&' || ScriptSym == '|')
	{
		switch (ScriptSym) {
		case  '&':
			getSym();
			compare();
			savenum = ScriptNum = (double)(savenum&&ScriptNum);
			break;
		case  '|':
			getSym();
			compare();
			savenum = ScriptNum = (double)(savenum || ScriptNum);
			break;
		}
	}
	return ScriptNum;
}
void compare(void) {
	double savenum;
	addsub();
	savenum = ScriptNum;
	while (ScriptSym == '=' || ScriptSym == '>' || ScriptSym == '<')
	{
		switch (ScriptSym) {
		case  '=':
			getSym();
			if (ScriptSym == '=') getSym();
			addsub();
			savenum = ScriptNum = (double)(savenum == ScriptNum);
			break;
		case  '>':
			getSym();
			if (ScriptSym == '=') {
				getSym();
				addsub();
				savenum = ScriptNum = (double)(savenum >= ScriptNum);
			}
			else {
				addsub();
				savenum = ScriptNum = (double)(savenum > ScriptNum);
			}
			break;
		case  '<':
			getSym();
			if (ScriptSym == '=') {
				getSym();
				addsub();
				savenum = ScriptNum = (double)(savenum <= ScriptNum);
			}
			else if (ScriptSym == '>') {
				getSym();
				addsub();
				savenum = ScriptNum = (double)(savenum != ScriptNum);
			}
			else {
				addsub();
				savenum = ScriptNum = (double)(savenum < ScriptNum);
			}
			break;
		}
	}
}
void addsub(void) {
	double savenum;
	muldiv();
	savenum = ScriptNum;

	while (ScriptSym == '+' || ScriptSym == '-') {
		if (ScriptSym == '+') {
			getSym();
			muldiv();
			savenum = ScriptNum = savenum + ScriptNum;
		}
		else {
			getSym();
			muldiv();
			savenum = ScriptNum = savenum - ScriptNum;
		}
	}
}
void muldiv(void)
{
	double savenum;
	factor();
	savenum = ScriptNum;
	while (ScriptSym == '*' || ScriptSym == '/') {
		if (ScriptSym == '*') {
			getSym();factor();
			savenum = ScriptNum = savenum*ScriptNum;
		}
		else {
			getSym();
			factor();
			if (ScriptNum == 0) {
				ScriptError(1);//"Cant divide by 0"
				return;
			}
			savenum = ScriptNum = savenum / ScriptNum;
		}
	}
}
void runFunc(int i) {
	int argc = 0;
	double argv[6] = { 0,0,0,0,0,0 };
	getSym();
	if (ScriptSym != '(' && function[i].argc > 0) { ScriptError(2);return; }//"missing '('"
	if (ScriptSym == '(') {
		getSym();
		if (ScriptSym != ')') {
			expression();
			argv[argc] = ScriptNum;
			argc++;
			while (ScriptSym == ',') {
				getSym();
				expression();
				argv[argc] = ScriptNum;
				argc++;if (argc >= 5) argc = 5;
			}
			if (function[i].argc > argc) { ScriptError(8);return; }//"Non ','"
			if (ScriptSym != ')') { ScriptError(3);return; }//"missing ')'"
		}
		if (function[i].argc == 0) ScriptNum = function[i].func0();
		else if (function[i].argc == 1) ScriptNum = function[i].func1(argv[0]);
		else if (function[i].argc == 2) ScriptNum = function[i].func2(argv[0], argv[1]);
		else if (function[i].argc == 3) ScriptNum = function[i].func3(argv[0], argv[1], argv[2]);
		if (ScriptSym == ')') getSym();
	}
}
void factor(void) {
	ScriptSymPrePC = ScriptSymPC;
	ScriptSymPC = ScriptPc - 1;
	int i;
	if (ScriptSym == IDENTSYM) {
		i = getFunction(ScriptStr);
		if (i >= 0) {
			runFunc(i);
		}
		else {
			i = getVariable(ScriptStr, 1);
			ScriptNum = variable[i].value;
			getSym();
		}
	}
	else if (ScriptSym == NUMSYM) {
		getSym();
	}
	else if (ScriptSym == '(') {
		getSym();
		expression();
		if (ScriptSym != ')') {
			ScriptError(3);return;//"missing ')'"
		}
		getSym();
	}
	else if (ScriptSym == '-') {
		getSym();
		factor();
		ScriptNum = -ScriptNum;
	}
	else { ScriptError(4);return; }//"Expression syntax ScriptError"
}

int getVariable(char name[], int createFlag)
{
	ScriptSymPrePC = ScriptSymPC;
	ScriptSymPC = ScriptPc - 1;
	int i = 0;
	while (strcmp(name, variable[i].name))
	{
		if (variable[i].name[0] == '\0') {
			if (createFlag == 0) { ScriptError(5);return -1; }//"Label not found"
			else {
				strcat(variable[i].name, name);
				return(i);
			}
		}
		i++;
		if (i > MAXVARIABLE) { ScriptError(6);return -1; }//"Variable or Label memory ScriptError"
	}
	return(i);
}

int getFunction(char *name)
{
	int i = 0;
	while (strcmp(name, function[i].name)) {
		if (!strcmp("???", function[i].name)) return -1;
		i++;
	}
	return(i);
}

void labelRead(void)
{
	int i;
	while (ScriptSym != '\0') {
		if (!strcmp(ScriptStr, "LABEL")) {
			getSym();
			if (ScriptSym != IDENTSYM) { ScriptError(7);return; }//"Syntax ScriptError"
			strcat(ScriptStr, ":");
			i = getVariable(ScriptStr, 1);
			variable[i].value = (double)(ScriptPc - 1);
		}
		getSym();
	}
}
void statement(void)
{
	char string[512];
	char str[512];
	int i;
	while (ScriptSym != '\0') {
		if (!strcmp(ScriptStr, "LABEL")) {
			getSym();
			getSym();
		}
		else if (!strcmp(ScriptStr, "PRINT") || !strcmp(ScriptStr, "OUT"))
		{
			int no;
			string[0] = '\0';
			getSym();
			expression();
			no = (int)ScriptNum;
			if (ScriptSym != ',') { ScriptError(8);return; }//"Non ','"
			do {
				getSym();
				if (ScriptSym == STRSYM) {
					if (strlen(string) + strlen(ScriptStr) < 255) strcat(string, ScriptStr);
					getSym();
				}
				else if (ScriptSym == '$') {
					getSym();
					expression();
					int nn = (int)ScriptNum;
					char *sn = (char*)&nn;
					char strn[5];
					strn[0] = sn[0];strn[1] = sn[1];strn[2] = sn[2];strn[3] = sn[3];
					strn[4] = '\0';
					if (strlen(string) + strlen(strn) < 255) strcat(string, strn);
				}
				else {
					expression();
					sprintf(str, "%.2f", ScriptNum);
					if (strlen(string) + strlen(str) < 255) strcat(string, str);

				}
			} while (ScriptSym == ',');
			output(no, string);
		}
		else if (!strcmp(ScriptStr, "FPRINT") || !strcmp(ScriptStr, "FOUT"))
		{
			int no;
			string[0] = '\0';
			getSym();
			expression();
			no = (int)ScriptNum;
			if (ScriptSym != ',') { ScriptError(8);return; }//"Non ','"
			do {
				getSym();
				if (ScriptSym == STRSYM) {
					if (strlen(string) + strlen(ScriptStr) < 255) strcat(string, ScriptStr);
					getSym();
				}
				else if (ScriptSym == '$') {
					getSym();
					expression();
					int nn = (int)ScriptNum;
					char *sn = (char*)&nn;
					char strn[5];
					strn[0] = sn[0];strn[1] = sn[1];strn[2] = sn[2];strn[3] = sn[3];
					strn[4] = '\0';
					if (strlen(string) + strlen(strn) < 255) strcat(string, strn);
				}
				else {
					expression();
					sprintf(str, "%.2f", ScriptNum);
					if (strlen(string) + strlen(str) < 255) strcat(string, str);

				}
			} while (ScriptSym == ',');
			output(no, string);
		}
		else if (!strcmp(ScriptStr, "GOTO"))
		{
			getSym();
			if (ScriptSym != IDENTSYM) { ScriptError(7);return; }//"Syntax ScriptError"
			strcat(ScriptStr, ":");
			i = getVariable(ScriptStr, 0);
			if (i < 0) { ScriptError(5);return; }//"Label not found"
			ScriptPc = (int)variable[i].value;
			ScriptChar = ' ';
			getSym();
		}
		else if (!strcmp(ScriptStr, "IF"))
		{
			getSym();
			expression();
			if (strcmp(ScriptStr, "{")) { ScriptError(7);return; }//"Syntax ScriptError"
			if (ScriptNum == 0) while (ScriptSym != '}') getSym();
			getSym();
		}
		else if (!strcmp(ScriptStr, "END") || ScriptSym == '\0') {
			return;
		}
		else if (ScriptSym == IDENTSYM)
		{
			i = getFunction(ScriptStr);
			if (i >= 0) {

				if (i >= 0)runFunc(i);
				getSym();
			}
			else {
				i = getVariable(ScriptStr, 1);
				getSym();
				if (ScriptSym != '=') { ScriptError(7);return; }//"Syntax ScriptError"
				getSym();
				expression();
				variable[i].value = ScriptNum;
				if (variable[i].type == 2) {
					ValList[i].Val = (float)ScriptNum;
					ValList[i].Updated = true;
					if (ValList[i].Val > ValList[i].Max) ValList[i].Val = ValList[i].Max;
					if (ValList[i].Val < ValList[i].Min) ValList[i].Val = ValList[i].Min;
					updateVal();
				}
			}
		}
		else if ((ScriptSym == ':') || (ScriptSym == '}')) getSym();
		else { ScriptError(7);return; }//"Syntax ScriptError"
		if (ScriptErrorCode != 0) return;
	}
}
