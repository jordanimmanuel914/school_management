#pragma once

#include "afxwin.h"

#include "MyLabel.h"
#include "MyBitmapButton.h"

// CTagDialog dialog

class CTagDialog : public CDialog
{
	DECLARE_DYNAMIC(CTagDialog)

public:
	CTagDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTagDialog();

// Dialog Data
	enum { IDD = IDD_TAGS_DIALOG };

	void SetNewTag(BOOL bNewTag);

	void SetTag(const CString& sTag);
	CString GetTag();

	void SetDescription(const CString& sDescription);
	CString GetDescription();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bNewTag;

public:
	CString m_sTitle;
	CString m_sDesc;
	CString m_sTag;
	afx_msg void OnBnClickedSubmitButton();
	afx_msg void OnBnClickedCancelButton();
	afx_msg void OnClose();
};
