#include "stdafx.h"
#include "MyLineEdit.h"

#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMyLineEdit::CMyLineEdit()
{
}

CMyLineEdit::~CMyLineEdit()
{
}
//***************************************************************************//
//***************************************************************************//
//***************************************************************************//
// APIs
BOOL CMyLineEdit::Create( int iStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	RECT nRect;
	DWORD dwStyle;
	int cx,cy;
///.............................//
	if(!CWnd::Create( NULL,NULL,WS_CHILD | WS_TABSTOP | WS_VISIBLE ,rect,pParentWnd,nID))
		return FALSE;
//.............................//
	switch( iStyle )
	{
	case 0: // One Line
		dwStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL;
		break;
	case 1: // Password
		dwStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_LEFT | ES_PASSWORD | ES_AUTOHSCROLL;
		break;
	}
	cx = rect.right - rect.left - 4;
	cy = rect.bottom - rect.top - 4;
	nRect.left = 2;
	nRect.top = 2;
	nRect.right = nRect.left + cx;
	nRect.bottom = nRect.top + cy;
	if(!m_Edit.Create( dwStyle,nRect,this,FS_EDIT_CTRL_ID ))
		return FALSE;
//	m_Edit.SetBackgroundColor(FALSE,RGB(255,255,0));
	return TRUE;
}
//-----------------------------------------------------//
CWnd *CMyLineEdit::GetEditWnd( )
{
	return &m_Edit;
}
//-----------------------------------------------------//
void CMyLineEdit::SetFocus( )
{
	m_Edit.SetFocus();
	m_Edit.SetSel(0,255);
}


//-----------------------------------------------------//
void CMyLineEdit::GetWindowText( CString& rString )
{
	CString szStr;
	m_Edit.GetWindowText( szStr );
	rString = szStr;
}

//-----------------------------------------------------//
void CMyLineEdit::SetWindowText( LPCTSTR pszStr )
{
	m_Edit.SetWindowText( pszStr );
}

#define	EDIT_FRMAE_WIDTH	2
#define	EDIT_SCROLL_WIDTH	16

//-----------------------------------------------------//
void CMyLineEdit::MoveWindow( int x, int y, int nWidth, int nHeight, BOOL bRepaint )
{
	CWnd::MoveWindow( x,y,nWidth,nHeight,bRepaint );
	m_Edit.MoveWindow( EDIT_FRMAE_WIDTH,EDIT_FRMAE_WIDTH,nWidth-EDIT_FRMAE_WIDTH*2,nHeight-EDIT_FRMAE_WIDTH*2,bRepaint );
}

//-----------------------------------------------------//
void CMyLineEdit::MoveWindow( LPCRECT lpRect, BOOL bRepaint )
{
	RECT nRect;
	int cx,cy;
	CWnd::MoveWindow( lpRect,bRepaint );
	cx = lpRect->right - lpRect->left - EDIT_FRMAE_WIDTH*2;
	cy = lpRect->bottom - lpRect->top - EDIT_FRMAE_WIDTH*2;
	nRect.left = EDIT_FRMAE_WIDTH;
	nRect.top = EDIT_FRMAE_WIDTH;
	nRect.right = nRect.left + cx;
	nRect.bottom = nRect.top + cy;
	m_Edit.MoveWindow( &nRect,bRepaint );
}

//-----------------------------------------------------//
void CMyLineEdit::PutWindow( CDC *pDC )
{
	CRect nRect;
	CRect cRect;

	GetClientRect( &nRect );
	
	CDC memDC;
	CBitmap memBmp;
	CBitmap *pOldBmp;

	memDC.CreateCompatibleDC( pDC );
	memBmp.CreateCompatibleBitmap(pDC,nRect.right,nRect.bottom);
	pOldBmp = memDC.SelectObject(&memBmp);

	PutFrame( &memDC,nRect );

	GetWindowRect( &cRect );
	GetParent()->ScreenToClient( &cRect );
	pDC->BitBlt( cRect.left,cRect.top,nRect.Width(),nRect.Height(),&memDC,0,0,SRCCOPY );

	memDC.SelectObject(pOldBmp );
	memBmp.DeleteObject();

}

//-----------------------------------------------------//
void CMyLineEdit::EnableWindow( BOOL bEnable )
{
	CWnd::EnableWindow( bEnable );
	m_Edit.EnableWindow( bEnable );
}

