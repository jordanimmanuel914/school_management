// QuestionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "ReportPage.h"
#include "TagsPage.h"
#include "QuestionDialog.h"
#include "afxdialogex.h"


extern CArray<STag*, STag*> g_tagArray;

// CQuestionDialog dialog
extern void JoinString(const CStringArray& sItems, CString sSplit, CString& sText);
extern void SplitString(CString sText, CString sSplit, CStringArray& sItems);

IMPLEMENT_DYNAMIC(CQuestionDialog, CDialogEx)

CQuestionDialog::CQuestionDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQuestionDialog::IDD, pParent)
{
}

CQuestionDialog::~CQuestionDialog()
{
	
}

void CQuestionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TAG_1, m_cmbTag1);
	DDX_Control(pDX, IDC_COMBO_TAG_10, m_cmbTag10);
	DDX_Control(pDX, IDC_COMBO_TAG_2, m_cmbTag2);
	DDX_Control(pDX, IDC_COMBO_TAG_3, m_cmbTag3);
	DDX_Control(pDX, IDC_COMBO_TAG_4, m_cmbTag4);
	DDX_Control(pDX, IDC_COMBO_TAG_5, m_cmbTag5);
	DDX_Control(pDX, IDC_COMBO_TAG_6, m_cmbTag6);
	DDX_Control(pDX, IDC_COMBO_TAG_7, m_cmbTag7);
	DDX_Control(pDX, IDC_COMBO_TAG_8, m_cmbTag8);
	DDX_Control(pDX, IDC_COMBO_TAG_9, m_cmbTag9);


}


BEGIN_MESSAGE_MAP(CQuestionDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_PREV, &CQuestionDialog::OnBnClickedBtnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CQuestionDialog::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_SUB_1, &CQuestionDialog::OnBnClickedBtnSub1)
	ON_BN_CLICKED(IDC_BTN_SUB_2, &CQuestionDialog::OnBnClickedBtnSub2)
	ON_BN_CLICKED(IDC_BTN_SUB_3, &CQuestionDialog::OnBnClickedBtnSub3)
	ON_BN_CLICKED(IDC_BTN_SUB_4, &CQuestionDialog::OnBnClickedBtnSub4)
	ON_BN_CLICKED(IDC_BTN_SUB_5, &CQuestionDialog::OnBnClickedBtnSub5)
	ON_BN_CLICKED(IDC_BTN_SUB_6, &CQuestionDialog::OnBnClickedBtnSub6)
	ON_BN_CLICKED(IDC_BTN_SUB_7, &CQuestionDialog::OnBnClickedBtnSub7)
	ON_BN_CLICKED(IDC_BTN_SUB_8, &CQuestionDialog::OnBnClickedBtnSub8)
	ON_BN_CLICKED(IDC_BTN_SUB_9, &CQuestionDialog::OnBnClickedBtnSub9)
	ON_BN_CLICKED(IDC_BTN_SUB_10, &CQuestionDialog::OnBnClickedBtnSub10)
	ON_BN_CLICKED(IDOK, &CQuestionDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CQuestionDialog::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_TAG_1, &CQuestionDialog::OnSelchangeComboTag1)
	ON_CBN_SELCHANGE(IDC_COMBO_TAG_2, &CQuestionDialog::OnCbnSelchangeComboTag2)
	ON_CBN_SELCHANGE(IDC_COMBO_TAG_3, &CQuestionDialog::OnCbnSelchangeComboTag3)
	ON_CBN_SELCHANGE(IDC_COMBO_TAG_4, &CQuestionDialog::OnCbnSelchangeComboTag4)
	ON_CBN_SELCHANGE(IDC_COMBO_TAG_5, &CQuestionDialog::OnCbnSelchangeComboTag5)
	ON_CBN_SELCHANGE(IDC_COMBO_TAG_6, &CQuestionDialog::OnCbnSelchangeComboTag6)
	ON_CBN_SELCHANGE(IDC_COMBO_TAG_7, &CQuestionDialog::OnCbnSelchangeComboTag7)
	ON_CBN_SELCHANGE(IDC_COMBO_TAG_8, &CQuestionDialog::OnCbnSelchangeComboTag8)
	ON_CBN_SELCHANGE(IDC_COMBO_TAG_9, &CQuestionDialog::OnCbnSelchangeComboTag9)
	ON_CBN_SELCHANGE(IDC_COMBO_TAG_10, &CQuestionDialog::OnCbnSelchangeComboTag10)
	ON_CBN_SELCHANGE(IDC_COMBO_TOPIC_10, &CQuestionDialog::OnCbnSelchangeComboTopic10)
	ON_CBN_SELCHANGE(IDC_COMBO_TOPIC_9, &CQuestionDialog::OnCbnSelchangeComboTopic9)
	ON_CBN_SELCHANGE(IDC_COMBO_TOPIC_8, &CQuestionDialog::OnCbnSelchangeComboTopic8)
	ON_CBN_SELCHANGE(IDC_COMBO_TOPIC_7, &CQuestionDialog::OnCbnSelchangeComboTopic7)
	ON_CBN_SELCHANGE(IDC_COMBO_TOPIC_6, &CQuestionDialog::OnCbnSelchangeComboTopic6)
	ON_CBN_SELCHANGE(IDC_COMBO_TOPIC_5, &CQuestionDialog::OnCbnSelchangeComboTopic5)
	ON_CBN_SELCHANGE(IDC_COMBO_TOPIC_4, &CQuestionDialog::OnCbnSelchangeComboTopic4)
	ON_CBN_SELCHANGE(IDC_COMBO_TOPIC_3, &CQuestionDialog::OnCbnSelchangeComboTopic3)
	ON_CBN_SELCHANGE(IDC_COMBO_TOPIC_2, &CQuestionDialog::OnCbnSelchangeComboTopic2)
	ON_CBN_SELCHANGE(IDC_COMBO_TOPIC_1, &CQuestionDialog::OnCbnSelchangeComboTopic1)
	ON_EN_CHANGE(IDC_EDIT_SCORE_1, &CQuestionDialog::OnEnChangeEditScore1)
	ON_EN_CHANGE(IDC_EDIT_SCORE_2, &CQuestionDialog::OnEnChangeEditScore2)
	ON_EN_CHANGE(IDC_EDIT_SCORE_3, &CQuestionDialog::OnEnChangeEditScore3)
	ON_EN_CHANGE(IDC_EDIT_SCORE_4, &CQuestionDialog::OnEnChangeEditScore4)
	ON_EN_CHANGE(IDC_EDIT_SCORE_5, &CQuestionDialog::OnEnChangeEditScore5)
	ON_EN_CHANGE(IDC_EDIT_SCORE_6, &CQuestionDialog::OnEnChangeEditScore6)
	ON_EN_CHANGE(IDC_EDIT_SCORE_7, &CQuestionDialog::OnEnChangeEditScore7)
	ON_EN_CHANGE(IDC_EDIT_SCORE_8, &CQuestionDialog::OnEnChangeEditScore8)
	ON_EN_CHANGE(IDC_EDIT_SCORE_9, &CQuestionDialog::OnEnChangeEditScore9)
	ON_EN_CHANGE(IDC_EDIT_SCORE_10, &CQuestionDialog::OnEnChangeEditScore10)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_1, &CQuestionDialog::OnCbnSelchangeComboType1)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_2, &CQuestionDialog::OnCbnSelchangeComboType2)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_4, &CQuestionDialog::OnCbnSelchangeComboType4)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_3, &CQuestionDialog::OnCbnSelchangeComboType3)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_5, &CQuestionDialog::OnCbnSelchangeComboType5)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_6, &CQuestionDialog::OnCbnSelchangeComboType6)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_7, &CQuestionDialog::OnCbnSelchangeComboType7)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_8, &CQuestionDialog::OnCbnSelchangeComboType8)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_9, &CQuestionDialog::OnCbnSelchangeComboType9)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_10, &CQuestionDialog::OnCbnSelchangeComboType10)
END_MESSAGE_MAP()


// CQuestionDialog message handlers

