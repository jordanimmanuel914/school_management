#include "stdafx.h"
#include "LogoWnd.h"

//***************************************************************************
//***************************************************************************
//***************************************************************************
BEGIN_MESSAGE_MAP(CLogoWnd, CWnd)
	//{{AFX_MSG_MAP(CMyLabel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CLogoWnd::CLogoWnd(void)
{
}


CLogoWnd::~CLogoWnd(void)
{
}

void CLogoWnd::OnPaint() {
	CClientDC dc(this);
	
	CRect rect;
	GetClientRect(&rect);

	if (m_dib.GetSafeHandle() == 0) {
		CBrush brush(RGB(100, 100, 100));
		dc.FillRect(&rect, &brush);
		brush.DeleteObject();
	} else {
		m_dib.Stretch(&dc, CPoint(0, 0), CSize(rect.Width(), rect.Height()));
	}
	
}

void CLogoWnd::SetFilePath(const CString& sFilePath) {
	m_sFilePath = sFilePath;

	m_dib.DeleteObject();
	m_dib.Load(sFilePath);

	Invalidate();
}

BOOL CLogoWnd::Create(CWnd *pParent,RECT *nRect)
{
	memcpy( &m_rect, nRect,sizeof(RECT) );

	BOOL ret = CWnd::Create(NULL,L"Label",
		WS_CHILD | WS_VISIBLE ,
		m_rect, pParent, IDC_STATIC,NULL);
	EnableWindow( TRUE );

	return ret;
}

void CLogoWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_ENABLESIZING | OFN_EXPLORER | OFN_READONLY | OFN_PATHMUSTEXIST | OFN_NODEREFERENCELINKS | OFN_LONGNAMES | OFN_PATHMUSTEXIST,
		_T("CSV Files (*.bmp)|*.bmp|"), this);
	dlg.m_ofn.lpstrTitle = _T("Choose Logo File");
	if (dlg.DoModal() == IDOK) {
		SetFilePath(dlg.GetFolderPath() + _T("\\") + dlg.GetFileName());
	}
	CWnd::OnLButtonUp(nFlags, point);
}
