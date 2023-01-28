// GradePage.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "GradePage.h"
#include "TextFile.h"
#include "GradeDialog.h"

#define USER_MESSAGE		WM_USER + 3000

CArray<SGrade*, SGrade*> g_gradeArray;

// CGradePage

IMPLEMENT_DYNAMIC(CGradePage, CPropertyPage)

CGradePage::CGradePage() :
	CPropertyPage(CGradePage::IDD)
{

}

CGradePage::~CGradePage()
{
}


void CGradePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGradePage, CPropertyPage)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()

	ON_MESSAGE(USER_MESSAGE + 01, OnAddBtn)
	ON_MESSAGE(USER_MESSAGE + 02, OnBackBtn)
	ON_MESSAGE(USER_MESSAGE + 03, OnNextBtn)
	ON_MESSAGE(USER_MESSAGE + 10, OnEditBtn)
	ON_MESSAGE(USER_MESSAGE + 11, OnDeleteBtn)
END_MESSAGE_MAP()



void CGradePage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	m_addButton.MoveWindow(10, cy - 35, 160, 25);
	m_backButton.MoveWindow(cx - 140, cy - 35, 50, 25);
	m_nextButton.MoveWindow(cx - 70, cy - 35, 50, 25);
}


int CGradePage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateWindows();
	LoadGrade();

	return 0;
}

void CGradePage::CreateWindows() 
{
	CRect rect;
	GetClientRect(&rect);

	UINT nResourceID[4];
	
	nResourceID[0] = IDB_NEW_GRADE_UP;
	nResourceID[1] = IDB_NEW_GRADE_FOCUS;
	nResourceID[2] = IDB_NEW_GRADE_FOCUS;
	nResourceID[3] = 0;

	CRect addBtnRect(10, rect.Height() - 35, 170, rect.Height() - 10);
	m_addButton.Create(this, nResourceID, &addBtnRect, USER_MESSAGE + 01);

	nResourceID[0] = IDB_BACK_UP;
	nResourceID[1] = IDB_BACK_FOCUS;
	nResourceID[2] = IDB_BACK_FOCUS;
	nResourceID[3] = 0;

	CRect backBtnRect(rect.Width() - 130, rect.Height() - 35, rect.Width() - 80, rect.Height() - 10);
	m_backButton.Create(this, nResourceID, &backBtnRect, USER_MESSAGE + 02);

	nResourceID[0] = IDB_NEXT_UP;
	nResourceID[1] = IDB_NEXT_FOCUS;
	nResourceID[2] = IDB_NEXT_FOCUS;
	nResourceID[3] = 0;
	
	CRect nextBtnRect(rect.Width() - 70, rect.Height() - 35, rect.Width() - 20, rect.Height() - 10);
	m_nextButton.Create(this, nResourceID, &nextBtnRect, USER_MESSAGE + 03);

	CreatePageControls();
}

void CGradePage::SaveGrade(void) 
{
	TCHAR szFilePath[512];
	TCHAR szDirectory[512];
	GetCurrentDirectory(512, szDirectory);

	wsprintf(szFilePath, _T("%s\\Data\\Base\\Grade.txt"), szDirectory);

	CTextFileWrite file(szFilePath);
	int count = g_gradeArray.GetCount();
	for (int i = 0; i < count; i++) {
		SGrade* pGrade = g_gradeArray.GetAt(i);

		file.Write(pGrade->mName);
		file.WriteEndl();
		file.Write(pGrade->mDescription);
		file.WriteEndl();
	}
	file.Close();
}

void CGradePage::LoadGrade(void)
{
	TCHAR szFilePath[512];
	TCHAR szDirectory[512];
	GetCurrentDirectory(512, szDirectory);

	wsprintf(szFilePath, _T("%s\\Data\\Base\\Grade.txt"), szDirectory);

	CTextFileRead file(szFilePath);
	int index = 0;

	CString sGrade, sDescription;

	g_gradeArray.RemoveAll();
	while (!file.Eof()) {
		if (index % 2 == 0) {
			file.ReadLine(sGrade);
		} else {
			file.ReadLine(sDescription);

			SGrade* pGrade = new SGrade;
			pGrade->mName = sGrade;
			pGrade->mDescription = sDescription;

			g_gradeArray.Add(pGrade);
		}
		index++;
	}
	file.Close();

	m_nPageCount = g_gradeArray.GetCount() / 20 + 1;
	m_nPageIndex = 0;

	MovePage(m_nPageIndex);

	VisibleProc();
}

void CGradePage::CreatePageControls() {
	int nYPos = 20;
	int nXPos = 0;

	UINT nEditResID[4] = {
		IDB_EDIT_UP,
		IDB_EDIT_FOCUS,
		IDB_EDIT_FOCUS,
		0
	};
	UINT nDelResID[4] = {
		IDB_DELETE_UP,
		IDB_DELETE_FOCUS,
		IDB_DELETE_FOCUS,
		0
	};

	CRect rect;
	GetClientRect(&rect);

	CFont* pFont = GetFont();

	TCHAR szNo[10];

	for (int nIdx = 0; nIdx < 20; nIdx++) {
		if (nIdx == 10) {
			nXPos += 560;
			nYPos = 20;
		}

		m_noLabels[nIdx] = new CMyLabel;
		wsprintf(szNo, _T("%d"), nIdx + 1);
		CRect noRect(nXPos + 20, nYPos, nXPos + 20 + 50, nYPos + 25);
		m_noLabels[nIdx]->Create(this, &noRect, CString(szNo));

		m_gradeLabels[nIdx] = new CMyLabel;
		CRect gradeRect(nXPos + 90, nYPos, nXPos + 90 + 200, nYPos + 25);
		m_gradeLabels[nIdx]->Create(this, &gradeRect, CString());

		m_editButtons[nIdx] = new CMyBitmapButton;

		CRect editBtnRect(nXPos + 330, nYPos, nXPos + 330 + 80, nYPos + 25);
		m_editButtons[nIdx]->Create(this, nEditResID, &editBtnRect, USER_MESSAGE + 10);

		m_deleteButtons[nIdx] = new CMyBitmapButton;

		CRect deleteBtnRect(nXPos + 420, nYPos, nXPos + 420 + 80, nYPos + 25);
		m_deleteButtons[nIdx]->Create(this, nDelResID, &deleteBtnRect, USER_MESSAGE + 11);

		nYPos += 35;
	}
}