void CQuestionDialog::SetQuestions(const CArray<SQuestion*, SQuestion*>& questionArray) {
	m_questionArray.RemoveAll();

	for (int i = 0; i < questionArray.GetCount(); i++) {
		SQuestion* newQuestion = new SQuestion(i + 1);
		newQuestion->copy(questionArray.GetAt(i));

		m_questionArray.Add(newQuestion);
	}

	m_nPageIndex = 0;
	m_nPageCount = m_questionArray.GetCount() / 10 + 1;
}

void CQuestionDialog::SetTopics(const CStringArray& sTopics) {
	m_sTopicArray.RemoveAll();
	for (int i = 0; i < sTopics.GetCount(); i++) {
		m_sTopicArray.Add(sTopics[i]);
	}
}

void CQuestionDialog::ClearQuestion() {
	for (int i = 0; i < m_questionArray.GetCount(); i++) {
		SQuestion* pQuestion = m_questionArray.GetAt(i);
		delete pQuestion, pQuestion = 0;
	}
	m_questionArray.RemoveAll();
}

BOOL CQuestionDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(_T("GENERATE QUESTIONS"));

	InitControls();
	MovePage(m_nPageIndex);

	return TRUE;
}

void CQuestionDialog::InitControls() {
	m_pNumberLabels[0] = (CStatic*)(GetDlgItem(IDC_NUMBER1));
	m_pNumberLabels[1] = (CStatic*)(GetDlgItem(IDC_NUMBER2));
	m_pNumberLabels[2] = (CStatic*)(GetDlgItem(IDC_NUMBER3));
	m_pNumberLabels[3] = (CStatic*)(GetDlgItem(IDC_NUMBER4));
	m_pNumberLabels[4] = (CStatic*)(GetDlgItem(IDC_NUMBER5));
	m_pNumberLabels[5] = (CStatic*)(GetDlgItem(IDC_NUMBER6));
	m_pNumberLabels[6] = (CStatic*)(GetDlgItem(IDC_NUMBER7));
	m_pNumberLabels[7] = (CStatic*)(GetDlgItem(IDC_NUMBER8));
	m_pNumberLabels[8] = (CStatic*)(GetDlgItem(IDC_NUMBER9));
	m_pNumberLabels[9] = (CStatic*)(GetDlgItem(IDC_NUMBER10));

	m_pTypeCombos[0] = (CComboBox*)(GetDlgItem(IDC_COMBO_TYPE_1));
	m_pTypeCombos[1] = (CComboBox*)(GetDlgItem(IDC_COMBO_TYPE_2));
	m_pTypeCombos[2] = (CComboBox*)(GetDlgItem(IDC_COMBO_TYPE_3));
	m_pTypeCombos[3] = (CComboBox*)(GetDlgItem(IDC_COMBO_TYPE_4));
	m_pTypeCombos[4] = (CComboBox*)(GetDlgItem(IDC_COMBO_TYPE_5));
	m_pTypeCombos[5] = (CComboBox*)(GetDlgItem(IDC_COMBO_TYPE_6));
	m_pTypeCombos[6] = (CComboBox*)(GetDlgItem(IDC_COMBO_TYPE_7));
	m_pTypeCombos[7] = (CComboBox*)(GetDlgItem(IDC_COMBO_TYPE_8));
	m_pTypeCombos[8] = (CComboBox*)(GetDlgItem(IDC_COMBO_TYPE_9));
	m_pTypeCombos[9] = (CComboBox*)(GetDlgItem(IDC_COMBO_TYPE_10));

	m_pTopicCombos[0] = (CComboBox*)(GetDlgItem(IDC_COMBO_TOPIC_1));
	m_pTopicCombos[1] = (CComboBox*)(GetDlgItem(IDC_COMBO_TOPIC_2));
	m_pTopicCombos[2] = (CComboBox*)(GetDlgItem(IDC_COMBO_TOPIC_3));
	m_pTopicCombos[3] = (CComboBox*)(GetDlgItem(IDC_COMBO_TOPIC_4));
	m_pTopicCombos[4] = (CComboBox*)(GetDlgItem(IDC_COMBO_TOPIC_5));
	m_pTopicCombos[5] = (CComboBox*)(GetDlgItem(IDC_COMBO_TOPIC_6));
	m_pTopicCombos[6] = (CComboBox*)(GetDlgItem(IDC_COMBO_TOPIC_7));
	m_pTopicCombos[7] = (CComboBox*)(GetDlgItem(IDC_COMBO_TOPIC_8));
	m_pTopicCombos[8] = (CComboBox*)(GetDlgItem(IDC_COMBO_TOPIC_9));
	m_pTopicCombos[9] = (CComboBox*)(GetDlgItem(IDC_COMBO_TOPIC_10));

	/*m_pTagCombos[0] = (CComboBox*)(GetDlgItem(IDC_COMBO_TAG_1));
	m_pTagCombos[1] = (CComboBox*)(GetDlgItem(IDC_COMBO_TAG_2));
	m_pTagCombos[2] = (CComboBox*)(GetDlgItem(IDC_COMBO_TAG_3));
	m_pTagCombos[3] = (CComboBox*)(GetDlgItem(IDC_COMBO_TAG_4));
	m_pTagCombos[4] = (CComboBox*)(GetDlgItem(IDC_COMBO_TAG_5));
	m_pTagCombos[5] = (CComboBox*)(GetDlgItem(IDC_COMBO_TAG_6));
	m_pTagCombos[6] = (CComboBox*)(GetDlgItem(IDC_COMBO_TAG_7));
	m_pTagCombos[7] = (CComboBox*)(GetDlgItem(IDC_COMBO_TAG_8));
	m_pTagCombos[8] = (CComboBox*)(GetDlgItem(IDC_COMBO_TAG_9));
	m_pTagCombos[9] = (CComboBox*)(GetDlgItem(IDC_COMBO_TAG_10));*/


	

	m_pTagCombos[0] = (CMyCheckCombiBox*)(GetDlgItem(IDC_COMBO_TAG_1));
	m_pTagCombos[1] = (CMyCheckCombiBox*)(GetDlgItem(IDC_COMBO_TAG_2));
	m_pTagCombos[2] = (CMyCheckCombiBox*)(GetDlgItem(IDC_COMBO_TAG_3));
	m_pTagCombos[3] = (CMyCheckCombiBox*)(GetDlgItem(IDC_COMBO_TAG_4));
	m_pTagCombos[4] = (CMyCheckCombiBox*)(GetDlgItem(IDC_COMBO_TAG_5));
	m_pTagCombos[5] = (CMyCheckCombiBox*)(GetDlgItem(IDC_COMBO_TAG_6));
	m_pTagCombos[6] = (CMyCheckCombiBox*)(GetDlgItem(IDC_COMBO_TAG_7));
	m_pTagCombos[7] = (CMyCheckCombiBox*)(GetDlgItem(IDC_COMBO_TAG_8));
	m_pTagCombos[8] = (CMyCheckCombiBox*)(GetDlgItem(IDC_COMBO_TAG_9));
	m_pTagCombos[9] = (CMyCheckCombiBox*)(GetDlgItem(IDC_COMBO_TAG_10));
	

	m_pScoreEdits[0] = (CEdit*)(GetDlgItem(IDC_EDIT_SCORE_1));
	m_pScoreEdits[1] = (CEdit*)(GetDlgItem(IDC_EDIT_SCORE_2));
	m_pScoreEdits[2] = (CEdit*)(GetDlgItem(IDC_EDIT_SCORE_3));
	m_pScoreEdits[3] = (CEdit*)(GetDlgItem(IDC_EDIT_SCORE_4));
	m_pScoreEdits[4] = (CEdit*)(GetDlgItem(IDC_EDIT_SCORE_5));
	m_pScoreEdits[5] = (CEdit*)(GetDlgItem(IDC_EDIT_SCORE_6));
	m_pScoreEdits[6] = (CEdit*)(GetDlgItem(IDC_EDIT_SCORE_7));
	m_pScoreEdits[7] = (CEdit*)(GetDlgItem(IDC_EDIT_SCORE_8));
	m_pScoreEdits[8] = (CEdit*)(GetDlgItem(IDC_EDIT_SCORE_9));
	m_pScoreEdits[9] = (CEdit*)(GetDlgItem(IDC_EDIT_SCORE_10));

	m_pGenerateSubBtn[0] = (CButton*)(GetDlgItem(IDC_BTN_SUB_1));
	m_pGenerateSubBtn[1] = (CButton*)(GetDlgItem(IDC_BTN_SUB_2));
	m_pGenerateSubBtn[2] = (CButton*)(GetDlgItem(IDC_BTN_SUB_3));
	m_pGenerateSubBtn[3] = (CButton*)(GetDlgItem(IDC_BTN_SUB_4));
	m_pGenerateSubBtn[4] = (CButton*)(GetDlgItem(IDC_BTN_SUB_5));
	m_pGenerateSubBtn[5] = (CButton*)(GetDlgItem(IDC_BTN_SUB_6));
	m_pGenerateSubBtn[6] = (CButton*)(GetDlgItem(IDC_BTN_SUB_7));
	m_pGenerateSubBtn[7] = (CButton*)(GetDlgItem(IDC_BTN_SUB_8));
	m_pGenerateSubBtn[8] = (CButton*)(GetDlgItem(IDC_BTN_SUB_9));
	m_pGenerateSubBtn[9] = (CButton*)(GetDlgItem(IDC_BTN_SUB_10));

	for (int i = 0; i < 10; i++) {
		m_pTypeCombos[i]->Clear();
		m_pTopicCombos[i]->Clear();
		m_pTagCombos[i]->Clear();
		
		m_pTypeCombos[i]->AddString(_T("1,2,3,..."));
		m_pTypeCombos[i]->AddString(_T("1),2),3),..."));
		m_pTypeCombos[i]->AddString(_T("A,B,C..."));
		m_pTypeCombos[i]->AddString(_T("A),B),C)..."));

		for (int j = 0; j < m_sTopicArray.GetCount(); j++) {
			m_pTopicCombos[i]->AddString(m_sTopicArray.GetAt(j));
		}
		for (int j = 0; j < g_tagArray.GetCount(); j++) {
			m_pTagCombos[i]->AddString(g_tagArray.GetAt(j)->mName);
		}
	}

	if (m_questionArray.GetCount() <= 10) {
		((CButton*)GetDlgItem(IDC_BTN_NEXT))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BTN_PREV))->ShowWindow(SW_HIDE);
	}
}

