#include "stdafx.h"
#include "MyLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//***************************************************************************
// CMyLabel
//***************************************************************************

CMyLabel::CMyLabel()
{
	m_textAlignment = Left;

	memset( &m_logFont, 0, sizeof(LOGFONT) );
	memset( &m_backColor, 0, sizeof(COLORREF));
	memset( &m_textColor, 0, sizeof(COLORREF));

	m_backColor = RGB(153, 255, 237);

	m_logFont.lfCharSet = DEFAULT_CHARSET;
	m_logFont.lfHeight = -16;
	m_logFont.lfWeight = 400;
	m_logFont.lfPitchAndFamily = 50;
	wcscpy_s(m_logFont.lfFaceName, _T("Arial")); 
}

//***************************************************************************
CMyLabel::~CMyLabel()
{
	DestroyWindow();
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
BOOL CMyLabel::Create(CWnd *pParent,RECT *nRect,CString szStr)
{
	memcpy( &m_nRect,nRect,sizeof(RECT) );
	m_nMessage = szStr;

	m_nSize.cx = m_nRect.right - m_nRect.left;
	m_nSize.cy = m_nRect.bottom - m_nRect.top;

	BOOL ret = CWnd::Create(NULL,L"Label",
		WS_CHILD | WS_VISIBLE ,
		m_nRect, pParent, IDC_STATIC,NULL);
	EnableWindow( FALSE );

	return ret;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
BEGIN_MESSAGE_MAP(CMyLabel, CWnd)
	//{{AFX_MSG_MAP(CMyLabel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//***************************************************************************
void CMyLabel::OnPaint() 
{
	PutText();
}

//***************************************************************************
void CMyLabel::PutText()
{
	CPaintDC dc(this); 
	WCHAR str[5][40], buf[255];
	wcscpy(buf, (LPCTSTR)m_nMessage );
	int n;
	WCHAR *p,*temp = buf;
	int i;
	for( i= 0 ; true; i++ )
	{
		str[i][0] = 0; 
		p = wcsstr(temp, L"\n");
		if( p && p - temp < 35 )
		{
			p[0] = 0;
			wcscpy( str[i],temp );
			temp = p+1;
		}
		else
		{
			wcsncat(str[i],temp,35 );
			str[i][35] = 0;
			if( wcslen( str[i] ) < 35 )
				break;
			temp += 35;
		}
	}
	n = i+1;
	CSize tsize;

	CFont font;
	font.CreateFontIndirect( &m_logFont );
	dc.SelectObject(&font);
	font.DeleteObject();

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_textColor);

	CRect rect;
	GetClientRect(&rect);
	CBrush brush(m_backColor);

	dc.FillRect(&rect, &brush);

	if (m_textAlignment == Left) {
		for(i = 0; i < n; i++ )
		{
			tsize = dc.GetTextExtent(CString(str[i]));
			dc.TextOut(0, ((m_nSize.cy - tsize.cy) / 2 - n / 2 * tsize.cy) + i * tsize.cy, CString(str[i]));
		}
	} else if (m_textAlignment == Center) {
		for(i = 0; i < n; i++ )
		{
			tsize = dc.GetTextExtent(CString(str[i]));
			dc.TextOut((m_nSize.cx - tsize.cx) / 2, ((m_nSize.cy - tsize.cy) / 2 - n / 2 * tsize.cy) + i * tsize.cy, CString(str[i]));
		}
	} else {
		for(i = 0; i < n; i++ )
		{
			tsize = dc.GetTextExtent(CString(str[i]));
			dc.TextOut(m_nSize.cx - tsize.cx, ((m_nSize.cy - tsize.cy) / 2 - n / 2 * tsize.cy) + i * tsize.cy, CString(str[i]));
		}
	}
}

void CMyLabel::SetTextColor(const COLORREF& color) {
	m_textColor = color;
	Invalidate();
}

void CMyLabel::SetBackColor(const COLORREF& color) {
	m_backColor = color;
	Invalidate();
}

void CMyLabel::SetFont(const LOGFONT& font) {
	m_logFont = font;
	Invalidate();
}

void CMyLabel::SetText(const CString& text) {
	m_nMessage = text;
	Invalidate();
}