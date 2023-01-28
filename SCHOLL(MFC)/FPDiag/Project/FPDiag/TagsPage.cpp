// TagPage.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "TagsPage.h"
#include "TextFile.h"
#include "TagDialog.h"

#define USER_MESSAGE		WM_USER + 3000

CArray<STag*, STag*> g_tagArray;

// CTagsPage

IMPLEMENT_DYNAMIC(CTagsPage, CPropertyPage)

CTagsPage::CTagsPage() :
	CPropertyPage(CTagsPage::IDD)
{

}

CTagsPage::~CTagsPage()
{
}


void CTagsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTagsPage, CPropertyPage)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()

	ON_MESSAGE(USER_MESSAGE + 01, OnAddBtn)
	ON_MESSAGE(USER_MESSAGE + 02, OnBackBtn)
	ON_MESSAGE(USER_MESSAGE + 03, OnNextBtn)
	ON_MESSAGE(USER_MESSAGE + 10, OnEditBtn)
	ON_MESSAGE(USER_MESSAGE + 11, OnDeleteBtn)
END_MESSAGE_MAP()



void CTagsPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	m_addButton.MoveWindow(10, cy - 35, 160, 25);
	m_backButton.MoveWindow(cx - 140, cy - 35, 50, 25);
	m_nextButton.MoveWindow(cx - 70, cy - 35, 50, 25);
}


int CTagsPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateWindows();
	LoadTags();

	return 0;
}

void CTagsPage::CreateWindows() 
{
	CRect rect;
	GetClientRect(&rect);

	UINT nResourceID[4];
	
	nResourceID[0] = IDB_NEW_TAG_UP;
	nResourceID[1] = IDB_NEW_TAG_MOVE;
	nResourceID[2] = IDB_NEW_TAG_MOVE;
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

void CTagsPage::SaveTags(void) 
{
	TCHAR szFilePath[512];
	TCHAR szDirectory[512];
	GetCurrentDirectory(512, szDirectory);

	wsprintf(szFilePath, _T("%s\\Data\\Base\\Tags.txt"), szDirectory);

	CTextFileWrite file(szFilePath);
	int count = g_tagArray.GetCount();
	for (int i = 0; i < count; i++) {
		STag* pTag = g_tagArray.GetAt(i);

		file.Write(pTag->mName);
		file.WriteEndl();
		file.Write(pTag->mDescription);
		file.WriteEndl();
	}
	file.Close();
}

void CTagsPage::LoadTags(void)
{
	TCHAR szFilePath[512];
	TCHAR szDirectory[512];
	GetCurrentDirectory(512, szDirectory);

	wsprintf(szFilePath, _T("%s\\Data\\Base\\Tags.txt"), szDirectory);

	CTextFileRead file(szFilePath);
	int index = 0;

	CString sTag, sDescription;

	g_tagArray.RemoveAll();
	while (!file.Eof()) {
		if (index % 2 == 0) {
			file.ReadLine(sTag);
		} else {
			file.ReadLine(sDescription);

			STag* pTag = new STag;
			pTag->mName = sTag;
			pTag->mDescription = sDescription;

			g_tagArray.Add(pTag);
		}
		index++;
	}
	file.Close();

	m_nPageCount = g_tagArray.GetCount() / 20 + 1;
	m_nPageIndex = 0;

	MovePage(m_nPageIndex);

	VisibleProc();
}

void CTagsPage::CreatePageControls() {
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
		CRect noRect(nXPos + 20, nYPos, nXPos + 20 + 20, nYPos + 25);
		m_noLabels[nIdx]->Create(this, &noRect, CString(szNo));

		m_nameLabels[nIdx] = new CMyLabel;
		CRect nameRect(nXPos + 60, nYPos, nXPos + 60 + 300, nYPos + 25);
		m_nameLabels[nIdx]->Create(this, &nameRect, CString());

		m_editButtons[nIdx] = new CMyBitmapButton;

		CRect editBtnRect(nXPos + 380, nYPos, nXPos + 380 + 80, nYPos + 25);
		m_editButtons[nIdx]->Create(this, nEditResID, &editBtnRect, USER_MESSAGE + 10);

		m_deleteButtons[nIdx] = new CMyBitmapButton;

		CRect deleteBtnRect(nXPos + 470, nYPos, nXPos + 470 + 80, nYPos + 25);
		m_deleteButtons[nIdx]->Create(this, nDelResID, &deleteBtnRect, USER_MESSAGE + 11);

		nYPos += 35;
	}
}

