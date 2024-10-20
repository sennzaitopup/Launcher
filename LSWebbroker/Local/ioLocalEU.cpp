#include "../stdafx.h"
#include ".\ioLocalEU.h"
#include "../Util/httpapp.h"
#include "../resource.h"
#include <strsafe.h>
#include "../StringManager/ioHashString.h"
#include "../SetupAndStart.h"

ioLocalEU::ioLocalEU(void)
{
	ZeroMemory( m_szRegKey, sizeof( m_szRegKey ) );
	ZeroMemory( m_szStartURL, sizeof( m_szStartURL ) );
}

ioLocalEU::~ioLocalEU(void)
{
}

ioLocalManager::LocalType ioLocalEU::GetType()
{
	return ioLocalManager::LCT_EU;
}

const char * ioLocalEU::GetTextListFileName()
{
	return "text_eu.txt";
}

const char * ioLocalEU::GetMemTextList()
{
	return "|EXE_CMainFrame::OnCreate_1|\r\n\r\nLost Saga is launching.\r\n\r\n     Please wait a moment.\r\n\r\n\r\n\r\n  Reboot and try again if \r\n\r\n   the game fails to launch.|"
		"|EXE_CMainFrame::OnCreate_2|Lost Saga is updating.  Please wait a moment. |"
		"|EXE_CMainFrame::ParseCmdAndStart_1|The parameter is incorrect.\r\nPlease attempt to try again later.(%1)\r\n(Error Code:104)|"
		"|EXE_CMainFrame::ParseCmdAndStart_2|First Data Value is incorrect.\r\nPlease attempt to try again later.(%1/%2)\r\n(Error Code:106)|"
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

bool ioLocalEU::GetNewCmdLine( IN const char *szCmd, OUT char *szNewCmd, IN int iNewCmdSize )
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

	enum 
	{ 
		MAX_CMD_LOOP = 3,
		MAX_CHANGE_CMD_LOOP = 6,

		TYPE_CHANGE_CMD_ID  = 3,
		TYPE_CHANGE_CMD_KEY = 5,
	};

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

	// new cmd 제작
	char szAfterServerIDValue[MAX_PATH*2]="";
	ZeroMemory( szAfterServerIDValue, sizeof( szAfterServerIDValue ) );

	char szGameServerIDData[MAX_PATH*4]="";
	ZeroMemory( szGameServerIDData, sizeof( szGameServerIDData ) );

	enum 
	{ 
		OGP_TYPE_EXPAND_KEY         = 9,  // key value가 생략되어 있어 9번째임
		OGP_TYPE_GAMESERVER_ID_LIST = 11, // key value가 생략되어 있어 11번째임, 앞으로는 통일성을 위해서 한국과 동일하게 처리 필요.
	};

	// main token
	for (int i = 0; i < CSetupAndStart::OPTION_TYPE_MAX ; i++)
	{
		char *pPos = NULL;
		if( i == 0 )
			pPos = strtok( szStartData, START_TOKEN );
		else
			pPos = strtok( NULL, START_TOKEN );

		if( pPos == NULL )
			break;

		if( i == OGP_TYPE_EXPAND_KEY )
		{
			StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
			StringCbCat( szNewCmd, iNewCmdSize, pPos );
			StringCbCat( szNewCmd, iNewCmdSize, START_EXPAND_TOKEN );
			StringCbCat( szNewCmd, iNewCmdSize, szOGPKey );
			StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
			StringCbCat( szNewCmd, iNewCmdSize, szUserID );
		}
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

bool ioLocalEU::IsRightGameServerID( const CString &rszGameServerID )
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

const char * ioLocalEU::GetRegKey()
{
	return m_szRegKey;
}

DWORD ioLocalEU::GetResourceIDBack()
{
	return IDB_BACK_US;
}

DWORD ioLocalEU::GetResourceIDChange()
{
	return IDB_CHANGEBTN_TH;
}

DWORD ioLocalEU::GetResourceIDSetup()
{
	return IDB_SETUPBTN_TH;
}

DWORD ioLocalEU::GetResourceIDErrorBack()
{
	return IDB_ERROR_BACK;
}

DWORD ioLocalEU::GetResourceIDErrorExit()
{
	return IDB_EXITBTN_TH;
}

DWORD ioLocalEU::GetResourceIDErrorSolution()
{
	return -1;
}

const char * ioLocalEU::GetErrorSolutionURL()
{
	return "http://lostsaga.ogplanet.com";
}

void ioLocalEU::SetRegKey( const char *szReg )
{
	StringCbCopy( m_szRegKey, sizeof( m_szRegKey ), szReg );
}

void ioLocalEU::SetStartURL( const char *szURL )
{
	StringCbCopy( m_szStartURL, sizeof( m_szStartURL ), szURL );
}

bool ioLocalEU::IsLoginDlg()
{
	return false;
}

bool ioLocalEU::GetLoginResultPostData( OUT char *szError, IN int iErrorSize, const char *szURL, const char *szID, const char *szPW )
{
	// 빈값
	return false;
}

const char * ioLocalEU::GetFontName()
{
	return "Tahoma";
}

