//-----------------------------------------------------------------------------
// File: Rigid.cpp
//
// Desc: DirectX window application created by the DirectX AppWizard
//-----------------------------------------------------------------------------

//#include <windows.h>
#include "resource.h"
#include "Globals.hpp"
#include "Consts.hpp"
#include "GParticle.hpp"
#include "commctrl.h"

#define __MEMLEAKCHECK 
#include "GVector.hpp"
#include "GRigid.hpp"
#include "Rigidmain.h"
#include "readData.hpp"
#include "luaScript.hpp"
#include "luaSystem.hpp"
#include "GDPlay.hpp"
#include "GPlayers.h"

//#include "WINNLS32.H"

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

bool MsgFlag = false;
char Kokuti[256] = "";
int dataCode = 0;
int gameCode = 0;
int landCode = 0;
LPD3DXFONT	g_pFont = NULL;		// D3DXFontインターフェイス
UINT	m_ShockMess;

/*--------------------------------------------
頂点フォーマット
---------------------------------------------*/
typedef struct {
	int type;
	float x, y, z;
	float size;
	float alpha;
	float r, g, b;
	int id;
}DustData;

typedef struct _D3DPOINTVERTEX_ {
	float x, y, z;
	DWORD color;
}D3DPOINTVERTEX;

//#define D3DFVF_POINTVERTEX 		(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1| D3DFVF_PSIZE)
#define D3DFVF_POINTVERTEX 		(D3DFVF_XYZ | D3DFVF_DIFFUSE)
inline DWORD FtoDW(FLOAT f) { return *((DWORD*)&f); }
D3DLIGHT8 light;
//D3DLIGHT8 light1;

int KeyRecord[GRECMAX][GKEYMAX + 7];//AnalogとHat
int RecCheckPoint;
int KeyRecordMode = 0;
int KeyRecordMax = 0;
int KeyRecordCount = 0;
int RecState = 0;

typedef struct {
	char Name[128];
	bool GravityFlag;
	bool AirFlag;
	bool TorqueFlag;
	bool JetFlag;
	bool UnbreakableFlag;
	bool CCDFlag;
	bool ScriptFlag;
	bool EfficientFlag;
	GVector StartPoint;
	float StartAngleY;
	GVector Point[GCHECKPOINTMAX];
	GVector Dir[GCHECKPOINTMAX];
	float Scale[GCHECKPOINTMAX];
	int Count;
} GCheckPoint;


int CurrentCourse = 0;
int CourseCount = 1;
GCheckPoint Course[GCOURSEMAX];
GRing Ring[GRINGMAX];

int GameTime = 0;
int RecGameTime = 0;
int RecLastBye = 0;
int waitCount = 0;
int RecWaitCount = 0;

int CurrentCheckPoint = 0;

DWORD SoundType;

CD3DMesh*	m_pSkyMesh;				// XMeshデータ
CD3DMesh*	m_pXMesh[GMODELMAX];	// XMeshデータ
D3DXMATRIX GMatWorld;
D3DXMATRIX GMatView;

//LPDIRECT3DVERTEXBUFFER8 pPointVB = NULL;
LPDIRECT3DVERTEXBUFFER8 pPointVB = NULL;
LPDIRECT3DTEXTURE8 pPointTexture = NULL;
LPD3DXSPRITE pSprite = NULL;
LPDIRECT3DTEXTURE8 pMyTexture[GTEXMAX];

HMIDIOUT hMidiOut;

static float g_FarMax = 600.0f;
static float g_MarkerSize = 1.0f;
static float g_NameSize = 1.0f;

static int s_ScreenWidth;
static int s_ScreenHeight;
static int s_NumFace;
int RecTickCount = 0;

bool LockGravityFlag = FALSE;
bool LockAirFlag = FALSE;
bool LockTorqueFlag = FALSE;
bool LockJetFlag = TRUE;
bool LockUnbreakableFlag = FALSE;
bool LockScriptFlag = FALSE;
bool LockCCDFlag = FALSE;
bool LockEnergyFlag = FALSE;

bool recGravityFlag = TRUE;
bool recAirFlag = TRUE;
bool recTorqueFlag = TRUE;
bool recJetFlag = TRUE;
bool recUnbreakableFlag = TRUE;
bool recScriptFlag = TRUE;
bool recCCDFlag = TRUE;
bool recEnergyFlag = TRUE;

bool HardShadowFlag = false;

int CallModeChange = 0;

BOOL ShowTitle = FALSE;
static DWORD ShowMeter = TRUE;
static DWORD ShowRegulation = TRUE;
static DWORD ShowMessage = TRUE;
static DWORD ShowData = FALSE;
static DWORD ShowExtra = FALSE;
static DWORD ShowNetwork = FALSE;
static DWORD ShowVariable = TRUE;
static DWORD ShowPower = FALSE;
static DWORD ShowCowl = TRUE;
static DWORD ShowGhost = FALSE;
static DWORD TextureAlpha = TRUE;
static DWORD BackFaces = FALSE;
static DWORD ShowShadowFlag = TRUE;
static DWORD ShowDustFlag = TRUE;
static DWORD DitherFlag = TRUE;
static DWORD FastShadow = 1;
bool MoveEnd = false;

TCHAR SessionName[256] = "RigidChips";
TCHAR PlayerName[MAX_PLAYER_NAME] = "Player";
long PlayerColor = 0xffffff;
TCHAR HostName[256] = "localhost";
DWORD PortNo = 2345;

#define	GMAXCHATLINE 101
char ChatData[GMAXCHATLINE][256];
char LastChatData[256];
int  ChatDataStart = 0;
int  ChatDataEnd = 0;

#define	GMAXATTACKLINE 101
char AttackData[GMAXATTACKLINE][256];
int  AttackDataStart = 0;
int  AttackDataEnd = 0;

//bool ObjectBallFlag=TRUE;
BOOL WindCalm = TRUE;
BOOL WindSoft = FALSE;
BOOL WindStrong = FALSE;
BOOL WindTyphoon = FALSE;
unsigned long TitleAlpha = 0x00ffffff;

int ResetCount = 90;

GVector lightColor;
GVector FogColor;
GVector EyePos, RefPos, UpVec;

GFloat TurnLR;
GFloat TurnUD;
int RunScript();

int myRand();
void MySrand(unsigned int seed);

void GRigid::CreateViewModel()
{
	//	Model=NULL;
}
char ChipDataStrng[200000];

bool InputCancel = false;

HWND SourceDlg = NULL;
HWND ExtraDlg = NULL;
HWND NetworkDlg = NULL;


//-----------------------------------------------------------------------------
// Defines, and constants
//-----------------------------------------------------------------------------
// This GUID must be unique for every game, and the same for 
// every instance of this app.  // {81A83B95-073B-45b4-A567-810F97F99B77}
// The GUID allows DirectInput to remember input settings
GUID g_guidApp = { 0x81a83b95, 0x73b, 0x45b4, { 0xa5, 0x67, 0x81, 0xf, 0x97, 0xf9, 0x9b, 0x77 } };


// Input semantics used by this app
enum INPUT_SEMANTICS
{
	// Gameplay semantics
	// TODO: change as needed
	INPUT_ROTATE_AXIS_LR = 1, INPUT_ROTATE_AXIS_UD, INPUT_ROTATE_AXIS_IO, INPUT_ROTATEX, INPUT_ROTATEY, INPUT_ROTATEZ, INPUT_HATSWITCH,
	INPUT_CONFIG_INPUT, INPUT_CONFIG_DISPLAY,
	INPUT_INIT, INPUT_RESET, INPUT_TITLE, INPUT_ZOOMIN, INPUT_ZOOMOUT, INPUT_YFORCE, INPUT_RESETVIEW, INPUT_TUP, INPUT_TDOWN, INPUT_TLEFT, INPUT_TRIGHT,
	INPUT_0, INPUT_1, INPUT_2, INPUT_3, INPUT_4, INPUT_5, INPUT_6, INPUT_7, INPUT_8, INPUT_9,
	INPUT_10, INPUT_11, INPUT_12, INPUT_13, INPUT_14, INPUT_15, INPUT_16,
	SYSTEM_0, SYSTEM_1, SYSTEM_2, SYSTEM_3
};

// Actions used by this app
DIACTION g_rgGameAction[] =
{


	// Device input (joystick, etc.) that is pre-defined by DInput, according
	// to genre type. The genre for this app is space simulators.
	{ INPUT_ROTATE_AXIS_LR,  DIAXIS_3DCONTROL_LATERAL,      0, TEXT("Rotate left/right"), },
	{ INPUT_ROTATE_AXIS_UD,  DIAXIS_3DCONTROL_MOVE,         0, TEXT("Rotate up/down"), },
	{ INPUT_ROTATE_AXIS_IO,  DIAXIS_3DCONTROL_INOUT,		0, TEXT("Rotate in/out"), },
	{ INPUT_ROTATEX,  DIAXIS_3DCONTROL_ROTATEX,         0, TEXT("Rotate X"), },
	{ INPUT_ROTATEY,  DIAXIS_3DCONTROL_ROTATEY,         0, TEXT("Rotate Y"), },
	{ INPUT_ROTATEZ,  DIAXIS_3DCONTROL_ROTATEZ,         0, TEXT("Rotate Z"), },
	{ INPUT_HATSWITCH,	DIHATSWITCH_3DCONTROL_HATSWITCH,0,TEXT("HatSwitch"),},
	// Keyboard input mappings
	{ INPUT_0,	DIKEYBOARD_UP,	0, TEXT("Button 0"), },
	{ INPUT_1,	DIKEYBOARD_DOWN,	0, TEXT("Button 1"), },
	{ INPUT_2,	DIKEYBOARD_LEFT,	0, TEXT("Button 2"), },
	{ INPUT_3,	DIKEYBOARD_RIGHT,	0, TEXT("Button 3"), },
	{ INPUT_4,	DIKEYBOARD_Z,	0, TEXT("Button 4"), },
	{ INPUT_5,	DIKEYBOARD_X,	0, TEXT("Button 5"), },
	{ INPUT_6,	DIKEYBOARD_C,	0, TEXT("Button 6"), },
	{ INPUT_7,	DIKEYBOARD_A,	0, TEXT("Button 7"), },
	{ INPUT_8,	DIKEYBOARD_S,	0, TEXT("Button 8"), },
	{ INPUT_9,	DIKEYBOARD_D,	0, TEXT("Button 9"), },
	{ INPUT_10,	DIKEYBOARD_V,	0, TEXT("Button 10"), },
	{ INPUT_11,	DIKEYBOARD_B,	0, TEXT("Button 11"), },
	{ INPUT_12,	DIKEYBOARD_F,	0, TEXT("Button 12"), },
	{ INPUT_13,	DIKEYBOARD_G,	0, TEXT("Button 13"), },
	{ INPUT_14,	DIKEYBOARD_Q,	0, TEXT("Button 14"), },
	{ INPUT_15,	DIKEYBOARD_W,	0, TEXT("Button 15"), },
	{ INPUT_16,	DIKEYBOARD_E,	0, TEXT("Button 16"), },
	{ SYSTEM_0,	DIKEYBOARD_H,	0, TEXT("System 0"), },
	{ SYSTEM_1,	DIKEYBOARD_J,	0, TEXT("System 1"), },
	{ SYSTEM_2,	DIKEYBOARD_N,	0, TEXT("System 2"), },
	{ SYSTEM_3,	DIKEYBOARD_M,	0, TEXT("System 3"), },
	{ INPUT_INIT,	DIKEYBOARD_U,	0, TEXT("Init"), },
	{ INPUT_RESET,	DIKEYBOARD_R,	0, TEXT("Reset"), },
	{ INPUT_TITLE,	DIKEYBOARD_T,	0, TEXT("Show About"), },
	{ INPUT_ZOOMIN,	DIKEYBOARD_I,	0, TEXT("Zoom-in"), },
	{ INPUT_ZOOMOUT,DIKEYBOARD_O,	0, TEXT("Zoom-out"), },
	{ INPUT_RESETVIEW,	DIKEYBOARD_P,	0, TEXT("Reset View"), },
	{ INPUT_TUP,	DIKEYBOARD_K,	0, TEXT("Turn Up"), },
	{ INPUT_TDOWN,	DIKEYBOARD_L,	0, TEXT("Turn Down"), },
	{ INPUT_TLEFT,	DIKEYBOARD_COMMA,	0, TEXT("Turn Left"), },
	{ INPUT_TRIGHT,	DIKEYBOARD_PERIOD,	0, TEXT("Turn Right"), },
	{ INPUT_YFORCE,	DIKEYBOARD_Y,	0, TEXT("Y Force"), },
	//    { INPUT_CONFIG_DISPLAY,	DIKEYBOARD_F2,	DIA_APPFIXED, TEXT("Configure Display"), },    
	//    { INPUT_CONFIG_INPUT,	DIKEYBOARD_F3,	DIA_APPFIXED, TEXT("Configure Input"), },    
};

#define NUMBER_OF_GAMEACTIONS    (sizeof(g_rgGameAction)/sizeof(DIACTION))

//-----------------------------------------------------------------------------
// Global access to the app (needed for the global WndProc())
//-----------------------------------------------------------------------------

HINSTANCE          g_hInst = NULL;


GPLAYERDATA PrePlayerData[GPLAYERMAX];

DWORD MyNetDataSize = 0;

void AttackDataDisp(char *s, DPNID dpnid, int attack) {
	if (NetworkDlg == NULL) return;
	int i;
	char str[256 * GMAXATTACKLINE];
	char name2[MAX_PLAYER_NAME];
	for (i = 0;i < g_DPlay->GetMaxPlayers();i++) {
		if (g_PlayerData[i].ReceiveData.info.dpnidPlayer == dpnid) {
			strcpy(name2, g_PlayerData[i].ReceiveData.info.strPlayerName);
			break;
		}
	}
	TCHAR* strLastSharp = _tcsrchr(name2, TEXT('#'));
	if (strLastSharp) *strLastSharp = '\0';
	sprintf(AttackData[AttackDataEnd], "%s %s\r\n", s, name2);
	AttackDataEnd++;
	if (AttackDataEnd >= GMAXATTACKLINE) {
		AttackDataEnd = 0;
	}
	if (AttackDataEnd <= AttackDataStart) {
		AttackDataStart = AttackDataEnd + 1;
	}
	AttackData[AttackDataEnd][0] = '\0';
	i = AttackDataStart;if (i >= GMAXATTACKLINE) i = 0;
	str[0] = '\0';
	while (i != AttackDataEnd) {
		strcat(str, AttackData[i]);
		i++;
		if (i >= GMAXATTACKLINE) i = 0;
	}
	SendDlgItemMessage(NetworkDlg, IDC_ATTACKTEXT, WM_SETTEXT, 0, (LPARAM)str);
	SendMessage(GetDlgItem(NetworkDlg, IDC_ATTACKTEXT), EM_SETSEL, 0, -1); //全てを選択
	SendMessage(GetDlgItem(NetworkDlg, IDC_ATTACKTEXT), EM_SETSEL, -1, -1); //解除
	SendMessage(GetDlgItem(NetworkDlg, IDC_ATTACKTEXT), EM_REPLACESEL, 0, (WPARAM)"_");
}

char  *ChatDataDisp(char *name, char *s) {
	if (NetworkDlg == NULL) return NULL;
	char str[256 * GMAXCHATLINE];
	char name2[MAX_PLAYER_NAME];
	strcpy(name2, name);
	//日付と時間を調べる
	SYSTEMTIME st;
	GetLocalTime(&st);
	TCHAR* strLastSharp = _tcsrchr(name2, TEXT('#'));
	if (strLastSharp) *strLastSharp = '\0';
	sprintf(ChatData[ChatDataEnd], "%02d/%02d %02d:%02d:%02d %s>%s\r\n", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, name2, s);
	char *ret = ChatData[ChatDataEnd];
	ChatDataEnd++;
	if (ChatDataEnd >= GMAXCHATLINE) {
		ChatDataEnd = 0;
	}
	if (ChatDataEnd <= ChatDataStart) {
		ChatDataStart = ChatDataEnd + 1;
	}
	ChatData[ChatDataEnd][0] = '\0';
	int i = ChatDataStart;if (i >= GMAXCHATLINE) i = 0;
	str[0] = '\0';
	while (i != ChatDataEnd) {
		strcat(str, ChatData[i]);
		i++;
		if (i >= GMAXCHATLINE) i = 0;
	}
	SendDlgItemMessage(NetworkDlg, IDC_CHATTEXT, WM_SETTEXT, 0, (LPARAM)str);
	SendMessage(GetDlgItem(NetworkDlg, IDC_CHATTEXT), EM_SETSEL, 0, -1); //全てを選択
	SendMessage(GetDlgItem(NetworkDlg, IDC_CHATTEXT), EM_SETSEL, -1, -1); //解除
	SendMessage(GetDlgItem(NetworkDlg, IDC_CHATTEXT), EM_REPLACESEL, 0, (WPARAM)"_");
	return ret;
}

HRESULT MyReceiveFunc(MYAPP_PLAYER_INFO* playerInfo, DWORD size, BYTE *stream) {

	HRESULT hr = S_OK;
	BYTE *strm = new BYTE[size];
	memcpy(strm, stream, size); //いったんコピー
	BYTE *data = strm + sizeof(short);
	short code = *((short*)strm);
	if (code == -1) {
		for (int i = 0;i < g_DPlay->GetMaxPlayers();i++) {
			if (g_PlayerData[i].ReceiveData.info.dpnidPlayer == playerInfo->dpnidPlayer) {
				PrePlayerData[i].ReceiveData.size = 0;
				g_PlayerData[i].ReceiveData.size = 0;
			}
		}
	}
	else if (code == 0) {
		for (int i = 0;i < g_DPlay->GetMaxPlayers();i++) {
			if (g_PlayerData[i].ReceiveData.info.dpnidPlayer == playerInfo->dpnidPlayer) {
				int s = g_PlayerData[i].ReceiveData.size;
				PrePlayerData[i].ReceiveData.size = 0;
				g_PlayerData[i].ReceiveData.size = 0;
				PrePlayerData[i] = g_PlayerData[i];
				PrePlayerData[i].ReceiveData.info = g_PlayerData[i].ReceiveData.info;
				for (int j = 0;j < s;j++) {
					((BYTE*)PrePlayerData[i].ReceiveData.data)[j] = ((BYTE*)g_PlayerData[i].ReceiveData.data)[j];
				}
				for (int j = 0;j < g_PlayerData[i].ChipCount;j++) {
					PrePlayerData[i].X[j] = g_PlayerData[i].X[j];
				}
				for (unsigned int j = 0;j < size;j++) {
					((BYTE*)g_PlayerData[i].ReceiveData.data)[j] = data[j];
				}
				g_PlayerData[i].time = timeGetTime();
				g_PlayerData[i].time2 = timeGetTime();
				PrePlayerData[i].ReceiveData.size = s;
				g_PlayerData[i].ReceiveData.size = size;
				break;
			}
		}
	}
	else if (code == 1) { //チャットデータ
		if (NetworkDlg) {
			char *ret = ChatDataDisp(playerInfo->strPlayerName, (char*)data);
			strcpy(LastChatData, ret);
			if (g_SystemLua != NULL && g_World->B26Bullet) luaSystemRun("OnChat");
		}
	}
	else if (code == 30) { //15B26追加　シナリオメッセージ
		for (int i = 0;i < g_DPlay->GetMaxPlayers();i++) {
			if (g_PlayerData[i].ReceiveData.info.dpnidPlayer == playerInfo->dpnidPlayer) {
				g_RecieaveMessageCode[i] = *((int*)data);
				if (scenarioCode == g_RecieaveMessageCode[i]) {
					char *str = (char*)&data[4];
					strcpy(g_RecieaveMessageData[i], str);
				}
			}
		}
	}

	else if (code == 10) { //15B20追加　Coreの位置情報のみ送る
		for (int i = 0;i < g_DPlay->GetMaxPlayers();i++) {
			if (g_PlayerData[i].ReceiveData.info.dpnidPlayer == playerInfo->dpnidPlayer) {
				int s1 = g_PlayerData[i].ReceiveData.size;
				int s2 = PrePlayerData[i].ReceiveData.size;
				PrePlayerData[i].ReceiveData.size = 0;
				g_PlayerData[i].ReceiveData.size = 0;
				PrePlayerData[i].time2 = g_PlayerData[i].time2;
				PrePlayerData[i].ReceiveData.data[0] = g_PlayerData[i].ReceiveData.data[0];
				PrePlayerData[i].X[0] = g_PlayerData[i].X[0];
				//位置情報だけを更新する
				GCHIPDATA *chip = (GCHIPDATA*)data;
				g_PlayerData[i].ReceiveData.data[0] = *chip;

				g_PlayerData[i].time2 = timeGetTime();
				PrePlayerData[i].ReceiveData.size = s2;
				g_PlayerData[i].ReceiveData.size = s1;
				break;
			}
		}
	}
	else if (g_World->B20Bullet && code == 11) {
		int s = (size - sizeof(short)) / sizeof(GBULLETDATA);
		GBULLETDATA *bullet = (GBULLETDATA*)data;
		for (int j = 0;j < s;j++) {
			GBulletVertex *b = g_Bullet->Add(NULL, bullet[j].Pos, bullet[j].Vec, bullet[j].Power, bullet[j].Size, bullet[j].Dist, bullet[j].Tar, playerInfo->dpnidPlayer);
			b->Net = 0;
		}
	}
	else if (g_World->B26Bullet && code == 31) {
		int i;
		for (i = 0;i < g_DPlay->GetNumPlayers();i++) {
			if (g_PlayerData[i].ReceiveData.info.dpnidPlayer == playerInfo->dpnidPlayer) break;
		}
		//シナリオが同じなら
		if (g_PlayerData[i].scenarioCode == scenarioCode) {
			int s = (size - sizeof(short)) / sizeof(GBULLETDATA);
			GBULLETDATA *bullet = (GBULLETDATA*)data;
			for (int j = 0;j < s;j++) {
				GBulletVertex *b = g_Bullet->Add(NULL, bullet[j].Pos, bullet[j].Vec, bullet[j].Power, bullet[j].Size, bullet[j].Dist, bullet[j].Tar, playerInfo->dpnidPlayer);
				b->Net = 0;
			}
		}
	}
	else if (!g_World->B20Bullet && !g_World->B26Bullet && code == 21) {
		int s = (size - sizeof(short)) / sizeof(GBULLETDATA);
		GBULLETDATA *bullet = (GBULLETDATA*)data;
		for (int j = 0;j < s;j++) {
			GBulletVertex *b = g_Bullet->Add(NULL, bullet[j].Pos, bullet[j].Vec, bullet[j].Power, bullet[j].Size, bullet[j].Dist, bullet[j].Tar, playerInfo->dpnidPlayer);
			b->Net = 0;
		}
	}
	else if (code == 100) {
		for (int i = 0;i < g_DPlay->GetMaxPlayers();i++) {
			if (g_PlayerData[i].ReceiveData.info.dpnidPlayer == playerInfo->dpnidPlayer) {
				GMYDATA *mydata = (GMYDATA*)data;
				g_PlayerData[i].crush = mydata->crush;
				g_PlayerData[i].init = mydata->init;
				g_PlayerData[i].reset = mydata->reset;
				g_PlayerData[i].yforce = mydata->yforce;
				g_PlayerData[i].scenarioCode = mydata->scenarioCode;
				break;
			}
		}
	}
	else if (g_World->B20Bullet && code == 12) {
		int s = (size - sizeof(short)) / sizeof(GEXPDATA);
		GEXPDATA *expo = (GEXPDATA*)data;
		for (int j = 0;j < s;j++) {
			GParticleVertex *part = NULL;
			if (expo[j].Type == 1) {
				part = g_JetParticle->Add(1, expo[j].Pos, GVector(0, 0, 0), GVector(0, 0, 0), (0.3f + (rand() % 50 / 200.0f))*expo[j].Power*0.08f, expo[j].Power, 0.04f, GVector(1, 1, 1), 0);
			}
			else {
				part = g_JetParticle->Add(2, expo[j].Pos, GVector(0, 0, 0), GVector(0, 0, 0), (0.2f + (rand() % 50 / 200.0f))*expo[j].Power*0.08f, expo[j].Power, 0.04f, GVector(1, 1, 1), 0);
			}
			part->Net = 0;
		}
	}
	else if (g_World->B26Bullet && code == 32) { //爆発を受け取った
		int i;
		for (i = 0;i < g_DPlay->GetNumPlayers();i++) {
			if (g_PlayerData[i].ReceiveData.info.dpnidPlayer == playerInfo->dpnidPlayer) break;
		}
		//シナリオが同じなら
		if (g_PlayerData[i].scenarioCode == scenarioCode) {
			int s = (size - sizeof(short)) / sizeof(GEXPDATA2);
			GEXPDATA2 *expo = (GEXPDATA2*)data;
			for (int j = 0;j < s;j++) {
				GParticleVertex *part = NULL;
				if (expo[j].Type == 1) {
					part = g_JetParticle->Add(1, expo[j].Pos, GVector(0, 0, 0), GVector(0, 0, 0), (0.3f + (rand() % 50 / 200.0f))*expo[j].Power*0.08f, expo[j].Power, 0.04f, GVector(1, 1, 1), 0);
					if (expo->dpnid == g_DPlay->GetLocalPlayerDPNID()) AttackDataDisp("O >> Crush ", playerInfo->dpnidPlayer, 0);
				}
				else {
					part = g_JetParticle->Add(2, expo[j].Pos, GVector(0, 0, 0), GVector(0, 0, 0), (0.2f + (rand() % 50 / 200.0f))*expo[j].Power*0.08f, expo[j].Power, 0.04f, GVector(1, 1, 1), 0);
					if (expo->dpnid == g_DPlay->GetLocalPlayerDPNID()) AttackDataDisp("O >> Hit ", playerInfo->dpnidPlayer, 0);
				}
				part->Net = 0;
			}
		}
	}
	else if (!g_World->B20Bullet && !g_World->B26Bullet &&code == 22) { //爆発を受け取った
		int s = (size - sizeof(short)) / sizeof(GEXPDATA2);
		GEXPDATA2 *expo = (GEXPDATA2*)data;
		for (int j = 0;j < s;j++) {
			GParticleVertex *part = NULL;
			if (expo[j].Type == 1) {
				part = g_JetParticle->Add(1, expo[j].Pos, GVector(0, 0, 0), GVector(0, 0, 0), (0.3f + (rand() % 50 / 200.0f))*expo[j].Power*0.08f, expo[j].Power, 0.04f, GVector(1, 1, 1), 0);
				if (expo->dpnid == g_DPlay->GetLocalPlayerDPNID()) AttackDataDisp("O >> Crush ", playerInfo->dpnidPlayer, 0);
			}
			else {
				part = g_JetParticle->Add(2, expo[j].Pos, GVector(0, 0, 0), GVector(0, 0, 0), (0.2f + (rand() % 50 / 200.0f))*expo[j].Power*0.08f, expo[j].Power, 0.04f, GVector(1, 1, 1), 0);
				if (expo->dpnid == g_DPlay->GetLocalPlayerDPNID()) AttackDataDisp("O >> Hit ", playerInfo->dpnidPlayer, 0);
			}
			part->Net = 0;
		}
	}
	else if (code == 62) {
		if (NetworkDlg) {
			GSTREAM strm2;
			DWORD *w = (DWORD*)data;
			w[1] = timeGetTime();

			strm2.code = 63;
			DWORD *sw = (DWORD*)strm2.data;
			sw[0] = w[0];
			sw[1] = w[1];
			DWORD size = sizeof(DWORD) * 4 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	else if (code == 63) {
		if (NetworkDlg) {
			GSTREAM strm2;
			DWORD *w = (DWORD*)data;
			w[2] = timeGetTime();

			strm2.code = 64;
			DWORD *sw = (DWORD*)strm2.data;
			sw[0] = w[0];
			sw[1] = w[1];
			sw[2] = w[2];
			DWORD size = sizeof(DWORD) * 4 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	else if (code == 64) {
		if (NetworkDlg) {
			GSTREAM strm2;
			DWORD *w = (DWORD*)data;
			w[3] = timeGetTime();

			strm2.code = 65;
			DWORD *sw = (DWORD*)strm2.data;
			sw[0] = w[0];
			sw[1] = w[1];
			sw[2] = w[2];
			sw[3] = w[3];
			DWORD size = sizeof(DWORD) * 4 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	else if (code == 65) {
		if (NetworkDlg) {
			char str[256];
			DWORD *w = (DWORD*)data;
			sprintf(str, "ping=%dms", (w[2] - w[0] + w[3] - w[1]) / 2);
			ChatDataDisp(playerInfo->strPlayerName, str);
		}
	}
	else if (code == 50) { //Land
		if (NetworkDlg) {
			GSTREAM strm2;
			strm2.code = 1;
			char *str = (char*)strm2.data;
			sprintf(str, "Land=%s(%X)", szLandFileName0, landCode);
			DWORD size = strlen(str) + 1 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	else if (code == 51) { //Scenario
		if (NetworkDlg) {
			GSTREAM strm2;
			strm2.code = 1;
			char *str = (char*)strm2.data;
			sprintf(str, "Scenario=%s(%X)", szSystemFileName0, scenarioCode);
			DWORD size = strlen(str) + 1 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	else if (code == 52) { //Version
		if (NetworkDlg) {
			GSTREAM strm2;
			strm2.code = 1;
			char *str = (char*)strm2.data;
			sprintf(str, "Version=1.5 B27");
			DWORD size = strlen(str) + 1 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	else if (code == 53) { //Position
		if (NetworkDlg) {
			GSTREAM strm2;
			strm2.code = 1;
			char *str = (char*)strm2.data;
			sprintf(str, "Position=X:%.1f Y:%.1f Z:%.1f ", g_Chip[0]->X.x, g_Chip[0]->X.y, g_Chip[0]->X.z);
			DWORD size = strlen(str) + 1 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	else if (code == 54) { //Name
		if (NetworkDlg) {
			GSTREAM strm2;
			strm2.code = 1;
			char *str = (char*)strm2.data;
			char buf[256];
			g_DPlay->GetPlayersName(g_DPlay->GetLocalPlayerDPNID(), buf);
			sprintf(str, "Name=%s", buf);
			DWORD size = strlen(str) + 1 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	else if (code == 55) { //告知を要求
		if (NetworkDlg) {
			GSTREAM strm2;
			strm2.code = 56;
			char *str = (char*)strm2.data;
			strcpy(str, Kokuti);
			DWORD size = strlen(str) + 1 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	else if (code == 56) { //告知を出す
		if (NetworkDlg) {
			strcpy(Kokuti, (char*)data);
			SendDlgItemMessage(NetworkDlg, IDC_KOKUTI, WM_SETTEXT, 0, (LPARAM)Kokuti);
		}
	}
	else if (code == 57) { //g_FPS
		if (NetworkDlg) {
			GSTREAM strm2;
			strm2.code = 1;
			char *str = (char*)strm2.data;
			if (g_World->NetStop) sprintf(str, "g_FPS=Pause (Base=%d) ", g_LimitFPS);
			else sprintf(str, "g_FPS=%.1f (Base=%d) ", g_FPS, g_LimitFPS);
			DWORD size = strlen(str) + 1 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	else if (code == 58) { //OS
		if (NetworkDlg) {
			GSTREAM strm2;
			strm2.code = 1;
			char *str = (char*)strm2.data;
			OSVERSIONINFO version;
			memset(&version, 0, sizeof(version));
			version.dwOSVersionInfoSize = sizeof(version);
			GetVersionEx(&version);
			sprintf(str, "OS=Major:%d,Minor:%d,Build:%d,ID:%d,CSD:%s",
				version.dwMajorVersion, version.dwMinorVersion, version.dwBuildNumber, version.dwPlatformId, version.szCSDVersion);
			DWORD size = strlen(str) + 1 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	else if (code == 59) { //Chips
		if (NetworkDlg) {
			GSTREAM strm2;
			strm2.code = 1;
			char *str = (char*)strm2.data;
			int i;
			int type[16];
			for (i = 0;i < 16;i++) type[i] = 0;
			int s = 0;
			for (i = 0;i < g_World->getChipCount();i++) {
				if (g_Chip[i]->Parent == NULL) s++;
				if (g_Chip[i]->ChipType < 11) type[g_Chip[i]->ChipType]++;
				else type[g_Chip[i]->ChipType - 33 + 11]++;
			}
			char buf[30];buf[0] = '\0';
			sprintf(str, "Chips=%d/%d(%d", g_World->getChipCount(), s, type[0]);
			for (i = 1;i < 16;i++) {
				if (i == 9) sprintf(buf, ",Cowl=%d", type[i]);
				else if (i == 10) sprintf(buf, ",Arm=%d", type[i]);
				else sprintf(buf, ",%d", type[i]);
				strcat(str, buf);
			}
			strcat(str, ")");
			DWORD size = strlen(str) + 1 + sizeof(short);
			g_DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size);
		}
	}
	delete strm;
	return S_OK;
}
void PlayersInfoDisp() {
	if (NetworkDlg) {

		SendMessage(GetDlgItem(NetworkDlg, IDC_PLAYERLIST), WM_SETREDRAW, FALSE, 0);
		int index = SendMessage(GetDlgItem(NetworkDlg, IDC_PLAYERLIST), LB_GETTOPINDEX, FALSE, 0);
		SendMessage(GetDlgItem(NetworkDlg, IDC_PLAYERLIST), LB_RESETCONTENT, 0, 0);
		SendMessage(GetDlgItem(NetworkDlg, IDC_PLAYERLIST), LB_SETHORIZONTALEXTENT, 500, 0);

		for (int i = 0;i < g_DPlay->GetMaxPlayers();i++) {
			TCHAR szMsg[MAX_PATH] = TEXT("");
			int c1 = ' ';
			int c2 = ' ';
			if (g_PlayerData[i].ReceiveData.info.dpnidPlayer != 0) {
				DWORD size = g_PlayerData[i].ReceiveData.size;
				if (g_PlayerData[i].ReceiveData.info.dpnidPlayer == g_DPlay->GetHostPlayerDPNID()) c1 = ' ' + 1;
				if (g_PlayerData[i].ReceiveData.info.dpnidPlayer == g_DPlay->GetLocalPlayerDPNID()) {
					c1 = c1 + 2;
					if (g_MyPlayerData.haveArm) c2 = c2 + 1;
					sprintf(szMsg, "%02d %06X %c%c%s:C%d/I%d/R%d/Y%d", i, g_PlayerData[i].ReceiveData.info.color, c1, c2, g_PlayerData[i].ReceiveData.info.strPlayerName,
						g_MyPlayerData.crush, g_MyPlayerData.init, g_MyPlayerData.reset, g_MyPlayerData.yforce);
				}
				else {
					if (g_PlayerData[i].haveArm) c2 = c2 + 1;
					sprintf(szMsg, "%02d %06X %c%c%s:C%d/I%d/R%d/Y%d", i, g_PlayerData[i].ReceiveData.info.color, c1, c2, g_PlayerData[i].ReceiveData.info.strPlayerName,
						g_PlayerData[i].crush, g_PlayerData[i].init, g_PlayerData[i].reset, g_PlayerData[i].yforce);
				}
				SendMessage(GetDlgItem(NetworkDlg, IDC_PLAYERLIST), LB_ADDSTRING, 0, (LPARAM)szMsg);
			}
		}
		SendMessage(GetDlgItem(NetworkDlg, IDC_PLAYERLIST), LB_SETTOPINDEX, index, 0);
		SendMessage(GetDlgItem(NetworkDlg, IDC_PLAYERLIST), WM_SETREDRAW, TRUE, 0);
	}
}
HRESULT MyCreateFunc(MYAPP_PLAYER_INFO* playerInfo) {
	HRESULT hr = S_OK;

	DPNID dpnid[GPLAYERMAX];
	int n = g_DPlay->GetPlayersDPID(dpnid);
	for (int i = 0;i < g_DPlay->GetMaxPlayers();i++) {
		g_PlayerData[i].ReceiveData.size = 0;
		if (i < n) {
			if (playerInfo->dpnidPlayer == dpnid[i]) {
				g_PlayerData[i].crush = 0;
				g_PlayerData[i].init = 0;
				g_PlayerData[i].reset = 0;
				g_PlayerData[i].yforce = 0;
			}
			g_PlayerData[i].ReceiveData.info.dpnidPlayer = dpnid[i];
			g_DPlay->GetPlayersName(dpnid[i], g_PlayerData[i].ReceiveData.info.strPlayerName);
			TCHAR* strLastSharp = _tcsrchr(g_PlayerData[i].ReceiveData.info.strPlayerName, TEXT('#'));
			if (strLastSharp == NULL) {
				g_PlayerData[i].ReceiveData.info.color = 0xffffff;
			}
			else {
				sscanf(strLastSharp + 1, "%x", &g_PlayerData[i].ReceiveData.info.color);
				*strLastSharp = '\0';
			}
		}
		else {
			g_PlayerData[i].ReceiveData.info.dpnidPlayer = 0;
			g_PlayerData[i].ReceiveData.info.strPlayerName[0] = '0';
		}
	}
	return S_OK;
}
HRESULT MyDestroyFunc(MYAPP_PLAYER_INFO* playerInfo) {
	HRESULT hr = S_OK;
	DPNID dpnid[GPLAYERMAX];
	int n = g_DPlay->GetPlayersDPID(dpnid);
	if (NetworkDlg) 	SendMessage(GetDlgItem(NetworkDlg, IDC_PLAYERLIST), LB_RESETCONTENT, 0, 0);
	for (int i = 0;i < g_DPlay->GetMaxPlayers();i++) {
		g_PlayerData[i].ReceiveData.size = 0;
		if (i < n) {
			g_PlayerData[i].ReceiveData.info.dpnidPlayer = dpnid[i];
			g_DPlay->GetPlayersName(dpnid[i], g_PlayerData[i].ReceiveData.info.strPlayerName);
			TCHAR* strLastSharp = _tcsrchr(g_PlayerData[i].ReceiveData.info.strPlayerName, TEXT('#'));
			if (strLastSharp == NULL) {
				g_PlayerData[i].ReceiveData.info.color = 0xffffff;
			}
			else {
				sscanf(strLastSharp + 1, "%x", &g_PlayerData[i].ReceiveData.info.color);
				*strLastSharp = '\0';
			}
		}
		else {
			g_PlayerData[i].ReceiveData.info.dpnidPlayer = 0;
			g_PlayerData[i].ReceiveData.info.strPlayerName[0] = '0';
		}
	}
	return S_OK;
}
HRESULT MyTerminateFunc() {
	HRESULT hr = S_OK;
	for (int i = 0;i < g_DPlay->GetMaxPlayers();i++) {
		g_PlayerData[i].ReceiveData.info.dpnidPlayer = 0;
		g_PlayerData[i].ReceiveData.size = 0;
	}
	return S_OK;
}
void BlockErrStr(int errCode, int dataCheck, char *str) {
	if (dataCheck != 0) sprintf(str, "Error at line %d.  ", dataCheck);
	else str[0] = '\0';
	switch (errCode) {
	case 1:strcat(str, "The file doesn't open it.");break;
	case 2:strcat(str, "'{' is necessary.");break;
	case 3:strcat(str, "There are a lot of variables.");break;
	case 4:strcat(str, "'}' is necessary.");break;
	case 5:strcat(str, "'(' is necessary.");break;
	case 6:strcat(str, "It is a key-word doesn't know.");break;
	case 7:strcat(str, "There is no 'VAL' block.");break;
	case 8:strcat(str, "':' is necessary");break;
	case 9:strcat(str, "The variable doesn't exist.");break;
	case 10:strcat(str, "There is no 'KEY' block.");break;
	case 11:strcat(str, "It is a key-word doesn't know.");break;
	case 12:strcat(str, "There is no 'BODY' block.");break;
	case 100:strcat(str, "':' is necessary.");break;
	case 101:strcat(str, "");break;
	case 102:strcat(str, "The chip-type is necessary.");break;
	case 103:strcat(str, "'(' is necessary.");break;
	case 104:strcat(str, "The caul cannot be connected with the caul.");break;
	case 105:strcat(str, "'=' is necessary.");break;
	case 106:strcat(str, "The numerical value or the variable is necessary.");break;
	case 107:strcat(str, "It is a key-word doesn't know.");break;
	case 108:strcat(str, "')' is necessary.");break;
	default:strcat(str, "Syntax error.");
	}
	strcat(str, "     ");
}
void CopyChip(GRigid *rd[], GRigid *rs[])
{
	for (int i = 0;i < g_World->ChipCount;i++) {
		if (rd[i] == NULL) rd[i] = new GRigid(0, false, 0, 0, 0);
		*rd[i] = *rs[i];
		rd[i]->Top = rd[i];
	}
	for (int i = 0;i < g_World->ChipCount;i++) {
		//接触はすべてクリアする
		rs[i]->HitN = 0;rd[i]->HitN = 0;
		rs[i]->TotalHitCount = 0;rd[i]->TotalHitCount = 0;
		if (rs[i]->Top) rd[i]->Top = rd[rs[i]->Top->ID];else rd[i]->Top = NULL;
		if (rs[i]->CowlTop) rd[i]->CowlTop = rd[rs[i]->CowlTop->ID];else rd[i]->CowlTop = NULL;
		if (rs[i]->Parent) rd[i]->Parent = rd[rs[i]->Parent->ID];else rd[i]->Parent = NULL;
		for (int j = 0;j < rs[i]->ChildCount;j++) {
			rs[i]->Child[j].PreDestroyRatio = 0;
			rd[i]->Child[j] = rs[i]->Child[j];
		}
		for (int j = 0;j < rs[i]->ChildCount;j++) {
			if (rs[i]->Child[j].RigidB) rd[i]->Child[j].RigidB = rd[rs[i]->Child[j].RigidB->ID];
			if (rs[i]->Child[j].RigidB2) rd[i]->Child[j].RigidB2 = rd[rs[i]->Child[j].RigidB2->ID];
		}
		if (rs[i]->Parent && rs[i]->LinkInfo) rd[i]->LinkInfo = &(rd[i]->Parent->Child[rs[i]->LinkInfo->ID]);
		else rd[i]->LinkInfo = rs[i]->LinkInfo;
	}
}
void CopyObject(GRigid *rd[], GRigid *rs[])
{
	for (int i = 0;i < g_World->ObjectCount;i++) {
		if (rd[i] == NULL) rd[i] = new GRigid(0, false, 0, 0, 0);
		*rd[i] = *rs[i];
		rd[i]->Top = rd[i];
	}

}
char *SearchFolder(char *path, char *filename, char *result)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char work[_MAX_PATH];
	char work2[_MAX_PATH];
	lstrcpy(work, path);
	lstrcat(work, TEXT("\\"));
	lstrcat(work, filename);
	//最初にファイルを探す
	hFind = FindFirstFile(work, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		//無い場合はサブフォルダを探す
		lstrcpy(work, path);
		strcat(work, "\\*");
		hFind = FindFirstFile(work, &FindFileData);
		do {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {//フォルダ
				if (strcmp(FindFileData.cFileName, ".") != 0
					&& strcmp(FindFileData.cFileName, "..") != 0) {
					lstrcpy(work2, path);
					lstrcat(work2, TEXT("\\"));
					lstrcat(work2, FindFileData.cFileName);
					char *s = SearchFolder(work2, filename, result);
					if (s != NULL) {
						return s;
					}
				}
			}
		} while (FindNextFile(hFind, &FindFileData)); //次のファイルを検索

		FindClose(hFind);
		result[0] = '\0';
		return NULL;
	}
	FindClose(hFind);
	lstrcpy(result, work);
	return result;
}
void StopChip()
{
	for (int i = 0;i < g_World->ChipCount;i++) {
		g_World->Rigid[i]->P = GVector(0, 0, 0);		//並進運動量
		g_World->Rigid[i]->L = GVector(0, 0, 0);		//角運動量

		g_World->Rigid[i]->P2 = GVector(0, 0, 0);		//並進運動量
		g_World->Rigid[i]->L2 = GVector(0, 0, 0);		//角運動量

		g_World->Rigid[i]->V = GVector(0, 0, 0);		//速度
		g_World->Rigid[i]->preV = GVector(0, 0, 0);		//速度
		g_World->Rigid[i]->W = GVector(0, 0, 0);		//角速度
	}
}

void ResetVal()
{
	for (int i = 0;i < g_VarCount;i++) {
		g_ValList[i].Val = g_ValList[i].Def;
		for (int j = 0;j < g_ValList[i].RefCount;j++) {
			if (g_ValList[i].Flag[j])
				*(g_ValList[i].Ref[j]) = -g_ValList[i].Val;
			else *(g_ValList[i].Ref[j]) = g_ValList[i].Val;
		}
	}
	g_Chip[0]->CalcTotalCenter();

	g_World->Land->List3Reset();
	for (int j = 0;j < g_World->ChipCount;j++) {
		if (g_World->Rigid[j]->Parent == NULL) {
			g_World->Land->List3up(g_World->Rigid[j]->TotalCenter, g_World->Rigid[j]->TotalRadius + 5.0f / g_World->StepTime);
		}
	}
	for (int j = 0;j < g_World->ObjectCount;j++) {
		if (g_World->Object[j]->Parent == NULL) {
			g_World->Land->List3up(g_World->Object[j]->TotalCenter, g_World->Object[j]->TotalRadius + 5.0f / g_World->StepTime);
		}
	}
	g_World->Land->List2Reset();
	for (j = 0;j < g_World->ChipCount;j++) {
		if (g_World->Rigid[j]->Parent == NULL) {
			g_World->Land->List2up(g_World->Rigid[j]->TotalCenter, g_World->Rigid[j]->TotalRadius + 5.0f);
		}
	}
	for (j = 0;j < g_World->ObjectCount;j++) {
		if (g_World->Rigid[j]->Parent == NULL) {
			g_World->Land->List2up(g_World->Object[j]->TotalCenter, g_World->Object[j]->TotalRadius + 5.0f);
		}
	}

}
void ResetRecVal()
{
	g_TickCount = RecTickCount;
	for (int i = 0;i < g_VarCount;i++) {
		g_ValList[i].Val = g_ValList[i].RecVal;
		for (int j = 0;j < g_ValList[i].RefCount;j++) {
			g_ValList[i].Updated = g_ValList[i].RecUpdated;
			if (g_ValList[i].Flag[j])
				*(g_ValList[i].Ref[j]) = -g_ValList[i].Val;
			else *(g_ValList[i].Ref[j]) = g_ValList[i].Val;
		}
	}
	g_Chip[0]->CalcTotalCenter();

	g_World->Land->List3Reset();
	for (int j = 0;j < g_World->ChipCount;j++) {
		if (g_World->Rigid[j]->Parent == NULL) {
			g_World->Land->List3up(g_World->Rigid[j]->TotalCenter, g_World->Rigid[j]->TotalRadius + 5.0f / g_World->StepTime);
		}
	}
	for (int j = 0;j < g_World->ObjectCount;j++) {
		if (g_World->Object[j]->Parent == NULL) {
			g_World->Land->List3up(g_World->Object[j]->TotalCenter, g_World->Object[j]->TotalRadius + 5.0f / g_World->StepTime);
		}
	}
	g_World->Land->List2Reset();
	for (j = 0;j < g_World->ChipCount;j++) {
		if (g_World->Rigid[j]->Parent == NULL) {
			g_World->Land->List2up(g_World->Rigid[j]->TotalCenter, g_World->Rigid[j]->TotalRadius + 5.0f);
		}
	}
	for (j = 0;j < g_World->ObjectCount;j++) {
		if (g_World->Rigid[j]->Parent == NULL) {
			g_World->Land->List2up(g_World->Object[j]->TotalCenter, g_World->Object[j]->TotalRadius + 5.0f);
		}
	}

}
void RecVal()
{
	RecTickCount = g_TickCount;
	for (int i = 0;i < g_VarCount;i++) {
		g_ValList[i].RecUpdated = g_ValList[i].Updated;
		g_ValList[i].RecVal = g_ValList[i].Val;
	}
}
void ResetChip(int n, GFloat a)
{
	g_TickCount = 0;
	CCDZoom = 0.3f*180.0f / (GFloat)M_PI;
	g_Chip[n]->Power = 0.0;
	g_Chip[n]->PowerSave = 0.0;
	g_Chip[n]->Power2 = 0.0;
	g_Chip[n]->PowerByFuel = 0;
	for (int i = 0;i < g_VarCount;i++) {
		g_ValList[i].Val = g_ValList[i].Def;
		for (int j = 0;j < g_ValList[i].RefCount;j++) {
			if (g_ValList[i].Flag[j])
				*(g_ValList[i].Ref[j]) = -g_ValList[i].Val;
			else *(g_ValList[i].Ref[j]) = g_ValList[i].Val;
		}
	}
	g_Chip[n]->R = GMatrix().rotateY(a);
	g_World->RestoreLink(g_Chip[n], g_Chip[n]);

	g_Chip[n]->CalcTotalCenter();

	g_Chip[n]->X = GVector(g_Chip[n]->X.x, g_Chip[n]->X.y + g_Chip[n]->TotalRadius + 0.5f, g_Chip[n]->X.z);
	g_Chip[n]->R = GMatrix().rotateY(a);
	g_World->RestoreLink(g_Chip[n], g_Chip[n]);
	g_World->MainStepCount = 0;
	ResetCount = 90;
	g_Chip[n]->Energy = 0;
	g_Chip[n]->Tolerant = 10000;
	g_Chip[n]->Bias.clear();
	g_Chip[n]->TopBias.clear();
	g_Chip[n]->HitN = 0;
	g_Chip[n]->HitObj = 0;
	g_Chip[n]->HitChip = 0;
	g_Chip[n]->HitLand = 0;
	g_Chip[n]->HitBullet = 0;
	g_Chip[n]->CalcTotalFuel();
	g_World->DestroyFlag = false;
}
void ResetObject(int n, GFloat a)
{
	if (g_World->Object[n] == NULL) return;
	g_TickCount = 0;
	g_World->Object[n]->R = GMatrix().rotateY(a);
	g_World->Object[n]->L.clear();
	g_World->Object[n]->P.clear();
	g_World->Object[n]->CalcTotalCenter();
	//	g_World->Object[n]->X=GVector(g_Chip[n]->X.x,g_Chip[n]->X.y+g_Chip[n]->TotalRadius+0.5f,g_Chip[n]->X.z);
	g_World->Object[n]->R = GMatrix().rotateY(a);
}

void ResetChip2(int n, GFloat a)
{
	g_TickCount = 0;
	g_Chip[n]->Power = 0.0;
	for (int i = 0;i < g_VarCount;i++) {
		g_ValList[i].Val = g_ValList[i].Def;
		for (int j = 0;j < g_ValList[i].RefCount;j++) {
			if (g_ValList[i].Flag[j])
				*(g_ValList[i].Ref[j]) = -g_ValList[i].Val;
			else *(g_ValList[i].Ref[j]) = g_ValList[i].Val;
		}
	}
	g_Chip[n]->R = GMatrix().rotateY(a);
	g_World->RestoreLink(g_Chip[n], g_Chip[n]);
	g_World->MainStepCount = 0;
	ResetCount = 90;
	g_World->DestroyFlag = false;
}
void ResetChip3(int n, GQuat q, GVector x)
{
	g_TickCount = 0;
	g_Chip[n]->Power = 0.0;
	g_Chip[n]->PowerSave = 0.0;
	g_Chip[n]->Power2 = 0.0;
	g_Chip[n]->PowerByFuel = 0.0;
	g_Chip[n]->Energy = 0;
	g_Chip[n]->Tolerant = 10000;
	for (int i = 0;i < g_VarCount;i++) {
		g_ValList[i].Val = g_ValList[i].Def;
		if (g_ValList[i].Val > g_ValList[i].Max) g_ValList[i].Val = g_ValList[i].Max;
		if (g_ValList[i].Val < g_ValList[i].Min) g_ValList[i].Val = g_ValList[i].Min;
		g_ValList[i].Updated = true;
		for (int j = 0;j < g_ValList[i].RefCount;j++) {
			if (g_ValList[i].Flag[j])
				*(g_ValList[i].Ref[j]) = -g_ValList[i].Val;
			else *(g_ValList[i].Ref[j]) = g_ValList[i].Val;
		}
	}
	g_Chip[n]->Reset();
	g_Chip[n]->HitN = 0;
	g_Chip[n]->Q = q;
	g_Chip[n]->R = q.matrix();
	g_Chip[n]->X = x;
	g_World->RestoreLink(g_Chip[n], g_Chip[n]);
	g_Chip[n]->CalcTotalCenter();
	/*
	g_World->Land->List3Reset();
	for(int j=0;j<g_World->ChipCount;j++) {
		if(g_World->Rigid[j]->Parent==NULL) {
			g_World->Land->List3up(g_World->Rigid[j]->TotalCenter,g_World->Rigid[j]->TotalRadius+5.0f/g_World->StepTime);
		}
	}
	for(int j=0;j<g_World->ObjectCount;j++) {
		if(g_World->Object[j]->Parent==NULL) {
			g_World->Land->List3up(g_World->Object[j]->TotalCenter,g_World->Object[j]->TotalRadius+5.0f/g_World->StepTime);
		}
	}
	g_World->Land->List2Reset();
	for(j=0;j<g_World->ChipCount;j++) {
		if(g_World->Rigid[j]->Parent==NULL) {
			g_World->Land->List2up(g_World->Rigid[j]->TotalCenter,g_World->Rigid[j]->TotalRadius+5.0f);
		}
	}
	for(j=0;j<g_World->ObjectCount;j++) {
		if(g_World->Rigid[j]->Parent==NULL) {
			g_World->Land->List2up(g_World->Object[j]->TotalCenter,g_World->Object[j]->TotalRadius+5.0f);
		}
	}
	*/
	g_World->MainStepCount = 0;
	ResetCount = 90;
	g_World->DestroyFlag = false;
}
void Text3D(CD3DFont*font, GVector &pos, GVector &rot, float s, char *str, DWORD col) {
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
	D3DXMATRIX tm;
	D3DXMatrixIdentity(&tm);
	D3DXMatrixTranslation(&tm, pos.x, pos.y, pos.z);
	g_D3DDevice->SetTransform(D3DTS_WORLD, &tm);
	// Establish colors for selected vs. normal menu items
	D3DMATERIAL8 mtrlText;
	float r1 = (col >> 16) / 255.0f;
	float g1 = ((col >> 8) & 0xff) / 255.0f;
	float b1 = (col & 0xff) / 255.0f;
	D3DUtil_InitMaterial(mtrlText, r1, g1, b1, 1.0f);
	g_D3DDevice->SetMaterial(&mtrlText);
	font->Render3DText(str, D3DFONT_CENTERED | D3DFONT_TWOSIDED);
}

void Text3Dm(CD3DFont*font, D3DXMATRIX &m, char *str, DWORD col) {
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
	g_D3DDevice->SetTransform(D3DTS_WORLD, &m);
	// Establish colors for selected vs. normal menu items
	D3DMATERIAL8 mtrlText;
	float r1 = (col >> 16) / 255.0f;
	float g1 = ((col >> 8) & 0xff) / 255.0f;
	float b1 = (col & 0xff) / 255.0f;
	D3DUtil_InitMaterial(mtrlText, r1, g1, b1, 1.0f);
	g_D3DDevice->SetMaterial(&mtrlText);
	font->Render3DText(str, D3DFONT_CENTERED);
}
void Line2D(GFloat x0, GFloat y0, GFloat x1, GFloat y1, int col)
{
	D3DXMATRIX mat1;
	D3DXMATRIX matV, mat2;
	g_D3DDevice->GetTransform(D3DTS_VIEW, &matV);
	D3DXMatrixInverse(&mat1, NULL, &matV);
	D3DXMatrixScaling(&mat1, 1, 1, 1);
	//	D3DXMatrixMultiply(&mat2,&mat1,&GMatWorld);
	g_D3DDevice->SetTransform(D3DTS_VIEW, &mat1);

	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
	g_D3DDevice->SetStreamSource(0, pPointVB, sizeof(D3DPOINTVERTEX));
	g_D3DDevice->SetVertexShader(D3DFVF_POINTVERTEX);
	g_D3DDevice->SetTexture(0, NULL);
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// カリングモード
	D3DPOINTVERTEX* p2V;
	pPointVB->Lock(0, 0, (BYTE**)&p2V, 0);

	float t;
	if (ViewType == 7) t = (float)tan(CCDZoom*M_PI / 360.0);
	else t = (float)tan(Zoom*M_PI / 360.0);
	float t2 = (t + 1) / t;

	p2V[0].x = (float)x0*t;
	p2V[0].y = (float)y0*t;
	p2V[0].z = (float)1;
	p2V[0].color = col | 0xff000000;
	p2V[1].x = (float)x1*t;
	p2V[1].y = (float)y1*t;
	p2V[1].z = (float)1;
	p2V[1].color = col | 0xff000000;

	pPointVB->Unlock();
	//	g_D3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	g_D3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_AMBIENT, 0x000F0F0F);
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングモード
	g_D3DDevice->SetTransform(D3DTS_VIEW, &GMatView);

}

void Line(GVector &p1, GVector &p2, unsigned int col)
{
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
	g_D3DDevice->SetStreamSource(0, pPointVB, sizeof(D3DPOINTVERTEX));
	g_D3DDevice->SetVertexShader(D3DFVF_POINTVERTEX);
	g_D3DDevice->SetTexture(0, NULL);
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// カリングモード
	D3DPOINTVERTEX* p2V;
	pPointVB->Lock(0, 0, (BYTE**)&p2V, 0);
	p2V[0].x = (float)p1.x;
	p2V[0].y = (float)p1.y;
	p2V[0].z = (float)p1.z;
	p2V[0].color = col | 0xff000000;
	p2V[1].x = (float)p2.x;
	p2V[1].y = (float)p2.y;
	p2V[1].z = (float)p2.z;
	p2V[1].color = col | 0xff000000;

	pPointVB->Unlock();
	g_D3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_AMBIENT, 0x000F0F0F);
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングモード
}
int CALLBACK DlgDataProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char str[1000];
	char *s;
	FILE *fp;
	HFONT font = (HFONT)GetStockObject(SYSTEM_FONT);
	switch (uMsg) {
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hDlg, IDC_EDIT1), WM_SETFONT, (WPARAM)font, 0);
		SendMessage(GetDlgItem(hDlg, IDC_STATIC1), WM_SETFONT, (WPARAM)font, 0);
		//SelectObject(hdc, font);
		ChipDataStrng[0] = '\0';
		sprintf(ChipDataStrng, "%s  Chips=%d (Cowl=%d), Weight=%g", szUpdateFileName0, g_World->Rigid[0]->TotalCount, g_World->Rigid[0]->TotalCowlCount, g_World->Rigid[0]->TotalMass);
		SendDlgItemMessage(hDlg, IDC_STATIC1, WM_SETTEXT, 0, (LPARAM)ChipDataStrng);
		ChipDataStrng[0] = '\0';

		if ((fp = fopen(szUpdateFileName, "r")) != 0) {
			while (1) {
				s = fgets(str, 1000, fp);
				if (s == NULL) break;
				if (str[strlen(str) - 1] < ' ') str[strlen(str) - 1] = '\0';
				strcat(ChipDataStrng, s);
				strcat(ChipDataStrng, "\r\n");
			}
			fclose(fp);
		}
		SendDlgItemMessage(hDlg, IDC_EDIT1, WM_SETTEXT, 0, (LPARAM)ChipDataStrng);
		break;
	case WM_CLOSE:
		if (hDlg)DestroyWindow(hDlg);
		hDlg = NULL;
		SourceDlg = NULL;
		break;
	case WM_DESTROY:
		//            if(hDlg)DestroyWindow(hDlg);
		//            hDlg = NULL;
		//			PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			if (hDlg)DestroyWindow(hDlg);
			hDlg = NULL;
			SourceDlg = NULL;
			break;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}
	return TRUE;
}

int CALLBACK DlgExtraProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int p;
	static int state = 0;
	char str[1000];
	COLORREF custColors[16];
	CHOOSECOLOR cc;
	int r;
	HDC hDC = GetDC(hDlg);
	switch (uMsg) {
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hDlg, IDC_FARSLIDER), TBM_SETRANGE, 0, MAKELPARAM(0, 4));
		SendMessage(GetDlgItem(hDlg, IDC_FARSLIDER), TBM_SETTICFREQ, (WPARAM)1, 0);
		SendMessage(GetDlgItem(hDlg, IDC_FASTSHADOWCHECK), BM_SETCHECK, (WPARAM)FastShadow, 0);
		SendMessage(GetDlgItem(hDlg, IDC_MARKERSLIDER), TBM_SETRANGE, 0, MAKELPARAM(0, 4));
		SendMessage(GetDlgItem(hDlg, IDC_MARKERSLIDER), TBM_SETTICFREQ, (WPARAM)1, 0);
		SendMessage(GetDlgItem(hDlg, IDC_NAMESLIDER), TBM_SETRANGE, 0, MAKELPARAM(0, 4));
		SendMessage(GetDlgItem(hDlg, IDC_NAMESLIDER), TBM_SETTICFREQ, (WPARAM)1, 0);
		SendMessage(GetDlgItem(hDlg, IDC_LUAEXTERNAL), BM_SETCHECK, (WPARAM)UseLuaExternal, 0);

		if (g_FarMax <= 300.0f) p = 0;
		else if (g_FarMax <= 600.0f) p = 1;
		else if (g_FarMax <= 1200.0f) p = 2;
		else if (g_FarMax <= 2400.0f) p = 3;
		else if (g_FarMax <= 4800.0f) p = 4;
		SendMessage(GetDlgItem(hDlg, IDC_FARSLIDER), TBM_SETPOS, 1, p);
		r = (((int)FogColor.z) << 16) + (((int)FogColor.y) << 8) + (int)FogColor.x;
		sprintf(str, "%06X", r);
		SendDlgItemMessage(hDlg, IDC_COLORSTATIC, WM_SETTEXT, 0, (LPARAM)str);

		if (g_MarkerSize <= 0.0f) p = 0;
		else if (g_MarkerSize <= 0.5f) p = 1;
		else if (g_MarkerSize <= 0.75f) p = 2;
		else if (g_MarkerSize <= 1.0f) p = 3;
		else if (g_MarkerSize <= 2.0f) p = 4;
		SendMessage(GetDlgItem(hDlg, IDC_MARKERSLIDER), TBM_SETPOS, 1, p);

		if (g_NameSize <= 0.0f) p = 0;
		else if (g_NameSize <= 0.5f) p = 1;
		else if (g_NameSize <= 0.75f) p = 2;
		else if (g_NameSize <= 1.0f) p = 3;
		else if (g_NameSize <= 2.0f) p = 4;
		SendMessage(GetDlgItem(hDlg, IDC_NAMESLIDER), TBM_SETPOS, 1, p);
		break;
	case WM_CLOSE:
		if (state == 1) break;
		if (hDlg)DestroyWindow(hDlg);
		hDlg = NULL;
		ExtraDlg = NULL;
		break;

	case WM_HSCROLL:
		p = (int)SendMessage(GetDlgItem(hDlg, IDC_FARSLIDER), TBM_GETPOS, 0, 0);
		if (p == 0) g_FarMax = 300.0f;
		else if (p == 1) g_FarMax = 600.0f;
		else if (p == 2) g_FarMax = 1200.0f;
		else if (p == 3) g_FarMax = 2400.0f;
		else  g_FarMax = 4800.0f;

		p = (int)SendMessage(GetDlgItem(hDlg, IDC_MARKERSLIDER), TBM_GETPOS, 0, 0);
		if (p == 0) g_MarkerSize = 0.0f;
		else if (p == 1) g_MarkerSize = 0.5f;
		else if (p == 2) g_MarkerSize = 0.75f;
		else if (p == 3) g_MarkerSize = 1.0f;
		else  g_MarkerSize = 2.0f;

		p = (int)SendMessage(GetDlgItem(hDlg, IDC_NAMESLIDER), TBM_GETPOS, 0, 0);
		if (p == 0) g_NameSize = 0.0f;
		else if (p == 1) g_NameSize = 0.5f;
		else if (p == 2) g_NameSize = 0.75f;
		else if (p == 3) g_NameSize = 1.0f;
		else  g_NameSize = 2.0f;
		break;
	case WM_DESTROY:
		//            if(hDlg)DestroyWindow(hDlg);
		//            hDlg = NULL;
		//			PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_FOGBUTTON | (BN_CLICKED << 16) :
			state = 1;
			ZeroMemory(custColors, sizeof(custColors));
			custColors[0] = 0x00ffe6c8;
			r = (((int)FogColor.z) << 16) + (((int)FogColor.y) << 8) + (int)FogColor.x;
			custColors[1] = r;
			ZeroMemory(&cc, sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.Flags = CC_ANYCOLOR;
			cc.hwndOwner = hDlg;
			cc.lpCustColors = custColors;
			cc.rgbResult = r;
			r = ChooseColor(&cc);
			FogColor.z = (float)((cc.rgbResult >> 16) & 0xff);
			FogColor.y = (float)((cc.rgbResult >> 8) & 0xff);
			FogColor.x = (float)(cc.rgbResult & 0xff);
			r = (((int)FogColor.z) << 16) + (((int)FogColor.y) << 8) + (int)FogColor.x;
			sprintf(str, "%06X", r);
			SendDlgItemMessage(hDlg, IDC_COLORSTATIC, WM_SETTEXT, 0, (LPARAM)str);
			state = 0;
			break;
		case IDC_FASTSHADOWCHECK | (BN_CLICKED << 16) :
		{
			int st = (int)SendMessage(GetDlgItem(hDlg, IDC_FASTSHADOWCHECK), BM_GETCHECK, 0, 0);
			if (st&BST_CHECKED) FastShadow = 1;
			else FastShadow = 0;
		}
													  break;
		case (IDC_LUAEXTERNAL | (BN_CLICKED << 16)) :
			if (((int)SendMessage(GetDlgItem(hDlg, IDC_LUAEXTERNAL), BM_GETCHECK, 0, 0)) & BST_CHECKED)
				UseLuaExternal = 1;
			else
				UseLuaExternal = 0;
			break;
		case IDOK:
			if (state == 1) break;
			if (hDlg)DestroyWindow(hDlg);
			hDlg = NULL;
			ExtraDlg = NULL;
			break;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}
	ReleaseDC(hDlg, hDC);
	return TRUE;
}

int CALLBACK DlgNetworkProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int state = 0;
	static HBRUSH DefaultBackBrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	HFONT font = (HFONT)GetStockObject(SYSTEM_FONT);
	SendMessage(GetDlgItem(hDlg, IDC_EDIT1), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_STATIC1), WM_SETFONT, (WPARAM)font, 0);
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendMessage(GetDlgItem(hDlg, IDC_SESSIONEDIT), WM_SETFONT, (WPARAM)font, 0);
		SendMessage(GetDlgItem(hDlg, IDC_USEREDIT), WM_SETFONT, (WPARAM)font, 0);
		SendMessage(GetDlgItem(hDlg, IDC_HOSTEDIT), WM_SETFONT, (WPARAM)font, 0);
		SendMessage(GetDlgItem(hDlg, IDC_PORTEDIT), WM_SETFONT, (WPARAM)font, 0);
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERLIST), WM_SETFONT, (WPARAM)font, 0);
		SendMessage(GetDlgItem(hDlg, IDC_KOKUTI), WM_SETFONT, (WPARAM)font, 0);
		SendMessage(GetDlgItem(hDlg, IDC_CHATTEXT), WM_SETFONT, (WPARAM)font, 0);
		SendMessage(GetDlgItem(hDlg, IDC_CHATEDIT), WM_SETFONT, (WPARAM)font, 0);
		SendMessage(GetDlgItem(hDlg, IDC_ATTACKTEXT), WM_SETFONT, (WPARAM)font, 0);
		char str[256], str2[256];
		int n = g_DPlay->GetNumPlayers();
		int st = (int)SendMessage(GetDlgItem(hDlg, IDC_HOSTRADIO), BM_GETCHECK, 0, 0);
		int st2 = (int)SendMessage(GetDlgItem(hDlg, IDC_CONNECTRADIO), BM_GETCHECK, 0, 0);
		if ((st&BST_CHECKED) == 0 && (st2&BST_CHECKED) == 0) {
			SendMessage(GetDlgItem(hDlg, IDC_HOSTRADIO), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			st = BST_CHECKED;
		}

		str[0] = '\0';
		if (st&BST_CHECKED) strcpy(str, SessionName);
		else if (g_DPlay->GetConnect()) g_DPlay->GetSessionName(str);
		else str[0] = '\0';

		if (str[0] != 0) SendDlgItemMessage(hDlg, IDC_SESSIONEDIT, WM_SETTEXT, 0, (LPARAM)str);
		if (g_DPlay->GetLocalPlayerDPNID() != 0) g_DPlay->GetPlayersName(g_DPlay->GetLocalPlayerDPNID(), PlayerName);
		SendDlgItemMessage(hDlg, IDC_USEREDIT, WM_SETTEXT, 0, (LPARAM)PlayerName);
		sprintf(str, "%d", PortNo);
		SendDlgItemMessage(hDlg, IDC_PORTEDIT, WM_SETTEXT, 0, (LPARAM)str);
		str[0] = '\0';
		if ((st&BST_CHECKED) == 0) strcpy(str, HostName);
		else if (n != 0) g_DPlay->GetHostName(str, str2);
		else str[0] = '\0';
		SendDlgItemMessage(hDlg, IDC_HOSTEDIT, WM_SETTEXT, 0, (LPARAM)str);
		if (g_DPlay->GetLocalPlayerDPNID() == 0) {
			sprintf(str, "Push 'Start' to Hosting or Connecting");
			//					SendMessage(GetDlgItem(hDlg,IDC_HOSTRADIO), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			//					SendMessage(GetDlgItem(hDlg,IDC_CONNECTRADIO), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, IDC_STARTBUTTON, WM_SETTEXT, 0, (LPARAM)"Start");
			EnableWindow(GetDlgItem(hDlg, IDC_SEND), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_COLORBUTTON), TRUE);
			SendMessage(GetDlgItem(hDlg, IDC_CHATEDIT), WM_SETTEXT, 0, (LPARAM)"");
			SendMessage(GetDlgItem(hDlg, IDC_CHATEDIT), EM_SETREADONLY, 1, 0);
			if (st&BST_CHECKED) {
				SendMessage(GetDlgItem(hDlg, IDC_HOSTEDIT), EM_SETREADONLY, 1, 0);
				SendMessage(GetDlgItem(hDlg, IDC_SESSIONEDIT), EM_SETREADONLY, 0, 0);
			}
			else {
				SendMessage(GetDlgItem(hDlg, IDC_HOSTEDIT), EM_SETREADONLY, 0, 0);
				SendMessage(GetDlgItem(hDlg, IDC_SESSIONEDIT), EM_SETREADONLY, 1, 0);
			}
		}
		if (g_DPlay->GetLocalPlayerDPNID() != 0) {
			if (g_DPlay->GetHostPlayerDPNID() == g_DPlay->GetLocalPlayerDPNID()) {
				sprintf(str, "Hosting. %d players in this session.", n);
				SendMessage(GetDlgItem(hDlg, IDC_HOSTRADIO), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
				SendMessage(GetDlgItem(hDlg, IDC_CONNECTRADIO), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			}
			else {
				sprintf(str, "Connecting. %d players in this session.", n);
				SendMessage(GetDlgItem(hDlg, IDC_CONNECTRADIO), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
				SendMessage(GetDlgItem(hDlg, IDC_HOSTRADIO), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			}
			SendDlgItemMessage(hDlg, IDC_STARTBUTTON, WM_SETTEXT, 0, (LPARAM)"End");
			EnableWindow(GetDlgItem(hDlg, IDC_SEND), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_COLORBUTTON), FALSE);
			SendMessage(GetDlgItem(hDlg, IDC_CHATEDIT), EM_SETREADONLY, 0, 0);
		}

		SendDlgItemMessage(hDlg, IDC_STATICMES, WM_SETTEXT, 0, (LPARAM)str);
	}
	break;
	case WM_CLOSE:
		if (state == 1) break;
		if (hDlg)ShowWindow(hDlg, SW_HIDE);
		//           hDlg = NULL;
		break;
	case WM_DRAWITEM:
	{
		//プレイヤリストの表示
		if ((UINT)wParam == IDC_PLAYERLIST) {
			HBRUSH hBrush, hBrushOld;
			RECT rc, rc2;
			LPDRAWITEMSTRUCT lpds = (LPDRAWITEMSTRUCT)lParam;
			CopyRect((LPRECT)&rc, (LPRECT)&lpds->rcItem);
			rc.left += 20;
			rc.right += 20;
			char str[256];
			if (SendMessage(GetDlgItem(NetworkDlg, IDC_PLAYERLIST), LB_GETTEXT, (WPARAM)lpds->itemID, (LPARAM)str) > 1) {
				;
				char *str2 = &str[10];
				char *str3 = &str[12];
				int h = rc.bottom - rc.top;
				//				str[6]='\0';
				BYTE r, g, b;
				r = g = b = 255;
				char c1 = str2[0] - ' ';
				char c2 = str2[1] - ' ';
				if (c1 & 1) {
					if (c1 & 2) g = 128;
					else g = b = 128;
				}
				else if (c1 & 2) r = g = 128;
				HPEN hPen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
				HPEN hPenOld = (HPEN)SelectObject(lpds->hDC, hPen);

				hBrush = CreateSolidBrush(RGB(r, g, b));
				hBrushOld = (HBRUSH)SelectObject(lpds->hDC, hBrush);
				rc2.left = rc.left;rc2.top = rc.top;rc2.right = rc.left + h;rc2.bottom = rc.top + h;
				Ellipse(lpds->hDC, rc.left + 1, rc.top + 1, rc.left + h - 2, rc.top + h - 2);
				SelectObject(lpds->hDC, hBrushOld);
				DeleteObject(hBrush);
				SelectObject(lpds->hDC, hPenOld);
				DeleteObject(hPen);

				DWORD color;
				int no;
				sscanf(str, "%d %X", &no, &color);
				str[2] = '\0';
				hBrush = CreateSolidBrush(RGB(color >> 16, (color >> 8) & 0xff, color & 0xff));
				hBrushOld = (HBRUSH)SelectObject(lpds->hDC, hBrush);
				if (c2 == 0) {
					Ellipse(lpds->hDC, rc.left + 4, rc.top + 4, rc.left + h - 5, rc.top + h - 5);
				}
				else {
					Rectangle(lpds->hDC, rc.left + 4, rc.top + 4, rc.left + h - 5, rc.top + h - 5);
				}
				SelectObject(lpds->hDC, hBrushOld);
				DeleteObject(hBrush);

				//SetBkMode(lpds->hDC, TRANSPARENT); 
				TextOut(lpds->hDC, rc.left - 20, rc.top, str, (DWORD)strlen(str));
				rc.left += h + 6;
				TextOut(lpds->hDC, rc.left, rc.top, str3, (DWORD)strlen(str3));
			}
		}
		return TRUE;
	}

	break;

	case WM_CTLCOLORSTATIC:
	{
		HDC hDC = (HDC)wParam;
		HWND hCtrl = (HWND)lParam;
		SetBkMode(hDC, OPAQUE);		// 背景を塗りつぶし
		if (hCtrl == GetDlgItem(hDlg, IDC_ATTACKTEXT))// エディットウィンドウのID
		{
			SetTextColor(hDC, RGB(0, 255, 0));	// テキストの色
			SetBkColor(hDC, RGB(0, 0, 0));	// テキストが書かれている部分のテキストの背景の色
			return (LRESULT)GetStockObject(BLACK_BRUSH);	// テキストが書かれていない部分の背景の色
		}
		else {
			SetTextColor(hDC, RGB(0, 0, 0));	// テキストの色
			SetBkColor(hDC, GetSysColor(COLOR_BTNFACE));	// テキストが書かれている部分のテキストの背景の色
			return (LRESULT)DefaultBackBrush;
		}
		break;
	}

	case WM_DESTROY:
		//            if(hDlg)DestroyWindow(hDlg);
		//            hDlg = NULL;
		//			PostQuitMessage(0);
		DeleteObject(DefaultBackBrush);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_SEND:
		{
			char str1[1000];
			char str2[MAX_PLAYER_NAME];
			SendDlgItemMessage(hDlg, IDC_CHATEDIT, WM_GETTEXT, 256, (LPARAM)str1);
			if (str1[0] != '\0') {
				GSTREAM stream;
				if (str1[0] == '&') {
					char *strm = &str1[2];
					int id = -1;
					if (isdigit(str1[2])) {
						id = str1[2] - '0';
						strm++;
						if (isdigit(str1[3])) {
							id = id * 10 + (str1[3] - '0');
							strm++;
						}
					}
					if (id >= GPLAYERMAX) id = -1;
					DWORD size = 0;

					if (str1[1] == 'P' || str1[1] == 'p') {
						stream.code = 62;//Ping
						DWORD *w = (DWORD*)stream.data;
						w[0] = timeGetTime();
						size = sizeof(DWORD) * 4 + sizeof(short);
					}
					else if (str1[1] == 'L' || str1[1] == 'l') {
						stream.code = 50;//Land
						char *w = (char*)stream.data;
						sprintf(w, "land");
						size = 5 + sizeof(short);
					}
					else if (str1[1] == 'S' || str1[1] == 's') {
						stream.code = 51;//Scenario
						char *w = (char*)stream.data;
						sprintf(w, "scen");
						size = 5 + sizeof(short);
					}
					else if (str1[1] == 'V' || str1[1] == 'v') {
						stream.code = 52;//Version
						char *w = (char*)stream.data;
						sprintf(w, "vers");
						size = 5 + sizeof(short);
					}
					else if (str1[1] == 'X' || str1[1] == 'x') {
						stream.code = 53;//Position
						char *w = (char*)stream.data;
						sprintf(w, "posi");
						size = 5 + sizeof(short);
					}
					else if (str1[1] == 'N' || str1[1] == 'n') {
						stream.code = 54;//Name
						char *w = (char*)stream.data;
						sprintf(w, "name");
						size = 5 + sizeof(short);
					}
					else if (str1[1] == 'K' || str1[1] == 'k') {
						stream.code = 55;//告知を受け取る
						char *w = (char*)stream.data;
						sprintf(w, "koku");
						size = 5 + sizeof(short);
						id = 0;
					}
					else if (str1[1] == 'J' || str1[1] == 'j') {
						stream.code = 56;//告知を出す
						char *w = (char*)stream.data;
						char name[128];
						g_DPlay->GetPlayersName(g_DPlay->GetLocalPlayerDPNID(), name);
						TCHAR* strLastSharp = _tcsrchr(name, TEXT('#'));
						if (strLastSharp) *strLastSharp = '\0';
						sprintf(w, "%s:%s", name, strm);
						strcpy(Kokuti, w);
						size = strlen(w) + 1 + sizeof(short);
						SendDlgItemMessage(NetworkDlg, IDC_KOKUTI, WM_SETTEXT, 0, (LPARAM)Kokuti);
						id = -1;
					}
					else if (str1[1] == 'F' || str1[1] == 'f') {
						stream.code = 57;//g_FPS
						char *w = (char*)stream.data;
						sprintf(w, "fps ");
						size = 5 + sizeof(short);
					}
					else if (str1[1] == 'O' || str1[1] == 'o') {
						stream.code = 58;//OS
						char *w = (char*)stream.data;
						sprintf(w, "Os  ");
						size = 5 + sizeof(short);
					}
					else if (str1[1] == 'C' || str1[1] == 'c') {
						stream.code = 59;//Chips
						char *w = (char*)stream.data;
						sprintf(w, "chip");
						size = 5 + sizeof(short);
					}
					if (id >= 0) {
						if (g_PlayerData[id].ReceiveData.info.dpnidPlayer != g_DPlay->GetLocalPlayerDPNID()) {
							g_DPlay->SendTo(g_PlayerData[id].ReceiveData.info.dpnidPlayer, (BYTE*)&stream, (DWORD)size);
						}
					}
					else g_DPlay->SendAll((BYTE*)&stream, (DWORD)size);
					SendDlgItemMessage(hDlg, IDC_CHATEDIT, WM_SETTEXT, 0, (LPARAM)"");
				}
				else {
					stream.code = 1;//chat
					strcpy((char*)stream.data, str1);
					DWORD size = (DWORD)(strlen(str1) + 1 + sizeof(short));
					g_DPlay->SendAll((BYTE*)&stream, size);
					g_DPlay->GetPlayersName(g_DPlay->GetLocalPlayerDPNID(), str2);
					char *ret = ChatDataDisp(str2, (char*)str1);
					strcpy(LastChatData, ret);
					if (g_SystemLua != NULL && g_World->B26Bullet) luaSystemRun("OnChat");
					SendDlgItemMessage(hDlg, IDC_CHATEDIT, WM_SETTEXT, 0, (LPARAM)"");
				}
			}
			SetFocus(GetDlgItem(hDlg, IDC_CHATEDIT));
		}
		break;
		case IDC_COLORBUTTON | (BN_CLICKED << 16) :
		{
			SendDlgItemMessage(hDlg, IDC_USEREDIT, WM_GETTEXT, MAX_PLAYER_NAME, (LPARAM)PlayerName);
			TCHAR* strLastSharp = _tcsrchr(PlayerName, TEXT('#'));
			DWORD color = 0xffffff;
			if (strLastSharp) {
				sscanf(strLastSharp + 1, "%x", &color);
				*strLastSharp = '\0';
			}
			COLORREF custColors[16];
			CHOOSECOLOR cc;
			int r;
			ZeroMemory(custColors, sizeof(custColors));
			r = ((color >> 16) & 0xff) + (((color >> 8) & 0xff) << 8) + ((color & 0xff) << 16);
			custColors[0] = r;
			ZeroMemory(&cc, sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.Flags = CC_ANYCOLOR;
			cc.hwndOwner = hDlg;
			cc.lpCustColors = custColors;
			cc.rgbResult = r;
			r = ChooseColor(&cc);
			color = ((cc.rgbResult >> 16) & 0xff) + (cc.rgbResult & 0xff00) + ((cc.rgbResult & 0xff) << 16);
			sprintf(PlayerName, "%s#%06X", PlayerName, color);
			SendDlgItemMessage(hDlg, IDC_USEREDIT, WM_SETTEXT, 0, (LPARAM)PlayerName);
		}
												  break;
		case IDC_RETURNMAIN:
			SetFocus(GetDlgItem(hDlg, IDC_CHATEDIT));
			SetFocus(g_hWnd);
			break;

			/*
							case IDM_LOBBY_REGISTER:
								if( SUCCEEDED(  RegisterProgramWithLobby(g_guidApp,g_DPlay->GetLobbyApp()) ) ) {

									LobbyRegister(g_guidApp); //DirectX7用
									MessageBox( hDlg, TEXT("Successfully registered lobby support."),
												TEXT("RigidChips Lobby"), MB_OK );

								}
								break;

							case IDM_LOBBY_UNREGISTER:
								if( SUCCEEDED( UnRegisterProgramWithLobby(g_guidApp,g_DPlay->GetLobbyApp()) ) ){
									LobbyUnregister(g_guidApp); //DirectX7用
									MessageBox( hDlg, TEXT("Application lobby information removed."),
												TEXT("RigidChips Lobby"), MB_OK );
								}
								break;
			*/
		case IDC_HOSTRADIO | (BN_CLICKED << 16) :
		case IDC_CONNECTRADIO | (BN_CLICKED << 16) :
		{
			SendMessage(hDlg, WM_INITDIALOG, 0, 0);
		}
												break;
		case IDC_PAUSECHECK | (BN_CLICKED << 16) :
		{
			int st = (int)SendMessage(GetDlgItem(hDlg, IDC_PAUSECHECK), BM_GETCHECK, 0, 0);
			if (st&BST_CHECKED) g_World->NetStop = true;
			else g_World->NetStop = false;
		}
												 break;
		case IDC_B20CHECK | (BN_CLICKED << 16) :
		{
			int st = (int)SendMessage(GetDlgItem(hDlg, IDC_B20CHECK), BM_GETCHECK, 0, 0);
			if (st&BST_CHECKED) g_World->B26Bullet = true;
			else g_World->B26Bullet = false;
		}
											   break;
		case IDC_STARTBUTTON:
		{
			if (g_DPlay->GetCancelConnect()) {
				g_DPlay->CancelConnect();
				SendMessage(hDlg, WM_INITDIALOG, 0, 0);
				break;
			}

			int n = g_DPlay->GetNumPlayers();
			g_MyPlayerData.ver_team = (15220 << 16) + 0;
			g_MyPlayerData.base_fps = (g_LimitFPS << 16) + 30;
			g_MyPlayerData.scenarioCode = scenarioCode;
			g_MyPlayerData.dummyf1 = 0;
			g_MyPlayerData.dummyf2 = 0;
			g_MyPlayerData.dummyf3 = 0;
			g_MyPlayerData.init = 0;
			g_MyPlayerData.reset = 0;
			g_MyPlayerData.yforce = 0;
			g_MyPlayerData.crush = 0;
			SendDlgItemMessage(hDlg, IDC_STATICMES, WM_SETTEXT, 0, (LPARAM)"Wait... ");
			if (n != 0) {
				g_pApp->Pause(TRUE);
				g_DPlay->Close();
				g_pApp->Pause(FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_HOSTRADIO), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDC_CONNECTRADIO), TRUE);
				SendMessage(GetDlgItem(hDlg, IDC_SESSIONEDIT), EM_SETREADONLY, 0, 0);
				SendMessage(GetDlgItem(hDlg, IDC_USEREDIT), EM_SETREADONLY, 0, 0);
				SendMessage(GetDlgItem(hDlg, IDC_HOSTEDIT), EM_SETREADONLY, 0, 0);
				SendMessage(GetDlgItem(hDlg, IDC_PORTEDIT), EM_SETREADONLY, 0, 0);
				SendMessage(hDlg, WM_INITDIALOG, 0, 0);
				break;
			}
			g_DPlay->SetReceiveFunc(MyReceiveFunc);
			g_DPlay->SetDestroyFunc(MyDestroyFunc);
			g_DPlay->SetCreateFunc(MyCreateFunc);
			g_DPlay->SetTerminateFunc(MyTerminateFunc);
			int st = (int)SendMessage(GetDlgItem(hDlg, IDC_HOSTRADIO), BM_GETCHECK, 0, 0);

			SendDlgItemMessage(hDlg, IDC_STARTBUTTON, WM_SETTEXT, 0, (LPARAM)"...");
			if (st&BST_CHECKED) {
				char str1[256];
				SendDlgItemMessage(hDlg, IDC_PORTEDIT, WM_GETTEXT, 256, (LPARAM)str1);
				sscanf(str1, "%d", &PortNo);
				SendDlgItemMessage(hDlg, IDC_SESSIONEDIT, WM_GETTEXT, 256, (LPARAM)SessionName);
				SendDlgItemMessage(hDlg, IDC_USEREDIT, WM_GETTEXT, 256, (LPARAM)PlayerName);
				g_DPlay->NewSession(SessionName, PlayerName, PortNo, g_guidApp);
				//							g_DPlay->GetHostName(str1,str2);
			}
			else {
				SendDlgItemMessage(hDlg, IDC_STATICMES, WM_SETTEXT, 0, (LPARAM)"Wait...('Esc' to Cancel) ");
				char str1[256];
				SendDlgItemMessage(hDlg, IDC_PORTEDIT, WM_GETTEXT, 256, (LPARAM)str1);
				sscanf(str1, "%d", &PortNo);
				SendDlgItemMessage(hDlg, IDC_HOSTEDIT, WM_GETTEXT, 256, (LPARAM)HostName);
				SendDlgItemMessage(hDlg, IDC_USEREDIT, WM_GETTEXT, 256, (LPARAM)PlayerName);
				g_DPlay->ConnectSession(HostName, PlayerName, PortNo, g_guidApp);
				//							g_DPlay->GetSessionName(str1);
			}
			SendMessage(hDlg, WM_INITDIALOG, 0, 0);
		}
		break;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

void GRigid::AddViewModel()
{
}
void GRigid::Disp()
{
	if (MeshNo == 23 && !ShowCowl) return;
	if (Ghost >= 1 && !ShowGhost) return;
	CD3DMesh* mesh = NULL;
	GMatrix m(TM);
	D3DXMATRIX mat1, mat2;
	D3DXMATRIX matLocal = D3DXMATRIX((FLOAT)m.elem[0][0], (FLOAT)m.elem[0][1], (FLOAT)m.elem[0][2], (FLOAT)m.elem[0][3],
		(FLOAT)m.elem[1][0], (FLOAT)m.elem[1][1], (FLOAT)m.elem[1][2], (FLOAT)m.elem[1][3],
		(FLOAT)m.elem[2][0], (FLOAT)m.elem[2][1], (FLOAT)m.elem[2][2], (FLOAT)m.elem[2][3],
		(FLOAT)m.elem[3][0], (FLOAT)m.elem[3][1], (FLOAT)m.elem[3][2], (FLOAT)m.elem[3][3]);
	D3DXMatrixIdentity(&mat1);
	if (MeshNo<0) {
		D3DXMatrixTranslation(&mat1, 0.0f, -0.01f, 0.0f);
		mesh = g_pLandMesh;
	}
	else {
		mesh = m_pXMesh[MeshNo];
		if (MeshNo == 2) {
			if (LinkInfo && (W*(LinkInfo->Axis*R)).abs()>M_PI / g_World->StepTime / 3.0f) mesh = m_pXMesh[8];
		}
		else if (MeshNo == 3) {
			if (LinkInfo && (W*(LinkInfo->Axis*R)).abs() > M_PI / g_World->StepTime / 3.0f) mesh = m_pXMesh[9];
		}
		else if (MeshNo == 23) { //カウル
			if (Option == 1) mesh = m_pXMesh[24];
			else if (Option == 2) mesh = m_pXMesh[25];
			else if (Option == 3) mesh = m_pXMesh[26];
			else if (Option == 4) mesh = m_pXMesh[27];
			else if (Option == 5) mesh = m_pXMesh[28];
		}
		else if (MeshNo == 10 && (Option == 1 || Option == 2)) {
			mesh = m_pXMesh[33];
		}

		switch (Dir) {
		case 1:D3DXMatrixRotationY(&mat1, (FLOAT)M_PI / 2);break;
		case 2:D3DXMatrixRotationY(&mat1, (FLOAT)M_PI);break;
		case 3:D3DXMatrixRotationY(&mat1, (FLOAT)M_PI * 3 / 2);break;
		default:D3DXMatrixRotationY(&mat1, (FLOAT)0);break;
		}
	}

	if (mesh) {
		D3DXMatrixMultiply(&mat1, &mat1, &matLocal);
		D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);

		if (MeshNo >= 0) { //地形以外なら色を変える
			float s = CHIPSIZE / 0.6f;
			D3DXMatrixScaling(&mat2, s, s, s);
			D3DXMatrixMultiply(&mat1, &mat2, &mat1);
			g_D3DDevice->SetTransform(D3DTS_WORLD, &mat1);
			D3DCOLORVALUE col;
			col.r = (((int)Color) >> 16) / 255.0f;
			col.g = ((((int)Color) & 0xff00) >> 8) / 255.0f;
			col.b = (((int)Color) & 0xff) / 255.0f;
			col.a = 1.0f;
			g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			if (MeshNo == 23) {
				float spe = (((int)Effect) & 0x0f) / 15.0f;
				float pow = (((((int)Effect) & 0xf0) >> 4)) / 15.0f;
				pow = pow*pow*46.0f + 4.0f;
				float emi = ((((int)Effect) & 0xf00) >> 8) / 15.0f;
				float emi_ = 1.0f - emi;
				float alpha = 1.0 - ((((int)Effect) & 0xf000) >> 12) / 15.0f;
				if (alpha < 1.0f) {
					g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
					g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				}
				mesh->m_pMaterials[0].Emissive.r = col.r*emi;
				mesh->m_pMaterials[0].Emissive.g = col.g*emi;
				mesh->m_pMaterials[0].Emissive.b = col.b*emi;
				mesh->m_pMaterials[0].Emissive.a = alpha;
				mesh->m_pMaterials[0].Diffuse.r = col.r*emi_;
				mesh->m_pMaterials[0].Diffuse.g = col.g*emi_;
				mesh->m_pMaterials[0].Diffuse.b = col.b*emi_;
				mesh->m_pMaterials[0].Diffuse.a = alpha;
				mesh->m_pMaterials[0].Ambient = mesh->m_pMaterials[0].Diffuse;
				mesh->m_pMaterials[0].Specular.r = spe*1.42f;
				mesh->m_pMaterials[0].Specular.g = spe*1.42f;
				mesh->m_pMaterials[0].Specular.b = spe*1.42f;
				mesh->m_pMaterials[0].Specular.a = 1.0f;
				mesh->m_pMaterials[0].Power = pow;
			}
			else {
				g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				mesh->m_pMaterials[0].Diffuse = col;
				mesh->m_pMaterials[0].Diffuse.a = 1.0f;
				mesh->m_pMaterials[0].Ambient = mesh->m_pMaterials[0].Diffuse;
			}
			if (MeshNo == 10 && (Option == 1 || Option == 2)) {
				FLOAT f = (FLOAT)(pow((double)fabs((double)PowerByFuel), 1.0 / 3.0) / 5.0);
				if (f<0.5f) f = 0.5f;
				D3DXMatrixScaling(&mat2, f, f, f);
				D3DXMatrixMultiply(&mat1, &mat2, &mat1);
				g_D3DDevice->SetTransform(D3DTS_WORLD, &mat1);
				m_pXMesh[33]->Render(g_D3DDevice);
			}
			else mesh->Render(g_D3DDevice);
			if ((MeshNo == 2 || MeshNo == 3)) {
				float e = Effect;if (e < 1.0) e = 1.0;else if (e>10) e = 10.0f;
				if (Option == 1 || Option == 2 || (e>1.0)) {
					if (Option == 2) D3DXMatrixScaling(&mat2, (FLOAT)1.0f, (FLOAT)e, (FLOAT)1.0f);
					if (Option == 1) D3DXMatrixScaling(&mat2, (FLOAT)0.75f, (FLOAT)e, (FLOAT)0.75f);
					if (Option == 0) D3DXMatrixScaling(&mat2, (FLOAT)0.51f, (FLOAT)e, (FLOAT)0.51f);
					D3DXMatrixMultiply(&mat1, &mat2, &mat1);
					g_D3DDevice->SetTransform(D3DTS_WORLD, &mat1);
					m_pXMesh[22]->Render(g_D3DDevice);
				}
			}
		}
		else {
			g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			g_D3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			g_D3DDevice->SetTransform(D3DTS_WORLD, &mat1);
			if (TextureAlpha) {
				g_D3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000066);
				g_D3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				g_D3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

			}
			g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングモード
			mesh->Render(g_D3DDevice);
			if (BackFaces) {
				g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	// カリングモード
				mesh->Render(g_D3DDevice);
			}
			g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			g_D3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}

	}

}
void GWorld::DispNetChip(int n)
{
	if (g_PlayerData[n].ReceiveData.info.dpnidPlayer == 0) return;
	//	if(PrePlayerData[n].ReceiveData.size!=g_PlayerData[n].ReceiveData.size) return;
	int size;

	g_PlayerData[n].haveArm = 0;
	g_PlayerData[n].maxY = -1000000;
	size = g_PlayerData[n].ReceiveData.size / sizeof(GCHIPDATA);
	float w1 = 0.0f, w2 = 1.0f;
	float ww1 = 0.0f, ww2 = 1.0f;
	if (PrePlayerData[n].ReceiveData.size == g_PlayerData[n].ReceiveData.size) {
		DWORD span = g_PlayerData[n].time - PrePlayerData[n].time;
		DWORD span2 = g_PlayerData[n].time2 - PrePlayerData[n].time2;
		DWORD t = timeGetTime() - g_PlayerData[n].time;
		DWORD t2 = timeGetTime() - g_PlayerData[n].time2;
		if (t < span) {
			w2 = (float)t / (float)span;
			w1 = 1.0f - w2;
			if (t < 0) {
				w2 = 0;
				w1 = 1;
			}
		}
		if (t2 < span2) {
			ww2 = (float)t2 / (float)span2;
			ww1 = 1.0f - ww2;
			if (t2 < 0) {
				ww2 = 0;
				ww1 = 1;
			}
		}

	}
	g_PlayerData[n].ChipCount = 0;
	g_PlayerData[n].w1 = w1;
	g_PlayerData[n].w2 = w2;
	g_PlayerData[n].ww1 = ww1;
	g_PlayerData[n].ww2 = ww2;

	for (int i = 0;i < size;i++) {
		GCHIPDATA *chip = &g_PlayerData[n].ReceiveData.data[i];
		GCHIPDATA *chip2 = &PrePlayerData[n].ReceiveData.data[i];
		int id = chip->id & 0xfff;
		int id2 = chip2->id & 0xfff;
		int dir = chip->id >> 12;
		if (id >= 512) {
			g_PlayerData[n].X[id - 512].x = chip->data.f[0];
			g_PlayerData[n].X[id - 512].y = chip->data.f[1];
			g_PlayerData[n].X[id - 512].z = chip->data.f[2];
			continue;
		}

		int type = chip->data.type & 0x3f;
		int op1 = (chip->data.type >> 6) & 0x01;
		int op2 = (chip->data.type >> 7) & 0x01;
		g_PlayerData[n].Jet[g_PlayerData[n].ChipCount] = 0;
		if (type == GT_COWL&&!ShowCowl) { g_PlayerData[n].ChipCount++;continue; }
		CD3DMesh* mesh = NULL;
		int meshNo = 0;

		GMatrix r;
		GQuat q, q2;
		GVector p;
		GVector X1 = g_PlayerData[n].X[id];
		GVector X2 = PrePlayerData[n].X[id];
		GVector X = (((X1 - X2)*w2 + X1) + g_PlayerData[n].X2[id]) / 2.0f;
		g_PlayerData[n].X[g_PlayerData[n].ChipCount].x = chip->data.pos.x / 100.0f + X1.x;
		g_PlayerData[n].X[g_PlayerData[n].ChipCount].y = chip->data.pos.y / 100.0f + X1.y;
		g_PlayerData[n].X[g_PlayerData[n].ChipCount].z = chip->data.pos.z / 100.0f + X1.z;
		if (i == 0) {
			p.x = (chip->data.pos.x / 100.0f + X.x)*ww2 + (chip2->data.pos.x / 100.0f + X.x)*ww1;
			p.y = (chip->data.pos.y / 100.0f + X.y)*ww2 + (chip2->data.pos.y / 100.0f + X.y)*ww1;
			p.z = (chip->data.pos.z / 100.0f + X.z)*ww2 + (chip2->data.pos.z / 100.0f + X.z)*ww1;
		}
		else {
			p.x = (chip->data.pos.x / 100.0f + X.x)*w2 + (chip2->data.pos.x / 100.0f + X.x)*w1;
			p.y = (chip->data.pos.y / 100.0f + X.y)*w2 + (chip2->data.pos.y / 100.0f + X.y)*w1;
			p.z = (chip->data.pos.z / 100.0f + X.z)*w2 + (chip2->data.pos.z / 100.0f + X.z)*w1;
		}
		if (g_PlayerData[n].ChipCount == 0) { g_PlayerData[n].x = p.x;g_PlayerData[n].y = p.y;g_PlayerData[n].z = p.z; }
		if (p.y > g_PlayerData[n].maxY) g_PlayerData[n].maxY = p.y;
		q.x = chip->data.quat.x / 100.0f;
		q.y = chip->data.quat.y / 100.0f;
		q.z = chip->data.quat.z / 100.0f;
		q.w = chip->data.quat.w / 100.0f;
		q2.x = chip2->data.quat.x / 100.0f;
		q2.y = chip2->data.quat.y / 100.0f;
		q2.z = chip2->data.quat.z / 100.0f;
		q2.w = chip2->data.quat.w / 100.0f;
		q = q.slerp(q2, w1);
		q.matrix(r);
		GMatrix m = r.translate(p);
		D3DXMATRIX mat1, mat2;
		D3DXMATRIX matLocal = D3DXMATRIX((FLOAT)m.elem[0][0], (FLOAT)m.elem[0][1], (FLOAT)m.elem[0][2], (FLOAT)m.elem[0][3],
			(FLOAT)m.elem[1][0], (FLOAT)m.elem[1][1], (FLOAT)m.elem[1][2], (FLOAT)m.elem[1][3],
			(FLOAT)m.elem[2][0], (FLOAT)m.elem[2][1], (FLOAT)m.elem[2][2], (FLOAT)m.elem[2][3],
			(FLOAT)m.elem[3][0], (FLOAT)m.elem[3][1], (FLOAT)m.elem[3][2], (FLOAT)m.elem[3][3]);

		switch (dir) {
		case 1:D3DXMatrixRotationY(&mat1, (FLOAT)M_PI / 2);break;
		case 2:D3DXMatrixRotationY(&mat1, (FLOAT)M_PI);break;
		case 3:D3DXMatrixRotationY(&mat1, (FLOAT)M_PI * 3 / 2);break;
		default:D3DXMatrixRotationY(&mat1, (FLOAT)0);break;
		}
		switch (type) {
		case GT_CORE:
			meshNo = 0;
			break;
		case GT_CHIP:
			meshNo = 1;
			break;
		case GT_RUDDER:
			meshNo = 4;
			break;
		case GT_DUMMY:
			meshNo = 6;
			break;
		case GT_WHEEL:
			meshNo = 2;
			break;
		case GT_RLW:
			meshNo = 3;
			break;
		case GT_TRIM:
			meshNo = 5;
			break;
		case GT_JET:
			meshNo = 10;
			break;
		case GT_CHIPH:
			meshNo = 21;
			break;
		case GT_COWL:
			meshNo = 23;
			break;
		case GT_ARM:
			meshNo = 30;
			g_PlayerData[n].haveArm++;
			break;
		case GT_CHIP2:
			meshNo = 7;
			break;
		case GT_RUDDER2:
			meshNo = 16;
			break;
		case GT_TRIM2:
			meshNo = 17;
			break;
		default:
			meshNo = 1;
			break;
		}
		mesh = m_pXMesh[meshNo];

		if (mesh) {
			D3DXMatrixMultiply(&mat1, &mat1, &matLocal);
			D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);

			float s = CHIPSIZE / 0.6f;
			D3DXMatrixScaling(&mat2, s, s, s);
			D3DXMatrixMultiply(&mat1, &mat2, &mat1);
			g_D3DDevice->SetTransform(D3DTS_WORLD, &mat1);
			g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			if (type >= GT_CHIP2) {
				if (chip->data.option >= 1 && !ShowGhost) { g_PlayerData[n].ChipCount++;continue; }
			}
			if (type == GT_COWL) {
				float r1 = ((((int)chip->color) >> 10) / 32.0f)*w2 + ((((int)chip2->color) >> 10) / 32.0f)*w1;
				float g1 = (((((int)chip->color) & 0x3e0) >> 5) / 32.0f)*w2 + (((((int)chip2->color) & 0x3e0) >> 5) / 32.0f)*w1;
				float b1 = ((((int)chip->color) & 0x1f) / 32.0f)*w2 + ((((int)chip->color) & 0x1f) / 32.0f)*w1;

				int opt = chip->data.option & 0x07;
				float spe = (chip->data.option & 0x08) >> 3;//1bit
				float emi = (chip->data.option & 0x10) >> 4;//1bit
				float emi_ = 1.0f - emi;
				float alpha = 1.0f - ((chip->data.option & 0xe0) >> 5) / 7.0f;//3bit
				if (alpha != 1) { g_PlayerData[n].ChipCount++;continue; }

				if (opt == 1) mesh = m_pXMesh[24];
				else if (opt == 2) mesh = m_pXMesh[25];
				else if (opt == 3) mesh = m_pXMesh[26];
				else if (opt == 4) mesh = m_pXMesh[27];
				else if (opt == 5) mesh = m_pXMesh[28];
				if (alpha < 1.0f) {
					g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				}
				mesh->m_pMaterials[0].Emissive.r = r1*emi;
				mesh->m_pMaterials[0].Emissive.g = g1*emi;
				mesh->m_pMaterials[0].Emissive.b = b1*emi;
				mesh->m_pMaterials[0].Emissive.a = alpha;
				mesh->m_pMaterials[0].Diffuse.r = r1*emi_;
				mesh->m_pMaterials[0].Diffuse.g = g1*emi_;
				mesh->m_pMaterials[0].Diffuse.b = b1*emi_;
				mesh->m_pMaterials[0].Diffuse.a = alpha;
				mesh->m_pMaterials[0].Ambient = mesh->m_pMaterials[0].Diffuse;
				mesh->m_pMaterials[0].Specular.r = spe*1.42f;
				mesh->m_pMaterials[0].Specular.g = spe*1.42f;
				mesh->m_pMaterials[0].Specular.b = spe*1.42f;
				mesh->m_pMaterials[0].Specular.a = 1.0f;
				mesh->m_pMaterials[0].Power = 50.0f;
			}
			if (type == GT_WHEEL || type == GT_RLW) {
				if (type == GT_WHEEL && op1) mesh = m_pXMesh[8];
				if (type == GT_RLW && op1) mesh = m_pXMesh[9];
				float e = (chip->data.option >> 2) / 6.3f;if (e < 1.0) e = 1.0f;else if (e>10) e = 10.0f;
				int o = chip->data.option & 0x3;
				if (o == 1 || o == 2 || (e > 1.0)) {
					if (o == 2) D3DXMatrixScaling(&mat2, (FLOAT)1.0f, (FLOAT)e, (FLOAT)1.0f);
					if (o == 1) D3DXMatrixScaling(&mat2, (FLOAT)0.75f, (FLOAT)e, (FLOAT)0.75f);
					if (o == 0) D3DXMatrixScaling(&mat2, (FLOAT)0.51f, (FLOAT)e, (FLOAT)0.51f);
					D3DXMatrixMultiply(&mat1, &mat2, &mat1);
					g_D3DDevice->SetTransform(D3DTS_WORLD, &mat1);
					m_pXMesh[22]->Render(g_D3DDevice);
				}
			}
			if (type == GT_JET) {
				if (op1 == 1 || op1 == 2) {
					FLOAT f = chip->data.option / 10.0f*w2 + chip2->data.option / 10.0f*w1;
					D3DXMatrixScaling(&mat2, f, f, f);
					D3DXMatrixMultiply(&mat1, &mat2, &mat1);
					g_D3DDevice->SetTransform(D3DTS_WORLD, &mat1);
					mesh = m_pXMesh[33];
				}
				else if (op1 == 0) g_PlayerData[n].Jet[g_PlayerData[n].ChipCount] = i;
			}
			if (type != GT_COWL) {
				float r1 = ((((int)chip->color) >> 10) / 32.0f)*w2 + ((((int)chip2->color) >> 10) / 32.0f)*w1;
				float g1 = (((((int)chip->color) & 0x3e0) >> 5) / 32.0f)*w2 + (((((int)chip2->color) & 0x3e0) >> 5) / 32.0f)*w1;
				float b1 = ((((int)chip->color) & 0x1f) / 32.0f)*w2 + ((((int)chip->color) & 0x1f) / 32.0f)*w1;
				mesh->m_pMaterials[0].Diffuse.r = r1;
				mesh->m_pMaterials[0].Diffuse.g = g1;
				mesh->m_pMaterials[0].Diffuse.b = b1;
				mesh->m_pMaterials[0].Diffuse.a = 1.0f;
				mesh->m_pMaterials[0].Ambient = mesh->m_pMaterials[0].Diffuse;
			}
			mesh->Render(g_D3DDevice);

		}
		g_PlayerData[n].ChipCount++;
	}
	//透明カウルを表示
	if (ShowCowl) {
		g_PlayerData[n].ChipCount = 0;
		for (int i = 0;i < size;i++) {
			GCHIPDATA *chip = &g_PlayerData[n].ReceiveData.data[i];
			GCHIPDATA *chip2 = &PrePlayerData[n].ReceiveData.data[i];
			int id = chip->id & 0xfff;
			int id2 = chip2->id & 0xfff;
			int dir = chip->id >> 12;
			if (id >= 512) {
				g_PlayerData[n].X[id - 512].x = chip->data.f[0];
				g_PlayerData[n].X[id - 512].y = chip->data.f[1];
				g_PlayerData[n].X[id - 512].z = chip->data.f[2];
				continue;
			}

			int type = chip->data.type & 0x3f;
			int op1 = (chip->data.type >> 6) & 0x01;
			int op2 = (chip->data.type >> 7) & 0x01;
			if (type != GT_COWL) { g_PlayerData[n].ChipCount++;continue; }
			CD3DMesh* mesh = NULL;
			int meshNo = 0;

			GMatrix r;
			GQuat q, q2;
			GVector p;
			GVector X1 = g_PlayerData[n].X[id];
			GVector X2 = PrePlayerData[n].X[id];
			GVector X = (((X1 - X2)*w2 + X1) + g_PlayerData[n].X2[id]) / 2.0f;
			g_PlayerData[n].X[g_PlayerData[n].ChipCount].x = chip->data.pos.x / 100.0f + X1.x;
			g_PlayerData[n].X[g_PlayerData[n].ChipCount].y = chip->data.pos.y / 100.0f + X1.y;
			g_PlayerData[n].X[g_PlayerData[n].ChipCount].z = chip->data.pos.z / 100.0f + X1.z;
			if (i == 0) {
				p.x = (chip->data.pos.x / 100.0f + X.x)*ww2 + (chip2->data.pos.x / 100.0f + X.x)*ww1;
				p.y = (chip->data.pos.y / 100.0f + X.y)*ww2 + (chip2->data.pos.y / 100.0f + X.y)*ww1;
				p.z = (chip->data.pos.z / 100.0f + X.z)*ww2 + (chip2->data.pos.z / 100.0f + X.z)*ww1;
			}
			else {
				p.x = (chip->data.pos.x / 100.0f + X.x)*w2 + (chip2->data.pos.x / 100.0f + X.x)*w1;
				p.y = (chip->data.pos.y / 100.0f + X.y)*w2 + (chip2->data.pos.y / 100.0f + X.y)*w1;
				p.z = (chip->data.pos.z / 100.0f + X.z)*w2 + (chip2->data.pos.z / 100.0f + X.z)*w1;
			}
			if (g_PlayerData[n].ChipCount == 0) { g_PlayerData[n].x = p.x;g_PlayerData[n].y = p.y;g_PlayerData[n].z = p.z; }
			if (p.y > g_PlayerData[n].maxY) g_PlayerData[n].maxY = p.y;
			q.x = chip->data.quat.x / 100.0f;
			q.y = chip->data.quat.y / 100.0f;
			q.z = chip->data.quat.z / 100.0f;
			q.w = chip->data.quat.w / 100.0f;
			q2.x = chip2->data.quat.x / 100.0f;
			q2.y = chip2->data.quat.y / 100.0f;
			q2.z = chip2->data.quat.z / 100.0f;
			q2.w = chip2->data.quat.w / 100.0f;
			q = q.slerp(q2, w1);
			q.matrix(r);
			GMatrix m = r.translate(p);
			D3DXMATRIX mat1, mat2;
			D3DXMATRIX matLocal = D3DXMATRIX((FLOAT)m.elem[0][0], (FLOAT)m.elem[0][1], (FLOAT)m.elem[0][2], (FLOAT)m.elem[0][3],
				(FLOAT)m.elem[1][0], (FLOAT)m.elem[1][1], (FLOAT)m.elem[1][2], (FLOAT)m.elem[1][3],
				(FLOAT)m.elem[2][0], (FLOAT)m.elem[2][1], (FLOAT)m.elem[2][2], (FLOAT)m.elem[2][3],
				(FLOAT)m.elem[3][0], (FLOAT)m.elem[3][1], (FLOAT)m.elem[3][2], (FLOAT)m.elem[3][3]);

			switch (dir) {
			case 1:D3DXMatrixRotationY(&mat1, (FLOAT)M_PI / 2);break;
			case 2:D3DXMatrixRotationY(&mat1, (FLOAT)M_PI);break;
			case 3:D3DXMatrixRotationY(&mat1, (FLOAT)M_PI * 3 / 2);break;
			default:D3DXMatrixRotationY(&mat1, (FLOAT)0);break;
			}
			meshNo = 23;
			mesh = m_pXMesh[meshNo];

			if (mesh) {
				D3DXMatrixMultiply(&mat1, &mat1, &matLocal);
				D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);

				float s = CHIPSIZE / 0.6f;
				D3DXMatrixScaling(&mat2, s, s, s);
				D3DXMatrixMultiply(&mat1, &mat2, &mat1);
				g_D3DDevice->SetTransform(D3DTS_WORLD, &mat1);
				g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				float r1 = ((((int)chip->color) >> 10) / 32.0f)*w2 + ((((int)chip2->color) >> 10) / 32.0f)*w1;
				float g1 = (((((int)chip->color) & 0x3e0) >> 5) / 32.0f)*w2 + (((((int)chip2->color) & 0x3e0) >> 5) / 32.0f)*w1;
				float b1 = ((((int)chip->color) & 0x1f) / 32.0f)*w2 + ((((int)chip->color) & 0x1f) / 32.0f)*w1;

				int opt = chip->data.option & 0x07;
				float spe = (chip->data.option & 0x08) >> 3;//1bit
				float emi = (chip->data.option & 0x10) >> 4;//1bit
				float emi_ = 1.0f - emi;
				float alpha = 1.0f - ((chip->data.option & 0xe0) >> 5) / 7.0f;//3bit

				if (opt == 1) mesh = m_pXMesh[24];
				else if (opt == 2) mesh = m_pXMesh[25];
				else if (opt == 3) mesh = m_pXMesh[26];
				else if (opt == 4) mesh = m_pXMesh[27];
				else if (opt == 5) mesh = m_pXMesh[28];
				if (alpha == 1) { g_PlayerData[n].ChipCount++;continue; }
				mesh->m_pMaterials[0].Emissive.r = r1*emi;
				mesh->m_pMaterials[0].Emissive.g = g1*emi;
				mesh->m_pMaterials[0].Emissive.b = b1*emi;
				mesh->m_pMaterials[0].Emissive.a = alpha;
				mesh->m_pMaterials[0].Diffuse.r = r1*emi_;
				mesh->m_pMaterials[0].Diffuse.g = g1*emi_;
				mesh->m_pMaterials[0].Diffuse.b = b1*emi_;
				mesh->m_pMaterials[0].Diffuse.a = alpha;
				mesh->m_pMaterials[0].Ambient = mesh->m_pMaterials[0].Diffuse;
				mesh->m_pMaterials[0].Specular.r = spe*1.42f;
				mesh->m_pMaterials[0].Specular.g = spe*1.42f;
				mesh->m_pMaterials[0].Specular.b = spe*1.42f;
				mesh->m_pMaterials[0].Specular.a = 1.0f;
				mesh->m_pMaterials[0].Power = 50.0f;
				mesh->Render(g_D3DDevice);
			}
			g_PlayerData[n].ChipCount++;
		}
	}
	g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	for (int i = 0;i < g_PlayerData[n].ChipCount;i++) {
		GVector X1 = g_PlayerData[n].X[i];
		GVector X2 = PrePlayerData[n].X[i];
		g_PlayerData[n].X2[i] = (X1 - X2)*w2 + X1;
	}
}
void GWorld::DispNetJetAll()
{
	for (int i = 0;i < g_DPlay->GetNumPlayers();i++) {
		if (g_PlayerData[i].ReceiveData.info.dpnidPlayer == 0) continue;
		float w1 = g_PlayerData[i].w1;
		float w2 = g_PlayerData[i].w2;

		for (int j = 0;j < g_PlayerData[i].ChipCount;j++) {
			if (g_PlayerData[i].Jet[j]>0) {
				//				MessageBox(NULL,"セッションの作成に失敗", "RigidChips Network", MB_OK | MB_ICONWARNING);
				int n = g_PlayerData[i].Jet[j];
				GCHIPDATA *chip = &g_PlayerData[i].ReceiveData.data[n];
				GCHIPDATA *chip2 = &PrePlayerData[i].ReceiveData.data[n];
				int id = chip->id & 0xfff;
				int id2 = chip2->id & 0xfff;
				int dir = chip->id >> 12;
				int type = chip->data.type & 0x3f;
				int op1 = (chip->data.type >> 6) & 0x01;
				int op2 = (chip->data.type >> 7) & 0x01;
				GMatrix r;
				GQuat q, q2;
				GVector p;
				GVector X1 = g_PlayerData[i].X[id];
				GVector X2 = PrePlayerData[i].X[id2];
				GVector X = (X1 - X2)*w2 + X1;


				p.x = (chip->data.pos.x / 100.0f + X.x)*w2 + (chip2->data.pos.x / 100.0f + X.x)*w1;
				p.y = (chip->data.pos.y / 100.0f + X.y)*w2 + (chip2->data.pos.y / 100.0f + X.y)*w1;
				p.z = (chip->data.pos.z / 100.0f + X.z)*w2 + (chip2->data.pos.z / 100.0f + X.z)*w1;
				q.x = chip->data.quat.x / 100.0f;
				q.y = chip->data.quat.y / 100.0f;
				q.z = chip->data.quat.z / 100.0f;
				q.w = chip->data.quat.w / 100.0f;
				q2.x = chip2->data.quat.x / 100.0f;
				q2.y = chip2->data.quat.y / 100.0f;
				q2.z = chip2->data.quat.z / 100.0f;
				q2.w = chip2->data.quat.w / 100.0f;
				q = q.slerp(q2, w1);
				q.matrix(r);
				GMatrix tm = r.translate(p);

				float f = chip->data.option / 50.0f;
				if (chip->data.type&GT_OPTION2) f = -f;
				DispNetJet(0, tm, f, dir);
			}
		}
	}
}

void GWorld::DispNetJet(int type, GMatrix tm, float f, int dir)
{
	//ジェットの表示
	//	g_D3DDevice->SetTexture(0,NULL);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// カリングモード
	g_D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);   //SRCの設定
	g_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);  //DESTの設定
	//g_D3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);// 引数の成分を乗算する
	if (type == 0 && JetFlag && f != 0) {
		D3DXMATRIX mat1;
		D3DXMATRIX matLocal = D3DXMATRIX((FLOAT)tm.elem[0][0], (FLOAT)tm.elem[0][1], (FLOAT)tm.elem[0][2], (FLOAT)tm.elem[0][3],
			(FLOAT)tm.elem[1][0], (FLOAT)tm.elem[1][1], (FLOAT)tm.elem[1][2], (FLOAT)tm.elem[1][3],
			(FLOAT)tm.elem[2][0], (FLOAT)tm.elem[2][1], (FLOAT)tm.elem[2][2], (FLOAT)tm.elem[2][3],
			(FLOAT)tm.elem[3][0], (FLOAT)tm.elem[3][1], (FLOAT)tm.elem[3][2], (FLOAT)tm.elem[3][3]);
		D3DXMatrixIdentity(&mat1);
		D3DXMatrixRotationY(&mat1, (FLOAT)(rand() % 100 * M_PI / 50.0));
		D3DXMatrixMultiply(&matLocal, &mat1, &matLocal);
		D3DXMatrixScaling(&mat1, (FLOAT)0.5, (FLOAT)f, (FLOAT)0.5);
		D3DXMatrixMultiply(&matLocal, &mat1, &matLocal);
		D3DXMatrixTranslation(&mat1, 0.0f, -0.32f, 0.0f);
		D3DXMatrixMultiply(&matLocal, &mat1, &matLocal);
		D3DXMatrixMultiply(&matLocal, &matLocal, &GMatWorld);
		g_D3DDevice->SetTransform(D3DTS_WORLD, &matLocal);
		m_pXMesh[11]->Render(g_D3DDevice);
	}
	else if (type == 1 && f != 0) { //ARM
		D3DXMATRIX mat1;
		D3DXMATRIX matLocal = D3DXMATRIX((FLOAT)tm.elem[0][0], (FLOAT)tm.elem[0][1], (FLOAT)tm.elem[0][2], (FLOAT)tm.elem[0][3],
			(FLOAT)tm.elem[1][0], (FLOAT)tm.elem[1][1], (FLOAT)tm.elem[1][2], (FLOAT)tm.elem[1][3],
			(FLOAT)tm.elem[2][0], (FLOAT)tm.elem[2][1], (FLOAT)tm.elem[2][2], (FLOAT)tm.elem[2][3],
			(FLOAT)tm.elem[3][0], (FLOAT)tm.elem[3][1], (FLOAT)tm.elem[3][2], (FLOAT)tm.elem[3][3]);
		D3DXMatrixIdentity(&mat1);
		D3DXMatrixRotationY(&mat1, (FLOAT)(M_PI*dir / 2.0));
		D3DXMatrixMultiply(&matLocal, &mat1, &matLocal);
		D3DXMatrixRotationZ(&mat1, (FLOAT)(rand() % 100 * M_PI / 50.0));
		D3DXMatrixMultiply(&matLocal, &mat1, &matLocal);
		D3DXMatrixScaling(&mat1, (FLOAT)((CHIPSIZE / 2.0f)*f), (FLOAT)((CHIPSIZE / 2.0f)*f), (FLOAT)f * 2);
		D3DXMatrixMultiply(&matLocal, &mat1, &matLocal);
		D3DXMatrixTranslation(&mat1, 0.0f, 0.0f, (CHIPSIZE / 2.0f));
		D3DXMatrixMultiply(&matLocal, &mat1, &matLocal);
		D3DXMatrixMultiply(&matLocal, &matLocal, &GMatWorld);
		g_D3DDevice->SetTransform(D3DTS_WORLD, &matLocal);
		m_pXMesh[31]->Render(g_D3DDevice);
	}
	g_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  //DESTの設定
}
void GWorld::DispNetChipInfo(int n, float zz)
{
	if (g_PlayerData[n].ReceiveData.info.dpnidPlayer == 0) return;
	if (g_PlayerData[n].ReceiveData.size == 0) return;
	if (zz <= 0) return;
	D3DXMATRIX smat, mat, vmat, vmat2;
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	GVector e = EyePos;
	float x = g_PlayerData[n].x;
	float hy = g_PlayerData[n].y + (float)pow((double)g_PlayerData[n].ChipCount, 0.3333) / 3.0f;

	float y = hy + 1.2f;
	float z = g_PlayerData[n].z;
	GVector infoV = GVector(x, y, z);
	float l;
	if (g_MarkerSize != 0.0f) {
		if (g_MarkerSize < 1.5) {
			l = zz*0.1f*g_MarkerSize;
		}
		else {
			l = zz;
			if (l < 50.0f) l = 1.0f;else l = l / 50.0f;
		}
		D3DXMatrixScaling(&smat, l, l, l);
		vmat = GMatView;
		vmat._41 = 0.0f; vmat._42 = 0.0f; vmat._43 = 0.0f;
		D3DXMatrixInverse(&vmat, NULL, &vmat);
		vmat2 = vmat;
		D3DXMatrixTranslation(&mat, x, y + 0.5f*l, z);
		D3DXMatrixMultiply(&vmat, &vmat, &mat);
		D3DXMatrixMultiply(&vmat, &vmat, &GMatWorld);

		D3DXMatrixTranslation(&mat, x, y + 0.3f*l, z);
		D3DXMatrixMultiply(&vmat2, &vmat2, &mat);
		D3DXMatrixMultiply(&vmat2, &vmat2, &GMatWorld);


		D3DXMatrixMultiply(&mat, &smat, &vmat);
		g_D3DDevice->SetTransform(D3DTS_WORLD, &mat);
		DWORD col = g_PlayerData[n].ReceiveData.info.color;
		float r1 = (col >> 16) / 255.0f;
		float g1 = ((col >> 8) & 0xff) / 255.0f;
		float b1 = (col & 0xff) / 255.0f;
		CD3DMesh *mesh;
		if (g_PlayerData[n].haveArm <= 0) mesh = m_pXMesh[34];
		else mesh = m_pXMesh[37];
		mesh->m_pMaterials[0].Emissive.r = r1;
		mesh->m_pMaterials[0].Emissive.g = g1;
		mesh->m_pMaterials[0].Emissive.b = b1;
		mesh->m_pMaterials[0].Emissive.a = 1.0f;
		mesh->m_pMaterials[0].Diffuse.r = 0;
		mesh->m_pMaterials[0].Diffuse.g = 0;
		mesh->m_pMaterials[0].Diffuse.b = 0;
		mesh->m_pMaterials[0].Diffuse.a = 1;
		mesh->m_pMaterials[0].Ambient = mesh->m_pMaterials[0].Diffuse;

		mesh->Render(g_D3DDevice);
	}
	if (g_NameSize != 0.0f) {
		if (g_NameSize < 1.5) {
			l = zz*0.1f*g_NameSize;
		}
		else {
			l = zz;
			if (l < 50.0f) l = 1.0f;else l = l / 50.0f;
		}
		l = l*0.25f;
		D3DXMatrixScaling(&smat, l, l, l);
		D3DXMatrixMultiply(&mat, &smat, &vmat2);
		g_D3DDevice->SetTransform(D3DTS_WORLD, &mat);

		D3DMATERIAL8 mtrlText;
		mtrlText.Power = 1.0f;
		mtrlText.Specular.r = mtrlText.Specular.g = mtrlText.Specular.b = mtrlText.Specular.a = 0;
		mtrlText.Emissive.r = 1;
		mtrlText.Emissive.g = 1;
		mtrlText.Emissive.b = 1;
		mtrlText.Emissive.a = 1.0f;
		mtrlText.Diffuse.r = 0;
		mtrlText.Diffuse.g = 0;
		mtrlText.Diffuse.b = 0;
		mtrlText.Diffuse.a = 0.5;
		mtrlText.Ambient = mtrlText.Diffuse;
		g_D3DDevice->SetMaterial(&mtrlText);
		g_pApp->m_pFont3D->Render3DText(g_PlayerData[n].ReceiveData.info.strPlayerName, D3DFONT_CENTERED | D3DFONT_FILTERED);
	}
}
void GRigid::DispObject()
{
	//	if(!ObjectBallFlag) return;
	//	CD3DMesh* mesh=NULL;
	//	mesh=m_pXMesh[20];
	GMatrix m(TM);
	D3DXMATRIX mat1, mat2;
	D3DXMATRIX matLocal = D3DXMATRIX((FLOAT)m.elem[0][0], (FLOAT)m.elem[0][1], (FLOAT)m.elem[0][2], (FLOAT)m.elem[0][3],
		(FLOAT)m.elem[1][0], (FLOAT)m.elem[1][1], (FLOAT)m.elem[1][2], (FLOAT)m.elem[1][3],
		(FLOAT)m.elem[2][0], (FLOAT)m.elem[2][1], (FLOAT)m.elem[2][2], (FLOAT)m.elem[2][3],
		(FLOAT)m.elem[3][0], (FLOAT)m.elem[3][1], (FLOAT)m.elem[3][2], (FLOAT)m.elem[3][3]);
	D3DXMatrixIdentity(&mat1);
	D3DXMatrixScaling(&mat1, (FLOAT)Param.x, (FLOAT)Param.x, (FLOAT)Param.x);
	D3DXMatrixMultiply(&mat1, &mat1, &matLocal);
	D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);
	g_D3DDevice->SetTransform(D3DTS_WORLD, &mat1);
	CD3DMesh* mesh = m_pXMesh[20];
	mesh->m_pMaterials[1].Diffuse.r = (((int)Color) >> 16) / 255.0f;
	mesh->m_pMaterials[1].Diffuse.g = ((((int)Color) & 0xff00) >> 8) / 255.0f;
	mesh->m_pMaterials[1].Diffuse.b = (((int)Color) & 0xff) / 255.0f;
	mesh->m_pMaterials[1].Diffuse.a = 1.0f;
	mesh->m_pMaterials[1].Ambient = mesh->m_pMaterials[1].Diffuse;
	mesh->m_pMaterials[0].Diffuse.r = mesh->m_pMaterials[1].Diffuse.r*0.5f;
	mesh->m_pMaterials[0].Diffuse.g = mesh->m_pMaterials[1].Diffuse.g*0.5f;
	mesh->m_pMaterials[0].Diffuse.b = mesh->m_pMaterials[1].Diffuse.b*0.5f;
	mesh->m_pMaterials[0].Diffuse.a = 1.0f;
	mesh->m_pMaterials[0].Ambient = mesh->m_pMaterials[0].Diffuse;
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	mesh->Render(g_D3DDevice);
}

void GWorld::DispNetShadow()
{
	//影の表示
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// カリングモード
	g_D3DDevice->SetStreamSource(0, pPointVB, sizeof(D3DPOINTVERTEX));
	g_D3DDevice->SetVertexShader(D3DFVF_POINTVERTEX);
	g_D3DDevice->SetTexture(0, NULL);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);


	g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングモード
	g_D3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void GRigid::DispShadow()
{
	//影の表示
	if (MeshNo == 23 && !ShowCowl) return;
	if (Ghost >= 1 && !ShowGhost) return;
	if (FrameFlag) return;

	if (ShowShadowFlag && (Type == GTYPE_DISK || Type == GTYPE_FACE || Type == GTYPE_BALL)) {
		static GVector sv1[GCHIPMAX * 2], sv2[GCHIPMAX * 2];
		int sn2;
		g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// カリングモード
		g_D3DDevice->SetStreamSource(0, pPointVB, sizeof(D3DPOINTVERTEX));
		g_D3DDevice->SetVertexShader(D3DFVF_POINTVERTEX);
		g_D3DDevice->SetTexture(0, NULL);
		g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		g_D3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
		D3DPOINTVERTEX* p2V;
		g_D3DDevice->SetTransform(D3DTS_WORLD, &GMatWorld);
		GVector n = GVector(0, 1, 0.5f).normalize2();
		int lc = g_World->Land->List2Count;
		int *list = g_World->Land->List2;
		if (FastShadow) {
			g_World->Land->List1up(Top->TotalCenter, Top->TotalRadius);
			lc = g_World->Land->List1Count;
			list = g_World->Land->List1;

		}
		FLOAT f = (FLOAT)(pow((double)fabs((double)Power), 1.0 / 3.0) / 5.0);
		if (f < 0.5f) f = 0.5f;
		GMatrix m1 = GMatrix().scale(GVector(f, f, f)).translate(X);
		GMatrix m2 = GMatrix().translate(X);
		for (int i = 0;i < lc;i++) {
			GVector norm = g_World->Land->Face[list[i]].Normal;
			GVector norm0 = norm / 100.0f;
			GVector vert = g_World->Land->Face[list[i]].Vertex[0];
			bool flag = false;
			GFloat tmin = 0.1f;
			int jj = 0;
			for (int j = 0;j < Shape.PointN - 1;j++) {
				if (ChipType == 7 && (Option == 1 || Option == 2)) { //気球
					GVector v = Shape.Point[j] * m1;
					GFloat t = v.distanceOnFaceAndLine(norm, vert, n);
					if (t > 0.1) { flag = true;break; }
					if (t < -1000000) t = -1000000;
					if (t < tmin) tmin = t;
					sv1[jj] = n*t + v + norm0;
					sv1[jj].y -= 0.01f;
					jj++;
				}
				else if (Type == GTYPE_DISK) {
					GVector v = Shape.Point[j] * TM;
					GFloat t = v.distanceOnFaceAndLine(norm, vert, n);
					if (t > 0.1) { flag = true;break; }
					if (t < -1000000) t = -1000000;
					if (t < tmin) tmin = t;
					sv1[jj] = n*t + v + norm0;
					sv1[jj].y -= 0.01f;
					jj++;
				}
				else if (Type == GTYPE_BALL) {
					GVector v = Shape.Point[j] * m2;
					GFloat t = v.distanceOnFaceAndLine(norm, vert, n);
					if (t > 0.1) { flag = true;break; }
					if (t < -1000000) t = -1000000;
					if (t < tmin) tmin = t;
					sv1[jj] = n*t + v + norm0;
					sv1[jj].y -= 0.01f;
					jj++;
				}
				else if (Type == GTYPE_FACE) {
					if (j >= 4) break;
					GVector v = (Shape.Point[j + 4] * TM);
					GFloat t = v.distanceOnFaceAndLine(norm, vert, n);
					if (t > 0.1) { flag = true;break; }
					if (t < -1000000) t = -1000000;
					if (t < tmin) tmin = t;
					sv1[jj] = n*t + v + norm0;
					sv1[jj].y -= 0.01f;
					jj++;
				}
			}
			if (flag) continue;
			if (FastShadow) {
				if (tmin < -3) tmin = -3;
				else if (tmin>0) tmin = 0;
				tmin = tmin / 3.0f;
			}
			else {
				if (tmin < -10) tmin = -10;
				else if (tmin>0) tmin = 0;
				tmin = tmin / 10.0f;
			}
			if (tmin >= 0.9999) continue;
			g_World->Land->ClipShadow(sv1, jj, list[i], sv2, &sn2);
			if (sn2 > 2) {
				pPointVB->Lock(0, 0, (BYTE**)&p2V, 0);
				int col = 0x00111111 + (long)((1.0 - tmin*tmin) * 0x88) * 0x01000000;
				for (j = 0;j < sn2;j++) {
					p2V[j].x = (float)sv2[j].x;
					p2V[j].y = (float)sv2[j].y;
					p2V[j].z = (float)sv2[j].z;
					p2V[j].color = col;
				}
				p2V[j].x = (float)sv2[0].x;
				p2V[j].y = (float)sv2[0].y;
				p2V[j].z = (float)sv2[0].z;
				p2V[j].color = col;
				pPointVB->Unlock();
				g_D3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, sn2 - 2);
			}
		}
		//水面の影の表示
		g_D3DDevice->SetStreamSource(0, pPointVB, sizeof(D3DPOINTVERTEX));
		g_D3DDevice->SetVertexShader(D3DFVF_POINTVERTEX);
		g_D3DDevice->SetTransform(D3DTS_WORLD, &GMatWorld);
		n = GVector(0, 1, 0.5f).normalize2();
		bool flag = false;
		GFloat tmin = 0.1f;
		int jj = 0;
		sn2 = 0;
		float ndot = n.dot(GVector(0, -1, 0));
		for (int j = 0;j < Shape.PointN - 1;j++) {
			if (Type == GTYPE_DISK) {
				GVector v = (Shape.Point[j] * TM);
				GFloat t = (v.y - WATER_LINE)*ndot;

				sv2[sn2] = v + n*t;
				if (t > 0.01) { flag = true;break; }
				if (t < -1000000) t = -1000000;
				if (t < tmin) tmin = t;
				sv2[sn2].y = WATER_LINE - 1.0f / 100.0f;
				sn2++;

			}
			else if (Type == GTYPE_FACE) {
				if (j >= 4) break;
				GVector v = (Shape.Point[j + 4] * TM);
				GFloat t = (v.y - WATER_LINE)*ndot;

				sv2[sn2] = v + n*t;
				if (t > 0.1) { flag = true;break; }
				if (t<-1000000) t = -1000000;
				if (t < tmin) tmin = t;
				sv2[sn2].y = WATER_LINE - 1.0f / 100.0f;
				sn2++;
			}
		}
		if (sn2 > 2) {
			if (FastShadow) {
				if (tmin < -3) tmin = -3;
				else if (tmin>0) tmin = 0;
				tmin = tmin / 3.0f;
			}
			else {
				if (tmin < -10) tmin = -10;
				else if (tmin>0) tmin = 0;
				tmin = tmin / 10.0f;
			}
			if (flag == false && tmin>-1.0) {
				pPointVB->Lock(0, 0, (BYTE**)&p2V, 0);
				int col = 0x00111111 + (long)((1.0 - tmin*tmin) * 0x88) * 0x01000000;
				for (j = 0;j < sn2;j++) {
					p2V[j].x = (float)sv2[j].x;
					p2V[j].y = (float)sv2[j].y;
					p2V[j].z = (float)sv2[j].z;
					p2V[j].color = col;
				}
				p2V[j].x = (float)sv2[0].x;
				p2V[j].y = (float)sv2[0].y;
				p2V[j].z = (float)sv2[0].z;
				p2V[j].color = col;
				pPointVB->Unlock();
				g_D3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, sn2 - 2);
			}
		}
	}
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングモード
	g_D3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

void GRigid::ApplyExtForce()
{
	float cd = Cd;
	int type = Type;
	float px = Param.x;
	float pz = Param.z;
	float volume = Volume;
	if (ChipType == 7 && (Option == 1 || Option == 2)) {
		cd = 0.1f;
		type = GTYPE_BALL;
		float f = (float)(pow((double)fabs((double)PowerByFuel*(float)GDTSTEP / 6.0), 1.0 / 3.0) / 5.0);
		if (f < 0.5f) f = 0.5f;
		px = pz = CHIPSIZE*f / 2;
		volume = (GFloat)((px / 2)*(px / 2)*(px / 2) * 4 / 3.0*M_PI);
	}
	Ext.clear();

	if (AirFlag == false) return;
	if (MeshNo == 23) return;
	GVector n = GVector(0, 1, 0)*R;
	GVector nw = GVector(0, 1, 0)*R;
	GVector vw = V - AirSpeed;
	GVector v = V;
	if (v.dot(n) < 0) n = -n;
	if (vw.dot(nw) < 0) nw = -nw;
	GFloat va = v.abs();
	//	if(v.y-Top->V.y<-0.001) va=va*(1.0f-(v.y-Top->V.y)/10);
	GFloat vwa = vw.abs();
	//	if(vw.y<0) vwa=vwa*(1.0f-vw.y/20);
	GFloat u, uw;
	GFloat k = 30.0f;		//インチキ係数
	if (type == GTYPE_BALL) {
		//6πμｒｖ 
		u = (GFloat)(5.0 + va / 36.0f) / k / px;
		//		if(u>M/10) u=M/10;
		uw = u;
		n = v.normalize2();
		nw = vw.normalize2();
	}
	else {
		u = (v.normalize()).dot(n);	//投影面積
		uw = (vw.normalize()).dot(nw);	//投影面積
	}
	if (X.y <= WATER_LINE + 0.3 && X.y >= WATER_LINE - 0.3) { //水表面
		if ((rand() % 200 + 2) < (int)V.abs()) {
			float a = 1.0f + (rand() % 100) / 5000.0f - 0.01f;
			if (a > 2.0f) a = 2.0f;else if (a <= 0.0f) a = 0.0f;
			GVector w = GVector(V.x / 100.0f, 0.02f + fabs(V.y) / 60.0f, V.z / 100.0f);
			//if(v.abs()>0.1f) v=v.normalize()/10.0f;
			g_WaterLineParticle->Add(X, w, GVector(0, -0.005f, 0), 0.08f, a, 0.04f, GVector(1, 1, 1));
		}
	}
	if (X.y <= WATER_LINE) { //水の中
		if (X.y <= WATER_LINE - 100 && Top && Top->TotalCount > 2) {
			Ext += (X - Top->TotalCenterOfGravity)*(X.y - WATER_LINE + 100)*M*0.01f;
			//ApplyImpulse((,X);
		}
		Ext += ((-g_World->G*M) - (g_World->G*(1100.0f*volume - M)))*g_World->Dt;
		Ext += -v.normalize()*va*va*1.225f*g_World->Dt*1.0f;
		Ext += -n*cd*va*va*u*px*pz*1.225f*g_World->Dt*0.5f*k*(30.0f);
		//ApplyImpulse(,X);
		//ApplyImpulse(,X);
		//ApplyImpulse(,X);
		L -= L*0.01f;
	}
	else {
		float a = X.y - WATER_LINE;
		float hk = 1.0f;
		if (X.y > 300) hk = 1.0f - (X.y - 300) / 10000.0f;if (hk <= 0) hk = 0.0f;
		hk = hk*hk;
		if (V.y < 0.0 && a <= 1) {
			Ext += GVector(0, (GFloat)fabs(GVector(0, 1, 0).dot(nw))*cd*V.y*V.y*uw*px*pz*1.225f*g_World->Dt*0.5f*k*(1 - a*a) * 100 * hk, 0);
			//ApplyImpulse(,X);	//1.225f[Kg/m^3]は空気密度
		}
		if (uw > 0) uw = (GFloat)pow((double)uw, 1.5);
		Ext += -nw*cd*vwa*vwa*uw*px*pz*1.225f*g_World->Dt*0.5f*k*hk;
		//ApplyImpulse(,X);	//1.225f[Kg/m^3]は空気密度

		if (va < 1.0f) {
			Ext += -vw.normalize()*vwa*M*g_World->Dt*2.0f*cd*hk;
			//ApplyImpulse(,X);
		}
		L -= L*0.5f*cd*g_World->Dt*hk;
	}

	ApplyImpulse(Ext, X);
}

// hookmain.cpp で実装されている
bool InitHook();

static const char* GetPCString(unsigned int control_word)
{
	switch (control_word & MCW_PC)
	{
	case _PC_24:
		return "_PC_24";

	case _PC_53:
		return "_PC_53";

	case _PC_64:
		return "_PC_64";

	default:
		return "Unknown";
	}
}

#ifdef _M_AMD64
bool InitFP(unsigned int pc_control_word)
{
	// x64 ビルド用
//    std::cout << "64bit version" << std::endl;
	const unsigned int old_control_word_x87 = _controlfp(0, 0);
	//    std::cout << "MCW_PC (x87): " << GetPCString( old_control_word_x87 ) << std::endl;
	return true;
}
#else
#if _MSC_VER >= 1400
bool InitFP(unsigned int precision_control)
{
	// Visual C++ 2005 以降
//    std::cout << "32bit version" << std::endl;
	unsigned int old_control_word_x87;
	if (0 != __control87_2(0, 0, &old_control_word_x87, 0))
	{
		//       std::cout << "Old MCW_PC (x87): " << GetPCString( old_control_word_x87 ) << std::endl;
	}

	unsigned int new_control_word_x87;
	if (0 != __control87_2(precision_control, MCW_PC, &new_control_word_x87, 0))
	{
		//        std::cout << "New MCW_PC (x87): " << GetPCString( new_control_word_x87 ) << std::endl;
	}

	return true;
}
#else
bool InitFP(unsigned int precision_control)
{
	// Visual C++ 2005 以前
//    std::cout << "32bit version" << std::endl;
	const unsigned int old_control_word_x87 = _control87(0, 0);
	//    std::cout << "Old MCW_PC (x87): " << GetPCString( old_control_word_x87 ) << std::endl;

	_control87(precision_control, _MCW_RC);

	const unsigned int new_control_word_x87 = _control87(0, 0);
	//    std::cout << "New MCW_PC (x87): " << GetPCString( new_control_word_x87 ) << std::endl;

	return true;
}
#endif
#endif
//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	/*
		//数値演算プロセッサの調整
		// フックの開始
		if( ! InitHook() )
		{
			return 1;
		}

		// D3DCREATE_FPU_PRESERVE を指定しなかった場合，
		// IDirect3DDevice9 作成時にこの精度に下げられる
		if( ! InitFP( _PC_24 ) )
		{
			return 1;
		}
	*/
	CMyD3DApplication d3dApp;

	g_pApp = &d3dApp;
	g_hInst = hInst;
	if (szCmdLine[0] != '\0') exit(-1);

	if (FAILED(d3dApp.Create(hInst)))
		return 0;
	/*
		// Windowモードなら、現在のディスプレイモードを取得する
		D3DDISPLAYMODE	d3ddm;
		if(d3dApp.m_bWindowed){
			if(FAILED(d3dApp.m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))){
				::OutputDebugString("Error CreateD3D() : GetAdapterDisplayMode()\n");
				return FALSE;
			}
		}
		else{
			d3ddm.Format = D3DFMT_A8R8G8B8;
		}
		// デバイスの能力を調べる
		D3DDEVTYPE	type = D3DDEVTYPE_HAL;
		DWORD		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

		// D3DFMT_D24S8 が使えるか調べる
		if(FAILED(d3dApp.m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
				d3ddm.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8))){
			type = D3DDEVTYPE_REF;
		}
		else if(FAILED(d3dApp.m_pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
				d3ddm.Format, d3ddm.Format, D3DFMT_D24S8))){
			type = D3DDEVTYPE_REF;
		}

		// 頂点シェーダが使用できるかどうか調べる
		if(d3dApp.m_d3dCaps.MaxVertexShaderConst < 4){
			behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
		else if(!(d3dApp.m_d3dCaps.VertexShaderVersion & 0x0000FF00)){
			behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
		if(type == D3DDEVTYPE_HAL && behavior == D3DCREATE_HARDWARE_VERTEXPROCESSING) HardShadowFlag=true;
		else HardShadowFlag=false;
	*/	return d3dApp.Run();
}




//-----------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication()
{
	int i;
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_CHECK_CRT_DF);
#endif
	g_DPlay = new GDPlay;
	g_DPlay->Init(g_hInst);
	g_MessageData[0] = '\0';
	for (i = 0;i < GPLAYERMAX;i++) {
		g_RecieaveMessageCode[i] = 0;
		g_RecieaveMessageData[i][0] = '\0';
	}

	MouseX = 0;
	MouseY = 0;
	MouseL = 0;
	MouseR = 0;
	MouseM = 0;
	CtrlKey = 0;

	SoundType = 1;

	m_dwCreationWidth = 640;
	m_dwCreationHeight = 480;
	m_strWindowTitle = TEXT("RigidChips 1.5.B27");
	m_bUseDepthBuffer = TRUE;

	m_dLimidFPS = 1000 / g_LimitFPS;
	m_inputFocus = true;

	// Create a D3D font using d3dfont.cpp


	m_pFont = new CD3DFont(_T("Arial"), 9, D3DFONT_BOLD);
	m_pFontL = new CD3DFont(_T("Arial"), 16, D3DFONT_BOLD);
	m_pFont3D = new CD3DFont(_T("Arial"), 9, D3DFONT_ZENABLE);
	m_bLoadingApp = TRUE;
	m_pInputDeviceManager = NULL;
	//    m_pMusicManager             = NULL;
	//    m_pBoundSound1              = NULL;
	m_pDIConfigSurface = NULL;
	m_hIMC = NULL;

	ZeroMemory(&m_UserInput, sizeof(m_UserInput));
	m_fWorldRotX = 0.0f;
	m_fWorldRotY = 0.0f;

	m_appID = 0;

	for (i = 0;i < GMODELMAX;i++) m_pXMesh[i] = new CD3DMesh();
	g_pLandMesh = NULL;
	m_pSkyMesh = new CD3DMesh();
	for (i = 0;i < GTEXMAX;i++) pMyTexture[i] = NULL;
	// Read settings from registry
	GetModuleFileName(NULL, AppDir, MAX_PATH);
	// Separate the path from the filename
	TCHAR* strLastSlash = _tcsrchr(AppDir, TEXT('\\'));
	*strLastSlash = TEXT('\0');
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char work[_MAX_PATH];
	_tcscpy(work, AppDir);
	_tcscpy(work, TEXT("\\Resources"));
	//最初にファイルを探す
	hFind = FindFirstFile(work, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		GetCurrentDirectory(MAX_PATH, AppDir);
	}
	_tcscpy(ResourceDir, AppDir);
	_tcscat(ResourceDir, _T("\\Resources"));
	_tcscpy(CurrScenarioDir, ResourceDir);
	_tcscpy(DataDir, AppDir);
	_tcscat(DataDir, _T("\\Data"));
	_tcscpy(CurrDataDir, DataDir);
	ReadSettings();
	if (SetCurrentDirectory(CurrDataDir) == 0) {
		_tcscpy(CurrDataDir, DataDir);
	}
	for (i = 0;i < 6;i++) ControlKeysLock[i] = false;
	LastChatData[0] = '\0';

}




//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::OneTimeSceneInit()
{
	//    HWND              g_hWnd;              // The main app window

		// TODO: perform one time initialization
		//**********************
	timeBeginPeriod(1);
	// See if we were lobby launched
//	if( g_DPlay->m_hLobbyHandle != NULL ) LobbyLaunch();
	// Drawing loading status message until app finishes
	SendMessage(m_hWnd, WM_PAINT, 0, 0);
	g_hWnd = m_hWnd;

	HMENU hMenu = GetMenu(m_hWnd);
	if (SoundType) CheckMenuItem(hMenu, IDM_SETTING_SOUND, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SETTING_SOUND, MF_UNCHECKED);

	if (ShowShadowFlag) CheckMenuItem(hMenu, IDM_SHOWSHADOW, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SHOWSHADOW, MF_UNCHECKED);
	if (ShowDustFlag) CheckMenuItem(hMenu, IDM_SHOWDUST, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SHOWDUST, MF_UNCHECKED);
	if (DitherFlag) CheckMenuItem(hMenu, IDM_DITHER, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_DITHER, MF_UNCHECKED);
	if (ShowMeter) CheckMenuItem(hMenu, IDM_SHOWMETER, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SHOWMETER, MF_UNCHECKED);
	if (ShowRegulation) CheckMenuItem(hMenu, IDM_SHOWREGULATION, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SHOWREGULATION, MF_UNCHECKED);
	if (ShowVariable) CheckMenuItem(hMenu, IDM_SHOWVARIABLE, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SHOWVARIABLE, MF_UNCHECKED);
	if (ShowMessage) CheckMenuItem(hMenu, IDM_SHOWSCRIPTMESSAGE, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SHOWSCRIPTMESSAGE, MF_UNCHECKED);
	if (ShowGhost) CheckMenuItem(hMenu, IDM_SETTING_SHOWGHOST, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SETTING_SHOWGHOST, MF_UNCHECKED);
	if (ShowCowl) CheckMenuItem(hMenu, IDM_SETTING_SHOWCOWL, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SETTING_SHOWCOWL, MF_UNCHECKED);
	if (TextureAlpha) CheckMenuItem(hMenu, IDM_SETTING_TEXTUREALPHA, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SETTING_TEXTUREALPHA, MF_UNCHECKED);
	if (BackFaces) CheckMenuItem(hMenu, IDM_SETTING_SHOWBACKFACE, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SETTING_SHOWBACKFACE, MF_UNCHECKED);
	if (m_dLimidFPS == (1000 / 15)) {
		CheckMenuItem(hMenu, IDM_LIMIT15, MF_CHECKED);
		CheckMenuItem(hMenu, IDM_LIMIT30, MF_UNCHECKED);
	}
	else if (m_dLimidFPS == (1000 / 30)) {
		CheckMenuItem(hMenu, IDM_LIMIT30, MF_CHECKED);
		CheckMenuItem(hMenu, IDM_LIMIT15, MF_UNCHECKED);
	}
	else {
		CheckMenuItem(hMenu, IDM_LIMIT30, MF_UNCHECKED);
		CheckMenuItem(hMenu, IDM_LIMIT15, MF_UNCHECKED);
	}

	// Initialize DirectInput
	InitInput(m_hWnd);
	ClearInput(&m_UserInput);

	m_hIMC = ImmAssociateContext(m_hWnd, NULL);//IMEを無効にする


	// Initialize audio
//    InitAudio( m_hWnd );
	hMidiOut = NULL;
	if (SoundType == 1) {
		long msg;
		if (midiOutOpen(&hMidiOut, (UINT)MIDI_MAPPER, NULL, 0, 0) == 0) {
			//GMシステム・オン
			//音色を変える(0xCn,音色,0x00)  nはチャンネル(0～0xF)
			msg = MAKELONG(MAKEWORD(0xC0, 0x7D), MAKEWORD(0x00, 0));
			midiOutShortMsg(hMidiOut, msg); // Note On
			msg = MAKELONG(MAKEWORD(0xC1, 0x7E), MAKEWORD(0x00, 0));
			midiOutShortMsg(hMidiOut, msg); // Note On
			msg = MAKELONG(MAKEWORD(0xB1, 0x07), MAKEWORD(0x00, 0));
			midiOutShortMsg(hMidiOut, msg); // Note On
			msg = MAKELONG(MAKEWORD(0x91, 70), MAKEWORD(0x3f, 0));
			midiOutShortMsg(hMidiOut, msg); // Note On
		}
	}
	GravityFlag = true;
	AirFlag = true;
	TorqueFlag = true;
	JetFlag = true;
	CCDFlag = true;
	lightColor = GVector(1.00f, 1.00f, 1.00f);
	FogColor = GVector(200.0f, 230.0f, 255.0f);

	g_World = new GWorld(1 / (float)g_LimitFPS, GDTSTEP);	//ステップ・サブステップ
	g_GroundParticle = new GParticle(1200);
	g_WaterLineParticle = new GParticle(300);
	g_JetParticle = new GParticle(2000);
	g_Bullet = new GBullet(500);

	AirSpeed.clear();

	if (SetCurrentDirectory(DataDir) == 0) return  E_FAIL;
	if (SetCurrentDirectory(ResourceDir) == 0) return  E_FAIL;

	if (readData("Basic.txt", false)) return  E_FAIL;
	if (LoadSystem("System.rcs")) return E_FAIL;
	luaSystemInit();
	lstrcpy(szUpdateFileName, ResourceDir);
	lstrcat(szUpdateFileName, TEXT("\\Basic.txt"));
	lstrcpy(szUpdateFileName0, TEXT("Basic.txt"));
	lstrcpy(szLandFileName, ResourceDir);
	lstrcat(szLandFileName, TEXT("\\Land.x"));
	lstrcpy(szLandFileName0, TEXT("Land.x"));
	lstrcpy(szSystemFileName, ResourceDir);
	lstrcat(szSystemFileName, TEXT("\\System.rcs"));
	lstrcpy(szSystemFileName0, TEXT("System.rcs"));
	if (g_World->getChipCount() == 0) return E_FAIL;
	//	g_Chip[0]->X.y=g_World->Land->GetY(0,0);
	Course[0].Count = 0;
	for (int i = 0;i < g_VarCount;i++) {
		g_ValList[i].Val = g_ValList[i].Def;
		if (g_ValList[i].Val > g_ValList[i].Max) g_ValList[i].Val = g_ValList[i].Max;
		if (g_ValList[i].Val < g_ValList[i].Min) g_ValList[i].Val = g_ValList[i].Min;
		g_ValList[i].Updated = true;
		for (int j = 0;j < g_ValList[i].RefCount;j++) {
			if (g_ValList[i].Flag[j])
				*(g_ValList[i].Ref[j]) = -g_ValList[i].Val;
			else *(g_ValList[i].Ref[j]) = g_ValList[i].Val;
		}
	}
	for (int c = 0;c < g_World->getChipCount();c++) {
		if (g_Chip[c]->ChipType == 0) {
			ResetChip(c, 0);
		}

	}
	g_World->MainStepCount = 0;
	ResetCount = 90;
	g_World->DestroyFlag = !UnbreakableFlag;
	//**********************
	UserRefPos = RefPos = g_Chip[0]->X;
	UserEyePos = EyePos = RefPos + GVector(-50, 50, -20);
	Zoom = 45.0f;
	TurnLR = 0.0f;
	TurnUD = 0.0f;

	for (int i = 0;i < GRINGMAX;i++) {
		Ring[i].Color = GVector(0, 0.3f, 1);
		Ring[i].State = 0;
		Ring[i].Dir = GVector(0, 0, 0);
		Ring[i].Point = GVector(0, 0, 0);
		Ring[i].Scale = 10.0f;
	}
	//オブジェクトの作成
/*	g_World->AddObject(GTYPE_BALL,false,1.2f,1.2f,1.2f,0.3f);
	g_World->Object[0]->X.x=g_Chip[0]->X.x+g_Chip[0]->Top->TotalRadius+1;
	g_World->Object[0]->X.y=g_Chip[0]->X.y+g_Chip[0]->Top->TotalRadius+1;
	g_World->Object[0]->X.z=g_Chip[0]->X.z;
*/

	m_bLoadingApp = FALSE;
	g_World->MainStepCount = 0;
	CompassTarget.y = -100000.0;
	UserUpVec = GVector(0, 1, 0);
	//	if(g_SystemLua!=NULL) luaSystemRun("OnOpenChips");
	if (g_SystemLua != NULL && (g_World->B26Bullet || g_DPlay->GetNumPlayers() == 0)) luaSystemRun("OnInit");
	if (ScriptType == 1 && g_ScriptLua != NULL) luaScriptRun(g_ScriptLua, "OnInit");
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ReadSettings()
// Desc: Read the app settings from the registry
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::ReadSettings()
{
	HKEY hkey;
	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, DXAPP_KEY,
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL))
	{
		// TODO: change as needed

		// Read the stored window width/height.  This is just an example,
		// of how to use DXUtil_Read*() functions.
		DXUtil_ReadIntRegKey(hkey, TEXT("Width"), &m_dwCreationWidth, m_dwCreationWidth);
		DXUtil_ReadIntRegKey(hkey, TEXT("Height"), &m_dwCreationHeight, m_dwCreationHeight);
		DXUtil_ReadIntRegKey(hkey, TEXT("Sound"), &SoundType, SoundType);

		DXUtil_ReadIntRegKey(hkey, TEXT("ShowMeter"), &ShowMeter, ShowMeter);
		DXUtil_ReadIntRegKey(hkey, TEXT("ShowRegulation"), &ShowRegulation, ShowRegulation);
		DXUtil_ReadIntRegKey(hkey, TEXT("ShowShadowFlag"), &ShowShadowFlag, ShowShadowFlag);
		DXUtil_ReadIntRegKey(hkey, TEXT("ShowMessage"), &ShowMessage, ShowMessage);
		DXUtil_ReadIntRegKey(hkey, TEXT("ShowVariable"), &ShowVariable, ShowVariable);
		DXUtil_ReadIntRegKey(hkey, TEXT("ShowCowl"), &ShowCowl, ShowCowl);
		DXUtil_ReadIntRegKey(hkey, TEXT("ShowGhost"), &ShowGhost, ShowGhost);
		DXUtil_ReadIntRegKey(hkey, TEXT("LimidFPS"), &m_dLimidFPS, m_dLimidFPS);
		DXUtil_ReadIntRegKey(hkey, TEXT("FastShadow"), &FastShadow, FastShadow);
		if (m_dLimidFPS == (1000 / 15)) g_LimitFPS = 15;
		else  g_LimitFPS = 30;

		DXUtil_ReadIntRegKey(hkey, TEXT("TextureAlpha"), &TextureAlpha, TextureAlpha);
		DXUtil_ReadIntRegKey(hkey, TEXT("BackFaces"), &BackFaces, BackFaces);
		DXUtil_ReadIntRegKey(hkey, TEXT("ShowShadowFlag"), &ShowShadowFlag, ShowShadowFlag);
		DXUtil_ReadIntRegKey(hkey, TEXT("ShowDustFlag"), &ShowDustFlag, ShowDustFlag);
		DXUtil_ReadIntRegKey(hkey, TEXT("DitherFlag"), &DitherFlag, DitherFlag);
		DWORD v = (DWORD)g_FarMax;
		DXUtil_ReadIntRegKey(hkey, TEXT("FogLevel"), &v, v);
		g_FarMax = (float)v;
		v = (DWORD)(g_MarkerSize * 1000);
		DXUtil_ReadIntRegKey(hkey, TEXT("MarkerSize"), &v, v);
		g_MarkerSize = (float)(v / 1000.0f);
		v = (DWORD)(g_NameSize * 1000);
		DXUtil_ReadIntRegKey(hkey, TEXT("NameSize"), &v, v);
		g_NameSize = (float)(v / 1000.0f);

		DXUtil_ReadStringRegKey(hkey, TEXT("CurrWorkDir"), CurrDataDir, MAX_PATH, DataDir);

		DXUtil_ReadStringRegKey(hkey, TEXT("SessionName"), SessionName, 256, "RigidChips");
		if (SessionName[0] == '\0') strcpy(SessionName, "RigidChips");
		DXUtil_ReadStringRegKey(hkey, TEXT("PlayerName"), PlayerName, MAX_PLAYER_NAME, "Player");
		if (PlayerName[0] == '\0') strcpy(PlayerName, "Player");
		DXUtil_ReadStringRegKey(hkey, TEXT("HostName"), HostName, 256, "localhost");
		if (HostName[0] == '\0') strcpy(HostName, "localhost");
		DXUtil_ReadIntRegKey(hkey, TEXT("PortNo"), &PortNo, PortNo);
		if (PortNo == 0) PortNo = 2345;

		if (m_dwCreationWidth < 160) m_dwCreationWidth = 160;
		if (m_dwCreationHeight < 120) m_dwCreationHeight = 120;

		RegCloseKey(hkey);
	}
}




//-----------------------------------------------------------------------------
// Name: WriteSettings()
// Desc: Write the app settings to the registry
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::WriteSettings()
{
	HKEY hkey;
	DWORD dwType = REG_DWORD;
	DWORD dwLength = sizeof(DWORD);

	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, DXAPP_KEY,
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL))
	{
		// TODO: change as needed

		// Write the window width/height.  This is just an example,
		// of how to use DXUtil_Write*() functions.
		DXUtil_WriteIntRegKey(hkey, TEXT("Width"), m_rcWindowClient.right);
		DXUtil_WriteIntRegKey(hkey, TEXT("Height"), m_rcWindowClient.bottom);
		DXUtil_WriteIntRegKey(hkey, TEXT("Sound"), SoundType);
		DXUtil_WriteIntRegKey(hkey, TEXT("ShowMeter"), ShowMeter);
		DXUtil_WriteIntRegKey(hkey, TEXT("ShowRegulation"), ShowRegulation);
		DXUtil_WriteIntRegKey(hkey, TEXT("ShowShadowFlag"), ShowShadowFlag);
		DXUtil_WriteIntRegKey(hkey, TEXT("ShowMessage"), ShowMessage);
		DXUtil_WriteIntRegKey(hkey, TEXT("ShowVariable"), ShowVariable);
		DXUtil_WriteIntRegKey(hkey, TEXT("ShowCowl"), ShowCowl);
		DXUtil_WriteIntRegKey(hkey, TEXT("ShowGhost"), ShowGhost);
		DXUtil_WriteIntRegKey(hkey, TEXT("LimidFPS"), m_dLimidFPS);
		DXUtil_WriteIntRegKey(hkey, TEXT("FastShadow"), FastShadow);
		DXUtil_WriteIntRegKey(hkey, TEXT("TextureAlpha"), TextureAlpha);
		DXUtil_WriteIntRegKey(hkey, TEXT("BackFaces"), BackFaces);
		DXUtil_WriteIntRegKey(hkey, TEXT("ShowShadowFlag"), ShowShadowFlag);
		DXUtil_WriteIntRegKey(hkey, TEXT("ShowDustFlag"), ShowDustFlag);
		DXUtil_WriteIntRegKey(hkey, TEXT("DitherFlag"), DitherFlag);
		DWORD v = (DWORD)g_FarMax;
		DXUtil_WriteIntRegKey(hkey, TEXT("FogLevel"), v);
		v = (DWORD)(g_MarkerSize * 1000);
		DXUtil_WriteIntRegKey(hkey, TEXT("MarkerSize"), v);
		v = (DWORD)(g_NameSize * 1000);
		DXUtil_WriteIntRegKey(hkey, TEXT("NameSize"), v);
		DXUtil_WriteStringRegKey(hkey, TEXT("CurrWorkDir"), CurrDataDir);

		DXUtil_WriteStringRegKey(hkey, TEXT("SessionName"), SessionName);
		DXUtil_WriteStringRegKey(hkey, TEXT("PlayerName"), PlayerName);
		DXUtil_WriteStringRegKey(hkey, TEXT("HostName"), HostName);
		DXUtil_WriteIntRegKey(hkey, TEXT("PortNo"), PortNo);

		RegCloseKey(hkey);
	}
}





//-----------------------------------------------------------------------------
// Name: StaticInputAddDeviceCB()
// Desc: Static callback helper to call into CMyD3DApplication class
//-----------------------------------------------------------------------------
HRESULT CALLBACK CMyD3DApplication::StaticInputAddDeviceCB(
	CInputDeviceManager::DeviceInfo* pDeviceInfo,
	const DIDEVICEINSTANCE* pdidi,
	LPVOID pParam)
{
	CMyD3DApplication* pApp = (CMyD3DApplication*)pParam;
	return pApp->InputAddDeviceCB(pDeviceInfo, pdidi);
}




//-----------------------------------------------------------------------------
// Name: InputAddDeviceCB()
// Desc: Called from CInputDeviceManager whenever a device is added. 
//       Set the crush zone, and creates a new InputDeviceState for each device
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InputAddDeviceCB(CInputDeviceManager::DeviceInfo* pDeviceInfo,
	const DIDEVICEINSTANCE* pdidi)
{
	// Setup the deadzone 
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 500;
	pDeviceInfo->pdidDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

	// Create a new InputDeviceState for each device so the 
	// app can record its state 
	InputDeviceState* pNewInputDeviceState = new InputDeviceState;
	ZeroMemory(pNewInputDeviceState, sizeof(InputDeviceState));
	pDeviceInfo->pParam = (LPVOID)pNewInputDeviceState;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitInput()
// Desc: Initialize DirectInput objects
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitInput(HWND hWnd)
{
	HRESULT hr;

	// Setup action format for the actual gameplay
	ZeroMemory(&m_diafGame, sizeof(DIACTIONFORMAT));
	m_diafGame.dwSize = sizeof(DIACTIONFORMAT);
	m_diafGame.dwActionSize = sizeof(DIACTION);
	m_diafGame.dwDataSize = NUMBER_OF_GAMEACTIONS * sizeof(DWORD);
	m_diafGame.guidActionMap = g_guidApp;

	// TODO: change the genre as needed
	m_diafGame.dwGenre = DIVIRTUAL_CAD_3DCONTROL;

	m_diafGame.dwNumActions = NUMBER_OF_GAMEACTIONS;
	m_diafGame.rgoAction = g_rgGameAction;
	m_diafGame.lAxisMin = -1000;
	m_diafGame.lAxisMax = 1000;
	m_diafGame.dwBufferSize = 32;
	_tcscpy(m_diafGame.tszActionMap, _T("Rigid Game"));

	// Create a new input device manager
	m_pInputDeviceManager = new CInputDeviceManager();

	if (FAILED(hr = m_pInputDeviceManager->Create(hWnd, NULL, m_diafGame,
		StaticInputAddDeviceCB, this)))
		return DXTRACE_ERR_NOMSGBOX("m_pInputDeviceManager->Create", hr);

	return S_OK;
}



/*
//-----------------------------------------------------------------------------
// Name: InitAudio()
// Desc: Initialize DirectX audio objects
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitAudio( HWND hWnd )
{
	HRESULT hr;

	// Create the music manager class, used to create the sounds
	m_pMusicManager = new CMusicManager();
	if( FAILED( hr = m_pMusicManager->Initialize( hWnd ) ) )
		return DXTRACE_ERR_NOMSGBOX( "m_pMusicManager->Initialize", hr );

	// Instruct the music manager where to find the files
	// TODO: Set this to the media directory, or use resources
	m_pMusicManager->SetSearchDirectory( ResourceDir );

	// TODO: load the sounds from resources (or files)
	m_pMusicManager->CreateSegmentFromFile( &m_pBoundSound1, _T("BOUND1.WAV") );

	return S_OK;
}
*/



//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device initialization, this code checks the display device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::ConfirmDevice(D3DCAPS8* pCaps, DWORD dwBehavior,
	D3DFORMAT Format)
{
	BOOL bCapsAcceptable;

	// TODO: Perform checks to see if these display caps are acceptable.
	bCapsAcceptable = TRUE;

	if (bCapsAcceptable)
		return S_OK;
	else
		return E_FAIL;
}

HRESULT CMyD3DApplication::LoadData(char *filename)
{
	GFloat x = g_Chip[0]->X.x, z = g_Chip[0]->X.z;
	int errCode = 0;
	if ((errCode = readData(filename, true)) == 0) {
		char szDrive[_MAX_DRIVE + 1];	// ドライブ名格納領域 
		char szPath[_MAX_PATH + 1];	// パス名格納領域 
		char szTitle[_MAX_FNAME + 1];	// ファイルタイトル格納領域 
		char szExt[_MAX_EXT + 1];		// ファイル拡張子格納領域 
		// ファイル拡張子格納領域 

		// 絶対パスを分解 
		_splitpath(filename,
			szDrive, szPath,
			szTitle, szExt);

		lstrcpy(CurrDataDir, szDrive);
		lstrcat(CurrDataDir, szPath);

		lstrcpy(szUpdateFileName, filename);
		lstrcpy(szUpdateFileName0, szTitle);
		lstrcat(szUpdateFileName0, szExt);
		readData(filename, false);
		if (g_World->getChipCount() == 0) return S_OK;
		ResetChips(x, z, 0);
		InitChips(0, 1);
	}
	else {
		return errCode;
	}
	return 0;
}
HRESULT CMyD3DApplication::SaveProp(FILE *fp, int i)
{
	GRigid *r = g_World->RecRigid[i];
	/* 構造体データをファイルに出力 */
	fwrite(r, sizeof(GRigid), 1, fp);
	/*
	//物理特性
	//GVector X;		//重心位置
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->X.x,r->X.y,r->X.z);
	//GVector preX;		//重心位置
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->preX.x,r->preX.y,r->preX.z);
	//GMatrix R;		//向き(行列)
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->R.elem[0][0],r->R.elem[0][1],r->R.elem[0][2],r->R.elem[0][3]);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->R.elem[1][0],r->R.elem[1][1],r->R.elem[1][2],r->R.elem[1][3]);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->R.elem[2][0],r->R.elem[2][1],r->R.elem[2][2],r->R.elem[2][3]);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->R.elem[3][0],r->R.elem[3][1],r->R.elem[3][2],r->R.elem[3][3]);
	//GMatrix Rt;		//向きの逆行列
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->Rt.elem[0][0],r->Rt.elem[0][1],r->Rt.elem[0][2],r->Rt.elem[0][3]);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->Rt.elem[1][0],r->Rt.elem[1][1],r->Rt.elem[1][2],r->Rt.elem[1][3]);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->Rt.elem[2][0],r->Rt.elem[2][1],r->Rt.elem[2][2],r->Rt.elem[2][3]);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->Rt.elem[3][0],r->Rt.elem[3][1],r->Rt.elem[3][2],r->Rt.elem[3][3]);
	//GQuat   Q;		//向き(クォータニオン)
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->Q.x,r->Q.y,r->Q.z,r->Q.w);
	//GVector P;		//並進運動量
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->P.x,r->P.y,r->P.z);
	//GVector L;		//角運動量
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->L.x,r->L.y,r->L.z);

	GVector P2;		//並進運動量
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->P2.x,r->P2.y,r->P2.z);
	GVector L2;		//角運動量
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->L2.x,r->L2.y,r->L2.z);

	//GVector V;		//速度
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->V.x,r->V.y,r->V.z);
	//GVector preV;		//速度
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->preV.x,r->preV.y,r->preV.z);
	//GVector W;		//角速度
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->W.x,r->W.y,r->W.z);

	if(r->ChipType==GT_COWL) {
		//GMatrix33 I;		//慣性テンソル
		fprintf(fp,"1 0 0\n");
		fprintf(fp,"0 1 0\n");
		fprintf(fp,"0 0 1\n");
		//GMatrix33 I_;		//慣性テンソルの逆数
		fprintf(fp,"1 0 0\n");
		fprintf(fp,"0 1 0\n");
		fprintf(fp,"0 0 1\n");
	}
	else {
		//GMatrix33 I;		//慣性テンソル
		fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->I.elem[0][0],r->I.elem[0][1],r->I.elem[0][2]);
		fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->I.elem[1][0],r->I.elem[1][1],r->I.elem[1][2]);
		fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->I.elem[2][0],r->I.elem[2][1],r->I.elem[2][2]);
		//GMatrix33 I_;		//慣性テンソルの逆数
		fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->I_.elem[0][0],r->I_.elem[0][1],r->I_.elem[0][2]);
		fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->I_.elem[1][0],r->I_.elem[1][1],r->I_.elem[1][2]);
		fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->I_.elem[2][0],r->I_.elem[2][1],r->I_.elem[2][2]);
	}

	//GMatrix TM;		//姿勢行列（位置・角度）
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->TM.elem[0][0],r->TM.elem[0][1],r->TM.elem[0][2],r->TM.elem[0][3]);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->TM.elem[1][0],r->TM.elem[1][1],r->TM.elem[1][2],r->TM.elem[1][3]);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->TM.elem[2][0],r->TM.elem[2][1],r->TM.elem[2][2],r->TM.elem[2][3]);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf %50.30lf\n",r->TM.elem[3][0],r->TM.elem[3][1],r->TM.elem[3][2],r->TM.elem[3][3]);
	//GVector VData[3];
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->Power,r->PowerSave,r->Power2);
	//int IData[3];
	int a=0;if(r->Parent==NULL) a=1;
	fprintf(fp,"%d %50.30lf %50.30lf %d\n",r->ByeFlag,r->preF,r->Effect,a);
	fprintf(fp,"%50.30lf\n",r->MaxImpulse);
	fprintf(fp,"%50.30lf\n",r->Color);

	fprintf(fp,"%50.30lf\n",r->Radius);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->Bias.x,r->Bias.y,r->Bias.z);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->TopBias.x,r->TopBias.y,r->TopBias.z);
	fprintf(fp,"%50.30lf\n",r->TotalRadius);
	fprintf(fp,"%50.30lf\n",r->TotalMass);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->TotalCenter.x,r->TotalCenter.y,r->TotalCenter.z);
	fprintf(fp,"%50.30lf %50.30lf %50.30lf\n",r->TotalCenterOfGravity.x,r->TotalCenterOfGravity.y,r->TotalCenterOfGravity.z);
	fprintf(fp,"%d\n",r->TotalCount);
	fprintf(fp,"%d\n",r->TotalCowlCount);
	fprintf(fp,"%d\n",r->TotalHitCount);
	*/

	return S_OK;
}
HRESULT CMyD3DApplication::LoadProp(FILE *fp, int i)
{
	GRigid *r = g_World->RecRigid[i];
	GRigid *rs = g_World->Rigid[i];
	/* 構造体データをファイルから入力 */
	fread(r, sizeof(GRigid), 1, fp);
	r->World = g_World;
	r->LinkInfo = rs->LinkInfo;
	//		r->ID=i;
	//		r->HitN=0;
	//		r->TotalHitCount=0;
	if (r->Top) r->Top = rs->Top;
	r->CowlTop = rs->CowlTop;
	if (r->Parent) r->Parent = rs->Parent;
	r->ChildCount = rs->ChildCount;
	for (int j = 0;j < rs->ChildCount;j++) {
		r->Child[j] = rs->Child[j];
	}

	/*
	//物理特性
	//GVector X;		//重心位置
	fscanf(fp,"%g %g %g",&r->X.x,&r->X.y,&r->X.z);
	//GVector preX;		//重心位置
	fscanf(fp,"%g %g %g",&r->preX.x,&r->preX.y,&r->preX.z);
	//GMatrix R;		//向き(行列)
	fscanf(fp,"%g %g %g %g",&r->R.elem[0][0],&r->R.elem[0][1],&r->R.elem[0][2],&r->R.elem[0][3]);
	fscanf(fp,"%g %g %g %g",&r->R.elem[1][0],&r->R.elem[1][1],&r->R.elem[1][2],&r->R.elem[1][3]);
	fscanf(fp,"%g %g %g %g",&r->R.elem[2][0],&r->R.elem[2][1],&r->R.elem[2][2],&r->R.elem[2][3]);
	fscanf(fp,"%g %g %g %g",&r->R.elem[3][0],&r->R.elem[3][1],&r->R.elem[3][2],&r->R.elem[3][3]);
	//GMatrix Rt;		//向きの逆行列
	fscanf(fp,"%g %g %g %g",&r->Rt.elem[0][0],&r->Rt.elem[0][1],&r->Rt.elem[0][2],&r->Rt.elem[0][3]);
	fscanf(fp,"%g %g %g %g",&r->Rt.elem[1][0],&r->Rt.elem[1][1],&r->Rt.elem[1][2],&r->Rt.elem[1][3]);
	fscanf(fp,"%g %g %g %g",&r->Rt.elem[2][0],&r->Rt.elem[2][1],&r->Rt.elem[2][2],&r->Rt.elem[2][3]);
	fscanf(fp,"%g %g %g %g",&r->Rt.elem[3][0],&r->Rt.elem[3][1],&r->Rt.elem[3][2],&r->Rt.elem[3][3]);
	//GQuat   Q;		//向き(クォータニオン)
	fscanf(fp,"%g %g %g %g",&r->Q.x,&r->Q.y,&r->Q.z,&r->Q.w);
	//GVector P;		//並進運動量
	fscanf(fp,"%g %g %g",&r->P.x,&r->P.y,&r->P.z);
	//GVector L;		//角運動量
	fscanf(fp,"%g %g %g",&r->L.x,&r->L.y,&r->L.z);

	GVector P2;		//並進運動量
	fscanf(fp,"%g %g %g",&r->P2.x,&r->P2.y,&r->P2.z);
	GVector L2;		//角運動量
	fscanf(fp,"%g %g %g",&r->L2.x,&r->L2.y,&r->L2.z);

	//GVector V;		//速度
	fscanf(fp,"%g %g %g",&r->V.x,&r->V.y,&r->V.z);
	//GVector preV;		//速度
	fscanf(fp,"%g %g %g",&r->preV.x,&r->preV.y,&r->preV.z);
	//GVector W;		//角速度
	fscanf(fp,"%g %g %g",&r->W.x,&r->W.y,&r->W.z);

	//GMatrix33 I;		//慣性テンソル
	fscanf(fp,"%g %g %g",&r->I.elem[0][0],&r->I.elem[0][1],&r->I.elem[0][2]);
	fscanf(fp,"%g %g %g",&r->I.elem[1][0],&r->I.elem[1][1],&r->I.elem[1][2]);
	fscanf(fp,"%g %g %g",&r->I.elem[2][0],&r->I.elem[2][1],&r->I.elem[2][2]);
	//GMatrix33 I_;		//慣性テンソルの逆数
	fscanf(fp,"%g %g %g",&r->I_.elem[0][0],&r->I_.elem[0][1],&r->I_.elem[0][2]);
	fscanf(fp,"%g %g %g",&r->I_.elem[1][0],&r->I_.elem[1][1],&r->I_.elem[1][2]);
	fscanf(fp,"%g %g %g",&r->I_.elem[2][0],&r->I_.elem[2][1],&r->I_.elem[2][2]);

	//GMatrix TM;		//姿勢行列（位置・角度）
	fscanf(fp,"%g %g %g %g",&r->TM.elem[0][0],&r->TM.elem[0][1],&r->TM.elem[0][2],&r->TM.elem[0][3]);
	fscanf(fp,"%g %g %g %g",&r->TM.elem[1][0],&r->TM.elem[1][1],&r->TM.elem[1][2],&r->TM.elem[1][3]);
	fscanf(fp,"%g %g %g %g",&r->TM.elem[2][0],&r->TM.elem[2][1],&r->TM.elem[2][2],&r->TM.elem[2][3]);
	fscanf(fp,"%g %g %g %g",&r->TM.elem[3][0],&r->TM.elem[3][1],&r->TM.elem[3][2],&r->TM.elem[3][3]);
	//GVector VData[3];
	fscanf(fp,"%g %g %g",&r->Power,&r->PowerSave,&r->Power2);
	//int IData[3];
	fscanf(fp,"%d %g %g %d",&r->ByeFlag,&r->preF,&r->Effect,&r->tmp);
	fscanf(fp,"%g",&r->MaxImpulse);
	fscanf(fp,"%g",&r->Color);

	fscanf(fp,"%g",&r->Radius);
	fscanf(fp,"%g %g %g",&r->Bias.x,&r->Bias.y,&r->Bias.z);
	fscanf(fp,"%g %g %g",&r->TopBias.x,&r->TopBias.y,&r->TopBias.z);
	fscanf(fp,"%g",&r->TotalRadius);
	fscanf(fp,"%g",&r->TotalMass);
	fscanf(fp,"%g %g %g",&r->TotalCenter.x,&r->TotalCenter.y,&r->TotalCenter.z);
	fscanf(fp,"%g %g %g",&r->TotalCenterOfGravity.x,&r->TotalCenterOfGravity.y,&r->TotalCenterOfGravity.z);
	fscanf(fp,"%d",&r->TotalCount);
	fscanf(fp,"%d",&r->TotalCowlCount);
	fscanf(fp,"%d",&r->TotalHitCount);
*/
	return S_OK;
}

HRESULT CMyD3DApplication::SaveLog(char *fname)
{
	char str[1024];
	char *s;
	FILE *fp1, *fp2;

	if ((fp1 = fopen(szUpdateFileName, "r")) == NULL) return E_FAIL;
	if ((fp2 = fopen(fname, "wb")) == NULL) { fclose(fp1); return E_FAIL; }
	int a = 0;
	fputs("// RCLOG Ver 1.6\n", fp2);
	do {
		s = fgets(str, 1024, fp1);
		if (s) fputs(str, fp2);
	} while (s);
	fclose(fp1);
	fprintf(fp2, "MODELEND\n");
	fprintf(fp2, "Log 1.6 {\n");
	fprintf(fp2, "%d ", recGravityFlag);
	fprintf(fp2, "%d ", recAirFlag);
	fprintf(fp2, "%d ", recTorqueFlag);
	fprintf(fp2, "%d ", recJetFlag);
	fprintf(fp2, "%d ", recUnbreakableFlag);
	fprintf(fp2, "%d ", recCCDFlag);
	fprintf(fp2, "%d ", ScriptFlag);
	fprintf(fp2, "%d ", EfficientFlag);
	//	fprintf(fp2,"%d\n",EfficientFlag);
	fprintf(fp2, "%s\n", szUpdateFileName0);
	fprintf(fp2, "%s\n", szLandFileName0);
	for (int i = 0;i < g_World->ChipCount;i++) {
		SaveProp(fp2, i);
	}
	fprintf(fp2, "%d\n", RecTickCount);
	for (int i = 0;i < g_VarCount;i++) {
		fwrite(&(g_ValList[i].RecVal), sizeof(GFloat), 1, fp2);
	}
	fprintf(fp2, "%d %d %d\n", RecCheckPoint, RecGameTime, RecWaitCount);
	fprintf(fp2, "%d %d %d\n", RecLastBye, 0, 0);

	for (int i = 0;i < KeyRecordMax;i++) {
		for (int j = 0;j < GKEYMAX + 7;j++) {
			fprintf(fp2, " %d", KeyRecord[i][j]);
		}
		fprintf(fp2, "\n");
	}
	fprintf(fp2, "-1\n}\n");

	fclose(fp2);
	return S_OK;
}

HRESULT CMyD3DApplication::LoadLog(char *fname)
{
	int i;
	FILE *fp;
	FILE *fp1, *fp2;
	char str[512];
	char fn[512];
	char logkey[256];
	float ver;
	char *s;
	//tempモデルファイルにコピー
	lstrcpy(fn, DataDir);
	lstrcat(fn, "\\temp.txt");
	if ((fp1 = fopen(fname, "r")) == NULL) return E_FAIL;
	fgets(str, 1024, fp1);
	sscanf(str, "// RCLOG Ver %g", &ver);
	if (ver < 1.6) {
		fclose(fp1);
		return E_FAIL;
	}
	if ((fp2 = fopen(fn, "w")) == NULL) { fclose(fp1); return E_FAIL; }
	do {
		s = fgets(str, 1024, fp1);
		sscanf(str, "%s", logkey);
		if (strncmp(logkey, "MODELEND", 8) == 0) break;
		if (s) fputs(str, fp2);
	} while (s);
	fclose(fp1);
	fclose(fp2);


	if ((fp = fopen(fname, "rb")) == NULL) return E_FAIL;
	int a;
	readData2(fp, false);
	g_World->RestoreLink(g_Chip[0], g_Chip[0]);
	fscanf(fp, "%s %g {\n", str, &ver);
	fscanf(fp, "%d ", &a);recGravityFlag = (a != 0) ? true : false;
	fscanf(fp, "%d ", &a);recAirFlag = (a != 0) ? true : false;
	fscanf(fp, "%d ", &a);recTorqueFlag = (a != 0) ? true : false;
	fscanf(fp, "%d ", &a);recJetFlag = (a != 0) ? true : false;
	fscanf(fp, "%d ", &a);recUnbreakableFlag = (a != 0) ? true : false;
	fscanf(fp, "%d ", &a);recCCDFlag = (a != 0) ? true : false;
	fscanf(fp, "%d ", &a);recScriptFlag = (a != 0) ? true : false;
	fscanf(fp, "%d", &a);recEnergyFlag = (a != 0) ? true : false;


	lstrcpy(szUpdateFileName, fn);
	fscanf(fp, "%s", szUpdateFileName0);
	//****
	lstrcpy(szUpdateFileName0, TEXT("temp.txt"));
	lstrcpy(szUpdateFileName, DataDir);
	lstrcat(szUpdateFileName, TEXT("\\"));
	lstrcat(szUpdateFileName, szUpdateFileName0);
	fscanf(fp, "%s\n", szTempFileName0);
	CopyChip(g_World->RecRigid, g_World->Rigid);
	//	CopyObject(g_World->RecObject,g_World->Object);
	for (i = 0;i < g_World->ChipCount;i++) {
		LoadProp(fp, i);
	}
	fscanf(fp, "%d\n", &RecTickCount);
	for (i = 0;i < g_World->ChipCount;i++) {
		if (g_World->RecRigid[i]->tmp) {
			g_World->DeleteLink(g_World->RecRigid[i]);
		}
	}

	for (i = 0;i < g_VarCount;i++) {
		/* 構造体データをファイルから入力 */
		fread(&(g_ValList[i].RecVal), sizeof(GFloat), 1, fp);
	}
	fscanf(fp, "%d %d %d", &RecCheckPoint, &RecGameTime, &RecWaitCount);
	if (ver > 1.2) {
		int di;
		fscanf(fp, "%d %d %d", &RecLastBye, &di, &di);
	}

	int eof = 0;
	for (i = 0;i < GRECMAX;i++) {
		for (int j = 0;j < GKEYMAX + 7;j++) {
			fscanf(fp, "%d", &a);
			if (j < GKEYMAX && a < 0) { eof = 1;break; }
			KeyRecord[i][j] = a;
		}
		if (eof != 0) break;
	}

	KeyRecordCount = 0;
	KeyRecordMax = i;
	g_World->MainStepCount = 0;
	CopyChip(g_World->Rigid, g_World->RecRigid);
	//	CopyChip(g_World->Object,g_World->RecObject);
	ResetRecVal();
	CurrentCheckPoint = RecCheckPoint;
	GameTime = RecGameTime;waitCount = RecWaitCount;

	return S_OK;
}
HRESULT CMyD3DApplication::PlayLog() {
	if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
	if (KeyRecordMax > 0) {
		GravityFlag = recGravityFlag;
		AirFlag = recAirFlag;
		TorqueFlag = recTorqueFlag;
		JetFlag = recJetFlag;
		UnbreakableFlag = recUnbreakableFlag;
		CCDFlag = recCCDFlag;
		ScriptFlag = recScriptFlag;
		EfficientFlag = recEnergyFlag;
		SetRegulationMenu();
		if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
		KeyRecordMode = 2;
		KeyRecordCount = 0;
		CopyChip(g_World->Rigid, g_World->RecRigid);
		CopyObject(g_World->Object, g_World->RecObject);
		ResetRecVal();
		CurrentCheckPoint = RecCheckPoint;
		GameTime = RecGameTime;waitCount = RecWaitCount;
	}
	return S_OK;
}
HRESULT LoadLand(LPDIRECT3DDEVICE8 Device, char *fname)
{
	char szDrive[_MAX_DRIVE + 1];	// ドライブ名格納領域 
	char szPath[_MAX_PATH + 1];	// パス名格納領域 
	char szTitle[_MAX_FNAME + 1];	// ファイルタイトル格納領域 
	char szExt[_MAX_EXT + 1];		// ファイル拡張子格納領域 

	// 絶対パスを分解 
	_splitpath(fname,
		szDrive, szPath,
		szTitle, szExt);
	char p[512];
	lstrcpy(p, szDrive);
	lstrcat(p, szPath);
	if (p[0] != '\0') {
		if (SetCurrentDirectory(p) == 0) return  E_FAIL;
	}
	FILE *fp;
	if ((fp = fopen(fname, "rb")) == NULL) return E_FAIL;
	fclose(fp);
	HRESULT hr;
	unsigned int i;
	if (g_pLandMesh) {
		g_pLandMesh->Destroy();
		delete g_pLandMesh;
	}

	if (g_World->Land) delete g_World->Land;
	g_World->Land = NULL;


	g_pLandMesh = new CD3DMesh();
	if (FAILED(hr = g_pLandMesh->Create(Device, fname))) return E_FAIL;
	//	g_pLandMesh->InvalidateDeviceObjects();
	//	g_pLandMesh->RestoreDeviceObjects(Device);
	LPDIRECT3DVERTEXBUFFER8 pMeshVB;
	LPDIRECT3DINDEXBUFFER8 pMeshIB;
	D3DVERTEX             *pVertex;
	struct { short p1, p2, p3; } *pIndex;

	for (i = 0;i < (signed int)g_pLandMesh->m_dwNumMaterials;i++) {
		g_pLandMesh->m_pMaterials[i].Diffuse.a = 0.5;
	}

	int v = g_pLandMesh->GetSysMemMesh()->GetNumVertices();
	int f = g_pLandMesh->GetSysMemMesh()->GetNumFaces();
	s_NumFace = f;
	g_World->AddLand(v, f);
	g_World->LandRigid->MeshNo = -1;

	g_pLandMesh->GetSysMemMesh()->GetVertexBuffer(&pMeshVB);

	// テーブルサイズ
	LPD3DXMESH lpMeshC;
	g_pLandMesh->GetSysMemMesh()->Optimize(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL, &lpMeshC);
	lpMeshC->GetAttributeTable(NULL, &(g_World->Land->AttribTableSize));
	D3DXATTRIBUTERANGE AttribTable[100];
	lpMeshC->GetAttributeTable(AttribTable, &(g_World->Land->AttribTableSize));
	pMeshVB->Lock(0, 0, (BYTE**)&pVertex, D3DLOCK_READONLY);
	landCode = 0;
	for (i = 0;i < (unsigned int)v;i++) {
		g_World->Land->Vertex[i].Pos.x = pVertex[i].x;
		g_World->Land->Vertex[i].Pos.y = pVertex[i].y;
		g_World->Land->Vertex[i].Pos.z = pVertex[i].z;
		landCode += (int)(g_World->Land->Vertex[i].Pos.x + g_World->Land->Vertex[i].Pos.y + g_World->Land->Vertex[i].Pos.z);
	}
	pMeshVB->Unlock();
	pMeshVB->Release();
	//	g_pLandMesh->RestoreDeviceObjects(Device);
	lpMeshC->GetIndexBuffer(&pMeshIB);
	pMeshIB->Lock(0, 0, (BYTE**)&pIndex, D3DLOCK_READONLY);
	for (i = 0;i < (unsigned int)f;i++) {
		g_World->Land->Face[i].AttribNo = 0;
		g_World->Land->Face[i].Ud = 1.0f;
		g_World->Land->Face[i].Us = 1.0f;
		g_World->Land->Face[i].Ux = 0.0f;
		for (unsigned int j = 0;j<g_World->Land->AttribTableSize;j++) {
			if (AttribTable[j].FaceStart <= i && AttribTable[j].FaceStart + AttribTable[j].FaceCount>i) {
				g_World->Land->Face[i].AttribNo = j;
				g_World->Land->Face[i].Ux = g_pLandMesh->m_pMaterials[j].Emissive.r;
				g_World->Land->Face[i].Us = g_pLandMesh->m_pMaterials[j].Emissive.g + 1.0f;
				g_World->Land->Face[i].Ud = g_pLandMesh->m_pMaterials[j].Emissive.b + 1.0f;
				//if(g_World->Land->Face[i].Ux<0.0) g_World->Land->Face[i].Ux=0.0f;
				if (g_World->Land->Face[i].Us < 0.0) g_World->Land->Face[i].Us = 0.0f;
				if (g_World->Land->Face[i].Ud < 0.0) g_World->Land->Face[i].Ud = 0.0f;
				break;
			}
		}
		g_World->Land->Face[i].Index[0] = pIndex[i].p1;
		g_World->Land->Face[i].Index[1] = pIndex[i].p2;
		g_World->Land->Face[i].Index[2] = pIndex[i].p3;
		landCode += (int)((pIndex[i].p1 * 10 + pIndex[i].p2 * 10 + pIndex[i].p3 * 10) + (g_World->Land->Face[i].Ud * 100 + g_World->Land->Face[i].Us * 100 + g_World->Land->Face[i].Ux * 100));
	}
	pMeshIB->Unlock();
	pMeshIB->Release();
	if (lpMeshC != NULL) lpMeshC->Release();

	for (unsigned int j = 0;j < g_World->Land->AttribTableSize;j++) {
		g_pLandMesh->m_pMaterials[j].Emissive.r = 0.0f;
		g_pLandMesh->m_pMaterials[j].Emissive.g = 0.0f;
		g_pLandMesh->m_pMaterials[j].Emissive.b = 0.0f;
	}
	g_World->Land->Set();
	g_World->MainStepCount = -1;
	if (SetCurrentDirectory(CurrDataDir) == 0) return  E_FAIL;

	return S_OK;
}
char *LoadGame(char *fname)
{
	int c = 1;
	FILE *fp;
	gameCode = 0;
	if ((fp = fopen(fname, "rb")) != NULL) {
		;
		char ch;
		while ((ch = getc(fp)) != EOF) gameCode += ch;
		fclose(fp);
	}

	if ((fp = fopen(fname, "r")) == NULL) return NULL;
	float ver;
	char str[512];
	static char landfilename[512];
	fscanf(fp, "%s %g", str, &ver);
	fscanf(fp, "%s", &landfilename);
	while (true) {
		char ch;
		do {
			ch = getc(fp);
		} while (ch != '"');
		int j = 0;
		do {
			ch = Course[c].Name[j++] = getc(fp);
		} while (ch != '"');
		Course[c].Name[j - 1] = '\0';
		if (strcmp(Course[c].Name, "END") == 0) break;
		fscanf(fp, "%g,%g,%g,%g", &Course[c].StartPoint.x, &Course[c].StartPoint.y, &Course[c].StartPoint.z, &Course[c].StartAngleY);
		int a1, a2, a3, a4, a5, a6, a7, a8;
		fscanf(fp, "%d,%d,%d,%d,%d,%d,%d,%d", &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8);
		Course[c].GravityFlag = (a1 != 0) ? true : false;
		Course[c].AirFlag = (a2 != 0) ? true : false;
		Course[c].TorqueFlag = (a3 != 0) ? true : false;
		Course[c].JetFlag = (a4 != 0) ? true : false;
		Course[c].UnbreakableFlag = (a5 != 0) ? true : false;
		Course[c].CCDFlag = (a6 != 0) ? true : false;
		Course[c].ScriptFlag = (a7 != 0) ? true : false;
		Course[c].EfficientFlag = (a8 != 0) ? true : false;
		float fuel;
		fscanf(fp, "%g", &fuel);

		j = 0;
		do {
			fscanf(fp, "%g,%g,%g,%g,%g,%g,%g"
				, &Course[c].Point[j].x, &Course[c].Point[j].y, &Course[c].Point[j].z
				, &Course[c].Dir[j].x, &Course[c].Dir[j].y, &Course[c].Dir[j].z, &Course[c].Scale[j]);
			j++;
		} while (Course[c].Point[j - 1].x > -9999);
		Course[c].Count = j;
		c++;
	}
	fclose(fp);
	CourseCount = c;
	GameTime = 0;
	return landfilename;
}
//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitDeviceObjects()
{
	// TODO: create device objects
	HRESULT hr;

	SetCurrentDirectory(ResourceDir);
	// Init the font
	m_pFont->InitDeviceObjects(m_pd3dDevice);
	m_pFontL->InitDeviceObjects(m_pd3dDevice);
	m_pFont3D->InitDeviceObjects(m_pd3dDevice);
	LOGFONT lfFont;
	lfFont.lfHeight = 12;
	lfFont.lfWidth = lfFont.lfEscapement =
		lfFont.lfOrientation = 0;
	lfFont.lfWeight = FW_BOLD;
	lfFont.lfItalic = 0;
	lfFont.lfUnderline = 0;
	lfFont.lfStrikeOut = FALSE;
	lfFont.lfCharSet = DEFAULT_CHARSET;
	lfFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfFont.lfQuality = DEFAULT_QUALITY;
	lfFont.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lfFont.lfFaceName[0] = '\0';
	/*
HFONT hFont = CreateFont(
		12,                // 高さ。0でデフォルト。
		0,                 // 平均文字幅。0で自動調整。
		0,                 // 文字送りの方向とX軸との角度
		0,                 // 各文字のベースラインとX軸との角度
		1000,                 // 太さを0から1000までの値で指定。0でデフォルト。
		0,                 // イタリック体かどうか。TRUEでイタリック体。
		0,                 // 下線付きかどうか。TRUEで下線付き。
		0,                 // 打ち消し線付きかどうか。TRUEで打ち消し線付き。
		SHIFTJIS_CHARSET,  // 文字セット
		0,                 // 出力精度
		0,                 // クリッピング精度
		0,                 // 出力品質
		0,                 // ピッチとファミリ。
		"ANSI_FIXED_FONT" );            // NULLで終わる文字列へのポインタ
	*/
	//	HFONT hFont = (HFONT) GetStockObject(OEM_FIXED_FONT);
	HFONT hFont = CreateFontIndirect(&lfFont);
	D3DXCreateFont(m_pd3dDevice, hFont, &g_pFont);
	DeleteObject(hFont);
	// Create a teapot mesh using D3DX

	//**********
	// Load the skybox
	if (FAILED(hr = m_pSkyMesh->Create(m_pd3dDevice, _T("SkyBox.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[0]->Create(m_pd3dDevice, _T("Core.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[1]->Create(m_pd3dDevice, _T("Chip.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[2]->Create(m_pd3dDevice, _T("Wheel.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[3]->Create(m_pd3dDevice, _T("NWheel.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[4]->Create(m_pd3dDevice, _T("Rudder.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[5]->Create(m_pd3dDevice, _T("Trim.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[6]->Create(m_pd3dDevice, _T("Dummy.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[7]->Create(m_pd3dDevice, _T("Chip2.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[8]->Create(m_pd3dDevice, _T("Wheel2.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[9]->Create(m_pd3dDevice, _T("NWheel2.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[10]->Create(m_pd3dDevice, _T("Jet.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[11]->Create(m_pd3dDevice, _T("Fire.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[12]->Create(m_pd3dDevice, _T("Compus.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[13]->Create(m_pd3dDevice, _T("Compus2.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[14]->Create(m_pd3dDevice, _T("water.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[15]->Create(m_pd3dDevice, _T("CheckPoint.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[16]->Create(m_pd3dDevice, _T("RudderF.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[17]->Create(m_pd3dDevice, _T("TrimF.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[18]->Create(m_pd3dDevice, _T("Dust.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[19]->Create(m_pd3dDevice, _T("Splash.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[20]->Create(m_pd3dDevice, _T("Ball.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[21]->Create(m_pd3dDevice, _T("ChipH.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[22]->Create(m_pd3dDevice, _T("WheelT.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[23]->Create(m_pd3dDevice, _T("Type0.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[24]->Create(m_pd3dDevice, _T("Type1.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[25]->Create(m_pd3dDevice, _T("Type2.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[26]->Create(m_pd3dDevice, _T("Type3.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[27]->Create(m_pd3dDevice, _T("Type4.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[28]->Create(m_pd3dDevice, _T("Type5.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[29]->Create(m_pd3dDevice, _T("JetEffect.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[30]->Create(m_pd3dDevice, _T("ARM.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[31]->Create(m_pd3dDevice, _T("Fire2.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[32]->Create(m_pd3dDevice, _T("Bullet.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[33]->Create(m_pd3dDevice, _T("Jet2.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[34]->Create(m_pd3dDevice, _T("user.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[35]->Create(m_pd3dDevice, _T("Explosion.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[36]->Create(m_pd3dDevice, _T("Explosion2.X")))) return E_FAIL;
	if (FAILED(hr = m_pXMesh[37]->Create(m_pd3dDevice, _T("userArm.X")))) return E_FAIL;

	if (FAILED(hr = LoadLand(m_pd3dDevice, szLandFileName))) return E_FAIL;
	for (int i = 0;i < g_VarCount;i++) {
		g_ValList[i].Val = g_ValList[i].Def;
		if (g_ValList[i].Val > g_ValList[i].Max) g_ValList[i].Val = g_ValList[i].Max;
		if (g_ValList[i].Val < g_ValList[i].Min) g_ValList[i].Val = g_ValList[i].Min;
		g_ValList[i].Updated = true;
		for (int j = 0;j < g_ValList[i].RefCount;j++) {
			if (g_ValList[i].Flag[j])
				*(g_ValList[i].Ref[j]) = -g_ValList[i].Val;
			else *(g_ValList[i].Ref[j]) = g_ValList[i].Val;
		}
	}
	for (int c = 0;c < g_World->getChipCount();c++) {
		if (g_Chip[c]->ChipType == 0) {
			g_Chip[c]->X.y = g_World->Land->GetY(0, 0);
			float a = -(GVector(0, 0, 1)*g_Chip[c]->R).Cut2(GVector(0, 1, 0)).angle2(GVector(0, 0, 1), GVector(0, 1, 0));
			if (g_Chip[c]->X.y <= -100000.0f)g_Chip[c]->X.y = 0.0f;
			ResetChip(c, a);
		}
	}
	ClearInput(&m_UserInput);

	SourceDlg = NULL;
	ExtraDlg = NULL;
	NetworkDlg = NULL;
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Restores scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RestoreDeviceObjects()
{
	// TODO: setup render states
	HRESULT hr;
	SetCurrentDirectory(ResourceDir);

	//	SAFE_RELEASE(pPointVB);
	SAFE_RELEASE(pPointVB);
	//	m_pd3dDevice->CreateVertexBuffer(sizeof(D3DPOINTVERTEX) * GPARTMAX,0  ,D3DFVF_POINTVERTEX,D3DPOOL_DEFAULT,&pPointVB);
	m_pd3dDevice->CreateVertexBuffer(sizeof(D3DPOINTVERTEX) * 100, D3DUSAGE_POINTS, D3DFVF_POINTVERTEX, D3DPOOL_DEFAULT, &pPointVB);
	SAFE_RELEASE(pPointTexture);
	//	D3DXCreateTextureFromFile(m_pd3dDevice,"dustw.png",&pPointTexture);
		// Setup a material
	D3DMATERIAL8 mtrl;
	D3DUtil_InitMaterial(mtrl, 1.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetMaterial(&mtrl);


	// Set up the textures
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);

	// Set miscellaneous render states
	m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x000F0F0F);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// Set the world matrix
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matIdentity);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vFromPt = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&GMatView, &vFromPt, &vLookatPt, &vUpVec);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &GMatView);

	// Set the projection matrix
	D3DXMATRIX matProj;
	FLOAT fAspect = ((FLOAT)m_d3dsdBackBuffer.Width) / m_d3dsdBackBuffer.Height;
	D3DXMatrixPerspectiveFovLH(&matProj, Zoom*(GFloat)M_PI / 180.0f, fAspect, 1.0f, g_FarMax);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	// Set up lighting states
	D3DUtil_InitLight(light, D3DLIGHT_DIRECTIONAL, 0.0f, -1.0f, -0.5f);
	light.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.3f);
	light.Diffuse = D3DXCOLOR(lightColor.x, lightColor.y, lightColor.z, 1.0f);
	light.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	m_pd3dDevice->SetLight(0, &light);
	m_pd3dDevice->LightEnable(0, TRUE);
	//    D3DUtil_InitLight( light1, D3DLIGHT_POINT, 0.0f, 10.0f, 0.0f );
	//	light1.Specular=D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	//	light1.Diffuse=D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	//	light1.Ambient=D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	//	light1.Range       = 70.0f;
	// 	light1.Attenuation0=0.1f;
	//	light1.Attenuation1=0.1f;
	//	light1.Attenuation2=0.0f; 
	//    m_pd3dDevice->SetLight( 1, &light1 );
	//    m_pd3dDevice->LightEnable( 1, FALSE );
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);


	g_pFont->OnResetDevice();

	//**********
	int i;
	for (i = 0;i < GMODELMAX;i++) if (m_pXMesh[i]) m_pXMesh[i]->RestoreDeviceObjects(m_pd3dDevice);
	m_pSkyMesh->RestoreDeviceObjects(m_pd3dDevice);
	g_pLandMesh->RestoreDeviceObjects(m_pd3dDevice);
	SAFE_RELEASE(pSprite);
	D3DXCreateSprite(m_pd3dDevice, &pSprite);

	//	for(i=0;i<GTEXMAX;i++) pMyTexture[i]->RestoreDeviceObjects(m_pd3dDevice);
	for (i = 0;i < GTEXMAX;i++) { SAFE_RELEASE(pMyTexture[i]); }
	//テクスチャの読み込み
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Meter1.png", &pMyTexture[0]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Meter2.png", &pMyTexture[1]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Meter3.png", &pMyTexture[2]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Meter4.png", &pMyTexture[3]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Meter5.png", &pMyTexture[4]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Stick1.png", &pMyTexture[5]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Stick2.png", &pMyTexture[6]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Stick3.png", &pMyTexture[7]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Stick4.png", &pMyTexture[8]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Stick5.png", &pMyTexture[9]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Stick6.png", &pMyTexture[10]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Stick7.png", &pMyTexture[11]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Stick8.png", &pMyTexture[12]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Stick9.png", &pMyTexture[13]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Stick10.png", &pMyTexture[14]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Title.png", &pMyTexture[15]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "State.png", &pMyTexture[16]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "State2.png", &pMyTexture[17]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "ViewState.png", &pMyTexture[18]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "Edge.png", &pMyTexture[19]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "CCD.png", &pMyTexture[20]))) return  E_FAIL;
	if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, "CheckPoint.png", &pMyTexture[21]))) return  E_FAIL;
	// Restore the font
	m_pFont->RestoreDeviceObjects();
	m_pFontL->RestoreDeviceObjects();
	m_pFont3D->RestoreDeviceObjects();



	if (!m_bWindowed)
	{
		// Create a surface for configuring DInput devices
		if (FAILED(hr = m_pd3dDevice->CreateImageSurface(640, 480,
			m_d3dsdBackBuffer.Format, &m_pDIConfigSurface)))
			return DXTRACE_ERR_NOMSGBOX("CreateImageSurface", hr);
	}
	g_World->MainStepCount = 0;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: StaticConfigureInputDevicesCB()
// Desc: Static callback helper to call into CMyD3DApplication class
//-----------------------------------------------------------------------------
BOOL CALLBACK CMyD3DApplication::StaticConfigureInputDevicesCB(
	IUnknown* pUnknown, VOID* pUserData)
{
	CMyD3DApplication* pApp = (CMyD3DApplication*)pUserData;
	return pApp->ConfigureInputDevicesCB(pUnknown);
}




//-----------------------------------------------------------------------------
// Name: ConfigureInputDevicesCB()
// Desc: Callback function for configuring input devices. This function is
//       called in fullscreen modes, so that the input device configuration
//       window can update the screen.
//-----------------------------------------------------------------------------
BOOL CMyD3DApplication::ConfigureInputDevicesCB(IUnknown* pUnknown)
{
	// Get access to the surface
	LPDIRECT3DSURFACE8 pConfigSurface = NULL;
	if (FAILED(pUnknown->QueryInterface(IID_IDirect3DSurface8,
		(VOID**)&pConfigSurface)))
		return TRUE;

	// Render the scene, with the config surface blitted on top
	Render();

	RECT  rcSrc;
	SetRect(&rcSrc, 0, 0, 640, 480);

	POINT ptDst;
	ptDst.x = (m_d3dsdBackBuffer.Width - 640) / 2;
	ptDst.y = (m_d3dsdBackBuffer.Height - 480) / 2;

	LPDIRECT3DSURFACE8 pBackBuffer;
	m_pd3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pd3dDevice->CopyRects(pConfigSurface, &rcSrc, 1, pBackBuffer, &ptDst);
	pBackBuffer->Release();

	// Present the backbuffer contents to the front buffer
	m_pd3dDevice->Present(0, 0, 0, 0);

	// Release the surface
	pConfigSurface->Release();

	return TRUE;
}




HRESULT CMyD3DApplication::ResetChips(float x, float z, float a)
{
	KeyRecordMax = 0;
	KeyRecordCount = 0;
	waitCount = 0;
	for (int c = 0;c < g_World->getChipCount();c++) {
		g_Chip[c]->Crush = false;
		if (g_Chip[c]->ChipType == 0) {
			if (g_pLandMesh) {
				g_Chip[c]->X = GVector(x, 0, z);
				g_Chip[c]->X.y = g_World->Land->GetY(x, z);
			}
			if (g_Chip[c]->X.y <= -100000.0f)g_Chip[c]->X.y = 0.0f;
			ResetChip(c, a);
		}
	}
	if (g_ScriptLua) {
		//Luaリセット
		luaScriptEnd(g_ScriptLua);
		g_ScriptLua = NULL;
	}
	if (ScriptType == 1 && ScriptSource[0] != '\0') g_ScriptLua = luaScriptInit(ScriptSource);

	g_GroundParticle->Clear();
	g_WaterLineParticle->Clear();
	g_JetParticle->Clear();
	g_Bullet->Clear();

	LockGravityFlag = FALSE;
	LockAirFlag = FALSE;
	LockTorqueFlag = FALSE;
	LockJetFlag = TRUE;
	LockUnbreakableFlag = FALSE;
	LockScriptFlag = FALSE;
	LockCCDFlag = FALSE;
	LockEnergyFlag = FALSE;

	return 0;
}
HRESULT CMyD3DApplication::ResetChips(float y, float a)
{
	KeyRecordMax = 0;
	KeyRecordCount = 0;
	waitCount = 0;
	LastBye = 0;
	for (int c = 0;c < g_World->getChipCount();c++) {
		g_Chip[c]->Crush = false;
		if (g_Chip[c]->ChipType == 0) {
			g_Chip[c]->X.y = y;
			ResetChip2(c, a);
		}
	}
	if (g_ScriptLua) {
		//Luaリセット
		luaScriptEnd(g_ScriptLua);
		g_ScriptLua = NULL;
	}
	if (ScriptType == 1 && ScriptSource[0] != '\0') g_ScriptLua = luaScriptInit(ScriptSource);
	g_GroundParticle->Clear();
	g_WaterLineParticle->Clear();
	g_JetParticle->Clear();
	g_Bullet->Clear();

	if (g_SystemLua != NULL && (g_World->B26Bullet || g_DPlay->GetNumPlayers() == 0)) luaSystemRun("OnReset");
	if (ScriptType == 1 && g_ScriptLua != NULL) luaScriptRun(g_ScriptLua, "OnReset");
	return 0;
}
HRESULT CMyD3DApplication::InitChips(float a, int hereFlag)
{
	KeyRecordMax = 0;
	KeyRecordCount = 0;
	LastBye = 0;
	waitCount = 0;
	if (hereFlag == 0) {
		for (int c = 0;c < g_World->getChipCount();c++) {
			g_Chip[c]->Crush = false;
			g_Chip[c]->Fuel = g_Chip[c]->FuelMax;
			if (g_Chip[c]->ChipType == 0) {
				g_Chip[c]->X = GVector(0, 0, 0);
				g_Chip[c]->X.y = g_World->Land->GetY(0, 0);
				if (g_Chip[c]->X.y <= -100000.0f)g_Chip[c]->X.y = 0.0f;
				ResetChip(c, a);
			}
		}
	}
	if (g_ScriptLua) {
		//Luaリセット
		luaScriptEnd(g_ScriptLua);
		g_ScriptLua = NULL;
	}
	if (ScriptType == 1 && ScriptSource[0] != '\0') g_ScriptLua = luaScriptInit(ScriptSource);
	g_GroundParticle->Clear();
	g_WaterLineParticle->Clear();
	g_JetParticle->Clear();

	g_Bullet->Clear();
	LockGravityFlag = FALSE;
	LockAirFlag = FALSE;
	LockTorqueFlag = FALSE;
	LockJetFlag = TRUE;
	LockUnbreakableFlag = FALSE;
	LockScriptFlag = FALSE;
	LockCCDFlag = FALSE;
	LockEnergyFlag = FALSE;

	g_World->DeleteObjects();
	for (int i = 0;i < GRINGMAX;i++) {
		Ring[i].Color = GVector(0, 0.3f, 1);
		Ring[i].State = 0;
		Ring[i].Dir = GVector(0, 0, 0);
		Ring[i].Point = GVector(0, 0, 0);
		Ring[i].Scale = 10.0f;
	}
	AirSpeed.clear();
	srand(0);
	g_World->MainStepCount = 0;
	CompassTarget.y = -100000.0;
	if (g_SystemLua != NULL && (g_World->B26Bullet || g_DPlay->GetNumPlayers() == 0)) luaSystemRun("OnInit");
	if (ScriptType == 1 && g_ScriptLua != NULL) luaScriptRun(g_ScriptLua, "OnInit");
	return 0;
}
//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FrameMove()
{
	static DWORD lastT = 0;
	static DWORD lastT2 = 0;
	static DWORD lastT3 = 0;
	static int soundCount = 0;
	static GFloat a = 0.0f;
	static int count = 0;
	static GFloat preY = 0.0;
	static int preSound = false;
	int i, j, k;
	POINT pos;
	DWORD t = timeGetTime();
	//録画開始
	if (!MsgFlag && RecState > 0) {
		if (RecState == 1) {
			RecState = 0;
			KeyRecordMode = 1;
			recGravityFlag = GravityFlag;
			recAirFlag = AirFlag;
			recTorqueFlag = TorqueFlag;
			recJetFlag = JetFlag;
			recUnbreakableFlag = UnbreakableFlag;
			recCCDFlag = CCDFlag;
			recScriptFlag = ScriptFlag;
			recEnergyFlag = EfficientFlag;
			KeyRecordMax = 0;
			KeyRecordCount = 0;
			CopyChip(g_World->RecRigid, g_World->Rigid);
			CopyObject(g_World->RecObject, g_World->Object);
			RecVal();
			RecCheckPoint = CurrentCheckPoint;
			RecGameTime = GameTime;
			RecLastBye = LastBye;
			RecWaitCount = waitCount;
		}
		//録画停止
		else if (RecState == 2) {
			RecState = 0;
			//レコードモードなら
			if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
			KeyRecordMode = 0;
			//ストップモードにする
		}
		//録画再生
		else if (RecState == 3) {
			if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
			if (KeyRecordMax > 0) {
				RecState = 0;
				LastBye = RecLastBye;
				PlayLog();
				KeyRecordMode = 2;
			}
			else KeyRecordMode = 0;
		}
		g_World->MainStepCount = 0;
		g_World->Alt = false;
		g_World->InitRndTable();
		MySrand(0);
	}

	if (ShowNetwork)
	{
		if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
		KeyRecordMode = 0;
		ShowNetwork = FALSE;
		Pause(TRUE);
		int win = m_bWindowed;
		if (m_bWindowed == FALSE)
		{
			if (FAILED(ToggleFullscreen()))
			{
				DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
				return 1;
			}
			// Prompt the user to change the mode
			InvalidateRect(g_hWnd, NULL, NULL);
			Resize3DEnvironment();
		}
		if (NetworkDlg == NULL) {
			NetworkDlg = CreateDialog((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_NETDIALOG), NULL, DlgNetworkProc);
		}

		ShowWindow(NetworkDlg, SW_SHOW);
		SetFocus(NetworkDlg);
		SetFocus(GetDlgItem(NetworkDlg, IDC_CHATEDIT));
		ClearInput(&m_UserInput);

		/*
		if( win == FALSE )
				{
					if( FAILED( ToggleFullscreen() ) )
					{
						DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
					}
				}
		*/
		Pause(FALSE);
	}
	//g_FPSの監視
	static int netON = 0;
	static int saveg_LimitFPS = 30;
	if (NetworkDlg) {
		if (g_DPlay->GetLocalPlayerDPNID() == 0) {
			//g_FPSを元に戻す
			if (netON) {
				m_dLimidFPS = saveg_LimitFPS;
				if (m_dLimidFPS == (1000 / 15)) {
					CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT15, MF_CHECKED);
					CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT30, MF_UNCHECKED);
					g_LimitFPS = 15;
				}
				else if (m_dLimidFPS == (1000 / 30)) {
					CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT30, MF_CHECKED);
					CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT15, MF_UNCHECKED);
					g_LimitFPS = 30;
				}
				else {
					CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT30, MF_UNCHECKED);
					CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT15, MF_UNCHECKED);
					g_LimitFPS = 30;
				}
			}
			netON = 0;
		}
		else {
			if (netON == 0 || m_dLimidFPS == 0) {
				saveg_LimitFPS = m_dLimidFPS;
				if (m_dLimidFPS == (1000 / 15)) {
					CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT15, MF_CHECKED);
					CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT30, MF_UNCHECKED);
					g_LimitFPS = 15;
				}
				else {
					CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT15, MF_UNCHECKED);
					CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT30, MF_CHECKED);
					g_LimitFPS = 30;
					m_dLimidFPS = 1000L / 30;
				}
			}
			netON = 1;
		}
	}
	//情報更新
	if (t - lastT3 > 1000) {
		lastT3 = t;
		if (NetworkDlg) {
			char str[100];
			int n = g_DPlay->GetNumPlayers();
			if (g_DPlay->GetLocalPlayerDPNID() == 0) {
				sprintf(str, "Push 'Start' to Hosting or Connecting");
				//g_FPSを元に戻す
				if (netON) {
					m_dLimidFPS = saveg_LimitFPS;
					if (m_dLimidFPS == (1000 / 15)) {
						CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT15, MF_CHECKED);
						CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT30, MF_UNCHECKED);
						g_LimitFPS = 15;
					}
					else if (m_dLimidFPS == (1000 / 30)) {
						CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT30, MF_CHECKED);
						CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT15, MF_UNCHECKED);
						g_LimitFPS = 30;
					}
					else {
						CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT30, MF_UNCHECKED);
						CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT15, MF_UNCHECKED);
						g_LimitFPS = 30;
					}
				}
				netON = 0;
			}
			else {
				if (g_DPlay->GetHostPlayerDPNID() == g_DPlay->GetLocalPlayerDPNID())
					sprintf(str, "Hosting. %d players in this session.", n);
				else sprintf(str, "Connecting. %d players in this session.", n);
				if (netON == 0 || m_dLimidFPS == 0) {
					saveg_LimitFPS = m_dLimidFPS;
					if (m_dLimidFPS == (1000 / 15)) {
						CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT15, MF_CHECKED);
						CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT30, MF_UNCHECKED);
						g_LimitFPS = 15;
					}
					else {
						CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT15, MF_UNCHECKED);
						CheckMenuItem(GetMenu(m_hWnd), IDM_LIMIT30, MF_CHECKED);
						g_LimitFPS = 30;
						m_dLimidFPS = 1000L / 30;
					}
				}
				netON = 1;
			}
			SendDlgItemMessage(NetworkDlg, IDC_STATICMES, WM_SETTEXT, 0, (LPARAM)str);
			if (g_DPlay->GetLocalPlayerDPNID() != 0) {
				EnableWindow(GetDlgItem(NetworkDlg, IDC_HOSTRADIO), FALSE);
				EnableWindow(GetDlgItem(NetworkDlg, IDC_CONNECTRADIO), FALSE);
				SendMessage(GetDlgItem(NetworkDlg, IDC_SESSIONEDIT), EM_SETREADONLY, 1, 0);
				SendMessage(GetDlgItem(NetworkDlg, IDC_USEREDIT), EM_SETREADONLY, 1, 0);
				SendMessage(GetDlgItem(NetworkDlg, IDC_HOSTEDIT), EM_SETREADONLY, 1, 0);
				SendMessage(GetDlgItem(NetworkDlg, IDC_PORTEDIT), EM_SETREADONLY, 1, 0);
				SendDlgItemMessage(NetworkDlg, IDC_STARTBUTTON, WM_SETTEXT, 0, (LPARAM)"End");
				EnableWindow(GetDlgItem(NetworkDlg, IDC_SEND), TRUE);
				EnableWindow(GetDlgItem(NetworkDlg, IDC_COLORBUTTON), FALSE);
				SendMessage(GetDlgItem(NetworkDlg, IDC_CHATEDIT), EM_SETREADONLY, 0, 0);
				g_DPlay->SetConnect(TRUE);
				g_DPlay->GetSessionName(str);
				SendMessage(GetDlgItem(NetworkDlg, IDC_SESSIONEDIT), WM_SETTEXT, 0, (LPARAM)str);
			}
			PlayersInfoDisp();
		}
	}
	if (g_World->NetStop) return S_OK;

	CtrlKey = GetAsyncKeyState(VK_CONTROL) != 0;
	MouseL = GetAsyncKeyState(VK_LBUTTON) != 0;
	MouseR = GetAsyncKeyState(VK_RBUTTON) != 0;
	MouseM = GetAsyncKeyState(VK_MBUTTON) != 0;
	GetCursorPos(&pos);
	ScreenToClient(m_hWnd, &pos);
	MouseX = pos.x;
	MouseY = pos.y;
	bool tempMoveEnd = MoveEnd;

	//ネットデータ送信0 Arm弾の情報を送る
	if (g_DPlay->GetNumPlayers() > 0 && t - lastT2 > (DWORD)GNETSPAN / 3 && tempMoveEnd) {
		MoveEnd = false;
		GBULLESTREAM stream;
		if (g_World->B26Bullet) stream.code = 31;
		else if (g_World->B20Bullet) stream.code = 11;
		else stream.code = 21;
		j = 0;
		for (i = 0;i < g_Bullet->MaxVertex;i++) {
			if (g_Bullet->Vertex[i].Net != 0) {
				float v = g_Bullet->Vertex[i].Vec.abs()*(g_Bullet->Vertex[i].Net - 1);
				stream.data[j].Dist = g_Bullet->Vertex[i].Dist2;
				stream.data[j].Pos = g_Bullet->Vertex[i].Pos2;
				stream.data[j].Power = g_Bullet->Vertex[i].Power;
				stream.data[j].Size = g_Bullet->Vertex[i].Size;
				stream.data[j].Tar = g_Bullet->Vertex[i].Tar;
				stream.data[j].Vec = g_Bullet->Vertex[i].Vec;
				g_Bullet->Vertex[i].Net = 0;
				j++;
				if (j >= GBULLETDATAMAX) break;
			}
		}

		if (j > 0) {
			DWORD mySize = j*sizeof(GBULLETDATA) + sizeof(short);
			g_DPlay->SendAll((BYTE*)&stream, mySize);
		}
	}

	//ネットデータ送信0 爆発の情報を送る
	if (g_DPlay->GetNumPlayers() > 0 && t - lastT2 > (DWORD)GNETSPAN / 3 && tempMoveEnd) {
		MoveEnd = false;
		if (g_World->B26Bullet) {
			GEXPSTREAM2 stream;
			stream.code = 32;
			j = 0;
			for (i = 0;i < g_JetParticle->MaxVertex;i++) {
				if (g_JetParticle->Vertex[i].Net != 0) {
					stream.data[j].Type = g_JetParticle->Vertex[i].Type;
					stream.data[j].Pos = g_JetParticle->Vertex[i].Pos;
					stream.data[j].Power = g_JetParticle->Vertex[i].Power;
					stream.data[j].dpnid = g_JetParticle->Vertex[i].dpnid;
					g_JetParticle->Vertex[i].Net = 0;

					j++;
					if (j >= GEXPDATAMAX) break;
				}
			}

			if (j > 0) {
				DWORD mySize = j*sizeof(GEXPDATA2) + sizeof(short);
				g_DPlay->SendAll((BYTE*)&stream, mySize);
			}
		}
		else if (g_World->B20Bullet) {
			GEXPSTREAM stream;
			stream.code = 12;
			j = 0;
			for (i = 0;i < g_JetParticle->MaxVertex;i++) {
				if (g_JetParticle->Vertex[i].Net != 0) {
					stream.data[j].Type = g_JetParticle->Vertex[i].Type;
					stream.data[j].Pos = g_JetParticle->Vertex[i].Pos;
					stream.data[j].Power = g_JetParticle->Vertex[i].Power;
					g_JetParticle->Vertex[i].Net = 0;
					j++;
					if (j >= GEXPDATAMAX) break;
				}
			}

			if (j > 0) {
				DWORD mySize = j*sizeof(GEXPDATA) + sizeof(short);
				g_DPlay->SendAll((BYTE*)&stream, mySize);
			}
		}
		else {
			GEXPSTREAM2 stream;
			stream.code = 22;
			j = 0;
			for (i = 0;i < g_JetParticle->MaxVertex;i++) {
				if (g_JetParticle->Vertex[i].Net != 0) {
					stream.data[j].Type = g_JetParticle->Vertex[i].Type;
					stream.data[j].Pos = g_JetParticle->Vertex[i].Pos;
					stream.data[j].Power = g_JetParticle->Vertex[i].Power;
					stream.data[j].dpnid = g_JetParticle->Vertex[i].dpnid;
					g_JetParticle->Vertex[i].Net = 0;

					j++;
					if (j >= GEXPDATAMAX) break;
				}
			}

			if (j > 0) {
				DWORD mySize = j*sizeof(GEXPDATA2) + sizeof(short);
				g_DPlay->SendAll((BYTE*)&stream, mySize);
			}
		}
	}
	//メッセージの送信
	if (g_DPlay->GetNumPlayers() > 0 && t - lastT > (DWORD)GNETSPAN && tempMoveEnd) {
		if (strlen(g_MessageData)) {
			GSTREAM stream;
			stream.code = 30;//シナリオメッセージ
			*((int*)stream.data) = scenarioCode;
			strcpy((char*)&stream.data[4], g_MessageData);
			DWORD size = (DWORD)(sizeof(int) + strlen(g_MessageData) + 1 + sizeof(short));
			g_DPlay->SendAll((BYTE*)&stream, size);
			g_MessageData[0] = '\0';
		}
	}

	//ネットデータ送信
	if (g_DPlay->GetNumPlayers() > 0 && t - lastT > (DWORD)GNETSPAN && tempMoveEnd) {
		MoveEnd = false;
		lastT = t;
		lastT2 = t;
		GCHIPSTREAM stream;
		stream.code = 0;
		i = 0;
		j = 0;
		do {
			if (g_World->Rigid[j]->Parent == NULL) {
				stream.data[i].id = j + 512;
				stream.data[i].data.f[0] = g_World->Rigid[j]->X.x;
				stream.data[i].data.f[1] = g_World->Rigid[j]->X.y;
				stream.data[i].data.f[2] = g_World->Rigid[j]->X.z;
				i++;
			}
			GRigid *r = g_World->Rigid[j];
			GVector p = r->X - r->Top->X;
			stream.data[i].id = r->Top->ID | (r->Dir << 12);
			stream.data[i].color = ((((int)r->Color) >> 16 >> 3) << 10) + ((((((int)r->Color) >> 8) & 0xff) >> 3) << 5) + ((((int)r->Color) & 0xff) >> 3);
			stream.data[i].data.type = r->ChipType;
			stream.data[i].data.option = (unsigned char)g_World->Rigid[j]->Option;
			if (r->ChipType == GT_WHEEL || r->ChipType == GT_RLW) {
				stream.data[i].data.option |= ((int)(g_World->Rigid[j]->Effect*6.3 + 0.5) << 2);
				if (r->LinkInfo && (r->W*(r->LinkInfo->Axis*r->R)).abs() > M_PI / g_World->StepTime / 3.0f)
					stream.data[i].data.type |= GT_OPTION1;
			}
			if (r->ChipType >= GT_CHIP2) {
				stream.data[i].data.option = g_World->Rigid[j]->Ghost;
			}
			if (r->ChipType == GT_ARM) {
				stream.data[i].data.option = (unsigned char)(g_World->Rigid[j]->Option / 5000.0f);
			}
			if (r->ChipType == GT_JET) {
				if (r->Option != 0) {
					stream.data[i].data.type |= GT_OPTION1;
					float f = (FLOAT)(pow((double)fabs((double)g_World->Rigid[j]->Power), 1.0 / 3.0) / 5.0);
					if (f < 0.5f) f = 0.5f;
					if (f > 25.0f) f = 25.0f;
					stream.data[i].data.option = (int)(f * 10 + 0.5);
				}
				else {
					float f = (float)fabs(r->Power / 2000.0);if (f > 2.5f) f = 2.5f;
					stream.data[i].data.option = (int)(f * 50 + 0.5f);
					if (r->Power < 0) stream.data[i].data.type |= GT_OPTION2;
				}
			}
			stream.data[i].data.pos.x = (short)(p.x * 100);
			stream.data[i].data.pos.y = (short)(p.y * 100);
			stream.data[i].data.pos.z = (short)(p.z * 100);
			if (r->ChipType == GT_COWL) {
				stream.data[i].data.option |= (((((int)g_World->Rigid[j]->Effect) & 0x08) >> 3) << 3);
				stream.data[i].data.option |= (((((int)g_World->Rigid[j]->Effect) & 0x0f00) >> 11) << 4);
				stream.data[i].data.option |= (((((int)g_World->Rigid[j]->Effect) & 0x0f000) >> 13) << 5);
				GQuat q = r->R.quat();
				stream.data[i].data.quat.x = (signed char)(q.x * 100);
				stream.data[i].data.quat.y = (signed char)(q.y * 100);
				stream.data[i].data.quat.z = (signed char)(q.z * 100);
				stream.data[i].data.quat.w = (signed char)(q.w * 100);
			}
			else {
				stream.data[i].data.quat.x = (signed char)(r->Q.x * 100);
				stream.data[i].data.quat.y = (signed char)(r->Q.y * 100);
				stream.data[i].data.quat.z = (signed char)(r->Q.z * 100);
				stream.data[i].data.quat.w = (signed char)(r->Q.w * 100);
			}
			i++;
			j++;
		} while (j < g_World->ChipCount);

		MyNetDataSize = i*sizeof(GCHIPDATA) + sizeof(short);
		g_DPlay->SendAll((BYTE*)&stream, MyNetDataSize);
	}
	//ネットデータ送信2 位置情報だけを送る
	if (g_DPlay->GetNumPlayers()>0 && t - lastT2 > (DWORD)GNETSPAN / 3 && MoveEnd) {
		lastT2 = t;
		GCHIPSTREAM stream;
		stream.code = 10;
		stream.data[0].id = 512;
		stream.data[0].data.f[0] = g_World->Rigid[0]->X.x;
		stream.data[0].data.f[1] = g_World->Rigid[0]->X.y;
		stream.data[0].data.f[2] = g_World->Rigid[0]->X.z;

		MyNetDataSize = sizeof(GCHIPDATA) + sizeof(short);//1Chip分だけ送る
		g_DPlay->SendAll((BYTE*)&stream, MyNetDataSize);

	}
	if (g_World->Stop == false && g_World->NetStop == false) {
		//チェックポイントのチェック
		if (CurrentCheckPoint < Course[CurrentCourse].Count && CurrentCheckPoint >= 0) {
			if ((Course[CurrentCourse].Point[CurrentCheckPoint] - g_Chip[0]->X).abs() < (Course[CurrentCourse].Scale[CurrentCheckPoint])) {
				if (CurrentCheckPoint == 0 && waitCount <= 0) {
					if (KeyRecordMode == 0) {
						GravityFlag = Course[CurrentCourse].GravityFlag;
						AirFlag = Course[CurrentCourse].AirFlag;
						TorqueFlag = Course[CurrentCourse].TorqueFlag;
						JetFlag = Course[CurrentCourse].JetFlag;
						UnbreakableFlag = Course[CurrentCourse].UnbreakableFlag;
						CCDFlag = Course[CurrentCourse].CCDFlag;
						ScriptFlag = Course[CurrentCourse].ScriptFlag;
						EfficientFlag = Course[CurrentCourse].EfficientFlag;

						SetRegulationMenu();
						RecCheckPoint = CurrentCheckPoint;
					}
					GameTime = 0;
					waitCount = 90;
				}
				CurrentCheckPoint++;
			}
		}
		if (CurrentCheckPoint > 0) {
			if (CurrentCheckPoint < Course[CurrentCourse].Count - 1) GameTime++;
		}
		else GameTime = -1;
		if (ResetCount >= 0) {
			ResetCount--;
			g_World->DestroyFlag = false;
		}
		else g_World->DestroyFlag = !UnbreakableFlag;
	}
	// Update user input state
	if (InputCancel == false) UpdateInput(&m_UserInput);else { DummyInput(&m_UserInput);InputCancel = false; }

	// Respond to input
	if (m_UserInput.bDoConfigureInput)
	{
		// One-shot per keypress
		m_UserInput.bDoConfigureInput = FALSE;

		Pause(TRUE);

		// Get access to the list of semantically-mapped input devices
		// to delete all InputDeviceState structs before calling ConfigureDevices()
		CInputDeviceManager::DeviceInfo* pDeviceInfos;
		DWORD dwNumDevices;
		m_pInputDeviceManager->GetDevices(&pDeviceInfos, &dwNumDevices);

		for (DWORD i = 0; i < dwNumDevices; i++)
		{
			InputDeviceState* pInputDeviceState = (InputDeviceState*)pDeviceInfos[i].pParam;
			SAFE_DELETE(pInputDeviceState);
			pDeviceInfos[i].pParam = NULL;
		}

		// Configure the devices (with edit capability)
		InvalidateRect(g_hWnd, NULL, NULL);
		if (m_bWindowed)
			m_pInputDeviceManager->ConfigureDevices(m_hWnd, NULL, NULL, DICD_EDIT, NULL);
		else
			m_pInputDeviceManager->ConfigureDevices(m_hWnd,
				m_pDIConfigSurface,
				(VOID*)StaticConfigureInputDevicesCB,
				DICD_EDIT, (LPVOID) this);

		Pause(FALSE);
	}

	if (m_UserInput.bDoConfigureDisplay)
	{
		// One-shot per keypress
		m_UserInput.bDoConfigureDisplay = FALSE;

		Pause(TRUE);

		// Configure the display device
		InvalidateRect(g_hWnd, NULL, NULL);
		UserSelectNewDevice();

		Pause(FALSE);
	}

	if (m_UserInput.bDoOpenChip)
	{
		if (ControlKeysLock[2] == false) {
			ClearInput(&m_UserInput);
			InputCancel = true;

			if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
			KeyRecordMode = 0;
			m_UserInput.bDoOpenChip = FALSE;
			Pause(TRUE);
			int win = m_bWindowed;
			if (m_bWindowed == FALSE)
			{
				if (FAILED(ToggleFullscreen()))
				{
					DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
					return 1;
				}
			}
			if (SetCurrentDirectory(CurrDataDir) == 0) {
				_tcscpy(CurrDataDir, DataDir);
			}
			char szFileName[512];
			char szFilter[] = "Text(*.txt or *.rcd)\0*.txt;*.rcd\0All files(*.*)\0*.*\0";
			OPENFILENAME ofn;
			szFileName[0] = '\0';
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = m_hWnd;
			ofn.lpstrFilter = szFilter;
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = 512;
			ofn.lpstrInitialDir = CurrDataDir;
			ofn.lpstrTitle = "Open Chips-Data";
			ofn.lpstrDefExt = "txt;RCD";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if (GetOpenFileName(&ofn)) {
				int errCode = 0;
				if ((errCode = LoadData(szFileName)) != 0) {
					char str[256];
					BlockErrStr(errCode, DataCheck, str);
					MessageBox(NULL, str, NULL, MB_ICONERROR | MB_OK);
				}
				float aa = g_Chip[0]->Fuel;
				if (g_DPlay->GetNumPlayers() > 0) {	//初期化を送信する
					GINFOSTREAM stream;
					stream.code = 100; //初期化
					g_MyPlayerData.init++;
					stream.data = g_MyPlayerData;
					DWORD sz = sizeof(GINFOSTREAM) + sizeof(short);//1Chip分だけ送る
					g_DPlay->SendAll((BYTE*)&stream, sz);
				}
			}
			if (win == FALSE)
			{
				if (FAILED(ToggleFullscreen()))
				{
					DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
				}
				Resize3DEnvironment();
			}
			Pause(FALSE);
		}
		else {
			m_UserInput.bDoOpenChip = FALSE;
			ClearInput(&m_UserInput);
		}
	}

	if (m_UserInput.bDoUpdateChip) {
		if (ControlKeysLock[3] == false) {
			float a = -(GVector(0, 0, 1)*g_Chip[0]->R).Cut2(GVector(0, 1, 0)).angle2(GVector(0, 0, 1), GVector(0, 1, 0));
			ClearInput(&m_UserInput);
			InputCancel = true;
			if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
			KeyRecordMode = 0;
			m_UserInput.bDoUpdateChip = FALSE;
			Pause(TRUE);
			GFloat x = g_Chip[0]->X.x, z = g_Chip[0]->X.z;
			DataCheck = 0;
			int errCode = 0;
			if ((errCode = readData(szUpdateFileName, true)) == 0) {
				readData(szUpdateFileName, false);
				if (g_World->getChipCount() == 0) return S_OK;

				ResetChips(x, z, a);
				InitChips(a, 1);
				ClearInput(&m_UserInput);
				if (g_DPlay->GetNumPlayers() > 0) {	//初期化を送信する
					GINFOSTREAM stream;
					stream.code = 100; //初期化
					g_MyPlayerData.init++;
					stream.data = g_MyPlayerData;
					DWORD sz = sizeof(GINFOSTREAM) + sizeof(short);//1Chip分だけ送る
					g_DPlay->SendAll((BYTE*)&stream, sz);

				}
				//			if(g_SystemLua!=NULL) luaSystemRun("OnOpenChips");
			}
			else {
				char str[100];
				BlockErrStr(errCode, DataCheck, str);
				MessageBox(NULL, str, NULL, MB_ICONERROR | MB_OK);
				//				MessageBox(NULL,str,NULL,MB_OK|MB_ICONEXCLAMATION);
			}
			Pause(FALSE);
		}
		else {
			m_UserInput.bDoUpdateChip = FALSE;
			ClearInput(&m_UserInput);
		}
	}

	if (m_UserInput.bDoOpenLand)
	{
		if (ControlKeysLock[4] == false) {
			ClearInput(&m_UserInput);
			InputCancel = true;
			int win = m_bWindowed;
			if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
			KeyRecordMode = 0;
			m_UserInput.bDoOpenLand = FALSE;
			Pause(TRUE);
			if (m_bWindowed == FALSE)
			{
				if (FAILED(ToggleFullscreen()))
				{
					DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
					return 1;
				}
			}
			if (SetCurrentDirectory(CurrDataDir) == 0) {
				_tcscpy(CurrDataDir, DataDir);
			}
			char szFileName[512];
			char szFilter[] = "Data(*.x)\0*.x\0All files(*.*)\0*.*\0";
			OPENFILENAME ofn;
			szFileName[0] = '\0';
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = m_hWnd;
			ofn.lpstrFilter = szFilter;
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = szFileName;
			ofn.lpstrInitialDir = CurrDataDir;
			ofn.nMaxFile = 512;
			ofn.lpstrTitle = "Open Land-Data";
			ofn.lpstrDefExt = "x";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			if (GetOpenFileName(&ofn)) {
				if (LoadLand(m_pd3dDevice, szFileName) == 0) {
					char szDrive[_MAX_DRIVE + 1];	// ドライブ名格納領域 
					char szPath[_MAX_PATH + 1];	// パス名格納領域 
					char szTitle[_MAX_FNAME + 1];	// ファイルタイトル格納領域 
					char szExt[_MAX_EXT + 1];		// ファイル拡張子格納領域 

					// 絶対パスを分解 
					_splitpath(szFileName,
						szDrive, szPath,
						szTitle, szExt);
					lstrcpy(CurrDataDir, szDrive);
					lstrcat(CurrDataDir, szPath);
					lstrcpy(szLandFileName, szFileName);
					lstrcpy(szLandFileName0, szTitle);
					lstrcat(szLandFileName0, szExt);
					KeyRecordMax = 0;
					KeyRecordCount = 0;
					for (int i = 0;i < g_World->getChipCount();i++) {
						if (g_Chip[i]->ChipType == 0) {
							GFloat y = g_World->Land->GetY(0, 0);
							if (y < -9000.0f)y = 0.0f;
							g_Chip[i]->CalcTotalCenter();
							g_Chip[i]->X = GVector(0, g_Chip[i]->Top->TotalRadius * 2 + y, 0);
							g_Chip[i]->CalcTotalCenter();
							//g_Chip[i]->R=GMatrix33();
							g_World->RestoreLink(g_Chip[i], g_Chip[i]);
						}
					}
					ResetCount = 90;
					g_World->DestroyFlag = false;
					CourseCount = 0;
					CurrentCourse = 0;
				}
			}
			if (win == FALSE)
			{
				if (FAILED(ToggleFullscreen()))
				{
					DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
				}
			}
			Resize3DEnvironment();
			Pause(FALSE);
		}
		else {
			m_UserInput.bDoOpenLand = FALSE;
			ClearInput(&m_UserInput);
		}
	}
	if (m_UserInput.bDoOpenGame) {
		if (ControlKeysLock[5] == false) {

			ClearInput(&m_UserInput);
			InputCancel = true;
			int win = m_bWindowed;
			if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
			KeyRecordMode = 0;
			m_UserInput.bDoOpenGame = FALSE;
			Pause(TRUE);
			if (m_bWindowed == FALSE)
			{
				if (FAILED(ToggleFullscreen()))
				{
					DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
					return 1;
				}
			}
			if (SetCurrentDirectory(CurrDataDir) == 0) {
				_tcscpy(CurrDataDir, DataDir);
			}
			char *LandFile;
			char szFileName[512];
			char szFilter[] = "Game-Data(*.rcg)\0*.rcg\0All files(*.*)\0*.*\0";
			OPENFILENAME ofn;
			szFileName[0] = '\0';
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = m_hWnd;
			ofn.lpstrFilter = szFilter;
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = szFileName;
			ofn.lpstrInitialDir = CurrDataDir;
			ofn.nMaxFile = 512;
			ofn.lpstrTitle = "Open Game-Data";
			ofn.lpstrDefExt = "rcg";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			if (GetOpenFileName(&ofn)) {
				LandFile = LoadGame(szFileName);
				if (LandFile) {
					char szDrive[_MAX_DRIVE + 1];	// ドライブ名格納領域 
					char szPath[_MAX_PATH + 1];	// パス名格納領域 
					char szTitle[_MAX_FNAME + 1];	// ファイルタイトル格納領域 
					char szExt[_MAX_EXT + 1];		// ファイル拡張子格納領域 

					// 絶対パスを分解 
					_splitpath(LandFile,
						szDrive, szPath,
						szTitle, szExt);

					lstrcpy(CurrDataDir, szDrive);
					lstrcat(CurrDataDir, szPath);

					lstrcpy(szLandFileName, DataDir);
					lstrcat(szLandFileName, TEXT("\\"));
					lstrcat(szLandFileName, LandFile);
					lstrcpy(szLandFileName0, szTitle);
					lstrcat(szLandFileName0, szExt);
				}
				InitChips(0, 0);
				ClearInput(&m_UserInput);
				CurrentCourse = 1;
				CurrentCheckPoint = 0;
			}
			if (win == FALSE)
			{
				if (FAILED(ToggleFullscreen()))
				{
					DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
				}
				Resize3DEnvironment();
			}
			Pause(FALSE);
		}
		else {
			m_UserInput.bDoOpenGame = FALSE;
			ClearInput(&m_UserInput);
		}
	}
	if (m_UserInput.bDoCloseScenario)
	{
		float a = -(GVector(0, 0, 1)*g_Chip[0]->R).Cut2(GVector(0, 1, 0)).angle2(GVector(0, 0, 1), GVector(0, 1, 0));
		InputCancel = true;
		m_UserInput.bDoCloseScenario = FALSE;

		char fn[_MAX_PATH];
		lstrcpy(fn, ResourceDir);
		lstrcat(fn, TEXT("\\System.rcs"));
		LoadSystem(fn);
		luaSystemInit();
		InitChips(a, 1);
		ClearInput(&m_UserInput);
		if (g_DPlay->GetNumPlayers() > 0) {	//初期化を送信する
			GINFOSTREAM stream;
			stream.code = 100; //初期化
			g_MyPlayerData.scenarioCode = scenarioCode;
			stream.data = g_MyPlayerData;
			DWORD sz = sizeof(GINFOSTREAM) + sizeof(short);//1Chip分だけ送る
			g_DPlay->SendAll((BYTE*)&stream, sz);
		}
	}
	if (m_UserInput.bDoOpenScenario)
	{
		float a = -(GVector(0, 0, 1)*g_Chip[0]->R).Cut2(GVector(0, 1, 0)).angle2(GVector(0, 0, 1), GVector(0, 1, 0));
		InputCancel = true;
		m_UserInput.bDoOpenScenario = FALSE;
		int win = m_bWindowed;
		if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
		KeyRecordMode = 0;
		Pause(TRUE);
		if (m_bWindowed == FALSE)
		{
			if (FAILED(ToggleFullscreen()))
			{
				DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
				return 1;
			}
		}
		if (SetCurrentDirectory(CurrDataDir) == 0) {
			_tcscpy(CurrDataDir, DataDir);
		}
		char szFileName[512];
		char szFilter[] = "Scenario-Data(*.rcs)\0*.rcs\0All files(*.*)\0*.*\0";
		OPENFILENAME ofn;
		szFileName[0] = '\0';
		memset(&ofn, 0, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFilter = szFilter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = szFileName;
		ofn.lpstrInitialDir = CurrDataDir;
		ofn.nMaxFile = 512;
		ofn.lpstrTitle = "Open Scenario-Data";
		ofn.lpstrDefExt = "rcs";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		if (GetOpenFileName(&ofn)) {
			LoadSystem(szFileName);
			char szDrive[_MAX_DRIVE + 1];	// ドライブ名格納領域 
			char szPath[_MAX_PATH + 1];	// パス名格納領域 
			char szTitle[_MAX_FNAME + 1];	// ファイルタイトル格納領域 
			char szExt[_MAX_EXT + 1];		// ファイル拡張子格納領域 

			// 絶対パスを分解 
			_splitpath(szFileName,
				szDrive, szPath,
				szTitle, szExt);

			lstrcpy(CurrDataDir, szDrive);
			lstrcat(CurrDataDir, szPath);

			_tcscpy(CurrScenarioDir, CurrDataDir);
			luaSystemInit();
			InitChips(a, 0);
			ClearInput(&m_UserInput);
			if (g_DPlay->GetNumPlayers() > 0) {	//初期化を送信する
				GINFOSTREAM stream;
				stream.code = 100; //初期化
				g_MyPlayerData.scenarioCode = scenarioCode;
				stream.data = g_MyPlayerData;
				DWORD sz = sizeof(GINFOSTREAM) + sizeof(short);//1Chip分だけ送る
				g_DPlay->SendAll((BYTE*)&stream, sz);
			}
		}
		if (win == FALSE)
		{
			if (FAILED(ToggleFullscreen()))
			{
				DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
			}
			Resize3DEnvironment();
		}
		Pause(FALSE);
	}

	if (m_UserInput.bDoSaveLog)
	{
		ClearInput(&m_UserInput);
		InputCancel = true;
		int win = m_bWindowed;
		if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
		KeyRecordMode = 0;
		m_UserInput.bDoSaveLog = FALSE;
		Pause(TRUE);
		if (m_bWindowed == FALSE)
		{
			if (FAILED(ToggleFullscreen()))
			{
				DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
				return 1;
			}
		}
		if (SetCurrentDirectory(CurrDataDir) == 0) {
			_tcscpy(CurrDataDir, DataDir);
		}
		char szFileName[512];
		char szFilter[] = "Logdata(*.rcl)\0*.rcl\0All files(*.*)\0*.*\0";
		OPENFILENAME ofn;
		szFileName[0] = '\0';
		memset(&ofn, 0, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFilter = szFilter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = szFileName;
		ofn.lpstrInitialDir = CurrDataDir;
		ofn.nMaxFile = 512;
		ofn.lpstrTitle = "Save Log";
		ofn.lpstrDefExt = "rcl";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if (GetSaveFileName(&ofn)) {
			char szDrive[_MAX_DRIVE + 1];	// ドライブ名格納領域 
			char szPath[_MAX_PATH + 1];	// パス名格納領域 
			char szTitle[_MAX_FNAME + 1];	// ファイルタイトル格納領域 
			char szExt[_MAX_EXT + 1];		// ファイル拡張子格納領域 

			// 絶対パスを分解 
			_splitpath(szFileName,
				szDrive, szPath,
				szTitle, szExt);

			lstrcpy(CurrDataDir, szDrive);
			lstrcat(CurrDataDir, szPath);
			SaveLog(szFileName);

		}
		if (win == FALSE)
		{
			if (FAILED(ToggleFullscreen()))
			{
				DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
			}
			Resize3DEnvironment();
		}
		Pause(FALSE);
	}


	if (m_UserInput.bDoOpenLog)
	{
		ClearInput(&m_UserInput);
		InputCancel = true;
		if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
		KeyRecordMode = 0;
		m_UserInput.bDoOpenLog = FALSE;
		Pause(TRUE);
		int win = m_bWindowed;
		if (m_bWindowed == FALSE)
		{
			if (FAILED(ToggleFullscreen()))
			{
				DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
				return 1;
			}
		}
		if (SetCurrentDirectory(CurrDataDir) == 0) {
			_tcscpy(CurrDataDir, DataDir);
		}
		char szFileName[512];
		char szFilter[] = "Logdata(*.rcl)\0*.rcl\0All files(*.*)\0*.*\0";
		OPENFILENAME ofn;
		szFileName[0] = '\0';
		memset(&ofn, 0, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFilter = szFilter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = 512;
		ofn.lpstrInitialDir = CurrDataDir;
		ofn.lpstrTitle = "Open Log-Data";
		ofn.lpstrDefExt = "rcl";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

		HRESULT r = GetOpenFileName(&ofn);
		if (r) {
			char szDrive[_MAX_DRIVE + 1];	// ドライブ名格納領域 
			char szPath[_MAX_PATH + 1];	// パス名格納領域 
			char szTitle[_MAX_FNAME + 1];	// ファイルタイトル格納領域 
			char szExt[_MAX_EXT + 1];		// ファイル拡張子格納領域 

			// 絶対パスを分解 
			_splitpath(szFileName,
				szDrive, szPath,
				szTitle, szExt);

			lstrcpy(CurrDataDir, szDrive);
			lstrcat(CurrDataDir, szPath);

			if (LoadLog(szFileName) == 0) {
				if (strcmp(szTempFileName0, szLandFileName0) != 0) {
					if (lstrcmp(szTempFileName0, TEXT("Land.x")) == 0) {
						lstrcpy(szLandFileName, ResourceDir);
					}
					else {
						lstrcpy(szLandFileName, CurrDataDir);
					}
					lstrcat(szLandFileName, TEXT("\\"));
					lstrcat(szLandFileName, szTempFileName0);
					lstrcpy(szLandFileName0, szTempFileName0);
					if (LoadLand(m_pd3dDevice, szLandFileName) != 0) KeyRecordMax = 0;
				}
				RecState = 3;
				Sleep(1000);
			}
			else {
				char str[256];
				sprintf(str, "Error  ");
				MessageBox(NULL, str, NULL, MB_ICONERROR | MB_OK);
			}
			Resize3DEnvironment();
		}
		Pause(FALSE);
		if (win == FALSE)
		{
			if (FAILED(ToggleFullscreen()))
			{
				DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
			}
		}
		return S_OK;
	}
	if (ShowData)
	{
		if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
		KeyRecordMode = 0;
		ShowData = FALSE;
		Pause(TRUE);
		int win = m_bWindowed;
		if (m_bWindowed == FALSE)
		{
			if (FAILED(ToggleFullscreen()))
			{
				DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
				return 1;
			}
		}
		// Prompt the user to change the mode
		if (SourceDlg == NULL) {
			SourceDlg = CreateDialog((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SOURCE), NULL, DlgDataProc);
		}
		if (win == FALSE)
		{
			if (FAILED(ToggleFullscreen()))
			{
				DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
			}
			Resize3DEnvironment();
		}
		Pause(FALSE);
	}

	if (ShowExtra)
	{
		if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
		KeyRecordMode = 0;
		ShowExtra = FALSE;
		Pause(TRUE);
		int win = m_bWindowed;
		if (m_bWindowed == FALSE)
		{
			if (FAILED(ToggleFullscreen()))
			{
				DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
				return 1;
			}
		}
		// Prompt the user to change the mode
		if (ExtraDlg == NULL) {
			ExtraDlg = CreateDialog((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SETTINGDIALOG), NULL, DlgExtraProc);
		}
		if (win == FALSE)
		{
			if (FAILED(ToggleFullscreen()))
			{
				DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
			}
			Resize3DEnvironment();
		}
		Pause(FALSE);
	}


	GFloat mass = g_Chip[0]->TotalMass*10.0f;
	if (m_UserInput.bButtonOneShotInit) {
		if (ControlKeysLock[0] == false) {
			if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
			KeyRecordMode = 0;
			a = 0.0f;
			count = 0;
			ResetChips(0, 0, a);
			InitChips(a, 0);
			//		BOOL f=m_UserInput.bButtonInit;
			//		ClearInput(&m_UserInput);
			//		m_UserInput.bButtonInit=f;
			preY = g_Chip[0]->X.y;
			if (g_DPlay->GetNumPlayers() > 0) {	//初期化を送信する
				GINFOSTREAM stream;
				stream.code = 100; //初期化
				g_MyPlayerData.init++;
				stream.data = g_MyPlayerData;
				DWORD sz = sizeof(GINFOSTREAM) + sizeof(short);//1Chip分だけ送る
				g_DPlay->SendAll((BYTE*)&stream, sz);

			}
		}

	}
	else if (m_UserInput.bButtonInit) {
		if (ControlKeysLock[0] == false) {
			if (count > 15) {
				a = a + (GFloat)(3.0f / 180.0f*M_PI);
				CurrentCourse = 0;
			}
			if (CurrentCourse > 0) {
				g_Chip[0]->X = Course[CurrentCourse].StartPoint;
				a = Course[CurrentCourse].StartAngleY / 180.0f*(GFloat)M_PI;
			}
			count++;
			g_Chip[0]->X.y = preY;
			//		InitChips(a);
			//		InitChips(a,0);
			ResetChips(0, 0, a);
			InitChips(a, 0);
			//		ClearInput(&m_UserInput);
			CurrentCheckPoint = 0;
			/*		ResetObject(0,0);
					g_World->Object[0]->X.x=g_Chip[0]->X.x+g_Chip[0]->Top->TotalRadius+1;
					g_World->Object[0]->X.y=g_Chip[0]->X.y+g_Chip[0]->Top->TotalRadius+1;
					g_World->Object[0]->X.z=g_Chip[0]->X.z;
					g_World->Object[0]->RSet();
			*/
			//		m_UserInput.bButtonInit=true;
		}
		else {
			if (g_SystemLua != NULL && (g_World->B26Bullet || g_DPlay->GetNumPlayers() == 0)) luaSystemRun("OnInit");
			if (ScriptType == 1 && g_ScriptLua != NULL) luaScriptRun(g_ScriptLua, "OnInit");
		}
	}
	if (m_UserInput.bButtonOneShotYForce) {
		if (ControlKeysLock[6] == false) {
			if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
			KeyRecordMode = 0;
			g_Chip[0]->P += GVector(0, mass, 0);
			ResetCount = 3;
			g_World->DestroyFlag = false;
			if (g_DPlay->GetNumPlayers() > 0) {	//初期化を送信する
				GINFOSTREAM stream;
				stream.code = 100; //初期化
				g_MyPlayerData.yforce++;
				stream.data = g_MyPlayerData;
				DWORD sz = sizeof(GINFOSTREAM) + sizeof(short);//1Chip分だけ送る
				g_DPlay->SendAll((BYTE*)&stream, sz);
			}
		}
	}
	if (m_UserInput.bButtonOneShotTitle) {
		if (ControlKeysLock[7] == false) {
			ShowTitle = !ShowTitle;
		}
	}
	if (m_UserInput.bButtonOneShotZoomIn) {
		Zoom = Zoom*0.8f;
		if (Zoom < 9.437184f) Zoom = 9.437184f;
	}
	if (m_UserInput.bButtonOneShotZoomOut) {
		Zoom = Zoom*1.25f;
		if (Zoom > 109.86328125f) Zoom = 109.86328125f;
	}
	if (m_UserInput.bButtonOneShotResetView) {
		Zoom = 45.0f;
		TurnLR = 0.0f;
		TurnUD = 0.0f;
	}
	if (m_UserInput.bButtonOneShotTurnLeft) {
		TurnLR += 10.0f*(float)M_PI / 180.0f;
	}
	if (m_UserInput.bButtonOneShotTurnRight) {
		TurnLR -= 10.0f*(float)M_PI / 180.0f;
	}
	if (m_UserInput.bButtonOneShotTurnUp) {
		TurnUD += 5.0f*(float)M_PI / 180.0f;
	}
	if (m_UserInput.bButtonOneShotTurnDown) {
		TurnUD -= 5.0f*(float)M_PI / 180.0f;
	}
	if (m_UserInput.bButtonOneShotReset) {
		if (ControlKeysLock[1] == false) {
			if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
			KeyRecordMode = 0;
			count = 0;
			LastBye = 0;
			for (int i = 0;i < g_VarCount;i++) {
				g_ValList[i].Val = g_ValList[i].Def;
				if (g_ValList[i].Val > g_ValList[i].Max) g_ValList[i].Val = g_ValList[i].Max;
				if (g_ValList[i].Val < g_ValList[i].Min) g_ValList[i].Val = g_ValList[i].Min;
				g_ValList[i].Updated = true;
				for (int j = 0;j < g_ValList[i].RefCount;j++) {
					if (g_ValList[i].Flag[j])
						*(g_ValList[i].Ref[j]) = -g_ValList[i].Val;
					else *(g_ValList[i].Ref[j]) = g_ValList[i].Val;
				}
			}
			for (int c = 0;c < g_World->getChipCount();c++) {
				if (g_Chip[c]->ChipType == 0) {
					a = -(GVector(0, 0, 1)*g_Chip[c]->R).Cut2(GVector(0, 1, 0)).angle2(GVector(0, 0, 1), GVector(0, 1, 0));
					ResetChip(c, a);
				}
			}
			if (g_ScriptLua) {
				//Luaリセット
				luaScriptEnd(g_ScriptLua);
				g_ScriptLua = luaScriptInit(ScriptSource);
			}
			g_GroundParticle->Clear();
			g_WaterLineParticle->Clear();
			g_JetParticle->Clear();
			g_Bullet->Clear();
			if (g_SystemLua != NULL && (g_World->B26Bullet || g_DPlay->GetNumPlayers() == 0)) luaSystemRun("OnReset");
			if (ScriptType == 1 && g_ScriptLua != NULL) luaScriptRun(g_ScriptLua, "OnReset");
			preY = g_Chip[0]->X.y;
			if (g_DPlay->GetNumPlayers() > 0) {	//初期化を送信する
				GINFOSTREAM stream;
				stream.code = 100; //初期化
				g_MyPlayerData.reset++;
				stream.data = g_MyPlayerData;
				DWORD sz = sizeof(GINFOSTREAM) + sizeof(short);
				g_DPlay->SendAll((BYTE*)&stream, sz);
			}
		}
		else {
			if (g_SystemLua != NULL && (g_World->B26Bullet || g_DPlay->GetNumPlayers() == 0)) luaSystemRun("OnReset");
			if (ScriptType == 1 && g_ScriptLua != NULL) luaScriptRun(g_ScriptLua, "OnReset");
		}
	}
	else if (m_UserInput.bButtonReset) {
		if (ControlKeysLock[1] == false) {
			if (count > 15) a = a + (GFloat)(3.0f / 180.0f*M_PI);
			count++;
			ResetChips(preY, a);
			BOOL f = m_UserInput.bButtonReset;
			ClearInput(&m_UserInput);
			m_UserInput.bButtonReset = f;
		}
	}
	for (i = 0;i < GSYSKEYMAX;i++) {
		if (SystemKeys[i] == false && m_UserInput.bSystem[i]) SystemKeysDown[i] = true;
		else  SystemKeysDown[i] = false;
		if (SystemKeys[i] == true && m_UserInput.bSystem[i] == false) SystemKeysUp[i] = true;
		else  SystemKeysUp[i] = false;
		SystemKeys[i] = m_UserInput.bSystem[i] != 0 ? true : false;
	}

	//変数の更新
	GFloat val;
	for (i = 0;i < GKEYMAX;i++) {
		int key = m_UserInput.bButton[i];
		if (KeyRecordMode == 1) KeyRecord[KeyRecordCount][i] = m_UserInput.bButton[i];
		else if (KeyRecordMode == 2) {
			if (key != 0) KeyRecordMode = 0;
			else key = KeyRecord[KeyRecordCount][i];
		}
		bool ks2 = g_KeyList[i].SPressed;
		g_KeyList[i].SPressed = key != 0 ? true : false;
		if (ks2 == false && key != 0) g_KeyList[i].SDown = true; else g_KeyList[i].SDown = false;
		if (ks2 == true && key == 0) g_KeyList[i].SUp = true;else g_KeyList[i].SUp = false;
		if (g_KeyList[i].Lock) {
			key = 0;
			g_KeyList[i].Down = 0;
			g_KeyList[i].Up = 0;
		}


		bool ks = g_KeyList[i].Pressed;
		if (g_KeyList[i].Lock) ks = false;
		g_KeyList[i].Pressed = key != 0 ? true : false;
		if (ks == false && key != 0) g_KeyList[i].Down = true; else g_KeyList[i].Down = false;
		if (ks == true && key == 0) g_KeyList[i].Up = true;else g_KeyList[i].Up = false;
		if (key) {
			for (j = 0;j < g_KeyList[i].Count;j++) {
				if (g_KeyList[i].ValNo[j] >= 0) {
					if (g_KeyList[i].ResetFlag[j]) {
						val = g_ValList[g_KeyList[i].ValNo[j]].Def;
					}
					else {
						val = g_ValList[g_KeyList[i].ValNo[j]].Val;
						val += g_KeyList[i].Step[j] * 30.0f / g_LimitFPS;
					}
					if (val > g_ValList[g_KeyList[i].ValNo[j]].Max) val = g_ValList[g_KeyList[i].ValNo[j]].Max;
					if (val < g_ValList[g_KeyList[i].ValNo[j]].Min) val = g_ValList[g_KeyList[i].ValNo[j]].Min;
					g_ValList[g_KeyList[i].ValNo[j]].Val = val;
					g_ValList[g_KeyList[i].ValNo[j]].Updated = true;
					for (k = 0;k < g_ValList[g_KeyList[i].ValNo[j]].RefCount;k++) {
						if (g_ValList[g_KeyList[i].ValNo[j]].Flag[k])
							*(g_ValList[g_KeyList[i].ValNo[j]].Ref[k]) = -val;
						else *(g_ValList[g_KeyList[i].ValNo[j]].Ref[k]) = val;
					}
				}
			}
		}
		else {
			for (j = 0;j < g_KeyList[i].Count;j++) {
				if (g_KeyList[i].ValNo[j] >= 0) {
					if (g_KeyList[i].ReleaseFlag[j]) {
						if (g_KeyList[i].ResetFlag2[j]) {
							val = g_ValList[g_KeyList[i].ValNo[j]].Def;
						}
						else {
							val = g_ValList[g_KeyList[i].ValNo[j]].Val;
							val += g_KeyList[i].Step2[j] * 30.0f / g_LimitFPS;
						}
						if (val > g_ValList[g_KeyList[i].ValNo[j]].Max) val = g_ValList[g_KeyList[i].ValNo[j]].Max;
						if (val < g_ValList[g_KeyList[i].ValNo[j]].Min) val = g_ValList[g_KeyList[i].ValNo[j]].Min;
						g_ValList[g_KeyList[i].ValNo[j]].Val = val;
						for (k = 0;k < g_ValList[g_KeyList[i].ValNo[j]].RefCount;k++) {
							if (g_ValList[g_KeyList[i].ValNo[j]].Flag[k])
								*(g_ValList[g_KeyList[i].ValNo[j]].Ref[k]) = -val;
							else *(g_ValList[g_KeyList[i].ValNo[j]].Ref[k]) = val;
						}
					}
				}
			}
		}
	}
	if (g_World->Stop == false && g_World->NetStop == false) {
		//変数の内容を更新
		for (i = 0;i<GVALMAX;i++) {
			if (g_ValList[i].Updated == false) {
				if (g_ValList[i].Val>g_ValList[i].Def) {
					g_ValList[i].Val -= (GFloat)fabs(g_ValList[i].Dec);
					if (g_ValList[i].Val < g_ValList[i].Def) g_ValList[i].Val = g_ValList[i].Def;
				}
				if (g_ValList[i].Val < g_ValList[i].Def) {
					g_ValList[i].Val += (GFloat)fabs(g_ValList[i].Dec);
					if (g_ValList[i].Val > g_ValList[i].Def) g_ValList[i].Val = g_ValList[i].Def;
				}
			}
			for (k = 0;k < g_ValList[i].RefCount;k++) {
				if (g_ValList[i].Flag[k])
					*(g_ValList[i].Ref[k]) = -g_ValList[i].Val;
				else *(g_ValList[i].Ref[k]) = g_ValList[i].Val;
			}
		}

		TotalPower = 0;
		for (i = 0;i < g_World->getChipCount();i++) {
			if (g_Chip[i]->Top != g_Chip[0] && g_Chip[i]->Top->ByeFlag >= 1) {
				g_Chip[i]->Power = g_Chip[i]->PowerSave;
				if (g_Chip[i]->Top->ByeFlag == 1) g_Chip[i]->PowerSave = g_Chip[i]->PowerSave*0.95f;
				else g_Chip[i]->PowerSave = 0.0f;
				if (fabs(g_Chip[i]->PowerSave) < 1.0) g_Chip[i]->PowerSave = 0;
			}
			if (g_Chip[i]->Top != NULL) {
				float s = (float)GDTSTEP / 6.0f;
				//ホイールのトルク
				if (TorqueFlag && (g_Chip[i]->MeshNo == 2 || g_Chip[i]->MeshNo == 3) && g_Chip[i]->Power != 0) {
					if (!EfficientFlag) {
						double f = g_Chip[i]->CheckFuel(g_Chip[i]->Power / WHL_EFF);
						g_Chip[i]->PowerByFuel = g_Chip[i]->Power = f*WHL_EFF;
						g_Chip[i]->Top->UseFuel(f*30.0 / g_LimitFPS);
						g_Chip[i]->Top->CalcTotalFuel();

					}
					else g_Chip[i]->PowerByFuel = g_Chip[i]->Power;
					float po = g_Chip[i]->PowerByFuel*s;
					g_Chip[i]->ApplyLocalTorque(GVector(0, 1, 0)*po / (1 + g_Chip[i]->W.abs() / 100));
					if (g_Chip[i]->MeshNo == 2 && g_Chip[i]->Parent) g_Chip[i]->Parent->ApplyTorque(-GVector(0, 1, 0)*g_Chip[i]->R*(po / (1 + g_Chip[i]->W.abs() / 100)));
					TotalPower += (GFloat)fabs(po / (1 + g_Chip[i]->W.abs() / 100));
				}
				//ARM弾発射
				else if (g_Chip[i]->ChipType == 10 && g_Chip[i]->ArmEnergy > 0 && g_Chip[i]->Energy >= g_Chip[i]->ArmEnergy && g_Chip[i]->Power >= g_Chip[i]->ArmEnergy && g_TickCount * 30 / g_LimitFPS > 150) {
					GVector dir;
					switch (g_Chip[i]->Dir) {
					case 1:dir = GVector(1, 0, 0);break;
					case 2:dir = GVector(0, 0, -1);break;
					case 3:dir = GVector(-1, 0, 0);break;
					default:dir = GVector(0, 0, 1);break;
					}
					GVector d = dir*g_Chip[i]->R;
					g_Chip[i]->ApplyForce(-d*g_Chip[i]->ArmEnergy*s, g_Chip[i]->X);
					TotalPower += (GFloat)fabs(g_Chip[i]->ArmEnergy*s);
					g_Chip[i]->Energy = (float)-5000 * 30 / g_LimitFPS;
					double f = sqrt(fabs(g_Chip[i]->ArmEnergy / 125000.0));if (f > 2.5) f = 2.5;
					BOOL hit;
					FLOAT dist;
					LPDIRECT3DVERTEXBUFFER8 pVB;
					LPDIRECT3DINDEXBUFFER8  pIB;
					WORD*            pIndices;
					D3DVERTEX*    pVertices;
					g_pLandMesh->GetSysMemMesh()->GetVertexBuffer(&pVB);
					g_pLandMesh->GetSysMemMesh()->GetIndexBuffer(&pIB);
					pIB->Lock(0, 0, (BYTE**)&pIndices, 0);
					pVB->Lock(0, 0, (BYTE**)&pVertices, 0);
					D3DXVECTOR3 v1, v2;
					float as = ARMSPEED;
					if (g_Chip[i]->X.y < WATER_LINE) as = as / 10;
					GVector dir2 = (d*as*30.0f / (GFloat)g_LimitFPS + g_Chip[i]->V*g_Chip[i]->World->Dt*(GFloat)GDTSTEP).normalize2();
					v1.x = g_Chip[i]->X.x;v1.y = g_Chip[i]->X.y;v1.z = g_Chip[i]->X.z;
					v2.x = dir2.x;v2.y = dir2.y;v2.z = dir2.z;
					D3DXIntersect(g_pLandMesh->GetSysMemMesh(), &v1, &v2, &hit, NULL, NULL, NULL, &dist, NULL, NULL);
					if (!hit) dist = 100000.0f;
					GVector p = g_Chip[i]->X + dir2*dist;
					GBulletVertex *bul = g_Bullet->Add(g_Chip[i], g_Chip[i]->X, d*as*30.0f / (GFloat)g_LimitFPS + g_Chip[i]->V*g_Chip[i]->World->Dt*(GFloat)GDTSTEP, g_Chip[i]->ArmEnergy, (GFloat)f*0.3f, dist, p, -1);
					if (g_Chip[i]->X.y < WATER_LINE) bul->Life = 150.0f;

					pVB->Unlock();
					pIB->Unlock();

					pVB->Release();
					pIB->Release();


				}
				//JET
				else if (g_Chip[i]->ChipType == 7) {
					if (!EfficientFlag) {
						double f = g_Chip[i]->CheckFuel(g_Chip[i]->Power / JET_EFF);
						g_Chip[i]->PowerByFuel = g_Chip[i]->Power = f*JET_EFF;
						g_Chip[i]->Top->UseFuel(f*30.0 / g_LimitFPS);
						g_Chip[i]->Top->CalcTotalFuel();
					}
					else g_Chip[i]->PowerByFuel = g_Chip[i]->Power;
					float po = g_Chip[i]->PowerByFuel*s;
					if (g_Chip[i]->Option == 1) {
						if (JetFlag) g_Chip[i]->ApplyForce(GVector(0, 1, 0)*po, g_Chip[i]->X);
						if (JetFlag) TotalPower += (GFloat)fabs(po);
					}
					else if (g_Chip[i]->Option == 2) {
						//g_Chip[i]->ApplyForce(GVector(0,1,0)*g_Chip[i]->PowerByFuel,g_Chip[i]->X);
						if (JetFlag) TotalPower += (GFloat)fabs(po);
					}
					else {
						if (JetFlag && g_Chip[i]->Power != 0) {
							g_Chip[i]->ApplyForce(GVector(0, 1, 0)*g_Chip[i]->R*po, g_Chip[i]->X);
							TotalPower += (GFloat)fabs(po);
						}
						if (g_Chip[i]->Effect == 1) {
							int nn = (int)((fabs(po) + 7900) / 8000);
							if (nn > 0) {
								float a = 0.5f + (rand() % 100) / 5000.0f;
								if (a > 2.0f) a = 2.0f;else if (a <= 0.0f) a = 0.0f;
								GVector vv = -GVector(0, 1, 0)*g_Chip[i]->R*po / 50000.0f;
								//if(v.abs()>0.1f) v=v.normalize()/10.0f;
								for (int ii = 0;ii < nn;ii++) {
									g_JetParticle->Add(g_Chip[i]->X + (g_Chip[i]->V / 30 - vv)*(GFloat)ii / (GFloat)nn, vv, GVector(0, 0, 0), 0.08f, a, 0.02f, GVector(1, 1, 1));
								}
							}
						}
						if (g_Chip[i]->Effect == 2) {
							int nn = (int)((fabs(po) + 7900) / 8000);
							if (nn > 0) {
								float a = 0.7f + (rand() % 1000) / 5000.0f;
								if (a > 1.0f) a = 1.0f;else if (a <= 0.0f) a = 0.0f;
								GVector vv = -GVector(0, 1, 0)*g_Chip[i]->R*po / 50000.0f;
								//						double f=fabs(Power/2000.0);if(f>2.5) f=2.5;
								for (int ii = 0;ii < nn;ii++) {
									GVector rv = GVector((rand() % 100 - 50) / 2000.0f, (rand() % 100 - 50) / 2000.0f, (rand() % 100 - 50) / 2000.0f);
									g_JetParticle->Add(g_Chip[i]->X + vv * 2 + (g_Chip[i]->V / 30 - vv)*(GFloat)ii / (GFloat)nn, vv + rv, GVector(0, 0, 0), 0.08f, a, 0.003f, GVector(1, 1, 1));
								}
							}
						}
						if (g_Chip[i]->Effect == 3) {
							int nn = (int)((fabs(po) + 7900) / 8000);
							if (nn > 0) {
								float a = 1.0f + (rand() % 100) / 5000.0f;
								if (a > 2.0f) a = 2.0f;else if (a <= 0.0f) a = 0.0f;
								GVector vv = -GVector(0, 1, 0)*g_Chip[i]->R*po / 50000.0f;
								//if(v.abs()>0.1f) v=v.normalize()/10.0f;
								for (int ii = 0;ii < nn;ii++) {
									g_JetParticle->Add(g_Chip[i]->X + (g_Chip[i]->V / 30 - vv)*(GFloat)ii / (GFloat)nn, vv, GVector(0, 0, 0), 0.08f, a, 0.005f, GVector(1, 1, 1));
								}
							}
						}
						if (g_Chip[i]->Effect == 4) {
							int nn = (int)((fabs(po) + 7900) / 8000);
							if (nn > 0) {
								float a = 1.0f;
								GVector vv = -GVector(0, 1, 0)*g_Chip[i]->R*po / 50000.0f;
								//if(v.abs()>0.1f) v=v.normalize()/10.0f;
								for (int ii = 0;ii < nn;ii++) {
									g_JetParticle->Add(g_Chip[i]->X + (g_Chip[i]->V / 30 - vv)*(GFloat)ii / (GFloat)nn, vv, GVector(0, 0, 0), 0.01f, a, 0.000f, GVector(1, 1, 1));
								}
							}
						}
					}
				}
			}
		}
		if (hMidiOut != NULL && SoundType == 1) {
			if (TotalPower > 0.1f) {
				if (!preSound) {
					//音を鳴らす(0x9n,音程,強さ,0) nはチャンネル(0～0xF)
					long msg = MAKELONG(MAKEWORD(0x90, 80), MAKEWORD(0x1f, 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
				}
				preSound = true;
			}
			else {
				//		m_fSoundPlayRepeatCountdown=0.0f;
						//音を止める(0x9n,音程,00,0) nはチャンネル(0～0xF) 
				long msg = MAKELONG(MAKEWORD(0x90, 80), MAKEWORD(0x00, 0));
				midiOutShortMsg(hMidiOut, msg); // Note Off
				preSound = false;
			}
		}

		if (waitCount <= 0) {
		}
		else {
			StopChip();
			GameTime = 0;
			waitCount--;
		}
		//if(g_World->MainStepCount<0) Resize3DEnvironment();
		int crush = g_World->Rigid[0]->Crush;
		g_World->Move(m_UserInput.bButtonReset || m_UserInput.bButtonInit);
		MoveEnd = true;
		if (crush == false && g_World->Rigid[0]->Crush) { //死んだ
			if (g_DPlay->GetNumPlayers() > 0) {	//初期化を送信する
				GINFOSTREAM stream;
				stream.code = 100; //初期化
				g_MyPlayerData.crush++;
				stream.data = g_MyPlayerData;
				DWORD sz = sizeof(GINFOSTREAM) + sizeof(short);//1Chip分だけ送る
				g_DPlay->SendAll((BYTE*)&stream, sz);
			}
		}
		if (hMidiOut != NULL && SoundType == 1) {

			soundCount--;
			if (soundCount < 0) soundCount = 0;
			if (g_Chip[0]->TotalHitCount>0 && soundCount == 0) {
				//		if( m_pBoundSound1 ) {
				int db = (int)(g_Chip[0]->MaxImpulse / 3.0f + 0.5f);
				if (db > 0x7f) db = 0x7f;
				if (db > 2) {
					long msg = MAKELONG(MAKEWORD(0x99, 35), MAKEWORD(db, 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
					msg = MAKELONG(MAKEWORD(0x99, 42), MAKEWORD((db * 2 / 5), 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
					soundCount = 2;
				}
				//		}
			}
		}
		if (!MsgFlag && KeyRecordMode > 0) {
			KeyRecordCount++;
			if (KeyRecordCount >= GRECMAX) {
				KeyRecordMax = KeyRecordCount - 1;
				KeyRecordMode = 0;
			}
			else if (KeyRecordMode == 2 && KeyRecordCount >= KeyRecordMax) {
				RecState = 3;
			}
		}
		g_TickCount++;
		g_SystemTickCount++;
	}
	MsgFlag = false;
	return S_OK;
}
HRESULT CMyD3DApplication::ViewSet() {
	//ビューの設定
	static int vcount = 0;
	int c = 0;
	GFloat r = 8 + g_Chip[0]->V.abs() / 40.0f;
	static GFloat sy = 0.0f;
	if (ViewType < 0)
	{
		UpVec = UserUpVec;
		EyePos = UserEyePos;
		RefPos = UserRefPos;
	}
	else if (ViewType == 0)
	{
		UpVec = GVector(0, 1, 0);
		EyePos = g_Chip[LastBye]->TotalCenter + (EyePos - g_Chip[LastBye]->TotalCenter).normalize2()*r;
		EyePos.y = g_Chip[LastBye]->TotalCenter.y + 2.0f;
		RefPos = g_Chip[LastBye]->TotalCenter + GVector(0, 0, 0);
		UserRefPos = RefPos;UserEyePos = EyePos;UserUpVec = UpVec;
	}
	else if (ViewType == 1)
	{
		UpVec = GVector(0, 1, 0);
		float v = g_Chip[LastBye]->V.abs() / 2.0f;
		if (v < 15) v = 15;
		if (((EyePos - g_Chip[LastBye]->TotalCenter).abs() >= v * 5 || vcount >= v * 10) && vcount >= v) {
			vcount = 0;
			EyePos = g_Chip[LastBye]->TotalCenter + (g_Chip[LastBye]->V).normalize2()*v*2.0f;
			EyePos.x += (GFloat)fabs((int)(g_Chip[LastBye]->TotalCenter.z) % 60 / 10.0f) - 3.0f;
			EyePos.y += (GFloat)(fabs((int)(g_Chip[LastBye]->TotalCenter.z) % 80 / 10.0f));
			if (EyePos.y < g_Chip[LastBye]->TotalCenter.y) EyePos.y = g_Chip[LastBye]->TotalCenter.y;
			EyePos.z += (GFloat)fabs((int)(g_Chip[LastBye]->TotalCenter.x) % 60 / 10.0f) - 3.0f;
		}
		RefPos = g_Chip[LastBye]->TotalCenter;
		UserRefPos = RefPos;UserEyePos = EyePos;UserUpVec = UpVec;
		vcount++;
	}
	else if (ViewType == 2)
	{
		UpVec = GVector(0, 1, 0);
		GVector v = g_Chip[LastBye]->X + GVector(0, 0.8f, 4)*g_Chip[LastBye]->R;
		//GFloat y=g_World->Land->GetY2(v.x,g_Chip[LastBye]->TotalCenter.y+g_Chip[LastBye]->TotalRadius,v.z);
		//if(y>v.y) v.y=y+0.8f;
		EyePos = EyePos*0.6f + v*0.4f;
		v = g_Chip[LastBye]->X + GVector(0, 0.8f, -4)*g_Chip[LastBye]->R;
		RefPos = RefPos*0.6f + v*0.4f;
		UserRefPos = RefPos;UserEyePos = EyePos;UserUpVec = UpVec;
	}
	else if (ViewType == 3)
	{
		UpVec = GVector(0, 1, 0);
		EyePos = g_Chip[LastBye]->TotalCenter + GVector(0, r*0.1f, r)*g_Chip[LastBye]->R;
		sy = (sy*5.0f + (GFloat)fabs(EyePos.y - g_Chip[LastBye]->TotalCenter.y)) / 10.0f;
		EyePos.y = sy + g_Chip[LastBye]->TotalCenter.y;
		RefPos = g_Chip[LastBye]->TotalCenter + GVector(0, 0, 0);
		UserRefPos = RefPos;UserEyePos = EyePos;UserUpVec = UpVec;
	}
	else if (ViewType == 4)
	{
		UpVec = GVector(0, 0, -1);
		EyePos = g_Chip[LastBye]->TotalCenter + GVector(0, 30, 0);
		RefPos = g_Chip[LastBye]->TotalCenter;
		UserRefPos = RefPos;UserEyePos = EyePos;UserUpVec = UpVec;
	}
	else if (ViewType == 5)
	{
		UpVec = GVector(0, 1, 0);
		EyePos = g_Chip[LastBye]->TotalCenter + (EyePos - g_Chip[LastBye]->TotalCenter).normalize2()*r;
		EyePos.y = g_Chip[LastBye]->TotalCenter.y + 2.0f;
		RefPos = g_Chip[LastBye]->TotalCenter + GVector(0, 0, 0);
		UserRefPos = RefPos;UserEyePos = EyePos;UserUpVec = UpVec;
	}
	else if (ViewType == 6)
	{
		UpVec = GVector(0, 1, 0);
		float v = g_Chip[LastBye]->V.abs() / 2.0f;
		if (v < 15) v = 15;
		if (((EyePos - g_Chip[LastBye]->TotalCenter).abs() >= v * 5 || vcount >= v * 10) && vcount >= v) {
			vcount = 0;
			EyePos = g_Chip[LastBye]->TotalCenter + (g_Chip[LastBye]->V).normalize2()*v*2.0f;
			EyePos.x += (GFloat)fabs((int)(g_Chip[LastBye]->TotalCenter.z) % 60 / 10.0f) - 3.0f;
			EyePos.y += (GFloat)(fabs((int)(g_Chip[LastBye]->TotalCenter.z) % 80 / 10.0f));
			if (EyePos.y < g_Chip[LastBye]->TotalCenter.y) EyePos.y = g_Chip[LastBye]->TotalCenter.y;
			EyePos.z += (GFloat)fabs((int)(g_Chip[LastBye]->TotalCenter.x) % 60 / 10.0f) - 3.0f;
		}
		RefPos = g_Chip[LastBye]->TotalCenter;
		vcount++;
		UserRefPos = RefPos;UserEyePos = EyePos;UserUpVec = UpVec;
	}
	else if (ViewType == 7)
	{
		UpVec = GVector(0, 1, 0)*g_Chip[LastBye]->R;
		EyePos = g_Chip[LastBye]->X + ((GVector(0, 0, 1)*g_Chip[LastBye]->R)*0.75f);
		RefPos = g_Chip[LastBye]->X + (GVector(0, 0, -1)*g_Chip[LastBye]->R)*3.0f;
	}
	else if (ViewType == 8)
	{
		UpVec = UpVec*0.6f + GVector(0, 1, 0)*g_Chip[LastBye]->R*0.4f;
		GVector v = g_Chip[LastBye]->X + GVector(0, 0, 4)*g_Chip[LastBye]->R;
		EyePos = EyePos*0.6f + v*0.4f;
		RefPos = g_Chip[LastBye]->TotalCenter + GVector(0, 0, 0);
		UserRefPos = RefPos;UserEyePos = EyePos;UserUpVec = UpVec;
	}
	else
	{
		UpVec = UpVec*0.6f + GVector(0, 1, 0)*g_Chip[LastBye]->R*0.4f;
		GVector v = g_Chip[LastBye]->X + GVector(0, 0.8f, 4)*g_Chip[LastBye]->R;
		//		GFloat y=g_World->Land->GetY2(v.x,g_Chip[LastBye]->TotalCenter.y+g_Chip[LastBye]->TotalRadius,v.z);
		//		if(y>v.y) v.y=y+0.8f;
		EyePos = EyePos*0.6f + v*0.4f;
		v = g_Chip[LastBye]->X + GVector(0, 0.8f, -4)*g_Chip[LastBye]->R;
		RefPos = RefPos*0.6f + v*0.4f;
		UserRefPos = RefPos;UserEyePos = EyePos;UserUpVec = UpVec;
	}
	GVector eye = EyePos;
	if (ViewType != 7) {
		GVector r = (EyePos - RefPos).cross(UpVec).normalize2();
		GVector v = (EyePos - RefPos)*(GMatrix().rotate(r, TurnUD).rotate(UpVec, TurnLR));
		eye = RefPos + v;
	}

	//	light1.Position.x= (float)EyePos.x;
	//	light1.Position.y= (float)EyePos.y;
	//	light1.Position.z= (float)EyePos.z;
	//   m_pd3dDevice->SetLight( 1, &light1 );


	// Set the projection matrix
	D3DXMATRIX matProj;
	FLOAT fAspect = ((FLOAT)m_d3dsdBackBuffer.Width) / m_d3dsdBackBuffer.Height;
	if (ViewType == 7) D3DXMatrixPerspectiveFovLH(&matProj, CCDZoom*(GFloat)M_PI / 180.0f, fAspect, 1.0f, g_FarMax);
	else D3DXMatrixPerspectiveFovLH(&matProj, Zoom*(GFloat)M_PI / 180.0f, fAspect, 1.0f, g_FarMax);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);


	D3DXVECTOR3 vFromPt = D3DXVECTOR3((FLOAT)eye.x, (FLOAT)eye.y, (FLOAT)eye.z);
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3((FLOAT)RefPos.x, (FLOAT)RefPos.y, (FLOAT)RefPos.z);
	D3DXVECTOR3 vUpVec = D3DXVECTOR3((FLOAT)UpVec.x, (FLOAT)UpVec.y, (FLOAT)UpVec.z);
	D3DXMatrixLookAtLH(&GMatView, &vFromPt, &vLookatPt, &vUpVec);

	// Update the world state according to user input
	D3DXMatrixIdentity(&GMatWorld);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &GMatWorld);

	m_pd3dDevice->SetTransform(D3DTS_VIEW, &GMatView);

	RenderSky();

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: UpdateInput()
// Desc: Update the user input.  Called once per frame 
//-----------------------------------------------------------------------------
void CMyD3DApplication::ClearInput(UserInput* pUserInput)
{
	pUserInput->bButtonOneShotInit = FALSE;
	pUserInput->bButtonOneShotReset = FALSE;
	pUserInput->bButtonOneShotTitle = FALSE;
	pUserInput->bButtonOneShotZoomIn = FALSE;
	pUserInput->bButtonOneShotZoomOut = FALSE;
	pUserInput->bButtonOneShotResetView = FALSE;
	pUserInput->bButtonOneShotTurnUp = FALSE;
	pUserInput->bButtonOneShotTurnDown = FALSE;
	pUserInput->bButtonOneShotTurnLeft = FALSE;
	pUserInput->bButtonOneShotTurnRight = FALSE;
	pUserInput->bButtonOneShotYForce = FALSE;

	pUserInput->bButtonInit = FALSE;
	pUserInput->bButtonReset = FALSE;
	pUserInput->bButtonTitle = FALSE;
	pUserInput->bButtonZoomIn = FALSE;
	pUserInput->bButtonZoomOut = FALSE;
	pUserInput->bButtonResetView = FALSE;
	pUserInput->bButtonTurnUp = FALSE;
	pUserInput->bButtonTurnDown = FALSE;
	pUserInput->bButtonTurnLeft = FALSE;
	pUserInput->bButtonTurnRight = FALSE;
	pUserInput->bButtonYForce = FALSE;
	for (int i = 0;i < GSYSKEYMAX;i++) {
		pUserInput->bSystem[i] = FALSE;
		pUserInput->bSystemOneShot[i] = FALSE;
	}
	for (int i = 0;i < GKEYMAX;i++) {
		pUserInput->bButton[i] = FALSE;
		pUserInput->bButtonOneShot[i] = FALSE;
	}
}void CMyD3DApplication::DummyInput(UserInput* pUserInput)
{
	UpdateInput(pUserInput);
	ClearInput(pUserInput);
}
void CMyD3DApplication::UpdateInput(UserInput* pUserInput)
{
	if (NULL == m_pInputDeviceManager)
		return;

	// Get access to the list of semantically-mapped input devices
	CInputDeviceManager::DeviceInfo* pDeviceInfos;
	DWORD dwNumDevices;
	m_pInputDeviceManager->GetDevices(&pDeviceInfos, &dwNumDevices);

	for (int c = 0;c < GKEYMAX;c++) pUserInput->bButtonOneShot[c] = FALSE;
	for (int c = 0;c < GSYSKEYMAX;c++) pUserInput->bSystemOneShot[c] = FALSE;
	pUserInput->bButtonOneShotInit = FALSE;
	pUserInput->bButtonOneShotReset = FALSE;
	pUserInput->bButtonOneShotTitle = FALSE;
	pUserInput->bButtonOneShotZoomIn = FALSE;
	pUserInput->bButtonOneShotZoomOut = FALSE;
	pUserInput->bButtonOneShotResetView = FALSE;
	pUserInput->bButtonOneShotTurnUp = FALSE;
	pUserInput->bButtonOneShotTurnDown = FALSE;
	pUserInput->bButtonOneShotTurnLeft = FALSE;
	pUserInput->bButtonOneShotTurnRight = FALSE;
	pUserInput->bButtonOneShotYForce = FALSE;
	// Loop through all devices and check game input
	for (DWORD i = 0; i < dwNumDevices; i++)
	{
		DIDEVICEOBJECTDATA rgdod[20];
		DWORD   dwItems = 10;
		HRESULT hr;
		LPDIRECTINPUTDEVICE8 pdidDevice = pDeviceInfos[i].pdidDevice;
		InputDeviceState* pInputDeviceState = (InputDeviceState*)pDeviceInfos[i].pParam;

		hr = pdidDevice->Acquire();
		hr = pdidDevice->Poll();
		hr = pdidDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),
			rgdod, &dwItems, 0);
		if (FAILED(hr))
			continue;

		// Get the sematics codes for the game menu
		for (DWORD j = 0; j < dwItems; j++)
		{
			BOOL  bButtonState = (rgdod[j].dwData == 0x80) ? TRUE : FALSE;
			FLOAT fButtonState = (rgdod[j].dwData == 0x80) ? 1.0f : 0.0f;
			FLOAT fAxisState = (FLOAT)((int)rgdod[j].dwData);

			switch (rgdod[j].uAppData)
			{
				// TODO: Handle semantics for the game 

				// Handle relative axis data
			case INPUT_ROTATE_AXIS_LR:
				pInputDeviceState->fAxisRotateLR = -fAxisState;
				pUserInput->fAxisRotateLR = 0.0f;
				break;
			case INPUT_ROTATE_AXIS_UD:
				pInputDeviceState->fAxisRotateUD = -fAxisState;
				pUserInput->fAxisRotateUD = 0.0f;
				break;
			case INPUT_ROTATE_AXIS_IO:
				pInputDeviceState->fAxisRotateIO = -fAxisState;
				pUserInput->fAxisRotateIO = 0.0f;
				break;
			case INPUT_ROTATEX:
				pInputDeviceState->fAxisRotateX = -fAxisState;
				pUserInput->fAxisRotateX = 0.0f;
				break;
			case INPUT_ROTATEY:
				pInputDeviceState->fAxisRotateY = -fAxisState;
				pUserInput->fAxisRotateY = 0.0f;
				break;

			case INPUT_ROTATEZ:
				pInputDeviceState->fAxisRotateZ = -fAxisState;
				pUserInput->fAxisRotateZ = 0.0f;
				break;


			case INPUT_HATSWITCH:
				pInputDeviceState->fAxisHat = -fAxisState;
				pUserInput->fAxisHat = 0.0f;
				break;

				// Handle buttons separately so the button state data
				// doesn't overwrite the axis state data, and handle
				// each button separately so they don't overwrite each other
			case INPUT_0:
				if (pInputDeviceState->bButton[0] == FALSE && bButtonState) pUserInput->bButtonOneShot[0] = TRUE;
				pInputDeviceState->bButton[0] = pUserInput->bButton[0] = bButtonState; break;
			case INPUT_1:
				if (pInputDeviceState->bButton[1] == FALSE && bButtonState) pUserInput->bButtonOneShot[1] = TRUE;
				pInputDeviceState->bButton[1] = pUserInput->bButton[1] = bButtonState; break;
			case INPUT_2:
				if (pInputDeviceState->bButton[2] == FALSE && bButtonState) pUserInput->bButtonOneShot[2] = TRUE;
				pInputDeviceState->bButton[2] = pUserInput->bButton[2] = bButtonState; break;
			case INPUT_3:
				if (pInputDeviceState->bButton[3] == FALSE && bButtonState) pUserInput->bButtonOneShot[3] = TRUE;
				pInputDeviceState->bButton[3] = pUserInput->bButton[3] = bButtonState; break;
			case INPUT_4:
				if (pInputDeviceState->bButton[4] == FALSE && bButtonState) pUserInput->bButtonOneShot[4] = TRUE;
				pInputDeviceState->bButton[4] = pUserInput->bButton[4] = bButtonState; break;
			case INPUT_5:
				if (pInputDeviceState->bButton[5] == FALSE && bButtonState) pUserInput->bButtonOneShot[5] = TRUE;
				pInputDeviceState->bButton[5] = pUserInput->bButton[5] = bButtonState; break;
			case INPUT_6:
				if (pInputDeviceState->bButton[6] == FALSE && bButtonState) pUserInput->bButtonOneShot[6] = TRUE;
				pInputDeviceState->bButton[6] = pUserInput->bButton[6] = bButtonState; break;
			case INPUT_7:
				if (pInputDeviceState->bButton[7] == FALSE && bButtonState) pUserInput->bButtonOneShot[7] = TRUE;
				pInputDeviceState->bButton[7] = pUserInput->bButton[7] = bButtonState; break;
			case INPUT_8:
				if (pInputDeviceState->bButton[8] == FALSE && bButtonState) pUserInput->bButtonOneShot[8] = TRUE;
				pInputDeviceState->bButton[8] = pUserInput->bButton[8] = bButtonState; break;
			case INPUT_9:
				if (pInputDeviceState->bButton[9] == FALSE && bButtonState) pUserInput->bButtonOneShot[9] = TRUE;
				pInputDeviceState->bButton[9] = pUserInput->bButton[9] = bButtonState; break;
			case INPUT_10:
				if (pInputDeviceState->bButton[10] == FALSE && bButtonState) pUserInput->bButtonOneShot[10] = TRUE;
				pInputDeviceState->bButton[10] = pUserInput->bButton[10] = bButtonState; break;
			case INPUT_11:
				if (pInputDeviceState->bButton[11] == FALSE && bButtonState) pUserInput->bButtonOneShot[11] = TRUE;
				pInputDeviceState->bButton[11] = pUserInput->bButton[11] = bButtonState; break;
			case INPUT_12:
				if (pInputDeviceState->bButton[12] == FALSE && bButtonState) pUserInput->bButtonOneShot[12] = TRUE;
				pInputDeviceState->bButton[12] = pUserInput->bButton[12] = bButtonState; break;
			case INPUT_13:
				if (pInputDeviceState->bButton[13] == FALSE && bButtonState) pUserInput->bButtonOneShot[13] = TRUE;
				pInputDeviceState->bButton[13] = pUserInput->bButton[13] = bButtonState; break;
			case INPUT_14:
				if (pInputDeviceState->bButton[14] == FALSE && bButtonState) pUserInput->bButtonOneShot[14] = TRUE;
				pInputDeviceState->bButton[14] = pUserInput->bButton[14] = bButtonState; break;
			case INPUT_15:
				if (pInputDeviceState->bButton[15] == FALSE && bButtonState) pUserInput->bButtonOneShot[15] = TRUE;
				pInputDeviceState->bButton[15] = pUserInput->bButton[15] = bButtonState; break;

			case INPUT_16:
				if (pInputDeviceState->bButton[16] == FALSE && bButtonState) pUserInput->bButtonOneShot[16] = TRUE;
				pInputDeviceState->bButton[16] = pUserInput->bButton[16] = bButtonState; break;

			case SYSTEM_0:
				if (pInputDeviceState->bSystem[0] == FALSE && bButtonState) pUserInput->bSystemOneShot[0] = TRUE;
				pInputDeviceState->bSystem[0] = pUserInput->bSystem[0] = bButtonState; break;
			case SYSTEM_1:
				if (pInputDeviceState->bSystem[1] == FALSE && bButtonState) pUserInput->bSystemOneShot[1] = TRUE;
				pInputDeviceState->bSystem[1] = pUserInput->bSystem[1] = bButtonState; break;
			case SYSTEM_2:
				if (pInputDeviceState->bSystem[2] == FALSE && bButtonState) pUserInput->bSystemOneShot[2] = TRUE;
				pInputDeviceState->bSystem[2] = pUserInput->bSystem[2] = bButtonState; break;
			case SYSTEM_3:
				if (pInputDeviceState->bButton[3] == FALSE && bButtonState) pUserInput->bSystemOneShot[3] = TRUE;
				pInputDeviceState->bSystem[3] = pUserInput->bSystem[3] = bButtonState; break;

			case INPUT_INIT:
				if (pInputDeviceState->bButtonInit == FALSE && bButtonState) pUserInput->bButtonOneShotInit = TRUE;
				pInputDeviceState->bButtonInit = pUserInput->bButtonInit = bButtonState; break;
			case INPUT_RESET:
				if (pInputDeviceState->bButtonReset == FALSE && bButtonState) pUserInput->bButtonOneShotReset = TRUE;
				pInputDeviceState->bButtonReset = pUserInput->bButtonReset = bButtonState; break;
			case INPUT_TITLE:
				if (pInputDeviceState->bButtonTitle == FALSE && bButtonState) pUserInput->bButtonOneShotTitle = TRUE;
				pInputDeviceState->bButtonTitle = pUserInput->bButtonTitle = bButtonState; break;
			case INPUT_ZOOMIN:
				if (pInputDeviceState->bButtonZoomIn == FALSE && bButtonState) pUserInput->bButtonOneShotZoomIn = TRUE;
				pInputDeviceState->bButtonZoomIn = pUserInput->bButtonZoomIn = bButtonState; break;
			case INPUT_ZOOMOUT:
				if (pInputDeviceState->bButtonZoomOut == FALSE && bButtonState) pUserInput->bButtonOneShotZoomOut = TRUE;
				pInputDeviceState->bButtonZoomOut = pUserInput->bButtonZoomOut = bButtonState; break;
			case INPUT_RESETVIEW:
				if (pInputDeviceState->bButtonResetView == FALSE && bButtonState) pUserInput->bButtonOneShotResetView = TRUE;
				pInputDeviceState->bButtonResetView = pUserInput->bButtonResetView = bButtonState; break;
			case INPUT_TUP:
				if (pInputDeviceState->bButtonTurnUp == FALSE && bButtonState) pUserInput->bButtonOneShotTurnUp = TRUE;
				pInputDeviceState->bButtonTurnUp = pUserInput->bButtonTurnUp = bButtonState; break;
			case INPUT_TDOWN:
				if (pInputDeviceState->bButtonTurnDown == FALSE && bButtonState) pUserInput->bButtonOneShotTurnDown = TRUE;
				pInputDeviceState->bButtonTurnDown = pUserInput->bButtonTurnDown = bButtonState; break;
			case INPUT_TLEFT:
				if (pInputDeviceState->bButtonTurnLeft == FALSE && bButtonState) pUserInput->bButtonOneShotTurnLeft = TRUE;
				pInputDeviceState->bButtonTurnLeft = pUserInput->bButtonTurnLeft = bButtonState; break;
			case INPUT_TRIGHT:
				if (pInputDeviceState->bButtonTurnRight == FALSE && bButtonState) pUserInput->bButtonOneShotTurnRight = TRUE;
				pInputDeviceState->bButtonTurnRight = pUserInput->bButtonTurnRight = bButtonState; break;
			case INPUT_YFORCE:
				if (pInputDeviceState->bButtonYForce == FALSE && bButtonState) pUserInput->bButtonOneShotYForce = TRUE;
				pInputDeviceState->bButtonYForce = pUserInput->bButtonYForce = bButtonState; break;
				// Handle one-shot buttons
			case INPUT_CONFIG_INPUT:   if (bButtonState) pUserInput->bDoConfigureInput = TRUE; break;
			case INPUT_CONFIG_DISPLAY: if (bButtonState) pUserInput->bDoConfigureDisplay = TRUE; break;
			}
		}
	}

	// TODO: change process code as needed

	// Process user input and store result into pUserInput struct

	// Concatinate the data from all the DirectInput devices
	for (i = 0; i<dwNumDevices; i++)
	{
		InputDeviceState* pInputDeviceState = (InputDeviceState*)pDeviceInfos[i].pParam;

		// Use the axis data that is furthest from zero
		if (fabs(pInputDeviceState->fAxisRotateLR) > fabs(pUserInput->fAxisRotateLR))
			pUserInput->fAxisRotateLR = pInputDeviceState->fAxisRotateLR;
		Analog[0] = (int)(pUserInput->fAxisRotateLR);

		if (fabs(pInputDeviceState->fAxisRotateUD) > fabs(pUserInput->fAxisRotateUD))
			pUserInput->fAxisRotateUD = pInputDeviceState->fAxisRotateUD;
		Analog[1] = (int)(pUserInput->fAxisRotateUD);

		if (fabs(pInputDeviceState->fAxisRotateIO) > fabs(pUserInput->fAxisRotateIO))
			pUserInput->fAxisRotateIO = pInputDeviceState->fAxisRotateIO;
		Analog[2] = (int)(pUserInput->fAxisRotateIO);

		if (fabs(pInputDeviceState->fAxisRotateX) > fabs(pUserInput->fAxisRotateX))
			pUserInput->fAxisRotateX = pInputDeviceState->fAxisRotateX;
		Analog[3] = (int)(pUserInput->fAxisRotateX);

		if (fabs(pInputDeviceState->fAxisRotateY) > fabs(pUserInput->fAxisRotateY))
			pUserInput->fAxisRotateY = pInputDeviceState->fAxisRotateY;
		Analog[4] = (int)(pUserInput->fAxisRotateY);

		if (fabs(pInputDeviceState->fAxisRotateZ) > fabs(pUserInput->fAxisRotateZ))
			pUserInput->fAxisRotateZ = pInputDeviceState->fAxisRotateZ;
		Analog[5] = (int)(pUserInput->fAxisRotateZ);


		if (fabs(pInputDeviceState->fAxisHat) > fabs(pUserInput->fAxisHat))
			pUserInput->fAxisHat = pInputDeviceState->fAxisHat;
		Hat[0] = (int)(pUserInput->fAxisHat);
		// Process the button data
	}
	if (KeyRecordMode == 1) {
		for (int i = 0;i < 6;i++) {
			KeyRecord[KeyRecordCount][GKEYMAX + i] = Analog[i];
		}
		KeyRecord[KeyRecordCount][GKEYMAX + 6] = Hat[0];
	}
	else if (KeyRecordMode == 2) {
		for (int i = 0;i < 6;i++) {
			Analog[i] = KeyRecord[KeyRecordCount][GKEYMAX + i];
		}
		Hat[0] = KeyRecord[KeyRecordCount][GKEYMAX + 6];
	}
}




typedef struct {
	float z;
	int	n;
} UserData;

int UserCompare(const void *arg1, const void *arg2)
{
	/* 2つの文字列を最後まで比較します。 */
	float z1 = ((UserData*)arg1)->z;
	float z2 = ((UserData*)arg2)->z;
	if (z1 < z2) return 1;
	if (z1 > z2) return -1;

	return 0;
}
//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
int CheckPointCompare(const void *arg1, const void *arg2)
{
	/* 2つの文字列を最後まで比較します。 */
	GFloat l1 = (EyePos - Course[CurrentCourse].Point[*(int*)arg1]).abs2();
	GFloat l2 = (EyePos - Course[CurrentCourse].Point[*(int*)arg2]).abs2();
	return (int)(l2 - l1);
}
void CMyD3DApplication::RenderSky() {
	// TODO: render world
	GFloat lb = 1.0f;
	GFloat lg = 1.0f;
	GFloat lr = 1.0f;
	if (EyePos.y < -3.0) {
		lb = 1.0f + (EyePos.y + 3) / 800.0f;
		lg = 1.0f + (EyePos.y + 3) / 1000.0f;
		lr = 1.0f + (EyePos.y + 3) / 500.0f;
		if (lb <= 0) lb = 0;
		if (lg <= 0) lg = 0;
		if (lr <= 0) lr = 0;
	}

	lightColor.x = lr;lightColor.y = lg;lightColor.z = lb;
	light.Diffuse = D3DXCOLOR(lightColor.x, lightColor.y, lightColor.z, 1.0f);
	light.Specular = D3DXCOLOR(lightColor.x*0.7f, lightColor.y*0.7f, lightColor.z*0.7f, 1.0f);
	for (unsigned int i = 0;i < m_pSkyMesh->m_dwNumMaterials;i++) {
		m_pSkyMesh->m_pMaterials[i].Diffuse.r = 0;
		m_pSkyMesh->m_pMaterials[i].Diffuse.g = 0;
		m_pSkyMesh->m_pMaterials[i].Diffuse.b = 0;
		if (EyePos.y < -3.0f) {
			m_pSkyMesh->m_pMaterials[i].Ambient.r = lightColor.x*(FogColor.x / 255.0f);
			m_pSkyMesh->m_pMaterials[i].Ambient.g = lightColor.y*(FogColor.y / 255.0f);
			m_pSkyMesh->m_pMaterials[i].Ambient.b = lightColor.z*(FogColor.z / 255.0f);
		}
		else {
			m_pSkyMesh->m_pMaterials[i].Ambient.r = lightColor.x;
			m_pSkyMesh->m_pMaterials[i].Ambient.g = lightColor.y;
			m_pSkyMesh->m_pMaterials[i].Ambient.b = lightColor.z;
		}
	}
	m_pd3dDevice->SetLight(0, &light);
	// Render the  mesh
	// 空の表示
	// Center view matrix for skybox and disable zbuffer
	D3DXMATRIX matView;
	matView = GMatView;
	//		m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );
	float a = 0.0f;
	if (g_World->getChipCount() > 0) {
		a = -g_Chip[0]->X.y / 1000.0f; if (a<-0.1) a = -0.1f; if (a>0.5f) a = 0.5f;
		if (EyePos.y < -3.0) m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);

	}
	matView._41 = 0.0f; matView._42 = a; matView._43 = 0.0f;
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングモード
	m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xb0b0b0);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pSkyMesh->Render(m_pd3dDevice);
	// Restore the render states
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &GMatView);
	m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x000F0F0F);
}
HRESULT CMyD3DApplication::Render()
{
	unsigned int i;
	static int soundCount = 0;
	static GVector preV;
	static GFloat ave1;
	static GVector ave2;
	static int count = 0;
	static int soundValue = 0;
	static DustData pV[GPARTMAX];

	static UserData data[GPLAYERMAX];


	static int viewFlag = 0;

	ViewUpdate = 1;

	//	D3DXMATRIX mat;
	//    D3DXMatrixIdentity( &mat );
		// Clear the viewport
	g_D3DDevice = m_pd3dDevice;
	float w = (FLOAT)m_d3dsdBackBuffer.Width;
	float h = (FLOAT)m_d3dsdBackBuffer.Height;
	//    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200, 230, 255), 1.0f, 0 );
		// Clear the viewport
	m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L);
	if (g_World->NetStop) return S_OK;
	m_pd3dDevice->SetRenderState(D3DRS_DITHERENABLE, DitherFlag);
	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		//スクリプトの呼び出し
		// TODO: update world
		g_FPS = m_fFPS;
		g_MyPlayerData.base_fps = (g_LimitFPS << 16) + (int)(m_fFPS + 0.5);
		s_ScreenWidth = m_d3dsdBackBuffer.Width;
		s_ScreenHeight = m_d3dsdBackBuffer.Height;
		if (CallModeChange && g_SystemLua != NULL && (g_World->B26Bullet || g_DPlay->GetNumPlayers() == 0)) {
			luaSystemRun("OnMode");
			CallModeChange = 0;
		}
		//変数の値がここでさらに更新される。
		if (g_SystemLua != NULL && (g_World->B26Bullet || g_DPlay->GetNumPlayers() == 0)) luaSystemRun("OnFrame");
		for (i = 0;i < GVALMAX;i++) {
			g_ValList[i].Updated = false;
		}
		if (ViewUpdate)	ViewSet();
		if (g_World->Stop == false && g_World->NetStop == false) {
			for (i = 0;i < GOUTPUTMAX;i++) ScriptOutput[i][0] = '\0';
			if (ScriptFlag && waitCount <= 0) {
				if (ScriptType == 1 && g_ScriptLua != NULL) luaScriptRun(g_ScriptLua, "OnFrame");
				else if (ScriptType == 0) RunScript();
			}
		}

		if (viewFlag == 0 && ViewType < 0) {
			HMENU hMenu = GetMenu(m_hWnd);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW1, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW2, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW3, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW4, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW5, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW6, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW7, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW8, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW9, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW10, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEWX, MF_CHECKED);
			viewFlag = 1;
		}
		// Restore the render states
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &GMatView);
		m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x000F0F0F);
		// チップごとの力の表示
		if (ShowPower) {
			//			Line2D(0,0,0.1,0.1,0x33DDDDDD);
			float v;
			//重心の表示-----------------------------------------
//			v=g_Chip[0]->TotalRadius+0.3f;
//			Line(g_Chip[0]->TotalCenterOfGravity-GVector(v,0,0)*g_Chip[0]->R,g_Chip[0]->TotalCenterOfGravity+GVector(v,0,0)*g_Chip[0]->R,0xffDDDDDD);
//			Line(g_Chip[0]->TotalCenterOfGravity-GVector(0,v,0)*g_Chip[0]->R,g_Chip[0]->TotalCenterOfGravity+GVector(0,v,0)*g_Chip[0]->R,0xffDDDDDD);
//			Line(g_Chip[0]->TotalCenterOfGravity-GVector(0,0,v)*g_Chip[0]->R,g_Chip[0]->TotalCenterOfGravity+GVector(0,0,v)*g_Chip[0]->R,0xffDDDDDD);

			for (i = 0;i < (unsigned int)g_World->getChipCount();i++) {
				//if(g_Chip[i]->Top!=g_Chip[0]) continue;
				if (g_Chip[i]->Parent == NULL) {
					v = g_Chip[i]->TotalRadius + 0.3f;
					Line(g_Chip[i]->TotalCenterOfGravity - GVector(v, 0, 0)*g_Chip[i]->R, g_Chip[i]->TotalCenterOfGravity + GVector(v, 0, 0)*g_Chip[i]->R, 0xffDDDDDD);
					Line(g_Chip[i]->TotalCenterOfGravity - GVector(0, v, 0)*g_Chip[i]->R, g_Chip[i]->TotalCenterOfGravity + GVector(0, v, 0)*g_Chip[i]->R, 0xffDDDDDD);
					Line(g_Chip[i]->TotalCenterOfGravity - GVector(0, 0, v)*g_Chip[i]->R, g_Chip[i]->TotalCenterOfGravity + GVector(0, 0, v)*g_Chip[i]->R, 0xffDDDDDD);
				}
				v = (GFloat)g_Chip[i]->Ext.abs() + 1.0f;
				v = log(v)*0.5f;
				Line(g_Chip[i]->X, g_Chip[i]->X + g_Chip[i]->Ext.normalize2()*v, 0xff0000ff);

				GVector na;
				int ht = g_Chip[i]->HitN;
				if (ht >= 1) {
					for (int j = 0;j < ht;j++) {
						na = -(g_Chip[i]->Hit[j].Pos - g_Chip[i]->X).cross(g_Chip[i]->Hit[j].Normal).cross(g_Chip[i]->Hit[j].Normal);
						if (j == 0) Line(g_Chip[i]->Hit[j].Pos, g_Chip[i]->Hit[j].Pos + g_Chip[i]->Hit[j].FricV / (GFloat)GDTSTEP*0.08f, 0xffff0000);
						else Line(g_Chip[i]->Hit[j].Pos, g_Chip[i]->Hit[j].Pos + g_Chip[i]->Hit[j].FricV*0.08f, 0xffff0000);
						v = (GFloat)g_Chip[i]->Hit[j].J.abs() + 1.0f;
						v = (GFloat)log(v)*0.5f;
						Line(g_Chip[i]->Hit[j].Pos, g_Chip[i]->Hit[j].Pos + g_Chip[i]->Hit[j].J.normalize2()*v, 0xff00ff00);
					}
				}
			}
		}

		if (ViewType >= 0) viewFlag = 0;

		//チップの表示

		m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
		m_pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		m_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_XRGB((int)(FogColor.x*lightColor.x), (int)(FogColor.y*lightColor.y), (int)(FogColor.z*lightColor.z)));
		m_pd3dDevice->SetRenderState(D3DRS_FOGSTART, FtoDW((float)(20.0f)));
		m_pd3dDevice->SetRenderState(D3DRS_FOGEND, FtoDW((float)(g_FarMax - 10.0f)));
		m_pd3dDevice->SetRenderState(D3DRS_FOGDENSITY, FtoDW(0.2f));
		m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		g_World->LandRigid->Disp();
		//		g_World->Disp(g_DPlay->GetNumPlayers()!=0);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  //DESTの設定

		g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		if (g_DPlay->GetNumPlayers() != 0) {
			m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングモード
			m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  //DESTの設定
			m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			FLOAT *f = (float*)GMatView;
			GMatrix vm(f);
			for (i = 0;i < (unsigned int)g_DPlay->GetMaxPlayers();i++) {
				float hy = g_PlayerData[i].y + (float)pow((double)g_PlayerData[i].ChipCount, 0.3333) / 3.0f;
				GVector infoV = GVector(g_PlayerData[i].x, hy + 1.2f, g_PlayerData[i].z)*vm;
				data[i].z = infoV.z;
				data[i].n = i;
			}
			//ソートする
			qsort((void*)data, GPLAYERMAX, sizeof(UserData), UserCompare);
			for (i = 0;i < (unsigned int)g_DPlay->GetMaxPlayers();i++) {
				g_World->DispNetChipInfo(data[i].n, data[i].z);
			}
		}
		if (g_DPlay->GetNumPlayers() != 0) {
			//ネットチップの表示
			m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングモード
			m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  //DESTの設定
			m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			for (i = 0;i < (unsigned int)g_DPlay->GetCurrentPlayers();i++) {
				g_World->DispNetChip(i);
			}
		}
		g_World->Disp(0);

		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  //DESTの設定
		{	//水面の表示
			g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// カリングモード
			g_D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
			g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			D3DXMATRIX mat1;
			D3DXMATRIX mat2;
			float x, z;
			int ii = (int)(g_FarMax / 10);
			int s = (int)(g_FarMax / 600);
			if (s == 0) {
				x = (int)(EyePos.x / 60)*60.0f;
				z = (int)(EyePos.z / 60)*60.0f;
				D3DXMatrixTranslation(&mat1, x + (GFloat)sin(count / 90.0*M_PI), WATER_LINE, z);
				D3DXMatrixScaling(&mat2, 2.0, 1.0f, 2.0);
				D3DXMatrixMultiply(&mat1, &mat2, &mat1);
				D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);
				m_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
				m_pXMesh[14]->Render(g_D3DDevice);
			}
			else {
				int xx = (int)(EyePos.x / 60) * 60;
				int zz = (int)(EyePos.z / 60) * 60;
				float si = (GFloat)sin(count / 88.0*M_PI) - 30;
				D3DXMatrixScaling(&mat2, 2.0f, 1.0f, 2.0f);
				for (int i = 0;i < s;i++) {
					for (int j = 0;j < s;j++) {
						x = (float)(xx + i * 1200 - (s - 1) * 600);
						z = (float)(zz + j * 1200 - (s - 1) * 600);
						D3DXMatrixTranslation(&mat1, x + si, WATER_LINE, z - 30);
						D3DXMatrixMultiply(&mat1, &mat2, &mat1);
						D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);
						m_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
						m_pXMesh[14]->Render(g_D3DDevice);
					}
				}
			}
			count++;
		}
		g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		g_World->DispJet(g_D3DDevice, GMatWorld, m_pXMesh[33], m_pXMesh[11], JetFlag);
		g_World->DispNetJetAll();

		//		m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
		int i, j = 0;
		//		for(int i=CurrentCheckPoint;i<Course[CurrentCourse].Count;i++) CheckPointWork[j++]=i;
		//		if(CurrentCheckPoint<Course[CurrentCourse].Count) qsort(CheckPointWork,Course[CurrentCourse].Count-CurrentCheckPoint,sizeof(int),CheckPointCompare);

		for (int ii = 0;ii < GRINGMAX;ii++) {	//リングの表示
			if (Ring[ii].State == 0) continue;
			D3DXMATRIX mat1, mat2;
			float s = Ring[ii].Scale / 5.0f;
			D3DXMatrixScaling(&mat1, s, s, s);
			D3DXMatrixRotationX(&mat2, (FLOAT)D3DXToRadian(Ring[ii].Dir.x));
			D3DXMatrixMultiply(&mat1, &mat1, &mat2);
			D3DXMatrixRotationY(&mat2, (FLOAT)D3DXToRadian(Ring[ii].Dir.y));
			D3DXMatrixMultiply(&mat1, &mat1, &mat2);
			D3DXMatrixRotationZ(&mat2, (FLOAT)D3DXToRadian(Ring[ii].Dir.z));
			D3DXMatrixMultiply(&mat1, &mat1, &mat2);
			D3DXMatrixTranslation(&mat2, (float)Ring[ii].Point.x, (float)Ring[ii].Point.y, (float)Ring[ii].Point.z);
			D3DXMatrixMultiply(&mat1, &mat1, &mat2);
			D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
			if (Ring[ii].State == 1) {
				m_pXMesh[15]->m_pMaterials[0].Diffuse.r = Ring[ii].Color.x;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.g = Ring[ii].Color.y;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.b = Ring[ii].Color.z;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.a = 0.2f;
				m_pXMesh[15]->m_pMaterials[0].Ambient = m_pXMesh[15]->m_pMaterials[0].Diffuse;
			}
			else {
				m_pXMesh[15]->m_pMaterials[0].Diffuse.r = Ring[ii].Color.x;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.g = Ring[ii].Color.y;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.b = Ring[ii].Color.z;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.a = 0.7f;
				m_pXMesh[15]->m_pMaterials[0].Ambient = m_pXMesh[15]->m_pMaterials[0].Diffuse;
			}
			m_pXMesh[15]->Render(m_pd3dDevice);
		}
		for (int ii = 0;ii < Course[CurrentCourse].Count;ii++) {	//チェックポイントの表示
			D3DXMATRIX mat1, mat2;
			float s = Course[CurrentCourse].Scale[ii] / 5.0f;
			D3DXMatrixScaling(&mat1, s, s, s);
			D3DXMatrixRotationX(&mat2, (FLOAT)D3DXToRadian(Course[CurrentCourse].Dir[ii].x));
			D3DXMatrixMultiply(&mat1, &mat1, &mat2);
			D3DXMatrixRotationY(&mat2, (FLOAT)D3DXToRadian(Course[CurrentCourse].Dir[ii].y));
			D3DXMatrixMultiply(&mat1, &mat1, &mat2);
			D3DXMatrixRotationZ(&mat2, (FLOAT)D3DXToRadian(Course[CurrentCourse].Dir[ii].z));
			D3DXMatrixMultiply(&mat1, &mat1, &mat2);
			D3DXMatrixTranslation(&mat2, (float)Course[CurrentCourse].Point[ii].x, (float)Course[CurrentCourse].Point[ii].y, (float)Course[CurrentCourse].Point[ii].z);
			D3DXMatrixMultiply(&mat1, &mat1, &mat2);
			D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
			if (ii == CurrentCheckPoint) {
				m_pXMesh[15]->m_pMaterials[0].Diffuse.r = 0;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.g = 0.3f;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.b = 1;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.a = 0.7f;
				m_pXMesh[15]->m_pMaterials[0].Ambient = m_pXMesh[15]->m_pMaterials[0].Diffuse;
			}
			else {
				m_pXMesh[15]->m_pMaterials[0].Diffuse.r = 1;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.g = 1;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.b = 1;
				m_pXMesh[15]->m_pMaterials[0].Diffuse.a = 0.2f;
				m_pXMesh[15]->m_pMaterials[0].Ambient = m_pXMesh[15]->m_pMaterials[0].Diffuse;
			}
			m_pXMesh[15]->Render(m_pd3dDevice);
		}
		m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングモード
		m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);



		int k = 0;
		//砂塵の表示
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  //DESTの設定
		soundCount--;
		if (soundCount < 0) soundCount = 0;
		if (ShowDustFlag) {
			//			m_pd3dDevice->SetTransform( D3DTS_WORLD, &mat );
			double noiseMax = 0.6f;
			int nFlag = false;
			for (i = 0;i < g_GroundParticle->MaxVertex;i++) {
				if (g_GroundParticle->Vertex[i].Life>0) {
					pV[k].x = (float)g_GroundParticle->Vertex[i].Pos.x;
					pV[k].y = (float)g_GroundParticle->Vertex[i].Pos.y;
					pV[k].z = (float)g_GroundParticle->Vertex[i].Pos.z;
					pV[k].id = i;

					float a = g_GroundParticle->Vertex[i].Life;
					if (g_GroundParticle->Vertex[i].Life < 0) a = 0.0f;
					pV[k].size = g_GroundParticle->Vertex[i].Size;
					pV[k].alpha = a;
					if (g_GroundParticle->Vertex[i].Life > 0.6) {
						if (noiseMax < g_GroundParticle->Vertex[i].Life) noiseMax = g_GroundParticle->Vertex[i].Life;
						nFlag = true;
					}
					k++;
					if (k >= GPARTMAX) break;
				}
			}
			if (hMidiOut != NULL && SoundType == 1) {
				long msg;
				static int preSoundValue = 0;
				if (nFlag) {
					soundValue = (int)(0x4f * (noiseMax - 0.6f));
					if (soundValue != preSoundValue) {
						msg = MAKELONG(MAKEWORD(0xB1, 0x07), MAKEWORD(soundValue, 0));
						midiOutShortMsg(hMidiOut, msg); // Note On
						preSoundValue = soundValue;

					}
				}
				else {
					msg = MAKELONG(MAKEWORD(0xB1, 0x07), MAKEWORD(0x00, 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
					preSoundValue = 0;
				}
			}
			if (k > 0) {
				m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x444444);
				m_pd3dDevice->SetVertexShader(D3DFVF_POINTVERTEX);
				m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				m_pXMesh[18]->m_pMaterials[0].Ambient.r = (float)lightColor.x;
				m_pXMesh[18]->m_pMaterials[0].Ambient.g = (float)lightColor.y;
				m_pXMesh[18]->m_pMaterials[0].Ambient.b = (float)lightColor.z;
				m_pXMesh[18]->m_pMaterials[0].Diffuse.r = (float)0;
				m_pXMesh[18]->m_pMaterials[0].Diffuse.g = (float)0;
				m_pXMesh[18]->m_pMaterials[0].Diffuse.b = (float)0;

				m_pXMesh[19]->m_pMaterials[0].Ambient.r = (float)lightColor.x;
				m_pXMesh[19]->m_pMaterials[0].Ambient.g = (float)lightColor.y;
				m_pXMesh[19]->m_pMaterials[0].Ambient.b = (float)lightColor.z;
				m_pXMesh[19]->m_pMaterials[0].Diffuse.r = (float)0;
				m_pXMesh[19]->m_pMaterials[0].Diffuse.g = (float)0;
				m_pXMesh[19]->m_pMaterials[0].Diffuse.b = (float)0;

				m_pXMesh[29]->m_pMaterials[0].Ambient.r = (float)lightColor.x;
				m_pXMesh[29]->m_pMaterials[0].Ambient.g = (float)lightColor.y;
				m_pXMesh[29]->m_pMaterials[0].Ambient.b = (float)lightColor.z;
				m_pXMesh[29]->m_pMaterials[0].Diffuse.r = (float)0;
				m_pXMesh[29]->m_pMaterials[0].Diffuse.g = (float)0;
				m_pXMesh[29]->m_pMaterials[0].Diffuse.b = (float)0;
				D3DXMATRIX mat1, mat2;
				for (i = 0;i < k;i++) {
					m_pXMesh[18]->m_pMaterials[0].Diffuse.a = (float)pV[i].alpha*0.3f;
					D3DXMatrixRotationZ(&mat1, pV[i].alpha*2.0f + pV[i].id);
					D3DXMatrixScaling(&mat2, pV[i].size, pV[i].size, pV[i].size);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					mat2 = GMatView;
					mat2._41 = 0.0f; mat2._42 = 0.0f; mat2._43 = 0.0f;
					D3DXMatrixInverse(&mat2, NULL, &mat2);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					D3DXMatrixTranslation(&mat2, pV[i].x, pV[i].y, pV[i].z);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);
					m_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
					m_pXMesh[18]->Render(m_pd3dDevice);
				}
			}
			//水しぶき
			k = 0;
			for (i = 0;i < g_WaterLineParticle->MaxVertex;i++) {
				if (g_WaterLineParticle->Vertex[i].Life>0) {
					pV[k].x = (float)g_WaterLineParticle->Vertex[i].Pos.x;
					pV[k].y = (float)g_WaterLineParticle->Vertex[i].Pos.y;
					pV[k].z = (float)g_WaterLineParticle->Vertex[i].Pos.z;
					pV[k].r = (float)g_WaterLineParticle->Vertex[i].Color.x;
					pV[k].g = (float)g_WaterLineParticle->Vertex[i].Color.y;
					pV[k].b = (float)g_WaterLineParticle->Vertex[i].Color.z;
					pV[k].id = i;

					float a = g_WaterLineParticle->Vertex[i].Life;
					if (g_WaterLineParticle->Vertex[i].Life < 0) a = 0.0f;
					pV[k].size = g_WaterLineParticle->Vertex[i].Size;
					pV[k].alpha = a;
					k++;
					if (k >= GPARTMAX) break;
					/*
										if(g_WaterLineParticle->Vertex[i].Life>0.6) {
											if(noiseMax<g_WaterLineParticle->Vertex[i].Life) noiseMax=g_GroundParticle->Vertex[i].Life;
											nFlag=true;
										}
					*/
				}
			}
			if (k > 0) {
				m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
				m_pd3dDevice->SetVertexShader(D3DFVF_POINTVERTEX);
				m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

				D3DXMATRIX mat1, mat2;
				for (i = 0;i < k;i++) {
					m_pXMesh[19]->m_pMaterials[0].Diffuse.a = (float)pV[i].alpha*0.3f;
					D3DXMatrixRotationZ(&mat1, (FLOAT)pV[i].id);
					D3DXMatrixScaling(&mat2, pV[i].size, pV[i].size, pV[i].size);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					mat2 = GMatView;
					mat2._41 = 0.0f; mat2._42 = 0.0f; mat2._43 = 0.0f;
					D3DXMatrixInverse(&mat2, NULL, &mat2);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					D3DXMatrixTranslation(&mat2, pV[i].x, pV[i].y, pV[i].z);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);
					m_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
					m_pXMesh[19]->m_pMaterials[0].Ambient.r = (float)pV[i].r;
					m_pXMesh[19]->m_pMaterials[0].Ambient.g = (float)pV[i].g;
					m_pXMesh[19]->m_pMaterials[0].Ambient.b = (float)pV[i].b;
					m_pXMesh[19]->Render(m_pd3dDevice);
				}
			}
		}

		//飛行機雲
		k = 0;
		for (i = 0;i < g_JetParticle->MaxVertex;i++) {
			if (g_JetParticle->Vertex[i].Life>0) {
				pV[k].type = g_JetParticle->Vertex[i].Type;
				pV[k].x = (float)g_JetParticle->Vertex[i].Pos.x;
				pV[k].y = (float)g_JetParticle->Vertex[i].Pos.y;
				pV[k].z = (float)g_JetParticle->Vertex[i].Pos.z;
				pV[k].r = (float)g_JetParticle->Vertex[i].Color.x;
				pV[k].g = (float)g_JetParticle->Vertex[i].Color.y;
				pV[k].b = (float)g_JetParticle->Vertex[i].Color.z;
				pV[k].id = i;

				float a = g_JetParticle->Vertex[i].Life;
				if (g_JetParticle->Vertex[i].Life < 0) a = 0.0f;
				pV[k].size = g_JetParticle->Vertex[i].Size;
				pV[k].alpha = a*a;
				k++;
				if (k >= GPARTMAX) break;
			}
		}
		if (k > 0) {
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
			m_pd3dDevice->SetVertexShader(D3DFVF_POINTVERTEX);
			m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			D3DXMATRIX mat1, mat2;
			for (i = 0;i < k;i++) {
				if (pV[i].type != 0 || ShowDustFlag) {
					CD3DMesh *mesh;
					if (pV[i].type == 0) {
						if (pV[i].y <= WATER_LINE) mesh = m_pXMesh[19];
						else mesh = m_pXMesh[29];
					}
					else if (pV[i].type == 1) mesh = m_pXMesh[35];
					else mesh = m_pXMesh[36];
					mesh->m_pMaterials[0].Ambient.r = (float)pV[i].r;
					mesh->m_pMaterials[0].Ambient.g = (float)pV[i].g;
					mesh->m_pMaterials[0].Ambient.b = (float)pV[i].b;
					mesh->m_pMaterials[0].Diffuse.a = (float)pV[i].alpha*0.3f;
					//					mesh->m_pMaterials[0].Ambient=mesh->m_pMaterials[0].Diffuse;
					D3DXMatrixRotationZ(&mat1, (FLOAT)pV[i].id);
					D3DXMatrixScaling(&mat2, pV[i].size, pV[i].size, pV[i].size);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					mat2 = GMatView;
					mat2._41 = 0.0f; mat2._42 = 0.0f; mat2._43 = 0.0f;
					D3DXMatrixInverse(&mat2, NULL, &mat2);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					D3DXMatrixTranslation(&mat2, pV[i].x, pV[i].y, pV[i].z);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);
					m_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
					mesh->Render(m_pd3dDevice);
				}
			}
		}
		//				m_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST,0,k);

		m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x000F0F0F);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pd3dDevice->LightEnable(0, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		//弾丸
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
		m_pd3dDevice->SetVertexShader(D3DFVF_POINTVERTEX);
		m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// カリングモード
		m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);   //SRCの設定
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);  //DESTの設定
		D3DXMATRIX mat1, mat2;
		k = 0;
		//		GVector v=(EyePos-RefPos).normalize2();
		for (i = 0;i < g_Bullet->MaxVertex;i++) {
			if (g_Bullet->Vertex[i].Life>0 && g_Bullet->Vertex[i].Life != 1200.0f) {
				//GVector v=g_Bullet->Vertex[i].Vec.normalize()*g_Bullet->Vertex[i].Size*10;
				GVector v = g_Bullet->Vertex[i].Vec;
				GFloat f = fabs((g_Bullet->Vertex[i].Pos - g_Bullet->Vertex[i].Vec / 2 - EyePos).normalize2().dot(v.normalize2()));
				int fn = (int)((4 + (int)((1.0f - f*f) * 15)) / g_Bullet->Vertex[i].Size);
				if (fn > 50) fn = 50;
				for (int j = 0;j < fn;j++) {
					float x = (float)g_Bullet->Vertex[i].Pos.x - v.x + v.x*j / fn;
					float y = (float)g_Bullet->Vertex[i].Pos.y - v.y + v.y*j / fn;
					float z = (float)g_Bullet->Vertex[i].Pos.z - v.z + v.z*j / fn;

					float size = g_Bullet->Vertex[i].Size;
					//pV[k].alpha=1.0f-j/(float)fn;
					float alpha = 1.0f;
					float va = v.abs();
					alpha = (float)j / fn;
					if ((g_Bullet->Vertex[i].Dist + va - va*(float)j / fn) <= 0) alpha = 0.0f;

					CD3DMesh *mesh;
					mesh = m_pXMesh[32];
					mesh->m_pMaterials[0].Diffuse.a = alpha;
					D3DXMatrixRotationZ(&mat1, (FLOAT)i);
					D3DXMatrixScaling(&mat2, size, size, size);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					mat2 = GMatView;
					mat2._41 = 0.0f; mat2._42 = 0.0f; mat2._43 = 0.0f;
					D3DXMatrixInverse(&mat2, NULL, &mat2);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					D3DXMatrixTranslation(&mat2, x, y, z);
					D3DXMatrixMultiply(&mat1, &mat1, &mat2);
					D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);
					m_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
					mesh->Render(m_pd3dDevice);
				}
			}
		}

		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングモード
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  //DESTの設定
		m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x000F0F0F);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pd3dDevice->LightEnable(0, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);



		// メーターの表示
		D3DVIEWPORT8 viewData = { (DWORD)0, (DWORD)0, (DWORD)w, (DWORD)h, 0.0f, 1.0f };
		D3DVIEWPORT8 viewData1 = { (DWORD)(w - 128 - 94 - 94 - 79), (DWORD)(h - 71), 64, 64, 0.0f, 1.0f };
		D3DVIEWPORT8 viewData2 = { (DWORD)(w - 128 - 94 - 94 - 94 - 79), (DWORD)(h - 71), 64, 64, 0.0f, 1.0f };
		D3DXMATRIX matProj;
		D3DXMATRIX matV;
		D3DXMATRIX mat;
		D3DXVECTOR3 vFromPt;
		D3DXVECTOR3 vLookatPt;
		D3DXVECTOR3 vUpVec;
		if (w - 128 - 94 - 94 - 79 > 0) {
			if (ShowMeter) {
				GMatrix m(g_Chip[0]->R);
				D3DXMATRIX matLocal = D3DXMATRIX((FLOAT)m.elem[0][0], (FLOAT)m.elem[0][1], (FLOAT)m.elem[0][2], (FLOAT)m.elem[0][3],
					(FLOAT)m.elem[1][0], (FLOAT)m.elem[1][1], (FLOAT)m.elem[1][2], (FLOAT)m.elem[1][3],
					(FLOAT)m.elem[2][0], (FLOAT)m.elem[2][1], (FLOAT)m.elem[2][2], (FLOAT)m.elem[2][3],
					(FLOAT)m.elem[3][0], (FLOAT)m.elem[3][1], (FLOAT)m.elem[3][2], (FLOAT)m.elem[3][3]);
				D3DXMatrixScaling(&mat, 0.5, 0.5, 0.5);
				D3DXMatrixMultiply(&matLocal, &mat, &matLocal);
				D3DXMatrixMultiply(&matLocal, &matLocal, &GMatWorld);
				g_D3DDevice->SetTransform(D3DTS_WORLD, &matLocal);

				GVector rv = (GVector(0, 0, 1)*g_Chip[0]->R).Cut2(GVector(0, 1, 0)).normalize2()*3.0f;
				vFromPt = D3DXVECTOR3((float)rv.x, (float)rv.y, (float)rv.z);
				vLookatPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				D3DXMatrixLookAtLH(&matV, &vFromPt, &vLookatPt, &vUpVec);
				// Set the projection matrix
				D3DXMatrixPerspectiveFovLH(&matProj, 0.1f, 1.0f, 1.0f, g_FarMax);
				m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
				m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
				m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
				m_pd3dDevice->SetViewport(&viewData1);
				m_pd3dDevice->SetTransform(D3DTS_VIEW, &matV);
				m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
				m_pXMesh[12]->Render(g_D3DDevice);
			}
			m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			g_D3DDevice->SetTransform(D3DTS_WORLD, &GMatWorld);
			if (w - 128 - 94 - 94 - 94 - 79 > 0 && CCDFlag && ShowMeter) {
				D3DXMatrixPerspectiveFovLH(&matProj, CCDZoom*(FLOAT)M_PI / 180.0f, 1.0f, 1.0f, 300.0f);
				m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
				GVector lv0 = g_Chip[0]->X + (GVector(0, 0, 1)*g_Chip[0]->R)*0.75f;
				GVector lv = g_Chip[0]->X + (GVector(0, 0, -1)*g_Chip[0]->R)*3.0f;
				GVector lvy = GVector(0, 1, 0)*g_Chip[0]->R;
				vFromPt = D3DXVECTOR3(lv0.x, lv0.y, lv0.z);
				vLookatPt = D3DXVECTOR3(lv.x, lv.y, lv.z);
				vUpVec = D3DXVECTOR3(lvy.x, lvy.y, lvy.z);
				D3DXMatrixLookAtLH(&matV, &vFromPt, &vLookatPt, &vUpVec);
				m_pd3dDevice->SetViewport(&viewData2);
				m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB((int)FogColor.x, (int)FogColor.y, (int)FogColor.z), 1.0f, 0);
				m_pd3dDevice->SetTransform(D3DTS_VIEW, &matV);
				m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x000f0f0f);
				g_pLandMesh->Render(g_D3DDevice);
				g_World->Disp2();
				g_World->ObjectDisp();
				//ネットチップの表示
				m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
				m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  //DESTの設定
				for (i = 0;i < g_DPlay->GetNumPlayers();i++) {
					g_World->DispNetChip(i);
				}
				{	//水面の表示
					D3DXMATRIX mat1;
					float x, z;
					x = (int)(g_Chip[0]->X.x / 50)*50.0f;
					z = (int)(g_Chip[0]->X.z / 50)*50.0f;
					D3DXMatrixTranslation(&mat1, x, WATER_LINE, z);
					D3DXMatrixMultiply(&mat1, &mat1, &GMatWorld);
					g_D3DDevice->SetTransform(D3DTS_WORLD, &mat1);
					g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
					g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// カリングモード
					m_pXMesh[14]->Render(g_D3DDevice);
					g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングモード
					g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
					/*					{	//Zバッファからの深度情報読み込み（D3DFMT_D16_LOCKABLEでないと無理
					D3DLOCKED_RECT zLock;
					LPDIRECT3DSURFACE8 ZBuffer;
					float widthRatio = 1.0f;
					unsigned char *pLine,*pBase;
					m_pd3dDevice->GetDepthStencilSurface(&ZBuffer);
					ZBuffer->LockRect(&zLock, NULL,0);//D3DLOCK_READONLY
					pBase = (unsigned char *)zLock.pBits;

					  for( int y = 0; y < 64; y++ )
					  {
					  pLine = &pBase[(y+viewData2.Y)*zLock.Pitch+viewData2.X*2];
					  for(int x = 0; x < 64; x++ )
					  {
					  int a=pLine[0]+pLine[1]*256;
					  //							   pLine[0] =  0xe0;  // red, lower 4 bits
					  //							   pLine[1] =  0x07;  // red, lower 4 bits
					  pLine += 2;
					  CCDImage[x][y]=a;
					  }
					  }

						ZBuffer->UnlockRect();
						ZBuffer->Release();
						}
					*/
					{	//バックバッファからの色情報読み込み
						D3DLOCKED_RECT Lock;
						LPDIRECT3DSURFACE8 backBuffer;
						float widthRatio = 1.0f;
						unsigned char *pLine, *pBase;
						m_pd3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
						backBuffer->LockRect(&Lock, NULL, D3DLOCK_READONLY);//D3DLOCK_READONLY 
						pBase = (unsigned char *)Lock.pBits;

						if ((int)(Lock.Pitch / w) == 2) {
							int y;
							for (y = 0; y < CCD_RESOLUTION; y++)
							{
								pLine = &pBase[(y + viewData2.Y)*Lock.Pitch + viewData2.X * 2];
								for (int x = 0; x < CCD_RESOLUTION; x++)
								{
									int v = pLine[0] + pLine[1] * (CCD_RESOLUTION*4);
									int r = v >> 11;
									int g = (v & 0x7e0) >> 6;
									int b = v & 0x1f;
									CCDImage[x][y] = (r << 10) + (g << 5) + b;
									pLine += 2;
								}
							}
						}
						else if ((int)(Lock.Pitch / w) == 4) {
							int y;
							for (y = 0; y < 64; y++)
							{
								pLine = &pBase[(y + viewData2.Y)*Lock.Pitch + viewData2.X * 4];
								for (int x = 0; x < 64; x++)
								{
									int b = pLine[0] >> 3;
									int g = pLine[1] >> 3;
									int r = pLine[2] >> 3;
									CCDImage[x][y] = (r << 10) + (g << 5) + b;
									pLine += 4;
								}
							}
						}

						backBuffer->UnlockRect();
						backBuffer->Release();
					}
				}
			}
			else {
				for (int y = 0; y < 64; y++) {
					for (int x = 0; x < 64; x++) {
						CCDImage[x][y] = 0;
					}
				}
			}
			m_pd3dDevice->SetViewport(&viewData);
		}
	}





	pSprite->Begin();

	RECT srcRect;

	D3DXVECTOR2 v(64, 64);
	D3DXVECTOR2 v1(32, 20);
	D3DXVECTOR2 v2(48, 48);
	D3DXVECTOR2 v3(24, 15);
	D3DXVECTOR2 v4(24, 0);
	//#if (D3D_SDK_VERSION >= 220)
	D3DXVECTOR2 s((FLOAT)0.75f, (FLOAT)0.75f);
	D3DXVECTOR2 s2((FLOAT)0.75f, (FLOAT)0.75f);
	D3DXVECTOR2 s3((FLOAT)1.0f, (FLOAT)1.0f);
	//#else

	//	D3DXVECTOR2 s((FLOAT)0.75f,(FLOAT)0.75f);
	//	D3DXVECTOR2 s2((FLOAT)0.75f,(FLOAT)1.5f);
	//	D3DXVECTOR2 s3((FLOAT)1.0f,(FLOAT)2.0f);
	//#endif
	D3DXVECTOR2 pos;
	D3DXVECTOR2 scale;
	// スクリーン縁処理
	SetRect(&srcRect, 0, 0, 16, 16);
	pos.x = 0.0f;
	pos.y = 0.0f;
	pSprite->Draw(pMyTexture[19], &srcRect, NULL, NULL, NULL, &pos, 0xffffffff);

	SetRect(&srcRect, 0, 16, 16, 32);
	pos.x = 0.0f;
	pos.y = h - 16.0f;
	pSprite->Draw(pMyTexture[19], &srcRect, NULL, NULL, NULL, &pos, 0xffffffff);

	SetRect(&srcRect, 16, 0, 32, 16);
	pos.x = w - 16.0f;
	pos.y = 0.0f;
	pSprite->Draw(pMyTexture[19], &srcRect, NULL, NULL, NULL, &pos, 0xffffffff);

	SetRect(&srcRect, 16, 16, 32, 32);
	pos.x = w - 16.0f;
	pos.y = h - 16.0f;
	pSprite->Draw(pMyTexture[19], &srcRect, NULL, NULL, NULL, &pos, 0xffffffff);
	// 状態
	if (ShowRegulation) {
		if (CurrentCourse != 0) {
			scale.x = 0.5;
			scale.y = 0.5;
			pos.x = w - 80.0f;
			pos.y = 60.0f;
			pSprite->Draw(pMyTexture[21], NULL, &scale, NULL, 0, &pos, 0xDDffffff);
			//			pos.x=w-70.0f;
			//			pos.y=80.0f;
			//			pSprite->Draw(pMyTexture[17],NULL,NULL,NULL,0,&pos,0x99ffffff);
		}
		float p = 236.0f;
		pos.x = w - p;
		pos.y = 11.0f;
		pSprite->Draw(pMyTexture[18], NULL, NULL, NULL, 0, &pos, 0x99ffffff);
		for (int i = 0;i < 8;i++) {
			pos.x = w - (p - 32.0f) + i*24.0f;
			pos.y = 11.0f;
			pSprite->Draw(pMyTexture[16], NULL, NULL, NULL, 0, &pos, 0x99ffffff);
		}
		if (KeyRecordMode != 0) {
			pos.x = w - 58.0f + (CurrentCourse != 0 ? -78 : 0);
			pos.y = 61.0f + (CurrentCourse == 0 ? -20 : 0);
			pSprite->Draw(pMyTexture[17], NULL, NULL, NULL, 0, &pos, 0x99ffffff);
		}
	}
	// スピードメーター
	if (ShowMeter) {
		/*
		//記録計
		D3DXVECTOR2 scale;
		pos.x=w-114.0f;
		pos.y=h-140;
		scale.x=2;scale.y=1;
		pSprite->Draw(pMyTexture[17],NULL,&scale,NULL,0,&pos,0x99ffffff);
		*/
		// 盤
		pos.x = w - 128.0f;
		pos.y = h - 117.0f;
		pSprite->Draw(pMyTexture[0], NULL, NULL, NULL, D3DXToRadian(0), &pos, 0x99ffffff);
		float d = -(float)g_Chip[0]->V.abs()*3.6f*270.0f / 450.0f;
		ave1 = ave1*0.8f + (-g_Chip[0]->W.y)*15.0f*0.2f;
		float d2 = ave1 + 180.0f;
		// 針の影2
		pos.x = w - 128 + 32.0f + 2.0f;
		pos.y = h - 117.0f + 44.0f + 1.0f;
		pSprite->Draw(pMyTexture[10], NULL, NULL, &v1, D3DXToRadian(d2), &pos, 0x77ffffff);
		// 針2
		pos.x = w - 128.0f + 32.0f;
		pos.y = h - 117.0f + 44.0f;
		pSprite->Draw(pMyTexture[9], NULL, NULL, &v1, D3DXToRadian(d2), &pos, 0xffffffff);
		// 針の影
		pos.x = w - 128 + 32.0f + 3.0f;
		pos.y = h - 117.0f + 44.0f + 2.0f;
		pSprite->Draw(pMyTexture[6], NULL, NULL, &v1, D3DXToRadian(d), &pos, 0x77ffffff);
		// 針
		pos.x = w - 128.0f + 32.0f;
		pos.y = h - 117.0f + 44.0f;
		pSprite->Draw(pMyTexture[5], NULL, NULL, &v1, D3DXToRadian(d), &pos, 0xffffffff);

		// 高度計
		// 盤
		pos.x = w - 94 - 128.0f;
		pos.y = h - 86.0f;
		pSprite->Draw(pMyTexture[1], NULL, &s, NULL, D3DXToRadian(0), &pos, 0x99ffffff);


		// 針の影2
		pos.x = w - 94.0f - 128.0f + 24.0f + 2.0f;
		pos.y = h - 86.0f + 32.0f + 1.0f;
		d2 = -(float)g_Chip[0]->X.y*360.0f / 10000.0f;
		pSprite->Draw(pMyTexture[10], NULL, &s2, &v3, D3DXToRadian(d2), &pos, 0x77ffffff);
		// 針2
		pos.x = w - 94.0f - 128.0f + 24.0f;
		pos.y = h - 86.0f + 32.0f;
		pSprite->Draw(pMyTexture[9], NULL, &s2, &v3, D3DXToRadian(d2), &pos, 0xffffffff);
		// 針の影
		pos.x = w - 94.0f - 128.0f + 24.0f + 3.0f;
		pos.y = h - 86.0f + 32.0f + 2.0f;
		d = -(float)g_Chip[0]->X.y*360.0f / 100.0f;
		pSprite->Draw(pMyTexture[6], NULL, &s2, &v3, D3DXToRadian(d), &pos, 0x77ffffff);
		// 針
		pos.x = w - 94.0f - 128.0f + 24.0f;
		pos.y = h - 86.0f + 32.0f;
		pSprite->Draw(pMyTexture[5], NULL, &s2, &v3, D3DXToRadian(d), &pos, 0xffffffff);

		// 出力計
		// 盤
		pos.x = w - 94.0f - 94.0f - 128.0f;
		pos.y = h - 86.0f;
		pSprite->Draw(pMyTexture[2], NULL, &s, NULL, D3DXToRadian(0), &pos, 0x99ffffff);

		/*
				// 針の影2
				pos.x=w-94-94-128+24.0f+2.0f;
				pos.y=h-86.0f+32.0f+1.0f;
				ave2=ave2*0.8f+(g_Chip[0]->V-preV)/g_World->Dt/2.0f*0.2f;
				d2=-ave2.abs()+90.0f;
				preV=g_Chip[0]->V;
				pSprite->Draw(pMyTexture[10],NULL,&s2,&v3,D3DXToRadian(d2),&pos,0x77ffffff);
				// 針2
				pos.x=w-94.0f-94.0f-128.0f+24.0f;
				pos.y=h-86.0f+32.0f;
				pSprite->Draw(pMyTexture[9],NULL,&s2,&v3,D3DXToRadian(d2),&pos,0xffffffff);
		*/
		// 針の影2
		pos.x = w - 94 - 94 - 128 + 24.0f + 2.0f;
		pos.y = h - 86.0f + 32.0f + 1.0f;
		d2 = -g_Chip[0]->TotalFuel / g_Chip[0]->TotalFuelMax * 180 + 90.0f;
		pSprite->Draw(pMyTexture[10], NULL, &s2, &v3, D3DXToRadian(d2), &pos, 0x77ffffff);
		// 針2
		pos.x = w - 94.0f - 94.0f - 128.0f + 24.0f;
		pos.y = h - 86.0f + 32.0f;
		pSprite->Draw(pMyTexture[9], NULL, &s2, &v3, D3DXToRadian(d2), &pos, 0xffffffff);
		// 針の影
		pos.x = w - 94 - 94 - 128 + 24.0f + 3.0f;
		pos.y = h - 86.0f + 32.0f + 2.0f;
		d = -(float)log(TotalPower / 100.0f + 1.0f)*180.0f / 10.0f - 90.0f;
		pSprite->Draw(pMyTexture[6], NULL, &s2, &v3, D3DXToRadian(d), &pos, 0x77ffffff);
		// 針
		pos.x = w - 94.0f - 94.0f - 128.0f + 24.0f;
		pos.y = h - 86.0f + 32.0f;
		pSprite->Draw(pMyTexture[5], NULL, &s2, &v3, D3DXToRadian(d), &pos, 0xffffffff);

		// 盤
		if (w - 128 - 94 - 94 - 79 > 0) {

			GVector dir = GVector(0, 0, 1)*g_Chip[0]->R;
			d2 = -(dir).Cut2(GVector(0, 1, 0)).angle2(GVector(0, 0, 1), GVector(0, 1, 0))*180.0f / (float)M_PI;
			pos.x = w - 94.0f - 94.0f - 94.0f - 128.0f;
			pos.y = h - 86.0f;
			pSprite->Draw(pMyTexture[3], NULL, &s, &v2, D3DXToRadian(0), &pos, 0x99ffffff);
			pSprite->Draw(pMyTexture[4], NULL, &s, &v2, D3DXToRadian(d2), &pos, 0x99ffffff);

			//コンパス
			if (CurrentCheckPoint<Course[CurrentCourse].Count || CompassTarget.y>-100000.0) {
				GVector target = Course[CurrentCourse].Point[CurrentCheckPoint];
				if (CurrentCheckPoint < Course[CurrentCourse].Count == 0) target = CompassTarget;
				d = (g_Chip[0]->X - target).Cut2(GVector(0, 1, 0)).angle2(GVector(0, 0, 1), GVector(0, 1, 0))*180.0f / (float)M_PI + d2 + 180.0f;
				// 針の影
				pos.x = w - 94.0f - 94.0f - 94.0f - 128.0f + 24.0f + 2.0f;
				pos.y = h - 86.0f + 48.0f + 1.0f;
				pSprite->Draw(pMyTexture[12], NULL, &s2, &v4, D3DXToRadian(d), &pos, 0x77ffffff);
				// 針
				pos.x = w - 94.0f - 94.0f - 94.0f - 128.0f + 24.0f;
				pos.y = h - 86.0f + 48.0f;
				pSprite->Draw(pMyTexture[11], NULL, &s2, &v4, D3DXToRadian(d), &pos, 0xffffffff);

				// 針の影
				float heightangle = (float)(atan2(g_Chip[0]->X.y - target.y, (g_Chip[0]->X - target).Cut2(GVector(0, 1, 0)).abs())*31.0f*2.0f / M_PI);
				pos.x = w - 94.0f - 94.0f - 94.0f - 128.0f + 24.0f + 2.0f;
				pos.y = h - 86.0f + 32.0f + 1.0f - heightangle;
				pSprite->Draw(pMyTexture[14], NULL, &s2, &v3, NULL, &pos, 0x77ffffff);
				// 針
				pos.x = w - 94.0f - 94.0f - 94.0f - 128.0f + 24.0f;
				pos.y = h - 86.0f + 32.0f - heightangle;
				pSprite->Draw(pMyTexture[13], NULL, &s2, &v3, NULL, &pos, 0xffffffff);
				// 針の影
				pos.x = w - 94.0f - 94.0f - 94.0f - 128.0f + 24.0f + 2.0f;
				pos.y = h - 86.0f + 48.0f + 1.0f;
				pSprite->Draw(pMyTexture[12], NULL, &s2, &v4, D3DXToRadian(d), &pos, 0x77ffffff);
				// 針
				pos.x = w - 94.0f - 94.0f - 94.0f - 128.0f + 24.0f;
				pos.y = h - 86.0f + 48.0f;
				pSprite->Draw(pMyTexture[11], NULL, &s2, &v4, D3DXToRadian(d), &pos, 0xffffffff);
			}

			// 針の影
			pos.x = w - 94.0f - 94.0f - 94.0f - 128.0f + 24.0f + 2.0f;
			pos.y = h - 86.0f + 48.0f + 1.0f;
			pSprite->Draw(pMyTexture[8], NULL, &s2, &v4, D3DXToRadian(180), &pos, 0x77ffffff);
			// 針
			pos.x = w - 94.0f - 94.0f - 94.0f - 128.0f + 24.0f;
			pos.y = h - 86.0f + 48.0f;
			pSprite->Draw(pMyTexture[7], NULL, &s2, &v4, D3DXToRadian(180), &pos, 0xffffffff);
		}
	}

	if (w - 128 - 94 - 94 - 94 - 79 > 0 && CCDFlag && ShowMeter) {
		pos.x = w - 128 - 94 - 94 - 94 - 79;
		pos.y = h - 71;
		pSprite->Draw(pMyTexture[20], NULL, NULL, NULL, D3DXToRadian(0), &pos, 0x99ffffff);
	}
	if (ShowTitle) {
		if (TitleAlpha <= 0xeeffffff) TitleAlpha += 0x11000000;
	}
	else {
		if (TitleAlpha > 0x00ffffff) TitleAlpha -= 0x11000000;
	}
	if (TitleAlpha != 0x00ffffff) {
		pos.x = w / 2.0f - 128.0f;
		pos.y = 0.0f;
		pSprite->Draw(pMyTexture[15], NULL, NULL, NULL, NULL, &pos, TitleAlpha);
	}
	pSprite->End();




	// 情報の表示 

	RenderText();
	// End the scene.
	//	m_pd3dDevice->SetTexture(0,NULL);
	m_pd3dDevice->EndScene();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RenderText()
// Desc: Renders stats and help text to the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RenderText()
{
	if (g_World->NetStop) return S_OK;
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 0, 0, 0);
	D3DCOLOR fontColor2 = D3DCOLOR_ARGB(255, 200, 200, 200);
	D3DCOLOR fontColor3 = D3DCOLOR_ARGB(255, 100, 100, 100);
	D3DCOLOR fontWarningColor = D3DCOLOR_ARGB(255, 255, 0, 0);
	D3DCOLOR fontPlayColor = D3DCOLOR_ARGB(255, 0, 30, 255);
	SIZE size;
	float w = (FLOAT)m_d3dsdBackBuffer.Width;
	float h = (FLOAT)m_d3dsdBackBuffer.Height;
	TCHAR szMsg[MAX_PATH] = TEXT("");

	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	szMsg[0] = '\0';
	// Output display stats
	FLOAT fNextLine = 0.0f;
	static bool goal = false;
	static int b = 0;
	int i;
	//	_stprintf( szMsg, _T("Course=%d"),g_Chip[0]->Top->TotalCount);
	//	m_pFont->DrawText( 16, fNextLine, fontColor2, szMsg );
	if (ShowMessage) {
		if (ScriptType == 1) {
			if (ScriptErrorCode != 0) {
				switch (ScriptErrorCode) {
				case -1:
					_stprintf(szMsg, _T("Lua compile error  %s"), ScriptErrorStr);
					break;
				case LUA_ERRSYNTAX:
					_stprintf(szMsg, _T("Lua statement error  %s"), ScriptErrorStr);
					break;
				case LUA_ERRMEM:
					_stprintf(szMsg, _T("Lua memory error  %s"), ScriptErrorStr);
					break;
				default:
					_stprintf(szMsg, _T("Lua runtime error  %s"), ScriptErrorStr);
					break;
				}
				m_pFont->DrawText(16, fNextLine, fontWarningColor, szMsg);

			}
		}
		else {
			if (ScriptErrorCode != 0) {
				TCHAR tstr[40];
				int pc = ScriptErrorPc - 38;if (pc < 0) pc = 0;
				for (int k = 0;k < 38;k++) {
					tstr[k] = ScriptSource[pc++];
					if (pc > ScriptErrorPc) break;
				}
				tstr[k] = '\0';
				_stprintf(szMsg, _T("Script Error  %d:%s [ %s ]"), ScriptErrorCode, ScriptErrorStr, tstr);
				m_pFont->DrawText(16, fNextLine, fontWarningColor, szMsg);

			}
		}

		//システム出力
		for (i = 0;i < GOUTPUTMAX;i++) {
			fNextLine += 15.0f;
			bool japan = false;
			if (SystemOutput[i][0] == '&') {
				japan = true;
				lstrcpy(szMsg, &SystemOutput[i][1]);
			}
			else {
				lstrcpy(szMsg, SystemOutput[i]);
				for (unsigned int j = 0;j < (unsigned int)strlen(SystemOutput[i]);j++) {
					if (SystemOutput[i][j] < 0) {
						japan = true;
						break;
					}
				}
			}
			int left = (int)w - 220;
			int top = 30;
			if (japan) {
				RECT rc = { left + 16,top + (int)fNextLine,16 * 40,16 };
				rc.left = left + 17;rc.top = top + (int)fNextLine + 1;
				g_pFont->DrawText(szMsg, -1, &rc, DT_NOCLIP, fontColor2);
				rc.left = left + 15;rc.top = top + (int)fNextLine - 1;
				g_pFont->DrawText(szMsg, -1, &rc, DT_NOCLIP, fontColor2);
				rc.left = left + 17;rc.top = top + (int)fNextLine - 1;
				g_pFont->DrawText(szMsg, -1, &rc, DT_NOCLIP, fontColor2);
				rc.left = left + 15;rc.top = top + (int)fNextLine + 1;
				g_pFont->DrawText(szMsg, -1, &rc, DT_NOCLIP, fontColor2);
				rc.left = left + 16;rc.top = top + (int)fNextLine;
				g_pFont->DrawText(szMsg, -1, &rc, DT_NOCLIP, fontColor);
			}
			else {
				m_pFont->DrawText((float)left + 17, top + fNextLine + 1, fontColor2, szMsg);
				m_pFont->DrawText((float)left + 15, top + fNextLine - 1, fontColor2, szMsg);
				m_pFont->DrawText((float)left + 17, top + fNextLine - 1, fontColor2, szMsg);
				m_pFont->DrawText((float)left + 15, top + fNextLine + 1, fontColor2, szMsg);
				m_pFont->DrawText((float)left + 16, top + fNextLine, fontColor, szMsg);
			}
			if (SystemErrorCode != 0) {
				switch (SystemErrorCode) {
				case -1:
					_stprintf(szMsg, _T("Scenario compile error  %s"), SystemErrorStr);
					break;
				case LUA_ERRSYNTAX:
					_stprintf(szMsg, _T("Scenario statement error  %s"), SystemErrorStr);
					break;
				case LUA_ERRMEM:
					_stprintf(szMsg, _T("Scenario memory error  %s"), SystemErrorStr);
					break;
				default:
					_stprintf(szMsg, _T("Scenario runtime error  %s"), SystemErrorStr);
					break;
				}
				m_pFont->DrawText(24, 0, fontWarningColor, szMsg);

			}

		}
		//スクリプト出力
		fNextLine = 0.0f;
		for (i = 0;i < GOUTPUTMAX;i++) {
			fNextLine += 15.0f;
			bool japan = false;
			if (ScriptOutput[i][0] == '&') {
				japan = true;
				lstrcpy(szMsg, &ScriptOutput[i][1]);
			}
			else {
				lstrcpy(szMsg, ScriptOutput[i]);
				for (int j = 0;j < (int)strlen(ScriptOutput[i]);j++) {
					if (ScriptOutput[i][j] < 0) {
						japan = true;
						break;
					}
				}
			}
			if (japan) {
				RECT rc = { 16,(long)fNextLine,16 * 40,16 };
				rc.left = 17;rc.top = (long)fNextLine + 1;
				g_pFont->DrawText(szMsg, -1, &rc, DT_NOCLIP, fontColor2);
				rc.left = 15;rc.top = (long)fNextLine - 1;
				g_pFont->DrawText(szMsg, -1, &rc, DT_NOCLIP, fontColor2);
				rc.left = 17;rc.top = (long)fNextLine - 1;
				g_pFont->DrawText(szMsg, -1, &rc, DT_NOCLIP, fontColor2);
				rc.left = 15;rc.top = (long)fNextLine + 1;
				g_pFont->DrawText(szMsg, -1, &rc, DT_NOCLIP, fontColor2);
				rc.left = 16;rc.top = (long)fNextLine;
				g_pFont->DrawText(szMsg, -1, &rc, DT_NOCLIP, fontColor);
			}
			else {
				m_pFont->DrawText(17, fNextLine + 1, fontColor2, szMsg);
				m_pFont->DrawText(15, fNextLine - 1, fontColor2, szMsg);
				m_pFont->DrawText(17, fNextLine - 1, fontColor2, szMsg);
				m_pFont->DrawText(15, fNextLine + 1, fontColor2, szMsg);
				m_pFont->DrawText(16, fNextLine, fontColor, szMsg);
			}
		}
	}

	//    lstrcpy( szMsg, m_strFrameStats );
	//   fNextLine += 15.0f;
	//    m_pFont->DrawText( 2, fNextLine, fontColor, szMsg );

	// Output statistics & help
	//   fNextLine += 15.0f;
	//    _stprintf( szMsg, _T("%d  %d %8.1f"),g_World->Land->List2Count,g_World->Land->List1Count,g_Chip[0]->MaxImpulse);
	//    m_pFont->DrawText( 2, fNextLine, fontColor, szMsg );
	//   fNextLine += 15.0f;
	//    _stprintf( szMsg, _T("%d"),NumFace);
	//    m_pFont->DrawText( 2, fNextLine, fontColor, szMsg );

	//    fNextLine += 15.0f;
	//    _stprintf( szMsg, _T("%.1f %.1f %.1f"),g_Chip[0]->X.x,g_Chip[0]->X.y,g_Chip[0]->X.z);
	//    m_pFont->DrawText( 2, fNextLine, fontColor, szMsg );

	if (ShowRegulation) {
		if (CurrentCourse != 0) {
			float t = (float)GameTime / 30.0f;
			if (t >= 0) {
				if (waitCount > 0) {
					_stprintf(szMsg, _T("%d"), waitCount / 30 + 1);
					m_pFontL->GetTextExtent(szMsg, &size);
					m_pFontL->DrawText(w - 48 - size.cx / 2, 80, fontWarningColor, szMsg);
				}
				else {
					_stprintf(szMsg, _T("%06.2f"), t);
					m_pFont->GetTextExtent(szMsg, &size);
					if (CurrentCheckPoint < Course[CurrentCourse].Count - 1) {
						m_pFont->DrawText(w - 48 - size.cx / 2, 85, fontColor2, szMsg);
						m_pFont->DrawText(w - 46 - size.cx / 2, 85, fontColor2, szMsg);
						m_pFont->DrawText(w - 47 - size.cx / 2, 84, fontColor2, szMsg);
						m_pFont->DrawText(w - 47 - size.cx / 2, 86, fontColor2, szMsg);
						m_pFont->DrawText(w - 47 - size.cx / 2, 85, fontPlayColor, szMsg);
						goal = false;
						b = 0;
					}
					else {
						if (goal == false) {
							b = (~((int)(GameTime / 3) + (int)(GameTime / 30) * 0xff)) * 0xffff + (int)(GameTime)* 0xfff + dataCode * 5 + gameCode * 3 + landCode;
							int bb = GameTime % 8;
							if (bb == 0) b = b ^ 0x55555555;
							else if (bb == 1) b = b ^ 0x55555555;
							else if (bb == 2) b = b ^ 0xaaaaaaaa;
							else if (bb == 3) b = b ^ 0xf0f0f0f0;
							else if (bb == 4) b = b ^ 0x0f0f0f0f;
							else if (bb == 5) b = b ^ 0xaf05af05;
							else if (bb == 6) b = b ^ 0x0fa50fa5;
							else if (bb == 7) b = b ^ 0x3c3c3c3c;

							goal = true;
						}
						m_pFont->DrawText(w - 48 - size.cx / 2, 85, fontColor2, szMsg);
						m_pFont->DrawText(w - 46 - size.cx / 2, 85, fontColor2, szMsg);
						m_pFont->DrawText(w - 47 - size.cx / 2, 84, fontColor2, szMsg);
						m_pFont->DrawText(w - 47 - size.cx / 2, 86, fontColor2, szMsg);
						m_pFont->DrawText(w - 47 - size.cx / 2, 85, fontWarningColor, szMsg);
						_stprintf(szMsg, _T("%08X"), b);
						m_pFont->GetTextExtent(szMsg, &size);
						m_pFont->DrawText(w - 48 - size.cx / 2, 130, fontColor2, szMsg);
						m_pFont->DrawText(w - 46 - size.cx / 2, 130, fontColor2, szMsg);
						m_pFont->DrawText(w - 47 - size.cx / 2, 129, fontColor2, szMsg);
						m_pFont->DrawText(w - 47 - size.cx / 2, 131, fontColor2, szMsg);
						m_pFont->DrawText(w - 47 - size.cx / 2, 130, fontWarningColor, szMsg);
					}

				}
			}
			else {
				_stprintf(szMsg, _T("READY"));
				m_pFont->GetTextExtent(szMsg, &size);
				m_pFont->DrawText(w - 47 - size.cx / 2, 85, fontColor, szMsg);
			}
		}
		FLOAT h1 = (FLOAT)(65 + (CurrentCourse == 0 ? -20 : 0));
		FLOAT w1 = w - 47 + (CurrentCourse != 0 ? -75 : 0);
		if (KeyRecordMode != 0) {
			if (CurrentCourse == 0) {
				_stprintf(szMsg, _T("%07.2f"), KeyRecordCount*g_World->StepTime);
				m_pFont->DrawText(w - 106, h1, fontColor2, szMsg);
			}
		}
		if (KeyRecordMode == 1) {
			_stprintf(szMsg, _T("REC"));
			m_pFont->DrawText(w1, h1, fontWarningColor, szMsg);
		}
		else if (KeyRecordMode == 2) {
			_stprintf(szMsg, _T("PLAY"));
			m_pFont->DrawText(w1 - 3, h1, fontPlayColor, szMsg);
		}
		int ch = 'A' + ViewType;
		if (ViewType < 0) ch = 'X';
		_stprintf(szMsg, _T("%c"), ch);
		m_pFont->DrawText(w - 228, 15, fontWarningColor, szMsg);

		if (GravityFlag) {
			_stprintf(szMsg, _T("G"));
			m_pFont->DrawText(w - 196, 15, fontColor2, szMsg);
		}
		if (AirFlag) {
			_stprintf(szMsg, _T("A"));
			m_pFont->DrawText(w - 172, 15, fontColor2, szMsg);
		}
		if (TorqueFlag) {
			_stprintf(szMsg, _T("T"));
			m_pFont->DrawText(w - 148, 15, fontColor2, szMsg);
		}
		if (JetFlag) {
			_stprintf(szMsg, _T("J"));
			m_pFont->DrawText(w - 124, 15, fontColor2, szMsg);
		}
		if (UnbreakableFlag) {
			_stprintf(szMsg, _T("U"));
			m_pFont->DrawText(w - 100, 15, fontColor2, szMsg);
		}
		if (CCDFlag) {
			_stprintf(szMsg, _T("C"));
			m_pFont->DrawText(w - 76, 15, fontColor2, szMsg);
		}
		if (ScriptFlag) {
			_stprintf(szMsg, _T("S"));
			m_pFont->DrawText(w - 52, 15, fontColor2, szMsg);
		}
		if (EfficientFlag) {
			_stprintf(szMsg, _T("E"));
			m_pFont->DrawText(w - 28, 15, fontColor2, szMsg);
		}
		_stprintf(szMsg, _T(Course[CurrentCourse].Name));
		m_pFont->GetTextExtent(szMsg, &size);
		m_pFont->DrawText(w - size.cx - 15, 40.0f, fontColor, szMsg);
		m_pFont->DrawText(w - size.cx - 17, 40.0f, fontColor, szMsg);
		m_pFont->DrawText(w - size.cx - 16, 39.0f, fontColor, szMsg);
		m_pFont->DrawText(w - size.cx - 16, 41.0f, fontColor, szMsg);
		m_pFont->DrawText(w - size.cx - 16, 40.0f, fontColor2, szMsg);
	}
	if (ShowMeter) {
		float height = (float)g_Chip[0]->X.y;
		_stprintf(szMsg, _T("%05.0f"), height);
		m_pFont->GetTextExtent(szMsg, &size);
		m_pFont->DrawText(w - 128 - 44.0f - size.cx / 2, h - 60.0f, fontColor2, szMsg);

		float power = (float)TotalPower;
		_stprintf(szMsg, _T("%05.0f"), power);
		m_pFont->GetTextExtent(szMsg, &size);
		m_pFont->DrawText(w - 128 - 94 - 44.0f - size.cx / 2, h - 60.0f, fontColor2, szMsg);

	}
	if (ShowVariable) {
		int n = 0;
		for (i = 0;i < g_VarCount;i++) if (g_ValList[i].visible) n++;
		int j = 0;
		for (i = 0;i < g_VarCount;i++) {
			if (g_ValList[i].visible) {
				_tcscpy(szMsg, _T(g_ValList[i].Name));
				szMsg[8] = '\0';
				if (_tcslen(_T(g_ValList[i].Name))>8) _tcscat(szMsg, _T("..:"));
				else  _tcscat(szMsg, _T(":"));
				m_pFont->GetTextExtent(szMsg, &size);
				m_pFont->DrawText(71.0f - size.cx, h - 14.0f*(n - j) - 9.0f, (DWORD)fontColor3, szMsg);
				m_pFont->DrawText(70.0f - size.cx, h - 14.0f*(n - j) - 10.0f, (DWORD)fontColor2, szMsg);
				_stprintf(szMsg, _T("%.2f"), g_ValList[i].Val);
				m_pFont->GetTextExtent(szMsg, &size);
				m_pFont->DrawText(141.0f - size.cx, h - 14.0f*(n - j) - 9.0f, (DWORD)fontColor3, szMsg, 0);
				m_pFont->DrawText(140.0f - size.cx, h - 14.0f*(n - j) - 10.0f, (DWORD)fontColor2, szMsg, 0);
				j++;
			}
		}
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::Pause(BOOL bPause)
{
	CInputDeviceManager::DeviceInfo* pDeviceInfos;
	DWORD dwNumDevices;
	m_pInputDeviceManager->GetDevices(&pDeviceInfos, &dwNumDevices);

	for (DWORD i = 0; i < dwNumDevices; i++)
	{
		InputDeviceState* pInputDeviceState = (InputDeviceState*)pDeviceInfos[i].pParam;
		ZeroMemory(pInputDeviceState, sizeof(InputDeviceState));
	}

	CD3DApplication::Pause(bPause);
}



void CMyD3DApplication::SetRegulationMenu() {
	HMENU hMenu = GetMenu(m_hWnd);
	if (GravityFlag) {
		CheckMenuItem(hMenu, IDM_GRAVITY, MF_CHECKED);
		g_World->G = GVector(0, -9.807f, 0);
	}
	else {
		CheckMenuItem(hMenu, IDM_GRAVITY, MF_UNCHECKED);
		g_World->G = GVector(0, 0, 0);
	}
	if (AirFlag) CheckMenuItem(hMenu, IDM_AIR, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_AIR, MF_UNCHECKED);
	if (TorqueFlag) CheckMenuItem(hMenu, IDM_TORQUE, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_TORQUE, MF_UNCHECKED);
	if (JetFlag) CheckMenuItem(hMenu, IDM_JET, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_JET, MF_UNCHECKED);
	g_World->DestroyFlag = (UnbreakableFlag == TRUE) ? true : false;
	if (UnbreakableFlag) CheckMenuItem(hMenu, IDM_UNBREAKABLE, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_UNBREAKABLE, MF_UNCHECKED);
	if (CCDFlag) CheckMenuItem(hMenu, IDM_CCD, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_CCD, MF_UNCHECKED);
	if (ScriptFlag) CheckMenuItem(hMenu, IDM_SCRIPT, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_SCRIPT, MF_UNCHECKED);
	if (EfficientFlag) CheckMenuItem(hMenu, IDM_ENERGY, MF_CHECKED);
	else CheckMenuItem(hMenu, IDM_ENERGY, MF_UNCHECKED);
}
//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Overrrides the main WndProc, so the sample can do custom message
//       handling (e.g. processing mouse, keyboard, or menu commands).
//-----------------------------------------------------------------------------
#define UMSG_RCUPDATE_START 0
#define UMSG_RCUPDATE_CHAR  1
#define UMSG_RCUPDATE_END   2

LRESULT CMyD3DApplication::MsgProc(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam)
{
	HDROP hDrop;
	UINT uFileNo;
	static UINT uMessage = 0;
	static UINT uMessage2 = 0;
	static char rcvmsg[256];
	char bf[3];
	bf[1] = 0;
	bf[2] = 0;

	if (msg == uMessage || msg == uMessage2) {
		switch (wParam) {
		case UMSG_RCUPDATE_START:
			// 受信メッセージクリア
			strcpy(rcvmsg, "");
			return(S_OK);
		case UMSG_RCUPDATE_CHAR:
			// 受信メッセージ結合
			bf[0] = (char)(lParam & 0xff);
			bf[1] = (char)(lParam >> 8);

			strcat(rcvmsg, bf);
			return(S_OK);
		case UMSG_RCUPDATE_END:
			// 受信メッセージ処理
			int errCode = 0;
			if ((errCode = LoadData(rcvmsg)) != 0) {
				char str[256];
				BlockErrStr(errCode, DataCheck, str);
				MessageBox(NULL, str, NULL, MB_ICONERROR | MB_OK);
			}
			return(S_OK);
		}
		return S_OK;
	}
	//g_FPSの再設定
//			if(m_appMax>1) g_LimitFPS=15;else g_LimitFPS=30;

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED) {
			//			GCHIPSTREAM stream;
			//			stream.code=-1;//CLEAR
			//			if(g_DPlay)	g_DPlay->SendAll((BYTE*)&stream,sizeof(short));
			//			CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
			return S_OK;
		}
		break;
	case WM_SETFOCUS:
		//    m_hWndFocused = GetFocus( )->GetSafeHwnd( );
		//		m_hIMC = ImmAssociateContext( m_hWnd, NULL );
		//		WINNLSEnableIME(m_hWnd, false);
		break;
	case WM_KILLFOCUS:
		//		ImmAssociateContext( m_hWnd, m_hIMC );
		break;

	case WM_CREATE:
		uMessage = RegisterWindowMessage("WM_RIGHTCHIP_LOAD");
		uMessage2 = RegisterWindowMessage("WM_RIGIDCHIP_LOAD");
		DragAcceptFiles(hWnd, TRUE);
		break;
	case WM_PAINT:
	{
		if (m_bLoadingApp)
		{
			HBITMAP hBit;
			BITMAP bmp_info;
			RECT rc;
			PAINTSTRUCT ps;
			HDC hdc_mem;

			// Draw on the window tell the user that the app is loading
			 // TODO: change as needed
			HDC hDC = GetDC(hWnd);
			//TCHAR strMsg[MAX_PATH];
			//wsprintf( strMsg, TEXT("Loading data ... ") );
			HBRUSH hBrush = CreateSolidBrush(RGB(204, 204, 204));
			RECT rct;
			GetClientRect(hWnd, &rct);
			FillRect(hDC, &rct, hBrush);
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(204, 0, 0));
			rct.bottom = rct.top + 4;
			FillRect(hDC, &rct, hBrush);
			DeleteObject(hBrush);

			GetClientRect(hWnd, &rc);
			HDC hdc = BeginPaint(hWnd, &ps);
			hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));

			GetObject(hBit, sizeof(BITMAP), &bmp_info);
			int wx = bmp_info.bmWidth;
			int wy = bmp_info.bmHeight;
			hdc_mem = CreateCompatibleDC(hdc);
			SelectObject(hdc_mem, hBit);
			BitBlt(hdc, (rc.right - wx) / 2,
				(rc.bottom - wy) * 4 / 5, wx, wy, hdc_mem, 0, 0, SRCCOPY);
			DeleteDC(hdc_mem);
			DeleteObject(hBit);
			EndPaint(hWnd, &ps);
			ReleaseDC(hWnd, hDC);
		}
		break;
	}

	case WM_DROPFILES:
		char szFileName[_MAX_PATH];
		hDrop = (HDROP)wParam;
		uFileNo = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
		for (int i = 0; i < (int)uFileNo; i++) {
			DragQueryFile(hDrop, i, szFileName, sizeof(szFileName));
			int errCode = 0;
			if ((errCode = LoadData(szFileName)) != 0) {
				char str[256];
				BlockErrStr(errCode, DataCheck, str);
				MessageBox(NULL, str, NULL, MB_ICONERROR | MB_OK);
			}
		}
		DragFinish(hDrop);
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_SEND:
		{
			if (NetworkDlg) {
				if (GetFocus() == GetDlgItem(NetworkDlg, IDC_CHATEDIT))
					SendMessage(NetworkDlg, WM_COMMAND, IDC_SEND, 0);
			}
		}
		break;
		case IDC_RETURNMAIN:
		{
			if (NetworkDlg) {
				SendMessage(NetworkDlg, WM_COMMAND, IDC_RETURNMAIN, 0);
			}
		}
		break;
		case IDM_SETTING_SOUND:
		{
			if (SoundType == 0) SoundType = 1; else SoundType = 0;
			HMENU hMenu = GetMenu(hWnd);
			if (SoundType) {
				CheckMenuItem(hMenu, IDM_SETTING_SOUND, MF_CHECKED);
				long msg;
				if (hMidiOut) {
					midiOutClose(hMidiOut);
					hMidiOut = NULL;
				}
				if (midiOutOpen(&hMidiOut, (UINT)MIDI_MAPPER, NULL, 0, 0) == 0) {
					//GMシステム・オン
					//音色を変える(0xCn,音色,0x00)  nはチャンネル(0～0xF)
					msg = MAKELONG(MAKEWORD(0xC0, 0x7D), MAKEWORD(0x00, 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
					msg = MAKELONG(MAKEWORD(0xC1, 0x7E), MAKEWORD(0x00, 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
					msg = MAKELONG(MAKEWORD(0xB1, 0x07), MAKEWORD(0x00, 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
					msg = MAKELONG(MAKEWORD(0x91, 70), MAKEWORD(0x3f, 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
				}
			}
			else {
				CheckMenuItem(hMenu, IDM_SETTING_SOUND, MF_UNCHECKED);
				if (hMidiOut) {
					midiOutClose(hMidiOut);
					hMidiOut = NULL;
				}
			}
			break;
		}
		case IDM_OPEN:
			m_UserInput.bDoOpenChip = TRUE;
			break;
		case IDM_UPDATE:
			m_UserInput.bDoUpdateChip = TRUE;
			break;
		case IDM_OPENLAND:
			m_UserInput.bDoOpenLand = TRUE;
			break;
		case IDM_OPENGAME:
			m_UserInput.bDoOpenGame = TRUE;
			break;
		case IDM_OPENSCENARIO:
			m_UserInput.bDoOpenScenario = TRUE;
			break;
		case IDM_CLOSESCENARIO:
			m_UserInput.bDoCloseScenario = TRUE;
			break;
		case IDM_CONFIGINPUT:
			m_UserInput.bDoConfigureInput = TRUE;
			break;

		case IDM_CONFIGDEVICE:
			m_UserInput.bDoConfigureDisplay = TRUE;
			return 0; // Don't hand off to parent
		case IDM_SHOWSHADOW:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowShadowFlag = !ShowShadowFlag;
			if (ShowShadowFlag) CheckMenuItem(hMenu, IDM_SHOWSHADOW, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SHOWSHADOW, MF_UNCHECKED);
			break;
		}
		case IDM_SHOWDUST:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowDustFlag = !ShowDustFlag;
			if (ShowDustFlag) CheckMenuItem(hMenu, IDM_SHOWDUST, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SHOWDUST, MF_UNCHECKED);
			break;
		}
		case IDM_DITHER:
		{
			HMENU hMenu = GetMenu(hWnd);
			DitherFlag = !DitherFlag;
			if (DitherFlag) CheckMenuItem(hMenu, IDM_DITHER, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_DITHER, MF_UNCHECKED);
			break;
		}
		case IDM_LIMIT30:
		{
			HMENU hMenu = GetMenu(hWnd);
			g_LimitFPS = 30;
			m_dLimidFPS = (m_dLimidFPS == (1000 / g_LimitFPS)) ? 0L : (1000 / g_LimitFPS);
			if (g_World) g_World->SetStepTime(1.0f / g_LimitFPS);
			if (m_dLimidFPS == (1000 / g_LimitFPS)) {
				CheckMenuItem(hMenu, IDM_LIMIT30, MF_CHECKED);
				CheckMenuItem(hMenu, IDM_LIMIT15, MF_UNCHECKED);
			}
			else {
				CheckMenuItem(hMenu, IDM_LIMIT30, MF_UNCHECKED);
				g_LimitFPS = 30;
			}

			break;
		}
		case IDM_LIMIT15:
		{
			HMENU hMenu = GetMenu(hWnd);
			g_LimitFPS = 15;
			m_dLimidFPS = (m_dLimidFPS == (1000 / g_LimitFPS)) ? 0L : (1000 / g_LimitFPS);
			if (g_World) g_World->SetStepTime(1.0f / g_LimitFPS);
			if (m_dLimidFPS == (1000 / g_LimitFPS)) {
				CheckMenuItem(hMenu, IDM_LIMIT15, MF_CHECKED);
				CheckMenuItem(hMenu, IDM_LIMIT30, MF_UNCHECKED);
			}
			else {
				CheckMenuItem(hMenu, IDM_LIMIT15, MF_UNCHECKED);
				g_LimitFPS = 30;
				m_dLimidFPS = (m_dLimidFPS == (1000 / g_LimitFPS)) ? 0L : (1000 / g_LimitFPS);
				if (g_World) g_World->SetStepTime(1.0f / g_LimitFPS);
			}
			break;
		}

		case IDM_RESETSETTING:
		{
			HMENU hMenu = GetMenu(hWnd);
			if (SoundType == 0) {
				SoundType = 1;
				CheckMenuItem(hMenu, IDM_SETTING_SOUND, MF_CHECKED);
				long msg;
				if (hMidiOut) {
					midiOutClose(hMidiOut);
					hMidiOut = NULL;
				}
				if (midiOutOpen(&hMidiOut, (UINT)MIDI_MAPPER, NULL, 0, 0) == 0) {
					//GMシステム・オン
					//音色を変える(0xCn,音色,0x00)  nはチャンネル(0～0xF)
					msg = MAKELONG(MAKEWORD(0xC0, 0x7D), MAKEWORD(0x00, 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
					msg = MAKELONG(MAKEWORD(0xC1, 0x7E), MAKEWORD(0x00, 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
					msg = MAKELONG(MAKEWORD(0xB1, 0x07), MAKEWORD(0x00, 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
					msg = MAKELONG(MAKEWORD(0x91, 70), MAKEWORD(0x3f, 0));
					midiOutShortMsg(hMidiOut, msg); // Note On
				}
			}
			ShowShadowFlag = 1;
			CheckMenuItem(hMenu, IDM_SHOWSHADOW, MF_CHECKED);
			ShowDustFlag = 1;
			CheckMenuItem(hMenu, IDM_SHOWDUST, MF_CHECKED);
			DitherFlag = 1;
			CheckMenuItem(hMenu, IDM_DITHER, MF_CHECKED);
			g_LimitFPS = 30;
			m_dLimidFPS = (1000 / g_LimitFPS);
			CheckMenuItem(hMenu, IDM_LIMIT15, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_LIMIT30, MF_CHECKED);
			FastShadow = 1;
			ShowGhost = 0;
			CheckMenuItem(hMenu, IDM_SETTING_SHOWGHOST, MF_UNCHECKED);
			ShowCowl = 1;
			CheckMenuItem(hMenu, IDM_SETTING_SHOWCOWL, MF_CHECKED);
			TextureAlpha = 1;
			CheckMenuItem(hMenu, IDM_SETTING_TEXTUREALPHA, MF_CHECKED);
			BackFaces = 0;
			CheckMenuItem(hMenu, IDM_SETTING_SHOWBACKFACE, MF_UNCHECKED);
			UseLuaExternal = 0;
			break;
		}

		case IDM_GRAVITY:
		{
			HMENU hMenu = GetMenu(hWnd);
			GravityFlag = !GravityFlag;
			if (GravityFlag) {
				CheckMenuItem(hMenu, IDM_GRAVITY, MF_CHECKED);
				g_World->G = GVector(0, -9.807f, 0);

			}
			else {
				CheckMenuItem(hMenu, IDM_GRAVITY, MF_UNCHECKED);
				g_World->G = GVector(0, 0, 0);
			}
			break;
		}

		case IDM_AIR:
		{
			HMENU hMenu = GetMenu(hWnd);
			AirFlag = !AirFlag;
			if (AirFlag) CheckMenuItem(hMenu, IDM_AIR, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_AIR, MF_UNCHECKED);
			break;
		}
		case IDM_TORQUE:
		{
			HMENU hMenu = GetMenu(hWnd);
			TorqueFlag = !TorqueFlag;
			if (TorqueFlag) CheckMenuItem(hMenu, IDM_TORQUE, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_TORQUE, MF_UNCHECKED);
			break;
		}
		case IDM_JET:
		{
			HMENU hMenu = GetMenu(hWnd);
			JetFlag = !JetFlag;
			if (JetFlag) CheckMenuItem(hMenu, IDM_JET, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_JET, MF_UNCHECKED);
			break;
		}
		case IDM_UNBREAKABLE:
		{
			HMENU hMenu = GetMenu(hWnd);
			UnbreakableFlag = !UnbreakableFlag;
			g_World->DestroyFlag = (UnbreakableFlag == TRUE) ? true : false;
			if (UnbreakableFlag) CheckMenuItem(hMenu, IDM_UNBREAKABLE, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_UNBREAKABLE, MF_UNCHECKED);
			break;
		}
		case IDM_SCRIPT:
		{
			HMENU hMenu = GetMenu(hWnd);
			ScriptFlag = !ScriptFlag;
			if (ScriptFlag) CheckMenuItem(hMenu, IDM_SCRIPT, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SCRIPT, MF_UNCHECKED);
			break;
		}
		case IDM_ENERGY:
		{
			HMENU hMenu = GetMenu(hWnd);
			EfficientFlag = !EfficientFlag;
			if (EfficientFlag) CheckMenuItem(hMenu, IDM_ENERGY, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_ENERGY, MF_UNCHECKED);
			break;
		}
		case IDM_CHANGEVIEW1:
		{
			if (ViewType == 0) {
				for (int i = LastBye + 1;i < g_World->ChipCount;i++) {
					if (g_World->Rigid[i]->Parent == NULL && g_World->Rigid[i]->ByeFlag != 2) {
						LastBye = i;
						break;
					}
				}
				if (i >= g_World->ChipCount) LastBye = 0;
			}
			HMENU hMenu = GetMenu(hWnd);
			ViewType = 0;
			CheckMenuItem(hMenu, IDM_CHANGEVIEW1, MF_CHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW2, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW3, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW4, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW5, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW6, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW7, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW8, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW9, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW10, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEWX, MF_UNCHECKED);
			break;
		}
		case IDM_CHANGEVIEW2:
		{
			if (ViewType == 1) {
				for (int i = LastBye + 1;i < g_World->ChipCount;i++) {
					if (g_World->Rigid[i]->Parent == NULL && g_World->Rigid[i]->ByeFlag != 2) {
						LastBye = i;
						break;
					}
				}
				if (i >= g_World->ChipCount) LastBye = 0;
			}
			HMENU hMenu = GetMenu(hWnd);
			ViewType = 1;
			CheckMenuItem(hMenu, IDM_CHANGEVIEW1, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW2, MF_CHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW3, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW4, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW5, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW6, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW7, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW8, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW9, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW10, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEWX, MF_UNCHECKED);
			break;
		}
		case IDM_CHANGEVIEW3:
		{
			if (ViewType == 2) {
				for (int i = LastBye + 1;i < g_World->ChipCount;i++) {
					if (g_World->Rigid[i]->Parent == NULL && g_World->Rigid[i]->ByeFlag != 2) {
						LastBye = i;
						break;
					}
				}
				if (i >= g_World->ChipCount) LastBye = 0;
			}
			HMENU hMenu = GetMenu(hWnd);
			ViewType = 2;
			CheckMenuItem(hMenu, IDM_CHANGEVIEW1, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW2, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW3, MF_CHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW4, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW5, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW6, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW7, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW8, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW9, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW10, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEWX, MF_UNCHECKED);
			break;
		}
		case IDM_CHANGEVIEW4:
		{
			if (ViewType == 3) {
				for (int i = LastBye + 1;i < g_World->ChipCount;i++) {
					if (g_World->Rigid[i]->Parent == NULL && g_World->Rigid[i]->ByeFlag != 2) {
						LastBye = i;
						break;
					}
				}
				if (i >= g_World->ChipCount) LastBye = 0;
			}
			HMENU hMenu = GetMenu(hWnd);
			ViewType = 3;
			CheckMenuItem(hMenu, IDM_CHANGEVIEW1, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW2, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW3, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW4, MF_CHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW5, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW6, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW7, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW8, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW9, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW10, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEWX, MF_UNCHECKED);
			break;
		}
		case IDM_CHANGEVIEW5:
		{
			if (ViewType == 4) {
				for (int i = LastBye + 1;i < g_World->ChipCount;i++) {
					if (g_World->Rigid[i]->Parent == NULL && g_World->Rigid[i]->ByeFlag != 2) {
						LastBye = i;
						break;
					}
				}
				if (i >= g_World->ChipCount) LastBye = 0;
			}
			HMENU hMenu = GetMenu(hWnd);
			ViewType = 4;
			CheckMenuItem(hMenu, IDM_CHANGEVIEW1, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW2, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW3, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW4, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW5, MF_CHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW6, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW7, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW8, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW9, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW10, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEWX, MF_UNCHECKED);
			break;
		}
		case IDM_CHANGEVIEW6:
		{
			if (ViewType == 5) {
				for (int i = LastBye + 1;i < g_World->ChipCount;i++) {
					if (g_World->Rigid[i]->Parent == NULL && g_World->Rigid[i]->ByeFlag != 2) {
						LastBye = i;
						break;
					}
				}
				if (i >= g_World->ChipCount) LastBye = 0;
			}
			HMENU hMenu = GetMenu(hWnd);

			ViewType = 5;
			CheckMenuItem(hMenu, IDM_CHANGEVIEW1, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW2, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW3, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW4, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW5, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW6, MF_CHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW7, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW8, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW9, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW10, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEWX, MF_UNCHECKED);
			break;
		}
		case IDM_CHANGEVIEW7:
		{
			if (ViewType == 6) {
				for (int i = LastBye + 1;i < g_World->ChipCount;i++) {
					if (g_World->Rigid[i]->Parent == NULL && g_World->Rigid[i]->ByeFlag != 2) {
						LastBye = i;
						break;
					}
				}
				if (i >= g_World->ChipCount) LastBye = 0;
			}
			ViewType = 6;
			HMENU hMenu = GetMenu(hWnd);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW1, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW2, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW3, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW4, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW5, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW6, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW7, MF_CHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW8, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW9, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW10, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEWX, MF_UNCHECKED);
			break;
		}
		case IDM_CHANGEVIEW8:
		{
			if (ViewType == 7) {
				for (int i = LastBye + 1;i < g_World->ChipCount;i++) {
					if (g_World->Rigid[i]->Parent == NULL && g_World->Rigid[i]->ByeFlag != 2) {
						LastBye = i;
						break;
					}
				}
				if (i >= g_World->ChipCount) LastBye = 0;
			}
			HMENU hMenu = GetMenu(hWnd);
			ViewType = 7;
			CheckMenuItem(hMenu, IDM_CHANGEVIEW1, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW2, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW3, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW4, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW5, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW6, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW7, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW8, MF_CHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW9, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW10, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEWX, MF_UNCHECKED);
			break;
		}
		case IDM_CHANGEVIEW9:
		{
			if (ViewType == 8) {
				for (int i = LastBye + 1;i < g_World->ChipCount;i++) {
					if (g_World->Rigid[i]->Parent == NULL && g_World->Rigid[i]->ByeFlag != 2) {
						LastBye = i;
						break;
					}
				}
				if (i >= g_World->ChipCount) LastBye = 0;
			}
			HMENU hMenu = GetMenu(hWnd);
			ViewType = 8;
			CheckMenuItem(hMenu, IDM_CHANGEVIEW1, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW2, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW3, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW4, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW5, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW6, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW7, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW8, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW9, MF_CHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW10, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEWX, MF_UNCHECKED);
			break;
		}
		case IDM_CHANGEVIEW10:
		{
			if (ViewType == 9) {
				for (int i = LastBye + 1;i < g_World->ChipCount;i++) {
					if (g_World->Rigid[i]->Parent == NULL && g_World->Rigid[i]->ByeFlag != 2) {
						LastBye = i;
						break;
					}
				}
				if (i >= g_World->ChipCount) LastBye = 0;
			}
			HMENU hMenu = GetMenu(hWnd);
			ViewType = 9;
			CheckMenuItem(hMenu, IDM_CHANGEVIEW1, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW2, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW3, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW4, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW5, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW6, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW7, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW8, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW9, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEW10, MF_CHECKED);
			CheckMenuItem(hMenu, IDM_CHANGEVIEWX, MF_UNCHECKED);
			break;
		}
		/*			case IDM_SETTING_CHEAPCOMPUTER:
						{
							HMENU hMenu = GetMenu( hWnd );
							CheapComputerFlag = !CheapComputerFlag;
							if(CheapComputerFlag) CheckMenuItem(hMenu,IDM_SETTING_CHEAPCOMPUTER,MF_CHECKED);
							else CheckMenuItem(hMenu,IDM_SETTING_CHEAPCOMPUTER,MF_UNCHECKED);
							break;
						}
		*/			case IDM_SHOWABOUT:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowTitle = !ShowTitle;
			if (ShowTitle) CheckMenuItem(hMenu, IDM_SHOWABOUT, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SHOWABOUT, MF_UNCHECKED);
			break;
		}
		case IDM_WEBHELP:
		{
			if (m_bWindowed == FALSE) {
				if (FAILED(ToggleFullscreen()))
				{
					DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT);
					return 1;
				}
				// Prompt the user to change the mode
				InvalidateRect(g_hWnd, NULL, NULL);
				Resize3DEnvironment();
			}
			ShellExecute(0, NULL, "http://www.google.com/search?q=RigidChips", NULL, NULL, SW_SHOW);
			break;
		}
		case IDM_SHOWMETER:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowMeter = !ShowMeter;
			if (ShowMeter) CheckMenuItem(hMenu, IDM_SHOWMETER, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SHOWMETER, MF_UNCHECKED);
			break;
		}
		case IDM_SHOWREGULATION:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowRegulation = !ShowRegulation;
			if (ShowRegulation) CheckMenuItem(hMenu, IDM_SHOWREGULATION, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SHOWREGULATION, MF_UNCHECKED);
			break;
		}
		case IDM_SHOWVARIABLE:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowVariable = !ShowVariable;
			if (ShowVariable) CheckMenuItem(hMenu, IDM_SHOWVARIABLE, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SHOWVARIABLE, MF_UNCHECKED);
			break;
		}
		case IDM_SHOWSCRIPTMESSAGE:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowMessage = !ShowMessage;
			if (ShowMessage) CheckMenuItem(hMenu, IDM_SHOWSCRIPTMESSAGE, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SHOWSCRIPTMESSAGE, MF_UNCHECKED);
			break;
		}
		case IDM_SHOWPOWER:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowPower = !ShowPower;
			if (ShowPower) CheckMenuItem(hMenu, IDM_SHOWPOWER, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SHOWPOWER, MF_UNCHECKED);
			break;
		}
		case IDM_SETTING_SHOWGHOST:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowGhost = !ShowGhost;
			if (ShowGhost) CheckMenuItem(hMenu, IDM_SETTING_SHOWGHOST, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SETTING_SHOWGHOST, MF_UNCHECKED);
			break;
		}
		case IDM_SETTING_SHOWCOWL:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowCowl = !ShowCowl;
			if (ShowCowl) CheckMenuItem(hMenu, IDM_SETTING_SHOWCOWL, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SETTING_SHOWCOWL, MF_UNCHECKED);
			break;
		}
		case IDM_SETTING_TEXTUREALPHA:
		{
			HMENU hMenu = GetMenu(hWnd);
			TextureAlpha = !TextureAlpha;
			if (TextureAlpha) CheckMenuItem(hMenu, IDM_SETTING_TEXTUREALPHA, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SETTING_TEXTUREALPHA, MF_UNCHECKED);
			break;
		}
		case IDM_SETTING_SHOWBACKFACE:
		{
			HMENU hMenu = GetMenu(hWnd);
			BackFaces = !BackFaces;
			if (BackFaces) CheckMenuItem(hMenu, IDM_SETTING_SHOWBACKFACE, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_SETTING_SHOWBACKFACE, MF_UNCHECKED);
			break;
		}

		/*
					case IDM_OBJECT_BALL:
						{
							HMENU hMenu = GetMenu( hWnd );
		//                    ObjectBallFlag = !ObjectBallFlag;
							if(ObjectBallFlag) {
								CheckMenuItem(hMenu,IDM_OBJECT_BALL,MF_CHECKED);
							}
							else CheckMenuItem(hMenu,IDM_OBJECT_BALL,MF_UNCHECKED);
							break;
						}
		*/
		case IDM_HELP_SHOWDATA:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowData = !ShowData;
			break;
		}
		case IDM_EXTRASETTING:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowExtra = !ShowExtra;
			break;
		}

		case IDM_NETWORKSETTING:
		{
			HMENU hMenu = GetMenu(hWnd);
			ShowNetwork = !ShowNetwork;
			break;
		}
		case IDM_CCD:
		{
			HMENU hMenu = GetMenu(hWnd);
			CCDFlag = !CCDFlag;
			if (CCDFlag) CheckMenuItem(hMenu, IDM_CCD, MF_CHECKED);
			else CheckMenuItem(hMenu, IDM_CCD, MF_UNCHECKED);
			break;
		}
		case IDM_RECORD:
		{
			MsgFlag = true;
			RecState = 1;
			break;
		}
		case IDM_STOP:
		{
			MsgFlag = true;
			RecState = 2;
			break;
		}
		case IDM_PLAY:
		{
			MsgFlag = true;
			RecState = 3;
			break;
		}
		case IDM_MODECHANGE:
		{
			CallModeChange = 1;
			break;
		}
		case IDM_SAVELOG:
		{
			if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
			if (KeyRecordMax > 0) {
				m_UserInput.bDoSaveLog = TRUE;
			}
			else {
				MessageBox(NULL, "No Log-Data!", NULL, MB_OK | MB_ICONEXCLAMATION);
			}
			break;
		}
		case IDM_LOADLOG:
		{
			if (KeyRecordMode == 1) KeyRecordMax = KeyRecordCount;
			m_UserInput.bDoOpenLog = TRUE;
			break;
		}
		}
		break;
	}

	}

	return CD3DApplication::MsgProc(hWnd, msg, wParam, lParam);
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Invalidates device objects.  
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InvalidateDeviceObjects()
{
	// TODO: Cleanup any objects created in RestoreDeviceObjects()
	//**********
	SAFE_RELEASE(pPointVB);
	SAFE_RELEASE(pPointTexture);

	if (g_pFont)
		g_pFont->OnLostDevice();
	int i;
	for (i = 0;i < GMODELMAX;i++)
		if (m_pXMesh[i]) m_pXMesh[i]->InvalidateDeviceObjects();

	m_pSkyMesh->InvalidateDeviceObjects();
	g_pLandMesh->InvalidateDeviceObjects();

	SAFE_RELEASE(pSprite);
	for (i = 0;i < GTEXMAX;i++) SAFE_RELEASE(pMyTexture[i]);

	m_pFont->InvalidateDeviceObjects();
	m_pFontL->InvalidateDeviceObjects();
	m_pFont3D->InvalidateDeviceObjects();
	SAFE_RELEASE(m_pDIConfigSurface);

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.  
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::DeleteDeviceObjects()
{
	// TODO: Cleanup any objects created in InitDeviceObjects()

	//**********
	SAFE_RELEASE(pPointVB);
	SAFE_RELEASE(pPointTexture);
	for (int i = 0;i < GMODELMAX;i++)
		if (m_pXMesh[i]) m_pXMesh[i]->Destroy();

	m_pSkyMesh->Destroy();
	g_pLandMesh->Destroy();

	m_pFont->DeleteDeviceObjects();
	m_pFontL->DeleteDeviceObjects();
	m_pFont3D->DeleteDeviceObjects();
	SAFE_RELEASE(g_pFont);


	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FinalCleanup()
{
	g_pApp->Pause(TRUE);
	if (g_DPlay) g_DPlay->End();
	if (NetworkDlg) DestroyWindow(NetworkDlg);
	NetworkDlg = NULL;
	// TODO: Perform any final cleanup needed
	// Cleanup D3D font
	//**********
	timeEndPeriod(1);
	delete g_GroundParticle;
	delete g_WaterLineParticle;
	delete g_JetParticle;
	delete g_Bullet;
	delete g_World;
	int i;
	if (g_ScriptLua) luaScriptEnd(g_ScriptLua);
	for (i = 0;i < GMODELMAX;i++)
		SAFE_DELETE(m_pXMesh[i]);
	SAFE_DELETE(m_pSkyMesh);
	SAFE_DELETE(g_pLandMesh);

	SAFE_RELEASE(pSprite);
	SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pFontL);
	SAFE_DELETE(m_pFont3D);

	SAFE_DELETE(g_pFont);


	// Cleanup DirectInput
	CleanupDirectInput();

	// Cleanup DirectX audio objects
//    SAFE_DELETE( m_pBoundSound1 );
//    SAFE_DELETE( m_pMusicManager );
	for (i = 0;i < GTEXMAX;i++) SAFE_RELEASE(pMyTexture[i]);
	// Write the settings to the registry
	WriteSettings();
	SetCurrentDirectory(AppDir);
	if (hMidiOut) {
		midiOutClose(hMidiOut);
		hMidiOut = NULL;
	}

	if (ScriptSource) delete ScriptSource;
	if (SystemSource) delete SystemSource;
	luaSystemEnd();
	delete g_DPlay;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CleanupDirectInput()
// Desc: Cleanup DirectInput 
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::CleanupDirectInput()
{
	if (NULL == m_pInputDeviceManager)
		return;

	// Get access to the list of semantically-mapped input devices
	// to delete all InputDeviceState structs
	CInputDeviceManager::DeviceInfo* pDeviceInfos;
	DWORD dwNumDevices;
	m_pInputDeviceManager->GetDevices(&pDeviceInfos, &dwNumDevices);

	for (DWORD i = 0; i < dwNumDevices; i++)
	{
		InputDeviceState* pInputDeviceState = (InputDeviceState*)pDeviceInfos[i].pParam;
		SAFE_DELETE(pInputDeviceState);
		pDeviceInfos[i].pParam = NULL;
	}

	// Cleanup DirectX input objects
	SAFE_DELETE(m_pInputDeviceManager);
	m_pInputDeviceManager = NULL;

}
bool IsShowDust()
{
	return ShowDustFlag;
}
int GetScreenWidth()
{
	return s_ScreenWidth;
}
int GetScreenHeight()
{
	return s_ScreenHeight;
}
int GetLandFaceNumber()
{
	return s_NumFace;
}