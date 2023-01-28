#include "stdafx.h"
#include "MyRadioButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//***************************************************************************
// CMyRadioButton
//***************************************************************************

CMyRadioButton::CMyRadioButton()
{
	m_nSelID = 0;
	m_nMouseflag = 0;
	m_nCheck = false;
	m_bTabStop = FALSE;
}

//***************************************************************************
CMyRadioButton::~CMyRadioButton()
{
	DestroyWindow();

}
//***************************************************************************
//***************************************************************************
//***************************************************************************
BOOL CMyRadioButton::Create( CWnd *pParent,UINT *pBmpID,RECT *nRect,UINT nCtlID)
{
	// Check Error
	ASSERT( !this->m_hWnd );
	ASSERT( pBmpID );
	// Set the value of Bitmap's ID
	m_nBitmapID[0] = pBmpID[0];
	m_nBitmapID[1] = pBmpID[1];
	m_nBitmapID[2] = pBmpID[2];
	m_nBitmapID[3] = pBmpID[3];
	m_nBitmapID[4] = pBmpID[4];
	m_nBitmapID[5] = pBmpID[5];
	// Set the position of button
	memcpy( &m_nRect,nRect,sizeof(RECT) );
	m_nSize.cx = m_nRect.right - m_nRect.left;
	m_nSize.cy = m_nRect.bottom - m_nRect.top;
//	m_nSize.cx = m_nRect.right;
//	m_nSize.cy = m_nRect.bottom;
//	m_nRect.right += m_nRect.left;
//	m_nRect.bottom += m_nRect.top;
	// Set Control ID
	m_nCtlID = nCtlID;
	m_hCursor = 0;

	if(m_bTabStop)
		return CWnd::Create(NULL,L"ButtonImage",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP ,
			m_nRect,pParent,m_nCtlID,NULL);
	else
		return CWnd::Create(NULL,L"ButtonImage",
			WS_CHILD | WS_VISIBLE ,
			m_nRect,pParent,m_nCtlID,NULL);
}

//***************************************************************************
void CMyRadioButton::SetCursor(UINT hCursor)
{
	m_hCursor=AfxGetApp()->LoadCursor( hCursor );
}

//***************************************************************************
void CMyRadioButton::EnableWindow(BOOL bEnable)
{
	ASSERT(this->m_hWnd);
	if( bEnable )
		m_nSelID = m_nCheck;
	else
		m_nSelID = m_nCheck + 4;
	PutBitmap();
	CWnd::EnableWindow( bEnable );
}

//***************************************************************************
void CMyRadioButton::SetCheck(int nCheck)
{
	m_nCheck = nCheck;
	m_nSelID = m_nCheck;
	PutBitmap();
}

//***************************************************************************
int CMyRadioButton::GetCheck()
{
	return m_nCheck;
}

//***************************************************************************
void CMyRadioButton::PutBitmap()
{
	if( m_nBitmapID[m_nSelID] == 0 )
		return;
	CClientDC dc(this); 

	CDC		dcMem;
	CBitmap bmpMem;
	CBitmap *pOldBmp;
	int		dx,dy,dw,dh;

	CRect nRect;
	GetClientRect( &nRect );
	m_nSize = nRect.Size();

	dx= 0;
	dy = 0;
	dw = m_nSize.cx;
	dh = m_nSize.cy;

	dcMem.CreateCompatibleDC( &dc );
	bmpMem.LoadBitmap( m_nBitmapID[m_nSelID] );
	pOldBmp = dcMem.SelectObject( &bmpMem );

	dc.BitBlt( dx,dy,dw,dh,&dcMem,0,0,SRCCOPY );
	
	dcMem.SelectObject( pOldBmp );
	bmpMem.DeleteObject();
}

