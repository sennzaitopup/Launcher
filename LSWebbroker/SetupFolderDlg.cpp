// SetupFolderDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "SetupFolderDlg.h"
#include ".\setupfolderdlg.h"
#include "MainFrm.h"
#include "Util/HelpFunc.h"
#include "stringmanager\iostringmanager.h"

#include <strsafe.h>
#include "Local\ioLocalParent.h"

// SetupFolderDlg 대화 상자입니다.

SetupFolderDlg::SetupFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialogSK(SetupFolderDlg::IDD, pParent)
{
}

SetupFolderDlg::~SetupFolderDlg()
{
}

void SetupFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETUP, m_SetupBtn);
	DDX_Control(pDX, IDC_EXIT, m_ExitBtn);
	DDX_Control(pDX, IDC_SETUP_FOLDER_BUTTON, m_ChangeBtn);
	DDX_Control(pDX, IDC_X, m_XBtn);
	DDX_Control(pDX, IDC_EDIT, m_Edit);
}


BEGIN_MESSAGE_MAP(SetupFolderDlg, CDialogSK)
	ON_BN_CLICKED(IDC_SETUP_FOLDER_BUTTON, OnBnClickedChangeFolder)
	ON_BN_CLICKED(IDC_SETUP, OnBnClickedSetup)
	ON_BN_CLICKED(IDC_EXIT, OnBnClickedExit)
	ON_BN_CLICKED(IDC_X, OnBnClickedExit)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// SetupFolderDlg 메시지 처리기입니다.

void SetupFolderDlg::OnBnClickedChangeFolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szNewPath[MAX_PATH*2]="";
	GetPath( szNewPath, sizeof(szNewPath) , STR(1) );

	if( strcmp( szNewPath, "") == 0 )
	{
		Help::MessageboxShow( STR(2), "LostSaga_Webbroker - Error", MB_TOPMOST );
		return;
	}

	size_t iSize = 0;
	StringCbLength( szNewPath, sizeof( szNewPath ), &iSize );

	enum { MAX_DIR_LENGHT = 100, }; // path는 260자까지이므로 로사 하위 폴더까지 고려해서 루트 폴더는 100까지만 설정.
	if( iSize >= MAX_DIR_LENGHT )
	{
		Help::MessageboxShow( STR(3), "LostSaga_Webbroker - Error", MB_TOPMOST );
		return;
	}

	CString szResultPath = szNewPath;
// 	if( szResultPath.GetLength() == DRIVE_ONLY_SIZE ) /* 드라이브만 선택시에는 \\붙는다. ( C:\\ ) */
// 		szResultPath += "LostSaga";
// 	else
// 		szResultPath += "\\LostSaga";


	if( !CreateSetupDirectory( (LPCTSTR)szResultPath ) )
	{
		Help::MessageboxShow( STR(4), "LostSaga_Webbroker - Error", MB_TOPMOST );
		return;
	}

	m_szClientPath = szResultPath;
	m_Edit.SetWindowText( (LPCTSTR)m_szClientPath );
}

void SetupFolderDlg::OnBnClickedSetup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if( GetDlgItem(IDC_SETUP) )
		GetDlgItem(IDC_SETUP)->EnableWindow( false );

	if( GetDlgItem(IDC_EXIT) )
		GetDlgItem(IDC_EXIT)->EnableWindow( false );

	if( GetDlgItem(IDC_SETUP_FOLDER_BUTTON) )
		GetDlgItem(IDC_SETUP_FOLDER_BUTTON)->EnableWindow( false );

	if(AfxGetApp())
	{
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();
		if(pMainFrame)
			pMainFrame->SetupAndStart(m_szClientPath);
	}
	EndDialog(0);
}

void SetupFolderDlg::OnBnClickedExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnClose();
}


BOOL SetupFolderDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_KEYDOWN &&pMsg->wParam == VK_RETURN 
	  ||pMsg->message == WM_KEYDOWN &&pMsg->wParam == VK_ESCAPE) 
		return FALSE;

	return CDialogSK::PreTranslateMessage(pMsg);
}

void SetupFolderDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogSK::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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

