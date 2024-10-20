#include "../stdafx.h"
#include ".\iolocaltaiwan.h"
#include "../resource.h"
#include <strsafe.h>
#include "../Util/HttpApp.h"
#include "ioLocalTaiwanLanguage.h"
#include <mmsystem.h>
#include "../SetupAndStart.h"

ioLocalTaiwan::ioLocalTaiwan(void)
{
	ZeroMemory( m_szRegKey, sizeof( m_szRegKey ) );
	ZeroMemory( m_szStartURL, sizeof( m_szStartURL ) );
}

ioLocalTaiwan::~ioLocalTaiwan(void)
{
}

ioLocalManager::LocalType ioLocalTaiwan::GetType()
{
	return ioLocalManager::LCT_TAIWAN;
}

const char * ioLocalTaiwan::GetTextListFileName()
{
	return "text_tw.txt";
}

const char * ioLocalTaiwan::GetMemTextList()
{
	return TaiwanLanguage::GetMemTextList();
}

bool ioLocalTaiwan::GetNewCmdLine( IN const char *szCmd, OUT char *szNewCmd, IN int iNewCmdSize )
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

bool ioLocalTaiwan::IsRightGameServerID( const CString &rszGameServerID )
{
	return true;
}

const char * ioLocalTaiwan::GetRegKey()
{
	return m_szRegKey;
}

DWORD ioLocalTaiwan::GetResourceIDBack()
{
	return IDB_BACK_TW;
}

DWORD ioLocalTaiwan::GetResourceIDChange()
{
	return IDB_CHANGEBTN_TW;
}

DWORD ioLocalTaiwan::GetResourceIDSetup()
{
	return IDB_SETUPBTN_TW;
}

DWORD ioLocalTaiwan::GetResourceIDErrorBack()
{
	return IDB_ERROR_BACK_TW;
}

DWORD ioLocalTaiwan::GetResourceIDErrorExit()
{
	return IDB_EXITBTN_TW;
}

DWORD ioLocalTaiwan::GetResourceIDErrorSolution()
{
	return IDB_SOLUTIONBTN_TW;
}

const char * ioLocalTaiwan::GetErrorSolutionURL()
{
	return "http://lostsaga.gamon.net/app/help/faq.aspx";
}

void ioLocalTaiwan::SetRegKey( const char *szReg )
{
	StringCbCopy( m_szRegKey, sizeof( m_szRegKey ), szReg );
}

void ioLocalTaiwan::SetStartURL( const char *szURL )
{
	StringCbCopy( m_szStartURL, sizeof( m_szStartURL ), szURL );
}

bool ioLocalTaiwan::IsLoginDlg()
{
	return false;
}

bool ioLocalTaiwan::GetLoginResultPostData( OUT char *szError, IN int iErrorSize, const char *szURL, const char *szID, const char *szPW )
{
	return false;
}

bool ioLocalTaiwan::IsReadRegClientPath()
{
	return false;
}

const char * ioLocalTaiwan::GetFontName()
{
	return FONT_NAME_TW;
}

