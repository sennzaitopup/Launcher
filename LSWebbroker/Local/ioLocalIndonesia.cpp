#include "../stdafx.h"
#include ".\ioLocalIndonesia.h"
#include "../resource.h"
#include <strsafe.h>
#include "../Util/HttpApp.h"
#include "../StringManager/ioHashString.h"
#include <mmsystem.h>
#include "../SetupAndStart.h"

ioLocalIndonesia::ioLocalIndonesia(void)
{
	ZeroMemory( m_szRegKey, sizeof( m_szRegKey ) );
	ZeroMemory( m_szStartURL, sizeof( m_szStartURL ) );
}

ioLocalIndonesia::~ioLocalIndonesia(void)
{
}

ioLocalManager::LocalType ioLocalIndonesia::GetType()
{
	return ioLocalManager::LCT_INDONESIA;
}

const char * ioLocalIndonesia::GetTextListFileName()
{
	return "text_id.txt";
}

const char * ioLocalIndonesia::GetMemTextList()
{
	return  "|EXE_CMainFrame::OnCreate_1|\r\n\r\nLostSaga sedang berjalan.\r\n\r\n     Silahkan menunggu sebentar.\r\n\r\n\r\n\r\n  Jika Game tidak berjalan dalam waktu lama\r\n\r\n   Silahkan coba lagi setelah reboot.|"
			"|EXE_CMainFrame::OnCreate_2|LostSaga sedang Update.Silahkan menunggu.|"
			"|EXE_CMainFrame::ParseCmdAndStart_1|Data tidak valid.\r\nSilahkan coba beberapa saat lagi.(%1:%2)\r\n(Error Code:104)|"
			"|EXE_CMainFrame::ParseCmdAndStart_2|Data Start tidak valid..\r\nSilahkan coba beberapa saat lagi.(%1/%2)\r\n(Error Code:106)|"
			"|EXE_CMainFrame::SetupAndStart_1|Nama Folder yang baru diset salah.\r\nSilahkan coba beberapa saat lagi.(%1)\r\n(Error Code:105)|"
			"|EXE_CSetupAndStart::SetupAndStartGame_1|Data telah berubah.\r\nSilahkan coba beberapa saat lagi.(%1)\r\n(Error Code:101)|"
			"|EXE_CSetupAndStart::SetupAndStartGame_2|Gagal Auto Update.\r\nSilahkan coba beberapa saat lagi.(%1:%2)\r\n(Error Code:102)|"
			"|EXE_CSetupAndStart::SetupAndStartGame_3|Gagal info Patch file.\r\nSilahkan coba beberapa saat lagi.(%1:%2)\r\n(Error Code:103)|"
			"|EXE_CSetupAndStart::SetupAndStartGame_4|Tidak ada Server atau Server sedang full.\r\nSilahkan coba beberapa saat lagi.(%1)\r\n(Error Code:107)|"
			"|EXE_ioErrorDlg::ShowErrorDlg_2|--- Spesifikasi System ---\r\n%1\r\n\r\n--- Info Error ---\r\n%2|"
			"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_1|Silahkan memilih folder yang akan install Lost Saga.|"
			"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_2|Folder yang dipilih kosong.\r\n\r\n     Silahkan set lagi.|"
			"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_3|Nama folder tidak dapat melebihi 100 huruf.\r\n\r\n              Silahkan set lagi.|"
			"|EXE_SetupFolderDlg::OnBnClickedChangeFolder_4|Tidak dapat membuat Folder.\r\n\r\n       Silahkan set lagi.|"
			"|EXE_SetupFolderDlg::OnBnClickedSetup_1|Sedang mempersiapkan untuk menginstal Lost Saga. Silahkan menunggu sebentar.|";
}

bool ioLocalIndonesia::GetNewCmdLine( IN const char *szCmd, OUT char *szNewCmd, IN int iNewCmdSize )
{
	char szRandomURL[MAX_PATH*2]="";

	if( !SetRandomURL( MAX_READ_URL_FILE_NAME_CNT, m_szStartURL, szRandomURL, sizeof( szRandomURL ) ) )
	{
		StringCbCopy( szNewCmd, iNewCmdSize, szRandomURL ); // error 

//		MessageBox(NULL, szRandomURL, "jal1", MB_OK);
// 		char szTempMsg[MAX_PATH*12]="";
// 		pMainFrame->SafeSprintf( szTempMsg, sizeof( szTempMsg ), "%1 %1 %2",m_szStartURL, szRandomURL, GetLastError() );
// 		pMainFrame->ShowErrorDlg( szTempMsg,534 );

		return false;
	}

	enum 
	{
		MAX_BUFF = 4096,
	};
	// txt 파일에서 start 정보를 읽어옴.
	char szStartData[MAX_BUFF]; // asp 파일의 결과 같은 4096바이트를 넘지 않는다.

	int i = 0;

	while(1)
	{
		i++;

		if( ReadURLData( (LPCTSTR) szRandomURL, szNewCmd, iNewCmdSize, szStartData, sizeof( szStartData ) ) )
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
				int nRet = MessageBox(NULL, "Saat ini server sedang penuh. \nDapatkah kamu coba login kembali?", 
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

bool ioLocalIndonesia::IsRightGameServerID( const CString &rszGameServerID )
{
	return true;
}

const char * ioLocalIndonesia::GetRegKey()
{
	return m_szRegKey;
}

DWORD ioLocalIndonesia::GetResourceIDBack()
{
	return IDB_BACK_ID;
}

DWORD ioLocalIndonesia::GetResourceIDChange()
{
	return IDB_CHANGEBTN_ID;
}

DWORD ioLocalIndonesia::GetResourceIDSetup()
{
	return IDB_SETUPBTN_ID;
}

DWORD ioLocalIndonesia::GetResourceIDErrorBack()
{
	return IDB_ERROR_BACK_ID;
}

DWORD ioLocalIndonesia::GetResourceIDErrorExit()
{
	return IDB_EXITBTN_ID;
}

DWORD ioLocalIndonesia::GetResourceIDErrorSolution()
{
	return IDB_SOLUTIONBTN_ID;
}

const char * ioLocalIndonesia::GetErrorSolutionURL()
{
	return "http://forum.gemscool.com/forum-143.html";
}

void ioLocalIndonesia::SetRegKey( const char *szReg )
{
	StringCbCopy( m_szRegKey, sizeof( m_szRegKey ), szReg );
}

void ioLocalIndonesia::SetStartURL( const char *szURL )
{
	StringCbCopy( m_szStartURL, sizeof( m_szStartURL ), szURL );
}

bool ioLocalIndonesia::IsLoginDlg()
{
	return false;
}


bool ioLocalIndonesia::GetLoginResultPostData( OUT char *szError, IN int iErrorSize, const char *szURL, const char *szID, const char *szPW )
{
	// 빈값
	return false;
}

bool ioLocalIndonesia::IsReadRegClientPath()
{
	return false;
}

const char * ioLocalIndonesia::GetFontName()
{
	return "Tahoma";
}

