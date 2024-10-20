#include "../stdafx.h"
#include ".\ioLocalGermany.h"
#include "../Util/httpapp.h"
#include "../resource.h"
#include <strsafe.h>

ioLocalGermany::ioLocalGermany(void)
{
	ZeroMemory( m_szRegKey, sizeof( m_szRegKey ) );
	ZeroMemory( m_szStartURL, sizeof( m_szStartURL ) );
}

ioLocalGermany::~ioLocalGermany(void)
{
}

ioLocalManager::LocalType ioLocalGermany::GetType()
{
	return ioLocalManager::LCT_GERMANY;
}

const char * ioLocalGermany::GetTextListFileName()
{
	return "text_de.txt";
}

const char * ioLocalGermany::GetMemTextList()
{
	return  "|EXE_CMainFrame::OnCreate_1|\n\nLost Saga wird gestartet.\n\n Bitte warte einen Moment.\n\n\n\n  Falls das Spiel nicht startet, \n\n reboote und versuche es noch einmal.|"
			"|EXE_CMainFrame::OnCreate_2|Lost Saga wird upgedatet. Bitte warte einen Moment.|"
			"|EXE_CMainFrame::ParseCmdAndStart_1|Das Parameter stimmt nicht.\nBitte versuche es spater noch einmal.(%1)\n(Fehlercode:104)|"
			"|EXE_CMainFrame::ParseCmdAndStart_2|Der erste Datenwert stimmt nicht.\nBitte versuche es spater noch einmal..(%1/%2)\n(Fehlercode:106)|"
			"|EXE_CMainFrame::SetupAndStart_1|Der Ordnername stimmt nicht.\nBitte versuche es spater noch einmal.(%1)\n(Fehlercode:105)|"
			"|EXE_CSetupAndStart::SetupAndStartGame_1|Das Parameter wurde verandert.\nBitte versuche es spater noch einmal.(%1)\n(Fehlercode:101)|"
			"|EXE_CSetupAndStart::SetupAndStartGame_2|Das automatische Update konnte nicht installiert werden.\nBitte versuche es spater noch einmal.(%1:%2)\n(Fehlercode:102)|"
			"|EXE_CSetupAndStart::SetupAndStartGame_3|Die Patchinfo-Datei konnte nicht installiert werden.\nBitte versuche es spater noch einmal.(%1:%2)\n(Fehlercode:103)|"
			"|EXE_CSetupAndStart::SetupAndStartGame_4|Entweder ist grade kein Server frei oder alle Server sind voll.\n Bitte versuche es spater noch einmal.(%1)\n(Fehlercode:107)|"
			"|EXE_ioErrorDlg::ShowErrorDlg_2|--- Systemanforderungen ---\n%1\n\n--- Fehler ---\n%2|"
			"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_1|Bitte wahle einen Ordner fur die Lost Saga Installation.|"
			"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_2|Der Installationspfad ist leer.\n\n Bitte wahle einen anderen aus.|"
			"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_3|Der Installationspfad darf nicht langer als 100 Zeichen sein.\n\n Bitte wahle eine anderen aus.|"
			"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_4|Auf diesem Pfad kann kein Ordner erstellt werden.\n\n Bitte wahle einen anderen aus.|"
			"|EXE_SetupFolderDlg::OnBnClickedSetup_1|Die Installation von Lost Saga wird vorbereitet. Einen Moment bitte.|";
}

