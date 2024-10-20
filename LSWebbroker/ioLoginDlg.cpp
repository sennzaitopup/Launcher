// ioLoginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LSWebBroker.h"
#include "ioLoginDlg.h"
#include ".\iologindlg.h"
#include "MainFrm.h"
#include "Util\ioINILoaderAU.h"
#include "Util\HelpFunc.h"
#include "Local\ioLocalParent.h"

// ioLoginDlg 대화 상자입니다.

ioLoginDlg::ioLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogSK(ioLoginDlg::IDD, pParent)
{
	m_bIDEditBlank = false;
	m_bPWEditBlank = false;
	m_bSelectIDEdit = false;
	m_bSelectPWEdit = false;

	m_rcBackImg.bottom	= 0;
	m_rcBackImg.left	= 0;
	m_rcBackImg.right	= 0;
	m_rcBackImg.top		= 0;
}

ioLoginDlg::~ioLoginDlg()
{
	m_IDEditNormalBitmap.DeleteObject();
	m_IDEditSelectBitmap.DeleteObject();
	m_PWEditNormalBitmap.DeleteObject();
	m_PWEditSelectBitmap.DeleteObject();
	m_EditNormalBGBrush.DeleteObject();
	m_EditSelectBGBrush.DeleteObject();
}

void ioLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGIN_X, m_XBtn);
	DDX_Control(pDX, IDC_IDSAVE, m_IDSaveBtn);
	DDX_Control(pDX, IDC_LOGIN, m_LoginBtn);
	DDX_Control(pDX, IDC_ENTRY, m_EntryBtn);
	DDX_Control(pDX, IDC_FIND, m_FindIDBtn);
	DDX_Control(pDX, IDC_EDIT_ID, m_IDEdit);
	DDX_Control(pDX, IDC_EDIT_PW, m_PWEdit);
	DDX_Control(pDX, IDC_LOGIN_FIND_PW, m_FindPWBtn);
	DDX_Control(pDX, IDC_LIST1, m_ChannelListBox);
	DDX_Control(pDX, IDC_CHANNEL, m_ChannelBtn);
	DDX_Control(pDX, IDC_NAVER, m_NaverButton);
	DDX_Control(pDX, IDC_MGAME, m_MgameButton);
	DDX_Control(pDX, IDC_LOSTSAGA, m_LostSagaButton);
	DDX_Control(pDX, IDC_NEXON, m_NexonButton);
	DDX_Control(pDX, IDC_TOONI, m_TooniButton);
	DDX_Control(pDX, IDC_DAUM, m_DaumButton);
	SetTimer(1,100,0);
}


BEGIN_MESSAGE_MAP(ioLoginDlg, CDialogSK)
	ON_BN_CLICKED(IDC_IDSAVE, OnBnClickedIdsave)
	ON_BN_CLICKED(IDC_LOGIN_X, OnBnClickedLoginX)
	ON_BN_CLICKED(IDC_LOGIN, OnBnClickedLogin)
	ON_BN_CLICKED(IDC_ENTRY, OnBnClickedEntry)
	ON_BN_CLICKED(IDC_FIND, OnBnClickedFind)
   	ON_BN_CLICKED(IDC_LOGIN_FIND_PW, OnBnClickedLoginFindPw)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHANNEL, &ioLoginDlg::OnBnClickedChannel)
	ON_BN_CLICKED(IDC_LOSTSAGA, &ioLoginDlg::OnBnClickedChannel)
	ON_BN_CLICKED(IDC_NEXON, &ioLoginDlg::OnBnClickedChannel)
	ON_BN_CLICKED(IDC_MGAME, &ioLoginDlg::OnBnClickedChannel)
	ON_BN_CLICKED(IDC_TOONI, &ioLoginDlg::OnBnClickedChannel)
	ON_BN_CLICKED(IDC_NAVER, &ioLoginDlg::OnBnClickedChannel)
	ON_BN_CLICKED(IDC_DAUM, &ioLoginDlg::OnBnClickedChannel)

	ON_LBN_SELCHANGE(IDC_LIST1, &ioLoginDlg::OnLbnSelchangeList)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ioLoginDlg 메시지 처리기입니다.