void CTagsPage::MovePage(int nPageIndex) {
	int nStartIndex = nPageIndex * 20;
	int nEndIndex = nStartIndex + 19;

	if (nEndIndex >= g_tagArray.GetCount()) nEndIndex = g_tagArray.GetCount() - 1;

	TCHAR szNo[10];
	int nCtrlIdx = 0;
	for (int i = nStartIndex; i <= nEndIndex; i++, nCtrlIdx++) {
		STag* pTag = g_tagArray.GetAt(i);
		wsprintf(szNo, _T("%d"), i + 1);
		
		m_noLabels[nCtrlIdx]->SetText(szNo);
		m_nameLabels[nCtrlIdx]->SetText(pTag->mName);
		
		m_noLabels[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_nameLabels[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_editButtons[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_deleteButtons[nCtrlIdx]->ShowWindow(SW_SHOW);
	}

	for (int i = nCtrlIdx; i < 20; i++) {
		m_noLabels[i]->ShowWindow(SW_HIDE);
		m_nameLabels[i]->ShowWindow(SW_HIDE);
		m_editButtons[i]->ShowWindow(SW_HIDE);
		m_deleteButtons[i]->ShowWindow(SW_HIDE);
	}
}

void CTagsPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);

	CBrush brush(RGB(153, 255, 237));
	dc.FillRect(&rect, &brush);
}

void CTagsPage::VisibleProc() {
	BOOL bPageControlVisible = (g_tagArray.GetCount() > 20);
	/*
	m_backButton.ShowWindow(bPageControlVisible ? SW_SHOW : SW_HIDE);
	m_nextButton.ShowWindow(bPageControlVisible ? SW_SHOW : SW_HIDE);*/
}

LRESULT CTagsPage::OnAddBtn(WPARAM wParam, LPARAM lParam) {
	CTagDialog dlg;
	dlg.SetNewTag(TRUE);

	if (dlg.DoModal() == IDOK) {
		CString sTag = dlg.GetTag();
		CString sDesc = dlg.GetDescription();

		STag* pTag = new STag;
		pTag->mName = sTag;
		pTag->mDescription = sDesc;

		g_tagArray.Add(pTag);

		SaveTags();

		m_nPageCount = g_tagArray.GetCount() / 20 + 1;
		m_nPageIndex = m_nPageCount - 1;

		MovePage(m_nPageIndex);
	}
	return 0;
}

LRESULT CTagsPage::OnNextBtn(WPARAM wParam, LPARAM lParam) {
	if (m_nPageIndex < m_nPageCount - 1) {
		m_nPageIndex++;
		MovePage(m_nPageIndex);
	}
	return 0;
}

LRESULT CTagsPage::OnBackBtn(WPARAM wParam, LPARAM lParam) {
	if (m_nPageIndex > 0) {
		m_nPageIndex--;
		MovePage(m_nPageIndex);
	}
	return 0;
}

LRESULT CTagsPage::OnEditBtn(WPARAM wParam, LPARAM lParam) {
	int nIdx = -1;
	for (int i = 0; i < 20; i++) {
		if (m_editButtons[i] == (CMyBitmapButton*)(wParam)) {
			nIdx = i;
			break;
		}
	}

	if (nIdx >= 0) {
		STag* pTag = g_tagArray.GetAt(m_nPageIndex * 20 + nIdx);
		CTagDialog dlg;
		dlg.SetTag(pTag->mName);
		dlg.SetDescription(pTag->mDescription);

		if (dlg.DoModal() == IDOK) {
			pTag->mName = dlg.GetTag();
			pTag->mDescription = dlg.GetDescription();

			m_nameLabels[nIdx]->SetText(pTag->mName);
			SaveTags();
		}
	}
	return 0;
}

LRESULT CTagsPage::OnDeleteBtn(WPARAM wParam, LPARAM lParam) {
	int nIdx = -1;
	for (int i = 0; i < 20; i++) {
		if (m_deleteButtons[i] == (CMyBitmapButton*)(wParam)) {
			nIdx = i;
			break;
		}
	}

	if (nIdx >= 0) {
		nIdx += m_nPageIndex * 20;

		STag* pTag = g_tagArray.GetAt(nIdx);
		g_tagArray.RemoveAt(nIdx);

		delete pTag, pTag = 0;

		m_nPageCount = g_tagArray.GetCount() / 20 + 1;

		MovePage(m_nPageIndex);
		VisibleProc();

		SaveTags();
	}
	return 0;
}