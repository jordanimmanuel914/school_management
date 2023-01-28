// SendDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FPDiag.h"
#include "SendDialog.h"
#include "TextFile.h"
#include "ReportPage.h"
#include "SubmitThread.h"
#include "afxdialogex.h"
#include "math.h"
#include "curl/curl.h"

extern void JoinString(const CStringArray& sItems, CString sSplit, CString& sText);
extern void SplitString(CString sText, CString sSplit, CStringArray& sItems);

BOOL SEmailData::DeleteDocument() {
	TCHAR szDestPath[512];
	TCHAR szDirectory[512];
	
	GetCurrentDirectory(512, szDirectory);
	
	wsprintf(szDestPath, _T("%s\\Data\\Reports\\%s"), szDirectory, sReportFileName);
	DeleteFile(szDestPath);
	return TRUE;


}

// SEmailData
BOOL SEmailData::SaveDocument() {
	TCHAR szTemplatePath[512];
	TCHAR szDestPath[512];

	TCHAR szDirectory[512];
	GetCurrentDirectory(512, szDirectory);

	int nTemplateIndex = ((int)bMathLiteracyScore << 3) | ((int)bLevel34Score << 2) | ((int)bCalculatorScore << 1) | (int)bNoLanguageScore;

	wsprintf(szTemplatePath, _T("%s\\Data\\Templates\\template%d.docx"), szDirectory, nTemplateIndex + 1);
	wsprintf(szDestPath, _T("%s\\Data\\Reports\\%s"), szDirectory, sReportFileName);

	DeleteFile(szDestPath);
	CopyFile(szTemplatePath, szDestPath, FALSE);

	

	m_document = new CMSWord;

	m_document->OpenDocument(szDestPath, FALSE);

	IDispatch* pApplication = 0;
	IDispatch* pSelection = 0;
	IDispatch* pFind = 0;
	
	pApplication = m_document->GetApplicationObject();
	if (pApplication) pSelection = m_document->GetSelectionObject(pApplication);
	if (pSelection) pFind = m_document->GetFindObject(pSelection);
	
	sAddressMessage.Replace(_T("@Student_Name"),sStudentName);
	sAddressMessage.Replace(_T("@Student_Surname"),sStudentSurname);
	sAddressMessage.Replace(_T("@School_Name"),sSchoolName);
	
		
	m_document->ReplaceString(pSelection, pFind, _T("#RECEIVER_NAME"), sGreeting);
	m_document->ReplaceString(pSelection, pFind, _T("#ADDRESS_MESSAGE"), sAddressMessage);
	m_document->ReplaceString(pSelection, pFind, _T("#SCHOOL_NAME"), sSchoolName);
	m_document->ReplacePicture(pSelection, pFind, _T("#SCHOOL_LOGO"), sSchoolLogoPath);
	m_document->ReplaceString(pSelection, pFind, _T("#STUDENT_NAME"), sStudentName);

	WCHAR szGeneralScore[5];
	wsprintf(szGeneralScore, _T("%d"), nGeneralScore);
	m_document->ReplaceString(pSelection, pFind, _T("#SCORE_TO_A_STRAIGHT"), szGeneralScore);
	m_document->ReplaceString(pSelection, pFind, _T("#STUDENT_NAME"), sStudentName);

	if (bNoLanguageScore) {
		WCHAR szScore[5];
		wsprintf(szScore, _T("%d"), nNoLanguageScore);
		m_document->ReplaceString(pSelection, pFind, _T("#ALL_SUMS_1"), szScore);
		m_document->ReplaceString(pSelection, pFind, _T("#STUDENT_NAME"), sStudentName);	
		m_document->ReplaceString(pSelection, pFind, _T("#STUDENT_NAME"), sStudentName);
	}

	if (bCalculatorScore) {
		WCHAR szScore[5];
		wsprintf(szScore, _T("%d"), nCalculatorScore);
		m_document->ReplaceString(pSelection, pFind, _T("#ALL_SUMS_2"), szScore);
		m_document->ReplaceString(pSelection, pFind, _T("#STUDENT_NAME"), sStudentName);	
		m_document->ReplaceString(pSelection, pFind, _T("#STUDENT_NAME"), sStudentName);
	}

	if (bLevel34Score) {
		WCHAR szScore[5];
		wsprintf(szScore, _T("%d"), nLevel34Score);
		m_document->ReplaceString(pSelection, pFind, _T("#ALL_SUMS_3"), szScore);
		m_document->ReplaceString(pSelection, pFind, _T("#STUDENT_NAME"), sStudentName);	
		m_document->ReplaceString(pSelection, pFind, _T("#STUDENT_NAME"), sStudentName);
	}

	if (bMathLiteracyScore) {
		WCHAR szScore1[5], szScore2[5];
		wsprintf(szScore1, _T("%d"), nCoreMathScore);
		wsprintf(szScore2, _T("%d"), nMathLiteracyScore);

		m_document->ReplaceString(pSelection, pFind, _T("#4"), szScore1);
		m_document->ReplaceString(pSelection, pFind, _T("#5"), szScore2);
	}

	m_document->ReplaceString(pSelection, pFind, _T("#STUDENT_NAME"), sStudentName);
	m_document->ReplaceTable(pSelection, pFind, _T("#TABLE"), sQuestionNameArray, nScoreArray);
	m_document->ReplaceChart(pSelection, pFind, _T("#CHART"), sQuestionNameArray, nScoreArray);
	//Sleep(100);
	m_document->CloseDocument(1);

	delete m_document, m_document = NULL;
	return TRUE;
}



