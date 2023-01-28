// GradeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "GradeDialog.h"
#include "afxdialogex.h"

#define USER_MESSAGE		WM_USER + 1000

// CGradeDialog dialog

IMPLEMENT_DYNAMIC(CGradeDialog, CDialog)

CGradeDialog::CGradeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGradeDialog::IDD, pParent)
	, m_sDesc(_T(""))
	, m_sGrade(_T(""))
{
	m_bNewGrade = FALSE;
	m_sTitle = _T("EDIT GRADE");
}

CGradeDialog::~CGradeDialog()
{
}

void CGradeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_sDesc);
	DDX_Text(pDX, IDC_EDIT_GRADE_LEVEL, m_sGrade);
	CEdit* pEdit = (CEdit*)(GetDlgItem(IDC_EDIT_GRADE_LEVEL));
	pEdit->SetLimitText(20);
}


BEGIN_MESSAGE_MAP(CGradeDialog, CDialog)
	ON_WM_CREATE()
	//ON_WM_PAINT()
ON_BN_CLICKED(IDC_SUBMIT_BUTTON, &CGradeDialog::OnBnClickedSubmitButton)
ON_BN_CLICKED(IDC_CANCEL_BUTTON, &CGradeDialog::OnBnClickedCancelButton)
END_MESSAGE_MAP()


// CGradeDialog message handlers

void CGradeDialog::SetGrade(const CString& sGrade) {
	m_sGrade = sGrade;
}

CString CGradeDialog::GetGrade() {
	return m_sGrade;
}

void CGradeDialog::SetDescription(const CString& sDesc) {
	m_sDesc = sDesc;
}

CString CGradeDialog::GetDescription() {
	return m_sDesc;
}

void CGradeDialog::SetNewGrade(BOOL bNew) {
	if (bNew) m_sTitle = _T("NEW GRADE");
	else m_sTitle = _T("EDIT GRADE");

	m_bNewGrade = bNew;
}

int CGradeDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(m_sTitle);
	return 0;
}


void CGradeDialog::OnBnClickedSubmitButton()
{
	CDialog::OnOK();
}


void CGradeDialog::OnBnClickedCancelButton()
{
	CDialog::OnCancel();
}
