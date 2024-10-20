#pragma once


// ioProgressBar

class ioProgressBar : public CStatic
{
	DECLARE_DYNAMIC(ioProgressBar)

public:
	ioProgressBar();
	virtual ~ioProgressBar();

protected:
	CBitmap  m_bmpBar;
	CBitmap  m_bmpBarBack;

	int      m_iCurPos;
	int      m_iMaxPos;

	int      m_iWidth;
	int      m_iHeight;

	int      m_iWidthBack;
	int      m_iHeightBack;

	float    m_fProgressRate;
	
protected:
	DECLARE_MESSAGE_MAP()

protected:
	void DrawBitmap(  CPaintDC *pDC, CBitmap *pBitmap, int iWidth, int iHeight, float fRate );

public:
	afx_msg void OnPaint();

public:
	void SetBitmap (UINT nIDResource, UINT nIDResourceBack );

	void SetCurPosition( int iCurPos );
	void SetMaxPosition( int iMaxPos );

	int GetMaxPos() const { return m_iMaxPos; }
	int GetCurPos() const { return m_iCurPos; }
	float GetProgressRate() const { return m_fProgressRate; }
};


