// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "LSWebBroker.h"
#include "MainFrm.h"
#include ".\mainfrm.h"
#include "Util\HelpFunc.h"
#include "stringmanager\iostringmanager.h"
#include "stringmanager\safesprintf.h"
#include "Local\ioLocalManager.h"
#include "Local\ioLocalParent.h"
#include "Local\ioLocalUS.h"
#include <strsafe.h>
#include "Util\ioINILoaderAU.h"
#include "Util\HttpManager.h"
#include "curl/curl.h"
#include "cJson/cJSON.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <time.h>
#include "Util/ioEncrypted.h"

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_h_mutex        = NULL;
	m_pStringMgr     = NULL;
	m_pLocalMgr      = NULL;
	m_pHttpMgr       = NULL;
	m_nSubType		= 0;
}

CMainFrame::~CMainFrame()
{
	SAFEDELETE( m_pStringMgr );
	SAFEDELETE( m_pLocalMgr );
	SAFEDELETE( m_pHttpMgr );
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	enum DulicationType
	{
		DT_NONE        = 0,
		DT_WEBBROKER   = 1,
		DT_AUTOUPGRADE = 2,
		DT_CLIENT      = 3,
	};

	DulicationType eDulicationType = DT_NONE;

	// webbroker
	m_h_mutex = CreateMutex(NULL, true, APPMUTEXNAME);
	if(GetLastError() == ERROR_ALREADY_EXISTS)			
		eDulicationType = DT_WEBBROKER;

	if( eDulicationType == DT_NONE )
	{
		// autoupgrade
		HANDLE hMutex = OpenMutex( MUTEX_ALL_ACCESS, FALSE, AUTOUPGRADEMUTEXNAME );  
		if ( hMutex ) 
		{
			eDulicationType = DT_AUTOUPGRADE;
			CloseHandle( hMutex );
		}
	}

	if( eDulicationType == DT_NONE)
	{
		// client
		HANDLE hMutex = OpenMutex( MUTEX_ALL_ACCESS, FALSE, CLIENTMUTEXNAME );  
		if ( hMutex ) 
		{
			eDulicationType = DT_CLIENT;
			CloseHandle( hMutex );
		}
	}

	if( !AfxGetApp() )
	{
		MessageBox( "AfxGetApp() == NULL.", "LostSaga_Webbroker - Error", MB_TOPMOST );
		DestroyWindow();
		return 0;
	}

	char szStartLocal[MAX_PATH]="";
	char szStartURL[MAX_PATH*2]="";
	char szStartReg[MAX_PATH]="";

	

	m_pLocalMgr = new ioLocalManager;
	if( m_pLocalMgr )
	{
		m_pLocalMgr->Init();
		if( !m_pLocalMgr->ParseCmdForSetLocal( AfxGetApp()->m_lpCmdLine ) )
		{
			ZeroMemory( szStartLocal, sizeof( szStartLocal ) );
			ZeroMemory( szStartURL, sizeof( szStartURL ) );
			ZeroMemory( szStartReg, sizeof( szStartReg ) );

#ifdef SRC_KOR
			strcpy_s(szStartLocal, sizeof(szStartLocal), "KOREA");
#else
			LoadStartINI( szStartLocal, sizeof( szStartLocal ), szStartURL, sizeof( szStartURL ), szStartReg, sizeof( szStartReg ) );
#endif


#if defined LG_UPLUS
			m_pLocalMgr->SetLocalType( szStartLocal );
#elif defined SRC_KOR
			m_pLocalMgr->SetLocalType( szStartLocal );
#else

			if( strcmp( szStartLocal , "" ) == 0 || strcmp( szStartURL , "" ) == 0 || strcmp( szStartReg, "" ) == 0 )
			{
				MessageBox( "Failed to read [start.ini]", "LostSaga_Webbroker - Error", MB_TOPMOST );
			}
			else
			{
				m_pLocalMgr->SetLocalType( szStartLocal );
			}
#endif
		}
	}

	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );

#ifdef SRC_OVERSEAS

	DWORD size = 0;
	const char * buffer = NULL;
	Help::LoadTextInResource(IDR_ERROR, size, buffer);


	m_pStringMgr = new ioStringManager;
	if( m_pStringMgr)
		m_pStringMgr->LoadData( "", "", buffer, true );



#else

	m_pStringMgr = new ioStringManager;
	if( m_pStringMgr && pLocal )
		m_pStringMgr->LoadData( "", "", pLocal->GetMemTextList(), true );


