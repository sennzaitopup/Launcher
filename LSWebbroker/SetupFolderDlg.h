#pragma once

// SetupFolderDlg 대화 상자입니다.
#include "skin/CDialogSK.h"
#include "skin/HoverButton.h"
#include "afxwin.h"

#define DEFAULT_PATH "\\Program Files\\LostSaga"

class SetupFolderDlg : public CDialogSK
{
public:
	SetupFolderDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~SetupFolderDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_FOLDER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	CHoverButton m_SetupBtn;
	CHoverButton m_ExitBtn;
	CHoverButton m_ChangeBtn;
	CHoverButton m_XBtn;

	CEdit        m_Edit;
	CBrush       m_EditBGBrush;
	COLORREF     m_EditBGColor;
	COLORREF     m_EditTextColor;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChangeFolder();
	afx_msg void OnBnClickedSetup();
	afx_msg void OnBnClickedExit();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

protected:
	enum
	{
		DRIVE_ONLY_SIZE = 3, 
	};
protected:
	CString m_szClientPath;
protected:
	void GetPath( OUT char *szPath, int size, const char*szTitle );
	bool CreateSetupDirectory( const char *szPath );
	static int CALLBACK BrowseCallbackProc(HWND hWnd,UINT uMsg,LPARAM lp, LPARAM pData);
public:
	afx_msg void OnClose();
};