void ioLoginDlg::OnBnClickedLoginX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnClose();
}

void ioLoginDlg::OnBnClickedIdsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void ioLoginDlg::SendLoginAuth(const char* szID, const char* szPW, const char* szURL)
{

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	if (!pMainFrame)
		return;

	char szError[2048] = "";
	ZeroMemory(szError, sizeof(szError));
	int iErrorCode;

	if (!pMainFrame->SendLoginData(szError, sizeof(szError), iErrorCode, szID, szPW, szURL))
	{

		m_IDEdit.EnableWindow(TRUE);
		m_PWEdit.EnableWindow(TRUE);
		m_PWEdit.Clear();

		pMainFrame->ShowErrorDlg(szError, iErrorCode);
		return;
	}
}

void ioLoginDlg::OnBnClickedLogin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( !pLocal )
		return;
	if(!AfxGetApp())
		return;
	
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();
	if(!pMainFrame)
		return;

	if( m_sLoginURL.IsEmpty() )
	{
		pMainFrame->ShowErrorDlg( "Errors : Login URL", 108 );
		return;
	}

	char szError[2048]="";
	if (m_bIDEditBlank || m_bPWEditBlank)
	{
		pMainFrame->ShowErrorDlg("Username and Password needed !", 108);
		return;
	}

	CString sID;
	m_IDEdit.GetWindowText(sID);
	if (strcmp(sID.GetString(), "ID") == 0)
	{
		pMainFrame->ShowErrorDlg("Input ID", 108);
		return;
	}
	CString sPW;
	m_PWEdit.GetWindowText(sPW);
	if (strcmp(sPW.GetString(), "PW") == 0)
	{
		pMainFrame->ShowErrorDlg("Input Password", 108);
		return;
	}

	if (strcmp(sID.GetString(), sPW.GetString()) == 0)
	{
		pMainFrame->ShowErrorDlg("Username and Password cant be same!", 108);
		return;
	}

	m_IDEdit.EnableWindow(FALSE);
	m_PWEdit.EnableWindow(FALSE);

	SendLoginAuth(sID.GetString(), sPW.GetString(), (LPCTSTR)m_sLoginURL);


	if( m_IDSaveBtn.IsChecked() )
	{
		Help::CreateDefaultSettingINI();
		ioINILoaderAU kIniLoder("Save\\default\\setting.ini");
		kIniLoder.SetTitle("Setting");
		CString sID;
		m_IDEdit.GetWindowText( sID );
		
		if( !sID.IsEmpty() )
		{
			CString sCh;
			kIniLoder.SaveString( "LoginID", (LPCTSTR) sID );

			int nSel = m_ChannelListBox.GetCurSel();
			CString sChannel;
			char szBuf[MAX_PATH];
			itoa(nSel, szBuf, 10 );
			sChannel.SetString(szBuf);
			kIniLoder.SaveString( "Channel", (LPCTSTR) sChannel );
		}
	}
	else
	{
		ioINILoaderAU kIniLoder("Save\\default\\setting.ini");
		kIniLoder.SetTitle("Setting");
		kIniLoder.SaveString( "LoginID", "" );
		kIniLoder.SaveString( "Channel", "" );
	}

	//pMainFrame->ParseCmdAndStart( pLocal, szError );
}

void ioLoginDlg::OnBnClickedEntry()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( !m_sEntryURL.IsEmpty() )
		Help::ExcuteOtherWeb( (LPCTSTR) m_sEntryURL );
}

void ioLoginDlg::OnBnClickedFind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( !m_sFindIDURL.IsEmpty() )
		Help::ExcuteWeb( (LPCTSTR) m_sFindIDURL );
}

