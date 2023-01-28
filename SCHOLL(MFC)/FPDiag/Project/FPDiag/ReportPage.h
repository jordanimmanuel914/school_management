#pragma once

#include "MyBitmapButton.h"
#include "MyLabel.h"
#include "MSWord.h"

#include "afxwin.h"

class CTextFileWrite;
class CTextFileRead;
class SReport;
class CMSWord;

class SEmailData {
public:
	SEmailData() {
		bNoLanguageScore = FALSE;
		bCalculatorScore = FALSE;
		bLevel34Score = FALSE;
		bCoreMathScore = FALSE;
		bMathLiteracyScore = FALSE;
	}

	BOOL GenerateEmailData(SReport* pReport);
	BOOL SaveDocument();
	BOOL DeleteDocument();
	BOOL SendEmail(const CString& myaddr,const CString& APPPASS, const CString& PEM);

	CString sGreeting;
	CString sAddressMessage;
	CString sSchoolName;
	CString sSchoolLogoPath;
	CString sReportDate;
	CString sStudentSurname;
	CString sStudentName;
	
	int nGeneralScore;
	
	int nNoLanguageScore;
	BOOL bNoLanguageScore;

	int nCalculatorScore;
	BOOL bCalculatorScore;

	int nLevel34Score;
	BOOL bLevel34Score;

	int nCoreMathScore;
	BOOL bCoreMathScore;

	int nMathLiteracyScore;
	BOOL bMathLiteracyScore;

	CStringArray sQuestionNameArray;
	CArray<int, int> nScoreArray;

	CString sEmailAddress1;
	CString sEmailAddress2;

	CString sReportFileName;

	CMSWord* m_document;
};

// CReportPage dialog
class SQuestion {
public:
	SQuestion(int nMainNo) {
		mSubNo = 0;
		mMainNo = nMainNo;

		mScore = 0;
		mIsSubQuestion = FALSE;
		mIsLastSub = TRUE;

		mType = _T("");
		mTopic = _T("");
		mTag = _T("");
	}

	void copy(SQuestion* other) {
		this->mScore = other->mScore;
		this->mType = other->mType;
		this->mTopic = other->mTopic;
		this->mTag = other->mTag;
		this->mMainNo = other->mMainNo;
		this->mSubNo = other->mSubNo;
		this->mIsLastSub = other->mIsLastSub;
		this->mIsSubQuestion = other->mIsSubQuestion;
	}
	BOOL mIsSubQuestion;
	BOOL mIsLastSub;

	int mSubNo;
	int mMainNo;

	CString mType;
	CString mTopic;
	CString mTag;
	int mScore;
};

class SReport {
public:
	CString mName;
	CString mTotalScore;
	CString mTime;
	CString mGrade;
	CStringArray mTopics;
	CStringArray mReportToArray;
	CArray<SQuestion*, SQuestion*> mQuestionArray;
	CString mMessage;
	CString mCSVPath;
	CString mSchoolName;
	CString mSchoolLogo;
};

class CReportPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CReportPage)

public:
	CReportPage();
	virtual ~CReportPage();

// Dialog Data
	enum { IDD = IDD_REPORTS_PAGE };

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
	afx_msg LRESULT OnGenerateBtn(WPARAM wParam, LPARAM lParam);

	int GetIndexOfTotal(SReport* pReport);

private:
	void CreateWindows();
	void CreatePageControls();

	void LoadReports();
	void SaveReports();
	void SaveQuestions(CTextFileWrite* pFile, SReport* pReport);
	void LoadQuestions(CTextFileRead* pFile, SReport* pReport);

	void VisibleProc();
	void MovePage(int nPageIndex);

private:
	CMyBitmapButton m_addButton;
	CMyBitmapButton m_nextButton;
	CMyBitmapButton m_backButton;

	CMyLabel* m_noLabels[20];
	CMyLabel* m_nameLabels[20];
	CMyLabel* m_dateLabels[20];
	CMyBitmapButton* m_editButtons[20];
	CMyBitmapButton* m_deleteButtons[20];
	CMyBitmapButton* m_generateButtons[20];

	CMyLabel m_searchLabel;
	CEdit m_searchEdit;

	int m_nPageCount;
	int m_nPageIndex;
};
