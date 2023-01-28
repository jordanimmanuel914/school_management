
// FPDiag.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFPDiagApp:
// See FPDiag.cpp for the implementation of this class
//

class CFPDiagApp : public CWinApp
{
public:
	CFPDiagApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	BOOL ExitInstance(void);
};

extern CFPDiagApp theApp;