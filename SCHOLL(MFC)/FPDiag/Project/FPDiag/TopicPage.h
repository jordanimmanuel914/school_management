#pragma once

#include "MyBitmapButton.h"
#include "MyLabel.h"
#include "afxwin.h"

// CTopicPage dialog
struct STopic {
	CString mName;
	CString mDescription;
	CStringArray mGrades;
};

class CTopicPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTopicPage)

public:
	CTopicPage();
	virtual ~CTopicPage();

// Dialog Data
	enum { IDD = IDD_TOPIC_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnChangeSearchEdit();

	afx_msg LRESULT OnAddBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBackBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNextBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteBtn(WPARAM wParam, LPARAM lParam);

	int GetIndexOfTotal(STopic* pTopic);

private:
	void CreateWindows();
	void CreatePageControls();

	void LoadTopics();
	void SaveTopics();

	void VisibleProc();
	void MovePage(int nPageIndex);

private:
	CMyBitmapButton m_addButton;
	CMyBitmapButton m_nextButton;
	CMyBitmapButton m_backButton;

	CMyLabel* m_noLabels[20];
	CMyLabel* m_topicLabels[20];
	CMyBitmapButton* m_editButtons[20];
	CMyBitmapButton* m_deleteButtons[20];

	CMyLabel m_searchLabel;
	CEdit m_searchEdit;

	int m_nPageCount;
	int m_nPageIndex;
};