void ioLoginDlg::OnBnClickedLoginFindPw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( !m_sFindPWURL.IsEmpty() )
		Help::ExcuteWeb( (LPCTSTR) m_sFindPWURL );
}

BOOL ioLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		OnBnClickedLogin();
		return TRUE;
	}
	else if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) 
		return FALSE;
#ifdef SRC_KOR
	m_ToolTipCtrl.RelayEvent(pMsg);
#endif

	return CDialogSK::PreTranslateMessage(pMsg);
}

BOOL ioLoginDlg::OnInitDialog()
{
	CDialogSK::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// skin
	EnableEasyMove();                    // enable moving of the dialog by clicking anywhere in the dialog

#ifdef SRC_KOR
	SetBitmap( IDB_BACK_UI_01 );         // set background bitmap
	SetStyle(LO_RESIZE);                 // resize dialog to the size of the bitmap

	m_XBtn.LoadBitmap(IDB_XBTN, FALSE );
	m_XBtn.MoveWindow(311, 5, 24, 26);

	m_LoginBtn.LoadBitmap(IDB_ENTER_UI);
	m_LoginBtn.MoveWindow(226, 261, 102, 91);

	m_EntryBtn.LoadBitmap(IDB_FIND_IDPW);
	m_EntryBtn.MoveWindow(14, 377, 314, 32);

	//--------------------------------------------------

	m_ChannelBtn.LoadBitmap(IDB_LOGIN_CHANNEL);
	m_ChannelBtn.MoveWindow(14, 261, 209, 29);
	m_ChannelBtn.ShowWindow(SW_HIDE);


	m_NaverButton.LoadBitmap(IDB_CHANNEL_NAVER);
	m_NaverButton.MoveWindow(14, 261, 209, 29);
	m_NaverButton.ShowWindow(SW_HIDE);

	m_MgameButton.LoadBitmap(IDB_CHANNEL_MGAME);
	m_MgameButton.MoveWindow(14, 261, 209, 29);
	m_MgameButton.ShowWindow(SW_HIDE);

	m_LostSagaButton.LoadBitmap(IDB_CHANNEL_MAIN);
	m_LostSagaButton.MoveWindow(14, 261, 209, 29);
	m_LostSagaButton.ShowWindow(SW_SHOW);

	m_NexonButton.LoadBitmap(IDB_CHANNEL_NEXON);
	m_NexonButton.MoveWindow(14, 261, 209, 29);
	m_NexonButton.ShowWindow(SW_HIDE);

	m_TooniButton.LoadBitmap(IDB_CHANNEL_TOONI);
	m_TooniButton.MoveWindow(14, 261, 209, 29);
	m_TooniButton.ShowWindow(SW_HIDE);

	m_DaumButton.LoadBitmap(IDB_CHANNEL_DAUM);
	m_DaumButton.MoveWindow(14, 261, 209, 29);
	m_DaumButton.ShowWindow(SW_HIDE);

	m_ChannelListBox.AddString("로스트 사가(공식)");
	m_ChannelListBox.AddString("넥슨 로스트 사가");
	m_ChannelListBox.AddString("네이버 게임 로스트 사가");
	m_ChannelListBox.AddString("투니랜드 로스트 사가");
	m_ChannelListBox.AddString("엠게임로스트 사가");
	m_ChannelListBox.AddString("다음 로스트 사가");

	m_ChannelListBox.MoveWindow(14, 290, 209, 90);
	m_ChannelListBox.ShowWindow(SW_HIDE);

	//--------------------------------------------------


	m_IDSaveBtn.LoadBitmap(IDB_LOGIN_IDSAVEBTN);
	m_IDSaveBtn.MoveWindow(193, 292, 29, 29);
	m_IDSaveBtn.SetCheckBtn( TRUE );

	m_FindIDBtn.ShowWindow(SW_HIDE);
	m_FindPWBtn.ShowWindow(SW_HIDE);

	m_IDEdit.MoveWindow(20, 294, 165, 25);
	m_PWEdit.MoveWindow(20, 325, 196, 25);

	// edit back img
	m_IDEditNormalBitmap.DeleteObject();
	m_IDEditNormalBitmap.LoadBitmap(IDB_LOGIN_EDIT_NORMAL );

	m_IDEditSelectBitmap.DeleteObject();
	m_IDEditSelectBitmap.LoadBitmap(IDB_LOGIN_EDIT_SELECT );

	m_PWEditNormalBitmap.DeleteObject();
	m_PWEditNormalBitmap.LoadBitmap(IDB_LOGIN_EDIT_PW_NORMAL );

	m_PWEditSelectBitmap.DeleteObject();
	m_PWEditSelectBitmap.LoadBitmap(IDB_LOGIN_EDIT_PW_SELECT );

	//공홈버튼 위치
	m_rcBackImg.left	= 0;
	m_rcBackImg.right	= 340;
	m_rcBackImg.bottom	= 250;
	m_rcBackImg.top		= 45;

	m_ToolTipCtrl.Create(this);
#else
	SetBitmap( IDB_LOGIN_BACK );         // set background bitmap
	SetStyle(LO_RESIZE);                 // resize dialog to the size of the bitmap

	m_XBtn.LoadBitmap(IDB_LOGIN_CANCELBTN, FALSE );
	m_XBtn.MoveWindow(311, 5, 25, 26);

	m_LoginBtn.LoadBitmap(IDB_LOGIN_ENTERBTN);
	m_LoginBtn.MoveWindow(224, 263, 102, 60);

	m_EntryBtn.LoadBitmap(IDB_LOGIN_ENTRYBTN);
	m_EntryBtn.MoveWindow(15, 378, 311, 29);

	m_FindIDBtn.LoadBitmap(IDB_LOGIN_FINDBTN);
	m_FindIDBtn.MoveWindow(15, 351, 154, 24);

	m_FindPWBtn.LoadBitmap(IDB_LOGIN_FINDPW_BTN);
	m_FindPWBtn.MoveWindow(171, 351, 155, 24);

	m_IDSaveBtn.LoadBitmap(IDB_LOGIN_IDSAVEBTN);
	m_IDSaveBtn.MoveWindow(192, 263, 29, 29);
	m_IDSaveBtn.SetCheckBtn( TRUE ); 

	m_IDEdit.MoveWindow(20, 265, 165, 25);
	m_PWEdit.MoveWindow(20, 296, 196, 25);


	m_NaverButton.ShowWindow(SW_HIDE);
	m_MgameButton.ShowWindow(SW_HIDE);
	m_LostSagaButton.ShowWindow(SW_HIDE);
	m_NexonButton.ShowWindow(SW_HIDE);
	m_TooniButton.ShowWindow(SW_HIDE);
	m_DaumButton.ShowWindow(SW_HIDE);
	m_ChannelBtn.ShowWindow(SW_HIDE);
	m_ChannelListBox.ShowWindow(SW_HIDE);

	// edit back img
	m_IDEditNormalBitmap.DeleteObject();
	m_IDEditNormalBitmap.LoadBitmap(IDB_LOGIN_EDIT_NORMAL );

	m_IDEditSelectBitmap.DeleteObject();
	m_IDEditSelectBitmap.LoadBitmap(IDB_LOGIN_EDIT_SELECT );

	m_PWEditNormalBitmap.DeleteObject();
	m_PWEditNormalBitmap.LoadBitmap(IDB_LOGIN_EDIT_PW_NORMAL );

	m_PWEditSelectBitmap.DeleteObject();
	m_PWEditSelectBitmap.LoadBitmap(IDB_LOGIN_EDIT_PW_SELECT );

#endif

	SetTransparentColor(RGB(0, 255, 0)); // set green as the transparent color

	m_EditTextColor = RGB(58,58,58);
	m_EditNoticeTextColor = RGB(124,124,124);

	m_EditNormalBGColor   = RGB(230, 230, 230);
	m_EditNormalBGBrush.CreateSolidBrush( m_EditNormalBGColor );

	m_EditSelectBGColor   = RGB(249, 249, 249);
	m_EditSelectBGBrush.CreateSolidBrush( m_EditSelectBGColor );
	//

	// font
	m_Font.CreateFont(13,0,0,0,FW_BOLD,FALSE,FALSE, FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,VARIABLE_PITCH,"Tahoma");
	m_IDEdit.SetFont(&m_Font,TRUE);
	m_PWEdit.SetFont(&m_Font,TRUE);

	

	// edit 위치 수정
	CRect rRect( 4, 6, 165, 25 );
	m_IDEdit.SetRect( &rRect );

	CRect rRectPW( 4, 6, 196, 25 );
	m_PWEdit.SetRect( &rRectPW );

	SetWindowText( "Lost Saga" ); 


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void ioLoginDlg::OnShowWindow(BOOL bShow, UINT nStatus)
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
		/*NULL,*/ &CWnd::wndTopMost, // 창 항상 위로 
		workrect.left + (workwidth-winwidth) / 2,
		workrect.top + (workheight-winheight) / 2,
		winwidth, winheight, 
		SWP_SHOWWINDOW);
	//----------------------------------------------------------
}

