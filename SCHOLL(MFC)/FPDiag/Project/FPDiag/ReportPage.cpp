// ReportPage.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "ReportPage.h"
#include "ReportDialog.h"
#include "MSWord.h"
#include "TextFile.h"
#include "SendDialog.h"

#include "afxdialogex.h"

CArray<SReport*, SReport*> g_reportArray;
CArray<SReport*, SReport*> g_filteredReportArray;

extern void JoinString(const CStringArray& sItems, CString sSplit, CString& sText);
extern void SplitString(CString sText, CString sSplit, CStringArray& sItems);

// CReportPage dialog
#define USER_MESSAGE		WM_USER + 4000

IMPLEMENT_DYNAMIC(CReportPage, CPropertyPage)

CReportPage::CReportPage()
	: CPropertyPage(CReportPage::IDD)
{
}

CReportPage::~CReportPage()
{
}

void CReportPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReportPage, CPropertyPage)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()

	ON_MESSAGE(USER_MESSAGE + 01, OnAddBtn)
	ON_MESSAGE(USER_MESSAGE + 02, OnBackBtn)
	ON_MESSAGE(USER_MESSAGE + 03, OnNextBtn)
	ON_MESSAGE(USER_MESSAGE + 10, OnEditBtn)
	ON_MESSAGE(USER_MESSAGE + 11, OnDeleteBtn)
	ON_MESSAGE(USER_MESSAGE + 12, OnGenerateBtn)
	ON_EN_CHANGE(USER_MESSAGE + 20, &CReportPage::OnEnChangeSearchEdit)

END_MESSAGE_MAP()


// CReportPage message handlers


int CReportPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateWindows();
	LoadReports();

	return 0;
}


void CReportPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	CBrush brush(RGB(153, 255, 237));
	dc.FillRect(&rect, &brush);
}


void CReportPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	m_addButton.MoveWindow(10, cy - 35, 160, 25);
	m_backButton.MoveWindow(cx - 140, cy - 35, 50, 25);
	m_nextButton.MoveWindow(cx - 70, cy - 35, 50, 25);

	m_searchLabel.MoveWindow(20, cy - 70,  70, 20);
	m_searchEdit.MoveWindow(100, cy - 70, 200, 20);
}

