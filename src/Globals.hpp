#ifndef G_GLOBALS
#define G_GLOBALS
#include <Windows.h>
#include "Consts.hpp"
#include "luaSystem.hpp"
#include "luaSub.hpp"
#include "readData.hpp"
#include "GPlayers.h"
#include "GDPlay.hpp"
#include "GParticle.hpp"

#include "Rigidmain.h"
//Global.cppでだけ実態定義、他ではexternの外部参照扱い
#ifdef _GLOBAL_BODY
#define EXTERN 
#else
#define EXTERN extern
#endif

//struct lua_State;
//typedef float GFloat;


//ここからグローバル変数定義
EXTERN LPDIRECT3DDEVICE8 G3dDevice;
EXTERN GValList ValList[GVALMAX];
EXTERN GKeyList KeyList[GKEYMAX];
EXTERN GWorld *World;
EXTERN GRigid *Chip[GCHIPMAX];

EXTERN CMyD3DApplication* g_pApp;

EXTERN GParticle *GroundParticle;
EXTERN GParticle *WATER_LINEParticle;
EXTERN GParticle *JetParticle;
EXTERN GBullet *Bullet;
//EXTERN bool ObjectBallFlag;
EXTERN DWORD ShowGhost;
EXTERN DWORD ShowDustFlag;
//モデル読み込みからのフレーム数
EXTERN int g_TickCount;
//起動からのフレーム数
EXTERN int g_SystemTickCount;

//正しい初期化位置がわからないので窮余の策
#ifdef _GLOBAL_BODY
EXTERN float g_FarMax=600.0f;
EXTERN float g_MarkerSize=1.0f;
EXTERN float g_NameSize=1.0f;
EXTERN GFloat TotalPower = 0;
#else
EXTERN float g_FarMax;
EXTERN float g_MarkerSize;
EXTERN float g_NameSize;
EXTERN GFloat TotalPower;
#endif

EXTERN char *ScriptSource;
EXTERN int ScriptType;
EXTERN char ScriptOutput[GOUTPUTMAX][512];
EXTERN int ScriptErrorCode;
EXTERN int ScriptErrorPc;
EXTERN char ScriptErrorStr[];

EXTERN lua_State *SystemL;
EXTERN char *SystemSource;
EXTERN char SystemOutput[GOUTPUTMAX][512];
EXTERN int SystemErrorCode;
EXTERN char SystemErrorStr[512];

EXTERN lua_State *ScriptL;

EXTERN char szUpdateFileName0[];
EXTERN float luaL3dx, luaL3dy, luaL3dz;
EXTERN int luaGraColor;
EXTERN int randTime;

EXTERN int ChipCount;
EXTERN int VarCount;

EXTERN double FPS;
#ifdef _GLOBAL_BODY
EXTERN int g_LimitFPS=30;
#else
EXTERN int g_LimitFPS;//=30
#endif
EXTERN int Width;
EXTERN int Height;
EXTERN int NumFace;
EXTERN int CCDImage[CCD_RESOLUTION][CCD_RESOLUTION];
EXTERN int LastBye;
EXTERN int Analog[6];
EXTERN int Hat[1];
EXTERN int	MouseX;
EXTERN int	MouseY;
EXTERN int	MouseL;
EXTERN int	MouseR;
EXTERN int	MouseM;
EXTERN int	CtrlKey;
EXTERN float CCDZoom; //float CCDZoom = 0.3f*180.0f / (GFloat)M_PI;
EXTERN char ScriptOutput[][512];
//extern bool ObjectBallFlag;
EXTERN char ScriptErrorStr[];
EXTERN int ViewType;
EXTERN GVector UserEyePos;
EXTERN GVector UserRefPos;
EXTERN GVector UserUpVec;


#ifdef _GLOBAL_BODY
EXTERN bool GravityFlag = TRUE;
EXTERN bool AirFlag = TRUE;
EXTERN bool TorqueFlag = TRUE;
EXTERN bool JetFlag = TRUE;
EXTERN bool UnbreakableFlag = TRUE;
EXTERN bool ScriptFlag = TRUE;
EXTERN bool CCDFlag = TRUE;
EXTERN bool EfficientFlag = TRUE;
#else
EXTERN bool GravityFlag;
EXTERN bool AirFlag;
EXTERN bool TorqueFlag;
EXTERN bool JetFlag;
EXTERN bool UnbreakableFlag;
EXTERN bool ScriptFlag;
EXTERN bool CCDFlag;
EXTERN bool EfficientFlag;
#endif

EXTERN DWORD UseLuaExternal;

EXTERN int ViewUpdate;//=0
EXTERN GDPlay *DPlay;
EXTERN GPLAYERDATA PlayerData[GPLAYERMAX];
EXTERN GMYDATA MyPlayerData;

#ifdef _GLOBAL_BODY
EXTERN char szUpdateFileName[_MAX_PATH] = "";
EXTERN char szUpdateFileName0[_MAX_PATH] = "";
EXTERN char szLandFileName[_MAX_PATH] = "";
EXTERN char szLandFileName0[_MAX_PATH] = "";
EXTERN char szTempFileName0[_MAX_PATH] = "";
EXTERN char szSystemFileName[_MAX_PATH] = "";
EXTERN char szSystemFileName0[_MAX_PATH] = "";
#else
EXTERN char szUpdateFileName[_MAX_PATH];
EXTERN char szUpdateFileName0[_MAX_PATH];
EXTERN char szLandFileName[_MAX_PATH];
EXTERN char szLandFileName0[_MAX_PATH];
EXTERN char szTempFileName0[_MAX_PATH];
EXTERN char szSystemFileName[_MAX_PATH];
EXTERN char szSystemFileName0[_MAX_PATH];
#endif
EXTERN GFloat Zoom;
EXTERN GVector AirSpeed;//風

EXTERN bool SystemKeys[GSYSKEYMAX];
EXTERN bool SystemKeysDown[GSYSKEYMAX];
EXTERN bool SystemKeysUp[GSYSKEYMAX];

EXTERN TCHAR AppDir[MAX_PATH];
EXTERN TCHAR ResourceDir[MAX_PATH];
EXTERN TCHAR DataDir[MAX_PATH];
EXTERN TCHAR CurrDataDir[MAX_PATH];
EXTERN TCHAR CurrScenarioDir[MAX_PATH];

EXTERN int scenarioCode;//=0

EXTERN int DataCheck;// = 0;


EXTERN GVector CompassTarget;
EXTERN GRing Ring[];
EXTERN char LastChatData[];

EXTERN char MessageData[MESSAGEMAX + 1];
EXTERN int RecieaveMessageCode[GPLAYERMAX];
EXTERN char RecieaveMessageData[GPLAYERMAX][MESSAGEMAX + 1];

EXTERN bool ControlKeysLock[8];//0:Init,1:Reset,2:Open,3:Update,4:OpenLand,5:OpenGame,6:YForce,7:Title

EXTERN CD3DMesh*	m_pLandMesh;	// XMeshデータ
EXTERN HWND g_hWnd;              // The main app window
#endif