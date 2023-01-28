#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyBitmapButton.h"
#include "MyLabel.h"

// CGradePage
struct SGrade {
	CString mName;
	CString mDescription;
};

class CGradePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGradePage)

public:
	CGradePage();
	virtual ~CGradePage();
	
// Dialog Data
	enum { IDD = IDD_GRADE_PAGE };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnAddBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNextBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBackBtn(WPARAM wParam, LPARAM lParam);
	
	afx_msg LRESULT OnEditBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteBtn(WPARAM wParam, LPARAM lParam);
	
//	CListCtrl m_listCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
private:
	void CreateWindows();
	void CreatePageControls();

	void SaveGrade(void);
	void LoadGrade(void);
	void VisibleProc();

	void MovePage(int nIndex);

private:
	CMyBitmapButton m_addButton;
	CMyBitmapButton m_nextButton;
	CMyBitmapButton m_backButton;

	CMyLabel* m_noLabels[20];
	CMyLabel* m_gradeLabels[20];
	CMyBitmapButton* m_editButtons[20];
	CMyBitmapButton* m_deleteButtons[20];

	int m_nPageCount;
	int m_nPageIndex;

public:
	afx_msg void OnPaint();
};