HBRUSH ioLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogSK::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch(nCtlColor)
	{
	case CTLCOLOR_EDIT:   // edit 박스가 쓰기상태일때
	case CTLCOLOR_STATIC: // edit 박스가 read only 일때
		{
			if( pDC && pWnd )
			{
				if( pWnd->GetDlgCtrlID() == IDC_EDIT_ID ) 
				{
					if( m_bIDEditBlank )
						pDC->SetTextColor( m_EditNoticeTextColor );
					else
						pDC->SetTextColor( m_EditTextColor );

					if( m_bSelectIDEdit )
					{
						pDC->SetBkColor( m_EditSelectBGColor );
						return m_EditSelectBGBrush;
					}
					else
					{
						pDC->SetBkColor( m_EditNormalBGColor );
						return m_EditNormalBGBrush;
					}
				}
				else if( pWnd->GetDlgCtrlID() == IDC_EDIT_PW )
				{
					if( m_bPWEditBlank )
						pDC->SetTextColor( m_EditNoticeTextColor );
					else
						pDC->SetTextColor( m_EditTextColor );

					if( m_bSelectPWEdit )
					{
						pDC->SetBkColor( m_EditSelectBGColor );
						return m_EditSelectBGBrush;
					}
					else
					{
						pDC->SetBkColor( m_EditNormalBGColor );
						return m_EditNormalBGBrush;
					}
				}
			}
		}
		break;
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void ioLoginDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	EndDialog(0);

	if(AfxGetApp())
	{
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();
		if(pMainFrame)
			pMainFrame->SetWantExit();
	}
}

