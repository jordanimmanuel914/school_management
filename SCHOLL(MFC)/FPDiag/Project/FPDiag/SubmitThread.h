#pragma once
#include "afxwin.h"

class CSendDialog;
class CSubmitThread : public CWinThread
{
	DECLARE_DYNCREATE(CSubmitThread)
protected:
	CSubmitThread();           // protected constructor used by dynamic creation

// Attributes
public:
	CSendDialog *m_pOwner;

public:
	void SetOwner(CSendDialog* pOwner) { m_pOwner = pOwner; };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSubmitThread();

	// Generated message map functions
	//{{AFX_MSG(CDialThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
