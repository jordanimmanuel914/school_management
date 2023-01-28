// textfile.h: interface for the textfile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef PEKSPRODUCTIONS_TEXTFILE
#define PEKSPRODUCTIONS_TEXTFILE



/*
	CTextFileDocument let you write and read text files with 
	different encodings (ASCII, UTF-8, Unicode 16 little/big 
	endian is supported.

	Let me now if you find something strange or just gets
	a clever idea...

	Get the latest version at 
	http://www.codeproject.com/file/textfiledocument.asp

	Version 1.12 - 17 Oct 2004
	 + Minor memory leak when open file failed, fixed.

	Version 1.11 - 28 Aug 2004
	 ! Calling WriteEndl() when writing an ASCII file could make
	   the file incorrectly written. Fixed.
	 + ASCII files is written faster.

	Version 1.10 - 13 Aug 2004
	Sorry about the quick update.
	 + Improved performance (much faster now, but code is more complicated :-/).
	 + Buffer is used when writing files.
	 + Buffer is used in non-mfc compilers

	Version 1.0	- 12 Aug 2004
	Initial version.

	PEK
  */


#ifdef _MFC_VER
	/*
		In windows it's possible to use unicode in filenames,
		in unix it's not possible (afaik). FILENAMECHAR is the 
		charactertype.
	  */

	#define	PEK_TX_USEMFC
	#include <afx.h>

	#ifndef _UNICODE
		typedef char FILENAMECHAR;
	#else
		typedef wchar_t FILENAMECHAR;
	#endif
#else
	#define USEFSTREAM
	typedef char FILENAMECHAR;
#endif


#include <fstream>
#include <string>
using namespace std;

class CTextFileBase
{
public:
	enum TEXTENCODING { ASCII, UNI16_BE, UNI16_LE, UTF_8 };

	CTextFileBase();
	~CTextFileBase();

	//Is the file open?
	int IsOpen();
	
	//Close the file
	virtual void Close();

	//Return the encoding of the file (ASCII, UNI16_BE, UNI16_LE or UTF_8);
	TEXTENCODING GetEncoding() const;
		

protected:
	//Convert char* to wstring
	void CharToWstring(const char* from, wstring &to) const;
	//Convert wchar_t* to string
	void WcharToString(const wchar_t* from, string &to) const;
		
	//The enocoding of the file
	TEXTENCODING m_encoding;

	//Buffersize
	#define BUFFSIZE 1024


#ifndef PEK_TX_USEMFC
	//Use fstream
	fstream m_file;
#else
	//In windows we are using CFile
	CFile* m_file;
	bool m_closeAndDeleteFile;
#endif
	
	//These controls the buffer for reading/writing

	//True if end of file
	bool m_endoffile;
	//Readingbuffer
	char m_buf[BUFFSIZE];
	//Bufferposition
	int m_buffpos;
	//Size of buffer
	int m_buffsize;

	

};

class CTextFileWrite : public CTextFileBase
{

public:
	CTextFileWrite(const FILENAMECHAR* filename, TEXTENCODING type=ASCII);
#ifdef PEK_TX_USEMFC
	CTextFileWrite(CFile* file, TEXTENCODING type=ASCII);
#endif
	~CTextFileWrite();

	//Write routines
	void Write(const char* text);
	void Write(const wchar_t* text);
	
	CTextFileWrite& operator << (const char wc);
	CTextFileWrite& operator << (const char* text);

	CTextFileWrite& operator << (const wchar_t wc);
	CTextFileWrite& operator << (const wchar_t* text);
	
	//Write new line (two characters, 13 and 10)
	void WriteEndl();

	//Close the file
	virtual void Close();

private:
	//Write and empty buffer
	void Flush();

	//Write a single one wchar_t, convert first
	void WriteWchar(const wchar_t ch);

	//Write one byte
	void WriteByte(const unsigned char byte);

	//Write a c-string in ASCII-format
	void WriteAsciiString(const char* s);

	//Write byte order mark
	void WriteBOM();
};


class CTextFileRead : public CTextFileBase
{

public:
	CTextFileRead(const FILENAMECHAR* filename);
#ifdef PEK_TX_USEMFC
	CTextFileRead(CFile* file);
#endif

	//Reading functions. Returns false if eof.
	bool ReadLine(string& line);
	bool ReadLine(wstring& line);

	bool Read(string& all, const string newline="\r\n");
	bool Read(wstring& all, const wstring newline=L"\r\n");

#ifdef PEK_TX_USEMFC
	bool ReadLine(CString& line);
	bool Read(CString& all, const CString newline=_T("\r\n"));
#endif

	//End of file?
	bool Eof() const;

private:
	//Guess the number of characters in the file
	int GuessCharacterCount();

	//Read line to wstring
	bool ReadWcharLine(wstring& line);

	//Read line to string
	bool ReadCharLine(string& line);

	//Reset the filepointer to start
	void ResetFilePointer();

	//Read one wchar_t
	void ReadWchar(wchar_t& ch);

	//Read one byte
	void ReadByte(unsigned char& ch);

	//Detect encoding
	void ReadBOM();

	//Use extra buffer. Sometimes we read one character to much, save it.
	bool m_useExtraBuffer;

	//Extra buffer. It's ok to share the memory
	union
	{
		char m_extraBuffer_char;
		wchar_t m_extraBuffer_wchar;
	};

};

#endif //PEKSPRODUCTIONS_TEXTFILE