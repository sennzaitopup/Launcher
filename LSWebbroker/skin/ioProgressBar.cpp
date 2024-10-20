// ioProgressBar.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "ioProgressBar.h"
#include "../resource.h"


// ioProgressBar

IMPLEMENT_DYNAMIC(ioProgressBar, CStatic)
ioProgressBar::ioProgressBar()
{
	m_iCurPos = 0;
	m_iMaxPos = 100;
	m_fProgressRate = 0.0f;
	m_iWidth  = 0;
	m_iHeight = 0;
	m_iWidthBack = 0;
	m_iHeightBack= 0;
}

ioProgressBar::~ioProgressBar()
{
	if(m_bmpBar.m_hObject)
		m_bmpBar.DeleteObject();

	if(m_bmpBarBack.m_hObject)
		m_bmpBarBack.DeleteObject();
}


BEGIN_MESSAGE_MAP(ioProgressBar, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// ioProgressBar 메시지 처리기입니다.


void ioProgressBar::SetCurPosition( int iCurPos )
{
	m_iCurPos = max( 0, min( iCurPos, m_iMaxPos ) );

	m_fProgressRate = (float)m_iCurPos/(float)m_iMaxPos;
	m_fProgressRate = max( 0.0f, min( m_fProgressRate, 1.0f ) );
}

void ioProgressBar::SetMaxPosition( int iMaxPos )
{
	m_iMaxPos = max( 1, iMaxPos );

	m_fProgressRate = 0.0f;
}

void ioProgressBar::OnPaint()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CPaintDC dc(this); // device context for painting

	DrawBitmap( &dc, &m_bmpBarBack, m_iWidthBack, m_iHeightBack, 1.0f ); // 1.0f : 바탕은 항상 풀이미지를 그린다.
	DrawBitmap( &dc, &m_bmpBar, m_iWidth, m_iHeight, m_fProgressRate );
}


void ioProgressBar::SetBitmap( UINT nIDResource, UINT nIDResourceBack )
{
	// bar
	if(m_bmpBar.m_hObject)
		m_bmpBar.DeleteObject();
	m_bmpBar.LoadBitmap(nIDResource);

	BITMAP kBitMap;
	m_bmpBar.GetBitmap( &kBitMap );

	m_iWidth  = kBitMap.bmWidth;
	m_iHeight = kBitMap.bmHeight;

	// back
	if(m_bmpBarBack.m_hObject)
		m_bmpBarBack.DeleteObject();
	m_bmpBarBack.LoadBitmap(nIDResourceBack);

	m_bmpBarBack.GetBitmap( &kBitMap );

	m_iWidthBack  = kBitMap.bmWidth;
	m_iHeightBack = kBitMap.bmHeight;
}

void ioProgressBar::DrawBitmap( CPaintDC *pDC, CBitmap *pBitmap, int iWidth, int iHeight, float fRate )
{
	if( !pDC )
		return;

	if( !pBitmap )
		return;

	CBitmap  *pOldBitmap = NULL;
	CDC      MemDC;

	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = (CBitmap *)MemDC.SelectObject(pBitmap);

	int iRateWidth =  (int)( iWidth * fRate );
	if( iRateWidth > iWidth )
		iRateWidth = iWidth;
	pDC->BitBlt(0, 0, iRateWidth, iHeight, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}