bool ioLocalGermany::GetNewCmdLine( IN const char *szCmd, OUT char *szNewCmd, IN int iNewCmdSize )
{
	if( strcmp( szCmd , "" ) == 0 )
		return false;

	// read form asp of start.
	HttpApp http;
	CString szURL = m_szStartURL;		
	CString szRet;
	if( !http.OpenUrl( szURL, szRet ) )
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

	char szStartData[4096]; // not over 4096 from asp return value.
	memset( szStartData, 0, sizeof(szStartData) );
	try
	{
		pIF->Read( szStartData, sizeof(szStartData) );
	}
	catch( CInternetException *e )
	{
		if(e)
		{
			TCHAR szError[1024];
			e->GetErrorMessage(szError, 1024);
			e->Delete();
			StringCbCopy( szNewCmd, iNewCmdSize, szError );
		}

		http.DeleteInternetFile();
		http.DeleteSession();
		return false;
	}

	http.DeleteInternetFile();
	http.DeleteSession();

	// parsing from cmd
	char szUserID[MAX_PATH]="";
	char szOGPKey[MAX_PATH]="";
	int  iDataCnt = 0;
	bool bCompleteData = false;

	enum { TOKEN_ID = 1, TOKEN_USERID= 2, TOKEN_LOGINKEY = 3, };
	int iTokenCnt = 0;
	int iSize = strlen( szCmd );
	for (int i = 0; i < iSize ; i++)
	{
		if( szCmd[i] == '=' )
		{
			iTokenCnt++;
			iDataCnt      = 0;
			bCompleteData = false;
			continue;
		}

		if( iDataCnt >= MAX_PATH )
			continue;

		if( szCmd[i] == ' ' )
		{
			bCompleteData = true;
			continue;
		}

		if( iTokenCnt == TOKEN_USERID )
		{
			if( bCompleteData )
				continue;

			szUserID[iDataCnt] = szCmd[i];
			iDataCnt++;
		}
		else if( iTokenCnt == TOKEN_LOGINKEY )
		{
			if( bCompleteData )
				continue;

			szOGPKey[iDataCnt] = szCmd[i];
			iDataCnt++;
		}
	}

	// create new cmd
	enum { CHECK_TOKEN = 10, SERVERID_TOKEN = 12, SERVERID_SIZE = 30, SERVERID_LIST_SIZE = 40, };

	char szServerIDList[SERVERID_LIST_SIZE][SERVERID_SIZE];
	ZeroMemory( szServerIDList, sizeof( szServerIDList ) );
	int iServerIDListCnt = 0;
	int iServerIDCnt     = 0;

	iSize     = strlen( szStartData );
	iTokenCnt = 0;
	iDataCnt  = 0;
	for (int i = 0; i < iSize ; i++)
	{
		if( iDataCnt >= iNewCmdSize )
			break;

		if( szStartData[i] == '?' )
		{
			iTokenCnt++;
		}

		if( CHECK_TOKEN == iTokenCnt )
		{			
			int iUserIDSize = strlen( szUserID );
			int iOGPKeySize = strlen( szOGPKey );
			if( ( iDataCnt + iUserIDSize + iOGPKeySize + 3 ) >= iNewCmdSize ) // +3 is ,??
				break;
			StringCbCat( szNewCmd, iNewCmdSize, "," );
			StringCbCat( szNewCmd, iNewCmdSize, szOGPKey ); // expend value
			StringCbCat( szNewCmd, iNewCmdSize, "?" );
			StringCbCat( szNewCmd, iNewCmdSize, szUserID ); // user id to loginkey
			StringCbCat( szNewCmd, iNewCmdSize, "?" );
			iDataCnt += ( iUserIDSize + iOGPKeySize + 3 );
			iTokenCnt++;// complete check_token
		}
		else if( SERVERID_TOKEN == iTokenCnt )
		{
			if( szStartData[i] == '?' )
			{
				szNewCmd[iDataCnt] = szStartData[i];
				iDataCnt++;
			}
			else if( szStartData[i] == ',' )
			{
				iServerIDListCnt++;
				iServerIDCnt = 0;
				if( iServerIDListCnt >= SERVERID_LIST_SIZE )
					break;
			}
			else
			{
				szServerIDList[iServerIDListCnt][iServerIDCnt] = szStartData[i];
				iServerIDCnt++;
				if( iServerIDCnt >= SERVERID_SIZE )
					break;
			}

			if( szStartData[i+1] == '?' )
			{
				int iRand = rand()%(iServerIDListCnt+1);
				if( iRand >= SERVERID_SIZE )
					break;
				StringCbCat( szNewCmd, iNewCmdSize, szServerIDList[iRand] );
				iDataCnt += strlen( szServerIDList[iRand] );
			}
		}
		else
		{
			szNewCmd[iDataCnt] = szStartData[i];
			iDataCnt++;
		}
	}

	return true;
}

bool ioLocalGermany::IsRightGameServerID( const CString &rszGameServerID )
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

const char * ioLocalGermany::GetRegKey()
{
	return m_szRegKey;
}

DWORD ioLocalGermany::GetResourceIDBack()
{
	return IDB_BACK;
}

DWORD ioLocalGermany::GetResourceIDChange()
{
	return IDB_CHANGEBTN;
}

DWORD ioLocalGermany::GetResourceIDSetup()
{
	return IDB_SETUPBTN;
}

DWORD ioLocalGermany::GetResourceIDErrorBack()
{
	return IDB_ERROR_BACK;
}

DWORD ioLocalGermany::GetResourceIDErrorExit()
{
	return IDB_EXITBTN;
}

DWORD ioLocalGermany::GetResourceIDErrorSolution()
{
	return -1;
}

const char * ioLocalGermany::GetErrorSolutionURL()
{
	return "http://lostsaga.ogplanet.com";
}

void ioLocalGermany::SetRegKey( const char *szReg )
{
	StringCbCopy( m_szRegKey, sizeof( m_szRegKey ), szReg );
}


void ioLocalGermany::SetStartURL( const char *szURL )
{
	StringCbCopy( m_szStartURL, sizeof( m_szStartURL ), szURL );
}

bool ioLocalGermany::IsLoginDlg()
{
	return false;
}

bool ioLocalGermany::GetLoginResultPostData( OUT char *szError, IN int iErrorSize, const char *szURL, const char *szID, const char *szPW )
{
  // ºó°ª
	return false;
}

const char * ioLocalGermany::GetFontName()
{
	return "Tahoma";
}