void ioLoginDlg::LoadINI()
{
	char szBuf[MAX_PATH*2]="";

	ioINILoaderAU kLoader( "info\\start.ini" );
	kLoader.SetTitle( "Info" );
	kLoader.LoadString( "LoginURL", "", szBuf, sizeof( szBuf ) );
	m_sLoginURL = szBuf; //get value from start.ini
//	m_sLoginURL = "http://172.20.20.75/playExecLGU.asp"; 개발존
//	m_sLoginURL = "http://www.lostsaga.com/play/playExecLGU.asp";  //
	//m_sLoginURL = "https://www.lostsaga.com/play/playExecClient.asp";  // 베타존
	ZeroMemory( szBuf, sizeof( szBuf ) );

	kLoader.LoadString( "EntryURL", "", szBuf, sizeof( szBuf ) );
	m_sEntryURL = szBuf;
#ifdef SRC_KOR
	m_sEntryURL = "https://www.lostsaga.com/sendmail/idpwd/searchID.asp";
	m_sOfficialURL = "http://www.lostsaga.com/main/index.asp";
#endif
	ZeroMemory( szBuf, sizeof( szBuf ) );

	kLoader.LoadString( "FindIDURL", "", szBuf, sizeof( szBuf ) );
	m_sFindIDURL = szBuf;
	ZeroMemory( szBuf, sizeof( szBuf ) );

	kLoader.LoadString( "FindPWURL", "", szBuf, sizeof( szBuf ) );
	m_sFindPWURL = szBuf;
	ZeroMemory( szBuf, sizeof( szBuf ) );

	int iSize = kLoader.LoadInt( "IDSize", 40 );
	m_IDEdit.SetLimitText( iSize );

	iSize = kLoader.LoadInt( "PWSize", 40 );
	m_PWEdit.SetLimitText( iSize );

	ioINILoaderAU kIDLoader("Save\\default\\setting.ini");
	kIDLoader.SetTitle("Setting");
	kIDLoader.LoadString( "LoginID", "", szBuf, sizeof( szBuf ) );
	if( strcmp( szBuf, "" ) != 0 )
	{
		m_IDEdit.SetWindowText( szBuf );
		m_PWEdit.SetPasswordChar(0);
		m_PWEdit.SetWindowText( "PW" );
		m_bIDEditBlank = false;
		m_bPWEditBlank = true;
		m_IDSaveBtn.SetChecked( TRUE );
		m_IDSaveBtn.Invalidate();

		kIDLoader.LoadString( "Channel", "", szBuf, sizeof( szBuf ) );
		int nSel = atoi(szBuf);
#ifdef SRC_KOR
		m_ChannelListBox.SetCurSel(0);
#else
		m_ChannelListBox.SetCurSel(nSel);
#endif
		OnLbnSelchangeList();
	}
	else
	{
		m_IDEdit.SetWindowText( "ID" );
		m_PWEdit.SetPasswordChar(0); // 글자표시
		m_PWEdit.SetWindowText( "PW" );
		m_ChannelListBox.SetCurSel(0);
		m_bIDEditBlank = true;
		m_bPWEditBlank = true;
		m_IDSaveBtn.SetChecked( FALSE );
	}
}



