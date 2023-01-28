#pragma once

#define		FS_EDIT_CTRL_ID					2002

class CMyLineEdit : public CWnd
{
public:
	CMyLineEdit();

public:
//-----------------------------------------------------//
// APIs
	BOOL	Create( int iStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
	CWnd	*GetEditWnd( );
	void	SetFocus( );
	void	GetWindowText( CString& rString );
	void	SetWindowText( LPCTSTR pszStr );
	void	MoveWindow( int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE );
	void	MoveWindow( LPCRECT lpRect, BOOL bRepaint = TRUE );
	void	PutWindow( CDC *pDC );
	void	EnableWindow( BOOL bEnable );

private:
// GUI Functions
	void	DrawLine( CDC *pDC,RECT nRect,COLORREF crColor );
	void	PutFrame( CDC *pDC,RECT nRect);

private:

	CEdit				m_Edit;		// Edit Control
	BOOL				m_bSFlag;

public:
	
	//{{AFX_VIRTUAL(CMyLineEdit)
	//}}AFX_VIRTUAL

public:
	virtual ~CMyLineEdit();

protected:
	//{{AFX_MSG(CMyLineEdit)
	afx_msg void OnChangeEdit();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}