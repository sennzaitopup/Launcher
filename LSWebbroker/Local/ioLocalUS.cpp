#include "../stdafx.h"
#include ".\iolocalus.h"
#include "../Util/httpapp.h"
#include "../resource.h"
#include <strsafe.h>
#include "../StringManager/ioHashString.h"
#include "../SetupAndStart.h"

ioLocalUS::ioLocalUS(void)
{
	ZeroMemory( m_szRegKey, sizeof( m_szRegKey ) );
	ZeroMemory( m_szStartURL, sizeof( m_szStartURL ) );
}

ioLocalUS::~ioLocalUS(void)
{
}

ioLocalManager::LocalType ioLocalUS::GetType()
{
	return ioLocalManager::LCT_US;
}

const char * ioLocalUS::GetTextListFileName()
{
	return "text_us.txt";
}

const char * ioLocalUS::GetMemTextList()
{
	return "|EXE_CMainFrame::OnCreate_1|\r\n\r\nLost Saga is launching.\r\n\r\n     Please wait a moment.\r\n\r\n\r\n\r\n  Reboot and try again if \r\n\r\n   the game fails to launch.|"
		"|EXE_CMainFrame::OnCreate_2|Lost Saga is updating.  Please wait a moment. |"
		"|EXE_CMainFrame::ParseCmdAndStart_1|The parameter is incorrect.\r\nPlease attempt to try again later.(%1)\r\n(Error Code:104)|"
		"|EXE_CMainFrame::ParseCmdAndStart_2|First Data Value is incorrect.\r\nPlease attempt to try again later.(%1:%2)\r\n(Error Code:106)|"
		"|EXE_CMainFrame::SetupAndStart_1|Folder Name does not match.\r\nPlease attempt to try again later.(%1)\r\n(Error Code:105)|"
		"|EXE_CSetupAndStart::SetupAndStartGame_1|The parameter has changed.\r\nPlease attempt to try again later.(%1)\r\n(Error Code:101)|"
		"|EXE_CSetupAndStart::SetupAndStartGame_2|Failed to install Auto Update.\r\nPlease attempt to try again later.(%1:%2)\r\n(Error Code:102)|"
		"|EXE_CSetupAndStart::SetupAndStartGame_3|Failed to install Patch Info File.\r\nPlease attempt to try again later.(%1:%2)\r\n(Error Code:103)|"
		"|EXE_CSetupAndStart::SetupAndStartGame_4|No server available or the servers are currently busy.\r\nPlease attempt to try again later.(%1)\r\n(Error Code:107)|"
		"|EXE_ioErrorDlg::ShowErrorDlg_2|--- System Requirement ---\r\n%1\r\n\r\n--- Error ---\r\n%2|"
		"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_1|Please select the folder for Lost Saga Installation.|"
		"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_2|Installation Path is empty.\r\n\r\n     Please select again.|"
		"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_3|Installation Path cannot exceed 100 letter and numbers combined.\r\n\r\n              Please select again.|"
		"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_4|Cannot make a folder on this path.\r\n\r\n       Please select again.|"
		"|EXE_SetupFolderDlg::OnBnClickedSetup_1|Preparing to install Lost Saga. Please wait a moment.|";
}

