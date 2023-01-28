#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "atltime.h"
#include "atlcomtime.h"

#include "LogoWnd.h"

class SReport;

// CReportDialog dialog

class CReportDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CReportDialog)

public:
	CReportDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportDialog();

// Dialog Data
	enum { IDD = IDD_REPORT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void SetReport(SReport* pReport);
	SReport* GetReport() { return m_pReport; }

private:
	void InitControls();
	void CreateLogoWnd();

	void InitGradeCombo();
	void InitTopicControls();
	void UpdateTopicControls();
	void UpdateSchoolInfo();
	void UpdateReportTo();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboGrade();

	BOOL CheckItems(CString& sErrorMessage);

	CComboBox m_comboGrade;
	CString m_sCSVPath;
	CString m_sMessage;
	CString m_sName;

	CString m_sSchool;
	CString m_sTotalScore;

	CString m_sLogoPath;
	CDIBSectionLite m_dib;

	CString m_sGrade;

	int m_nTotalTopicCount;
	CButton* m_topicButtons[20];
	CButton* m_reportToButtons[3];

	COleDateTime m_time;

	SReport *m_pReport;
	CArray<SQuestion*, SQuestion*> m_questionArray;

	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedQuestion();
	afx_msg void OnBnClickedBtnCsv();
	afx_msg void OnPaint();
	afx_msg void OnClickedBtnLogoChange();
};
