//-----------------------------------------------------------------------------
// File: Rigid.h
//
// Desc: Header file Rigid sample app
//-----------------------------------------------------------------------------
#ifndef AFX_RIGID_H__F533E4AC_435C_4933_8AC7_50A6C021E6DF__INCLUDED_
#define AFX_RIGID_H__F533E4AC_435C_4933_8AC7_50A6C021E6DF__INCLUDED_
#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <mmsystem.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <D3DX9.h>
#include <DXErr9.h>
#include <tchar.h>
#include <dinput.h>
#include "D3DUtil.h"
#include "DXUtil.h"
#include "D3DEnumeration.h"
#include "D3DSettings.h"
#include "D3DApp.h"
#include "D3DFont.h"
#include "D3DFile.h"
#include "DIUtil.h"
//#include "DMUtil.h"
#include "DSUtil.h"
#include "D3Dtypes.h"
#include "GVector.hpp"


typedef struct {
	int State;
	GVector Color;
	GVector Point;
	GVector Dir;
	float Scale;
} GRing;

//-----------------------------------------------------------------------------
// Defines, and constants
//-----------------------------------------------------------------------------
// TODO: change "DirectX AppWizard Apps" to your name or the company name
#define DXAPP_KEY        TEXT("Software\\RigidChips3D\\Rigid")

// DirectInput action mapper reports events only when buttons/axis change
// so we need to remember the present state of relevant axis/buttons for 
// each DirectInput device.  The CInputDeviceManager will store a 
// pointer for each device that points to this struct
struct InputDeviceState
{
	// TODO: change as needed
	FLOAT fAxisRotateUD;
	FLOAT fAxisRotateLR;
	FLOAT fAxisRotateIO;
	FLOAT fAxisRotateX;
	FLOAT fAxisRotateY;
	FLOAT fAxisRotateZ;
	FLOAT fAxisHat;
	BOOL  bButton[GKEYMAX];
	BOOL  bSystem[GSYSKEYMAX];
	BOOL  bButtonInit;
	BOOL  bButtonReset;
	BOOL  bButtonTitle;
	BOOL  bButtonZoomIn;
	BOOL  bButtonZoomOut;
	BOOL  bButtonResetView;
	BOOL  bButtonTurnUp;
	BOOL  bButtonTurnDown;
	BOOL  bButtonTurnLeft;
	BOOL  bButtonTurnRight;
	BOOL  bButtonYForce;
};


// Struct to store the current input state
struct UserInput
{
	// TODO: change as needed
	FLOAT fAxisRotateUD;
	FLOAT fAxisRotateLR;
	FLOAT fAxisRotateIO;
	FLOAT fAxisRotateX;
	FLOAT fAxisRotateY;
	FLOAT fAxisRotateZ;
	FLOAT fAxisHat;
	BOOL bButton[GKEYMAX];
	BOOL bButtonOneShot[GKEYMAX];
	BOOL bSystem[GSYSKEYMAX];
	BOOL bSystemOneShot[GSYSKEYMAX];
	BOOL  bButtonInit;
	BOOL  bButtonReset;
	BOOL  bButtonTitle;
	BOOL  bButtonZoomIn;
	BOOL  bButtonZoomOut;
	BOOL  bButtonResetView;
	BOOL  bButtonTurnUp;
	BOOL  bButtonTurnDown;
	BOOL  bButtonTurnLeft;
	BOOL  bButtonTurnRight;
	BOOL  bButtonYForce;
	BOOL  bButtonOneShotInit;
	BOOL  bButtonOneShotReset;
	BOOL  bButtonOneShotTitle;
	BOOL  bButtonOneShotZoomIn;
	BOOL  bButtonOneShotZoomOut;
	BOOL  bButtonOneShotResetView;
	BOOL  bButtonOneShotTurnUp;
	BOOL  bButtonOneShotTurnDown;
	BOOL  bButtonOneShotTurnLeft;
	BOOL  bButtonOneShotTurnRight;
	BOOL  bButtonOneShotYForce;
	BOOL bDoConfigureInput;
	BOOL bDoConfigureDisplay;
	BOOL bDoOpenChip;
	BOOL bDoOpenLand;
	BOOL bDoOpenGame;
	BOOL bDoOpenScenario;
	BOOL bDoCloseScenario;
	BOOL bDoUpdateChip;
	BOOL bDoOpenLog;
	BOOL bDoSaveLog;
};




//-----------------------------------------------------------------------------
// Name: class CMyD3DApplication
// Desc: Application class. The base class (CD3DApplication) provides the 
//       generic functionality needed in all Direct3D samples. CMyD3DApplication 
//       adds functionality specific to this sample program.
//-----------------------------------------------------------------------------
class CMyD3DApplication : public CD3DApplication
{
public:
	BOOL                    m_bLoadingApp;          // TRUE, if the app is loading
	CD3DFont*               m_pFont;                // Font for drawing text
	CD3DFont*               m_pFontL;                // Font for drawing text
	CD3DFont*               m_pFont3D;                // Font for drawing text
	//    ID3DXMesh*              m_pD3DXMesh;            // D3DX mesh to store teapot

