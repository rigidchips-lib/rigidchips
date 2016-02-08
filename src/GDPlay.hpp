#pragma warning(disable : 4996)
#ifndef G_GDPLAY_HPP
#define G_GDPLAY_HPP
#include <windows.h>
#include <commctrl.h>
#include <basetsd.h>
#include <dplay8.h>
//#include <dplobby8.h>
#include <dplobby.h>
#include <dxerr8.h>
#include <locale.h>

//メモリリーク検出用
#include <crtdbg.h>  
#ifdef _DEBUG 
#ifdef __MEMLEAKCHECK 
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__) 
#define malloc(p1) _malloc_dbg((p1),_NORMAL_BLOCK,__FILE__,__LINE__) 
#define __MEMLEAKCHECK
#endif
#endif 

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//-----------------------------------------------------------------------------
// Player context locking defines
//-----------------------------------------------------------------------------
#define PLAYER_LOCK(x)                   EnterCriticalSection( &((x)->m_csPlayerContext) ); 
#define PLAYER_ADDREF( pPlayerInfo )    if( pPlayerInfo ) pPlayerInfo->lRefCount++;
#define PLAYER_RELEASE( pPlayerInfo )   if( pPlayerInfo ) { pPlayerInfo->lRefCount--; if( pPlayerInfo->lRefCount <= 0 ) SAFE_DELETE( pPlayerInfo ); } pPlayerInfo = NULL;
#define PLAYER_UNLOCK(x)                 LeaveCriticalSection(  &((x)->m_csPlayerContext) );

#define GPLAYERMAX 30

#define MAX_PLAYER_NAME         128

// プレイヤー情報
struct MYAPP_PLAYER_INFO
{
    LONG  lRefCount;                        // Ref count so we can cleanup when all threads 
                                            // are done w/ this object
    DPNID dpnidPlayer;                      // DPNID of player
    TCHAR strPlayerName[MAX_PLAYER_NAME];   // Player name
    DWORD color;   // Player Color
};
//-----------------------------------------------------------------------------
// App specific DirectPlay messages and structures 
//-----------------------------------------------------------------------------
//#define GAME_MSGID_DATA        1

// Change compiler pack alignment to be BYTE aligned, and pop the current value
#pragma pack( push, 1 )


typedef HRESULT ( * ReceiveFunc ) ( MYAPP_PLAYER_INFO*,DWORD,BYTE * ) ;
typedef HRESULT ( * CreateFunc ) ( MYAPP_PLAYER_INFO*) ;
typedef HRESULT ( * DestroyFunc ) ( MYAPP_PLAYER_INFO*) ;
typedef HRESULT ( * TerminateFunc ) ( void ) ;

static WCHAR g_wstrAppName[] = L"RigidChips";
static char g_strAppName[] = "RigidChips";
static WCHAR g_wstrExecutable[256];
static char g_strExecutable[256];


class GDPlay {




public:
	GDPlay () {
		m_hInst=NULL;
		m_pDP=NULL;
		m_MaxPlayers=GPLAYERMAX;
		m_dpnidLocalPlayer=0;
		m_dpnidHostPlayer=0;
		m_NumberOfActivePlayers=0;
		m_ReceiveFunc=NULL;
		m_CreateFunc=NULL;
		m_DestroyFunc=NULL;
		m_TerminateFunc=NULL;
		m_pLobbyApp=NULL;
		m_hHostAsyncOp=NULL;
		m_hConnectAsyncOp=NULL;
		m_Connect=false;
	}
	~GDPlay () {
		DeleteCriticalSection( &m_csPlayerContext );
		End();
	}
	IDirectPlay8LobbiedApplication* GetLobbyApp() {
		return m_pLobbyApp;
	}
	void SetReceiveFunc(ReceiveFunc func) {
		m_ReceiveFunc=func;
	}
	void SetCreateFunc(CreateFunc func) {
		m_CreateFunc=func;
	}
	void SetDestroyFunc(DestroyFunc func) {
		m_DestroyFunc=func;
	}
	void SetTerminateFunc(TerminateFunc func) {
		m_TerminateFunc=func;
	}