void CQuestionDialog::MovePage(int nPageIndex) {
	int nCount = (nPageIndex == m_nPageCount - 1) ? m_questionArray.GetCount() % 10 : 10;
	
	for (int i = 0; i < 10; i++) {
		if (i < nCount) {
			m_pNumberLabels[i]->ShowWindow(SW_SHOW);
			m_pTypeCombos[i]->ShowWindow(SW_SHOW);
			m_pTopicCombos[i]->ShowWindow(SW_SHOW);
			m_pTagCombos[i]->ShowWindow(SW_SHOW);
			m_pScoreEdits[i]->ShowWindow(SW_SHOW);
			m_pGenerateSubBtn[i]->ShowWindow(SW_SHOW);
		} else {
			m_pNumberLabels[i]->ShowWindow(SW_HIDE);
			m_pTypeCombos[i]->ShowWindow(SW_HIDE);
			m_pTopicCombos[i]->ShowWindow(SW_HIDE);
			m_pTagCombos[i]->ShowWindow(SW_HIDE);
			m_pScoreEdits[i]->ShowWindow(SW_HIDE);
			m_pGenerateSubBtn[i]->ShowWindow(SW_HIDE);
		}
	}

	int nStartIndex = nPageIndex * 10;
	int nEndIndex = nStartIndex + nCount;
	int nCtrlIndex = 0;
	for (int i = nStartIndex; i < nEndIndex; i++, nCtrlIndex++) {
		SQuestion* pQuestion = m_questionArray.GetAt(i);

		CString sText;
		BOOL bFind = FALSE;
		for (int j = 0; j < m_pTypeCombos[nCtrlIndex]->GetCount(); j++) {
			m_pTypeCombos[nCtrlIndex]->GetLBText(j, sText);
			if (sText == pQuestion->mType) {
				m_pTypeCombos[nCtrlIndex]->SetCurSel(j);
				bFind = TRUE;
				break;
			}
		}
		if (!bFind) m_pTypeCombos[nCtrlIndex]->SetCurSel(-1);

		bFind = FALSE;
		for (int j = 0; j < m_pTopicCombos[nCtrlIndex]->GetCount(); j++) {
			m_pTopicCombos[nCtrlIndex]->GetLBText(j, sText);
			if (sText == pQuestion->mTopic) {
				m_pTopicCombos[nCtrlIndex]->SetCurSel(j);
				bFind = TRUE;
				break;
			}
		}
		if (!bFind) m_pTopicCombos[nCtrlIndex]->SetCurSel(-1);
		CStringArray sTags;
		SplitString(pQuestion->mTag, _T(","), sTags);
		for(int j=0; j<m_pTagCombos[nCtrlIndex]->GetCount();j++)		{
			m_pTagCombos[nCtrlIndex]->SetCheck(j, FALSE);
			
			CString sTag;
			m_pTagCombos[nCtrlIndex]->GetLBText(j,sTag);
			for(int k=0;k< sTags.GetCount(); k++){
				if(sTags.GetAt(k) == sTag){
					m_pTagCombos[nCtrlIndex]->SetCheck(j, TRUE);
					break;
				}
			}
		}

		/*bFind = FALSE;
		for (int j = 0; j < m_pTagCombos[nCtrlIndex]->GetCount(); j++) {
			m_pTagCombos[nCtrlIndex]->GetLBText(j, sText);
			if (sText == pQuestion->mTag) {
				m_pTagCombos[nCtrlIndex]->SetCurSel(j);
				bFind = TRUE;
				break;
			}
		}*/

		if (!bFind) m_pTagCombos[nCtrlIndex]->SetCurSel(-1);

		


		WCHAR szNumber[5];
		if (pQuestion->mIsSubQuestion) {
			wsprintf(szNumber, _T("%d.%d"), pQuestion->mMainNo, pQuestion->mSubNo);
		} else {
			wsprintf(szNumber, _T("%d"), pQuestion->mMainNo);
		}
		m_pNumberLabels[nCtrlIndex]->SetWindowText(szNumber);

		if (pQuestion->mIsLastSub) {
			m_pGenerateSubBtn[nCtrlIndex]->ShowWindow(SW_SHOW);
		} else {
			m_pGenerateSubBtn[nCtrlIndex]->ShowWindow(SW_HIDE);
		}

		WCHAR szScore[5];
		wsprintf(szScore, _T("%d"), (int)pQuestion->mScore);
		m_pScoreEdits[nCtrlIndex]->SetWindowText(szScore);
	}
}