#endif
	if(pLocal)	
	{
		if(pLocal->GetType() == ioLocalManager::LCT_LATIN)
		{
			if(strcmp(AfxGetApp()->m_lpCmdLine, "") == 0)
			{
				MessageBox( "Please, Execute [NyxLauncher.exe]", "LostSaga_Webbroker - Error", MB_TOPMOST );
				DestroyWindow();
				return 0;
			}
		}

		else if(pLocal->GetType() == ioLocalManager::LCT_THAILAND )
		{
			if(strcmp(AfxGetApp()->m_lpCmdLine, "") == 0)
			{
				MessageBox( "Please, Execute [Garena Plus]", "LostSaga_Webbroker - Error", MB_TOPMOST );
				DestroyWindow();
				return 0;
			}
		}

		else if(pLocal->GetType() == ioLocalManager::LCT_TAIWAN )
		{
			if(strcmp(AfxGetApp()->m_lpCmdLine, "") == 0)
			{
				MessageBox( "Please, Execute [Garena Plus]", "LostSaga_Webbroker - Error", MB_TOPMOST );
				DestroyWindow();
				return 0;
			}
		}


		pLocal->SetRegKey( szStartReg );	
		pLocal->SetStartURL( szStartURL );
	}

	if( eDulicationType != DT_NONE )
	{
		if( eDulicationType == DT_CLIENT )
			Help::MessageboxShow( STR(1), "LostSaga_Webbroker - Error", MB_TOPMOST );

		CWnd *pWnd = FindWindow( AUTOUPGRADENAME, NULL );
		if ( pWnd )
			pWnd->SetForegroundWindow();			

		DestroyWindow();
		return 0;
	}
		
	m_ErrorDlg.Create(IDD_IOERRORDLG);
	m_SetupFolderDlg.Create( IDD_SETUP_FOLDER_DLG );
	m_LoginDlg.Create( IDD_LOGINDLG );
	m_LocalDlg.Create( IDD_LOCAL_DLG );



#ifdef SRC_KOR
	if (!m_TrayIcon.Create(this, WM_ICON_NOTIFY, _T("LSWebBroker"), NULL, IDI_REDICON)) return -1;
	m_TrayIcon.SetIcon(IDI_REDICON);
	m_TrayIcon.SetTooltipText(STR(2));
#else
	if (!m_TrayIcon.Create(this, WM_ICON_NOTIFY, _T("LSWebBroker"), NULL, IDI_SHIP)) return -1;
	m_TrayIcon.SetIcon(IDI_SHIP);
	m_TrayIcon.SetTooltipText(STR(2));
#endif

#ifdef SRC_KOR
#else
	if( strcmp( AfxGetApp()->m_lpCmdLine, "" ) == 0 )
