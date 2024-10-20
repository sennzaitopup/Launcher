// TrayIcon.h: interface for the CTrayIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAYICON_H__776BD1A9_F764_11D1_994B_444553540000__INCLUDED_)
#define AFX_TRAYICON_H__776BD1A9_F764_11D1_994B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTrayIcon
{
// Construction/destruction
public:
	CTrayIcon();
	CTrayIcon(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);
	virtual ~CTrayIcon();

	//Create the tray icon
	BOOL Create(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);

	//Change or retrieve the Tooltip text
	BOOL    SetTooltipText(LPCTSTR pszTooltipText);
	BOOL    SetTooltipText(UINT nID);
	CString GetTooltipText() const;

	//Change or retrieve the icon displayed
	BOOL  SetIcon(HICON hIcon);
	BOOL  SetIcon(LPCTSTR lpIconName);
	BOOL  SetIcon(UINT nIDResource);
	BOOL  SetStandardIcon(LPCTSTR lpIconName);
	BOOL  SetStandardIcon(UINT nIDResource);
	HICON GetIcon() const;
	void  RemoveIcon();

	//Change or retrieve the window to send notification messages to
	BOOL  SetNotificationWnd(CWnd* pNotifyWnd);
	CWnd* GetNotificationWnd() const;

// Implementation
protected:
	NOTIFYICONDATA	m_tnd;
};


#endif // !defined(AFX_TRAYICON_H__776BD1A9_F764_11D1_994B_444553540000__INCLUDED_)
