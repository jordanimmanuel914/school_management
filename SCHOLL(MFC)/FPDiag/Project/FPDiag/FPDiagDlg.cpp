
// FPDiagDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "FPDiagDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define USER_MESSAGE		WM_USER + 2000

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnClose();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFPDiagDlg dialog



CFPDiagDlg::CFPDiagDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFPDiagDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

void CFPDiagDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFPDiagDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()

	ON_MESSAGE(USER_MESSAGE + 01, OnHomeBtn)
	ON_MESSAGE(USER_MESSAGE + 02, OnGradeBtn)
	ON_MESSAGE(USER_MESSAGE + 03, OnTopicBtn)
	ON_MESSAGE(USER_MESSAGE + 04, OnTagsBtn)
	ON_MESSAGE(USER_MESSAGE + 05, OnReportsBtn)
END_MESSAGE_MAP()


// CFPDiagDlg message handlers

BOOL CFPDiagDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CreateWindows();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFPDiagDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFPDiagDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFPDiagDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFPDiagDlg::CreateWindows(void)
{
	CreateButtons();
	CreatePages();

	m_bInitialized = TRUE;
}

void CFPDiagDlg::CreateButtons() 
{
	UINT nResourceID[6];
	
	nResourceID[0] = IDB_HOME_UP;
	nResourceID[1] = IDB_HOME_DOWN;
	nResourceID[2] = IDB_HOME_UP;
	nResourceID[3] = IDB_HOME_DOWN;
	nResourceID[4] = 0;
	nResourceID[5] = 0;

	CRect homeBtnRect(20, 20, 20 + 120, 20 + 40);
	m_homeButton.Create(this, nResourceID, &homeBtnRect, USER_MESSAGE + 01);
	m_homeButton.ShowWindow(SW_SHOW);

	nResourceID[0] = IDB_GRADE_UP;
	nResourceID[1] = IDB_GRADE_DOWN;
	nResourceID[2] = IDB_GRADE_UP;
	nResourceID[3] = IDB_GRADE_DOWN;

	CRect gradeBtnRect(150, 20, 150 + 120, 20 + 40);
	m_gradeButton.Create(this, nResourceID, &gradeBtnRect, USER_MESSAGE + 02);

	nResourceID[0] = IDB_TOPIC_UP;
	nResourceID[1] = IDB_TOPIC_DOWN;
	nResourceID[2] = IDB_TOPIC_UP;
	nResourceID[3] = IDB_TOPIC_DOWN;

	CRect topicBtnRect(280, 20, 280 + 120, 20 + 40);
	m_topicButton.Create(this, nResourceID, &topicBtnRect, USER_MESSAGE + 03);

	nResourceID[0] = IDB_TAGS_UP;
	nResourceID[1] = IDB_TAGS_DOWN;
	nResourceID[2] = IDB_TAGS_UP;
	nResourceID[3] = IDB_TAGS_DOWN;

	CRect tagsBtnRect(410, 20, 410 + 120, 20 + 40);
	m_tagsButton.Create(this, nResourceID, &tagsBtnRect, USER_MESSAGE + 04);
		
	nResourceID[0] = IDB_REPORTS_UP;
	nResourceID[1] = IDB_REPORTS_DOWN;
	nResourceID[2] = IDB_REPORTS_UP;
	nResourceID[3] = IDB_REPORTS_DOWN;

	CRect reportsBtnRect(540, 20, 540 + 120, 20 + 40);
	m_reportsButton.Create(this, nResourceID, &reportsBtnRect, USER_MESSAGE + 05);

	m_homeButton.SetCheck(1);
}

void CFPDiagDlg::CreatePages()
{
	CRect rect;
	GetClientRect(&rect);

	m_homePage.Create(IDD_HOME_PAGE);
	m_homePage.MoveWindow(20, 70, rect.Width() - 40, rect.Height() - 90);

	m_gradePage.Create(IDD_GRADE_PAGE);
	m_gradePage.MoveWindow(20, 70, rect.Width() - 40, rect.Height() - 90);

	m_topicPage.Create(IDD_TOPIC_PAGE);
	m_topicPage.MoveWindow(20, 70, rect.Width() - 40, rect.Height() - 90);

	m_tagsPage.Create(IDD_TAGS_PAGE);
	m_tagsPage.MoveWindow(20, 70, rect.Width() - 40, rect.Height() - 90);

	m_reportsPage.Create(IDD_REPORTS_PAGE);
	m_reportsPage.MoveWindow(20, 70, rect.Width() - 40, rect.Height() - 90);

	ShowHomePage();
}

void CFPDiagDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_bInitialized) {
		m_homePage.MoveWindow(20, 70, cx - 40, cy - 90);
		m_gradePage.MoveWindow(20, 70, cx - 40, cy - 90);
		m_topicPage.MoveWindow(20, 70, cx - 40, cy - 90);
		m_tagsPage.MoveWindow(20, 70, cx - 40, cy - 90);
		m_reportsPage.MoveWindow(20, 70, cx - 40, cy - 90);
	}
}

LRESULT CFPDiagDlg::OnHomeBtn(WPARAM wParam, LPARAM lParam)
{
	m_homeButton.SetCheck(1);
	m_gradeButton.SetCheck(0);
	m_topicButton.SetCheck(0);
	m_tagsButton.SetCheck(0);
	m_reportsButton.SetCheck(0);

	ShowHomePage();
	return 0;
}

LRESULT CFPDiagDlg::OnGradeBtn(WPARAM wParam, LPARAM lParam)
{
	m_homeButton.SetCheck(0);
	m_gradeButton.SetCheck(1);
	m_topicButton.SetCheck(0);
	m_tagsButton.SetCheck(0);
	m_reportsButton.SetCheck(0);

	ShowGradePage();
	return 0;
}

LRESULT CFPDiagDlg::OnTopicBtn(WPARAM wParam, LPARAM lParam)
{
	m_homeButton.SetCheck(0);
	m_gradeButton.SetCheck(0);
	m_topicButton.SetCheck(1);
	m_tagsButton.SetCheck(0);
	m_reportsButton.SetCheck(0);

	ShowTopicPage();
	return 0;
}

LRESULT CFPDiagDlg::OnTagsBtn(WPARAM wParam, LPARAM lParam)
{
	m_homeButton.SetCheck(0);
	m_gradeButton.SetCheck(0);
	m_topicButton.SetCheck(0);
	m_tagsButton.SetCheck(1);
	m_reportsButton.SetCheck(0);

	ShowTagsPage();
	return 0;
}

LRESULT CFPDiagDlg::OnReportsBtn(WPARAM wParam, LPARAM lParam)
{
	m_homeButton.SetCheck(0);
	m_gradeButton.SetCheck(0);
	m_topicButton.SetCheck(0);
	m_tagsButton.SetCheck(0);
	m_reportsButton.SetCheck(1);

	ShowReportsPage();
	return 0;
}

void CFPDiagDlg::ShowHomePage() {
	m_homePage.ShowWindow(SW_SHOW);
	m_gradePage.ShowWindow(SW_HIDE);
	m_topicPage.ShowWindow(SW_HIDE);
	m_tagsPage.ShowWindow(SW_HIDE);
	m_reportsPage.ShowWindow(SW_HIDE);
}

void CFPDiagDlg::ShowGradePage() {
	m_homePage.ShowWindow(SW_HIDE);
	m_gradePage.ShowWindow(SW_SHOW);
	m_topicPage.ShowWindow(SW_HIDE);
	m_tagsPage.ShowWindow(SW_HIDE);
	m_reportsPage.ShowWindow(SW_HIDE);
}

void CFPDiagDlg::ShowTopicPage() {
	m_homePage.ShowWindow(SW_HIDE);
	m_gradePage.ShowWindow(SW_HIDE);
	m_topicPage.ShowWindow(SW_SHOW);
	m_tagsPage.ShowWindow(SW_HIDE);
	m_reportsPage.ShowWindow(SW_HIDE);
}

void CFPDiagDlg::ShowTagsPage() {
	m_homePage.ShowWindow(SW_HIDE);
	m_gradePage.ShowWindow(SW_HIDE);
	m_topicPage.ShowWindow(SW_HIDE);
	m_tagsPage.ShowWindow(SW_SHOW);
	m_reportsPage.ShowWindow(SW_HIDE);
}

void CFPDiagDlg::ShowReportsPage() {
	m_homePage.ShowWindow(SW_HIDE);
	m_gradePage.ShowWindow(SW_HIDE);
	m_topicPage.ShowWindow(SW_HIDE);
	m_tagsPage.ShowWindow(SW_HIDE);
	m_reportsPage.ShowWindow(SW_SHOW);
}



void CFPDiagDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}