BOOL ioLoginDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(HIWORD(wParam) == EN_SETFOCUS)
	{
		CEdit *pEdit=(CEdit*)GetDlgItem(LOWORD(wParam));
		if( pEdit )
		{
			if( pEdit->GetDlgCtrlID() == IDC_EDIT_ID )
			{
				m_bSelectIDEdit = true;

				if( m_bIDEditBlank )
				{
					pEdit->SetWindowText( "" );
					m_bIDEditBlank = false; // 글자입력시 글자색 변경을 위해서
				}
				Invalidate(FALSE);
			}
			else if( pEdit->GetDlgCtrlID() == IDC_EDIT_PW )
			{
				m_bSelectPWEdit = true;

				if( m_bPWEditBlank )
				{
					m_PWEdit.SetPasswordChar('*');
					pEdit->SetWindowText( "" );
					m_bPWEditBlank = false; // 글자입력시 글자색 변경을 위해서
				}
				Invalidate(FALSE);
			}
		}
	}
	else if( HIWORD(wParam) == EN_KILLFOCUS )
	{
		CEdit *pEdit=(CEdit*)GetDlgItem(LOWORD(wParam));
		if( pEdit )
		{
			if( pEdit->GetDlgCtrlID() == IDC_EDIT_ID )
			{
				m_bSelectIDEdit = false;

				CString sText;
				pEdit->GetWindowText(sText);
				if( !sText.IsEmpty() )
					m_bIDEditBlank = false;
				else
					m_bIDEditBlank = true;

				if( m_bIDEditBlank )
					pEdit->SetWindowText( "ID" );

				Invalidate(FALSE);
			}
			else if( pEdit->GetDlgCtrlID() == IDC_EDIT_PW )
			{
				m_bSelectPWEdit = false;

				CString sText;
				pEdit->GetWindowText(sText);
				if( !sText.IsEmpty() )
					m_bPWEditBlank = false;
				else
					m_bPWEditBlank = true;

				if( m_bPWEditBlank )
				{
					m_PWEdit.SetPasswordChar(0); // 글자표시
					pEdit->SetWindowText( "PW" );
				}

				Invalidate(FALSE);
			}
		}
	}
	return CDialogSK::OnCommand(wParam, lParam);
}

