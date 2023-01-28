#pragma once

class CMyLabel : public CWnd
{
public:
	enum TextAlign { Left = 0, Center, Right };

	CMyLabel();
	~CMyLabel();

public:	
	BOOL Create(CWnd *pParent, RECT *nRect, CString szStr);

	void SetBackColor(const COLORREF& color);
	void SetTextColor(const COLORREF& color);
	void SetFont(const LOGFONT& font);
	void SetText(const CString& text);

private:
	void PutText();

private:
	RECT m_nRect;
	CSize m_nSize;
	CString	m_nMessage;

	//{{AFX_VIRTUAL(CLabel)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CLabel)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	COLORREF m_textColor;
	COLORREF m_backColor;

	LOGFONT m_logFont;

	TextAlign m_textAlignment;
};
