// TopicPage.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "TopicPage.h"
#include "TopicDialog.h"

#include "TextFile.h"

#include "afxdialogex.h"

CArray<STopic*, STopic*> g_topicArray;
CArray<STopic*, STopic*> g_filteredTopicArray;

void JoinString(const CStringArray& sItems, CString sSplit, CString& sText) {
	if (sItems.IsEmpty()) sText = _T("");
	else {
		int nCount = sItems.GetCount();
		for (int i = 0; i < nCount; i++) {
			if (i == 0) sText = sItems.GetAt(i);
			else sText = sText + _T("\t") + sItems.GetAt(i);
		}
	}
}

void SplitString(CString sText, CString sSplit, CStringArray& sItems) {
	sItems.RemoveAll();

	int nPos = sText.FindOneOf(sSplit);
	while (nPos >= 0) {
		sItems.Add(sText.Left(nPos));
		sText = sText.Right(sText.GetLength() - nPos - 1);

		nPos = sText.FindOneOf(sSplit);
	}
	sItems.Add(sText);
}

// CTopicPage dialog
#define USER_MESSAGE		WM_USER + 4000

IMPLEMENT_DYNAMIC(CTopicPage, CPropertyPage)

CTopicPage::CTopicPage()
	: CPropertyPage(CTopicPage::IDD)
{

}

CTopicPage::~CTopicPage()
{
}

void CTopicPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTopicPage, CPropertyPage)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()

	ON_MESSAGE(USER_MESSAGE + 01, OnAddBtn)
	ON_MESSAGE(USER_MESSAGE + 02, OnBackBtn)
	ON_MESSAGE(USER_MESSAGE + 03, OnNextBtn)
	ON_MESSAGE(USER_MESSAGE + 10, OnEditBtn)
	ON_MESSAGE(USER_MESSAGE + 11, OnDeleteBtn)

	ON_EN_CHANGE(USER_MESSAGE + 20, &CTopicPage::OnEnChangeSearchEdit)
END_MESSAGE_MAP()


// CTopicPage message handlers


int CTopicPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateWindows();
	LoadTopics();

	return 0;
}


void CTopicPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	CBrush brush(RGB(153, 255, 237));
	dc.FillRect(&rect, &brush);
}


void CTopicPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	m_addButton.MoveWindow(10, cy - 35, 160, 25);
	m_backButton.MoveWindow(cx - 140, cy - 35, 50, 25);
	m_nextButton.MoveWindow(cx - 70, cy - 35, 50, 25);

	m_searchLabel.MoveWindow(20, cy - 70,  70, 20);
	m_searchEdit.MoveWindow(100, cy - 70, 200, 20);
}

void CTopicPage::CreateWindows() {
	CRect rect;
	GetClientRect(&rect);

	UINT nResourceID[4];
	
	nResourceID[0] = IDB_NEW_TOPIC_UP;
	nResourceID[1] = IDB_NEW_TOPIC_MOVE;
	nResourceID[2] = IDB_NEW_TOPIC_MOVE;
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

	CRect searchLabelRect(10, rect.Height() - 60, 10 + 70, rect.Height() - 40);
	m_searchLabel.Create(this, &searchLabelRect, _T("Search"));

	CRect searchEditRect(90, rect.Height() - 70, 90 + 120, rect.Height() - 30);
	m_searchEdit.Create(WS_VISIBLE | WS_CHILD | ES_LEFT, searchEditRect, this, USER_MESSAGE + 20);

	CFont font;
	LOGFONT logFont;
	memset( &logFont, 0, sizeof(LOGFONT) );

	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = -16;
	logFont.lfWeight = 400;
	logFont.lfPitchAndFamily = 50;
	wcscpy_s(logFont.lfFaceName, _T("Arial")); 
	font.CreateFontIndirect(&logFont);

	m_searchEdit.SetFont(&font);

	CreatePageControls();
}

