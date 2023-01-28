#pragma once


// CHomePage dialog

class CHomePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CHomePage)

public:
	CHomePage();
	virtual ~CHomePage();

// Dialog Data
	enum { IDD = IDD_HOME_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
