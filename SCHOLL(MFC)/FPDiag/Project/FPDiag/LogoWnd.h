#pragma once

#include "DIBSectionLite.h"

class CLogoWnd : public CWnd
{
public:
	CLogoWnd();
	~CLogoWnd();

public:	
	BOOL Create(CWnd *pParent, RECT *nRect);
	
	void SetFilePath(const CString& sFilePath);
	CString GetFilePath() { return m_sFilePath; }

protected:
	//{{AFX_MSG(CLabel)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_sFilePath;
	CRect m_rect;

	CDIBSectionLite m_dib;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

