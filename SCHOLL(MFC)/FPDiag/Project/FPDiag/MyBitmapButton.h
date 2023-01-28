#if !defined(_BUTTON_BITMAP_H_)
#define _BUTTON_BITMAP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyBitmapButton : public CWnd
{
public:
	CMyBitmapButton();
	~CMyBitmapButton();

public:
	BOOL	Create( CWnd *pParent,UINT *pBmpID,RECT *nRect,UINT nCtlID = 1000);
	void	SetBitmap(UINT *pBmpID );
	void	SetCursor(UINT hCursor);
	void	EnableWindow(BOOL bEnable);
	void	PutWindow( CDC *pDC );
	void	SetTabStop( BOOL bStop ) { m_bTabStop = bStop ; };
	void	PutBitmap( int nID );

private:
	void	PutBitmap();

private:
	UINT	m_nBitmapID[4];
	RECT	m_nRect;
	UINT	m_nCtlID;

	CSize	m_nSize;
	int		m_nSelID;

	int		m_nMouseflag;

	HCURSOR m_hCursor;

	BOOL	m_bTabStop;
	BOOL	m_bKeyFlag;
public:
	//{{AFX_VIRTUAL(CButtonBitmap)
public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CButtonBitmap)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif
