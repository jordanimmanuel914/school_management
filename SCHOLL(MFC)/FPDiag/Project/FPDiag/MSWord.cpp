#include "stdafx.h"
#include "MSWord.h"

HRESULT OLEMethod(int nType, VARIANT* pvResult, IDispatch* pDisp, LPOLESTR ptName, int cArgs...)
{
	if (!pDisp) return E_FAIL;

	va_list marker;
	va_start(marker, cArgs);

	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	DISPID dispidNamed = DISPID_PROPERTYPUT;
	DISPID dispID;
	char szName[20];

	WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);

	HRESULT hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);
	if (FAILED(hr)) return hr;

	VARIANT *pArgs = new VARIANT[cArgs + 1];
	for (int i = 0; i < cArgs; i++) {
		pArgs[i] = va_arg(marker, VARIANT);
	}

	dp.cArgs = cArgs;
	dp.rgvarg = pArgs;

	if (nType & DISPATCH_PROPERTYPUT) {
		dp.cNamedArgs = 1;
		dp.rgdispidNamedArgs = &dispidNamed;
	}

	hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, nType, &dp, pvResult, NULL, NULL);
	if (FAILED(hr)) return hr;

	va_end(marker);

	delete [] pArgs;
	return hr;
}

CMSWord::CMSWord(void)
{
	m_pWApp = NULL;
	m_pDocuments = NULL;
	m_pActiveDocument = NULL;
}


CMSWord::~CMSWord(void)
{
	Quit();
	if (m_pWApp != NULL) {
		m_pWApp->Release();
		m_pWApp = NULL;
	}
	CoUninitialize();
}

HRESULT CMSWord::Initialize(BOOL bVisible) {
	CoInitialize(NULL);

	CLSID clsid;
	m_hr = CLSIDFromProgID(L"Word.Application", &clsid);
	if (SUCCEEDED(m_hr)) {
		m_hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&m_pWApp);
		if (FAILED(m_hr)) m_pWApp = NULL;
	}
	m_hr = SetVisible(bVisible);

	return m_hr;
}

HRESULT CMSWord::SetVisible(BOOL bVisible) {
	VARIANT x;
	x.vt = VT_I4;
	x.lVal = bVisible;

	return OLEMethod(DISPATCH_PROPERTYPUT, NULL, m_pWApp, L"Visible", 1, x);
}

HRESULT CMSWord::OpenDocument(LPCTSTR szFileName, BOOL bVisible) {
	if (m_pWApp == NULL) {
		if (FAILED(m_hr == Initialize(bVisible))) return m_hr;
	}

	COleVariant vFName(szFileName);
	VARIANT fname = vFName.Detach();

	// GetDocuments
	{
		VARIANT result;
		VariantInit(&result);
		m_hr = OLEMethod(DISPATCH_PROPERTYGET, &result, m_pWApp, L"Documents", 0);
		m_pDocuments = result.pdispVal;
	}

	// OpenDocument
	{
		VARIANT result;
		VariantInit(&result);
		m_hr = OLEMethod(DISPATCH_METHOD, &result, m_pDocuments, L"Open", 1, fname);
		m_pActiveDocument = result.pdispVal;
	}
	return m_hr;
}

HRESULT CMSWord::NewDocument(BOOL bVisible) {
	if (m_pWApp == NULL) {
		if (FAILED(m_hr = Initialize(bVisible))) return m_hr;
	}

	{
		VARIANT result;
		VariantInit(&result);
		m_hr = OLEMethod(DISPATCH_PROPERTYGET, &result, m_pWApp, L"Documents", 0);
		m_pDocuments = result.pdispVal;
	}

	{
		VARIANT result;
		VariantInit(&result);
		m_hr = OLEMethod(DISPATCH_METHOD, &result, m_pDocuments, L"Add", 0);
		m_pActiveDocument = result.pdispVal;
	}
	return m_hr;
}