bool ioLocalUS::GetNewCmdLine( IN const char *szCmd, OUT char *szNewCmd, IN int iNewCmdSize )
{
	// txt 파일에서 start 정보를 읽어옴.
	char szStartData[4096]; // asp 파일의 결과 같은 4096바이트를 넘지 않는다.
	if( !ReadURLData( (LPCTSTR) m_szStartURL, szNewCmd, iNewCmdSize, szStartData, sizeof( szStartData ) ) )
		return false;

	// parsing
	if( strcmp( szStartData, "" ) == 0 )
	{
		StringCbCopy( szNewCmd, iNewCmdSize, "StartData is empty." );
		return false;
	}

	// cmd에서 정보 parsing
// 	if( strcmp( szCmd , "" ) == 0 )
// 	{
// 		StringCbCopy( szNewCmd, iNewCmdSize, "Cmd is empty." );
// 		return false;
// 	}

	char szCmdData[MAX_PATH*2]="";
	ZeroMemory( szCmdData, sizeof( szCmdData ) );
	StringCbCopy( szCmdData, sizeof( szCmdData ), szCmd );

	char szChangeCmdData[MAX_PATH*2]="";
	ZeroMemory( szChangeCmdData, sizeof( szChangeCmdData ) );

	char szUserID[MAX_PATH]="";
	ZeroMemory( szUserID, sizeof( szUserID ) );
	char szOGPKey[MAX_PATH]="";
	ZeroMemory( szOGPKey, sizeof( szOGPKey ) );

#ifdef STEAM_ATTACH
	char szSTEAMID[MAX_PATH]="";
	ZeroMemory( szSTEAMID, sizeof( szSTEAMID ) );
#endif
	enum 
	{ 
		MAX_CMD_LOOP = 3,
		MAX_CHANGE_CMD_LOOP = 6,

		TYPE_CHANGE_CMD_ID  = 3,
		TYPE_CHANGE_CMD_KEY = 5,

#ifdef STEAM_ATTACH
		TYPE_CHANGE_CMD_SGW_ID  = 0,
		TYPE_CHANGE_CMD_SGW_AUTH = 1,
		TYPE_CHANGE_CMD_STEAM_ID = 2,
#endif

	};

#ifdef STEAM_ATTACH

	// 북미용 authcode sgptest01:20677986|MTAwc2dwdGVzdDAxMjAxNDkyMzQzODUxNDgz:0
	// sgptest01:20677986|MTAwc2dwdGVzdDAxMjAxNDkyMzQzODUxNDgz:0   ":"분리
	for (int i = 0; i < MAX_CHANGE_CMD_LOOP ; i++)
	{
		char *pPos = NULL;
		if( i == 0 )
			pPos = strtok( szCmdData, SGW_VALUE_TOKEN);
		else
			pPos = strtok( NULL, SGW_VALUE_TOKEN);

		if( pPos == NULL )
			break;

		if( i == TYPE_CHANGE_CMD_SGW_ID )
			StringCbCopy( szUserID, sizeof( szUserID ), pPos );
		else if( i == TYPE_CHANGE_CMD_SGW_AUTH )
			StringCbCopy( szOGPKey, sizeof( szOGPKey ), pPos );
		else if( i == TYPE_CHANGE_CMD_STEAM_ID )
			StringCbCopy( szSTEAMID, sizeof( szSTEAMID ), pPos );

	}

	// 커멘드 인자가 비었다.
	bool isCmdEmpty = false;
	if(strlen(szCmdData) == 0)
		isCmdEmpty = true;

#else

	// serviceid=17 userid=derks2007 token=08179a0624514991b36d73553dfbb8e8 : " " 분리
	for (int i = 0; i < MAX_CMD_LOOP ; i++)
	{
		char *pPos = NULL;
		if( i == 0 )
			pPos = strtok( szCmdData, OGP_TOKEN );
		else
			pPos = strtok( NULL, OGP_TOKEN );

		if( pPos == NULL )
			break;

		if( i != 0 )
			StringCbCat( szChangeCmdData, sizeof( szChangeCmdData ), OGP_VALUE_TOKEN );
		StringCbCat( szChangeCmdData, sizeof( szChangeCmdData ), pPos );
	}

	// serviceid=17=userid=derks2007=token=08179a0624514991b36d73553dfbb8e8 : = 분리
	for (int i = 0; i < MAX_CHANGE_CMD_LOOP ; i++)
	{
		char *pPos = NULL;
		if( i == 0 )
			pPos = strtok( szChangeCmdData, OGP_VALUE_TOKEN );
		else
			pPos = strtok( NULL, OGP_VALUE_TOKEN );

		if( pPos == NULL )
			break;

		if( i == TYPE_CHANGE_CMD_ID )
			StringCbCopy( szUserID, sizeof( szUserID ), pPos );
		else if( i == TYPE_CHANGE_CMD_KEY )
			StringCbCopy( szOGPKey, sizeof( szOGPKey ), pPos );
	}

#endif

	// new cmd 제작
	char szAfterServerIDValue[MAX_PATH*2]="";
	ZeroMemory( szAfterServerIDValue, sizeof( szAfterServerIDValue ) );

	char szGameServerIDData[MAX_PATH*4]="";
	ZeroMemory( szGameServerIDData, sizeof( szGameServerIDData ) );

	enum 
	{ 
		OGP_TYPE_EXPAND_KEY         = 9,  // key value가 생략되어 있어 9번째임
		OGP_TYPE_GAMESERVER_ID_LIST = 11, // key value가 생략되어 있어 11번째임, 앞으로는 통일성을 위해서 한국과 동일하게 처리 필요.
		OGP_TYPE_PASS				= 10,
	};

	// main token
	for (int i = 0; i < CSetupAndStart::OPTION_TYPE_MAX ; i++)
	{
		char *pPos = NULL;
		if( i == 0 )
			pPos = strtok( szStartData, START_TOKEN);
		else
			pPos = strtok( NULL, START_TOKEN);

		if( pPos == NULL )
			break;

		if( i == OGP_TYPE_EXPAND_KEY )
		{
			StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
			StringCbCat( szNewCmd, iNewCmdSize, pPos );
			StringCbCat( szNewCmd, iNewCmdSize, START_EXPAND_TOKEN );
			StringCbCat( szNewCmd, iNewCmdSize, szOGPKey );
#ifdef STEAM_ATTACH
			if(!isCmdEmpty)
			{
				StringCbCat( szNewCmd, iNewCmdSize, START_EXPAND_TOKEN );
				StringCbCat( szNewCmd, iNewCmdSize, szSTEAMID );
				StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
				StringCbCat( szNewCmd, iNewCmdSize, szUserID );
			}
			else
			{
				StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
				StringCbCat( szNewCmd, iNewCmdSize, szUserID );
			}
#else
			StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
			StringCbCat( szNewCmd, iNewCmdSize, szUserID );
#endif
		}
#ifdef STEAM_ATTACH
		else if(i == OGP_TYPE_PASS)
		{
			if(!isCmdEmpty)
			{
				continue;
			}
			else
			{
				if( i != 0 )
					StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
				StringCbCat( szNewCmd, iNewCmdSize, pPos );
			}
		}
#endif
		else if( i == OGP_TYPE_GAMESERVER_ID_LIST )
		{
			StringCbCopy( szGameServerIDData, sizeof( szGameServerIDData ), pPos );			
		}
		else if( i > OGP_TYPE_GAMESERVER_ID_LIST )
		{
			StringCbCat( szAfterServerIDValue, sizeof( szAfterServerIDValue ), START_TOKEN );
			StringCbCat( szAfterServerIDValue, sizeof( szAfterServerIDValue ), pPos );
		}
		else 
		{
			if( i != 0 )
				StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
			StringCbCat( szNewCmd, iNewCmdSize, pPos );
		}
	}

	char szServerID[MAX_PATH];
	ZeroMemory( szServerID, sizeof( szServerID ) );
	ParseServerIDList( szGameServerIDData, szServerID, sizeof( szServerID ) );
	
	StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
	StringCbCat( szNewCmd, iNewCmdSize, szServerID );

	// save data add
	StringCbCat( szNewCmd, iNewCmdSize, szAfterServerIDValue );

	return true;
}