static const char *headers_txt[] = {
  "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n",
  "To: TEST\r\n",
  "From: TEST\r\n",
  "Subject: TEST TEXT send!\r\n",
  "\r\n", /* empty line to divide headers from body, see RFC5322 */
  "This is text email!\r\n",
  NULL
};

 
static const char inline_text[] =
  "This is the inline text message of the email.\r\n"
  "\r\n"
  "  It could be a lot of lines that would be displayed in an email\r\n"
  "viewer that is not able to handle HTML.\r\n";
 
static const char inline_html[] =
  "<html><body>\r\n"
  "<p>This is the inline <b>HTML</b> message of the email.</p>"
  "<br />\r\n"
  "<p>It could be a lot of HTML data that would be displayed by "
  "email viewers able to handle HTML.</p>"
  "</body></html>\r\n"; 
 struct upload_status {
  int lines_read;
};


 static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  data = headers_txt[upload_ctx->lines_read];

  if(data) {
    size_t len = strlen(data);
    memcpy(ptr, data, len);
    upload_ctx->lines_read++;

    return len;
  }

  return 0;
}

BOOL SEmailData::SendEmail(const CString& myaddr,const CString& APPPASS, const CString& PEM) {
	TCHAR szPath[512];

	TCHAR szDirectory[512];
	GetCurrentDirectory(512, szDirectory);
	wsprintf(szPath, _T("%s\\Data\\Reports\\%s"), szDirectory, sReportFileName);
	CString repaddr, FILENAME;
	
	// Send Email 
	CString sEmailAddress = this->sEmailAddress1;
	if(!this->sEmailAddress2.IsEmpty()){
		//sEmailAddress = this->sEmailAddress2;

	}
	FILENAME = szPath;
	repaddr = sEmailAddress;

	if(APPPASS.GetLength() != 16) {
		//MessageBox(_T(L"Google App password length is 16"), _T(L"Error"), MB_OK);
		
		return FALSE;
		
	}
	if(repaddr.GetLength() ==0) {
		//MessageBox(L"Please input Recipient mail address.", L"Error", MB_OK);
		return FALSE;
	}
	if(myaddr.GetLength() ==0) {
		//MessageBox(L"Please input From mail address.", L"Error", MB_OK);
		return FALSE;
	}
	if(PEM.GetLength() ==0) {
		//MessageBox(L"Please input peer license PEM file path.", L"Error", MB_OK);
		return FALSE;
	}
	char fromaddr[260] = {0};
	char toaddr[260]={0};
	char pathaddr[260]={0};
	char pempath[260]={0};
	char password[20]={0};
	CString ext;
	char mimtype[100] = "text/plain";
	if(FILENAME.GetLength()) {
		FILENAME.ReverseFind('.');
		ext = FILENAME.Mid(FILENAME.ReverseFind('.') + 1);
		ext.MakeUpper();
		if(ext=="DOC")
			strcpy_s(mimtype, "application/msword");
		if(ext == "DOCX")
			strcpy_s(mimtype, "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
	}

	WideCharToMultiByte(CP_ACP, 0, myaddr, myaddr.GetLength(), fromaddr, 260, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, repaddr, repaddr.GetLength(), toaddr, 260, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FILENAME, FILENAME.GetLength(), pathaddr, 260, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, APPPASS, APPPASS.GetLength(), password, 20, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, PEM, PEM.GetLength(), pempath, 260, 0, 0);
	CURL *curl;
  CURLcode res = CURLE_OK;
	struct upload_status upload_ctx;

  upload_ctx.lines_read = 0;
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *headers = NULL;
    struct curl_slist *recipients = NULL;
    struct curl_slist *slist = NULL;
    curl_mime *mime=NULL;
    curl_mime *alt;
    curl_mimepart *part;
    const char **cpp;
 
    /* This is the URL for your mailserver */
    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
 
    /* Note that this option is not strictly required, omitting it will result
     * in libcurl sending the MAIL FROM command with empty sender data. All
     * autoresponses should have an empty reverse-path, and should be directed
     * to the address in the reverse-path which triggered them. Otherwise,
     * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
     * details.
     */
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, fromaddr);
	/* Add two recipients, in this particular case they correspond to the
     * To: and Cc: addressees in the header, but they could be any kind of
     * recipient. */
	recipients = curl_slist_append(recipients, toaddr);
	curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
	
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

	curl_easy_setopt(curl, CURLOPT_USERNAME, fromaddr);
	curl_easy_setopt(curl, CURLOPT_PASSWORD, password); //gernerated by google app key
		curl_easy_setopt(curl, CURLOPT_CAINFO, pempath); //this key file is generated from https://curl.haxx.se/docs/sslcerts.html
		curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
	if(FILENAME.GetLength()) {
		mime = curl_mime_init(curl);		
    
		/* Build and set the message header list. */
		for(cpp = headers_txt; *cpp; cpp++)
			headers = curl_slist_append(headers, *cpp);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		/* Build the mime message. */
 
		/* The inline part is an alternative proposing the html and the text
		   versions of the email. */
		alt = curl_mime_init(curl);
	 
		/* HTML message. */
		part = curl_mime_addpart(alt);
		curl_mime_data(part, inline_html, CURL_ZERO_TERMINATED);
		curl_mime_type(part, mimtype);
 
		/* Text message. */
		part = curl_mime_addpart(alt);
		curl_mime_data(part, inline_text, CURL_ZERO_TERMINATED);
	
		/* Create the inline part. */
		part = curl_mime_addpart(mime);
		curl_mime_subparts(part, alt);
		if(ext=="DOC") {
			curl_mime_type(part, "multipart/attachment");
			slist = curl_slist_append(NULL, "Content-Disposition: attachment");
		}
		else {
			curl_mime_type(part, "multipart/alternative");
			slist = curl_slist_append(NULL, "Content-Disposition: inline");
		}
		curl_mime_headers(part, slist, 1);
	
		/* Add the current source program as an attachment. */
		part = curl_mime_addpart(mime);
		curl_mime_filedata(part, pathaddr);
		curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
	}
	else {
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	}
	
    /* Send the message */
    res = curl_easy_perform(curl);
 
    /* Check for errors */
    if(res != CURLE_OK) {
		//MessageBox(L"Send Error", L"Error", MB_ICONWARNING);
		return FALSE;
	}
    /* Free lists. */
    curl_slist_free_all(recipients);
    curl_slist_free_all(headers);
 
    /* curl will not send the QUIT command until you call cleanup, so you
     * should be able to re-use this connection for additional messages
     * (setting CURLOPT_MAIL_FROM and CURLOPT_MAIL_RCPT as required, and
     * calling curl_easy_perform() again. It may not be a good idea to keep
     * the connection open for a very long time though (more than a few
     * minutes may result in the server timing out the connection), and you do
     * want to clean up in the end.
     */
    curl_easy_cleanup(curl);
	if(FILENAME.GetLength())
    /* Free multipart message. */
		curl_mime_free(mime);
	//MessageBox(L"Sent",L"Information",MB_OK);
  }

	// Code for send email (attachment file path: szPath)
	return TRUE;
}

