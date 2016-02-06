#ifndef G_LUASYSTEM_H
#define G_LUASYSTEM_H

#include "GVector.hpp"
#include "GRigid.hpp"
#include "readData.hpp"

#define lua_c

extern "C" {
    #include "Lua/include/lua.h"
    #include "Lua/include/lualib.h"
    #include "Lua/include/lauxlib.h"
}

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

extern LPDIRECT3DDEVICE8 G3dDevice;
extern GValList ValList[];
extern GKeyList KeyList[];
extern GWorld *World;
extern GRigid *Chip[];
extern int ChipCount;
extern int VarCount;
extern int TickCount;
extern int SystemTickCount;
extern double FPS;
extern int Width;
extern int Height;
extern int NumFace;
extern int CCDImage[64][64];
extern int LastBye;
extern int Analog[];
extern int Hat[];
extern float CCDZoom;
extern char ScriptOutput[][512];
//extern bool ObjectBallFlag;
extern int ScriptErrorCode;
extern char ScriptErrorStr[];
extern CMyD3DApplication* g_pApp;
extern GVector AirSpeed;//ïó
extern bool SystemKeys[];
extern bool SystemKeysDown[];
extern bool SystemKeysUp[];
extern char szUpdateFileName[];
extern char szUpdateFileName0[];
extern char szLandFileName[];
extern char szLandFileName0[];
extern char szTempFileName0[];
extern char szSystemFileName[];
extern char szSystemFileName0[];
extern TCHAR AppDir[];
extern TCHAR ResourceDir[];
extern TCHAR DataDir[];
extern TCHAR CurrDataDir[];
extern TCHAR CurrScenarioDir[];

extern CD3DMesh*	m_pLandMesh;	// XMeshÉfÅ[É^
extern HWND g_hWnd;              // The main app window

HRESULT LoadLand(LPDIRECT3DDEVICE8 Device,char *fname);
char *SearchFolder(char *path,char *filename,char *result);
void ResetChip2(int n,GFloat a);
void setOption(GRigid *rigid,GFloat value);

#define	MESSAGEMAX 64
extern char MessageData[];
extern int RecieaveMessageCode[];
extern char RecieaveMessageData[][MESSAGEMAX+1];


int LoadSystem(char *fileName);
int luaSystemInit();
void luaSystemEnd();
int luaSystemRun (char *funcName);
#endif