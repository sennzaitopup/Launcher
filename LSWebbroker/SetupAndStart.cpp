#include "stdafx.h"
#include ".\setupandstart.h"
#include "Util\Crc32Static.h"
#include "MainFrm.h"
#include "Util\HelpFunc.h"
#include <strsafe.h>
#include "stringmanager\safesprintf.h"
#include "stringmanager\iostringmanager.h"
#include "local\iolocalparent.h"
#include "local\iolocalmanager.h"

#define CLIENT_FILE_NAME "%s\\lostsaga.exe"

CSetupAndStart::CSetupAndStart(void)
{
	m_hSetupReturnValue = 0;
}

CSetupAndStart::~CSetupAndStart(void)
{
}



bool CSetupAndStart::IsRightCRC( LPCTSTR szFileName, LPCTSTR szServerCRC )
{
	DWORD dwCRC = 0;
	CCrc32Static::FileCrc32Streams(szFileName, dwCRC);
	char szCRC[MAX_PATH]="";
	StringCbPrintf(szCRC,sizeof(szCRC), "%u", dwCRC);

	if(strcmp(szServerCRC, szCRC) != 0)
		return false;

	return true;
}

void CSetupAndStart::ExcuteProcess( LPCTSTR szFileName , LPCTSTR szCmdLine)
{
	char szDrive[_MAX_DRIVE]="";
	char szDir[_MAX_DIR*2] = "";
	_splitpath(szFileName, szDrive, szDir, NULL, NULL);
	char szPath[1024]="";
	StringCbPrintf(szPath,sizeof(szPath), "%s%s", szDrive, szDir);
	SetCurrentDirectory(szPath);

	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO         StartupInfo;
	StartupInfo.cb               = sizeof(STARTUPINFO);
	StartupInfo.lpReserved       = NULL;
	StartupInfo.lpDesktop        = NULL;
	StartupInfo.lpTitle          = NULL;
	StartupInfo.dwFlags          = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow      = SW_SHOWNORMAL;
	StartupInfo.cbReserved2      = 0;
	StartupInfo.lpReserved2      = NULL;

	CreateProcess(szFileName,(LPTSTR)szCmdLine,NULL,NULL,FALSE,0,NULL,NULL,&StartupInfo,&ProcessInfo);
}


/* 캐쉬 문제로 주석 처리
bool CSetupAndStart::Setup( LPCTSTR szCreateFolder, LPCTSTR szDownLoadURL, LPCTSTR szCreateFileName )
{
	Help::SetCreateDirectory(szCreateFolder);

	SetFileAttributes(szCreateFileName,FILE_ATTRIBUTE_NORMAL);
	::DeleteFile(szCreateFileName);

	// 캐쉬를 지우기 위해서 아래 함수를 사용해야 함 하지만 windows 7에서는 작동하지 않음.
	// DeleteUrlCacheEntry("http://lostsaga.nefficient.co.kr/ServiceZone/AutoUpgrade/autoupgrade.exe");
	HRESULT hr = URLDownloadToFile(0, szDownLoadURL, szCreateFileName, 0, NULL);
	m_hSetupReturnValue = hr;
	if(hr != S_OK)
		return false;

	return true;
}*/


bool CSetupAndStart::SetupAndStartGame( bool bReadRegClientPath )
{
	char szRegKey[MAX_PATH*2]="";
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal )
		StringCbPrintf( szRegKey , sizeof( szRegKey ), "%sWebBroker", pLocal->GetRegKey() );

	if(m_szAutoFileName.IsEmpty()||
		m_szServerAutoCRC.IsEmpty()||
		m_szINIFileName.IsEmpty()||
		m_szServerINICRC.IsEmpty()||
		m_szAutoCreateFolder.IsEmpty()||
		m_szAutoDownloadURL.IsEmpty()||
		m_szINICreateFolder.IsEmpty()||
		m_szINIDownloadURL.IsEmpty()||
		m_szFullzipFileName.IsEmpty()||
		m_szExpandValue.IsEmpty()||
		m_szKeyValue.IsEmpty()||
		m_szOptions.IsEmpty()||
		m_szGameServerID.IsEmpty() ||
		m_szFullZipVer.IsEmpty() ||
		m_szReadRegClientPath.IsEmpty() )
	{
		char szTempMsg[MAX_PATH*4]="";
		SafeSprintf( szTempMsg, sizeof( szTempMsg ), STR(1), GetLastError());
		ShowErrorDlg( szTempMsg, 101 );
		Help::RegWriteInt( REG_SHCU, szRegKey, "Error", 101);
		return false;
	}

	if( pLocal )
	{
		if( !pLocal->IsRightGameServerID( m_szGameServerID ) )
		{
			char szTempMsg[MAX_PATH*4]="";
			SafeSprintf( szTempMsg, sizeof( szTempMsg ), STR(4), GetLastError());
			ShowErrorDlg( szTempMsg, 107 );
			Help::RegWriteInt( REG_SHCU, szRegKey, "Error", 107);
			return false;
		}
	}

	if( bReadRegClientPath )
	{
		SetNewPathToRegPath();

		if( pLocal && pLocal->IsReadRegClientPath() )
			Help::RegWriteString( REG_SHCU, szRegKey, "ClientPath", (LPCTSTR)m_szClientPath);
	}

