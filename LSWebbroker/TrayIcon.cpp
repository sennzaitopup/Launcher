// TrayIcon.cpp: implementation of the CTrayIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrayIcon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrayIcon::CTrayIcon()
{
	memset(&m_tnd, 0, sizeof(m_tnd));
}

CTrayIcon::CTrayIcon(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szToolTip, 
					 HICON icon, UINT uID)
{
	Create(pWnd, uCallbackMessage, szToolTip, icon, uID);
}

BOOL CTrayIcon::Create(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szToolTip, 
					   HICON icon, UINT uID)
{
	//Make sure we avoid conflict with other messages
	if( !(uCallbackMessage >= WM_USER) )
		return false;

	//Tray only supports tooltip text up to 64 characters
	if( !(_tcslen(szToolTip) <= 64) )
		return false;

	// NOTIFYICONDATA 구조체의 값 설정
	m_tnd.cbSize = sizeof(NOTIFYICONDATA);
	m_tnd.hWnd	 = pWnd->GetSafeHwnd();
	m_tnd.uID	 = uID;
	m_tnd.hIcon  = icon;
	m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_tnd.uCallbackMessage = uCallbackMessage;
	strcpy (m_tnd.szTip, szToolTip);

	// 트레이 아이콘 추가
	return Shell_NotifyIcon(NIM_ADD, &m_tnd);
}

CTrayIcon::~CTrayIcon()
{
	RemoveIcon();
}


/////////////////////////////////////////////////////////////////////////////
// CTrayIcon icon manipulation

void CTrayIcon::RemoveIcon()
{
	m_tnd.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &m_tnd);
}


BOOL CTrayIcon::SetIcon(HICON hIcon)
{
	m_tnd.uFlags = NIF_ICON;
	m_tnd.hIcon = hIcon;

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL CTrayIcon::SetIcon(LPCTSTR lpszIconName)
{
	HICON hIcon = AfxGetApp()->LoadIcon(lpszIconName);

	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetIcon(UINT nIDResource)
{
	HICON hIcon = AfxGetApp()->LoadIcon(nIDResource);

	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetStandardIcon(LPCTSTR lpIconName)
{
	HICON hIcon = LoadIcon(NULL, lpIconName);

	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetStandardIcon(UINT nIDResource)
{
	HICON hIcon = LoadIcon(NULL, MAKEINTRESOURCE(nIDResource));

	return SetIcon(hIcon);
}
 
HICON CTrayIcon::GetIcon() const
{
	HICON hIcon = NULL;
	hIcon = m_tnd.hIcon;

	return hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon tooltip text manipulation

BOOL CTrayIcon::SetTooltipText(LPCTSTR pszTip)
{
	m_tnd.uFlags = NIF_TIP;
	_tcscpy(m_tnd.szTip, pszTip);

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL CTrayIcon::SetTooltipText(UINT nID)
{
	CString strText;
	if(!strText.LoadString(nID))
		return false;

	return SetTooltipText(strText);
}

CString CTrayIcon::GetTooltipText() const
{
	CString strText;
	strText = m_tnd.szTip;

	return strText;
}

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon notification window stuff

BOOL CTrayIcon::SetNotificationWnd(CWnd* pWnd)
{
	//Make sure Notification window is valid
	if( !(pWnd && ::IsWindow(pWnd->GetSafeHwnd())) )
		return false;
		

	m_tnd.hWnd = pWnd->GetSafeHwnd();
	m_tnd.uFlags = 0;

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

CWnd* CTrayIcon::GetNotificationWnd() const
{
	return CWnd::FromHandle(m_tnd.hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon implentation of OnTrayNotification

