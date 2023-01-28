#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyBitmapButton.h"
#include "MyLabel.h"

// CTagsPage
struct STag {
	CString mName;
	CString mDescription;
};

class CTagsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTagsPage)

public:
	CTagsPage();
	virtual ~CTagsPage();
	
// Dialog Data
	enum { IDD = IDD_TAGS_PAGE };
	
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

	void SaveTags(void);
	void LoadTags(void);
	void VisibleProc();

	void MovePage(int nIndex);

private:
	CMyBitmapButton m_addButton;
	CMyBitmapButton m_nextButton;
	CMyBitmapButton m_backButton;

	CMyLabel* m_noLabels[20];
	CMyLabel* m_nameLabels[20];
	CMyBitmapButton* m_editButtons[20];
	CMyBitmapButton* m_deleteButtons[20];

	int m_nPageCount;
	int m_nPageIndex;

public:
	afx_msg void OnPaint();
};