BOOL SetupFolderDlg::OnInitDialog()
{
	CDialogSK::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	char szWindowsPath[MAX_PATH*2]="";
	if( GetWindowsDirectory( szWindowsPath, sizeof( szWindowsPath ) ) != 0 )
	{
		char szDrive[MAX_PATH]="";
		_splitpath(szWindowsPath, szDrive, NULL, NULL, NULL);

		m_szClientPath = szDrive;
		m_szClientPath += DEFAULT_PATH;
		m_Edit.SetWindowText( (LPCTSTR)m_szClientPath);
	}

#ifdef SRC_KOR
	SetIcon( AfxGetApp()->LoadIcon(IDI_REDICON), FALSE );
#else
	SetIcon( AfxGetApp()->LoadIcon(IDI_SHIP), FALSE );
#endif

	DWORD dwBack   = IDB_BACK;
	DWORD dwChange = IDB_CHANGEBTN;
	DWORD dwSetup  = IDB_SETUPBTN;
	DWORD dwExit   = IDB_EXITBTN;
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal )
	{
		dwBack   = pLocal->GetResourceIDBack();
		dwChange = pLocal->GetResourceIDChange();
		dwSetup  = pLocal->GetResourceIDSetup();
		dwExit   = pLocal->GetResourceIDErrorExit();
	}

	// skin
	EnableEasyMove();                    // enable moving of the dialog by clicking anywhere in the dialog
	SetBitmap(dwBack);                   // set background bitmap
	SetStyle(LO_RESIZE);                 // resize dialog to the size of the bitmap
	SetTransparentColor(RGB(0, 255, 0)); // set green as the transparent color

	m_XBtn.LoadBitmap(IDB_XBTN);
	m_XBtn.MoveWindow(380, 5, 25, 26);

	m_ChangeBtn.LoadBitmap(dwChange);
	m_ChangeBtn.MoveWindow(296, 47, 97, 18);

	m_SetupBtn.LoadBitmap(dwSetup);
	m_SetupBtn.MoveWindow(17, 95, 286, 31);

	m_ExitBtn.LoadBitmap(dwExit);
	m_ExitBtn.MoveWindow(305, 95, 88, 31);

	m_Edit.MoveWindow(74, 50, 212, 13);
	m_EditBGColor   = RGB(247, 243, 247);
	m_EditTextColor = RGB(58,58,58);
	m_EditBGBrush.CreateSolidBrush( m_EditBGColor );
	//

	SetWindowText( "Lost Saga" ); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//--------
int CALLBACK SetupFolderDlg::BrowseCallbackProc(HWND hWnd,UINT uMsg,LPARAM lp, LPARAM pData) 
{
	switch(uMsg) 
	{
	case BFFM_INITIALIZED:
		{
			//대화상자가 중앙에 오도록 한다.
			RECT rc;
			::GetClientRect(hWnd, &rc);
			::SetWindowPos(hWnd, NULL,
				(GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left))/2,
				(GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top))/2,
				0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
		break;
	default:
		break;
	}
	return 0;
}

void SetupFolderDlg::GetPath( OUT char *szPath, int size, const char*szTitle )
{
	char            pszPathName[MAX_PATH*2]="";
	LPITEMIDLIST    pidl;
	LPMALLOC        pMalloc;
	BROWSEINFO      BrInfo;

	if(SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		memset(&BrInfo, 0, sizeof(BrInfo));
		BrInfo.hwndOwner = NULL;
		BrInfo.pidlRoot  = NULL;
		BrInfo.pszDisplayName = NULL;
		BrInfo.lpszTitle = _T(szTitle);
		BrInfo.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
		BrInfo.lpfn = BrowseCallbackProc;
		BrInfo.lParam = NULL;
		BrInfo.iImage=NULL;

		pidl = ::SHBrowseForFolder(&BrInfo);    
		if( pidl != NULL)		
			::SHGetPathFromIDList(pidl, pszPathName);  // 폴더명 얻기 pszPathName

		pMalloc->Free(pidl); 
		pMalloc->Release();

		memset(szPath, 0, size);

		if(sizeof(pszPathName) > size )
			StringCbCopyN(szPath, size, pszPathName, size-1);
		else
			StringCbCopyN(szPath, size, pszPathName, sizeof(pszPathName)-1);
	}
}

bool SetupFolderDlg::CreateSetupDirectory( const char *szPath )
{
	Help::SetCreateDirectory( szPath );
	bool bCreated = true;
	DWORD dwAtt = GetFileAttributes(szPath);
	if( dwAtt == INVALID_FILE_ATTRIBUTES )
	{
		bCreated = false;
	}
	else
	{
		if( dwAtt & FILE_ATTRIBUTE_DIRECTORY )
			bCreated = true;
		else
			bCreated = false;
	}

	return bCreated;
}
//----



HBRUSH SetupFolderDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

			if( pWnd && pWnd->GetDlgCtrlID() == IDC_EDIT )
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

void SetupFolderDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(AfxGetApp())
	{
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();
		if(pMainFrame)
			pMainFrame->SetWantExit();
	}

	EndDialog(0);
}