	BOOL GetConnect() {
		return m_Connect;
	}
	void SetConnect(BOOL f) {
		m_Connect=f;
	}
	
	DPNID GetLocalPlayerDPNID() {
		return m_dpnidLocalPlayer;
	}
	DPNID GetHostPlayerDPNID() {
		return m_dpnidHostPlayer;
	}
	HRESULT GetSessionName(char *strSessionName) {
		HRESULT	hr=S_OK;
		strSessionName[0]='\0';
		DPN_APPLICATION_DESC	*dpnAppDesc;
		DWORD size=0;
		//データサイズを取得する
		m_pDP->GetApplicationDesc(NULL,&size,0);

		if(size==0) return FALSE;

		// Allocate the memory
		dpnAppDesc = (DPN_APPLICATION_DESC*) new BYTE[ size ];

		// Initialize the struct
		ZeroMemory( dpnAppDesc, size );
		// 各メンバを設定する
		dpnAppDesc->dwSize = sizeof(DPN_APPLICATION_DESC);
		if(m_pDP->GetApplicationDesc(dpnAppDesc,&size,0)==S_OK) {;
			//ワイド文字列からマルチバイト文字列への変換
			DWORD len = WideCharToMultiByte(CP_ACP,0,(LPCWSTR)dpnAppDesc->pwszSessionName,-1,NULL,0,NULL,NULL);
			WideCharToMultiByte(CP_ACP,0,(LPCWSTR)dpnAppDesc->pwszSessionName,-1,strSessionName,len,NULL,NULL);
			strSessionName[len] = '\0';
		}
		SAFE_DELETE(dpnAppDesc);
		return hr; 
	}
	DWORD GetCurrentPlayers() {
		HRESULT	hr=S_OK;
		DPN_APPLICATION_DESC	*dpnAppDesc;
		DWORD size=0;
		//データサイズを取得する
		m_pDP->GetApplicationDesc(NULL,&size,0);

		// Allocate the memory
		dpnAppDesc = (DPN_APPLICATION_DESC*) new BYTE[ size ];

		// Initialize the struct
		ZeroMemory( dpnAppDesc, size );
		// 各メンバを設定する
		dpnAppDesc->dwSize = sizeof(DPN_APPLICATION_DESC);
		m_pDP->GetApplicationDesc(dpnAppDesc,&size,0);

		DWORD n=dpnAppDesc->dwCurrentPlayers;
		SAFE_DELETE(dpnAppDesc);
		return n;

	}

