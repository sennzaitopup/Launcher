#include "../stdafx.h"
#include ".\ioLocalChina.h"
#include "../resource.h"
#include <strsafe.h>
#include "../Util/HttpApp.h"
#include "../StringManager/ioHashString.h"
#include <mmsystem.h>
#include "../SetupAndStart.h"
#include "ioLocalChinaLanguage.h"

ioLocalChina::ioLocalChina(void)
{
	ZeroMemory( m_szRegKey, sizeof( m_szRegKey ) );
	ZeroMemory( m_szStartURL, sizeof( m_szStartURL ) );
}

ioLocalChina::~ioLocalChina(void)
{
}

ioLocalManager::LocalType ioLocalChina::GetType()
{
	return ioLocalManager::LCT_CHINA;
}

const char * ioLocalChina::GetTextListFileName()
{
	return "text.txt";
}

const char * ioLocalChina::GetMemTextList()
{
	return  ChinaLanguage::GetMemTextList();
}

bool ioLocalChina::GetNewCmdLine( IN const char *szCmd, OUT char *szNewCmd, IN int iNewCmdSize )
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

	char szAfterServerIDValue[MAX_PATH*2]="";
	ZeroMemory( szAfterServerIDValue, sizeof( szAfterServerIDValue ) );

	char szGameServerIDData[MAX_PATH*4]="";
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

	char szServerID[MAX_PATH];
	ZeroMemory( szServerID, sizeof( szServerID ) );
	ParseServerIDList( szGameServerIDData, szServerID, sizeof( szServerID ) );
	
	StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
	StringCbCat( szNewCmd, iNewCmdSize, szServerID );

	// save data add
	StringCbCat( szNewCmd, iNewCmdSize, szAfterServerIDValue );

	return true;
}

bool ioLocalChina::IsRightGameServerID( const CString &rszGameServerID )
{
	return true;
}

const char * ioLocalChina::GetRegKey()
{
	return m_szRegKey;
}

DWORD ioLocalChina::GetResourceIDBack()
{
	return IDB_BACK_ID;
}

DWORD ioLocalChina::GetResourceIDChange()
{
	return IDB_CHANGEBTN_ID;
}

DWORD ioLocalChina::GetResourceIDSetup()
{
	return IDB_SETUPBTN_ID;
}

DWORD ioLocalChina::GetResourceIDErrorBack()
{
	return IDB_ERROR_BACK_ID;
}

DWORD ioLocalChina::GetResourceIDErrorExit()
{
	return IDB_EXITBTN_ID;
}

DWORD ioLocalChina::GetResourceIDErrorSolution()
{
	return IDB_SOLUTIONBTN_ID;
}

const char * ioLocalChina::GetErrorSolutionURL()
{
	return "http://forum.gemscool.com/forum-143.html";
}

void ioLocalChina::SetRegKey( const char *szReg )
{
	StringCbCopy( m_szRegKey, sizeof( m_szRegKey ), szReg );
}

void ioLocalChina::SetStartURL( const char *szURL )
{
	StringCbCopy( m_szStartURL, sizeof( m_szStartURL ), szURL );
}

bool ioLocalChina::IsLoginDlg()
{
	return false;
}


bool ioLocalChina::GetLoginResultPostData( OUT char *szError, IN int iErrorSize, const char *szURL, const char *szID, const char *szPW )
{
	// 빈값
	return false;
}

bool ioLocalChina::IsReadRegClientPath()
{
	return false;
}

const char * ioLocalChina::GetFontName()
{
	return FONT_NAME_CH;
}