int CQuestionDialog::GetQuestionCount() {
	return m_questionArray.GetCount();
}

SQuestion* CQuestionDialog::GetQuestion(int nIndex) {
	if (nIndex >= m_questionArray.GetCount()) return 0;
	return m_questionArray.GetAt(nIndex);
}

void CQuestionDialog::OnBnClickedBtnPrev()
{
	if (m_nPageIndex == 0) return;

	m_nPageIndex--;
	MovePage(m_nPageIndex);
}


void CQuestionDialog::OnBnClickedBtnNext()
{
	if (m_nPageIndex == m_nPageCount - 1) return;

	m_nPageIndex++;
	MovePage(m_nPageIndex);
}


void CQuestionDialog::OnBnClickedBtnSub1()
{
	int nIndex = m_nPageIndex * 10;
	GenerateSubQuestion(nIndex);
}


void CQuestionDialog::OnBnClickedBtnSub2()
{
	int nIndex = m_nPageIndex * 10 + 1;
	GenerateSubQuestion(nIndex);
}


void CQuestionDialog::OnBnClickedBtnSub3()
{
	int nIndex = m_nPageIndex * 10 + 2;
	GenerateSubQuestion(nIndex);
}


void CQuestionDialog::OnBnClickedBtnSub4()
{
	int nIndex = m_nPageIndex * 10 + 3;
	GenerateSubQuestion(nIndex);
}