#ifndef LG_UPLUS

	char szExcutePath[MAX_PATH]="";
	::GetCurrentDirectory(MAX_PATH,szExcutePath);

	m_szAutoFileName = "";

	m_szAutoFileName.Format("%s\\autoupgrade.exe", szExcutePath);

	if(!IsRightCRC(m_szAutoFileName, m_szServerAutoCRC))
	{
		DWORD dwDownloadedSize = 0;
		if(!DownLoadProcess( m_szAutoDownloadURL, 0, m_szAutoFileName, dwDownloadedSize ))
		{
			char szTempMsg[MAX_PATH*4]="";
			SafeSprintf( szTempMsg, sizeof( szTempMsg ), STR(2), (int)GetLastError(), (int)m_hSetupReturnValue );
			ShowErrorDlg( szTempMsg,102 );
			Help::RegWriteInt( REG_SHCU, szRegKey, "Error", 102);
			return false;
		}
	}

	char szFileName[MAX_PATH];
	char szFileExt[MAX_PATH];
	memset(szFileName, 0, sizeof(szFileName) );
	memset(szFileExt, 0, sizeof(szFileExt) );
	_splitpath( (LPCTSTR)m_szINIFileName, NULL, NULL, szFileName, szFileExt );
	m_szINIFileName = szExcutePath;
	m_szINIFileName += "\\info\\";
	m_szINIFileName += szFileName;
	m_szINIFileName += szFileExt;


	if(!IsRightCRC(m_szINIFileName, m_szServerINICRC))
	{
		DWORD dwDownloadedSize = 0;
		if(!DownLoadProcess( m_szINIDownloadURL, 0, m_szINIFileName, dwDownloadedSize ))
		{
			char szTempMsg[MAX_PATH*4]="";
			SafeSprintf( szTempMsg, sizeof( szTempMsg ), STR(3), (int)GetLastError(), (int)m_hSetupReturnValue );
			ShowErrorDlg( szTempMsg,103 );
			Help::RegWriteInt( REG_SHCU, szRegKey, "Error", 103);
			return false;
		}
	}

	memset(szFileName, 0, sizeof(szFileName) );
	memset(szFileExt, 0, sizeof(szFileExt) );
	_splitpath( (LPCTSTR)m_szFullzipFileName, NULL, NULL, szFileName, szFileExt );
	m_szFullzipFileName = szExcutePath;
	m_szFullzipFileName += "\\";
	m_szFullzipFileName += szFileName;
	m_szFullzipFileName += szFileExt;

	m_szAutoCreateFolder = szExcutePath;
	m_szINICreateFolder  = szExcutePath;
	m_szINICreateFolder += "\\info";

	if(!IsRightFullZipVer(m_szFullzipFileName, m_szFullZipVer))
	{
		SetFileAttributes((LPCTSTR)m_szFullzipFileName,FILE_ATTRIBUTE_NORMAL);
		::DeleteFile((LPCTSTR)m_szFullzipFileName);
	}
#endif
	// start
	char CmdLine[1024] = "";
	memset(CmdLine, 0, sizeof(CmdLine));

	StringCbPrintf(CmdLine, sizeof(CmdLine), " %s?%s?%s?%s?%s?",
		(LPCTSTR)m_szExpandValue,(LPCTSTR)m_szKeyValue,(LPCTSTR)m_szOptions,
		(LPCTSTR)m_szGameServerID,(LPCTSTR)m_szFullZipVer);

	// 대만 태국(가레나) 추가 인자값 처리
	if( (ioLocalManager::GetLocalType() == ioLocalManager::LCT_TAIWAN || ioLocalManager::GetLocalType() == ioLocalManager::LCT_THAILAND) && !m_szAutoDownloadExt.IsEmpty() )
	{
		char tempCmdLine[1024] = {0,};
		memcpy_s(tempCmdLine, sizeof(tempCmdLine), CmdLine, sizeof(CmdLine));
		StringCbPrintf(CmdLine, sizeof(CmdLine), "%s%s", tempCmdLine, (LPCTSTR)m_szAutoDownloadExt);
	}

