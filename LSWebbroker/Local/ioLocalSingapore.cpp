#include "../stdafx.h"
#include <mmsystem.h>
#include <strsafe.h>
#include ".\ioLocalSingapore.h"
#include "../resource.h"
#include "../Util/HttpApp.h"
#include "../StringManager/ioHashString.h"
#include "../SetupAndStart.h"


ioLocalSingapore::ioLocalSingapore(void)
{
	ZeroMemory( m_szRegKey, sizeof( m_szRegKey ) );
	ZeroMemory( m_szStartURL, sizeof( m_szStartURL ) );
}

ioLocalSingapore::~ioLocalSingapore(void)
{
}

ioLocalManager::LocalType ioLocalSingapore::GetType()
{
	return ioLocalManager::LCT_SINGAPORE;
}

const char * ioLocalSingapore::GetTextListFileName()
{
	return "text_sg.txt";
}

const char * ioLocalSingapore::GetMemTextList()
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

bool ioLocalSingapore::GetNewCmdLine( IN const char *szCmd, OUT char *szNewCmd, IN int iNewCmdSize )
{
	enum 
	{
		MAX_BUFF = 4096,
	};
	// txt 파일에서 start 정보를 읽어옴.
	char szStartData[MAX_BUFF] = {0,}; // asp 파일의 결과 같은 4096바이트를 넘지 않는다.

	int i = 0;

	while(1)
	{
		i++;

		if( ReadURLData( (LPCTSTR) m_szStartURL, szNewCmd, iNewCmdSize, szStartData, sizeof( szStartData ) ) )
		{
			break;
		}
		else
		{
			if( i < 3)
			{
				continue;
			}
			else
			{
				int nRet = MessageBox(NULL, "Currently the server is full. \nCan you try to log in again?", 
					"Lost Saga", MB_RETRYCANCEL | MB_ICONASTERISK | MB_SETFOREGROUND);

				if(nRet == IDRETRY)
					continue;
				else
					return false;
			}
		}
	}

	// 	if( !ReadURLData( (LPCTSTR) szRandomURL, szNewCmd, iNewCmdSize, szStartData, sizeof( szStartData ) ) )
	// 		return false;

	// parsing
	if( strcmp( szStartData, "" ) == 0 )
	{
		StringCbCopy( szNewCmd, iNewCmdSize, "StartData is empty." );
		return false;
	}

	char szAfterServerIDValue[MAX_BUFF]="";
	ZeroMemory( szAfterServerIDValue, sizeof( szAfterServerIDValue ) );

	char szGameServerIDData[MAX_BUFF]="";
	ZeroMemory( szGameServerIDData, sizeof( szGameServerIDData ) );


	// parsing main token
	for (int i = 0; i < CSetupAndStart::OPTION_TYPE_MAX ; i++)
	{
		char *pPos = NULL;
		if( i == 0 )
			pPos = strtok( szStartData, START_TOKEN );
		else
			pPos = strtok( NULL, START_TOKEN );

		if( pPos == NULL )
			break;

		if( i == CSetupAndStart::OPTION_TYPE_GAMESERVER_ID )
		{
			StringCbCopy( szGameServerIDData, sizeof( szGameServerIDData ), pPos );			
		}
		else if( i > CSetupAndStart::OPTION_TYPE_GAMESERVER_ID )
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

	// 서버IP목록 중 1개의 IP만 추출하는 것을 주석처리함.
	//	char szServerID[MAX_PATH];
	//	ZeroMemory( szServerID, sizeof( szServerID ) );
	//	ParseServerIDList( szGameServerIDData, szServerID, sizeof( szServerID ) );

	StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
	StringCbCat( szNewCmd, iNewCmdSize, szGameServerIDData );

	// save data add
	StringCbCat( szNewCmd, iNewCmdSize, szAfterServerIDValue );

	return true;
}

bool ioLocalSingapore::IsRightGameServerID( const CString &rszGameServerID )
{
	return true;
}

const char * ioLocalSingapore::GetRegKey()
{
	return m_szRegKey;
}

DWORD ioLocalSingapore::GetResourceIDBack()
{
	return IDB_BACK_US;
}

DWORD ioLocalSingapore::GetResourceIDChange()
{
	return IDB_CHANGEBTN;
}

DWORD ioLocalSingapore::GetResourceIDSetup()
{
	return IDB_SETUPBTN;
}

DWORD ioLocalSingapore::GetResourceIDErrorBack()
{
	return IDB_ERROR_BACK;
}

DWORD ioLocalSingapore::GetResourceIDErrorExit()
{
	return IDB_EXITBTN;
}

DWORD ioLocalSingapore::GetResourceIDErrorSolution()
{
	return IDB_SOLUTIONBTN;
}

const char * ioLocalSingapore::GetErrorSolutionURL()
{
	return " ";
}

void ioLocalSingapore::SetRegKey( const char *szReg )
{
	StringCbCopy( m_szRegKey, sizeof( m_szRegKey ), szReg );
}

void ioLocalSingapore::SetStartURL( const char *szURL )
{
	StringCbCopy( m_szStartURL, sizeof( m_szStartURL ), szURL );
}

bool ioLocalSingapore::IsLoginDlg()
{
	return false;
}


bool ioLocalSingapore::GetLoginResultPostData( OUT char *szError, IN int iErrorSize, const char *szURL, const char *szID, const char *szPW )
{
	// 빈값
	return false;
}

bool ioLocalSingapore::IsReadRegClientPath()
{
	return false;
}

const char * ioLocalSingapore::GetFontName()
{
	return "Tahoma";
}

