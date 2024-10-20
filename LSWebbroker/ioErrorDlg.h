#pragma once
#include "afxwin.h"
#include "resource.h"

// ioErrorDlg 대화 상자입니다.
#include "skin/CDialogSK.h"
#include "skin/HoverButton.h"

class ioErrorDlg : public CDialogSK
{
public:
	ioErrorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ioErrorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IOERRORDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit        m_Edit;
	CBrush       m_EditBGBrush;
	COLORREF     m_EditBGColor;
	COLORREF     m_EditTextColor;

	CHoverButton m_XBtn;
	CHoverButton m_SolutionBtn;
	CHoverButton m_ExitBtn;
	int          m_iErrorNumber;
	CFont        m_EditFont;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ShowErrorDlg(CString szErrorText, int iErrorNumber );
	afx_msg void OnBnClickedSolution();
	afx_msg void OnBnClickedXbtn();
	afx_msg void OnBnClickedExitBtn();
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