#ifndef LG_UPLUS

	ExcuteProcess(m_szAutoFileName, CmdLine);

#else
	char rootDir[MAX_PATH]="";
	::GetCurrentDirectory(MAX_PATH,rootDir);

	char szClientName[MAX_PATH] = "";
	StringCbPrintf(szClientName, sizeof(szClientName), CLIENT_FILE_NAME,rootDir);

	ExcuteProcess(szClientName, CmdLine);
#endif

	return true;
}

bool CSetupAndStart::ParseCmd( LPCTSTR szCmdLine )
{
	if( szCmdLine == NULL )
		return false; 

 	char szCopiedCmd[2048]="";
	memset(szCopiedCmd, 0, sizeof(szCopiedCmd));
	StringCbCopy(szCopiedCmd,sizeof(szCopiedCmd), szCmdLine);

	char szExpand[2048] = {0, };

	// cmd parsing
	for(int i = 0; i < OPTION_TYPE_MAX; i++)
	{
		char *pos = NULL;

		if( i == 0)
			pos = strtok(szCopiedCmd, CMD_SECTION_TOKEN);
		else
			pos = strtok(NULL, CMD_SECTION_TOKEN);

		if(pos == NULL)
			break;

		switch(i)
		{
		case OPTION_TYPE_AUTO_FILE_NAME: m_szAutoFileName = pos; break;
		case OPTION_TYPE_SERVER_AUTO_VERSION: m_szServerAutoCRC = pos; break;

		case OPTION_TYPE_INI_FILE_NAME: m_szINIFileName = pos; break;
		case OPTION_TYPE_SERVER_INI_CRC: m_szServerINICRC = pos; break;

		case OPTION_TYPE_AUTO_CREATE_FOLDER: m_szAutoCreateFolder = pos; break;
		case OPTION_TYPE_AUTO_DOWNLOAD_URL: m_szAutoDownloadURL = pos; break;
		
		case OPTION_TYPE_INI_CREATE_FOLDER: m_szINICreateFolder = pos; break;
		case OPTION_TYPE_INI_DOWNLOAD_URL: m_szINIDownloadURL = pos; break;
		
		case OPTION_TYPE_FULLZIP_FILE_NAME: m_szFullzipFileName = pos; break;
		case OPTION_TYPE_CHECK_VALUE: strcpy_s(szExpand, pos); break;
		case OPTION_TYPE_KEY_VALUE: m_szKeyValue = pos; break;
		case OPTION_TYPE_OPTIONS: m_szOptions = pos; break;
		case OPTION_TYPE_GAMESERVER_ID: m_szGameServerID = pos; break;

		case OPTION_TYPE_FULLZIP_VER: m_szFullZipVer = pos; break;
		case OPTION_TYPE_READ_REG_CLIENT_PATH: m_szReadRegClientPath = pos; break;
		case OPTION_TYPE_AUTO_DOWNLOAD_EXTEND_URL: m_szAutoDownloadExt = pos; break;
		}
	}

	char szLocalPath[1024] = {0,};
	StringCbCopy(szLocalPath,sizeof(szLocalPath), szExpand);
	
	int idx = 0;
	char szBuf[MAX_PATH] = {0,};
	int nSize = strlen(szExpand);
	for(int i=0; i<nSize; ++i)
	{
		char *localPos = NULL;
		if(i == 0)
			localPos = strtok(szLocalPath, CMD_COMMA_TOKEN);
		else
			localPos = strtok(NULL, CMD_COMMA_TOKEN);

		if(i == 9)
		{
			CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();

			if(pMainFrame)
			{
				char szCountry[1024] = {0, };
				strcpy_s(szCountry, localPos);
				int nTemp = atoi(szCountry);
				int nLocalType = nTemp + pMainFrame->GetSubLocalType();
				itoa(nLocalType, szBuf, 10);
				strcat(szBuf, ",");
			}
			else
				break;
		}

		if(szExpand[i] == ',')
			++idx;

		if(idx == 9)
		{
			int nCopyCnt = i+1;
			int nBufSize = strlen(szBuf);
			for(int j=0; j<nBufSize; ++j)
			{
				szExpand[nCopyCnt] = szBuf[j];
				++nCopyCnt;
			}

			szExpand[nCopyCnt] = '\0';

			m_szExpandValue = szExpand;
			break;
		}
	}

	if(ioLocalManager::GetLocalType() == ioLocalManager::LCT_TAIWAN || ioLocalManager::GetLocalType() == ioLocalManager::LCT_THAILAND )
	{
		// 대만일경우 추가 인자값이 있으면.
		if(!m_szAutoDownloadExt.IsEmpty())
		{

			const char * cDownKeyWord =  "AutoUpgrade";

			int nStart = m_szAutoDownloadURL.Find(cDownKeyWord);
			int nEnd = m_szAutoDownloadURL.GetLength() - nStart;
			CString strTemp;
			strTemp.FormatMessage("%1%2/%3", m_szAutoDownloadURL.Left(nStart), m_szAutoDownloadExt, m_szAutoDownloadURL.Right(nEnd));
			m_szAutoDownloadURL = strTemp;

			int nStart2 = m_szINIDownloadURL.Find(cDownKeyWord);
			int nEnd2 = m_szINIDownloadURL.GetLength() - nStart2;
			CString strTemp2;
			strTemp2.FormatMessage("%1%2/%3", m_szINIDownloadURL.Left(nStart2), m_szAutoDownloadExt, m_szINIDownloadURL.Right(nEnd2));
			m_szINIDownloadURL = strTemp2;


		}

	}

	if(m_szAutoDownloadURL)

	if(m_szAutoFileName.IsEmpty()||
	   m_szServerAutoCRC.IsEmpty()||
	   m_szINIFileName.IsEmpty()||
	   m_szServerINICRC.IsEmpty()||
	   m_szAutoCreateFolder.IsEmpty()||
	   m_szAutoDownloadURL.IsEmpty()||
	   m_szINICreateFolder.IsEmpty()||
	   m_szINIDownloadURL.IsEmpty()||
	   m_szFullzipFileName.IsEmpty()||
	   m_szExpandValue.IsEmpty()||
	   m_szKeyValue.IsEmpty()||
	   m_szOptions.IsEmpty()||
	   m_szGameServerID.IsEmpty() ||
	   m_szFullZipVer.IsEmpty() ||
	   m_szReadRegClientPath.IsEmpty() )
	{
	   return false;
	}

	return true;
}

