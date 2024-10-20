// HttpApp.cpp: implementation of the HttpApp class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
//#include "AutoUpgrade.h"
#include "HttpApp.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HttpApp::HttpApp()
{
	m_pSession = NULL;
	m_pInternetFile = NULL;
}

HttpApp::~HttpApp()
{
	DeleteInternetFile();
	DeleteSession();
}

BOOL HttpApp::OpenUrl( const CString &rszURL, OUT CString &rszErrorMessage )
{
	try
	{
		DeleteSession();

		m_pSession = new CInternetSession;

		if( !m_pSession )
			return FALSE;

		m_pInternetFile = (CInternetFile*)m_pSession->OpenURL((LPCTSTR)rszURL, 1,
			                INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD | INTERNET_FLAG_TRANSFER_BINARY);
	}
	catch(CInternetException *pException)
	{
		m_pInternetFile = NULL;
		if( pException )
		{
			TCHAR sz[1024];
			pException->GetErrorMessage(sz, 1024);
			pException->Delete();
			rszErrorMessage = sz;
		}
		return FALSE;
	}
	return TRUE;
}

DWORD HttpApp::GetFileSize()
{
	if(m_pInternetFile == NULL) return 0;
	
	CHttpFile* pHttpFile = (CHttpFile*)m_pInternetFile;
	char strContentLength[50];
	memset( strContentLength, 0 , sizeof(strContentLength) );
	DWORD dwSize = 50;
	if( pHttpFile)
		pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, strContentLength, &dwSize);
	DWORD dwFileSize = atol(strContentLength);			

	return dwFileSize;
}

void HttpApp::DeleteSession()
{
	if(m_pSession)
	{
		delete m_pSession;
		m_pSession = NULL;
	}
}

void HttpApp::DeleteInternetFile()
{
	if(m_pInternetFile)
	{
		delete m_pInternetFile;
		m_pInternetFile = NULL;
	}
}