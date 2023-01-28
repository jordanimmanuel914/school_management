// ReportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"

#include "SendDialog.h"
#include "GradePage.h"
#include "TopicPage.h"
#include "TagsPage.h"
#include "ReportPage.h"
#include "ReportDialog.h"
#include "QuestionDialog.h"
#include "TextFile.h"
#include "afxdialogex.h"

extern CArray<SGrade*, SGrade*> g_gradeArray;
extern CArray<STag*, STag*> g_tagArray;
extern CArray<STopic*, STopic*> g_topicArray;

// CReportDialog dialog
extern void SplitString(CString sText, CString sSplit, CStringArray& sItems);
extern void JoinString(const CStringArray& sItems, CString sSplit, CString& sText);

int GetMainQuestionCount(const CArray<SQuestion*, SQuestion*>& questions){
	int nCount = 0;
	for(int i = 0; i < questions.GetCount(); i++){
		SQuestion* pQuestion = questions.GetAt(i);
		if(pQuestion->mIsSubQuestion){
			if(pQuestion->mSubNo != 1) continue;
		}
		nCount++;
	}
	return nCount;
}


IMPLEMENT_DYNAMIC(CReportDialog, CDialogEx)

CReportDialog::CReportDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReportDialog::IDD, pParent)
	, m_sCSVPath(_T(""))
	, m_sTotalScore(_T(""))
	, m_sMessage(_T("This is the Mathematics report for @Student_Name @Student_Surname.  This report is based on his/her June exam results written at @School_Name"))
	, m_sName(_T(""))
	, m_sSchool(_T("High School Die Wilgers"))
	, m_time(COleDateTime::GetCurrentTime())
{
	m_pReport = 0;
}

CReportDialog::~CReportDialog()
{
}

void CReportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_GRADE, m_comboGrade);
	DDX_Text(pDX, IDC_EDIT_TOTALSCORE, m_sTotalScore);
	DDX_Text(pDX, IDC_EDIT_CSV, m_sCSVPath);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_sMessage);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Text(pDX, IDC_EDIT_SCHOOL, m_sSchool);
	//  DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_time);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_time);
}


BEGIN_MESSAGE_MAP(CReportDialog, CDialogEx)
	//ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_GRADE, &CReportDialog::OnSelchangeComboGrade)
	ON_BN_CLICKED(IDSAVE, &CReportDialog::OnBnClickedSave)
	ON_BN_CLICKED(IDCANCEL, &CReportDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_QUESTION, &CReportDialog::OnBnClickedQuestion)
	ON_BN_CLICKED(IDC_BTN_CSV, &CReportDialog::OnBnClickedBtnCsv)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_LOGO_CHANGE, &CReportDialog::OnClickedBtnLogoChange)
END_MESSAGE_MAP()


// CReportDialog message handlers


BOOL CReportDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CreateLogoWnd();
	InitControls();

	return TRUE;
}

void CReportDialog::CreateLogoWnd() {
	/*
	CRect rect(440, 468, 440 + 60, 468 + 60);
	m_logoWnd.Create(this, &rect);*/
}

void CReportDialog::InitControls()
{
	InitGradeCombo();
	InitTopicControls();
	UpdateReportTo();

	if (m_pReport) {
		SetWindowText(_T("EDIT REPORT"));
		
		m_sSchool = m_pReport->mSchoolName;
		UpdateData(FALSE);

		WCHAR szCount[4];
		_itow(GetMainQuestionCount(m_pReport->mQuestionArray), szCount, 10);

		((CEdit*)(GetDlgItem(IDC_EDIT_Q_NUM)))->SetWindowText(szCount);
		((CButton*)(GetDlgItem(IDC_BTN_QUESTION)))->SetWindowText(_T("Edit Questions"));

		m_sLogoPath = m_pReport->mSchoolLogo;
		m_dib.Load(m_pReport->mSchoolLogo);
		
	} else {
		SetWindowText(_T("ADD REPORT"));
	}
}

void CReportDialog::InitGradeCombo() {
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GRADE);
	int nCount = g_gradeArray.GetCount();
	int nIndex = -1;

	for (int i = 0; i < nCount; i++) {
		pComboBox->AddString(g_gradeArray[i]->mName);
		if (m_sGrade == g_gradeArray[i]->mName) nIndex = i;
	}
	if (nCount > 0) pComboBox->SetCurSel(0);

	if (m_pReport) {
		if (nCount > 0) {
			if (nIndex >= 0) pComboBox->SetCurSel(nIndex);
			else pComboBox->SetCurSel(0);
		}
	} else {
		if (nCount > 0) pComboBox->SetCurSel(0);
	}
}