// CSendDialog dialog

IMPLEMENT_DYNAMIC(CSendDialog, CDialogEx)

CSendDialog::CSendDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSendDialog::IDD, pParent)
	, m_sStatus(_T(""))
	, PEMString(_T("cacert.pem"))
	, YourEmailString(_T(""))
	, PasswordString(_T(""))
{
	m_bSendEmail = FALSE;
	m_pThread = NULL;

	m_bCancel = FALSE;
	m_bDone = FALSE;
}

CSendDialog::~CSendDialog()
{
}

void CSendDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressBar);
	DDX_Text(pDX, IDC_STATUS_LABEL, m_sStatus);
	DDX_Text(pDX, IDC_EDIT_PEM, PEMString);

	DDX_Text(pDX, IDC_EDIT_PEM, PEMString);
	DDX_Text(pDX, IDC_EDIT_YOUREMAIL, YourEmailString);
	DDX_Text(pDX, IDC_EDIT_APPPASS, PasswordString);
}


BEGIN_MESSAGE_MAP(CSendDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SUBMIT_BUTTON, &CSendDialog::OnBnClickedSubmitButton)
	ON_BN_CLICKED(IDC_CANCEL_BUTTON, &CSendDialog::OnBnClickedCancelButton)
	ON_MESSAGE(WM_PROGRESS, &CSendDialog::OnProgress)
	ON_MESSAGE(WM_FINISHED, &CSendDialog::OnFinished)