void CReportPage::CreateWindows() {
	CRect rect;
	GetClientRect(&rect);

	UINT nResourceID[4];
	
	nResourceID[0] = IDB_NEW_REPORT_UP;
	nResourceID[1] = IDB_NEW_REPORT_MOVE;
	nResourceID[2] = IDB_NEW_REPORT_MOVE;
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

void CReportPage::CreatePageControls() {
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
	UINT nGenerateResID[4] = {
		IDB_GENERATE_UP,
		IDB_GENERATE_FOCUS,
		IDB_GENERATE_FOCUS,
		0
	};

	CRect rect;
	GetClientRect(&rect);

	CFont* pFont = GetFont();

	TCHAR szNo[10];

	for (int nIdx = 0; nIdx < 12; nIdx++) {
		m_noLabels[nIdx] = new CMyLabel;
		wsprintf(szNo, _T("%d"), nIdx + 1);
		CRect noRect(nXPos + 20, nYPos, nXPos + 20 + 50, nYPos + 25);
		m_noLabels[nIdx]->Create(this, &noRect, CString(szNo));

		m_nameLabels[nIdx] = new CMyLabel;
		CRect nameRect(nXPos + 90, nYPos, nXPos + 90 + 320, nYPos + 25);
		m_nameLabels[nIdx]->Create(this, &nameRect, CString());

		m_dateLabels[nIdx] = new CMyLabel;
		CRect dateRect(nXPos + 430, nYPos, nXPos + 430 + 150, nYPos + 25);
		m_dateLabels[nIdx]->Create(this, &dateRect, CString());

		m_editButtons[nIdx] = new CMyBitmapButton;

		CRect editBtnRect(nXPos + 600, nYPos, nXPos + 600 + 80, nYPos + 25);
		m_editButtons[nIdx]->Create(this, nEditResID, &editBtnRect, USER_MESSAGE + 10);

		m_deleteButtons[nIdx] = new CMyBitmapButton;

		CRect deleteBtnRect(nXPos + 700, nYPos, nXPos + 700 + 80, nYPos + 25);
		m_deleteButtons[nIdx]->Create(this, nDelResID, &deleteBtnRect, USER_MESSAGE + 11);

		m_generateButtons[nIdx] = new CMyBitmapButton;

		CRect generateBtnRect(nXPos + 800, nYPos, nXPos + 800 + 100, nYPos + 25);
		m_generateButtons[nIdx]->Create(this, nGenerateResID, &generateBtnRect, USER_MESSAGE + 12);

		nYPos += 35;
	}

}

void CReportPage::SaveReports() {
	TCHAR szFilePath[512];
	TCHAR szDirectory[512];
	GetCurrentDirectory(512, szDirectory);

	wsprintf(szFilePath, _T("%s\\Data\\Base\\Report.txt"), szDirectory);

	CTextFileWrite file(szFilePath);
	int count = g_reportArray.GetCount();
	for (int i = 0; i < count; i++) {
		SReport* pReport = g_reportArray.GetAt(i);
		
		file.Write(pReport->mName);
		file.WriteEndl();
		file.Write(pReport->mTotalScore);
		file.WriteEndl();
		file.Write(pReport->mTime);
		file.WriteEndl();
		file.Write(pReport->mGrade);
		file.WriteEndl();
		file.Write(pReport->mSchoolName);
		file.WriteEndl();
		file.Write(pReport->mSchoolLogo);
		file.WriteEndl();

		CString sTopicStr;
		JoinString(pReport->mTopics, _T("\t"), sTopicStr);
		file.Write(sTopicStr);
		file.WriteEndl();

		CString sReportToStr;
		JoinString(pReport->mReportToArray, _T("\t"), sReportToStr);
		file.Write(sReportToStr);
		file.WriteEndl();

		file.Write(pReport->mMessage);
		file.WriteEndl();

		file.Write(pReport->mCSVPath);
		file.WriteEndl();

		SaveQuestions(&file, pReport);
	}
	file.Close();
}

void CReportPage::SaveQuestions(CTextFileWrite* pFile, SReport* pReport) {
	WCHAR szCount[2];
	int nCount = pReport->mQuestionArray.GetCount();

	wsprintf(szCount, _T("%d"), pReport->mQuestionArray.GetCount());
	CString sCount = szCount;

	pFile->Write(sCount);
	pFile->WriteEndl();

	for (int i = 0; i < nCount; i++) {
		SQuestion* pQuestion = pReport->mQuestionArray.GetAt(i);

		CStringArray sItems;

		WCHAR szMainNo[5];
		wsprintf(szMainNo, _T("%d"), pQuestion->mMainNo);
		sItems.Add(szMainNo);

		WCHAR szSubNo[5];
		wsprintf(szSubNo, _T("%d"), pQuestion->mSubNo);
		sItems.Add(szSubNo);

		WCHAR szIsSub[5];
		wsprintf(szIsSub, _T("%d"), pQuestion->mIsSubQuestion ? 1 : 0);
		sItems.Add(szIsSub);

		WCHAR szIsLastSub[5];
		wsprintf(szIsLastSub, _T("%d"), pQuestion->mIsLastSub ? 1 : 0);
		sItems.Add(szIsLastSub);

		WCHAR szScore[5];
		wsprintf(szScore, _T("%d"), (int)pQuestion->mScore);
		sItems.Add(szScore);

		sItems.Add(pQuestion->mType);
		sItems.Add(pQuestion->mTopic);
		sItems.Add(pQuestion->mTag);

		CString sItemStr;
		JoinString(sItems, _T("\t"), sItemStr);

		pFile->Write(sItemStr);
		pFile->WriteEndl();
	}
}

void CReportPage::LoadReports() {
	TCHAR szFilePath[512];
	TCHAR szDirectory[512];
	GetCurrentDirectory(512, szDirectory);

	wsprintf(szFilePath, _T("%s\\Data\\Base\\Report.txt"), szDirectory);

	CTextFileRead file(szFilePath);
	int index = 0;

	CString sName, sTotalScore, sTime, sGrade;
	CString sTopicStr, sReportToStr, sMessage, sCSVPath;
	CString sSchoolName, sSchoolLogo;

	g_reportArray.RemoveAll();
	g_filteredReportArray.RemoveAll();

	while (!file.Eof()) {
		if (index % 11 == 0) {
			file.ReadLine(sName);
		} else if (index % 11 == 1) {
			file.ReadLine(sTotalScore);
		} else if (index % 11 == 2) {
			file.ReadLine(sTime);
		} else if (index % 11 == 3) {
			file.ReadLine(sGrade);
		} else if (index % 11 == 4) {
			file.ReadLine(sSchoolName);
		} else if (index % 11 == 5) {
			file.ReadLine(sSchoolLogo);
		} else if (index % 11 == 6) {
			file.ReadLine(sTopicStr);
		} else if (index % 11 == 7) {
			file.ReadLine(sReportToStr);
		} else if (index % 11 == 8) {
			file.ReadLine(sMessage);
		} else if (index % 11 == 9) {
			file.ReadLine(sCSVPath);
		} else if (index % 11 == 10) {
			SReport* pReport = new SReport;
			pReport->mName = sName;
			pReport->mTotalScore = sTotalScore;
			pReport->mCSVPath = sCSVPath;
			pReport->mGrade = sGrade;
			pReport->mMessage = sMessage;
			pReport->mTime = sTime;
			pReport->mSchoolLogo = sSchoolLogo;
			pReport->mSchoolName = sSchoolName;

			SplitString(sTopicStr, _T("\t"), pReport->mTopics);
			SplitString(sReportToStr, _T("\t"), pReport->mReportToArray);

			LoadQuestions(&file, pReport);

			g_reportArray.Add(pReport);
			g_filteredReportArray.Add(pReport);
		}
		index++;
	}
	file.Close();

	m_nPageCount = g_filteredReportArray.GetCount() / 12 + 1;
	m_nPageIndex = 0;

	MovePage(m_nPageIndex);
	VisibleProc();
}

void CReportPage::LoadQuestions(CTextFileRead* pFile, SReport* pReport)
{
	CString sCount;
	pFile->ReadLine(sCount);

	int nCount = _wtoi(sCount);
	for (int i = 0; i < nCount; i++) {
		CString sItemStr;
		pFile->ReadLine(sItemStr);

		CStringArray sItems;
		SplitString(sItemStr, _T("\t"), sItems);

		SQuestion* pQuestion = new SQuestion(0);
		pQuestion->mMainNo = _wtoi(sItems[0]);
		pQuestion->mSubNo = _wtoi(sItems[1]);
		pQuestion->mIsSubQuestion = _wtoi(sItems[2]);
		pQuestion->mIsLastSub = _wtoi(sItems[3]);
		pQuestion->mScore = _wtoi(sItems[4]);
		pQuestion->mType = sItems[5];
		pQuestion->mTopic = sItems[6];
		pQuestion->mTag = sItems[7];

		pReport->mQuestionArray.Add(pQuestion);
	}
}

void CReportPage::MovePage(int nPageIndex)
{
	int nStartIndex = nPageIndex * 12;
	int nEndIndex = nStartIndex + 19;

	if (nEndIndex >= g_filteredReportArray.GetCount()) nEndIndex = g_filteredReportArray.GetCount() - 1;

	TCHAR szNo[10];
	int nCtrlIdx = 0;
	for (int i = nStartIndex; i <= nEndIndex; i++, nCtrlIdx++) {
		SReport* pReport = g_filteredReportArray.GetAt(i);
		wsprintf(szNo, _T("%d"), i + 1);
		
		m_noLabels[nCtrlIdx]->SetText(szNo);
		m_nameLabels[nCtrlIdx]->SetText(pReport->mName);
		m_dateLabels[nCtrlIdx]->SetText(pReport->mTime);

		m_noLabels[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_nameLabels[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_dateLabels[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_editButtons[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_deleteButtons[nCtrlIdx]->ShowWindow(SW_SHOW);
		m_generateButtons[nCtrlIdx]->ShowWindow(SW_SHOW);
	}

	for (int i = nCtrlIdx; i < 12; i++) {
		m_noLabels[i]->ShowWindow(SW_HIDE);
		m_nameLabels[i]->ShowWindow(SW_HIDE);
		m_dateLabels[i]->ShowWindow(SW_HIDE);
		m_editButtons[i]->ShowWindow(SW_HIDE);
		m_deleteButtons[i]->ShowWindow(SW_HIDE);
		m_generateButtons[i]->ShowWindow(SW_HIDE);
	}
}

void CReportPage::VisibleProc() {
	BOOL bPageControlVisible = (g_filteredReportArray.GetCount() > 12);
	/*
	m_backButton.ShowWindow(bPageControlVisible ? SW_SHOW : SW_HIDE);
	m_nextButton.ShowWindow(bPageControlVisible ? SW_SHOW : SW_HIDE);*/
}

LRESULT CReportPage::OnAddBtn(WPARAM wParam, LPARAM lParam) {
	CReportDialog dlg;
	if (dlg.DoModal() == IDOK) {
		SReport* pReport = dlg.GetReport();
		g_reportArray.Add(pReport);
		g_filteredReportArray.Add(pReport);

		SaveReports();

		m_nPageCount = g_reportArray.GetCount() / 12 + 1;
		m_nPageIndex = m_nPageCount - 1;

		MovePage(m_nPageIndex);
	}
	return 0;
}

LRESULT CReportPage::OnDeleteBtn(WPARAM wParam, LPARAM lParam) {
	int nIdx = -1;
	for (int i = 0; i < 12; i++) {
		if (m_deleteButtons[i] == (CMyBitmapButton*)(wParam)) {
			nIdx = i;
			break;
		}
	}

	if (nIdx >= 0) {
		nIdx += m_nPageIndex * 12;

		SReport* pReport = g_filteredReportArray.GetAt(nIdx);
		g_filteredReportArray.RemoveAt(nIdx);

		int nIndexOfTotal = GetIndexOfTotal(pReport);
		g_reportArray.RemoveAt(nIdx);

		delete pReport, pReport = 0;

		m_nPageCount = g_filteredReportArray.GetCount() / 12 + 1;

		MovePage(m_nPageIndex);
		VisibleProc();

		SaveReports();
	}
	return 0;
}

LRESULT CReportPage::OnEditBtn(WPARAM wParam, LPARAM lParam) {
	int nIdx = -1;
	for (int i = 0; i < 12; i++) {
		if (m_editButtons[i] == (CMyBitmapButton*)(wParam)) {
			nIdx = i;
			break;
		}
	}

	if (nIdx >= 0) {
		SReport* pReport = g_filteredReportArray.GetAt(m_nPageIndex * 12 + nIdx);
		CReportDialog dlg;
		dlg.SetReport(pReport);

		if (dlg.DoModal() == IDOK) {
			m_nameLabels[nIdx]->SetText(pReport->mName);
			m_dateLabels[nIdx]->SetText(pReport->mTime);

			SaveReports();
		}
	}
	return 0;
}

LRESULT CReportPage::OnGenerateBtn(WPARAM wParam, LPARAM lParam) {
	
	int nIdx = -1;
	for (int i = 0; i < 12; i++) {
		if (m_generateButtons[i] == (CMyBitmapButton*)(wParam)) {
			nIdx = i;
			break;
		}
	}

	int nReportIdx = m_nPageIndex * 12 + nIdx;
	SReport* pReport = g_reportArray.GetAt(nReportIdx);

	if (pReport->mCSVPath.IsEmpty()) {
		AfxMessageBox(_T("Please upload csv file."));
		return 0;
	}
	
	CTextFileRead file(pReport->mCSVPath);
	if (!file.IsOpen()) {
		AfxMessageBox(_T("Couldn't read csv file. Please upload csv file."));
		return 0;
	}

	CSendDialog dlg;
	dlg.SetSendEmail(TRUE);		// If only generate document, the param should be FALSE
	dlg.SetReportIndex(nReportIdx);
	dlg.GenerateEmailData(&file, pReport);

	dlg.DoModal();

	return 0;
}

LRESULT CReportPage::OnBackBtn(WPARAM wParam, LPARAM lParam) {
	if (m_nPageIndex > 0) {
		m_nPageIndex--;
		MovePage(m_nPageIndex);
	}
	return 0;
}

LRESULT CReportPage::OnNextBtn(WPARAM wParam, LPARAM lParam) {
	if (m_nPageIndex < m_nPageCount - 1) {
		m_nPageIndex++;
		MovePage(m_nPageIndex);
	}
	return 0;
}

int CReportPage::GetIndexOfTotal(SReport* pReport) {
	int nIdx = -1;
	for (int i = 0; i < g_reportArray.GetCount(); i++) {
		if (pReport == g_reportArray.GetAt(i)) {
			return i;
		}
	}
	return -1;
}

void CReportPage::OnEnChangeSearchEdit() {
	CString sFind;
	m_searchEdit.GetWindowText(sFind);
	sFind = sFind.MakeUpper();
	g_filteredReportArray.RemoveAll();
	for(int i = 0; i<g_reportArray.GetSize(); i++){
		SReport* pReport = g_reportArray.GetAt(i);

		CString mName = pReport->mName;
		mName = mName.MakeUpper();

		if(mName.Find(sFind) >= 0) {
			g_filteredReportArray.Add(pReport);
		}
		m_nPageCount = g_filteredReportArray.GetCount()/12 + 1;
		m_nPageIndex = 0;
		MovePage(m_nPageIndex);
		VisibleProc();
	}

}