	CInputDeviceManager*    m_pInputDeviceManager;  // DirectInput device manager
	DIACTIONFORMAT          m_diafGame;             // Action format for game play
	LPDIRECT3DSURFACE9      m_pDIConfigSurface;     // Surface for config'ing DInput devices
	UserInput               m_UserInput;            // Struct for storing user input 

	FLOAT                   m_fSoundPlayRepeatCountdown; // Sound repeat timer
	FLOAT                   m_fSoundPlayRepeatCountdown2; // Sound repeat timer

	FLOAT                   m_fWorldRotX;           // World rotation state X-axis
	FLOAT                   m_fWorldRotY;           // World rotation state Y-axis

	int						m_appID;
	BOOL					m_inputFocus;
	HIMC					m_hIMC;

public:
	HRESULT ViewSet();
	VOID    Pause(BOOL bPause);
protected:
	HRESULT LoadData(char *filename);
	HRESULT LoadProp(FILE *fp, int i);
	HRESULT SaveProp(FILE *fp, int i);
	HRESULT LoadLog(char *fname);
	HRESULT SaveLog(char *fname);
	HRESULT PlayLog();
	HRESULT OneTimeSceneInit();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void RenderSky();
	HRESULT Render();
	HRESULT FrameMove();
	HRESULT FinalCleanup();
	HRESULT ConfirmDevice(D3DCAPS9*, DWORD, D3DFORMAT);

	HRESULT RenderText();

	HRESULT InitInput(HWND hWnd);
	void	ClearInput(UserInput* pUserInput);
	void	DummyInput(UserInput* pUserInput);
	void    UpdateInput(UserInput* pUserInput);
	void    CleanupDirectInput();

	HRESULT InitAudio(HWND hWnd);

	VOID    ReadSettings();
	VOID    WriteSettings();

public:
	void SetRegulationMenu();
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	CMyD3DApplication();

	HRESULT ResetChips(float x, float z, float a);
	HRESULT ResetChips(float y, float a);
	HRESULT InitChips(float a, int hereFlag);
	HRESULT InputAddDeviceCB(CInputDeviceManager::DeviceInfo* pDeviceInfo, const DIDEVICEINSTANCE* pdidi);
	static HRESULT CALLBACK StaticInputAddDeviceCB(CInputDeviceManager::DeviceInfo* pDeviceInfo, const DIDEVICEINSTANCE* pdidi, LPVOID pParam);
	BOOL    ConfigureInputDevicesCB(IUnknown* pUnknown);
	static BOOL CALLBACK StaticConfigureInputDevicesCB(IUnknown* pUnknown, VOID* pUserData);
private:
	HRESULT _record();
	HRESULT _show_network();
	HRESULT _check_FPS_setting();
	HRESULT _update_information(DWORD t);
	HRESULT _update_system_input();
	HRESULT _send_network_bullet(DWORD t, bool moveEnd);
	HRESULT _send_network_explosion(DWORD t, bool moveEnd);
	HRESULT _send_network_scenario_message(DWORD t, bool moveEnd);
	HRESULT _send_network_model_full(DWORD t, bool moveEnd);
	HRESULT _send_network_model_short(DWORD t,bool moveEnd);
	HRESULT _check_checkpoint();
	HRESULT _acquire_user_input();
	HRESULT _display_data();
	HRESULT _display_extra_data();
	HRESULT _acquire_user_model_input();
	HRESULT _update_keys();
	HRESULT _update_vals();
	HRESULT _apply_vals();
	
	DWORD m_lastT;
	DWORD m_lastT2;
	DWORD m_lastT3;
	int m_preSound;
	int m_soundCount;
	GFloat m_a;
	int m_count;
	GFloat m_preY;


	//s_FPSの監視
	int m_netON;
	int m_saves_LimitFPS;
};

bool IsShowDust();
int GetScreenWidth();
int GetScreenHeight();
int GetLandFaceNumber();
double GetFPS();
int GetLimitFPS();
//Init直後の無敵時間かどうか
bool IsInvulnerableTime();
//総出力
GFloat GetTotalPower();
void AddTotalPower(GFloat v);
//起動からのフレーム数。Win32とかぶるので名前にRC入れてる
int GetRCTickCount();
void SetRCTickCount(int v);
int GetRCSystemTickCount();
//シナリオから送るメッセージ分を入力
void SetMessageData(const char* str);
int GetReceiveMessageCode(int playerNumber);
void ClearReceiveMessage(int playerNumber);
const char* GetMessageData(int playerNumber);
//_SKEY関係
bool GetSkey(int n);
bool GetSkeyUp(int n);
bool GetSkeyDown(int n);
//各種ファイルのフォルダを取得

const TCHAR* GetAppDir();
const TCHAR* GetResourceDir();
const TCHAR* GetDataDir();
const TCHAR* GetCurrentDataDir();
const TCHAR* GetCurrentScenarioDir();

#define Resize3DEnvironment Reset3DEnvironment

#endif // !defined(AFX_RIGID_H__F533E4AC_435C_4933_8AC7_50A6C021E6DF__INCLUDED_)
