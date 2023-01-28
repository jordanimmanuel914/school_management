#pragma once

#include "afxwin.h"
#include "afxcmn.h"

// CSendDialog dialog
class SEmailData;
class SReport;

class CTextFileRead;
class CSubmitThread;

#define WM_PROGRESS		WM_USER + 9000
#define WM_FINISHED		WM_USER + 9001

class CSendDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSendDialog)

public:
	CSendDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSendDialog();

// Dialog Data
	enum { IDD = IDD_SEND_DIALOG };

	void SetSendEmail(BOOL bSendEmail);
	void SetReportIndex(int nReportIdx) { m_nReportIdx = nReportIdx; }

	void ClearAll();
	void GenerateEmailData(CTextFileRead* pFile, SReport* pReport);
	void Execute();

	void OnFinished();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bSendEmail;
	BOOL m_bCancel;
	BOOL m_bDone;

	int m_nReportIdx;

	CArray<SEmailData*, SEmailData*> m_emailDataArray;

	CSubmitThread* m_pThread;
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSubmitButton();
	afx_msg void OnBnClickedCancelButton();
	afx_msg HRESULT OnProgress(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnFinished(WPARAM wParam, LPARAM lParam);

	CProgressCtrl m_progressBar;
	CString m_sStatus;

	CString PEMString;
	CString YourEmailString;
	CString PasswordString;
};