void CReportDialog::InitTopicControls() {
	m_topicButtons[0] = (CButton*)GetDlgItem(IDC_TOPIC_1);
	m_topicButtons[1] = (CButton*)GetDlgItem(IDC_TOPIC_2);
	m_topicButtons[2] = (CButton*)GetDlgItem(IDC_TOPIC_3);
	m_topicButtons[3] = (CButton*)GetDlgItem(IDC_TOPIC_4);
	m_topicButtons[4] = (CButton*)GetDlgItem(IDC_TOPIC_5);
	m_topicButtons[5] = (CButton*)GetDlgItem(IDC_TOPIC_6);
	m_topicButtons[6] = (CButton*)GetDlgItem(IDC_TOPIC_7);
	m_topicButtons[7] = (CButton*)GetDlgItem(IDC_TOPIC_8);
	m_topicButtons[8] = (CButton*)GetDlgItem(IDC_TOPIC_9);
	m_topicButtons[9] = (CButton*)GetDlgItem(IDC_TOPIC_10);
	m_topicButtons[10] = (CButton*)GetDlgItem(IDC_TOPIC_11);
	m_topicButtons[11] = (CButton*)GetDlgItem(IDC_TOPIC_12);
	m_topicButtons[12] = (CButton*)GetDlgItem(IDC_TOPIC_13);
	m_topicButtons[13] = (CButton*)GetDlgItem(IDC_TOPIC_14);
	m_topicButtons[14] = (CButton*)GetDlgItem(IDC_TOPIC_15);
	m_topicButtons[15] = (CButton*)GetDlgItem(IDC_TOPIC_16);
	m_topicButtons[16] = (CButton*)GetDlgItem(IDC_TOPIC_17);
	m_topicButtons[17] = (CButton*)GetDlgItem(IDC_TOPIC_18);
	m_topicButtons[18] = (CButton*)GetDlgItem(IDC_TOPIC_19);
	m_topicButtons[19] = (CButton*)GetDlgItem(IDC_TOPIC_20);

	CEdit* pEdit = (CEdit*)(GetDlgItem(IDC_EDIT_NAME));
	pEdit->SetLimitText(20);


	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GRADE);
	int nSel = pComboBox->GetCurSel();

	if (nSel >= 0) pComboBox->GetLBText(nSel, m_sGrade);

	int nCount = g_topicArray.GetCount();
	CStringArray sTopics;
	for (int i = 0; i < nCount; i++) {
		STopic* pTopic = g_topicArray.GetAt(i);
		int nIndex = -1;
		for (int j = 0; j < pTopic->mGrades.GetCount(); j++) {
			if (pTopic->mGrades[j] == m_sGrade) sTopics.Add(pTopic->mName);
		}
	}

	m_nTotalTopicCount = min(sTopics.GetCount(), 20);
	for (int i = 0; i < m_nTotalTopicCount; i++) {
		m_topicButtons[i]->SetWindowText(sTopics.GetAt(i));
		m_topicButtons[i]->ShowWindow(SW_SHOW);
	}
	for (int i = m_nTotalTopicCount; i < 20; i++) {
		m_topicButtons[i]->ShowWindow(SW_HIDE);
	}
	UpdateTopicControls();
}

void CReportDialog::UpdateTopicControls() {
	if (!m_pReport) return;
	for(int i = 0; i<m_nTotalTopicCount; i++){
		m_topicButtons[i]->SetCheck(0);
	}

	int nCurrentCount = m_pReport->mTopics.GetCount();
	for (int i = 0; i < nCurrentCount; i++) {
		CString sTopic1 = m_pReport->mTopics.GetAt(i);
		for (int j = 0; j < m_nTotalTopicCount; j++) {
			CString sTopic2;
			m_topicButtons[j]->GetWindowText(sTopic2);

			//SGrade* pGrade = g_gradeArray.GetAt(j);
			if (sTopic1 == sTopic2) m_topicButtons[j]->SetCheck(1);
		}
	}
}

void CReportDialog::UpdateReportTo() {
	m_reportToButtons[0] = (CButton*)GetDlgItem(IDC_REPORT_TO_1);
	m_reportToButtons[0]->SetCheck(0);

	m_reportToButtons[1] = (CButton*)GetDlgItem(IDC_REPORT_TO_2);
	m_reportToButtons[1]->SetCheck(0);

	m_reportToButtons[2] = (CButton*)GetDlgItem(IDC_REPORT_TO_3);
	m_reportToButtons[2]->SetCheck(0);

	if (m_pReport) {
		int nCurrentCount = m_pReport->mReportToArray.GetCount();
		for (int i = 0; i < nCurrentCount; i++) {
			CString sReportTo1 = m_pReport->mReportToArray.GetAt(i);		
			for (int j = 0; j < 3; j++) {
				CString sReportTo2;
				m_reportToButtons[j]->GetWindowText(sReportTo2);

				if (sReportTo1 == sReportTo2) m_reportToButtons[j]->SetCheck(1);
			}
		}
	}
}

