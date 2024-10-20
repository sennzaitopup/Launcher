#include "../stdafx.h"
#include ".\ioLocalThailand.h"
#include "../resource.h"
#include <strsafe.h>
#include "../Util/HttpApp.h"
#include "../StringManager/ioHashString.h"
#include <mmsystem.h>
#include "../SetupAndStart.h"
#include "ioLocalThailandLanguage.h"

ioLocalThailand::ioLocalThailand(void)
{
	ZeroMemory( m_szRegKey, sizeof( m_szRegKey ) );
	ZeroMemory( m_szStartURL, sizeof( m_szStartURL ) );
}

ioLocalThailand::~ioLocalThailand(void)
{
}

ioLocalManager::LocalType ioLocalThailand::GetType()
{
	return ioLocalManager::LCT_THAILAND;
}

const char * ioLocalThailand::GetTextListFileName()
{
	return "text.txt";
}

const char * ioLocalThailand::GetMemTextList()
{
	return  ThailandLanguage::GetMemTextList();
}

bool ioLocalThailand::GetNewCmdLine( IN const char *szCmd, OUT char *szNewCmd, IN int iNewCmdSize )
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

	enum 
	{ 
		OGP_TYPE_EXPAND_KEY         = 9,  // key value가 생략되어 있어 9번째임
		OGP_TYPE_GAMESERVER_ID_LIST = 11, // key value가 생략되어 있어 11번째임, 앞으로는 통일성을 위해서 한국과 동일하게 처리 필요.
		OGP_TYPE_PASS				= 10,
	};

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

		if( i == OGP_TYPE_EXPAND_KEY )
		{
			StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
			StringCbCat( szNewCmd, iNewCmdSize, pPos );
			StringCbCat( szNewCmd, iNewCmdSize, START_EXPAND_TOKEN );
			StringCbCat( szNewCmd, iNewCmdSize, szCmd );
			StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
			StringCbCat( szNewCmd, iNewCmdSize, "Temp" );
		}

		else if(i == OGP_TYPE_PASS)
		{
			continue;
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

bool ioLocalThailand::IsRightGameServerID( const CString &rszGameServerID )
{
	return true;
}

const char * ioLocalThailand::GetRegKey()
{
	return m_szRegKey;
}

DWORD ioLocalThailand::GetResourceIDBack()
{
	return IDB_BACK_TH;
}

DWORD ioLocalThailand::GetResourceIDChange()
{
	return IDB_CHANGEBTN_TH;
}

DWORD ioLocalThailand::GetResourceIDSetup()
{
	return IDB_SETUPBTN_TH;
}

DWORD ioLocalThailand::GetResourceIDErrorBack()
{
	return IDB_ERROR_BACK_TH;
}

DWORD ioLocalThailand::GetResourceIDErrorExit()
{
	return IDB_EXITBTN_TH;
}

DWORD ioLocalThailand::GetResourceIDErrorSolution()
{
	return IDB_SOLUTIONBTN_TH;
}

const char * ioLocalThailand::GetErrorSolutionURL()
{
	return "http://www.lostsaga.in.th";
}

void ioLocalThailand::SetRegKey( const char *szReg )
{
	StringCbCopy( m_szRegKey, sizeof( m_szRegKey ), szReg );
}

void ioLocalThailand::SetStartURL( const char *szURL )
{
	StringCbCopy( m_szStartURL, sizeof( m_szStartURL ), szURL );
}

bool ioLocalThailand::IsLoginDlg()
{
	return false;
}


bool ioLocalThailand::GetLoginResultPostData( OUT char *szError, IN int iErrorSize, const char *szURL, const char *szID, const char *szPW )
{
	// 빈값
	return false;
}

bool ioLocalThailand::IsReadRegClientPath()
{
	return false;
}

const char * ioLocalThailand::GetFontName()
{
	return FONT_NAME_TH;
}