bool CSetupAndStart::IsRightFullZipVer( LPCTSTR szFileName, LPCTSTR szServerVersion )
{
	CFile file;
	if(!file.Open(szFileName, CFile::modeRead))
		return true;
	char szText[MAX_PATH]="";
	file.Read(szText, sizeof(szText));
	file.Close();

	if(strcmp(szText, szServerVersion) != 0)
		return false;
	
	return true;
}

void CSetupAndStart::ShowErrorDlg( const char* szErrorText, int iErrorNumber )
{
	if(AfxGetApp())
	{
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();
		if(pMainFrame)
			pMainFrame->ShowErrorDlg(szErrorText,iErrorNumber);
	}
}

void CSetupAndStart::ReadClientPath( const char *szRegKeyIO )
{
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal && !pLocal->IsReadRegClientPath() )
	{
		char szCurDir[MAX_PATH]="";
		GetCurrentDirectory(MAX_PATH, (LPSTR)szCurDir);
		m_szClientPath = szCurDir;
	}
	else
	{
		char szPath[MAX_PATH*2]="";
		Help::RegReadString( REG_SHCU, szRegKeyIO, "ClientPath", "",  szPath, sizeof(szPath) );
		if(szPath[0] == NULL) //client path is not registered
		{
			GetCurrentDirectory(MAX_PATH,(LPSTR)szPath);
			Help::RegWriteString( REG_SHCU, szRegKeyIO, "ClientPath", (LPCTSTR)szPath);
		}
		m_szClientPath = szPath;
	}
}

bool CSetupAndStart::IsEmptyClientPath()
{
	return m_szClientPath.IsEmpty();
}

bool CSetupAndStart::IsReadRegClientPath()
{
	if(m_szReadRegClientPath == "1")
		return true;

	return false;
}