HRESULT CMSWord::CloseDocument(BOOL bSave) {
	if (m_pWApp == NULL) return E_FAIL;

	VARIANT result;
	VariantInit(&result);

	if (bSave) m_hr = OLEMethod(DISPATCH_METHOD, &result, m_pDocuments, L"Save", 0);
	m_hr = OLEMethod(DISPATCH_METHOD, &result, m_pDocuments, L"Close", 0);
	m_pDocuments = NULL;
	m_pActiveDocument = NULL;

	return m_hr;
}

IDispatch* CMSWord::GetApplicationObject() {
	if (!m_pWApp || !m_pActiveDocument) return NULL;

	IDispatch* pDocApp;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, m_pActiveDocument, L"Application", 0);
		pDocApp = result.pdispVal;
	}
	return pDocApp;
}

IDispatch* CMSWord::GetSelectionObject(IDispatch* pApplication) {
	if (!pApplication) return NULL;

	IDispatch* pSelection;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pApplication, L"Selection", 0);
		pSelection = result.pdispVal;
	}
	return pSelection;
}

IDispatch* CMSWord::GetFindObject(IDispatch* pSelection) {
	if (!pSelection) return NULL;

	IDispatch* pFind;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pSelection, L"Find", 0);
		pFind = result.pdispVal;
	}
	return pFind;
}

HRESULT CMSWord::ReplaceString(IDispatch* pSelection, IDispatch* pFind, LPCTSTR szFind, LPCTSTR szReplace) {
	if (!pFind) return E_FAIL;

	{
		OLEMethod(DISPATCH_METHOD, NULL, pFind, L"ClearFormatting", 0);

		OLEMethod(DISPATCH_METHOD, NULL, pFind, L"Execute", 1, COleVariant(szFind).Detach());
		OLEMethod(DISPATCH_METHOD, NULL, pSelection, L"TypeText", 1, COleVariant(szReplace).Detach());
	}

	return 0;
}

HRESULT CMSWord::ReplacePicture(IDispatch* pSelection, IDispatch* pFind, LPCTSTR szFind, LPCTSTR szFilePath) {
	if (!pFind) return E_FAIL;

	{
		OLEMethod(DISPATCH_METHOD, NULL, pFind, L"ClearFormatting", 0);

		OLEMethod(DISPATCH_METHOD, NULL, pFind, L"Execute", 1, COleVariant(szFind).Detach());
		OLEMethod(DISPATCH_METHOD, NULL, pSelection, L"TypeBackspace", 0);
	}

	IDispatch* pInlineShapes;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pSelection, L"InlineShapes", 0);
		pInlineShapes = result.pdispVal;
	}

	{
		COleVariant varFile(szFilePath);
		COleVariant varLink((BYTE)0);
		COleVariant varSave((BYTE)1);
		OLEMethod(DISPATCH_METHOD, NULL, pInlineShapes, L"AddPicture", 3, varSave.Detach(), varLink.Detach(), varFile.Detach());
	}
	return m_hr;
}

HRESULT CMSWord::ReplaceTable(IDispatch* pSelection, IDispatch* pFind, LPCTSTR szFind, const CStringArray& sTopics, const CArray<int, int>& nScores) {
	if (!pFind) return E_FAIL;

	{
		OLEMethod(DISPATCH_METHOD, NULL, pFind, L"ClearFormatting", 0);

		OLEMethod(DISPATCH_METHOD, NULL, pFind, L"Execute", 1, COleVariant(szFind).Detach());
		OLEMethod(DISPATCH_METHOD, NULL, pSelection, L"TypeBackspace", 0);
	}

	IDispatch* pTables, *pTable, *pColumns, *pColumn;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pSelection, L"Tables", 0);
		pTables = result.pdispVal;
	}

	{
		VARIANT result1, result2;
		VariantInit(&result1);
		VariantInit(&result2);

		m_hr = OLEMethod(DISPATCH_PROPERTYGET, &result1, pSelection, L"Range", 0);

		VARIANT varRange;
		varRange.vt = VT_DISPATCH;
		varRange.pdispVal = result1.pdispVal;
		m_hr = OLEMethod(DISPATCH_METHOD, &result2, pTables, L"Add", 3, COleVariant((LONG)2).Detach(), COleVariant((short)(sTopics.GetCount() + 1)).Detach(), varRange);
		pTable = result2.pdispVal;
		
		m_hr = OLEMethod(DISPATCH_PROPERTYGET, &result2, pTable, L"Columns", 0);
		pColumns = result2.pdispVal;

		// Set Column Width
		m_hr = OLEMethod(DISPATCH_PROPERTYGET, &result2, pColumns, L"First", 0);
		pColumn = result2.pdispVal;

		m_hr = OLEMethod(DISPATCH_PROPERTYPUT, &result2, pColumn, L"Width", 1, COleVariant((float)170).Detach());

		// Set Column Width
		m_hr = OLEMethod(DISPATCH_PROPERTYGET, &result2, pColumns, L"Last", 0);
		pColumn = result2.pdispVal;

		m_hr = OLEMethod(DISPATCH_PROPERTYPUT, &result2, pColumn, L"Width", 1, COleVariant((float)50).Detach());

		SetCellText(pTable, 1, 1, _T("Topic"));
		SetCellText(pTable, 1, 2, _T("Score"));

		for (int i = 0; i < sTopics.GetCount(); i++) {
			CString sTopic = sTopics[i];
			WCHAR szScore[5];
			wsprintf(szScore, _T("%d"), nScores[i]);

			SetCellText(pTable, i + 2, 1, sTopic);
			SetCellText(pTable, i + 2, 2, szScore);
		}
	}
	return m_hr;
}

