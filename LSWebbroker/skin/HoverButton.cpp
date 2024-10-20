// HoverButton.cpp : implementation file
//

#include "../stdafx.h"
#include "HoverButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoverButton

CHoverButton::CHoverButton()
{
	m_bHover = FALSE;
	m_bTracking = FALSE;
	m_bChecked  = FALSE;
	m_bCheckBtn = FALSE;
	m_bDisableImg = FALSE;
}

CHoverButton::~CHoverButton()
{
}
IMPLEMENT_DYNAMIC(CHoverButton, CBitmapButton)

BEGIN_MESSAGE_MAP(CHoverButton, CBitmapButton)
	//{{AFX_MSG_MAP(CHoverButton)
	ON_WM_MOUSEMOVE()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
    ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
 //	CHoverButton message handlers
		
void CHoverButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	//	TODO: Add your message handler code here and/or call default

	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CBitmapButton::OnMouseMove(nFlags, point);
}

BOOL CHoverButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);		
	return CButton::PreTranslateMessage(pMsg);
}

// Set the tooltip with a string resource
void CHoverButton::SetToolTipText(int nId, BOOL bActivate)
{
	CString sText;

	// load string resource
	sText.LoadString(nId);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetToolTipText(&sText, bActivate);

}

// Set the tooltip with a CString
void CHoverButton::SetToolTipText(CString *spText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (spText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)*spText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText((LPCTSTR)*spText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CHoverButton::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
	}
} // End of InitToolTip

// Activate the tooltip
void CHoverButton::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} // End of EnableTooltip


void CHoverButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	if( !lpDrawItemStruct )
		return;

	CDC *mydc=CDC::FromHandle(lpDrawItemStruct->hDC);
	if( !mydc )
		return;

	CDC * pMemDC = new CDC;
	if( !pMemDC )
		return;

	pMemDC -> CreateCompatibleDC(mydc);


	CBitmap * pOldBitmap;
	pOldBitmap = pMemDC -> SelectObject(&mybitmap);
	
	CPoint point(0,0);	
		
	if( m_bCheckBtn )
	{
		if( m_bChecked )
		{
			if( IsWindowEnabled() )
				mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx,0,SRCCOPY);
			else
			{
				if( m_bDisableImg )
					mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*3,0,SRCCOPY);
				else
					mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx,0,SRCCOPY);
			}
		}
		else
		{
			if( IsWindowEnabled() )
			{
				if(m_bHover)
					mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*2,0,SRCCOPY);
				else
					mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
			}
			else
			{
				if( m_bDisableImg )
					mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*3,0,SRCCOPY);
				else
					mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
			}
		}
	}
	else
	{
		if( IsWindowEnabled() )
		{
	if(lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx,0,SRCCOPY);
	}
	else
	{
		if(m_bHover)
		{
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*2,0,SRCCOPY);
				}
				else
		{
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
				}	
		}	
		}
		else
		{
			if( m_bDisableImg )
				mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*3,0,SRCCOPY);
			else
				mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
		}
	}

	// clean up
	pMemDC -> SelectObject(pOldBitmap);
	delete pMemDC;
}

// Load a bitmap from the resources in the button, the bitmap has to have 3 buttonsstates next to each other: Up/Down/Hover
BOOL CHoverButton::LoadBitmap( UINT bitmapid, BOOL bDisableImg /*= FALSE */ )
{
	mybitmap.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(bitmapid), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	BITMAP	bitmapbits;
	mybitmap.GetBitmap(&bitmapbits);
	m_ButtonSize.cy=bitmapbits.bmHeight;
	int iSize = 3;
	if( bDisableImg )
		iSize = 4;
	m_ButtonSize.cx=bitmapbits.bmWidth/iSize;
	SetWindowPos( 
		/*NULL,*/&CWnd::wndTopMost, // 창 항상 위로 
		0,0, m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE   |SWP_NOOWNERZORDER   );
	m_bDisableImg = bDisableImg;

	return TRUE;
}



LRESULT  CHoverButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	// TODO: Add your message handler code here and/or call default
	m_bHover=TRUE;
	Invalidate();
	return 0;
}


LRESULT CHoverButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	Invalidate();
	return 0;
}

void CHoverButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bChecked = !m_bChecked;

	CBitmapButton::OnLButtonUp(nFlags, point);
}
