
// FPDiagDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "GradePage.h"
#include "HomePage.h"
#include "TopicPage.h"
#include "TagsPage.h"
#include "ReportPage.h"

#include "MyRadioButton.h"

#include "afxwin.h"

// CFPDiagDlg dialog
class CFPDiagDlg : public CDialogEx
{
// Construction
public:
	CFPDiagDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FPDIAG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnHomeBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGradeBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTopicBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTagsBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReportsBtn(WPARAM wParam, LPARAM lParam);

public:
	void CreateWindows(void);
	void CreateButtons();
	void CreatePages();

private:
	void ShowHomePage();
	void ShowGradePage();
	void ShowTopicPage();
	void ShowTagsPage();
	void ShowReportsPage();

private:
	CHomePage m_homePage;
	CGradePage m_gradePage;
	CTopicPage m_topicPage;
	CTagsPage m_tagsPage;
	CReportPage m_reportsPage;

	CMyRadioButton m_homeButton;
	CMyRadioButton m_gradeButton;
	CMyRadioButton m_topicButton;
	CMyRadioButton m_tagsButton;
	CMyRadioButton m_reportsButton;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	BOOL m_bInitialized;
	virtual void OnOK();
};
