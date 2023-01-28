#pragma once
class CMSWord
{
public:
	IDispatch* m_pWApp;
	IDispatch* m_pDocuments;
	IDispatch* m_pActiveDocument;
	HRESULT m_hr;
	HRESULT Initialize(BOOL bVisible = TRUE);

public:
	CMSWord(void);
	~CMSWord(void);

	HRESULT SetVisible(BOOL bVisible = TRUE);
	HRESULT Quit();
	HRESULT OpenDocument(LPCTSTR szFileName, BOOL bVisible = TRUE);
	HRESULT NewDocument(BOOL bVisible = TRUE);
	HRESULT CloseActiveDocument(BOOL bSave = TRUE);
	HRESULT CloseDocument(BOOL bSave = TRUE);
	HRESULT SetSelectionText(LPCTSTR szText);
	HRESULT InsertPicture(LPCTSTR szFilePath);
	HRESULT AddComment(LPCTSTR szComment);
	HRESULT MoveCursor(int nDirection = 2, BOOL bSelection = FALSE);
	HRESULT DeleteChar(BOOL bBack = FALSE);
	HRESULT CheckSpelling(LPCTSTR szWord, BOOL &bResult);
	HRESULT CheckGrammer(LPCTSTR szString, BOOL &bResult);
	HRESULT SetFont(LPCTSTR szFontName, int nSize, BOOL bBold, BOOL bItalic, COLORREF crColor);

	HRESULT ReplaceString(IDispatch* pSelection, IDispatch* pFind, LPCTSTR szFind, LPCTSTR szReplace);
	HRESULT ReplacePicture(IDispatch* pSelection, IDispatch* pFind, LPCTSTR szFind, LPCTSTR szFilePath);
	HRESULT ReplaceChart(IDispatch* pSelection, IDispatch* pFind, LPCTSTR szFind, const CStringArray& sTopics, const CArray<int, int>& nScores);
	HRESULT ReplaceTable(IDispatch* pSelection, IDispatch* pFind, LPCTSTR szFind, const CStringArray& sTopics, const CArray<int, int>& nScores);

	HRESULT SetCellText(IDispatch* pTable, int nRow, int nCol, LPCTSTR szText);

	IDispatch* GetApplicationObject();
	IDispatch* GetSelectionObject(IDispatch* pApplication);
	IDispatch* GetFindObject(IDispatch* pSelection);
};

