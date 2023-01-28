// TopicDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "GradePage.h"
#include "TopicDialog.h"
#include "afxdialogex.h"

extern CArray<SGrade*, SGrade*> g_gradeArray;

extern void SplitString(CString sText, CString sSplit, CStringArray& sItems);
extern void JoinString(const CStringArray& sItems, CString sSplit, CString& sText);

// CTopicDialog dialog

IMPLEMENT_DYNAMIC(CTopicDialog, CDialogEx)

CTopicDialog::CTopicDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTopicDialog::IDD, pParent)
	, m_sName(_T(""))
	, m_sDesc(_T(""))
{
	m_bNewTopic = FALSE;
}

CTopicDialog::~CTopicDialog()
{
}

void CTopicDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Text(pDX, IDC_EDIT_DESC, m_sDesc);
}


BEGIN_MESSAGE_MAP(CTopicDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTopicDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTopicDialog::OnBnClickedCancel)
	//ON_WM_PAINT()
ON_WM_CREATE()
ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTopicDialog message handlers


void CTopicDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	int nGradeCount = 0;
	m_sGradeStr = _T("");

	CStringArray sGrades;
	for (int i = 0; i < m_nTotalGradeCount; i++) {
		if (m_gradeButtons[i]->GetCheck()) {
			sGrades.Add(g_gradeArray[i]->mName);
		}
	}

	if (sGrades.IsEmpty()) {	
		AfxMessageBox(_T("Please choose one grade at least."));
		return;
	} else {
		JoinString(sGrades, _T("\t"), m_sGradeStr);
	}

	CString sName;
	
	UpdateData(TRUE);
	sName = m_sName;
	UpdateData(FALSE);

	if (sName.IsEmpty()) {
		AfxMessageBox(_T("Please input the name of topic."));
		return;
	}
	CDialogEx::OnOK();
}


void CTopicDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

int CTopicDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CTopicDialog::SetNewTopic(BOOL bNew) {
	m_bNewTopic = bNew;
}

void CTopicDialog::InitGradeButtons() {
	m_gradeButtons[0] = (CButton*)GetDlgItem(IDC_GRADE_1);
	m_gradeButtons[1] = (CButton*)GetDlgItem(IDC_GRADE_2);
	m_gradeButtons[2] = (CButton*)GetDlgItem(IDC_GRADE_3);
	m_gradeButtons[3] = (CButton*)GetDlgItem(IDC_GRADE_4);
	m_gradeButtons[4] = (CButton*)GetDlgItem(IDC_GRADE_5);
	m_gradeButtons[5] = (CButton*)GetDlgItem(IDC_GRADE_6);
	m_gradeButtons[6] = (CButton*)GetDlgItem(IDC_GRADE_7);
	m_gradeButtons[7] = (CButton*)GetDlgItem(IDC_GRADE_8);
	m_gradeButtons[8] = (CButton*)GetDlgItem(IDC_GRADE_9);
	m_gradeButtons[9] = (CButton*)GetDlgItem(IDC_GRADE_10);
	m_gradeButtons[10] = (CButton*)GetDlgItem(IDC_GRADE_11);
	m_gradeButtons[11] = (CButton*)GetDlgItem(IDC_GRADE_12);
	m_gradeButtons[12] = (CButton*)GetDlgItem(IDC_GRADE_13);
	m_gradeButtons[13] = (CButton*)GetDlgItem(IDC_GRADE_14);
	m_gradeButtons[14] = (CButton*)GetDlgItem(IDC_GRADE_15);
	m_gradeButtons[15] = (CButton*)GetDlgItem(IDC_GRADE_16);
	m_gradeButtons[16] = (CButton*)GetDlgItem(IDC_GRADE_17);
	m_gradeButtons[17] = (CButton*)GetDlgItem(IDC_GRADE_18);
	m_gradeButtons[18] = (CButton*)GetDlgItem(IDC_GRADE_19);
	m_gradeButtons[19] = (CButton*)GetDlgItem(IDC_GRADE_20);

	CEdit* pEdit = (CEdit*)(GetDlgItem(IDC_EDIT_NAME));
	pEdit->SetLimitText(30);
	m_nTotalGradeCount = min(g_gradeArray.GetSize(), 20);
	for (int i = 0; i < m_nTotalGradeCount; i++) {
		m_gradeButtons[i]->SetWindowText(g_gradeArray[i]->mName);
	}
	for (int i = m_nTotalGradeCount; i < 20; i++) {
		m_gradeButtons[i]->ShowWindow(SW_HIDE);
	}
}

BOOL CTopicDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitGradeButtons();
	if (m_bNewTopic) {
		SetWindowText(_T("NEW TOPIC"));
	} else {
		SetWindowText(_T("EDIT TOPIC"));

		UpdateGradeButtons();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTopicDialog::UpdateGradeButtons() 
{
	CStringArray sGrades;
	SplitString(m_sGradeStr, _T("\t"), sGrades);

	int nCurrentCount = sGrades.GetCount();
	for (int i = 0; i < nCurrentCount; i++) {
		CString sGrade = sGrades.GetAt(i);
		for (int j = 0; j < m_nTotalGradeCount; j++) {
			SGrade* pGrade = g_gradeArray.GetAt(j);
			if (sGrade == pGrade->mName) {
				m_gradeButtons[j]->SetCheck(1);
			}
		}
	}
}



void CTopicDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
}