HRESULT CMSWord::SetCellText(IDispatch* pTable, int nRow, int nCol, LPCTSTR szText) {
	if (!pTable) return E_FAIL;

	IDispatch *pCell, *pRange;
	{
		VARIANT result; 
		VariantInit(&result);

		m_hr = OLEMethod(DISPATCH_METHOD, &result, pTable, L"Cell", 2, COleVariant((BYTE)nCol).Detach(), COleVariant((BYTE)nRow).Detach());
		pCell = result.pdispVal;

		m_hr = OLEMethod(DISPATCH_PROPERTYGET, &result, pCell, L"Range", 0);
		pRange = result.pdispVal;

		m_hr = OLEMethod(DISPATCH_PROPERTYPUT, &result, pRange, L"Text", 1, COleVariant(szText).Detach());
	}
	return m_hr;
}

HRESULT CMSWord::ReplaceChart(IDispatch* pSelection, IDispatch* pFind, LPCTSTR szFind, const CStringArray& sTopics, const CArray<int, int>& nScores) {
	if (!pFind) return E_FAIL;

	{
		OLEMethod(DISPATCH_METHOD, NULL, pFind, L"ClearFormatting", 0);

		OLEMethod(DISPATCH_METHOD, NULL, pFind, L"Execute", 1, COleVariant(szFind).Detach());
		OLEMethod(DISPATCH_METHOD, NULL, pSelection, L"TypeBackspace", 0);
	}

	IDispatch* pInlineShapes;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pSelection, L"InlineShapes", 0);
		pInlineShapes = result.pdispVal;
	}

	IDispatch *pInlineShape, *pChart, *pChartData;
	IDispatch *pWorkBook, *pSheet, *pChartTable, *pDataBodyRange;
	IDispatch *pChartRange, *pChartRows, *pWBApp, *pChartTitle;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_METHOD, &result, pInlineShapes, L"AddChart", 1, COleVariant((BYTE)57).Detach());
		pInlineShape = result.pdispVal;

		m_hr = OLEMethod(DISPATCH_PROPERTYPUT, &result, pInlineShape, L"Width", 1, COleVariant((short)270).Detach());

		OLEMethod(DISPATCH_PROPERTYGET, &result, pInlineShape, L"Chart", 0);
		pChart = result.pdispVal;

		m_hr = OLEMethod(DISPATCH_METHOD, &result, pChart, L"ApplyLayout", 1, COleVariant((BYTE)2));

		TCHAR szChartTemplatePath[512];
		TCHAR szDirectory[512];
		GetCurrentDirectory(512, szDirectory);

		wsprintf(szChartTemplatePath, _T("%s\\Data\\Templates\\Chart1.crtx"), szDirectory);

		OLEMethod(DISPATCH_PROPERTYGET, &result, pChart, L"ChartData", 0);
		pChartData = result.pdispVal;

		OLEMethod(DISPATCH_PROPERTYGET, &result, pChartData, L"WorkBook", 0);
		pWorkBook = result.pdispVal;

		OLEMethod(DISPATCH_PROPERTYGET, &result, pWorkBook, L"Application", 0);
		pWBApp = result.pdispVal;

		OLEMethod(DISPATCH_PROPERTYGET, &result, pWorkBook, L"Sheets", 1, COleVariant((BYTE)1).Detach());
		pSheet = result.pdispVal;

		OLEMethod(DISPATCH_PROPERTYGET, &result, pSheet, L"ListObjects", 1, COleVariant(_T("Table1")).Detach());
		pChartTable = result.pdispVal;

		OLEMethod(DISPATCH_PROPERTYGET, &result, pChartTable, L"DataBodyRange", 0);
		pDataBodyRange = result.pdispVal;

		OLEMethod(DISPATCH_PROPERTYGET, &result, pChartTable, L"Range", 0);
		pChartRange = result.pdispVal;

		OLEMethod(DISPATCH_PROPERTYGET, &result, pChartRange, L"Resize", 2, COleVariant((BYTE)2).Detach(), COleVariant((BYTE)(sTopics.GetCount() + 1)).Detach());
		pChartRange = result.pdispVal;

		m_hr = OLEMethod(DISPATCH_METHOD, NULL, pChartTable, L"Resize", 1, COleVariant(result).Detach());
			Sleep(50);
		OLEMethod(DISPATCH_METHOD, NULL, pDataBodyRange, L"ClearContents", 0);
		OLEMethod(DISPATCH_PROPERTYPUT, &result, pChartRange, L"Cells", 3, COleVariant(_T("% SCORE")).Detach(), COleVariant((BYTE)2).Detach(), COleVariant((LONG)(1)).Detach());
			Sleep(50);	
		for (int i = 0; i < sTopics.GetCount(); i++) {
				Sleep(5);
			OLEMethod(DISPATCH_PROPERTYPUT, &result, pChartRange, L"Cells", 3, COleVariant(sTopics[i]).Detach(), COleVariant((BYTE)1).Detach(), COleVariant((LONG)(i + 2)).Detach());
			OLEMethod(DISPATCH_PROPERTYPUT, &result, pChartRange, L"Cells", 3, COleVariant((short)nScores[i]).Detach(), COleVariant((BYTE)2).Detach(), COleVariant((LONG)(i + 2)).Detach());
		}
		//Sleep(50);
		OLEMethod(DISPATCH_METHOD, NULL, pWorkBook, L"Close", 1, COleVariant((BYTE)2));
		//	Sleep(50);
		OLEMethod(DISPATCH_METHOD, NULL, pWBApp, L"Quit", 0);
		//	Sleep(50);
		m_hr = OLEMethod(DISPATCH_METHOD, NULL, pChart, L"ApplyChartTemplate", 1, COleVariant(szChartTemplatePath).Detach());
		//	Sleep(50);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pChart, L"ChartTitle", 0);
		pChartTitle = result.pdispVal;
		//	Sleep(50);
		OLEMethod(DISPATCH_PROPERTYPUT, NULL, pChartTitle, L"Caption", 1, COleVariant(_T("% SCORE")).Detach());
		//	Sleep(50);
	}
	pSheet->Release();
	pWBApp->Release();
	pWorkBook->Release();

	return m_hr;
}

