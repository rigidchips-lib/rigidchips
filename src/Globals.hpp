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

//ここからグローバル変数定義
EXTERN LPDIRECT3DDEVICE8 g_D3DDevice;
EXTERN GValList g_ValList[GVALMAX];
EXTERN GKeyList g_KeyList[GKEYMAX];
EXTERN GWorld *g_World;
EXTERN GRigid *g_Chip[GCHIPMAX];
EXTERN CMyD3DApplication* g_pApp;
EXTERN CD3DMesh* g_pLandMesh;	// XMeshデータ
EXTERN HWND g_hWnd;              // The main app window
//ネットワーク関係
EXTERN GDPlay *g_DPlay;
EXTERN GPLAYERDATA g_PlayerData[GPLAYERMAX];
EXTERN GMYDATA g_MyPlayerData;

//パーティクル描画関係
EXTERN GParticle *g_GroundParticle;
EXTERN GParticle *g_WaterLineParticle;
EXTERN GParticle *g_JetParticle;
EXTERN GBullet *g_Bullet;
//現在のモデルのVar数
EXTERN int g_VarCount;

//Script関係（あんまり手を付けたくない）
EXTERN char *ScriptSource;
EXTERN int ScriptType;
EXTERN char ScriptOutput[GOUTPUTMAX][512];
EXTERN int ScriptErrorCode;
EXTERN int ScriptErrorPc;
EXTERN char ScriptErrorStr[];

//Lua関係
//モデルの方のLua
EXTERN lua_State *g_ScriptLua;
//シナリオLua
EXTERN lua_State *g_SystemLua;
//多分３ＤLINEの描画位置
EXTERN float luaL3dx, luaL3dy, luaL3dz;
//現在のLuaで引く色の色
EXTERN int luaGraColor;
EXTERN char *SystemSource;
EXTERN char SystemOutput[GOUTPUTMAX][512];
EXTERN int SystemErrorCode;
EXTERN char SystemErrorStr[512];
//ネットワークノイズの内、長期のサインカーブの方 ScriptとLuaで共用する必要から、グローバルのまま
EXTERN int g_RandTime;

//システム情報関連。グローバルである必要あるのか？
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
EXTERN GFloat Zoom;
EXTERN GVector AirSpeed;//風

//レギュレーションの設定
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
EXTERN bool ControlKeysLock[8];//0:Init,1:Reset,2:Open,3:Update,4:OpenLand,5:OpenGame,6:YForce,7:Title

EXTERN int ViewUpdate;//=0

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

EXTERN int scenarioCode;//=0
//モデル読み込み時のエラーフラグぽい？
EXTERN int DataCheck;// = 0;

EXTERN GVector CompassTarget;
EXTERN GRing Ring[];
EXTERN char LastChatData[];
#endif