//*************************************************************//
//*************************************************************//
//*************************************************************//
// GUI API
void CMyLineEdit::DrawLine( CDC *pDC,RECT nRect,COLORREF crColor )
{
	CPen pen;
	CPen *oldPen;
	pen.CreatePen(PS_SOLID,1,crColor);
	oldPen = pDC->SelectObject( &pen );

	pDC->MoveTo( nRect.left,nRect.top );
	pDC->LineTo( nRect.right,nRect.bottom );

	pDC->SelectObject( oldPen );
	pen.DeleteObject();
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
#define	EDIT_FRAME_COLOR1	RGB(66,69,49)
#define	EDIT_FRAME_COLOR2	RGB(41,40,24)
#define	EDIT_FRAME_COLOR3	RGB(255,255,247)
#define	EDIT_FRAME_COLOR4	RGB(231,227,214)
#define	EDIT_FRAME_COLOR5	RGB(255,255,255)
//-----------------------------------------------------//
void CMyLineEdit::PutFrame( CDC *pDC,RECT nRect)
{
	RECT cRect;
	CBrush frmBr(EDIT_FRAME_COLOR5);
	pDC->FillRect( &nRect,&frmBr );
	//........................//

	cRect.left = nRect.left;
	cRect.top = nRect.top;
	cRect.right = nRect.left;
	cRect.bottom = nRect.bottom;
	DrawLine( pDC, cRect, EDIT_FRAME_COLOR1);

	cRect.left = nRect.left + 1;
	cRect.top = nRect.top + 1;
	cRect.right = nRect.left + 1;
	cRect.bottom = nRect.bottom - 2;
	DrawLine( pDC, cRect, EDIT_FRAME_COLOR2);

	//........................//
	cRect.left = nRect.left;
	cRect.top = nRect.top;
	cRect.right = nRect.right;
	cRect.bottom = nRect.top;
	DrawLine( pDC, cRect, EDIT_FRAME_COLOR1);

	cRect.left = nRect.left + 1;
	cRect.top = nRect.top + 1;
	cRect.right = nRect.right - 1;
	cRect.bottom = nRect.top + 1;
	DrawLine( pDC, cRect, EDIT_FRAME_COLOR2);

	//........................//
	cRect.left = nRect.right-1;
	cRect.top = nRect.top + 1;
	cRect.right = nRect.right-1;
	cRect.bottom = nRect.bottom;
	DrawLine( pDC, cRect, EDIT_FRAME_COLOR3);

	cRect.left = nRect.right - 2;
	cRect.top = nRect.top + 2;
	cRect.right = nRect.right - 2;
	cRect.bottom = nRect.bottom - 1;
	DrawLine( pDC, cRect, EDIT_FRAME_COLOR4);

	//........................//
	cRect.left = nRect.right-1;
	cRect.top = nRect.bottom-1;
	cRect.right = nRect.left + 1;
	cRect.bottom = nRect.bottom-1;
	DrawLine( pDC, cRect, EDIT_FRAME_COLOR3);

	cRect.left = nRect.right - 2;
	cRect.top = nRect.bottom - 2;
	cRect.right = nRect.left + 1;
	cRect.bottom = nRect.bottom - 2;
	DrawLine( pDC, cRect, EDIT_FRAME_COLOR4);

}

//*************************************************************//
//*************************************************************//
//*************************************************************//
// Event

BEGIN_MESSAGE_MAP(CMyLineEdit, CWnd)
	//{{AFX_MSG_MAP(CMyLineEdit)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_EN_CHANGE(FS_EDIT_CTRL_ID, OnChangeEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMyLineEdit::OnChangeEdit() 
{
}

void CMyLineEdit::OnPaint() 
{
	CPaintDC dc(this);

	CRect nRect;
	CDC memDC;
	CBitmap memBmp;
	CBitmap *pOldBmp;

	GetClientRect( &nRect );
	memDC.CreateCompatibleDC( &dc );
	memBmp.CreateCompatibleBitmap(&dc,nRect.right,nRect.bottom);
	pOldBmp = memDC.SelectObject(&memBmp);

	PutFrame( &memDC,nRect );
	dc.BitBlt( 0,0,nRect.Width(),nRect.Height(),&memDC,0,0,SRCCOPY );

	memDC.SelectObject(pOldBmp );
	memBmp.DeleteObject();
}

BOOL CMyLineEdit::OnEraseBkgnd(CDC* pDC) 
{
	RECT nRect;
	GetClientRect( &nRect );
	PutFrame( pDC,nRect );
	return TRUE;	
}

void CMyLineEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	m_Edit.SetFocus();	
}