#endif
	{
		if( pLocal && pLocal->IsLoginDlg() )
		{
			m_pHttpMgr = new HttpManager;
			pLocal->InitWinSock();
			m_LoginDlg.LoadINI();
#if defined(SRC_KOR)
			m_LoginDlg.ShowWindow(SW_SHOW);
#elif defined(LG_UPLUS)
			m_LoginDlg.OnBnClickedLogin();
#else
			
#endif
			return 0;
		}
	}

	if( (ioLocalManager::GetLocalType() == ioLocalManager::LCT_EU) || 
		(ioLocalManager::GetLocalType() == ioLocalManager::LCT_SINGAPORE) )
	{
		m_LocalDlg.LoadINI();
		return 0;
	}

	ParseCmdAndStart( pLocal );
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::SetWantExit()
{
	SetTimer(ID_EXIT_TIMER, 1000, NULL);
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(ID_EXIT_TIMER == nIDEvent)
	{
		if(!m_ErrorDlg.IsWindowVisible() && !m_SetupFolderDlg.IsWindowVisible() )
		{
			KillTimer(ID_EXIT_TIMER);
			PostQuitMessage(0);
			// 소멸자가 호출이 안되어 아래에서 다시 처리함.
			SAFEDELETE( m_pStringMgr );
			SAFEDELETE( m_pLocalMgr );
		}
	}

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::ShowErrorDlg( const char* szErrorText, int iErrorNumber )
{
	CString szTemp = szErrorText;
#ifdef STEAM_ATTACH
	szTemp += "\r\n\r\nERROR";
#else
	szTemp += "\r\n\r\n";
	szTemp += AfxGetApp()->m_lpCmdLine;
#endif
	m_ErrorDlg.ShowErrorDlg(szTemp, iErrorNumber);
}

void CMainFrame::SetupAndStart( CString &rszClientPath )
{
	if(rszClientPath.IsEmpty())
	{
		char szRegKey[MAX_PATH*2]="";
		ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
		if( pLocal )
			StringCbPrintf( szRegKey , sizeof( szRegKey ), "%sWebBroker", pLocal->GetRegKey() );

		char szTempMsg[MAX_PATH*4]="";
		SafeSprintf( szTempMsg, sizeof( szTempMsg ), STR(1), GetLastError());
		ShowErrorDlg( szTempMsg, 105 );
		Help::RegWriteInt( REG_SHCU, szRegKey, "Error", 105);
		return;
	}

	m_SetupAndStart.SetClientPath(rszClientPath);
	if( m_SetupAndStart.SetupAndStartGame(true) )
		SetWantExit();
}

void CMainFrame::SetLoginKey(LPCTSTR szKey)
{
	m_SetupAndStart.SetLoginKey(szKey);
}
void CMainFrame::SetLocalType(LPCTSTR szLocal)
{
	m_SetupAndStart.SetLocalType(szLocal);
}


static size_t WriteCallback(void* buffer, size_t size, size_t nmemb, void* param)
{
	std::string& text = *static_cast<std::string*>(param);
	size_t totalsize = size * nmemb;
	text.append(static_cast<char*>(buffer), totalsize);
	return totalsize;
}


void CMainFrame::DeleteClientDirectorys(const char* szFolder)
{
	if (szFolder == NULL)
		return;

	char rootDir[MAX_PATH] = "";
	::GetCurrentDirectory(MAX_PATH, rootDir);

	//디렉토리 삭제.
	char delName[MAX_PATH] = "";
	StringCbPrintf(delName, sizeof(delName), "%s\\%s", rootDir, szFolder);
	SHFILEOPSTRUCT FileOp = { 0 };
	FileOp.hwnd = NULL;
	FileOp.wFunc = FO_DELETE;
	FileOp.pFrom = delName;
	FileOp.pTo = NULL;
	FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;
	FileOp.fAnyOperationsAborted = false;
	FileOp.hNameMappings = NULL;
	FileOp.lpszProgressTitle = NULL;
	::SHFileOperation(&FileOp);
}


bool CMainFrame::SendLoginData(OUT char* szError, IN int iErrorSize, OUT int iErrorCode, const char* szID, const char* szPW, IN const char* szURL)
{
	bool bResult = true;

	std::string result;
	long httpCode(0);

	CURL* curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, szURL);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		std::string prefixKey = "LSFD";
		std::string apikey = ""; // apikey
		std::string bearerToken = "Authorization: " + prefixKey + " " + apikey;

		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, bearerToken.c_str());
		headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		char postData[MAX_PATH] = { 0 };
		ZeroMemory(postData, sizeof(postData));
		char szLocalIP[MAX_PATH] = "";
		ZeroMemory(szLocalIP, sizeof(szLocalIP));
		Help::GetMyIP(szLocalIP, sizeof(szLocalIP));
		char szExternalIP[MAX_PATH] = "";
		ZeroMemory(szExternalIP, sizeof(szExternalIP));
		Help::GetExternalIP(szExternalIP, sizeof(szExternalIP));

		StringCbPrintf(postData, sizeof(postData), "username=%s&password=%s&privateIP=%s&publicIP=%s&HWID=%s", szID, szPW, szLocalIP, szExternalIP, "HOWTOGETIT?");

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


		res = curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);

		if (CURLE_OK != res) {
			SafeSprintf(szError, sizeof(char) * MAX_PATH, "Internal Server Error. Please contact admin!");
			iErrorCode = 500; // Internal Server Error
			bResult = false;
		}
		else if (httpCode == 200) {

			cJSON* json_object = cJSON_Parse(result.c_str());

			cJSON* AppName = cJSON_GetObjectItem(json_object, "AppName");
			cJSON* name = cJSON_GetObjectItem(json_object, "userID");
			cJSON* encodeKey = cJSON_GetObjectItem(json_object, "encodeKey");
			cJSON* serverID = cJSON_GetObjectItem(json_object, "serverID");

			char encode[256];
			char encid[256];
			char encodeip[256];
			char encip[256];
			char loginKey[MAX_PATH] = "";

			ioEncrypted::Encode15("111111111111111", "111111111111111", encode);
			ioEncrypted::Encode15(name->valuestring, encodeKey->valuestring, encid);
			ioEncrypted::Encode15(szExternalIP, szLocalIP, encodeip);
			ioEncrypted::Encode15(szLocalIP, encodeKey->valuestring, encip);


			char rootDir[MAX_PATH] = "";
			::GetCurrentDirectory(MAX_PATH, rootDir);
			char szClientName[MAX_PATH] = "";

			StringCbPrintf(szClientName, sizeof(szClientName), "%s\\%s", rootDir, AppName->valuestring);
			StringCbPrintf(loginKey, sizeof(loginKey), "EDEW3940FVDP4950,10,20,30,1,autoupgrade_info.ini,1000,0,1,0,?%s%s?0?%s%s?%s?2010,7,15,1?10201?", encode, encid, encodeip, encip, serverID->valuestring);


			Help::CreateProcessWithInfo((LPCTSTR)szClientName, loginKey);
			
			cJSON_Delete(json_object);
			memset(encip, 0, sizeof(encip));
			memset(encodeip, 0, sizeof(encodeip));
			memset(encid, 0, sizeof(encid));
			memset(encode, 0, sizeof(encode));
			memset(loginKey, 0, sizeof(loginKey));
			memset(szClientName, 0, sizeof(szClientName));
			memset(rootDir, 0, sizeof(rootDir));
			


			//Deleting an folder
			DeleteClientDirectorys("losadown");

			DestroyWindow();
			exit(0);
		}
		else {

			cJSON* json_object = cJSON_Parse(result.c_str());
			cJSON* message = cJSON_GetObjectItem(json_object, "error");

			SafeSprintf(szError, sizeof(char) * MAX_PATH, message->valuestring);
			iErrorCode = httpCode;
			bResult = false;
		}

		curl_global_cleanup();
	}

	return bResult;
}




