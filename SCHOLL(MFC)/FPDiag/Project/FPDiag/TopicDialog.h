#pragma once
#include "afxwin.h"


// CTopicDialog dialog

class CTopicDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTopicDialog)

public:
	CTopicDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTopicDialog();

// Dialog Data
	enum { IDD = IDD_TOPIC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	DECLARE_MESSAGE_MAP()

private:
	void InitGradeButtons();
	void UpdateGradeButtons();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void SetName(const CString& sName) { m_sName = sName; }
	void SetDesc(const CString& sDesc) { m_sDesc = sDesc; }
	CString GetName() { return m_sName; }
	CString GetDesc() { return m_sDesc; }

	void SetGrades(const CString& sGradeStr) { m_sGradeStr = sGradeStr; }
	CString GetGrades() { return m_sGradeStr; }

	void SetNewTopic(BOOL bNew);

	CString m_sName;
	CString m_sDesc;
	CString m_sGradeStr;
	BOOL m_bNewTopic;
	int m_nTotalGradeCount;

	CButton* m_gradeButtons[20];
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