END_MESSAGE_MAP()


// CSendDialog message handlers


void CSendDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bDone) {
		DWORD dwStatus;
		if (m_pThread != NULL) {
			VERIFY(::GetExitCodeThread(m_pThread->m_hThread, &dwStatus));
			if (dwStatus == STILL_ACTIVE) {
				m_pThread = NULL;
				Sleep(100);
			} else {
				delete m_pThread, m_pThread = NULL;
			}
		} 
	} else {
		m_pThread = NULL;
	}
	CDialogEx::OnClose();
}


BOOL CSendDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!m_bSendEmail) {
		((CButton*)GetDlgItem(IDC_SUBMIT_BUTTON))->SetWindowText(_T("Generate"));
	}

	return TRUE;
}


void CSendDialog::OnBnClickedSubmitButton()
{
	UpdateData(TRUE);
	if (!m_pThread) {
		m_pThread = (CSubmitThread*)AfxBeginThread(RUNTIME_CLASS(CSubmitThread), THREAD_PRIORITY_NORMAL,
			0, CREATE_SUSPENDED);
		m_pThread->SetOwner(this);
		m_pThread->ResumeThread();
	}
	((CButton*)GetDlgItem(IDC_SUBMIT_BUTTON))->EnableWindow(FALSE);
}


void CSendDialog::OnBnClickedCancelButton()
{
	if (m_pThread) m_bCancel = TRUE;
	else CDialogEx::OnCancel();
}

void CSendDialog::ClearAll() {
	for (int i = 0; i < m_emailDataArray.GetCount(); i++) {
		SEmailData* pEmailData = m_emailDataArray.GetAt(i);
		delete pEmailData, pEmailData = NULL;
	}
	m_emailDataArray.RemoveAll();
}

