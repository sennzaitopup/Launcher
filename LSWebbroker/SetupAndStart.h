#pragma once
#include "Util\HttpApp.h"

#define CMD_SECTION_TOKEN "?"
#define CMD_COMMA_TOKEN ","

#define REG_SHCU HKEY_CURRENT_USER

class CSetupAndStart
{
public:
	enum
	{
		OPTION_TYPE_AUTO_FILE_NAME = 0,
		OPTION_TYPE_SERVER_AUTO_VERSION,
		OPTION_TYPE_INI_FILE_NAME,
		OPTION_TYPE_SERVER_INI_CRC,
		OPTION_TYPE_AUTO_CREATE_FOLDER,
		OPTION_TYPE_AUTO_DOWNLOAD_URL,
		OPTION_TYPE_INI_CREATE_FOLDER,
		OPTION_TYPE_INI_DOWNLOAD_URL,
		OPTION_TYPE_FULLZIP_FILE_NAME,
		OPTION_TYPE_CHECK_VALUE,
		OPTION_TYPE_KEY_VALUE,
		OPTION_TYPE_OPTIONS,
		OPTION_TYPE_GAMESERVER_ID,
		OPTION_TYPE_FULLZIP_VER,
		OPTION_TYPE_READ_REG_CLIENT_PATH,
		OPTION_TYPE_UNKNOWN,
		OPTION_TYPE_AUTO_DOWNLOAD_EXTEND_URL,
		OPTION_TYPE_MAX,
	};
protected:
		// IsRightVersion
		CString m_szAutoFileName;
		CString m_szServerAutoCRC;

		// IsRightCRC
		CString m_szINIFileName;
		CString m_szServerINICRC;

		// auto setup
        CString m_szAutoCreateFolder;
		CString m_szAutoDownloadURL;
		CString m_szAutoDownloadExt;
		

		// ini setup
		CString m_szINICreateFolder;
		CString m_szINIDownloadURL;

		// full zip 
		CString m_szFullzipFileName;

		// Start
		CString m_szExpandValue;
		CString m_szKeyValue;
		CString m_szOptions;
		CString m_szGameServerID;
		CString m_szFullZipVer;

		CString m_szReadRegClientPath;

		CString m_szClientPath;

		CString m_szLocalType;

		HRESULT   m_hSetupReturnValue;

		HttpApp m_HTTP;

protected:
	bool IsRightCRC( LPCTSTR szFileName, LPCTSTR szServerCRC );
	bool IsRightFullZipVer( LPCTSTR szFileName, LPCTSTR szServerVersion );
#if STEAM_ATTACH
public :
	void ExcuteProcess( LPCTSTR szFileName , LPCTSTR szCmdLine);
#else
protected:
	void ExcuteProcess( LPCTSTR szFileName , LPCTSTR szCmdLine);
#endif
	
	//bool Setup( LPCTSTR szCreateFolder, LPCTSTR szDownLoadURL, LPCTSTR szCreateFileName ); 캐쉬문제로 사용하지 않음
#ifdef STEAM_ATTACH
public :
	bool DownLoadProcess( const CString &szURL, int iRemoteFileSize, const CString &rszDownloadedFullPath, OUT DWORD &rdwDownloadedSize );	
	void DownLoadLauncherFileAutoupgrade();
protected:
#else
	bool DownLoadProcess( const CString &szURL, int iRemoteFileSize, const CString &rszDownloadedFullPath, OUT DWORD &rdwDownloadedSize );	
#endif
	void ShowErrorDlg(const char* szErrorText, int iErrorNumber);

	void SetNewPathToRegPath();

public:
	bool ParseCmd( LPCTSTR szCmdLine );
	bool IsReadRegClientPath();

	void ReadClientPath( const char *szRegKeyIO );
	bool IsEmptyClientPath();

	bool SetupAndStartGame( bool bReadRegClientPath );

	void SetClientPath(CString &rszClientPath) { m_szClientPath = rszClientPath; }

	void SetLoginKey(LPCTSTR szKey) {m_szKeyValue = szKey;};

	void SetLocalType(LPCTSTR szLocal) { m_szLocalType = szLocal; }

public:
	CSetupAndStart(void);
	~CSetupAndStart(void);
};