bool ioLocalUS::IsRightGameServerID( const CString &rszGameServerID )
{
	if( rszGameServerID == "0" )
	{
		return false;
	}

	if( rszGameServerID == "1" )
	{
		return false;
	}

	return true;
}

const char * ioLocalUS::GetRegKey()
{
	return m_szRegKey;
}

DWORD ioLocalUS::GetResourceIDBack()
{
	return IDB_BACK;
}

DWORD ioLocalUS::GetResourceIDChange()
{
	return IDB_CHANGEBTN;
}

DWORD ioLocalUS::GetResourceIDSetup()
{
	return IDB_SETUPBTN;
}

DWORD ioLocalUS::GetResourceIDErrorBack()
{
	return IDB_ERROR_BACK;
}

DWORD ioLocalUS::GetResourceIDErrorExit()
{
	return IDB_EXITBTN;
}

DWORD ioLocalUS::GetResourceIDErrorSolution()
{
	return -1;
}

const char * ioLocalUS::GetErrorSolutionURL()
{
	return "http://lostsaga.ogplanet.com";
}

void ioLocalUS::SetRegKey( const char *szReg )
{
	StringCbCopy( m_szRegKey, sizeof( m_szRegKey ), szReg );
}

void ioLocalUS::SetStartURL( const char *szURL )
{
	StringCbCopy( m_szStartURL, sizeof( m_szStartURL ), szURL );
}

bool ioLocalUS::IsLoginDlg()
{
	return false;
}

bool ioLocalUS::GetLoginResultPostData( OUT char *szError, IN int iErrorSize, const char *szURL, const char *szID, const char *szPW )
{
	// 빈값
	return false;
}

const char * ioLocalUS::GetFontName()
{
	return "Tahoma";
}

#ifdef STEAM_ATTACH

bool ioLocalUS::CheckFileInFolder( CString* const strPath, CString* const strFile )
{
	CFileFind pFind;
	CString strSearchPath;
	strSearchPath = *strPath;
	strSearchPath += '\\';
	strSearchPath += *strFile;
	return pFind.FindFile(strSearchPath);
}

bool ioLocalUS::UpdateNRunSteamLauncher( CSetupAndStart* SetUpAndStart )
{
	CString strSteamFileURL = "http://na-qa.launcher.z8games.com/lsna/LastVersion/";
	//CString strSteamFileURL = "http://cdn.lostsaga.z8games.com/Launcher/";
	CFileFind pFind;
	CString strFilePath;
	char szName[MAX_PATH]={0,};
	::GetCurrentDirectory(MAX_PATH,(LPSTR)szName); //현재 경로 얻기
	strFilePath = szName;
	strFilePath += "\\";
	enum {STEAMFILECNT = 5};
	CString strFileName[STEAMFILECNT] = { "LoginLauncherN.exe", "LoginLauncher.exe", "loginLauncherinfo.ini",	"LoginLauncherSkin_LS.dat",	"ResLoginLauncher_LS.dll"};
	DWORD dwFileSize;
	int iDownCnt = 0;
	for( int i = 0 ; i < STEAMFILECNT ; ++i )
	{
		if(!CheckFileInFolder(&strFilePath,&strFileName[i]))
		{
			if(!SetUpAndStart->DownLoadProcess(strSteamFileURL+strFileName[i],0, strFilePath + strFileName[i], dwFileSize))
			{
				++iDownCnt;
				if(iDownCnt > 2)
				{
					MessageBox(NULL, "Download Fail SteamLauncher", "DOWNLOAD ERROR", MB_OK);
					return false; //fail down
				}
				--i;
				continue;
			}
		}
		break; //하나만 받는다.
	}

	//다운로드 파일 검사!!
	CString strPath = (strFilePath+*strFileName);
	SetUpAndStart->ExcuteProcess(strPath, "");


}

#endif