void CSendDialog::GenerateEmailData(CTextFileRead* pFile, SReport* pReport) {
	ClearAll();

	BOOL bFirst = TRUE;
	while (!pFile->Eof()) {
		CString sLine;
		pFile->ReadLine(sLine);

		if(bFirst){
			bFirst = FALSE;
			continue;
		}

		CStringArray sItems;
		SplitString(sLine, _T(","), sItems);

		int nItemCount = sItems.GetCount();
		if (nItemCount >= 6 + pReport->mQuestionArray.GetCount()) {
			CString sLearnerName = sItems[0];
			CString sLearnerSurName = sItems[1];
			CString sParentName = sItems[2];
			CString sParentSurName = sItems[3];
			
			SEmailData* emailData = new SEmailData;
			emailData->sEmailAddress1 = sItems[4];
			emailData->sEmailAddress2 = sItems[5];
			emailData->sSchoolName = pReport->mSchoolName;
			emailData->sSchoolLogoPath = pReport->mSchoolLogo;
			emailData->sReportDate = pReport->mTime;
			
			// Get Greetings
			BOOL bLearnerNameAndSurName = FALSE;
			BOOL bParentNameAndSurName = FALSE;
			BOOL bBothNames = FALSE;

			for (int i = 0; i < pReport->mReportToArray.GetCount(); i++) {
				CString sReportTo = pReport->mReportToArray.GetAt(i);
				if (sReportTo == _T("Learner Name and Surname")) bLearnerNameAndSurName = TRUE;
				if (sReportTo == _T("Parent Name and Surname")) bParentNameAndSurName = TRUE;
				if (sReportTo == _T("Learner and Parent names")) bBothNames = TRUE;
			}

			emailData->sGreeting = _T("");
			if (bLearnerNameAndSurName) {
				if (bParentNameAndSurName) {
					emailData->sGreeting = sLearnerName + _T("  ") + sLearnerSurName + _T("  and  ") + sParentName + _T("  ") + sParentSurName;
				} else {
					emailData->sGreeting = sLearnerName + _T("  ") + sParentName;
				}
			} else {
				if (bParentNameAndSurName) emailData->sGreeting = sParentName + _T("  ") + sParentSurName;
				else if (bBothNames) emailData->sGreeting = sLearnerName + _T("  and  ") + sParentName;
			}
			emailData->sStudentName = sLearnerName;
			emailData->sAddressMessage = pReport->mMessage;
			emailData->sStudentSurname = sLearnerSurName;
			


			int nTotalScore1 = 0, nTotalScore2 = 0;
			int nNoLanguageScore1 = 0, nNoLanguageScore2 = 0;
			int nCalculatorScore1 = 0, nCalculatorScore2 = 0;
			int nLevel34Score1 = 0, nLevel34Score2 = 0;
			int nCoreMathScore1 = 0, nCoreMathScore2 = 0;
			int nMathLiteracyScore1 = 0, nMathLiteracyScore2 = 0;

			for (int i = 6; i < 6 + pReport->mQuestionArray.GetCount(); i++) {
				SQuestion* pQuestion = pReport->mQuestionArray.GetAt(i - 6);

				int nScore1 = pQuestion->mScore;
				int nScore2 = _wtoi(sItems[i]);

				

				nTotalScore1 += nScore1;
				nTotalScore2 += nScore2;

				if (pQuestion->mTag.Find(_T("Language")) < 0) {
					nNoLanguageScore1 += nScore1;
					nNoLanguageScore2 += nScore2;
					emailData->bNoLanguageScore = TRUE;
				}

				if (pQuestion->mTag.Find(_T("Calculator")) >= 0) {
					nCalculatorScore1 += nScore1;
					nCalculatorScore2 += nScore2;
					emailData->bCalculatorScore = TRUE;
				}

				if (pQuestion->mTag.Find(_T("Level3")) >= 0) {
					nLevel34Score1 += nScore1;
					nLevel34Score2 += nScore2;
					emailData->bLevel34Score = TRUE;
				}
				if (pQuestion->mTag.Find(_T("Level4")) >= 0) {
					nLevel34Score1 += nScore1;
					nLevel34Score2 += nScore2;
					emailData->bLevel34Score = TRUE;
				}

				if (pQuestion->mTag.Find(_T("Core Mathematics")) >= 0) {
					nCoreMathScore1 += nScore1;
					nCoreMathScore2 += nScore2;
					emailData->bCoreMathScore = TRUE;
				}

				if (pQuestion->mTag.Find(_T("Mathematics Literacy")) >= 0) {
					nMathLiteracyScore1 += nScore1;
					nMathLiteracyScore2 += nScore2;
					emailData->bMathLiteracyScore = TRUE;
				}

				emailData->sQuestionNameArray.Add(pQuestion->mTopic);

				int nPercent = 0;
				if (nScore1 != 0) {
					nPercent = (int)(((double)nScore2 / (double)nScore1) * 100);
					if (nPercent > 100) nPercent = 100;
				}
				emailData->nScoreArray.Add(nPercent);
			}

			if (nTotalScore1 != 0) {
				emailData->nGeneralScore = (int)(((double)nTotalScore2 / (double)nTotalScore1) * 100);
				if (emailData->nGeneralScore > 100) emailData->nGeneralScore = 100;
			}
			if (nNoLanguageScore1 != 0) {
				emailData->nNoLanguageScore = (int)(((double)nNoLanguageScore2 / (double)nNoLanguageScore1) * 100);
				if (emailData->nNoLanguageScore > 100) emailData->nNoLanguageScore = 100;
			}
			if (nCalculatorScore1 != 0) {
				emailData->nCalculatorScore = (int)(((double)nCalculatorScore2 / (double)nCalculatorScore1) * 100);
				if (emailData->nCalculatorScore > 100) emailData->nCalculatorScore = 100;
			}
			if (nLevel34Score1 != 0) {
				emailData->nLevel34Score = (int)(((double)nLevel34Score2 / (double)nLevel34Score1) * 100);
				if (emailData->nLevel34Score > 100) emailData->nLevel34Score = 100;
			}
			if (nCoreMathScore1 != 0) {
				emailData->nCoreMathScore = (int)(((double)nCoreMathScore2 / (double)nNoLanguageScore1) * 100);
				if (emailData->nCoreMathScore > 100) emailData->nCoreMathScore = 100;
			}
			if (nMathLiteracyScore1 != 0) {
				emailData->nMathLiteracyScore = (int)(((double)nMathLiteracyScore2 / (double)nMathLiteracyScore1) * 100);
				if (emailData->nMathLiteracyScore > 100) emailData->nMathLiteracyScore = 100;
			}
			m_emailDataArray.Add(emailData);
		}
	}
}