	HRESULT GetHostName(char *strHostName,char *strProvider) {
		HRESULT	hr=S_OK;
		LPDIRECTPLAY8ADDRESS *rpAddresses = NULL;   
        DWORD dwNumAddresses = 0;

		strProvider[0]='\0';
		strHostName[0]='\0';
		if(m_dpnidHostPlayer==0 && m_dpnidLocalPlayer==0) {//未設定
			return hr;
		}
		if(m_dpnidHostPlayer==m_dpnidLocalPlayer) { //ホストとして設定
			//アドレスを保持するのに必要なバッファサイズの決定
			hr = m_pDP->GetLocalHostAddresses( rpAddresses, &dwNumAddresses, 0 );
			if(hr!=DPNERR_BUFFERTOOSMALL) goto LCleanReturn;
			// 配列の作成
			rpAddresses = new LPDIRECTPLAY8ADDRESS[ dwNumAddresses ];
			if(rpAddresses==NULL) {
				hr = E_OUTOFMEMORY;
				goto LCleanReturn;
			}
			ZeroMemory( rpAddresses, dwNumAddresses * sizeof(LPDIRECTPLAY8ADDRESS) );
			// アドレスの回復
			hr = m_pDP->GetLocalHostAddresses( rpAddresses, &dwNumAddresses, 0 );
			if( FAILED(hr) ) goto LCleanReturn;
		}
		else {	//リモートセッションとして設定
			dwNumAddresses = 1;

			// 配列の作成
			rpAddresses = new LPDIRECTPLAY8ADDRESS[ dwNumAddresses ];
			if(rpAddresses==NULL) {
				hr = E_OUTOFMEMORY;
				goto LCleanReturn;
			}
			ZeroMemory( rpAddresses, dwNumAddresses * sizeof(LPDIRECTPLAY8ADDRESS) );
			// アドレスの回復
			hr = m_pDP->GetPeerAddress( m_dpnidHostPlayer, &rpAddresses[0], 0 );
			if( FAILED(hr) ) goto LCleanReturn;
		}


		if( dwNumAddresses > 0 ){
			// DirectPlayアドレスからプロバイダ名文字列を取得
			GUID guidSP;
			hr = rpAddresses[0]->GetSP( &guidSP );
			if( FAILED(hr) ) goto LCleanReturn;
	  
			if( CLSID_DP8SP_IPX == guidSP )
				strcpy( strProvider,"IPX");
			else if( CLSID_DP8SP_TCPIP == guidSP )
				strcpy( strProvider, "TCP/IP");
			else if( CLSID_DP8SP_MODEM == guidSP )
				strcpy( strProvider, "Modem");
			else if( CLSID_DP8SP_SERIAL == guidSP )
				strcpy( strProvider, "Serial");
			else if( CLSID_DP8SP_BLUETOOTH == guidSP )
				strcpy( strProvider,"Bluetooth");
	        
			// TCP/IPまたはIPXを使っているときは, アドレスリストを作成
			if( CLSID_DP8SP_IPX == guidSP || CLSID_DP8SP_TCPIP == guidSP ){
				for(int i=0; i < (int)dwNumAddresses; i++ ){
					//DirectPlayアドレスからホスト名文字列を取得
					WCHAR wstrHostname[ 256 ] = {0};
					DWORD dwSize = sizeof(wstrHostname);
					DWORD dwDataType = DPNA_DATATYPE_STRING;
					hr = rpAddresses[ i ]->GetComponentByName( DPNA_KEY_HOSTNAME, wstrHostname, &dwSize, &dwDataType );
					if( FAILED(hr) ) goto LCleanReturn;
					char buf[256];
					//ワイド文字列からマルチバイト文字列への変換
					DWORD len = WideCharToMultiByte(CP_ACP,0,(LPCWSTR)wstrHostname,-1,NULL,0,NULL,NULL);
					WideCharToMultiByte(CP_ACP,0,(LPCWSTR)wstrHostname,-1,buf,len,NULL,NULL);
					buf[len] = '\0';
					//ホストネームの連結
					if(i>0) strcat(strHostName," , ");
					strcat(strHostName,buf);
				}
			}
		}

		//廃棄/リターン処理
		LCleanReturn:
			for(int i=0; i < (int)dwNumAddresses; i++ )
				SAFE_RELEASE( rpAddresses[i] );

			SAFE_DELETE_ARRAY( rpAddresses );

			return hr;
	};


