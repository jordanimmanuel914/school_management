// HomePage.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "HomePage.h"
#include "afxdialogex.h"


// CHomePage dialog

IMPLEMENT_DYNAMIC(CHomePage, CPropertyPage)

CHomePage::CHomePage()
	: CPropertyPage(CHomePage::IDD)
{

}

CHomePage::~CHomePage()
{
}

void CHomePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHomePage, CPropertyPage)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CHomePage message handlers


int CHomePage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}


void CHomePage::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	CBrush brush(RGB(153, 255, 237));
	dc.FillRect(&rect, &brush);
}


void CHomePage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}
