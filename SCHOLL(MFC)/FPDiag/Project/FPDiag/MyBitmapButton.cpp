#include "stdafx.h"
#include "MyBitmapButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMyBitmapButton::CMyBitmapButton()
{
	m_nSelID = 0;
	m_nMouseflag = 0;
	m_bTabStop = FALSE;
	m_bKeyFlag = FALSE;	
}

//***************************************************************************
CMyBitmapButton::~CMyBitmapButton()
{
	DestroyWindow();
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
void CMyBitmapButton::SetBitmap( UINT *pBmpID )
{
	// Check Error
	ASSERT( pBmpID );
	// Set the value of Bitmap's ID
	m_nBitmapID[0] = pBmpID[0];
	m_nBitmapID[1] = pBmpID[1];
	m_nBitmapID[2] = pBmpID[2];
	m_nBitmapID[3] = pBmpID[3];
}

BOOL CMyBitmapButton::Create( CWnd *pParent,UINT *pBmpID,RECT *nRect,UINT nCtlID)
{
	// Check Error
	ASSERT( !this->m_hWnd );
	ASSERT( pBmpID );
	// Set the value of Bitmap's ID
	m_nBitmapID[0] = pBmpID[0];
	m_nBitmapID[1] = pBmpID[1];
	m_nBitmapID[2] = pBmpID[2];
	m_nBitmapID[3] = pBmpID[3];
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
	BOOL ret;
	if(m_bTabStop)
	{
		ret = CWnd::Create(NULL, L"ButtonImage",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP ,
			m_nRect, pParent, m_nCtlID, NULL);
	}
	else
	{
		ret = CWnd::Create(NULL, L"ButtonImage",
			WS_CHILD | WS_VISIBLE ,
			m_nRect, pParent, m_nCtlID, NULL);
	}
	return ret;
}

//***************************************************************************
void CMyBitmapButton::PutWindow( CDC *pDC )
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
void CMyBitmapButton::SetCursor(UINT hCursor)
{
	m_hCursor=AfxGetApp()->LoadCursor( hCursor );
}

//***************************************************************************
void CMyBitmapButton::EnableWindow(BOOL bEnable)
{
	ASSERT(this->m_hWnd);
	if( bEnable )
		m_nSelID = 0;
	else
		m_nSelID = 3;
	PutBitmap();
	CWnd::EnableWindow( bEnable );
}
//***************************************************************************
void CMyBitmapButton::PutBitmap(int nID)
{
	m_nSelID = nID;
	PutBitmap();
}
//***************************************************************************
void CMyBitmapButton::PutBitmap()
{
	if( m_nBitmapID[m_nSelID] == 0 )
	{
		return;
	}
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
//***************************************************************************
//***************************************************************************
BEGIN_MESSAGE_MAP(CMyBitmapButton, CWnd)
	//{{AFX_MSG_MAP(CMyBitmapButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_GETDLGCODE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//***************************************************************************
void CMyBitmapButton::OnPaint() 
{
	CPaintDC dc(this);
	PutBitmap( );
}

//***************************************************************************
void CMyBitmapButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	m_nMouseflag = 1;
	m_nSelID = 1;
	PutBitmap();
	SetCapture();
}

//***************************************************************************
void CMyBitmapButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( !m_nMouseflag )
	{
		ReleaseCapture();
		return;
	}

	if (GetCapture() != this)
		return;
	m_nSelID = 0;
	PutBitmap();
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
		GetParent()->SendMessage(m_nCtlID, (WPARAM)this);
}

//***************************************************************************
void CMyBitmapButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd *pWnd = GetCapture();
	if (pWnd != this)
	{
		if( pWnd )
			return;
		if( !pWnd && (nFlags == MK_LBUTTON || nFlags == MK_RBUTTON || nFlags == MK_MBUTTON ) )
			return;
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
		m_nSelID = 0;
		PutBitmap();
		if( nFlags != MK_LBUTTON )
			ReleaseCapture();
		return;
	}
	if( nFlags == MK_LBUTTON )
	{
		if( m_nMouseflag )
			m_nSelID = 1;
		else
			m_nSelID = 2;
	}
	else
	{
		//		SetFocus();
		m_nSelID = 2;
	}
	PutBitmap();
}

//***************************************************************************
BOOL CMyBitmapButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if ( m_hCursor )
	{	
		::SetCursor( m_hCursor );
	}
	else
	{
		::SetCursor( AfxGetApp()->LoadStandardCursor(IDC_ARROW ) );
	}
	return TRUE;
}

//***************************************************************************
void CMyBitmapButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags, point);
	//	CWnd::OnLButtonDblClk(nFlags, point);
}


UINT CMyBitmapButton::OnGetDlgCode() 
{
	if( m_bTabStop )
	{
		if( GetFocus() == this )
			return DLGC_BUTTON;	
		//			return DLGC_DEFPUSHBUTTON | DLGC_BUTTON;	
		else
			return DLGC_UNDEFPUSHBUTTON;// | DLGC_BUTTON;	
	}
	else
		return DLGC_STATIC;//CWnd::OnGetDlgCode();
}

void CMyBitmapButton::OnSetFocus(CWnd* pOldWnd) 
{
	if( !m_bTabStop )
	{
		m_nSelID = 0;
		//		SendMessage(WM_KILLFOCUS,(WPARAM)(this->m_hWnd),0 );
		return;
	}

	CWnd::OnSetFocus(pOldWnd);

	m_nSelID = 2;
	PutBitmap();
}

void CMyBitmapButton::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	if(m_nSelID == 3 )
		return;
	m_nSelID = 0;
	PutBitmap();
}

void CMyBitmapButton::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(!m_bKeyFlag )
		return;	
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
		*/
		break;
	case VK_SPACE:
		bRet = TRUE;
		SendMessage(WM_KILLFOCUS,(WPARAM)(this->m_hWnd),0);
		GetParent()->SendMessage(m_nCtlID);
		break;
	default:
		bRet = FALSE;
		break;
	}
	m_bKeyFlag = FALSE;	
	//	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMyBitmapButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bKeyFlag = TRUE;	

	//	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CMyBitmapButton::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			SendMessage(WM_KILLFOCUS,(WPARAM)(this->m_hWnd),0);
			GetParent()->PostMessage(m_nCtlID);
			return TRUE;
		}
		if( pMsg->wParam >= 0x25 && pMsg->wParam <= 0x28 &&
			!(GetParent()->GetExStyle() & WS_EX_RIGHT) )
			return TRUE;
	}	

	return CWnd::PreTranslateMessage(pMsg);
}
LRESULT CMyBitmapButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_IME_COMPOSITION )
	{
		return TRUE;
	}

	return CWnd::DefWindowProc(message, wParam, lParam);
}

