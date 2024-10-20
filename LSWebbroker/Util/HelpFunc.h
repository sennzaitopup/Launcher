#pragma once

namespace Help
{
	bool RegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nSize);
	bool RegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);
	bool RegWriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, int iData);
	void SetCreateDirectory( const char *szDir );
	void CreateDefaultSettingINI();
	void ExcuteWeb( const char *szURL );
	void ExcuteOtherWeb( const char *szURL );
	enum 
	{
		MAX_FILE_CONTROL_LOOP = 100,
	};
	bool DeleteFileRepeat(const char *szPath);

	void GetMyIP(OUT char* szMyIP, IN int iSize);
	void GetExternalIP(OUT char* szMyIP, IN int iSize);
	void CreateProcessWithInfo(const char* szFileName, char* szCmdLine);

	void LoadFileInResource(int name, const char * type, DWORD& size, const char*& data);
	void LoadTextInResource(int name, DWORD& size, const char*& data);
	int	 MessageboxShow(const char * pStr, const char * pCaption, UINT uType);
}