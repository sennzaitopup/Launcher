// ioLocalDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "LSWebBroker.h"
#include "ioLocalDlg.h"
#include "Util/SystemInfo.h"
#include "MainFrm.h"
#include "Util/HelpFunc.h"

#include ".\ioLocalDlg.h"
#include <StrSafe.h>
#include "stringmanager\safesprintf.h"
#include "stringmanager\iostringmanager.h"
#include "Local\ioLocalParent.h"
#include "Local\ioLocalManager.h"
#include "Util\ioINILoaderAU.h"
#include "Util\HelpFunc.h"

// ioLocalDlg 대화 상자입니다.

ioLocalDlg::ioLocalDlg(CWnd* pParent /*=NULL*/)
	: CDialogSK(ioLocalDlg::IDD, pParent)
{
	
}

ioLocalDlg::~ioLocalDlg()
{
}

void ioLocalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON7, m_XBtn);
	DDX_Control(pDX, IDC_BUTTON1, m_English);
	DDX_Control(pDX, IDC_BUTTON2, m_Germany);
//	DDX_Control(pDX, IDC_BUTTON3, m_French);
	DDX_Control(pDX, IDC_BUTTON4, m_Italian);
//	DDX_Control(pDX, IDC_BUTTON5, m_Polish);
//	DDX_Control(pDX, IDC_BUTTON6, m_Turkey);
	DDX_Control(pDX, IDOK, m_LocalStart);
}


BEGIN_MESSAGE_MAP(ioLocalDlg, CDialogSK)

	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &ioLocalDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON7, &ioLocalDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &ioLocalDlg::OnBnClickedButtonEnglish)
	ON_BN_CLICKED(IDC_BUTTON2, &ioLocalDlg::OnBnClickedButtonGermany)
//	ON_BN_CLICKED(IDC_BUTTON3, &ioLocalDlg::OnBnClickedButtonFrench)
//	ON_BN_CLICKED(IDC_BUTTON5, &ioLocalDlg::OnBnClickedButtonPolish)
	ON_BN_CLICKED(IDC_BUTTON4, &ioLocalDlg::OnBnClickedButtonItalian)
//	ON_BN_CLICKED(IDC_BUTTON6, &ioLocalDlg::OnBnClickedButtonTurkey)
END_MESSAGE_MAP()



BOOL ioLocalDlg::OnInitDialog()
{
	CDialogSK::OnInitDialog();

	EnableEasyMove();
/*
	m_LocalList.AddString("English");
	m_LocalList.AddString("Germany");
	m_LocalList.AddString("French");
	m_LocalList.AddString("Italian");
	m_LocalList.AddString("Polish");
*/
#ifdef SRC_EU
	SetBitmap( IDB_BACK_LOCAL_SMALL );         // set background bitmap
#elif defined (SRC_SEA)
	SetBitmap( IDB_BACK_LOCAL_SMALL );         // set background bitmap
#else
	SetBitmap( IDB_BACK_LOCAL );         // set background bitmap
#endif
	
	SetStyle(LO_RESIZE);                 // resize dialog to the size of the bitmap

	m_XBtn.LoadBitmap(IDB_XBTN , FALSE);
	m_XBtn.MoveWindow(311, 5, 24, 26);
	m_XBtn.ShowWindow(SW_SHOW);
	//*/

	//IDB_LOCAL_START
	m_LocalStart.LoadBitmap(IDB_LOCAL_START);
	m_LocalStart.MoveWindow(15,200,311, 29);
	m_LocalStart.ShowWindow(SW_SHOW);

	m_English.LoadBitmap(IDB_LOCAL_ENGLISH);
	m_English.MoveWindow(19, 70, 95, 82);
	m_English.ShowWindow(SW_SHOW);
	m_English.SetCheckBtn(true);

	m_Germany.LoadBitmap(IDB_LOCAL_GERMANY);
	m_Germany.MoveWindow(123, 70, 95, 82);
	m_Germany.ShowWindow(SW_SHOW);
	m_Germany.SetCheckBtn(true);

// 	m_French.LoadBitmap(IDB_LOCAL_FRANCE);
// 	m_French.MoveWindow(19, 161, 95, 82);
// 	m_French.ShowWindow(SW_SHOW);
// 	m_French.SetCheckBtn(true);

	m_Italian.LoadBitmap(IDB_LOCAL_ITALIAN);
	m_Italian.MoveWindow(227, 70, 95, 82);
	m_Italian.ShowWindow(SW_SHOW);
	m_Italian.SetCheckBtn(true);

// 	m_Polish.LoadBitmap(IDB_LOCAL_POLISH);
// 	m_Polish.MoveWindow(123, 161, 95, 82);
// 	m_Polish.ShowWindow(SW_SHOW);
// 	m_Polish.SetCheckBtn(true);

// 	m_Turkey.LoadBitmap(IDB_LOCAL_TURKEY);
// 	m_Turkey.MoveWindow(227, 161, 95, 82);
// 	m_Turkey.ShowWindow(SW_SHOW);
// 	m_Turkey.SetCheckBtn(true);

	m_XBtn.ShowWindow(SW_SHOW);
	m_English.ShowWindow(SW_SHOW);
	m_Germany.ShowWindow(SW_SHOW);
//	m_French.ShowWindow(SW_SHOW);
 	m_Italian.ShowWindow(SW_SHOW);
// 	m_Polish.ShowWindow(SW_SHOW);


	SetTransparentColor(RGB(0, 255, 0)); // set green as the transparent color

	m_nLocalCode = 0;

	return TRUE;
}