void CSendDialog::Execute() {
	int nCount = m_emailDataArray.GetCount();
	m_progressBar.SetRange(0, nCount);

	for (int i = 0; i < nCount; i++) {
		if (m_bCancel) break;

		this->PostMessage(WM_PROGRESS, i, 0);
		
		SEmailData* pEmailData = m_emailDataArray.GetAt(i);

		WCHAR szFileName[100];
		wsprintf(szFileName, _T("%s_%d_%d.docx"), pEmailData->sReportDate, m_nReportIdx + 1, i + 1);
		pEmailData->sReportFileName = szFileName;

		pEmailData->SaveDocument(); // Function for generating docx
		if (!m_bCancel && m_bSendEmail) 
			{
				//AfxMessageBox(YourEmailString);
				pEmailData->SendEmail(YourEmailString, PasswordString, PEMString); // Send email
				
		}

		//pEmailData->DeleteDocument();

		Sleep(10);
	}
	m_bDone = TRUE;
}

HRESULT CSendDialog::OnProgress(WPARAM wParam, LPARAM lParam) {
	int nIndex = (int)wParam;
	int nCount = m_emailDataArray.GetCount();

	if (m_bSendEmail) {
		WCHAR szStatus[100];
		wsprintf(szStatus, _T("Submitting email... (%d/%d)"), nIndex + 1, nCount);
		m_sStatus = szStatus;
	} else {
		WCHAR szStatus[100];
		wsprintf(szStatus, _T("Generating report... (%d/%d)"), nIndex + 1, nCount);
		m_sStatus = szStatus;
	}
	UpdateData(FALSE);

	m_progressBar.SetPos(nIndex);

	return S_OK;
}

void CSendDialog::SetSendEmail(BOOL bSendEmail) {
	m_bSendEmail = bSendEmail;

	if (bSendEmail) m_sStatus = _T("If you want to send report to parents, please click submit button.");
	else m_sStatus = _T("If you want to make documents for reporting, please click generate button.");
}

HRESULT CSendDialog::OnFinished(WPARAM wParam, LPARAM lParam) {
	this->PostMessage(WM_CLOSE, 0, 0);
	return S_OK;
}