void CSetupAndStart::SetNewPathToRegPath()
{
	if(m_szClientPath.IsEmpty()) return;

	char szFileName[MAX_PATH]="";
	char szFileExt[MAX_PATH]="";
	_splitpath( (LPCTSTR)m_szAutoFileName, NULL, NULL, szFileName, szFileExt );
	m_szAutoFileName = m_szClientPath;
	m_szAutoFileName += "\\";
	m_szAutoFileName += szFileName;
	m_szAutoFileName += szFileExt;

	memset(szFileName, 0, sizeof(szFileName) );
	memset(szFileExt, 0, sizeof(szFileExt) );
	_splitpath( (LPCTSTR)m_szINIFileName, NULL, NULL, szFileName, szFileExt );
	m_szINIFileName = m_szClientPath;
	m_szINIFileName += "\\info\\";
	m_szINIFileName += szFileName;
	m_szINIFileName += szFileExt;


	memset(szFileName, 0, sizeof(szFileName) );
	memset(szFileExt, 0, sizeof(szFileExt) );
	_splitpath( (LPCTSTR)m_szFullzipFileName, NULL, NULL, szFileName, szFileExt );
	m_szFullzipFileName = m_szClientPath;
	m_szFullzipFileName += "\\";
	m_szFullzipFileName += szFileName;
	m_szFullzipFileName += szFileExt;

	m_szAutoCreateFolder = m_szClientPath;
	m_szINICreateFolder  = m_szClientPath;
	m_szINICreateFolder += "\\info";
}

// 자동업그레이 함수와 동일 구조
bool CSetupAndStart::DownLoadProcess( const CString &szURL,
									  int iRemoteFileSize,
									  const CString &rszDownloadedFullPath,
									  OUT DWORD &rdwDownloadedSize )
{
	CString szErrorMsg;

	if( !m_HTTP.OpenUrl(szURL, szErrorMsg) )
	{
		m_HTTP.DeleteSession();
		return false;
	}

	CInternetFile *pIF = m_HTTP.GetInternetFile(); 
	if(!pIF)
	{
		m_HTTP.DeleteSession();
		return false;
	}

	char szDrive[MAX_PATH]= "";
	char szDir[MAX_PATH*2]= "";
	char szDriveDir[MAX_PATH*2]= "";
	::_splitpath((LPCTSTR)rszDownloadedFullPath,szDrive,szDir,NULL,NULL);
	StringCbCopy( szDriveDir , sizeof( szDriveDir ), szDrive );
	StringCbCat( szDriveDir , sizeof( szDriveDir ), szDir );
	Help::SetCreateDirectory( szDriveDir );

	if(!Help::DeleteFileRepeat( (LPCTSTR) rszDownloadedFullPath ) )
	{
		m_HTTP.DeleteInternetFile();
		m_HTTP.DeleteSession();
		return false;
	}

	CFile file;
	CFileException ex;

	if(!file.Open((LPCTSTR)rszDownloadedFullPath,CFile::modeCreate | CFile::modeWrite, &ex))
	{
		m_HTTP.DeleteInternetFile();
		m_HTTP.DeleteSession();
		return false;	
	}

	DWORD write_file_size = 0;
	try
	{
		static char buf[4096]; 
		memset( buf, 0, sizeof(buf) );

		DWORD read_file_cnt = pIF->Read( buf, sizeof(buf) );
		while(read_file_cnt)
		{
			write_file_size += read_file_cnt;
			file.Write((LPCSTR)buf,read_file_cnt);

			memset( buf, 0, sizeof(buf) );
			read_file_cnt = pIF->Read( buf,sizeof(buf) );
		}

		file.Flush();
		file.Close();
	}
	catch( ... )
	{
		file.Close();
		m_HTTP.DeleteInternetFile();
		m_HTTP.DeleteSession();
		return false;
	}

	m_HTTP.DeleteInternetFile();
	m_HTTP.DeleteSession();
	rdwDownloadedSize = write_file_size;
	return true;
}
#ifdef STEAM_ATTACH
void CSetupAndStart::DownLoadLauncherFileAutoupgrade()
{
	if(strcmp(AfxGetApp()->m_lpCmdLine, "") == 0) //cmd가 없는 경우
	{
		char szExcutePath[MAX_PATH]="";
		::GetCurrentDirectory(MAX_PATH,szExcutePath);
		m_szAutoFileName = "";
		m_szAutoFileName.Format("%s\\autoupgrade.exe", szExcutePath);

		char CmdLine[1024] = "";
		memset(CmdLine, 0, sizeof(CmdLine));

		//임시용 인자~
		//StringCbPrintf(CmdLine, sizeof(CmdLine), " %s", "EDEW3940FVDP4950,10,20,30,1,autoupgrade_info_mgame.ini,0,0,1,365849202,?0?0?60172358915244,60176653882540?2013,11,29,0?");
		ExcuteProcess(m_szAutoFileName , CmdLine);
	}
}
#endif