void CTopicPage::CreatePageControls() {
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

		m_topicLabels[nIdx] = new CMyLabel;
		CRect topicRect(nXPos + 60, nYPos, nXPos + 60 + 300, nYPos + 25);
		m_topicLabels[nIdx]->Create(this, &topicRect, CString());

		m_editButtons[nIdx] = new CMyBitmapButton;

		CRect editBtnRect(nXPos + 380, nYPos, nXPos + 380 + 80, nYPos + 25);
		m_editButtons[nIdx]->Create(this, nEditResID, &editBtnRect, USER_MESSAGE + 10);

		m_deleteButtons[nIdx] = new CMyBitmapButton;

		CRect deleteBtnRect(nXPos + 470, nYPos, nXPos + 470 + 80, nYPos + 25);
		m_deleteButtons[nIdx]->Create(this, nDelResID, &deleteBtnRect, USER_MESSAGE + 11);

		nYPos += 35;
	}
}

void CTopicPage::SaveTopics() {
	TCHAR szFilePath[512];
	TCHAR szDirectory[512];
	GetCurrentDirectory(512, szDirectory);

	wsprintf(szFilePath, _T("%s\\Data\\Base\\Topic.txt"), szDirectory);

	CTextFileWrite file(szFilePath);
	int count = g_topicArray.GetCount();
	for (int i = 0; i < count; i++) {
		STopic* pTopic = g_topicArray.GetAt(i);

		file.Write(pTopic->mName);
		file.WriteEndl();
		file.Write(pTopic->mDescription);
		file.WriteEndl();
		
		CString sGradeStr;
		JoinString(pTopic->mGrades, _T("\t"), sGradeStr);
		file.Write(sGradeStr);
		file.WriteEndl();
	}
	file.Close();
}

void CTopicPage::LoadTopics() {
	TCHAR szFilePath[512];
	TCHAR szDirectory[512];
	GetCurrentDirectory(512, szDirectory);

	wsprintf(szFilePath, _T("%s\\Data\\Base\\Topic.txt"), szDirectory);

	CTextFileRead file(szFilePath);
	int index = 0;

	CString sTopic, sGrade, sDescription;
	CStringArray sGradeArray;

	g_topicArray.RemoveAll();
	g_filteredTopicArray.RemoveAll();

	while (!file.Eof()) {
		if (index % 3 == 0) {
			file.ReadLine(sTopic);
		} else if (index % 3 == 1) {
			file.ReadLine(sDescription);
		} else {
			file.ReadLine(sGrade);

			STopic* pTopic = new STopic;
			pTopic->mName = sTopic;
			pTopic->mDescription = sDescription;

			SplitString(sGrade, _T("\t"), pTopic->mGrades);
			g_topicArray.Add(pTopic);
			g_filteredTopicArray.Add(pTopic);
		}
		index++;
	}
	file.Close();

	m_nPageCount = g_filteredTopicArray.GetCount() / 20 + 1;
	m_nPageIndex = 0;

	MovePage(m_nPageIndex);
	VisibleProc();
}

