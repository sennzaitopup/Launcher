// ioErrorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LSWebBroker.h"
#include "ioErrorDlg.h"
#include "Util/SystemInfo.h"
#include "MainFrm.h"
#include "Util/HelpFunc.h"

#include ".\ioerrordlg.h"
#include <StrSafe.h>
#include "stringmanager\safesprintf.h"
#include "stringmanager\iostringmanager.h"
#include "Local\ioLocalParent.h"
#include "Local\ioLocalManager.h"


// ioErrorDlg 대화 상자입니다.

ioErrorDlg::ioErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialogSK(ioErrorDlg::IDD, pParent)
{
	m_iErrorNumber = 0;
}

ioErrorDlg::~ioErrorDlg()
{
}

void ioErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ERROR_EDIT, m_Edit);
	DDX_Control(pDX, ID_XBTN, m_XBtn);
	DDX_Control(pDX, ID_SOLUTION, m_SolutionBtn);
	DDX_Control(pDX, ID_EXIT_BTN, m_ExitBtn);
}


BEGIN_MESSAGE_MAP(ioErrorDlg, CDialogSK)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_SOLUTION, OnBnClickedSolution)
	ON_BN_CLICKED(ID_XBTN, OnBnClickedXbtn)
	ON_BN_CLICKED(ID_EXIT_BTN, OnBnClickedExitBtn)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// ioErrorDlg 메시지 처리기입니다.
BOOL ioErrorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_KEYDOWN &&pMsg->wParam == VK_RETURN 
	  ||pMsg->message == WM_KEYDOWN &&pMsg->wParam == VK_ESCAPE) 
		return FALSE;

	return CDialogSK::PreTranslateMessage(pMsg);
}

void ioErrorDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogSK::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(nStatus != 0) return;

	//----------창 중간에 띄우기 ---------------------
	RECT winrect, workrect;
	// Find how large the desktop work area is
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workrect, 0);
	int workwidth = workrect.right -  workrect.left;
	int workheight = workrect.bottom - workrect.top;

	// And how big the window is
	GetWindowRect(&winrect);
	int winwidth = winrect.right - winrect.left;
	int winheight = winrect.bottom - winrect.top;
	// Make sure it"s not bigger than the work area
	winwidth = min(winwidth, workwidth);
	winheight = min(winheight, workheight);

	// Now center it
	SetWindowPos( 
		/*NULL,*/&CWnd::wndTopMost, // 창 항상 위로 
		workrect.left + (workwidth-winwidth) / 2,
		workrect.top + (workheight-winheight) / 2,
		winwidth, winheight, 
		SWP_SHOWWINDOW);
	//----------------------------------------------------------
}

void ioErrorDlg::ShowErrorDlg( CString szErrorText, int iErrorNumber )
{
	m_iErrorNumber = iErrorNumber;

	CSystemInfo kSystemInfo;
	kSystemInfo.SetupSystemInfo();

	char szEditText[MAX_PATH*8]="";
	SafeSprintf( szEditText, sizeof(szEditText), STR(2) , (LPCTSTR)kSystemInfo.GetSystemInfo(),	(LPCTSTR)szErrorText, iErrorNumber );

	m_Edit.SetWindowText(szEditText);
	ShowWindow(SW_SHOW);
}


void ioErrorDlg::OnBnClickedSolution()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal )
	{
		char szURL[MAX_PATH]="";
		StringCbPrintf( szURL, sizeof(szURL), pLocal->GetErrorSolutionURL(), m_iErrorNumber );
		Help::ExcuteWeb( szURL );
	}
}

void ioErrorDlg::OnBnClickedXbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnClose();
}

void ioErrorDlg::OnBnClickedExitBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnClose();
}

void ioErrorDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//if(AfxGetApp())
	//{
	//	// 에러 메시지가 출력 되면 클라이언트를 강제 종료 하는 코드
	//	CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();
	//	if(pMainFrame)
	//	pMainFrame->SetWantExit();
	//}

	EndDialog(0);
}

HBRUSH ioErrorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogSK::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch(nCtlColor)
	{
		//case CTLCOLOR_EDIT: // edit 박스가 쓰기상태일때
	case CTLCOLOR_STATIC: // edit 박스가 read only 일때
		{
			if( pDC )
				pDC->SetTextColor( m_EditTextColor );

			if( pWnd && pWnd->GetDlgCtrlID() == IDC_ERROR_EDIT )
			{
				if( pDC )
					pDC->SetBkColor( m_EditBGColor );
				return m_EditBGBrush;
			}
		}
		break;
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

BOOL ioErrorDlg::OnInitDialog()
{
	CDialogSK::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	DWORD dwErrorBack     = IDB_ERROR_BACK;
	DWORD dwErrorSolution = IDB_SOLUTIONBTN;
	DWORD dwErrorExit     = IDB_EXITBTN;
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal )
	{
		dwErrorBack     = pLocal->GetResourceIDErrorBack();
		dwErrorSolution = pLocal->GetResourceIDErrorSolution();
		dwErrorExit     = pLocal->GetResourceIDErrorExit();
	}

	// skin
	EnableEasyMove();                    // enable moving of the dialog by clicking anywhere in the dialog
	SetBitmap(dwErrorBack);           // set background bitmap
	SetStyle(LO_RESIZE);                 // resize dialog to the size of the bitmap
	SetTransparentColor(RGB(0, 255, 0)); // set green as the transparent color

	m_XBtn.LoadBitmap(IDB_XBTN);
	m_XBtn.MoveWindow(380, 5, 25, 26);

	if( dwErrorSolution == -1 )
	{
		m_SolutionBtn.EnableWindow( FALSE );
		m_SolutionBtn.ShowWindow( SW_HIDE );
	}
	else
	{
		m_SolutionBtn.LoadBitmap(dwErrorSolution);
		m_SolutionBtn.MoveWindow(17, 252, 286, 31);
	}

	m_ExitBtn.LoadBitmap(dwErrorExit);
	m_ExitBtn.MoveWindow(305, 252, 88, 31);

	m_Edit.MoveWindow(18, 47, 376, 191);
	m_EditBGColor   = RGB(235, 235, 235);
	m_EditTextColor = RGB(58,58,58);
	m_EditBGBrush.CreateSolidBrush( m_EditBGColor );

	char szFontName[MAX_PATH] = "돋움";
	if( pLocal )
		StringCbCopy( szFontName, sizeof( szFontName ), pLocal->GetFontName() );
	m_EditFont.CreatePointFont( 100, szFontName );
	m_Edit.SetFont( &m_EditFont );
	//

	SetWindowText( "Lost Saga" ); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