HRESULT CMSWord::InsertPicture(LPCTSTR szFileName) {
	if (!m_pWApp || !m_pActiveDocument) return E_FAIL;

	IDispatch* pDocApp;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, m_pActiveDocument, L"Application", 0);
		pDocApp = result.pdispVal;
	}
	IDispatch* pSelection;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pDocApp, L"Selection", 0);
		pSelection = result.pdispVal;
	}
	IDispatch* pInlineShapes;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pSelection, L"InlineShapes", 0);
		pInlineShapes = result.pdispVal;
	}

	{
		COleVariant varFile(szFileName);
		COleVariant varLink((BYTE)0);
		COleVariant varSave((BYTE)1);
		OLEMethod(DISPATCH_METHOD, NULL, pInlineShapes, L"AddPicture", 3, varSave.Detach(), varLink.Detach(), varFile.Detach());
	}

	return m_hr;
}

HRESULT CMSWord::DeleteChar(BOOL bBack) {
	if (!m_pWApp || !m_pActiveDocument) return E_FAIL;

	IDispatch* pDocApp;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, m_pActiveDocument, L"Application", 0);
		pDocApp = result.pdispVal;
	}
	IDispatch* pSelection;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pDocApp, L"Selection", 0);
		pSelection = result.pdispVal;
	}

	{
		VARIANT wdCharacter, Count;
		wdCharacter.vt = VT_I4;
		wdCharacter.lVal = 1;
		Count.vt = VT_I4;
		Count.lVal = 1;
		if (bBack) OLEMethod(DISPATCH_METHOD, NULL, pSelection, L"TypeBackspace", 0);
		else OLEMethod(DISPATCH_METHOD, NULL, pSelection, L"Delete", 2, Count, wdCharacter);
	}
	return m_hr;
}

