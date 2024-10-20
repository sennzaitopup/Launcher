#include "../StdAfx.h"
#include "HelpFunc.h"
#include <strsafe.h>
#include "atlstr.h"
#include <string>
#include <tlhelp32.h> 


#include <wininet.h>
#pragma comment(lib, "wininet")

namespace Help
{
	bool RegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, 
		LPTSTR lpRet, DWORD nSize) 
	{
		HKEY key;
		DWORD dwDisp;
		if (RegCreateKeyEx(hKey, lpKey,0,NULL,REG_OPTION_NON_VOLATILE, KEY_READ,NULL,&key,&dwDisp)!=ERROR_SUCCESS) 
		{
			return false;
		}

		DWORD dwSize = nSize;
		if (RegQueryValueEx(key, lpValue, 0, NULL,(LPBYTE)lpRet, &dwSize)!=ERROR_SUCCESS) 
		{
			StringCbCopy(lpRet, nSize, lpDefault);
			return false;
		}

		RegCloseKey(key);   
		return true;
	}

	bool RegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
	{
		HKEY key;
		DWORD dwDisp;
		if (RegCreateKeyEx(hKey, lpKey,0,NULL,REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp)!=ERROR_SUCCESS) 
			return false;

		if (RegSetValueEx(key, lpValue,0,REG_SZ,(LPBYTE)lpData,(DWORD)strlen(lpData)+1)!=ERROR_SUCCESS) 
			return false;

		RegCloseKey(key);
		return true;
	}

	bool RegWriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, int iData)
	{
		HKEY key;
		DWORD dwDisp;
		if (RegCreateKeyEx(hKey, lpKey,0,NULL,REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp)!=ERROR_SUCCESS) 
			return false;

		if (RegSetValueEx(key, lpValue,0,REG_DWORD,(LPBYTE)&iData,sizeof(int))!=ERROR_SUCCESS) 
			return false;

		RegCloseKey(key);
		return true;
	}

	void SetCreateDirectory( const char *szDir )
	{
		if(szDir == NULL) return;

		const char *pDir = szDir;
		char createDir[MAX_PATH] = "";
		int len = 0;
		while (*pDir != '\0') 
		{
			if( *pDir=='/' || 
				*pDir=='\\' )
			{
				CreateDirectory(createDir,NULL);			
			}
			if(len >= MAX_PATH)
				break;
			createDir[len++] = *pDir;
			pDir++;
		}
		CreateDirectory(createDir,NULL);
	}

	void CreateDefaultSettingINI()
	{
		char fName[MAX_PATH]="";
		StringCbCopy( fName, sizeof(fName), "Save\\default\\setting.ini" );
		FILE *fp = fopen( fName, "r" );
		if( !fp )
		{
			CreateDirectory("Save",NULL);
			ZeroMemory(fName, sizeof(fName));
			StringCbCopy( fName, sizeof(fName), "Save\\default" );
			CreateDirectory(fName,NULL);
			ZeroMemory(fName, sizeof(fName));
			StringCbCopy( fName, sizeof(fName), "Save\\default\\setting.ini" );

			fp = fopen( fName, "w" );
			if( !fp )
				return;
		}
		fclose( fp );
	}

	void ExcuteWeb( const char *szURL )
	{
		PROCESS_INFORMATION process_info;
		ZeroMemory( &process_info, sizeof(process_info) );	
		STARTUPINFO si;
		ZeroMemory( &si, sizeof(si) );				
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = 0;

		char ProcessName[1024]="";
		Help::RegReadString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE","","",ProcessName,sizeof(ProcessName));
		char ExecProcess[1024]="";
		StringCbCopy(ExecProcess,sizeof(ExecProcess), ProcessName);
		StringCbCat(ExecProcess,sizeof(ExecProcess), " ");
		StringCbCat(ExecProcess,sizeof(ExecProcess), szURL);  //검색내용

		if(CreateProcess(NULL,ExecProcess,NULL,NULL,FALSE,0,NULL,NULL,&si,&process_info))
			WaitForInputIdle(process_info.hProcess,10000);
	}

	void ExcuteOtherWeb( const char *szURL )
	{

		IWebBrowser2* pWebBrowser;

		CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pWebBrowser);

		CString strHeader, strTarget, strUrl; 
		strHeader = "Content-Type: application/x-www-form-urlencoded\r\n"; 
		strTarget = "_top"; // 새로운 창으로 띄움
		strUrl = szURL;

		VARIANT vtHeader = {0}, vtTarget= {0}, vtEmpty= {0}, vtPostData = {0}; 

		vtHeader.vt = VT_BSTR; 
		vtHeader.bstrVal = strHeader.AllocSysString();
		vtTarget.vt = VT_BSTR; 
		vtTarget.bstrVal = strTarget.AllocSysString();

		::VariantInit(&vtEmpty);

		pWebBrowser->put_Left(200); 
		pWebBrowser->put_Top(300); 
		pWebBrowser->put_Width(388); 
		pWebBrowser->put_Height(440);
		

		VariantInit( &vtEmpty );
		VariantInit( &vtPostData );
		VariantInit( &vtHeader );

		pWebBrowser->put_Visible(VARIANT_TRUE); 
		pWebBrowser->put_ToolBar(VARIANT_FALSE); 
		pWebBrowser->put_MenuBar(VARIANT_FALSE);
		pWebBrowser->put_AddressBar(VARIANT_FALSE);
		pWebBrowser->put_StatusBar(VARIANT_FALSE);
		pWebBrowser->put_Resizable(VARIANT_FALSE);

		pWebBrowser->Navigate( strUrl.AllocSysString(), &vtEmpty, &vtEmpty, &vtPostData, &vtHeader );

		HWND hIE;
		pWebBrowser->get_HWND((long *)&hIE); 
		::SetWindowPos(hIE, HWND_TOPMOST, 0, 0,0 ,0 , SWP_NOMOVE | SWP_NOSIZE);

	}

	bool DeleteFileRepeat(const char *szPath) 
	{
		for(int i = 0 ; i < MAX_FILE_CONTROL_LOOP ; i++) 
		{
			DWORD dwReturn = GetFileAttributes(szPath);
			if( dwReturn == FILE_ATTRIBUTE_DIRECTORY)
				return true;
			if( dwReturn == INVALID_FILE_ATTRIBUTES) // 파일 없다. 
				return true;
			if( DeleteFile(szPath) != 0) // 성공 
				return true;

			SetFileAttributes( szPath, FILE_ATTRIBUTE_ARCHIVE ); // 보통 속성으로 변경 
			Sleep(1);
		}
		return false;
	}

	void GetMyIP(OUT char* szMyIP, IN int iSize)
	{
		char szHost[MAX_PATH + 1] = "";  // Ensure enough space for null terminator
		if (gethostname(szHost, MAX_PATH) == 0)  // Check if gethostname succeeds
		{
			HOSTENT* pHost = gethostbyname(szHost);
			if (!pHost)
			{
				StringCbPrintf(szMyIP, iSize, "%d.%d.%d.%d", 127, 0, 0, 1);
			}
			else
			{
				StringCbPrintf(szMyIP, iSize, "%d.%d.%d.%d", (BYTE)pHost->h_addr_list[0][0],
					(BYTE)pHost->h_addr_list[0][1],
					(BYTE)pHost->h_addr_list[0][2],
					(BYTE)pHost->h_addr_list[0][3]);
			}
		}
		else
		{
			// Handle error when gethostname fails
			StringCbPrintf(szMyIP, iSize, "%d.%d.%d.%d", 127, 0, 0, 1);
		}
	}


	void GetExternalIP(OUT char* szMyIP, IN int iSize)
	{
		HINTERNET hInternet, hFile;
		DWORD rSize;
		char buffer[32];

		hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		hFile = InternetOpenUrl(hInternet, "http://api.ipify.org/", NULL, 0, INTERNET_FLAG_RELOAD, 0);
		InternetReadFile(hFile, &buffer, sizeof(buffer), &rSize);
		buffer[rSize] = '\0';

		StringCbPrintf(szMyIP, iSize, "%s", buffer);
		InternetCloseHandle(hFile);
		InternetCloseHandle(hInternet);
	}

	void CreateProcessWithInfo(const char* szFileName, char* szCmdLine)
	{
		PROCESS_INFORMATION ProcessInfo;
		STARTUPINFO         StartupInfo;
		StartupInfo.cb = sizeof(STARTUPINFO);
		StartupInfo.lpReserved = NULL;
		StartupInfo.lpDesktop = NULL;
		StartupInfo.lpTitle = NULL;
		StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
		StartupInfo.wShowWindow = SW_SHOWNORMAL;
		StartupInfo.cbReserved2 = 0;
		StartupInfo.lpReserved2 = NULL;

		if (strcmp(szCmdLine, " ")) //앞에 공백이 없으면 명령인자가 제대로 전달되지 않습니다.
		{
			char newCmd[256] = { 0, };
			sprintf(newCmd, " %s", szCmdLine);
			CreateProcess(szFileName, (LPSTR)newCmd, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);
		}

	}

	void LoadFileInResource(int name, const char * type, DWORD& size, const char*& data)
	{
		HMODULE handle = ::GetModuleHandle(NULL);
		HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(name), type);
		HGLOBAL rcData = ::LoadResource(handle, rc);
		size = ::SizeofResource(handle, rc);
		data = static_cast<const char*>(::LockResource(rcData));
	}

	void LoadTextInResource( int name, DWORD& size, const char*& data )
	{
		LoadFileInResource(name, TEXT_RESOURCE, size, data);
	}

	int MessageboxShow(const char * pStr, const char * pCaption, UINT uType)
	{
		if(pStr && pCaption)
		{
#ifdef MULTI_BYTE_CHECK
			std::wstring strText = CA2W(pStr, COUNTRY_CODE_PAGE);
			std::wstring strCaption = CA2W(pCaption, COUNTRY_CODE_PAGE);
			return MessageBoxW( NULL, strText.c_str(), strCaption.c_str(), uType);
#else
			return MessageBox( NULL, pStr, pCaption, uType);
#endif
		}
		else
		{
			return MessageBox( NULL, "Unknown Error", "Unknown Error", uType);
		}
	}

} //namespace Help