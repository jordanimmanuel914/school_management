// DialThread.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "SubmitThread.h"
#include "SendDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialThread

IMPLEMENT_DYNCREATE(CSubmitThread, CWinThread)

CSubmitThread::CSubmitThread()
{
}

CSubmitThread::~CSubmitThread()
{
}

BOOL CSubmitThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CSubmitThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSubmitThread, CWinThread)
	//{{AFX_MSG_MAP(CDialThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialThread message handlers

int CSubmitThread::Run() 
{
	m_pOwner->Execute();
	m_pOwner->PostMessage(WM_FINISHED);

	return 0;
}
