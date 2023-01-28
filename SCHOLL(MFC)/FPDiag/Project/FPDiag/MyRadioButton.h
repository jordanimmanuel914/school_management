#pragma once

class CMyRadioButton : public CWnd
{
public:
	CMyRadioButton();
	~CMyRadioButton();

public:

	BOOL	Create( CWnd *pParent,UINT *pBmpID,RECT *nRect,UINT nCtlID = 1000);
	void	SetCursor(UINT hCursor);
	void	EnableWindow(BOOL bEnable);
	void	SetCheck(int nCheck);
	int		GetCheck();
	void	PutWindow( CDC *pDC );
	void	SetTabStop( BOOL bStop ) { m_bTabStop = bStop ; };

private:
	void	PutBitmap();

private:
	UINT	m_nBitmapID[6];
	RECT	m_nRect;
	UINT	m_nCtlID;

	CSize	m_nSize;
	int		m_nSelID;

	int		m_nMouseflag;
	int		m_nCheck;

	HCURSOR m_hCursor;
	BOOL	m_bTabStop;

	//{{AFX_VIRTUAL(CMyRadioButton)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

public:

protected:
	//{{AFX_MSG(CMyRadioButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