void ioLoginDlg::DrawBitmap( CPaintDC *pDC, CBitmap *pBitmap, int iX, int iY, int iWidth, int iHeight )
{
	if( !pDC )
		return;

	if( !pBitmap )
		return;

	CBitmap  *pOldBitmap = NULL;
	CDC      MemDC;

	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = (CBitmap *)MemDC.SelectObject(pBitmap);
	pDC->BitBlt( iX, iY, iWidth, iHeight, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}
void ioLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogSK::OnPaint()을(를) 호출하지 마십시오.
	enum 
	{
#ifdef SRC_KOR
		X = 14,
		Y = 292,
		WIDTH  = 177,
		HEIGHT = 29,

		X_PW      = 14,
		Y_PW      = 323,
		WIDTH_PW  = 209,
		HEIGHT_PW = 29,
#else
		X = 15,
		Y = 263,
		WIDTH  = 175,
		HEIGHT = 29,

		X_PW      = 15,
		Y_PW      = 294,
		WIDTH_PW  = 206,
		HEIGHT_PW = 29,
#endif
	};

	if( m_bSelectIDEdit )
		DrawBitmap( &dc, &m_IDEditSelectBitmap, X, Y, WIDTH, HEIGHT ); 
	else
		DrawBitmap( &dc, &m_IDEditNormalBitmap, X, Y, WIDTH, HEIGHT ); 
	

	if( m_bSelectPWEdit )
		DrawBitmap( &dc, &m_PWEditSelectBitmap, X_PW, Y_PW, WIDTH_PW, HEIGHT_PW ); 
	else
		DrawBitmap( &dc, &m_PWEditNormalBitmap, X_PW, Y_PW, WIDTH_PW, HEIGHT_PW ); 
}


void ioLoginDlg::OnBnClickedChannel()
{
#ifndef SRC_KOR
	m_ChannelListBox.ShowWindow(SW_HIDE);
	m_ChannelListBox.EnableWindow(true);
	m_PWEdit.EnableWindow(false);
	m_IDEdit.EnableWindow(false);
	m_LoginBtn.EnableWindow(false);
	m_IDSaveBtn.EnableWindow(false);
#endif

}