void CReportDialog::OnSelchangeComboGrade()
{
	// TODO: Add your control notification handler code here
	InitTopicControls();
}

BOOL CReportDialog::CheckItems(CString& sErrorMessage) 
{
	CStringArray sTopics;
	CStringArray sReportToArray;

	if (m_sName.IsEmpty()) {
		sErrorMessage = _T("Please input the name of report.");
		return FALSE;
	}
	if (m_sMessage.IsEmpty()) {
		sErrorMessage = _T("Please input the address message.");
		return FALSE;
	}
	int nGradeSel = ((CComboBox*)GetDlgItem(IDC_COMBO_GRADE))->GetCurSel();
	if (nGradeSel < 0) {
		sErrorMessage = _T("Please choose the grade.");
		return FALSE;
	}

	int nTopicCount = 0;
	for (int i = 0; i < m_nTotalTopicCount; i++) {
		if (m_topicButtons[i]->GetCheck()) {
			CString sTopic;
			m_topicButtons[i]->GetWindowText(sTopic);
			sTopics.Add(sTopic);
		}
	}
	if (sTopics.IsEmpty()) {
		sErrorMessage = _T("Please choose one topic at least.");
		return FALSE;
	}
	
	for (int i = 0; i < 3; i++) {
		if (m_reportToButtons[i]->GetCheck()) {
			CString sReportTo;
			m_reportToButtons[i]->GetWindowText(sReportTo);
			sReportToArray.Add(sReportTo);
		}
	}
	if (sReportToArray.IsEmpty()) {
		sErrorMessage = _T("Please choose one at report to items at least.");
		return FALSE;
	}

	m_pReport->mName = m_sName;
	m_pReport->mGrade = m_sGrade;
	m_pReport->mCSVPath = m_sCSVPath;
	m_pReport->mMessage = m_sMessage;
	m_pReport->mTotalScore = m_sTotalScore;
	m_pReport->mSchoolName = m_sSchool;
	m_pReport->mSchoolLogo = m_sLogoPath;
//	m_pReport->mSchoolLogo = m_logoWnd.GetFilePath();

	WCHAR szTime[20];
	wsprintf(szTime, _T("%04d-%02d-%02d"), m_time.GetYear(), m_time.GetMonth(), m_time.GetDay());
	m_pReport->mTime = szTime;

	m_pReport->mTopics.RemoveAll();
	for (int i = 0; i < sTopics.GetCount(); i++) {
		m_pReport->mTopics.Add(sTopics.GetAt(i));
	}
	m_pReport->mReportToArray.RemoveAll();
	for (int i = 0; i < sReportToArray.GetCount(); i++) {
		m_pReport->mReportToArray.Add(sReportToArray.GetAt(i));
	}

	for (int i = 0; i < m_pReport->mQuestionArray.GetCount(); i++) {
		SQuestion* pQuestion = m_pReport->mQuestionArray.GetAt(i);
		delete pQuestion, pQuestion = 0;
	}
	m_pReport->mQuestionArray.RemoveAll();

	for (int i = 0; i < m_questionArray.GetCount(); i++) {
		SQuestion* newQuestion = new SQuestion(i + 1);
		newQuestion->copy(m_questionArray.GetAt(i));
		m_pReport->mQuestionArray.Add(newQuestion);
	}
	return TRUE;
}

void CReportDialog::OnBnClickedSave()
{
	UpdateData(TRUE);
	
	CString sErrorMessage;

	BOOL bUpdate = (m_pReport != NULL);
	if (!bUpdate) m_pReport = new SReport;

	if (CheckItems(sErrorMessage)) {
		CDialogEx::OnOK();
	} else {
		if (!bUpdate) delete m_pReport, m_pReport = NULL;		
		AfxMessageBox(sErrorMessage);

	}
}


void CReportDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CReportDialog::OnBnClickedQuestion()
{
	CString sQuestionCount;
	CEdit* pEdit = (CEdit*)(GetDlgItem(IDC_EDIT_Q_NUM));
	pEdit->GetWindowText(sQuestionCount);

	int nQuestionCount = _wtoi(sQuestionCount);
	if (nQuestionCount == 0) return;

	int nExistingCount = GetMainQuestionCount(m_questionArray);
	if (nQuestionCount > nExistingCount) {
		int nMainNo = 1;
		if (nExistingCount > 0) {
			nMainNo = m_questionArray.GetAt(m_questionArray.GetCount() - 1)->mMainNo + 1;
		}
		for (int i = nExistingCount; i < nQuestionCount; i++, nMainNo++) {
			m_questionArray.Add(new SQuestion(nMainNo));
		}
	} else {
		int nCount = m_questionArray.GetCount();
		for (int i = nCount - 1; i >= 0; i--) {
			SQuestion* pQuestion = m_questionArray.GetAt(i);
			if(pQuestion->mMainNo > nQuestionCount){
				delete pQuestion, pQuestion = NULL;
				m_questionArray.RemoveAt(i);
			}
		}
	}

	CStringArray sTopicArray;
	for (int i = 0; i < m_nTotalTopicCount; i++) {
		if (m_topicButtons[i]->GetCheck()) {
			CString sTopic;
			m_topicButtons[i]->GetWindowText(sTopic);
			sTopicArray.Add(sTopic);
		}
	}

	if (sTopicArray.GetCount() == 0) {
		AfxMessageBox(_T("Please choose one topic at least."));     	
		
		return;
	}

	CQuestionDialog dlg(this);
	dlg.SetTopics(sTopicArray);
	dlg.SetQuestions(m_questionArray);
	if (dlg.DoModal() == IDOK) {
		for (int i = 0; i < m_questionArray.GetCount(); i++) {
			SQuestion* question = m_questionArray.GetAt(i);
			delete question, question = 0;
		}
		m_questionArray.RemoveAll();

		int nCount = dlg.GetQuestionCount();
		for (int i = 0; i < nCount; i++) {
			SQuestion* newQuestion = new SQuestion(i + 1);
			SQuestion* question = dlg.GetQuestion(i);
			newQuestion->copy(question);

			m_questionArray.Add(newQuestion);
		}

		WCHAR szCount[4];
		_itow(GetMainQuestionCount(m_questionArray), szCount, 10);
		((CEdit*)(GetDlgItem(IDC_EDIT_Q_NUM)))->SetWindowText(szCount);

		dlg.ClearQuestion();
	}
}

void CReportDialog::SetReport(SReport* pReport) {
	m_pReport = pReport;

	if (pReport) {
		m_sName = pReport->mName;
		m_sCSVPath = pReport->mCSVPath;
		m_sTotalScore = pReport->mTotalScore;
		m_sGrade = pReport->mGrade;
		m_sMessage = pReport->mMessage;

		m_questionArray.RemoveAll();
		for (int i = 0; i < pReport->mQuestionArray.GetCount(); i++) {
			SQuestion* newQuestion = new SQuestion(i + 1);
			SQuestion* question = pReport->mQuestionArray.GetAt(i);

			newQuestion->copy(question);
			m_questionArray.Add(newQuestion);
		}
	}
}

void CReportDialog::OnBnClickedBtnCsv()
{
	CFileDialog dlg(TRUE, _T("csv"), NULL, OFN_ENABLESIZING | OFN_EXPLORER | OFN_READONLY | OFN_PATHMUSTEXIST | OFN_NODEREFERENCELINKS | OFN_LONGNAMES | OFN_PATHMUSTEXIST,
		_T("CSV Files (*.csv)|*.csv|"), this);
	dlg.m_ofn.lpstrTitle = _T("Open File");

	if (dlg.DoModal() == IDOK) {
		UpdateData(TRUE);
		m_sCSVPath = dlg.GetFolderPath() + _T("\\") + dlg.GetFileName();
		UpdateData(FALSE);
	}
}


void CReportDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

		// TODO: Add your message handler code here
		// Do not call CDialogEx::OnPaint() for painting messages
	CRect rect(440, 468, 440+60, 468+60);
	if(m_dib.GetSafeHandle() == 0){
		CBrush brush(RGB(100,100,100));
		dc.FillRect(&rect, &brush);
		brush.DeleteObject();
	} else {
		m_dib.Stretch(&dc, CPoint(440, 468), CSize(60, 60));
	}



}


void CReportDialog::OnClickedBtnLogoChange()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_ENABLESIZING | OFN_EXPLORER | OFN_READONLY |
		OFN_PATHMUSTEXIST | OFN_NODEREFERENCELINKS | OFN_LONGNAMES | OFN_PATHMUSTEXIST ,
		_T("Bitmap Files (*.bmp)|*.bmp|"), this);
	dlg.m_ofn.lpstrTitle = _T("Choose Logo File");
	if(dlg.DoModal() == IDOK){
		m_sLogoPath = dlg.GetFolderPath() + _T("\\") +dlg.GetFileName();
		m_dib.Load(m_sLogoPath);
		Invalidate();
	}
}
