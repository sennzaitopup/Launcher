// HttpApp.h: interface for the HttpApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPAPP_H__66D20AB3_8696_4580_8399_BAD9C0CB52D9__INCLUDED_)
#define AFX_HTTPAPP_H__66D20AB3_8696_4580_8399_BAD9C0CB52D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxinet.h>

class HttpApp  
{
	CInternetSession *m_pSession;
	CInternetFile    *m_pInternetFile;
	
	public:
	BOOL OpenUrl(const CString &rszURL, OUT CString &rszErrorMessage);
	void DeleteSession();

	public:
	CInternetFile    *GetInternetFile(){ return m_pInternetFile; }
	DWORD GetFileSize();
	void DeleteInternetFile();

	public:
	HttpApp();
	virtual ~HttpApp();

};

#endif // !defined(AFX_HTTPAPP_H__66D20AB3_8696_4580_8399_BAD9C0CB52D9__INCLUDED_)
