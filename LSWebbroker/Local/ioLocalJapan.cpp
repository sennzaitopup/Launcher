#include "../stdafx.h"
#include ".\iolocalJapan.h"
#include "../resource.h"
#include <strsafe.h>
#include "../SetupAndStart.h"
#include "../Netmarble/NMCrypt.h"
#include "ioLocalJapanLanguage.h"

ioLocalJapan::ioLocalJapan(void)
{
	ZeroMemory( m_szRegKey, sizeof( m_szRegKey ) );
	ZeroMemory( m_szStartURL, sizeof( m_szStartURL ) );
}

ioLocalJapan::~ioLocalJapan(void)
{
}

ioLocalManager::LocalType ioLocalJapan::GetType()
{
	return ioLocalManager::LCT_JAPAN;
}

const char * ioLocalJapan::GetTextListFileName()
{
	return "text.txt";
}

const char * ioLocalJapan::GetMemTextList()
{
	return JapanLanguage::GetMemTextList();
}

bool ioLocalJapan::GetNewCmdLine( IN const char *szCmd, OUT char *szNewCmd, IN int iNewCmdSize )
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

	// cmd에서 체크
	if( strcmp( szCmd, "" ) == 0 )
	{
		StringCbCopy( szNewCmd, iNewCmdSize, "Cmd is empty." );
		return false;
	}

	enum 
	{ 
		MAX_KEY_DATA         = 1024, 
		MAX_CIPHER_DATA      = 4816,

		MAX_TOKEN            = 5, 
		TOKEN_TIME_ARRAY     = 0, 
		TOKEN_MD5_ARRAY      = 1, 
		TOKEN_ID_ARRAY       = 2, 
		TOKEN_PW_ARRAY       = 3, 
		TOKEN_EXTRA_ARRAY    = 4, 
	};

	if( strlen( szCmd ) >= MAX_CIPHER_DATA ) 
	{
		StringCbCopy( szNewCmd, iNewCmdSize, "Cmd is over." );
		return false;
	}

	char szKey[MAX_KEY_DATA];
	ZeroMemory( szKey, MAX_KEY_DATA );

	char szEncryptData[MAX_CIPHER_DATA];
	ZeroMemory( szEncryptData, MAX_CIPHER_DATA );

	char szDecryptData[MAX_CIPHER_DATA];
	ZeroMemory( szDecryptData, MAX_CIPHER_DATA );

	char szEncryptTTMDID[MAX_KEY_DATA];
	ZeroMemory( szEncryptTTMDID, MAX_KEY_DATA );

	StringCbCopy( szEncryptData, MAX_CIPHER_DATA , szCmd );

#ifdef NDEBUG // CJ에서 디버그용 lib을 전달해 주지 않았어 컴파일 오류로 디서블
	if( !GetCryptKey( szKey, MAX_KEY_DATA-1 ) )
	{
		StringCbCopy( szNewCmd, iNewCmdSize, "Error Get Key." );
		return false;
	}

	int iReturn = DecryptString( szEncryptData, szKey, szDecryptData );
	if( CRYPT_SUCCESS != iReturn )
	{
		StringCbPrintf( szNewCmd, iNewCmdSize, "Error Decrypt.(%d).", iReturn );
		return false;
	}
	// TEST 버전
	//StringCbCopy( szDecryptData, MAX_CIPHER_DATA, szCmd );
#endif

	for (int i = 0; i < MAX_TOKEN ; i++)
	{
		char *pPos = NULL;
		if( i == 0 )
			pPos = strtok( szDecryptData, JAPAN_TOKEN );
		else
			pPos = strtok( NULL, JAPAN_TOKEN );

		if( pPos == NULL )
			break;

		if( i == TOKEN_TIME_ARRAY || i == TOKEN_MD5_ARRAY  )
		{
			StringCbCat( szEncryptTTMDID, MAX_KEY_DATA, pPos  );
			StringCbCat( szEncryptTTMDID, MAX_KEY_DATA, JAPAN_NEW_TOKEN  );
		}
		else if( i == TOKEN_ID_ARRAY )
		{
			StringCbCat( szEncryptTTMDID, MAX_KEY_DATA, pPos  );
			break;
		}
	}

	// new cmd 제작
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

		if( i == CSetupAndStart::OPTION_TYPE_KEY_VALUE )
		{
			StringCbCat( szNewCmd, iNewCmdSize, START_TOKEN );
			StringCbCat( szNewCmd, iNewCmdSize, szEncryptTTMDID ); 

		}
		else if( i == CSetupAndStart::OPTION_TYPE_GAMESERVER_ID )
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

bool ioLocalJapan::IsRightGameServerID( const CString &rszGameServerID )
{
	return true;
}

const char * ioLocalJapan::GetRegKey()
{
	return m_szRegKey;
}

DWORD ioLocalJapan::GetResourceIDBack()
{
	return IDB_BACK_JP;
}

DWORD ioLocalJapan::GetResourceIDChange()
{
	return IDB_CHANGEBTN_JP;
}

DWORD ioLocalJapan::GetResourceIDSetup()
{
	return IDB_SETUPBTN_JP;
}

DWORD ioLocalJapan::GetResourceIDErrorBack()
{
	return IDB_ERROR_BACK_JP;
}

DWORD ioLocalJapan::GetResourceIDErrorExit()
{
	return IDB_EXITBTN_JP;
}

DWORD ioLocalJapan::GetResourceIDErrorSolution()
{
	return IDB_SOLUTIONBTN_JP;
}

const char * ioLocalJapan::GetErrorSolutionURL()
{
	return "http://www.lostsaga.jp";
}

void ioLocalJapan::SetRegKey( const char *szReg )
{
	StringCbCopy( m_szRegKey, sizeof( m_szRegKey ), szReg );
}

void ioLocalJapan::SetStartURL( const char *szURL )
{
	StringCbCopy( m_szStartURL, sizeof( m_szStartURL ), szURL );
}

bool ioLocalJapan::IsLoginDlg()
{
	return false;
}

bool ioLocalJapan::GetLoginResultPostData( OUT char *szError, IN int iErrorSize, const char *szURL, const char *szID, const char *szPW )
{
	// 빈값
	return false;
}

bool ioLocalJapan::IsReadRegClientPath()
{
	return false;
}

const char * ioLocalJapan::GetFontName()
{
	return FONT_NAME_JP;
}