void CTopicPage::MovePage(int nPageIndex)
{
	int nStartIndex = nPageIndex * 20;
	int nEndIndex = nStartIndex + 19;

	if (nEndIndex >= g_filteredTopicArray.GetCount()) nEndIndex = g_filteredTopicArray.GetCount() - 1;

	TCHAR szNo[10];
	int nCtrlIdx = 0;
	for (int i = nStartIndex; i <= nEndIndex; i++, nCtrlIdx++) {
		STopic* pTopic = g_filteredTopicArray.GetAt(i);
		wsprintf(szNo, _T("%d"), i + 1);
		
		m_noLabels[nCtrlIdx]->SetText(szNo);
		m_topicLabels[nCtrlIdx]->SetText(pTopic->mName);
		
		m_noLabels[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_topicLabels[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_editButtons[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_deleteButtons[nCtrlIdx]->ShowWindow(SW_SHOW);
	}

	for (int i = nCtrlIdx; i < 20; i++) {
		m_noLabels[i]->ShowWindow(SW_HIDE);
		m_topicLabels[i]->ShowWindow(SW_HIDE);
		m_editButtons[i]->ShowWindow(SW_HIDE);
		m_deleteButtons[i]->ShowWindow(SW_HIDE);
	}
}

void CTopicPage::VisibleProc() {
	BOOL bPageControlVisible = (g_filteredTopicArray.GetCount() > 20);
	/*
	m_backButton.ShowWindow(bPageControlVisible ? SW_SHOW : SW_HIDE);
	m_nextButton.ShowWindow(bPageControlVisible ? SW_SHOW : SW_HIDE);*/
}

LRESULT CTopicPage::OnAddBtn(WPARAM wParam, LPARAM lParam) {
	CTopicDialog dlg;
	dlg.SetNewTopic(TRUE);

	if (dlg.DoModal() == IDOK) {
		CString sName = dlg.GetName();
		CString sDesc = dlg.GetDesc();
		CString sGrade = dlg.GetGrades();

		STopic* pTopic = new STopic;
		pTopic->mName = sName;
		pTopic->mDescription = sDesc;
		SplitString(sGrade, _T("\t"), pTopic->mGrades);

		g_topicArray.Add(pTopic);
		g_filteredTopicArray.Add(pTopic);

		SaveTopics();

		m_nPageCount = g_topicArray.GetCount() / 20 + 1;
		m_nPageIndex = m_nPageCount - 1;

		MovePage(m_nPageIndex);
	}
	return 0;
}

LRESULT CTopicPage::OnDeleteBtn(WPARAM wParam, LPARAM lParam) {
	int nIdx = -1;
	for (int i = 0; i < 20; i++) {
		if (m_deleteButtons[i] == (CMyBitmapButton*)(wParam)) {
			nIdx = i;
			break;
		}
	}

	if (nIdx >= 0) {
		nIdx += m_nPageIndex * 20;

		STopic* pTopic = g_filteredTopicArray.GetAt(nIdx);
		g_filteredTopicArray.RemoveAt(nIdx);

		int nIndexOfTotal = GetIndexOfTotal(pTopic);
		g_topicArray.RemoveAt(nIdx);

		delete pTopic, pTopic = 0;

		m_nPageCount = g_filteredTopicArray.GetCount() / 20 + 1;

		MovePage(m_nPageIndex);
		VisibleProc();

		SaveTopics();
	}
	return 0;
}

LRESULT CTopicPage::OnEditBtn(WPARAM wParam, LPARAM lParam) {
	int nIdx = -1;
	for (int i = 0; i < 20; i++) {
		if (m_editButtons[i] == (CMyBitmapButton*)(wParam)) {
			nIdx = i;
			break;
		}
	}

	if (nIdx >= 0) {
		STopic* pTopic = g_filteredTopicArray.GetAt(m_nPageIndex * 20 + nIdx);
		CTopicDialog dlg;
		dlg.SetName(pTopic->mName);
		dlg.SetDesc(pTopic->mDescription);
		
		CString sGradeStr;
		JoinString(pTopic->mGrades, _T("\t"), sGradeStr);
		dlg.SetGrades(sGradeStr);

		if (dlg.DoModal() == IDOK) {
			pTopic->mName = dlg.GetName();
			pTopic->mDescription = dlg.GetDesc();
			SplitString(dlg.GetGrades(), _T("\t"), pTopic->mGrades);

			m_topicLabels[nIdx]->SetText(pTopic->mName);
			SaveTopics();
		}
	}
	return 0;
}

LRESULT CTopicPage::OnBackBtn(WPARAM wParam, LPARAM lParam) {
	if (m_nPageIndex > 0) {
		m_nPageIndex--;
		MovePage(m_nPageIndex);
	}
	return 0;
}

LRESULT CTopicPage::OnNextBtn(WPARAM wParam, LPARAM lParam) {
	if (m_nPageIndex < m_nPageCount - 1) {
		m_nPageIndex++;
		MovePage(m_nPageIndex);
	}
	return 0;
}

int CTopicPage::GetIndexOfTotal(STopic* pTopic) {
	int nIdx = -1;
	for (int i = 0; i < g_topicArray.GetCount(); i++) {
		if (pTopic == g_topicArray.GetAt(i)) {
			return i;
		}
	}
	return -1;
}

void CTopicPage::OnEnChangeSearchEdit() {
	CString sFind;
	m_searchEdit.GetWindowText(sFind);
	sFind = sFind.MakeUpper();
	g_filteredTopicArray.RemoveAll();
	for(int i = 0; i<g_topicArray.GetSize(); i++){
		STopic* pTopic = g_topicArray.GetAt(i);

		CString mName = pTopic->mName;
		mName = mName.MakeUpper();

		if(mName.Find(sFind) >= 0) {
			g_filteredTopicArray.Add(pTopic);
		}
		m_nPageCount = g_filteredTopicArray.GetCount()/20 + 1;
		m_nPageIndex = 0;
		MovePage(m_nPageIndex);
		VisibleProc();
	}

}