void ioLoginDlg::OnLbnSelchangeList()
{
	m_ChannelListBox.ShowWindow(SW_HIDE);

	m_ChannelListBox.EnableWindow(false);
	m_PWEdit.EnableWindow(true);
	m_IDEdit.EnableWindow(true);
	m_LoginBtn.EnableWindow(true);
	m_IDSaveBtn.EnableWindow(true);

	int nSel = m_ChannelListBox.GetCurSel();
	
	switch(nSel)
	{
	case 0:// 로스트사가 공식
		m_ChannelBtn.ShowWindow(SW_HIDE);
		m_NaverButton.ShowWindow(SW_HIDE);
		m_MgameButton.ShowWindow(SW_HIDE);
		m_LostSagaButton.ShowWindow(SW_SHOW);
		m_NexonButton.ShowWindow(SW_HIDE);
		m_TooniButton.ShowWindow(SW_HIDE);
		m_DaumButton.ShowWindow(SW_HIDE);
		break;
	case 1:// 넥슨 로스트사가
		m_ChannelBtn.ShowWindow(SW_HIDE);
		m_NaverButton.ShowWindow(SW_HIDE);
		m_MgameButton.ShowWindow(SW_HIDE);
		m_LostSagaButton.ShowWindow(SW_HIDE);
		m_NexonButton.ShowWindow(SW_SHOW);
		m_TooniButton.ShowWindow(SW_HIDE);
		m_DaumButton.ShowWindow(SW_HIDE);
		break;
	case 2:// 네이버 로스트사가
		m_ChannelBtn.ShowWindow(SW_HIDE);
		m_NaverButton.ShowWindow(SW_SHOW);
		m_MgameButton.ShowWindow(SW_HIDE);
		m_LostSagaButton.ShowWindow(SW_HIDE);
		m_NexonButton.ShowWindow(SW_HIDE);
		m_TooniButton.ShowWindow(SW_HIDE);
		m_DaumButton.ShowWindow(SW_HIDE);
		break;
	case 3:// 투니랜드 로스트사가
		m_ChannelBtn.ShowWindow(SW_HIDE);
		m_NaverButton.ShowWindow(SW_HIDE);
		m_MgameButton.ShowWindow(SW_HIDE);
		m_LostSagaButton.ShowWindow(SW_HIDE);
		m_NexonButton.ShowWindow(SW_HIDE);
		m_TooniButton.ShowWindow(SW_SHOW);
		m_DaumButton.ShowWindow(SW_HIDE);
		break;
	case 4:// 엠게임 로스트사가
		m_ChannelBtn.ShowWindow(SW_HIDE);
		m_NaverButton.ShowWindow(SW_HIDE);
		m_MgameButton.ShowWindow(SW_SHOW);
		m_LostSagaButton.ShowWindow(SW_HIDE);
		m_NexonButton.ShowWindow(SW_HIDE);
		m_TooniButton.ShowWindow(SW_HIDE);
		m_DaumButton.ShowWindow(SW_HIDE);
		break;
	case 5:// 다음 로스트사가
		m_ChannelBtn.ShowWindow(SW_HIDE);
		m_NaverButton.ShowWindow(SW_HIDE);
		m_MgameButton.ShowWindow(SW_HIDE);
		m_LostSagaButton.ShowWindow(SW_HIDE);
		m_NexonButton.ShowWindow(SW_HIDE);
		m_TooniButton.ShowWindow(SW_HIDE);
		m_DaumButton.ShowWindow(SW_SHOW);
		break;
	case 6:
		m_ChannelBtn.ShowWindow(SW_SHOW);
		m_NaverButton.ShowWindow(SW_HIDE);
		m_MgameButton.ShowWindow(SW_HIDE);
		m_LostSagaButton.ShowWindow(SW_HIDE);
		m_NexonButton.ShowWindow(SW_HIDE);
		m_TooniButton.ShowWindow(SW_HIDE);
		m_DaumButton.ShowWindow(SW_HIDE);
		break;
	}
}


void ioLoginDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
#ifdef SRC_KOR
	POINT MSpt;
	GetCursorPos(&MSpt);
	ScreenToClient(&MSpt);
	if(PtInRect(&m_rcBackImg,MSpt))
	{
		if( !m_sOfficialURL.IsEmpty() )
			Help::ExcuteWeb( (LPCTSTR) m_sOfficialURL );
	}
#endif
	CDialogSK::OnLButtonDown(nFlags, point);
}

void ioLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
#ifdef SRC_KOR
	POINT MSpt;
	GetCursorPos(&MSpt);
	ScreenToClient(&MSpt);
	if(PtInRect(&m_rcBackImg,MSpt))
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND ));
		m_ToolTipCtrl.SetDelayTime(500);
		m_ToolTipCtrl.AddTool(this,"홈페이지 바로가기",  NULL,0);
	}
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW ));
#endif

	CDialogSK::OnTimer(nIDEvent);
}
