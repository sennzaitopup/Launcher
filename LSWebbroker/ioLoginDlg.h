#pragma once
#include "afxwin.h"

// ioLoginDlg 대화 상자입니다.
#include "skin/CDialogSK.h"
#include "skin/HoverButton.h"

class ioLoginDlg : public CDialogSK
{
public:
	ioLoginDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ioLoginDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	CHoverButton m_XBtn;
	CHoverButton m_IDSaveBtn;
	CHoverButton m_LoginBtn;
	CHoverButton m_EntryBtn;
	CHoverButton m_FindIDBtn;
	CHoverButton m_FindPWBtn;
	CEdit m_IDEdit;
	CEdit m_PWEdit;

	CBrush       m_EditSelectBGBrush;
	COLORREF     m_EditSelectBGColor;

	CBrush       m_EditNormalBGBrush;
	COLORREF     m_EditNormalBGColor;

	COLORREF     m_EditTextColor;
	COLORREF     m_EditNoticeTextColor;

	CFont        m_Font;
	CBitmap      m_IDEditNormalBitmap;
	CBitmap      m_IDEditSelectBitmap;

	CBitmap      m_PWEditNormalBitmap;
	CBitmap      m_PWEditSelectBitmap;

	CString      m_sLoginURL;
	CString      m_sEntryURL;
	CString      m_sFindIDURL;
	CString      m_sFindPWURL;
	bool         m_bIDEditBlank;
	bool         m_bPWEditBlank;
	bool         m_bSelectIDEdit;
	bool         m_bSelectPWEdit;

	RECT		 m_rcBackImg;
	CString		 m_sOfficialURL;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedIdsave();
	afx_msg void OnBnClickedLoginX();
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedEntry();
	afx_msg void OnBnClickedFind();
	afx_msg void OnBnClickedLoginFindPw();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

public:
	void LoadINI();

protected:
	void DrawBitmap( CPaintDC *pDC, CBitmap *pBitmap, int iX, int iY, int iWidth, int iHeight );
	void SendLoginAuth(const char* szID, const char* szPW, const char* szURL);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:

	CListBox	m_ChannelListBox;
	CHoverButton m_ChannelBtn;

	CBitmap     m_NexonBitmap;
	CBitmap		m_DaumBitamp;
	CBitmap		m_LostSagaBitamp;
	CBitmap		m_mGameBitamp;
	CBitmap		m_TooniBitamp;
	CBitmap		m_NaverBitmap;

	afx_msg void OnBnClickedChannel();
	afx_msg void OnLbnSelchangeList();

	CHoverButton m_NaverButton;
	CHoverButton m_MgameButton;
	CHoverButton m_LostSagaButton;
	CHoverButton m_NexonButton;
	CHoverButton m_TooniButton;
	CHoverButton m_DaumButton;
private :
	CToolTipCtrl m_ToolTipCtrl;
};