BOOL CMainFrame::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_h_mutex != NULL)
		ReleaseMutex(m_h_mutex);

	return CFrameWnd::DestroyWindow();
}

void CMainFrame::LoadStartINI( OUT char *szLocal, IN int iLocalSize, OUT char *szURL, IN int iURLSize, OUT char *szReg, IN int iRegSize )
{
	ioINILoaderAU kLoader( "info\\start.ini" );
	kLoader.SetTitle( "Info" );
	kLoader.LoadString( "Local", "", szLocal, iLocalSize );
	kLoader.LoadString( "URL", "", szURL, iURLSize );
#ifdef SRC_OVERSEAS
	//해외일 경우 캐시 초기화 하자
	ULONG uRand = 0;
	srand((unsigned)time(NULL));
	uRand=( ( (long) rand()<<15) | rand() ) % 1000000;
	strcat( szURL, "?") ;
	char szTemp[MAX_PATH] = "";
	itoa(uRand , szTemp, 10);
	strcat( szURL, szTemp);
#endif
	kLoader.LoadString( "Reg", "", szReg, iRegSize );
}

void CMainFrame::ParseCmdAndStart( ioLocalParent *pLocal, const char* szRet )
{
	if( !pLocal )
		return;

	char szRegKey[MAX_PATH*2]="";
	StringCbPrintf( szRegKey , sizeof( szRegKey ), "%sWebBroker", pLocal->GetRegKey() );

	char szNewCmd[4096]="";
	ZeroMemory( szNewCmd, sizeof( szNewCmd ) );
	if( !pLocal->GetNewCmdLine( AfxGetApp()->m_lpCmdLine, szNewCmd, sizeof( szNewCmd ) ) )
	{
		char szTempMsg[4096]="";
#ifdef STEAM_ATTACH
		SafeSprintf( szTempMsg, sizeof( szTempMsg ), STR(2), "", GetLastError() ); //인자값 노출 제거
#else
		SafeSprintf( szTempMsg, sizeof( szTempMsg ), STR(2), szNewCmd, GetLastError() );
#endif
		
		ShowErrorDlg( szTempMsg,106 );
		Help::RegWriteInt( REG_SHCU, szRegKey, "Error", 106);
		return;
	}
#ifdef SRC_KOR
	if( !szRet || !m_SetupAndStart.ParseCmd( szRet ))
	{
		char szTempMsg[MAX_PATH*4]="";
		SafeSprintf( szTempMsg, sizeof( szTempMsg ), STR(1), GetLastError() );
		ShowErrorDlg( szTempMsg,104 );
		Help::RegWriteInt( REG_SHCU, szRegKey, "Error", 104);
		return;
	}
#else
	if(!m_SetupAndStart.ParseCmd( szNewCmd ))
	{
		char szTempMsg[MAX_PATH*4]="";
		SafeSprintf( szTempMsg, sizeof( szTempMsg ), STR(1), GetLastError() );
		ShowErrorDlg( szTempMsg,104 );
		Help::RegWriteInt( REG_SHCU, szRegKey, "Error", 104);
		return;
	}
#endif

	if( m_SetupAndStart.IsReadRegClientPath() )
	{
		m_SetupAndStart.ReadClientPath( szRegKey );
		if( m_SetupAndStart.IsEmptyClientPath() )
			m_SetupFolderDlg.ShowWindow(SW_SHOW);
		else
		{
			if( m_SetupAndStart.SetupAndStartGame(true) )
				DestroyWindow();
		}
	}
	else
	{
		if( m_SetupAndStart.SetupAndStartGame(false) )
			DestroyWindow();
	}
}

void CMainFrame::ShowSetupFolderDlg()
{
	m_SetupFolderDlg.ShowWindow(SW_SHOW);
}