void CQuestionDialog::OnBnClickedBtnSub5()
{
	int nIndex = m_nPageIndex * 10 + 4;
	GenerateSubQuestion(nIndex);
}


void CQuestionDialog::OnBnClickedBtnSub6()
{
	int nIndex = m_nPageIndex * 10 + 5;
	GenerateSubQuestion(nIndex);
}


void CQuestionDialog::OnBnClickedBtnSub7()
{
	int nIndex = m_nPageIndex * 10 + 6;
	GenerateSubQuestion(nIndex);
}


void CQuestionDialog::OnBnClickedBtnSub8()
{
	int nIndex = m_nPageIndex * 10 + 7;
	GenerateSubQuestion(nIndex);
}


void CQuestionDialog::OnBnClickedBtnSub9()
{
	int nIndex = m_nPageIndex * 10 + 8;
	GenerateSubQuestion(nIndex);
}


void CQuestionDialog::OnBnClickedBtnSub10()
{
	int nIndex = m_nPageIndex * 10 + 9;
	GenerateSubQuestion(nIndex);
}

void CQuestionDialog::GenerateSubQuestion(int nIndex) {
	SQuestion* pQuestion = m_questionArray.GetAt(nIndex);
	if (!pQuestion->mIsSubQuestion) {
		pQuestion->mIsSubQuestion = TRUE;
		pQuestion->mSubNo = 1;
	}
	pQuestion->mIsLastSub = FALSE;

	SQuestion* pNewQuestion = new SQuestion(pQuestion->mMainNo);
	pNewQuestion->copy(pQuestion);
	pNewQuestion->mSubNo++;
	pNewQuestion->mScore = 0;
	pNewQuestion->mIsLastSub = TRUE;

	m_questionArray.InsertAt(nIndex + 1, pNewQuestion);

	m_nPageCount = m_questionArray.GetCount() / 10 + 1;
	if (nIndex == 9) {
		m_nPageIndex++;
	}
	MovePage(m_nPageIndex);
}


void CQuestionDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CQuestionDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CQuestionDialog::OnSelchangeComboTag1()
{
	OnTagChanged(0);
}


void CQuestionDialog::OnCbnSelchangeComboTag2()
{
	OnTagChanged(1);
}


void CQuestionDialog::OnCbnSelchangeComboTag3()
{
	OnTagChanged(2);
}


void CQuestionDialog::OnCbnSelchangeComboTag4()
{
	OnTagChanged(3);
}


void CQuestionDialog::OnCbnSelchangeComboTag5()
{
	OnTagChanged(4);
}


void CQuestionDialog::OnCbnSelchangeComboTag6()
{
	OnTagChanged(5);
}


void CQuestionDialog::OnCbnSelchangeComboTag7()
{
	OnTagChanged(6);
}


void CQuestionDialog::OnCbnSelchangeComboTag8()
{
	OnTagChanged(7);
}


void CQuestionDialog::OnCbnSelchangeComboTag9()
{
	OnTagChanged(8);
}


void CQuestionDialog::OnCbnSelchangeComboTag10()
{
	OnTagChanged(9);
}


void CQuestionDialog::OnCbnSelchangeComboTopic10()
{
	OnTopicChanged(9);
}


void CQuestionDialog::OnCbnSelchangeComboTopic9()
{
	OnTopicChanged(8);
}


void CQuestionDialog::OnCbnSelchangeComboTopic8()
{
	OnTopicChanged(7);
}


void CQuestionDialog::OnCbnSelchangeComboTopic7()
{
	OnTopicChanged(6);
}


void CQuestionDialog::OnCbnSelchangeComboTopic6()
{
	OnTopicChanged(5);
}


void CQuestionDialog::OnCbnSelchangeComboTopic5()
{
	OnTopicChanged(4);
}


void CQuestionDialog::OnCbnSelchangeComboTopic4()
{
	OnTopicChanged(3);
}


void CQuestionDialog::OnCbnSelchangeComboTopic3()
{
	OnTopicChanged(2);
}


void CQuestionDialog::OnCbnSelchangeComboTopic2()
{
	OnTopicChanged(1);
}


void CQuestionDialog::OnCbnSelchangeComboTopic1()
{
	OnTopicChanged(0);
}


