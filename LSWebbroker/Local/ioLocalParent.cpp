#include "../stdafx.h"
#include ".\iolocalparent.h"
#include <strsafe.h>
#include "..\Util\HttpApp.h"
#include <string.h>
#include <mmsystem.h>
#include "..\Util\timeoutsocket.h"

ioLocalParent::ioLocalParent(void)
{
	srand( timeGetTime() );
}

ioLocalParent::~ioLocalParent(void)
{
}

bool ioLocalParent::Connect( const char *szIP, int iPort )
{
	if( szIP == NULL )
		return false;

	static bool bInit = false;
	if( !bInit )
	{
		AfxSocketInit();
		bInit = true;
	}
	CTimeOutSocket sock;
	sock.Create();
	sock.SetTimeOut( 5000 ); // 5초 타임아웃
	bool bConnect = true;
	if( !sock.Connect( szIP, iPort ) )
		bConnect = false;

	sock.KillTimeOut();
	sock.Close();

	return bConnect;
}

void ioLocalParent::GetIPPort( IN const char *szIPPortNum , OUT char *szIP, IN int iIPSize, OUT int &riPort )
{
	if( szIPPortNum == NULL )
		return;

	__int64 iIPAndPort = _atoi64( szIPPortNum );

	unsigned char szIPAndPort[MAX_PATH];
	memset( szIPAndPort, 0, MAX_PATH );
	memcpy( szIPAndPort, &iIPAndPort, 8 );

	StringCbPrintf(szIP, iIPSize, "%d.%d.%d.%d", szIPAndPort[0], szIPAndPort[1], szIPAndPort[2], szIPAndPort[3]);
	memcpy(&riPort, &szIPAndPort[4], 4);
}

bool ioLocalParent::ReadURLData( IN const char *szURL, OUT char *szNewCmd, IN int iNewCmdSize, OUT char *szStartData, IN int iStartDataSize )
{
	// txt 파일에서 start 정보를 읽어옴.
	HttpApp http;
	CString sURL = szURL;
	CString szRet;

//	MessageBox(NULL, szURL, "test", MB_OK);

	if( !http.OpenUrl( sURL, szRet ) )
	{
		StringCbCopy( szNewCmd, iNewCmdSize, (LPCTSTR) szRet );
		return false;
	}

	CInternetFile *pIF = http.GetInternetFile(); 
	if( !pIF )
	{
		StringCbCopy( szNewCmd, iNewCmdSize, "pIF == NULL." );
		return false;
	}

	memset( szStartData, 0, iStartDataSize );
	try
	{
		pIF->Read( szStartData, iStartDataSize );
	}
	catch( CInternetException *e )
	{
		if(e)
		{
			TCHAR szError[1024];
			e->GetErrorMessage(szError, 1024);
			e->Delete();
			StringCbCopy( szNewCmd, iNewCmdSize, szError );

//			MessageBox(NULL, szNewCmd, "test2", MB_OK);
		}

		http.DeleteInternetFile();
		http.DeleteSession();

		return false;
	}

	http.DeleteInternetFile();
	http.DeleteSession();

	return true;
}

void ioLocalParent::ParseServerIDList( IN char* szServerIDData, OUT char *szServerID, IN int iSize )
{
	enum 
	{ 
		SERVERID_SIZE      = 30, 
		SERVERID_LIST_SIZE = 100, 
	};

	// parsing server id list
	char szServerIDList[SERVERID_LIST_SIZE][SERVERID_SIZE];
	ZeroMemory( szServerIDList, sizeof( szServerIDList ) );

	int iListCnt=0;
	for (int i = 0; i < SERVERID_LIST_SIZE ; i++)
	{
		char *pPos = NULL;
		if( i == 0 )
			pPos = strtok( szServerIDData, START_EXPAND_TOKEN );
		else
			pPos = strtok( NULL, START_EXPAND_TOKEN );

		if( pPos == NULL )
			break;

		StringCbCopy( szServerIDList[i], sizeof( szServerIDList[i] ), pPos  );
		iListCnt++;
	}

	// search live server ip
	int iRand = 0;
	enum { MAX_CONNECT = 30, }; // connect 5초대기 최고 30*5 로 2분 정도 대기
	for (int i = 0; i < MAX_CONNECT ; i++)
	{
		if( iListCnt > 0 )
			iRand = (rand()+(DWORD)GetCurrentProcessId())%iListCnt;

		char szIP[MAX_PATH]="";
		int iPort = 0;
		GetIPPort( szServerIDList[iRand], szIP, sizeof( szIP ), iPort );	
		if( Connect( szIP, iPort ) )
			break;
	}

	StringCbCopy( szServerID, iSize, szServerIDList[iRand] );
}

bool ioLocalParent::SetRandomURL( IN int iMaxFileNameCnt, IN const char *szDefaultURL, OUT char *szNewURL, IN int iNewURLSize )
{
	memset( szNewURL, 0, iNewURLSize );

	if( !szDefaultURL )
	{
		StringCbCopy( szNewURL, iNewURLSize, "DefaultURL == NULL." );
		return false;
	}

	enum { DEFAULT_SIZE = 4, };
	int iSize = strlen( szDefaultURL );
	if( iSize < DEFAULT_SIZE )
	{
		StringCbCopy( szNewURL, iNewURLSize, "DefaultURL size error." );
		return false;
	}

	if( iMaxFileNameCnt <= 0 )
	{
		StringCbCopy( szNewURL, iNewURLSize, "FileNameCnt <= 0." );
		return false;
	}

	// start.txt --> start1.txt or start2.txt로 변경
	int iRandom = rand()%iMaxFileNameCnt;
	StringCbCopyN( szNewURL, iNewURLSize, szDefaultURL, iSize-DEFAULT_SIZE );
	char szNewName[MAX_PATH]="";
	StringCbPrintf( szNewName, sizeof( szNewName ), "%d.txt", iRandom+1 );
	StringCbCat( szNewURL, iNewURLSize, szNewName ); 

	return true;
}