	HRESULT Init(HINSTANCE hInst) {
		HRESULT	hr;
		CoInitialize(NULL);
		m_hInst = hInst;
		InitializeCriticalSection( &m_csPlayerContext );
		// DirectPlay8Peerインタフェースを取得する
		m_pDP = NULL;
		hr = CoCreateInstance(CLSID_DirectPlay8Peer, NULL, CLSCTX_INPROC_SERVER,
							IID_IDirectPlay8Peer, (void **)&m_pDP);
		if (FAILED(hr))
		{
			// 取得に失敗した
			MessageBox(NULL,"Failed to create instance of IDirectPlay8Peer.", 
					"RigidChips Network", MB_OK | MB_ICONWARNING);
			return FALSE;
		}

		// DirectPlay8Peerインタフェースの初期化
		hr = m_pDP->Initialize((LPVOID)this, &myMessageHandler, 0);
		if (FAILED(hr))
		{
			// 初期化に失敗した
			MessageBox(NULL,"Failed initializing IDirectPlay8Peer.", 
					"RigidChips Network", MB_OK | MB_ICONWARNING);
			m_pDP->Release();
			return FALSE;
		}

		CoUninitialize();

		return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。

	}
	void NewSession(char *sessionName,char *username,int tcpPort,GUID appGuid) 
	{
		// 新規にセッションを開始する
		IDirectPlay8Address	*prgDeviceInfo;
		HRESULT	hr;
		WCHAR wSessionName[256];
		WCHAR wUsername[256];
		//マルチバイト文字列からワイド文字列への変換
		DWORD len = MultiByteToWideChar(CP_UTF8,0,(LPCSTR)sessionName,-1,NULL,0);
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)sessionName,-1,wSessionName,len);
		wSessionName[len] = '\0';
		len = MultiByteToWideChar(CP_UTF8,0,(LPCSTR)username,-1,NULL,0);
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)username,-1,wUsername,len);
		wUsername[len] = '\0';


		// DirectPlay8Addressオブジェクトの作成
		hr = CoCreateInstance(CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER,
							IID_IDirectPlay8Address, (void **)&prgDeviceInfo);
		if (FAILED(hr))
		{
			MessageBox(NULL,"Failed  to create instance of IDirectPlay8Peer.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			return;
		}

		// サービス・プロバイダとしてTCP/IPを選択する
		hr = prgDeviceInfo->BuildFromURLA("x-directplay:/provider=%7BEBFE7BA0-628D-11D2-AE0F-006097B01411%7D");
		if (FAILED(hr))
		{
			MessageBox(NULL,"Failed setting provider.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			prgDeviceInfo->Release();
			return;
		}

		// ポート番号の設定
		DWORD port = tcpPort;
		hr = prgDeviceInfo->AddComponent(L"port", (void*)&port, sizeof(DWORD), DPNA_DATATYPE_DWORD);
		if (FAILED(hr))
		{
			MessageBox(NULL,"Failed setting port number.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			prgDeviceInfo->Release();
			return;
		}

		// プレーヤー情報を設定する
		DPN_PLAYER_INFO	dpnPlayerInfo;


		dpnPlayerInfo.dwSize = sizeof(DPN_PLAYER_INFO);
		dpnPlayerInfo.dwInfoFlags = DPNINFO_NAME;
		dpnPlayerInfo.pwszName = wUsername;
		dpnPlayerInfo.pvData = NULL;
		dpnPlayerInfo.dwDataSize = 0;
		dpnPlayerInfo.dwPlayerFlags = 0;

		hr = m_pDP->SetPeerInfo(&dpnPlayerInfo, NULL, NULL, DPNSETPEERINFO_SYNC);

		if (FAILED(hr))
		{
			MessageBox(NULL,"Failed setting player infomation.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			prgDeviceInfo->Release();
			return;
		}

		// セッションを作成する
		DPN_APPLICATION_DESC	dpnAppDesc;

		// 構造体を0で初期化する
		memset((void*)&dpnAppDesc, 0, sizeof(DPN_APPLICATION_DESC));

		// 各メンバを設定する
		dpnAppDesc.dwSize = sizeof(DPN_APPLICATION_DESC);
		dpnAppDesc.dwFlags =  DPNSESSION_MIGRATE_HOST;
		dpnAppDesc.guidApplication = appGuid;
		dpnAppDesc.dwMaxPlayers    = m_MaxPlayers;
		dpnAppDesc.pwszSessionName = wSessionName;

		hr = m_pDP->Host(&dpnAppDesc, &prgDeviceInfo, 1, 
									NULL, NULL, NULL, 
									DPNHOST_OKTOQUERYFORADDRESSING);

		m_hHostAsyncOp=NULL;
		if (FAILED(hr))
		{
			// IDirectPlay8Addressインタフェースの解放
			prgDeviceInfo->Release();
			MessageBox(NULL,"Failed  to create session.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			return;
		}
		// IDirectPlay8Addressインタフェースの解放
		prgDeviceInfo->Release();
		m_Connect=true;
		return;
	}
	void ConnectSession(char *hostname,char *username,int tcpPort,GUID appGuid) 
	{
		static DWORD lastTIme=0;
		// 既存のセッションに参加する
		IDirectPlay8Address	*prgDeviceInfo;	// 接続に使うサービス・プロバイダなど
		IDirectPlay8Address	*prgHostAddr;	// 接続先のホスト情報
		HRESULT	hr;
		WCHAR wUsername[256]={0};
		WCHAR wHostname[256]={0};
		//マルチバイト文字列からワイド文字列への変換
		DWORD len = MultiByteToWideChar(CP_UTF8,0,(LPCSTR)hostname,-1,NULL,0);
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)hostname,-1,wHostname,len);
		wHostname[len] = '\0';
		len = MultiByteToWideChar(CP_UTF8,0,(LPCSTR)username,-1,NULL,0);
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)username,-1,wUsername,len);
		wUsername[len] = '\0';

		// DirectPlay8Addressオブジェクトの作成
		hr = CoCreateInstance(CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER,
							IID_IDirectPlay8Address, (void **)&prgDeviceInfo);
		if (FAILED(hr))
		{
			MessageBox(NULL,"Failed  to create instance of IDirectPlay8Address.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			return;
		}

		// サービス・プロバイダとしてTCP/IPを選択する
		hr = prgDeviceInfo->BuildFromURLA("x-directplay:/provider=%7BEBFE7BA0-628D-11D2-AE0F-006097B01411%7D");
		if (FAILED(hr))
		{
			MessageBox(NULL,"Failed setting provider.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			prgDeviceInfo->Release();
			return;
		}

		// 接続先の情報を設定する
		// ここではDuplicateメソッドを使って複製することでTCP/IPが選択されたIDirectPlay8Addressインタフェースを得る
		hr = prgDeviceInfo->Duplicate(&prgHostAddr);
		if (FAILED(hr))
		{
			MessageBox(NULL,"Failed to duplicate IDirectPlay8Address.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			prgDeviceInfo->Release();
			return;
		}

		// ポート番号の設定
		DWORD port = tcpPort;
		hr = prgHostAddr->AddComponent(DPNA_KEY_HOSTNAME, wHostname, (DWORD) (wcslen(wHostname)+1)*sizeof(WCHAR), DPNA_DATATYPE_STRING);
		hr = prgHostAddr->AddComponent(DPNA_KEY_PORT, (void*)&port, sizeof(DWORD), DPNA_DATATYPE_DWORD);
		if (FAILED(hr))
		{
			MessageBox(NULL,"Failed setting port number.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			prgDeviceInfo->Release();
			prgHostAddr->Release();
			return;
		}

		// プレーヤー情報を設定する
		DPN_PLAYER_INFO	dpnPlayerInfo;


		dpnPlayerInfo.dwSize = sizeof(DPN_PLAYER_INFO);
		dpnPlayerInfo.dwInfoFlags = DPNINFO_NAME;
		dpnPlayerInfo.pwszName = wUsername;
		dpnPlayerInfo.pvData = NULL;
		dpnPlayerInfo.dwDataSize = 0;
		dpnPlayerInfo.dwPlayerFlags = 0;

		hr = m_pDP->SetPeerInfo(&dpnPlayerInfo, NULL, NULL, DPNCONNECT_SYNC);
		if (FAILED(hr))
		{
			MessageBox(NULL,"Failed setting player infomation.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			prgDeviceInfo->Release();
			prgHostAddr->Release();
			return;
		}
		// セッションに接続する
		DPN_APPLICATION_DESC	dpnAppDesc;

		// 構造体を0で初期化する
		memset((void*)&dpnAppDesc, 0, sizeof(DPN_APPLICATION_DESC));

		// 各メンバを設定する
		dpnAppDesc.dwSize = sizeof(DPN_APPLICATION_DESC);
		dpnAppDesc.guidApplication = appGuid;
		
		// 接続する
		hr = m_pDP->Connect(&dpnAppDesc, prgHostAddr, prgDeviceInfo, 
						NULL, NULL, NULL, 0, NULL, NULL, &m_hConnectAsyncOp, 
						DPNHOST_OKTOQUERYFORADDRESSING);


		DWORD lastTime=timeGetTime();
		while(lastTime+10000>timeGetTime()) {
			if(GetAsyncKeyState(VK_ESCAPE)!=0) break;
			if(m_dpnidLocalPlayer!=0 || m_hConnectAsyncOp==NULL) {
				break;
			}
			Sleep(10);
		}
		if(m_hConnectAsyncOp) m_pDP->CancelAsyncOperation( m_hConnectAsyncOp, 0 );
		// IDirectPlay8Addressインタフェースの解放
		m_hConnectAsyncOp=NULL;
		prgDeviceInfo->Release();
		prgHostAddr->Release();


		if (FAILED(hr))
		{
			if(hr==DPNERR_SESSIONFULL) 
				MessageBox(NULL,"The maximum number of players allotted for the session has been reached.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			else 
				MessageBox(NULL,"Failed to connect session.", "RigidChips Network", MB_OK | MB_ICONWARNING);
			return;
		}


	}
	void CancelConnect(){
		if(m_hConnectAsyncOp)m_pDP->CancelAsyncOperation( m_hConnectAsyncOp, 0 );
		m_hConnectAsyncOp=NULL;
	}
	bool GetCancelConnect(){
		if(m_hConnectAsyncOp) return true;
		return false;
	}
	HRESULT SendAll(BYTE *data,DWORD size) {
		HRESULT	hr=S_OK;
        // Send a message to all of the players

        DPN_BUFFER_DESC bufferDesc;
        bufferDesc.dwBufferSize = size;
        bufferDesc.pBufferData  = (BYTE*) data;

        DPNHANDLE hAsync;
        m_pDP->SendTo( DPNID_ALL_PLAYERS_GROUP, &bufferDesc, 1,
                       60, NULL, &hAsync, DPNSEND_NOLOOPBACK|DPNSEND_NOCOMPLETE );
		return hr;
	}
	HRESULT SendTo(DPNID dpnid,BYTE *data,DWORD size) {
		HRESULT	hr=S_OK;
        // Send a message to all of the players

        DPN_BUFFER_DESC bufferDesc;
        bufferDesc.dwBufferSize = size;
        bufferDesc.pBufferData  = (BYTE*) data;

        DPNHANDLE hAsync;
        m_pDP->SendTo( dpnid, &bufferDesc, 1,
                       60, NULL, &hAsync, DPNSEND_NOLOOPBACK|DPNSEND_NOCOMPLETE );
		return hr;
	}
	int GetNumPlayers(){
		return m_NumberOfActivePlayers;
	}
	int GetMaxPlayers(){
		return m_MaxPlayers;
	}
	DWORD GetPlayersDPID(DPNID *dpnid){
		DWORD	cbDPNID=0;
		m_pDP->EnumPlayersAndGroups(NULL,&cbDPNID,DPNENUM_PLAYERS);
		m_pDP->EnumPlayersAndGroups(dpnid,&cbDPNID,DPNENUM_PLAYERS);
		return cbDPNID;
	}
	HRESULT GetPlayersName(DPNID dpnid,char *name){
		if(dpnid==0) {name[0]='\0';return DPN_OK;}
		DPN_PLAYER_INFO *playerInfo;
		DWORD cbPlayerInfo=0;
		m_pDP->GetPeerInfo(dpnid,NULL,&cbPlayerInfo,0);
		playerInfo=(DPN_PLAYER_INFO *)malloc(cbPlayerInfo);
		memset(playerInfo,0,cbPlayerInfo);
		playerInfo->dwSize=sizeof(DPN_PLAYER_INFO);
		m_pDP->GetPeerInfo(dpnid,playerInfo,&cbPlayerInfo,0);
		//ワイド文字列からマルチバイト文字列への変換
		DWORD len = WideCharToMultiByte(CP_ACP,0,(LPCWSTR)playerInfo->pwszName,-1,NULL,0,NULL,NULL);
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)playerInfo->pwszName,-1,name,len,NULL,NULL);
		name[len] = '\0';
		delete playerInfo;
		return DPN_OK;
	}
	void Close(){
		if (m_pDP != NULL){
			m_pDP ->Close(DPNCLOSE_IMMEDIATE);
			m_Connect=false;
			m_pDP->Release();
			m_pDP=NULL;
		}

		m_pLobbyApp=NULL;
		m_NumberOfActivePlayers=0;
		m_dpnidLocalPlayer=0;
		m_dpnidHostPlayer=0;
//		CoUninitialize();
		DeleteCriticalSection( &m_csPlayerContext );
		Init(m_hInst);
	}
	void End(){
		if (m_pDP != NULL){
			m_pDP ->Close(DPNCLOSE_IMMEDIATE);
			m_pDP->Release();
			m_Connect=false;
			m_pDP=NULL;
		}

		m_pLobbyApp=NULL;
		DeleteCriticalSection( &m_csPlayerContext );
		m_NumberOfActivePlayers=0;
		m_dpnidLocalPlayer=0;
		m_dpnidHostPlayer=0;
	}
	static HRESULT CALLBACK myMessageHandler(PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage)
	{
		// メッセージハンドラ
		//	pvUsercontext = Initializeメソッドの第1引数に渡した値(このクラスへのポインタ)
		//	dwMessageType = 届いたメッセージの種類
		//	pMessage = 届いたメッセージ本体。内容はdwMessageTypeの値によって異なる
		GDPlay *gdplay=(GDPlay*)pvUserContext;
		
		static int y = 20;

        EnterCriticalSection( &gdplay->m_csPlayerContext );
		switch(dwMessageType)
		{
		case DPN_MSGID_CREATE_PLAYER:
			// プレイヤーが作成されたときの処理
			{
				HRESULT hr;
				PDPNMSG_CREATE_PLAYER pCreatePlayerMsg;
				pCreatePlayerMsg = (PDPNMSG_CREATE_PLAYER)pMessage;

				// プレイヤー情報MYAPP_PLAYER_INFOを作成し値を設定する
				MYAPP_PLAYER_INFO* pPlayerInfo = new MYAPP_PLAYER_INFO;
				if( NULL == pPlayerInfo )break;

				PLAYER_LOCK(gdplay);                  // enter player context CS
				ZeroMemory( pPlayerInfo, sizeof(MYAPP_PLAYER_INFO) );
				pPlayerInfo->lRefCount   = 1;
				pPlayerInfo->dpnidPlayer = pCreatePlayerMsg->dpnidPlayer;
				PLAYER_UNLOCK(gdplay);                  // enter player context CS

				if(gdplay->m_CreateFunc!=NULL) gdplay->m_CreateFunc(pPlayerInfo);

				// ピア情報の取得
				DWORD dwSize = 0;
				DPN_PLAYER_INFO* pdpPlayerInfo = NULL;
				hr = DPNERR_CONNECTING;
	            
				while( hr == DPNERR_CONNECTING ) 
					hr = gdplay->m_pDP->GetPeerInfo( pCreatePlayerMsg->dpnidPlayer, pdpPlayerInfo, &dwSize, 0 );                                
	                
				if( hr == DPNERR_BUFFERTOOSMALL )
				{
					pdpPlayerInfo = (DPN_PLAYER_INFO*) new BYTE[ dwSize ];
					if( NULL == pdpPlayerInfo )
					{
						SAFE_DELETE( pPlayerInfo );
						break;
					}

					ZeroMemory( pdpPlayerInfo, dwSize );
					pdpPlayerInfo->dwSize = sizeof(DPN_PLAYER_INFO);
	                
					hr = gdplay->m_pDP->GetPeerInfo( pCreatePlayerMsg->dpnidPlayer, pdpPlayerInfo, &dwSize, 0 );
					if( SUCCEEDED(hr) )
					{

						if( pdpPlayerInfo->dwPlayerFlags & DPNPLAYER_LOCAL )
							gdplay->m_dpnidLocalPlayer = pCreatePlayerMsg->dpnidPlayer;
						if( pdpPlayerInfo->dwPlayerFlags & DPNPLAYER_HOST )
							gdplay->m_dpnidHostPlayer = pCreatePlayerMsg->dpnidPlayer;
						//ワイド文字列からマルチバイト文字列への変換
						DWORD len = WideCharToMultiByte(CP_ACP,0,(LPCWSTR)pdpPlayerInfo->pwszName,-1,NULL,0,NULL,NULL);
						WideCharToMultiByte(CP_ACP,0,(LPCWSTR)pdpPlayerInfo->pwszName,-1,pPlayerInfo->strPlayerName,len,NULL,NULL);
						pPlayerInfo->strPlayerName[len] = '\0';
					}
	                
					SAFE_DELETE_ARRAY( pdpPlayerInfo );
				}

				pCreatePlayerMsg->pvPlayerContext = pPlayerInfo;

				// Update the number of active players, and 
				// post a message to the dialog thread to update the 
				// UI.  This keeps the DirectPlay message handler 
				// from blocking
				InterlockedIncrement( &(gdplay->m_NumberOfActivePlayers) );

			}
			break;

		case DPN_MSGID_DESTROY_PLAYER:
			// プレイヤーが削除されたときの処理
			{
				PDPNMSG_DESTROY_PLAYER pDestroyPlayerMsg;
				pDestroyPlayerMsg = (PDPNMSG_DESTROY_PLAYER)pMessage;
				MYAPP_PLAYER_INFO* pPlayerInfo = (MYAPP_PLAYER_INFO*) pDestroyPlayerMsg->pvPlayerContext;
				if(gdplay->m_DestroyFunc!=NULL) gdplay->m_DestroyFunc(pPlayerInfo);

				PLAYER_LOCK(gdplay);                  // enter player context CS
				PLAYER_RELEASE( pPlayerInfo );  // Release player and cleanup if needed
				PLAYER_UNLOCK(gdplay);                // leave player context CS


				InterlockedDecrement( &(gdplay->m_NumberOfActivePlayers) );


			}
			break;
		case DPN_MSGID_HOST_MIGRATE:
			{
				PDPNMSG_HOST_MIGRATE pHostMigrateMsg;
				pHostMigrateMsg = (PDPNMSG_HOST_MIGRATE)pMessage;


				gdplay->m_dpnidHostPlayer = pHostMigrateMsg->dpnidNewHost;

			}
			break;

		case DPN_MSGID_TERMINATE_SESSION:
			{
				gdplay->m_Connect=false;
				PDPNMSG_TERMINATE_SESSION pTerminateSessionMsg;
				pTerminateSessionMsg = (PDPNMSG_TERMINATE_SESSION)pMessage;

				if(gdplay->m_TerminateFunc!=NULL) gdplay->m_TerminateFunc();

			}
			break;

		case DPN_MSGID_RECEIVE:
 			// メッセージが届いたときの処理
			{
				PDPNMSG_RECEIVE pReceiveMsg;
				pReceiveMsg = (PDPNMSG_RECEIVE)pMessage;
				MYAPP_PLAYER_INFO* pPlayerInfo = (MYAPP_PLAYER_INFO*) pReceiveMsg->pvPlayerContext;
				if( NULL == pPlayerInfo )
					break;

				PLAYER_LOCK(gdplay);                  // enter player context CS
				DWORD size=pReceiveMsg->dwReceiveDataSize;
				BYTE *data = (BYTE *) pReceiveMsg->pReceiveData;
				if(gdplay->m_ReceiveFunc!=NULL) gdplay->m_ReceiveFunc(pPlayerInfo,size,data);
				PLAYER_UNLOCK(gdplay);                  // enter player context CS
			}
			break;
		}

        LeaveCriticalSection( &gdplay->m_csPlayerContext );
		return DPN_OK;
	};
	private:
		HINSTANCE			m_hInst;
		CRITICAL_SECTION	m_csPlayerContext;
		IDirectPlay8Peer*	m_pDP;	// DirectPlay peer object
		LONG				m_MaxPlayers;

		LONG				m_NumberOfActivePlayers;	// アクティブプレイヤーの数

		ReceiveFunc			m_ReceiveFunc;
		CreateFunc			m_CreateFunc;
		DestroyFunc			m_DestroyFunc;
		TerminateFunc		m_TerminateFunc;

		DPNID				m_dpnidLocalPlayer;      // ローカルプレイヤーのDPNID 
		DPNID				m_dpnidHostPlayer;       // ホストプレイヤーのDPNID
		IDirectPlay8LobbiedApplication*  m_pLobbyApp;    // LobbyApp object

		DPNHANDLE           m_hHostAsyncOp;
		DPNHANDLE           m_hConnectAsyncOp;
		BOOL				m_Connect;
};

#endif