void CGradePage::MovePage(int nPageIndex) {
	int nStartIndex = nPageIndex * 20;
	int nEndIndex = nStartIndex + 19;

	if (nEndIndex >= g_gradeArray.GetCount()) nEndIndex = g_gradeArray.GetCount() - 1;

	TCHAR szNo[10];
	int nCtrlIdx = 0;
	for (int i = nStartIndex; i <= nEndIndex; i++, nCtrlIdx++) {
		SGrade* pGrade = g_gradeArray.GetAt(i);
		wsprintf(szNo, _T("%d"), i + 1);
		
		m_noLabels[nCtrlIdx]->SetText(szNo);
		m_gradeLabels[nCtrlIdx]->SetText(pGrade->mName);
		
		m_noLabels[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_gradeLabels[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_editButtons[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_deleteButtons[nCtrlIdx]->ShowWindow(SW_SHOW);
	}

	for (int i = nCtrlIdx; i < 20; i++) {
		m_noLabels[i]->ShowWindow(SW_HIDE);
		m_gradeLabels[i]->ShowWindow(SW_HIDE);
		m_editButtons[i]->ShowWindow(SW_HIDE);
		m_deleteButtons[i]->ShowWindow(SW_HIDE);
	}
}

void CGradePage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);

	CBrush brush(RGB(153, 255, 237));
	dc.FillRect(&rect, &brush);
}

void CGradePage::VisibleProc() {
	BOOL bPageControlVisible = (g_gradeArray.GetCount() > 20);
	/*
	m_backButton.ShowWindow(bPageControlVisible ? SW_SHOW : SW_HIDE);
	m_nextButton.ShowWindow(bPageControlVisible ? SW_SHOW : SW_HIDE);*/
}

LRESULT CGradePage::OnAddBtn(WPARAM wParam, LPARAM lParam) {
	CGradeDialog dlg;
	dlg.SetNewGrade(TRUE);

	if (dlg.DoModal() == IDOK) {
		CString sGrade = dlg.GetGrade();
		CString sDesc = dlg.GetDescription();

		SGrade* pGrade = new SGrade;
		pGrade->mName = sGrade;
		pGrade->mDescription = sDesc;

		g_gradeArray.Add(pGrade);

		SaveGrade();

		m_nPageCount = g_gradeArray.GetCount() / 20 + 1;
		m_nPageIndex = m_nPageCount - 1;

		MovePage(m_nPageIndex);
	}
	return 0;
}

LRESULT CGradePage::OnNextBtn(WPARAM wParam, LPARAM lParam) {
	if (m_nPageIndex < m_nPageCount - 1) {
		m_nPageIndex++;
		MovePage(m_nPageIndex);
	}
	return 0;
}

LRESULT CGradePage::OnBackBtn(WPARAM wParam, LPARAM lParam) {
	if (m_nPageIndex > 0) {
		m_nPageIndex--;
		MovePage(m_nPageIndex);
	}
	return 0;
}

LRESULT CGradePage::OnEditBtn(WPARAM wParam, LPARAM lParam) {
	int nIdx = -1;
	for (int i = 0; i < 20; i++) {
		if (m_editButtons[i] == (CMyBitmapButton*)(wParam)) {
			nIdx = i;
			break;
		}
	}

	if (nIdx >= 0) {
		SGrade* pGrade = g_gradeArray.GetAt(m_nPageIndex * 20 + nIdx);
		CGradeDialog dlg;
		dlg.SetGrade(pGrade->mName);
		dlg.SetDescription(pGrade->mDescription);

		if (dlg.DoModal() == IDOK) {
			pGrade->mName = dlg.GetGrade();
			pGrade->mDescription = dlg.GetDescription();

			m_gradeLabels[nIdx]->SetText(pGrade->mName);
			SaveGrade();
		}
	}
	return 0;
}

LRESULT CGradePage::OnDeleteBtn(WPARAM wParam, LPARAM lParam) {
	int nIdx = -1;
	for (int i = 0; i < 20; i++) {
		if (m_deleteButtons[i] == (CMyBitmapButton*)(wParam)) {
			nIdx = i;
			break;
		}
	}

	if (nIdx >= 0) {
		nIdx += m_nPageIndex * 20;

		SGrade* pGrade = g_gradeArray.GetAt(nIdx);
		g_gradeArray.RemoveAt(nIdx);

		delete pGrade, pGrade = 0;

		m_nPageCount = g_gradeArray.GetCount() / 20 + 1;

		MovePage(m_nPageIndex);
		VisibleProc();

		SaveGrade();
	}
	return 0;
}