HRESULT CMSWord::CheckSpelling(LPCTSTR szWord, BOOL &bResult) {
	if (!m_pWApp || !m_pActiveDocument) return E_FAIL;

	IDispatch* pDocApp;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, m_pActiveDocument, L"Application", 0);
		pDocApp = result.pdispVal;
	}

	{
		VARIANT result;
		VariantInit(&result);
		COleVariant varWord(szWord);
		OLEMethod(DISPATCH_METHOD, &result, pDocApp, L"CheckSpelling", 1, varWord.Detach());
		bResult = result.boolVal;
	}
	return m_hr;
}

HRESULT CMSWord::CheckGrammer(LPCTSTR szString, BOOL &bResult) {
	if (!m_pWApp || !m_pActiveDocument) return E_FAIL;

	IDispatch* pDocApp;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, m_pActiveDocument, L"Application", 0);
		pDocApp = result.pdispVal;
	}

	{
		VARIANT result;
		VariantInit(&result);
		COleVariant varString(szString);
		OLEMethod(DISPATCH_METHOD, &result, pDocApp, L"CheckGrammer", 1, varString.Detach());
		bResult = result.boolVal;
	}
	return m_hr;
}

HRESULT CMSWord::SetFont(LPCTSTR szFontName, int nSize, BOOL bBold, BOOL bItalic, COLORREF crColor) {
	if (!m_pWApp || !m_pActiveDocument) return E_FAIL;

	IDispatch* pDocApp;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, m_pActiveDocument, L"Application", 0);
		pDocApp = result.pdispVal;
	}
	IDispatch* pSelection;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pDocApp, L"Selection", 0);
		pSelection = result.pdispVal;
	}
	IDispatch* pFont;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pSelection, L"Font", 0);
		pFont = result.pdispVal;
	}

	{
		COleVariant oleName(szFontName);
		m_hr = OLEMethod(DISPATCH_PROPERTYPUT, NULL, pFont, L"Name", 1, oleName.Detach());
		VARIANT x;
		x.vt = VT_I4;
		x.lVal = nSize;
		m_hr = OLEMethod(DISPATCH_PROPERTYPUT, NULL, pFont, L"Size", 1, x);
		x.vt = VT_I4;
		x.lVal = crColor;
		m_hr = OLEMethod(DISPATCH_PROPERTYPUT, NULL, pFont, L"Color", 1, x);
		x.vt = VT_I4;
		x.lVal = bBold ? 1 : 0;
		m_hr = OLEMethod(DISPATCH_PROPERTYPUT, NULL, pFont, L"Bold", 1, x);
		x.vt = VT_I4;
		x.lVal = bItalic ? 1 : 0;
		m_hr = OLEMethod(DISPATCH_PROPERTYPUT, NULL, pFont, L"Italic", 1, x);
	}
	pFont->Release();
	pSelection->Release();
	pDocApp->Release();

	return m_hr;
}

