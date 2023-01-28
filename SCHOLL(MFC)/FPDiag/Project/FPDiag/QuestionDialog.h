#pragma once

#include "MyCheckCombiBox.h"
// CQuestionDialog dialog

class CQuestionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CQuestionDialog)

public:
	CQuestionDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQuestionDialog();

// Dialog Data
	enum { IDD = IDD_QUESTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	void ClearQuestion();

	int GetQuestionCount();
	SQuestion* GetQuestion(int nIndex);
	void SetQuestions(const CArray<SQuestion*, SQuestion*>& questions);

	void SetTopics(const CStringArray& sTopicArray);
	void InitControls();

	void MovePage(int nPageIndex);

	void GenerateSubQuestion(int nIndex);
	void OnScoreChanged(int nIndex);
	void OnTypeChanged(int nIndex);
	void OnTopicChanged(int nIndex);
	void OnTagChanged(int nIndex);
private:
	CArray<SQuestion*, SQuestion*> m_questionArray;
	CStringArray m_sTopicArray;

	CStatic* m_pNumberLabels[10];
	CComboBox* m_pTypeCombos[10];
	CComboBox* m_pTopicCombos[10];
	CMyCheckCombiBox* m_pTagCombos[10];
	CEdit* m_pScoreEdits[10];
	CButton* m_pGenerateSubBtn[10];

	BOOL m_bGenerate;
	
	int m_nPageCount;
	int m_nPageIndex;

public:
	afx_msg void OnBnClickedBtnPrev();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnSub1();
	afx_msg void OnBnClickedBtnSub2();
	afx_msg void OnBnClickedBtnSub3();
	afx_msg void OnBnClickedBtnSub4();
	afx_msg void OnBnClickedBtnSub5();
	afx_msg void OnBnClickedBtnSub6();
	afx_msg void OnBnClickedBtnSub7();
	afx_msg void OnBnClickedBtnSub8();
	afx_msg void OnBnClickedBtnSub9();
	afx_msg void OnBnClickedBtnSub10();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSelchangeComboTag1();
	afx_msg void OnCbnSelchangeComboTag2();
	afx_msg void OnCbnSelchangeComboTag3();
	afx_msg void OnCbnSelchangeComboTag4();
	afx_msg void OnCbnSelchangeComboTag5();
	afx_msg void OnCbnSelchangeComboTag6();
	afx_msg void OnCbnSelchangeComboTag7();
	afx_msg void OnCbnSelchangeComboTag8();
	afx_msg void OnCbnSelchangeComboTag9();
	afx_msg void OnCbnSelchangeComboTag10();
	afx_msg void OnCbnSelchangeComboTopic10();
	afx_msg void OnCbnSelchangeComboTopic9();
	afx_msg void OnCbnSelchangeComboTopic8();
	afx_msg void OnCbnSelchangeComboTopic7();
	afx_msg void OnCbnSelchangeComboTopic6();
	afx_msg void OnCbnSelchangeComboTopic5();
	afx_msg void OnCbnSelchangeComboTopic4();
	afx_msg void OnCbnSelchangeComboTopic3();
	afx_msg void OnCbnSelchangeComboTopic2();
	afx_msg void OnCbnSelchangeComboTopic1();
	afx_msg void OnEnChangeEditScore1();
	afx_msg void OnEnChangeEditScore2();
	afx_msg void OnEnChangeEditScore3();
	afx_msg void OnEnChangeEditScore4();
	afx_msg void OnEnChangeEditScore5();
	afx_msg void OnEnChangeEditScore6();
	afx_msg void OnEnChangeEditScore7();
	afx_msg void OnEnChangeEditScore8();
	afx_msg void OnEnChangeEditScore9();
	afx_msg void OnEnChangeEditScore10();
	afx_msg void OnCbnSelchangeComboType1();
	afx_msg void OnCbnSelchangeComboType2();
	afx_msg void OnCbnSelchangeComboType4();
	afx_msg void OnCbnSelchangeComboType3();
	afx_msg void OnCbnSelchangeComboType5();
	afx_msg void OnCbnSelchangeComboType6();
	afx_msg void OnCbnSelchangeComboType7();
	afx_msg void OnCbnSelchangeComboType8();
	afx_msg void OnCbnSelchangeComboType9();
	afx_msg void OnCbnSelchangeComboType10();

	CMyCheckCombiBox m_cmbTag1;
	CMyCheckCombiBox m_cmbTag10;
	CMyCheckCombiBox m_cmbTag2;
	CMyCheckCombiBox m_cmbTag3;
	CMyCheckCombiBox m_cmbTag4;
	CMyCheckCombiBox m_cmbTag5;
	CMyCheckCombiBox m_cmbTag6;
	CMyCheckCombiBox m_cmbTag7;
	CMyCheckCombiBox m_cmbTag8;
	CMyCheckCombiBox m_cmbTag9;


	
};
