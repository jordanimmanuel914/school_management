#pragma once

#include "afxwin.h"

#include "MyLabel.h"
#include "MyBitmapButton.h"

// CGradeDialog dialog

class CGradeDialog : public CDialog
{
	DECLARE_DYNAMIC(CGradeDialog)

public:
	CGradeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGradeDialog();

// Dialog Data
	enum { IDD = IDD_GRADE_DIALOG };

	void SetNewGrade(BOOL bNewGrade);

	void SetGrade(const CString& sGrade);
	CString GetGrade();

	void SetDescription(const CString& sDescription);
	CString GetDescription();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bNewGrade;

public:
	CString m_sTitle;
	CString m_sDesc;
	CString m_sGrade;
	afx_msg void OnBnClickedSubmitButton();
	afx_msg void OnBnClickedCancelButton();
};