void ioLocalDlg::LoadINI()
{
	char szBuf[MAX_PATH*2]="";

	ioINILoaderAU kIDLoader("Save\\default\\setting.ini");
	kIDLoader.SetTitle("Setting");
	kIDLoader.LoadString( "LocalType", "", szBuf, sizeof( szBuf ) );
	if( strcmp( szBuf, "" ) != 0 )
	{
		int nSel = atoi(szBuf);
#ifdef SRC_EU //3,5,6
		if( nSel == 2 || nSel == 4 || nSel == 5 )
			nSel = 0; //이제 지원하지 않는 파일

#elif defined SRC_SEA
		if( nSel == 2 || nSel == 4 || nSel == 5 )
			nSel = 0; //이제 지원하지 않는 파일
#endif
		SetCheckedLocalBtn(nSel);
		//m_LocalList.SetCurSel(nSel);
		//SendMessage(WM_COMMAND, MAKEWPARAM(IDC_LIST1, LBN_SELCHANGE) , (LPARAM)(m_LocalList.GetSafeHwnd()));
	}



	ShowWindow(SW_SHOW);
}


void ioLocalDlg::OnClose()
{
	EndDialog(0);

	if(AfxGetApp())
	{
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();
		if(pMainFrame)
			pMainFrame->SetWantExit();
	}
}

void ioLocalDlg::OnBnClickedOk()
{
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( !pLocal )
		return;
	if(!AfxGetApp())
		return;

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();
	if(!pMainFrame)
		return;

	Help::CreateDefaultSettingINI();
	ioINILoaderAU kIniLoder("Save\\default\\setting.ini");
	kIniLoder.SetTitle("Setting");

	int nSel = m_nLocalCode;
	char szBuf[MAX_PATH];
	itoa(nSel, szBuf, 10 );
	kIniLoder.SaveString( "LocalType", (LPCTSTR) szBuf );

	pMainFrame->SetSubLocalType(nSel);
	pMainFrame->ParseCmdAndStart( pLocal, szBuf );
}


void ioLocalDlg::OnBnClickedCancel()
{
	OnClose();
}

void ioLocalDlg::SetCheckedLocalBtn(int nLocalCode)
{
	switch( nLocalCode )
	{
	case 0:
		OnBnClickedButtonEnglish();
		break;
	case 1:
		OnBnClickedButtonGermany();
		break;
	case 2:
		OnBnClickedButtonFrench();
		break;
	case 3:
		OnBnClickedButtonItalian();
		break;
	case 4:
		OnBnClickedButtonPolish();
		break;
	case 5:
		OnBnClickedButtonTurkey();
		break;
	}
}

void ioLocalDlg::OnBnClickedButtonEnglish()
{
	m_nLocalCode = 0;

	m_English.SetChecked(true);
	m_English.SetCheckBtn(true);

	m_Germany.SetCheckBtn(false);
//	m_French.SetCheckBtn(false);
	m_Italian.SetCheckBtn(false);
//	m_Polish.SetCheckBtn(false);
//	m_Turkey.SetCheckBtn(false);
	Invalidate(true);
}


void ioLocalDlg::OnBnClickedButtonGermany()
{
	m_nLocalCode = 1;

	m_English.SetCheckBtn(false);

	m_Germany.SetCheckBtn(true);
	m_Germany.SetChecked(true);

//	m_French.SetCheckBtn(false);
	m_Italian.SetCheckBtn(false);
//	m_Polish.SetCheckBtn(false);
//	m_Turkey.SetCheckBtn(false);

	Invalidate(true);
}


void ioLocalDlg::OnBnClickedButtonFrench()
{
	
	m_nLocalCode = 0; //언어 제거 : Default : English

	m_English.SetCheckBtn(false);
	m_Germany.SetCheckBtn(false);

// 	m_French.SetCheckBtn(true);
// 	m_French.SetChecked(true);

	m_Italian.SetCheckBtn(false);
// 	m_Polish.SetCheckBtn(false);
// 	m_Turkey.SetCheckBtn(false);
	Invalidate(true);
}




void ioLocalDlg::OnBnClickedButtonItalian()
{
	m_nLocalCode = 3; 

	m_English.SetCheckBtn(false);
	m_Germany.SetCheckBtn(false);
//	m_French.SetCheckBtn(false);

	m_Italian.SetCheckBtn(true);
	m_Italian.SetChecked(true);

//	m_Polish.SetCheckBtn(false);

	Invalidate(true);
}

void ioLocalDlg::OnBnClickedButtonPolish()
{
	m_nLocalCode = 0; //언어 제거 : Default : English

	m_English.SetCheckBtn(false);
	m_Germany.SetCheckBtn(false);
//	m_French.SetCheckBtn(false);
	m_Italian.SetCheckBtn(false);

//	m_Polish.SetCheckBtn(true);
//	m_Polish.SetChecked(true);
//	m_Turkey.SetCheckBtn(false);
	Invalidate(true);
}

void ioLocalDlg::OnBnClickedButtonTurkey()
{
	m_nLocalCode = 0; //언어 제거 : Default : English

	m_English.SetCheckBtn(false);
	m_Germany.SetCheckBtn(false);
//	m_French.SetCheckBtn(false);
	m_Italian.SetCheckBtn(false);
//	m_Polish.SetCheckBtn(false);

//	m_Turkey.SetChecked(true);
//	m_Turkey.SetCheckBtn(true);
	Invalidate(true);
}