void CQuestionDialog::OnScoreChanged(int nCtrlIdx) {
	CString sScore;
	m_pScoreEdits[nCtrlIdx]->GetWindowText(sScore);

	int nIndex = m_nPageIndex * 10 + nCtrlIdx;
	SQuestion* pQuestion = m_questionArray.GetAt(nIndex);
	pQuestion->mScore = _wtoi(sScore);
}


void CQuestionDialog::OnTagChanged(int nCtrlIdx) {
	CString sTag;
	m_pTagCombos[nCtrlIdx]->GetWindowText(sTag);
	sTag.Replace(_T(", "), _T(","));
	int nIndex = m_nPageIndex * 10 + nCtrlIdx;
	SQuestion* pQuestion = m_questionArray.GetAt(nIndex);
	pQuestion->mTag = sTag;
}

void CQuestionDialog::OnTopicChanged(int nCtrlIdx) {
	CString sTopic;
	m_pTopicCombos[nCtrlIdx]->GetLBText(m_pTopicCombos[nCtrlIdx]->GetCurSel(), sTopic);

	int nIndex = m_nPageIndex * 10 + nCtrlIdx;
	SQuestion* pQuestion = m_questionArray.GetAt(nIndex);
	pQuestion->mTopic = sTopic;
}

void CQuestionDialog::OnTypeChanged(int nCtrlIdx) {
	CString sType;
	m_pTypeCombos[nCtrlIdx]->GetLBText(m_pTypeCombos[nCtrlIdx]->GetCurSel(), sType);

	int nIndex = m_nPageIndex * 10 + nCtrlIdx;
	SQuestion* pQuestion = m_questionArray.GetAt(nIndex);
	pQuestion->mType = sType;
}

void CQuestionDialog::OnEnChangeEditScore1()
{
	int nIndex = 0;
	OnScoreChanged(nIndex);
}


void CQuestionDialog::OnEnChangeEditScore2()
{
	int nIndex = 1;
	OnScoreChanged(nIndex);
}


void CQuestionDialog::OnEnChangeEditScore3()
{
	int nIndex = 2;
	OnScoreChanged(nIndex);
}


void CQuestionDialog::OnEnChangeEditScore4()
{
	int nIndex = 3;
	OnScoreChanged(nIndex);
}


void CQuestionDialog::OnEnChangeEditScore5()
{
	int nIndex = 4;
	OnScoreChanged(nIndex);
}


void CQuestionDialog::OnEnChangeEditScore6()
{
	int nIndex = 5;
	OnScoreChanged(nIndex);
}


void CQuestionDialog::OnEnChangeEditScore7()
{
	int nIndex = 6;
	OnScoreChanged(nIndex);
}


void CQuestionDialog::OnEnChangeEditScore8()
{
	int nIndex = 7;
	OnScoreChanged(nIndex);
}


void CQuestionDialog::OnEnChangeEditScore9()
{
	int nIndex = 8;
	OnScoreChanged(nIndex);
}


void CQuestionDialog::OnEnChangeEditScore10()
{
	int nIndex = 9;
	OnScoreChanged(nIndex);
}


void CQuestionDialog::OnCbnSelchangeComboType1()
{
	OnTypeChanged(0);
}


void CQuestionDialog::OnCbnSelchangeComboType2()
{
	OnTypeChanged(1);
}


void CQuestionDialog::OnCbnSelchangeComboType3()
{
	OnTypeChanged(2);
}


void CQuestionDialog::OnCbnSelchangeComboType4()
{
	OnTypeChanged(3);
}


void CQuestionDialog::OnCbnSelchangeComboType5()
{
	OnTypeChanged(4);
}


void CQuestionDialog::OnCbnSelchangeComboType6()
{
	OnTypeChanged(5);
}


void CQuestionDialog::OnCbnSelchangeComboType7()
{
	OnTypeChanged(6);
}


void CQuestionDialog::OnCbnSelchangeComboType8()
{
	OnTypeChanged(7);
}


void CQuestionDialog::OnCbnSelchangeComboType9()
{
	OnTypeChanged(8);
}


void CQuestionDialog::OnCbnSelchangeComboType10()
{
	OnTypeChanged(9);
}