HRESULT CMSWord::MoveCursor(int nDirection, BOOL bSelection) {
	if (!m_pWApp || !m_pActiveDocument) return E_FAIL;

	IDispatch* pDocApp;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, m_pActiveDocument, L"Application", 0);
		pDocApp = result.pdispVal;
	}
	IDispatch* pSelection;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pDocApp, L"Selection", 0);
		pSelection = result.pdispVal;
	}

	{
		VARIANT wdCharacter, wdExtend, Count;
		wdCharacter.vt = VT_I4;
		wdCharacter.lVal = -1;
		wdExtend.vt = VT_I4;
		wdExtend.lVal = bSelection ? 1 : 0;
		Count.vt = VT_I4;
		Count.lVal = 1;
		switch (nDirection)
		{
		case 1:
			OLEMethod(DISPATCH_METHOD, NULL, pSelection, L"MoveLeft", 3, wdExtend, Count, wdCharacter);
			break;
		case 2:
			OLEMethod(DISPATCH_METHOD, NULL, pSelection, L"MoveRight", 3, wdExtend, Count, wdCharacter);
			break;
		case 3:
			wdCharacter.lVal = 5;
			OLEMethod(DISPATCH_METHOD, NULL, pSelection, L"MoveUp", 3, wdExtend, Count, wdCharacter);
			break;
		case 4:
			OLEMethod(DISPATCH_METHOD, NULL, pSelection, L"MoveDown", 3, wdExtend, Count, wdCharacter);
			break;
		}
	}
	pDocApp->Release();
	pSelection->Release();

	return m_hr;
}

HRESULT CMSWord::AddComment(LPCTSTR szComment) {
	if (!m_pWApp || !m_pActiveDocument) return E_FAIL;

	IDispatch* pDocApp;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, m_pActiveDocument, L"Application", 0);
		pDocApp = result.pdispVal;
	}
	IDispatch* pSelection;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pDocApp, L"Selection", 0);
		pSelection = result.pdispVal;
	}
	IDispatch* pRange;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pSelection, L"Range", 0);
		pRange = result.pdispVal;
	}
	IDispatch* pComments;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, m_pActiveDocument, L"Application", 0);
		pComments = result.pdispVal;
	}
	{
		COleVariant sCmt(szComment);
		VARIANT result;
		VariantInit(&result);
		VARIANT varRange;
		varRange.vt = VT_DISPATCH;
		varRange.pdispVal = pRange;
		OLEMethod(DISPATCH_METHOD, &result, pComments, L"Add", 2, sCmt.Detach(), varRange);
	}
	pRange->Release();
	pComments->Release();
	pDocApp->Release();
	pSelection->Release();

	return m_hr;
}

HRESULT CMSWord::SetSelectionText(LPCTSTR szText) {
	if (m_pWApp == NULL) return E_FAIL;

	IDispatch* pDocApp;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, m_pActiveDocument, L"Application", 0);
		pDocApp = result.pdispVal;
	}
	IDispatch* pSelection;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pDocApp, L"Selection", 0);
		pSelection = result.pdispVal;
	}

	{
		COleVariant sText(szText);
		VARIANT text = sText.Detach();
		OLEMethod(DISPATCH_METHOD, NULL, pSelection, L"TypeText", 1, text);
	}
	pDocApp->Release();
	pSelection->Release();

	return m_hr;
}

HRESULT CMSWord::CloseActiveDocument(BOOL bSave) {
	if (m_pWApp == NULL) return E_FAIL;

	VARIANT result;
	VariantInit(&result);
	if (bSave) m_hr = OLEMethod(DISPATCH_METHOD, &result, m_pActiveDocument, L"Save", 0);
	m_hr = OLEMethod(DISPATCH_METHOD, &result, m_pActiveDocument, L"Close", 0);
	m_pActiveDocument = NULL;

	return m_hr;
}

HRESULT CMSWord::Quit() {
	if (m_pWApp == NULL) return E_FAIL;

	DISPID dispID;
	LPOLESTR ptName = _T("Quit");
	m_hr = m_pWApp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);

	if (SUCCEEDED(m_hr)) {
		DISPPARAMS dp = { NULL, NULL, 0, 0 };
		m_hr = m_pWApp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, NULL, NULL, NULL);
	}
	return m_hr;
}