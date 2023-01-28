// TagDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "TagDialog.h"
#include "afxdialogex.h"

#define USER_MESSAGE		WM_USER + 1000

// CTagDialog dialog

IMPLEMENT_DYNAMIC(CTagDialog, CDialog)

CTagDialog::CTagDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTagDialog::IDD, pParent)
	, m_sDesc(_T(""))
	, m_sTag(_T(""))
{
	m_bNewTag = FALSE;
	m_sTitle = _T("EDIT TAG");
}

CTagDialog::~CTagDialog()
{
}

void CTagDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_sDesc);
	DDX_Text(pDX, IDC_EDIT_TAG_NAME, m_sTag);

		CEdit* pEdit = (CEdit*)(GetDlgItem(IDC_EDIT_TAG_NAME));
	pEdit->SetLimitText(20);



}


BEGIN_MESSAGE_MAP(CTagDialog, CDialog)
	ON_WM_CREATE()
	//ON_WM_PAINT()
ON_BN_CLICKED(IDC_SUBMIT_BUTTON, &CTagDialog::OnBnClickedSubmitButton)
ON_BN_CLICKED(IDC_CANCEL_BUTTON, &CTagDialog::OnBnClickedCancelButton)
ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTagDialog message handlers

void CTagDialog::SetTag(const CString& sTag) {
	m_sTag = sTag;
}

CString CTagDialog::GetTag() {
	return m_sTag;
}

void CTagDialog::SetDescription(const CString& sDesc) {
	m_sDesc = sDesc;
}

CString CTagDialog::GetDescription() {
	return m_sDesc;
}

void CTagDialog::SetNewTag(BOOL bNew) {
	if (bNew) m_sTitle = _T("NEW TAG");
	else m_sTitle = _T("EDIT TAG");

	m_bNewTag = bNew;
}

int CTagDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(m_sTitle);


	return 0;
}


void CTagDialog::OnBnClickedSubmitButton()
{
	CDialog::OnOK();
}


void CTagDialog::OnBnClickedCancelButton()
{
	CDialog::OnCancel();
}


void CTagDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}