//***************************************************************************
void CMyRadioButton::PutWindow( CDC *pDC )
{
	CRect nRect;
	GetWindowRect( &nRect );
	GetParent()->ScreenToClient( &nRect );
	m_nSize = nRect.Size();

	int nID;
	if( m_nBitmapID[m_nSelID] == 0 )
		nID = 0;
	else
		nID = m_nSelID;
	CDC		dcMem;
	CBitmap bmpMem;
	CBitmap *pOldBmp;
	int		dx,dy,dw,dh;

	dx= nRect.left;
	dy = nRect.top;
	dw = m_nSize.cx;
	dh = m_nSize.cy;

	dcMem.CreateCompatibleDC( pDC );
	bmpMem.LoadBitmap( m_nBitmapID[nID] );
	pOldBmp = dcMem.SelectObject( &bmpMem );

	pDC->BitBlt( dx,dy,dw,dh,&dcMem,0,0,SRCCOPY );
	
	dcMem.SelectObject( pOldBmp );
	bmpMem.DeleteObject();
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
BEGIN_MESSAGE_MAP(CMyRadioButton, CWnd)
	//{{AFX_MSG_MAP(CMyRadioButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_GETDLGCODE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//***************************************************************************
void CMyRadioButton::OnPaint() 
{
	CPaintDC dc(this);
	PutBitmap();
}

//***************************************************************************
void CMyRadioButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	m_nMouseflag = 1;
	m_nSelID = 1;
	PutBitmap();
	SetCapture();	
}

//***************************************************************************
void CMyRadioButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( !m_nMouseflag )
	{
		m_nSelID = m_nCheck;
		PutBitmap();
		ReleaseCapture();
		return;
	}

	if (GetCapture() != this)
		return;
	ReleaseCapture();
	m_nMouseflag = 0;

	CRect rect,rect1;	
	rect1 = m_nRect;
	ClientToScreen(&rect1);
	rect = rect1;
	point.x += m_nRect.left;
	point.y += m_nRect.top;
	ClientToScreen(&point);
	if( rect.PtInRect(point ) )
	{
		m_nSelID = 1;
		m_nCheck = 1;
		GetParent()->SendMessage(m_nCtlID);
	}
	else
		m_nSelID = m_nCheck;
	PutBitmap();

}

//***************************************************************************
void CMyRadioButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (GetCapture() != this)
	{
		SetCapture();
	}

	CRect rect,rect1;	
	rect1 = m_nRect;
	ClientToScreen(&rect1);
	rect = rect1;
	point.x += m_nRect.left;
	point.y += m_nRect.top;
	ClientToScreen(&point);

	if( !rect.PtInRect(point ) )
	{
		m_nSelID = m_nCheck;
		PutBitmap();
		if( nFlags != MK_LBUTTON )
			ReleaseCapture();
		return;
	}
	if( m_nMouseflag )
		m_nSelID = 1;
	else
	{
//		SetFocus();
		m_nSelID = m_nCheck + 2;
	}
	PutBitmap();
}

//***************************************************************************
BOOL CMyRadioButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if ( m_hCursor )
	{	
		::SetCursor( m_hCursor );
	}
	else
	::SetCursor( AfxGetApp()->LoadStandardCursor(IDC_ARROW ) );
	return TRUE;
}
UINT CMyRadioButton::OnGetDlgCode() 
{
	if( m_bTabStop )
	{
		if( GetFocus() == this )
			return DLGC_DEFPUSHBUTTON | DLGC_BUTTON;	
		else
			return DLGC_UNDEFPUSHBUTTON | DLGC_BUTTON;	
	}
	else
		return CWnd::OnGetDlgCode();
}

void CMyRadioButton::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);

	m_nSelID = m_nCheck + 2;
	PutBitmap();
}

void CMyRadioButton::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);

	m_nSelID = m_nCheck;
	PutBitmap();
}

void CMyRadioButton::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	static BOOL bRet = FALSE;
	switch(nChar)
	{
	case VK_RETURN:
/*		if( bRet )
		{
			bRet = FALSE;
			break;
		}
		bRet = TRUE;
		SendMessage(WM_KILLFOCUS,(WPARAM)(this->m_hWnd),0);
		GetParent()->SendMessage(m_nCtlID);
*/		break;
	case VK_SPACE:
		bRet = TRUE;
		SendMessage(WM_KILLFOCUS,(WPARAM)(this->m_hWnd),0);
		GetParent()->SendMessage(m_nCtlID);
		break;
	default:
		bRet = FALSE;
		break;
	}
//	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CMyRadioButton::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			SendMessage(WM_KILLFOCUS,(WPARAM)(this->m_hWnd),0);
			GetParent()->PostMessage(m_nCtlID);
			return TRUE;
		}
		if( pMsg->wParam >= 0x25 && pMsg->wParam <= 0x28)
		{
			return TRUE;
		}
	}	
	
	return CWnd::PreTranslateMessage(pMsg);
}
