/*	A little change: I would like to disable precompiled headers in
	this file, so it's possible to use this in other compilers.
	I could do this in the project settings, but then I has to
	do this in every project... So I search for a solution
	that always work. Check if PEK_TX_USEMFC is defined doesn't work.

  */
//#ifdef PEK_TX_USEMFC
	//#include <afx.h>
	#include "stdafx.h"
//#endif

#include "textfile.h"
#include <iostream>


CTextFileBase::CTextFileBase()
{
}

CTextFileBase::~CTextFileBase()
{
	Close();
}

//Convert char* to wstring
void CTextFileBase::CharToWstring(const char* from, wstring &to) const
{

	to = L"";

#ifdef _WINDOWS
	int length = strlen(from);

	//Use api convert routine
	wchar_t* wbuffer = new wchar_t[length+1];

	MultiByteToWideChar(	CP_ACP,
							0,
							from,
							length,
							wbuffer,
							length+1);
							
	wbuffer[length] = '\0';

	to = wbuffer;
	delete [] wbuffer;
#else
	//Create string, make sure it's large enough
	int count = (strlen(from) + 1)*sizeof(char);
	wchar_t* to2 = new wchar_t[ count ];

	mbstowcs(to2, from, count);

	//Copy to string...
	to = to2;

	//and clean up
	delete [] to2;
#endif

}

//Convert wchar_* to string
void CTextFileBase::WcharToString(const wchar_t* from, string &to) const
{
	to = "";

#ifdef _WINDOWS
	int length = wcslen(from);

	//Use mfc convert routine
	char* abuffer = new char[(length+1)*sizeof(wchar_t)];

	WideCharToMultiByte(	CP_ACP,
							0,
							from,
							length,
							abuffer,
							(length+1)*sizeof(wchar_t),
							0,
							0);
							
	abuffer[length] = '\0';

	to = abuffer;
	delete [] abuffer;
#else
	//Create string, make sure it's large enough
	int count = (wcslen(from) + 1)*sizeof(wchar_t);
	char* to2 = new char[ count ];

	wcstombs(to2, from, count);

	//Copy to string...
	to = to2;

	//and clean up
	delete [] to2;
#endif

}

//Return file encoding
CTextFileBase::TEXTENCODING CTextFileBase::GetEncoding() const 
{
	return m_encoding;
};

//Is file open?
int CTextFileBase::IsOpen() 
{
#ifndef PEK_TX_USEMFC
	return m_file.is_open();
#else
	return (m_file != NULL && m_file->m_hFile != CFile::hFileNull);
#endif

}

//Close file
void CTextFileBase::Close()
{
	if(IsOpen())
	{
#ifndef PEK_TX_USEMFC
		m_file.close();
#else
		if(m_closeAndDeleteFile)
		{
			m_file->Close();
		}		
#endif
	}
#ifdef PEK_TX_USEMFC
	if(m_closeAndDeleteFile)
	{
		delete m_file;
		m_file = NULL;
	}
#endif
}


//Create textfile
CTextFileWrite::CTextFileWrite(const FILENAMECHAR* filename, TEXTENCODING encoding)
{
#ifndef PEK_TX_USEMFC
	m_file.open(filename, ios::binary | ios::out );
#else
	m_file = new CFile;
	m_file->Open(filename, CFile::modeCreate | CFile::modeWrite);

	m_closeAndDeleteFile = true;
#endif

	m_buffpos = -1;
	m_buffsize = 0;

	m_encoding = encoding;
	
	WriteBOM();
}

#ifdef PEK_TX_USEMFC
//Create textfile from CFile object
CTextFileWrite::CTextFileWrite(CFile* file, TEXTENCODING encoding)
{
	ASSERT(file);

	m_file = file;
	m_closeAndDeleteFile = false;

	m_encoding = encoding;

	m_buffpos = -1;
	m_buffsize = 0;

	WriteBOM();
}
#endif

CTextFileWrite::~CTextFileWrite()
{
	Close();
}

void CTextFileWrite::WriteBOM()
{
	//Write BOM
	if(IsOpen())
	{
		if(m_encoding == UNI16_BE || m_encoding == UNI16_LE)
		{
			//Write BOM
			WriteWchar( 0xFEFF );
		}
		else if(m_encoding == UTF_8)
		{
			//Write UTF-8 BOM.  0xEF 0xBB 0xBF
			WriteByte(0xEF);
			WriteByte(0xBB);
			WriteByte(0xBF);
		}
	}
}

//Write one byte
void CTextFileWrite::WriteByte(const unsigned char byte)
{
	//Instead of writing, save data in buffer and write when buffer is full
	if(m_buffpos+1 >= BUFFSIZE)
		Flush();

	m_buffpos++;
	m_buf[m_buffpos] = byte;
}

//Write and empty buffer
void CTextFileWrite::Flush()
{
#ifdef PEK_TX_USEMFC
	m_file->Write(m_buf, m_buffpos+1);
#else
	m_file.write(m_buf, m_buffpos+1);
#endif
	m_buffpos = -1;
}

void CTextFileWrite::WriteWchar(const wchar_t ch)
{
	//Write HO byte first?
	if(m_encoding == UNI16_BE)
	{
		//Write HO byte
		WriteByte((unsigned char) (ch >> 8) );
		//Write LO byte
		WriteByte((unsigned char) ch);
	}
	else if(m_encoding == UNI16_LE)
	{
		//Write LO byte
		WriteByte((unsigned char) ch);
		//Write HO byte
		WriteByte((unsigned char) (ch >> 8) );
	}
	else
	{
		//http://www.cl.cam.ac.uk/~mgk25/unicode.html#examples
		//http://www.ietf.org/rfc/rfc3629.txt

		//Just a single byte?
		if(ch <= 0x7F)
		{
			//U-00000000 - U-0000007F:  0xxxxxxx  
			WriteByte( (unsigned char) ch );
		}

		//Two bytes?
		else if(ch <= 0x7FF)
		{
			//U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
			WriteByte( (unsigned char) (0xC0 | (ch>>6)) );
			WriteByte( (unsigned char) (0x80 | (ch&0x3F)) );
		}

		//Three bytes?
		else if(ch <= 0xFFFF)
		{
			//U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
			WriteByte( (unsigned char) (0xE0 | (  ch>>12)		));
			WriteByte( (unsigned char) (0x80 | ( (ch>>6)&0x3F ) ));
			WriteByte( (unsigned char) (0x80 | ( ch&0x3F )		));
		}

		/* //UPS! I did some coding for UTF-32, may be useful in the future :-)
		//Four bytes?
		else if(ch <= 0x1FFFFF)
		{
			//U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
			WriteByte( (unsigned char) (0xF0 | (  ch>>18)		 ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>12)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>6)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( ch&0xA0 )		));
		}
		
		//Five bytes bytes?
		else if(ch <= 0x3FFFFFF)
		{
			//U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
			WriteByte( (unsigned char) (0xF8 | (  ch>>24)	 	 ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>18)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>12)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>6)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( ch&0xA0 )		));
		}
		
		//Five bytes bytes?
		else if(ch <= 0x7FFFFFFF)
		{
			//U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
			WriteByte( (unsigned char) (0xFC | (  ch>>30)		 ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>24)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>18)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>12)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( (ch>>6)&0xA0 ) ));
			WriteByte( (unsigned char) (0xA0 | ( ch&0xA0 )		));
		}*/

	}
}

//Write new line
void CTextFileWrite::WriteEndl()
{
	if(m_encoding == ASCII)
	{
		WriteByte(0x0D);
		WriteByte(0x0A);
	}
	else 
	{
		WriteWchar(0x0D);
		WriteWchar(0x0A);
	}
}


//Write a c-string in ASCII. 
//In versions before 1.02 this function wrote directly to file,
//no buffer was used. But sense WriteEndl() used buffer the file was
//written incorretly. Now buffer is used here too, this is the
//fastest solution.
void CTextFileWrite::WriteAsciiString(const char* s)
{	
	while(*s != '\0')
	{
		WriteByte(*s);
		s++;
	}

	/*
	//The following used in 1.01. Bad idea, becuse WriteEndl() was buffered, so
	//all wraps where written at the end of file.
#ifndef PEK_TX_USEMFC
	m_file << s;
#else
	m_file->Write(s, strlen(s));
#endif
	*/
}


CTextFileWrite& CTextFileWrite::operator<< (const char* text)
{
	Write(text); 
	return *this;
};

CTextFileWrite& CTextFileWrite::operator<< (const wchar_t* text)
{
	Write(text); 
	return *this;
};

CTextFileWrite& CTextFileWrite::operator<< (const wchar_t wc)
{
	//Not the perfect code, but it's easy!
	wstring text;
	text=wc;
	Write(text.c_str());
	return *this;
};

CTextFileWrite& CTextFileWrite::operator<< (const char c)
{
	//Not the perfect code, but it's easy!
	string text;
	text=c;
	Write(text.c_str());
	return *this;
};

//Write char*
void CTextFileWrite::Write(const char* text)
{
	//ASCIItext file format?
	if(m_encoding == ASCII)
		WriteAsciiString(text);
	else
	{
		//Convert text to unicode
		wstring utext;
		CharToWstring(text, utext);

		//OK, lets write unicode
		for(wstring::const_iterator i=utext.begin();
			i < utext.end();
			i++)
		{
			WriteWchar(*i);
		}

	}
}

//Write wcar_t*
void CTextFileWrite::Write(const wchar_t* utext)
{
	//ASCII text file format?
	if(m_encoding == ASCII)
	{
		//Convert to string and write
		string text;
		WcharToString(utext, text);
		WriteAsciiString(text.c_str());
	}
	else
	{
		while(*utext != 0)
		{
			WriteWchar(*utext);
			utext++;
		}
	}
}

//Close the file
void CTextFileWrite::Close()
{
	if(IsOpen())
	{
		Flush();

#ifndef PEK_TX_USEMFC
		m_file.close();
#else
		if(m_closeAndDeleteFile)
			m_file->Close();
#endif

	}

#ifdef PEK_TX_USEMFC
	if(m_closeAndDeleteFile)
	{
		delete m_file;
		m_file = NULL;
	}
#endif

}

CTextFileRead::CTextFileRead(const FILENAMECHAR* filename)
{

#ifndef PEK_TX_USEMFC
	//If not Windows, do this
	m_file.open(filename, ios::binary | ios::in);
#else
	m_file = new CFile;
	//In windows, do this
	m_file->Open(filename, CFile::modeRead | CFile::shareDenyWrite );
	m_closeAndDeleteFile = true;
#endif

	m_endoffile = (IsOpen()==0);

	//Force reading to buffer next time
	m_buffpos=-1;

	m_useExtraBuffer=false;
	
	ReadBOM();
}

#ifdef PEK_TX_USEMFC
CTextFileRead::CTextFileRead(CFile* file)
{
	ASSERT(file);

	m_file = file;
	m_closeAndDeleteFile = false;

	m_endoffile = (IsOpen()==0);

	//Force reading to buffer next time
	m_buffpos=-1;

	m_useExtraBuffer=false;
	
	ReadBOM();
}
#endif

void CTextFileRead::ReadBOM()
{
	if( IsOpen() )
	{
		unsigned char bytes[2];

		//Read the first two bytes
		ReadByte(bytes[0]);
		ReadByte(bytes[1]);

		//Figure out what format the file is in
		if( bytes[0] == 0xFF && bytes[1] == 0xFE)
			m_encoding = UNI16_LE;
		else if( bytes[0] == 0xFE && bytes[1] == 0xFF)
			m_encoding = UNI16_BE;
		else if( bytes[0] == 0xEF && bytes[1] == 0xBB)
		{
			//This is probably UTF-8, check the third byte
			unsigned char temp;
			ReadByte(temp);
			if( temp == 0xBF)
				m_encoding = UTF_8;
			else
			{
				//Set text format.
				m_encoding = ASCII;
				ResetFilePointer();
			}
		}
		else
		{
			m_encoding = ASCII;

			//Set start pos
			ResetFilePointer();
		}
	}
}

//End of file?
bool CTextFileRead::Eof() const
{
	return m_endoffile;
}


//Read one byte
void CTextFileRead::ReadByte(unsigned char& ch)
{
	//Use extrabuffer if needed
	if(m_useExtraBuffer)
	{
		m_useExtraBuffer=false;
		ch = m_extraBuffer_char;
		return;
	}

//#ifndef PEK_TX_USEMFC
//	return (char) m_file.get();
//#else

	//In Windows, do this...

	//If buffer used or not read
	if(m_buffpos==-1 || m_buffpos == BUFFSIZE-1)
	{
#ifndef PEK_TX_USEMFC
		//TODO: Is there correct???
		m_buffsize = m_file.read(m_buf, BUFFSIZE).gcount();	
#else
		m_buffsize=m_file->Read(m_buf, BUFFSIZE);
#endif

		if(m_buffsize == 0)
		{
			m_endoffile=true;
			ch = 0;
			return;
		}

		m_buffpos=0;
	}
	else
	{
		m_buffpos++;

		if(m_buffpos >= m_buffsize)
		{
			m_endoffile=true;
			ch = 0;
			return;
		}
	}

//	TRACE("%c", buf[buffpos]);

	ch = m_buf[m_buffpos];
//#endif
}


void CTextFileRead::ReadWchar(wchar_t& ch)
{
	if(m_useExtraBuffer)
	{
		m_useExtraBuffer=false;
		ch = m_extraBuffer_wchar;
		return;
	}

	if(m_encoding == UTF_8)
	{	
		//This is quite tricky :-/
		//http://www.cl.cam.ac.uk/~mgk25/unicode.html#examples
		unsigned char byte;
		ReadByte(byte);

		int onesBeforeZero = 0;

		{	//Calc how many ones before the first zero...
			unsigned char temp = byte;

			while( (temp & 0x80)!=0 )
			{
				temp = (unsigned char) (temp << 1);
				onesBeforeZero++;
			}
		}

		if(onesBeforeZero==0)
		{
			ch = byte;
			return;
		}
		else if(onesBeforeZero == 2)
		{
			//U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
			unsigned char byteb;
			ReadByte(byteb);

			ch =				 ( ((0x1F & byte) << 6)| 
								    (0x3F & byteb)
								 ) ;

			return;
		}
		else if(onesBeforeZero == 3)
		{
			//U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
			unsigned char byteb, bytec;
			ReadByte(byteb);
			ReadByte(bytec);

			ch = ((0x0F & byte) << 12) |
				 ((0x3F & byteb) << 6) | 
				  (0x3F & bytec) ;

			return;
		}

		//This should never happend! It it do, something is wrong with the file.
		ch = 0xFFFD;

	}
	else
	{

		unsigned char bytes[2];
		ReadByte(bytes[0]);
		ReadByte(bytes[1]); 

		if(m_encoding == UNI16_BE)
			ch = (wchar_t) ( ((wchar_t) bytes[0] << 8) | 
							  (wchar_t) bytes[1] 
						   ) ;
		else
			ch = (wchar_t) ( ((wchar_t) bytes[1] << 8) | 
							  (wchar_t) bytes[0] 
							);
	}
}


void CTextFileRead::ResetFilePointer()
{
	m_useExtraBuffer=false;

#ifndef PEK_TX_USEMFC
	m_file.clear();
	m_file.seekg(0, ios::beg);
#else
	m_file->SeekToBegin();
#endif
	//Force reread buffer
	m_buffpos=-1;

}


bool CTextFileRead::ReadLine(string& line)
{
	//EOF?
	if(Eof())
		return false;

	if(m_encoding == ASCII)
	{
		return ReadCharLine(line);
	}
	else
	{
		wstring wline;

		if(!ReadWcharLine(wline))
			return false;
	
		//Convert
		WcharToString(wline.c_str(), line);

		return true;
	}

	return false;
}

bool CTextFileRead::ReadLine(wstring& line)
{
	//EOF?
	if(Eof())
		return false;

	if(m_encoding == ASCII)
	{
		string cline;

		if(!ReadCharLine(cline))
			return false;

		//Convert to wstring
		CharToWstring(cline.c_str(), line);

		return true;
	}
	else
	{
		return ReadWcharLine(line);
	}

	return false;
}


bool CTextFileRead::Read(string& all, const string newline)
{
	if(!IsOpen())
		return false;

	int buffsize = GuessCharacterCount()+2;
	int buffpos = 0;

	//Create buffer
	char* buffer = new char[buffsize];

	//If not possible, don't use any buffer
	if(buffer == NULL) 
		buffsize = 0;

	string temp;
	all = temp;
	all.reserve(buffsize);

	while(!Eof())
	{
		if(ReadLine(temp))
		{
			if(!Eof())
				temp.append(newline.c_str());

			//Add to buffer if possible
			if(buffpos + (int) temp.size() < buffsize)
			{
				strcpy(buffer+buffpos, temp.c_str());
				buffpos += (int) temp.size();
			}
			else
			{
				//Copy to all string
				if(buffpos != 0)
				{
					all.append(buffer, buffpos);
					buffpos = 0;
				}

				all += temp;
			}
		}
	};

	//Copy to all string
	if(buffpos != 0)
	{
		all.append(buffer, buffpos);
	}

	if(buffer != NULL)
		delete [] buffer;

	return true;
}

bool CTextFileRead::Read(wstring& all, const wstring newline)
{
	if(!IsOpen())
		return false;

	int buffsize = GuessCharacterCount()+2;
	int buffpos = 0;

	//Create buffer
	wchar_t* buffer = new wchar_t[buffsize];

	//If not possible, don't use any buffer
	if(buffer == NULL) 
		buffsize = 0;

	wstring temp;
	all = temp;
	all.reserve(buffsize);

	while(!Eof())
	{
		if(ReadLine(temp))
		{
			if(!Eof())
				temp.append(newline.c_str());

			//Add to buffer if possible
			if(buffpos + (int) temp.size() < buffsize)
			{
				wcscpy(buffer+buffpos, temp.c_str());
				buffpos += (int) temp.size();
			}
			else
			{
				//Copy to all string
				if(buffpos != 0)
				{
					all.append(buffer, buffpos);
					buffpos = 0;
				}

				all += temp;
			}
		}
	};

	//Copy to all string
	if(buffpos != 0)
	{
		all.append(buffer, buffpos);
	}

	if(buffer != NULL)
		delete [] buffer;

	/*
	wstring temp;
	all = temp;

	//Reserv memory (makes reading a lot faster)
	all.reserve(GuessCharacterCount());

	while(!Eof())
	{
		if(ReadLine(temp))
		{
			if(!Eof())
				all += temp;

			all += temp;
		}
	};*/

	return true;;
}

int CTextFileRead::GuessCharacterCount()
{
	#ifdef PEK_TX_USEMFC
	int bytecount = (int) m_file->GetLength();
	#else
	//Coded needed to get file size when using ifstream
	int bytecount = 1024*1024; //Default: 1 MB
	#endif

	//If ASCII, the number of characters is the byte count.
	//If UTF-8, it can't be more than bytecount, so use byte count
	if(m_encoding == ASCII || m_encoding == UTF_8)
		return bytecount;

	//Otherwise, every two byte in one character
	return bytecount/2;
}

#ifdef PEK_TX_USEMFC

bool CTextFileRead::ReadLine(CString& line)
{
#ifndef _UNICODE
	string temp;
#else
	wstring temp;
#endif

	if(!ReadLine(temp))
		return false;

	line = temp.c_str();
	return true;
}

bool CTextFileRead::Read(CString& all, const CString newline)
{
#ifndef _UNICODE
	string temp, n=newline;
#else
	wstring temp, n=newline;
#endif

	if(!Read(temp, n))
		return false;

	all = temp.c_str();
	return true;
}

#endif



bool CTextFileRead::ReadWcharLine(wstring& line)
{
	wchar_t ch;
	
	//Ignore 0x0D and 0x0A
	//or just 0x0D
	//or just 0x0A
	ReadWchar(ch);
	if(ch == 0x0D) //If next is 0x0A, ignore that too
	{
		ReadWchar(ch);

		if(ch == 0x0A)
			ReadWchar(ch);

	}else if(ch == 0x0A)
			ReadWchar(ch);

	//EOF?
	if(Eof())
		return false;

	//Clear
	line = L"";

	//It would be a lot easier if we didn't used a buffer, and added directly to
	//line, but that is to slow.
	wchar_t buffer[BUFFSIZE];
	buffer[BUFFSIZE-1] = '\0';
	//Where to insert next character
	int bufpos = 0;

	//Read line
	while(ch != 0x0D && ch != 0x0A && !Eof())
	{
		//End of buffer?
		if(bufpos+1 >= BUFFSIZE)
		{
			//Add to line
			line.append(buffer, bufpos);
			bufpos=0;
		}

		buffer[bufpos] = ch;
		bufpos++;

		ReadWchar(ch);
	};

	buffer[bufpos] = L'\0';
	line += buffer;
	
	//Save current character i extra buffer
	m_useExtraBuffer=true;
	m_extraBuffer_wchar=ch;

	return true;
}

//Read line to string
bool CTextFileRead::ReadCharLine(string& line)
{
	unsigned char ch;
	
	//Ignore 0x0D and 0x0A
	//or just 0x0D
	//or just 0x0A
	ReadByte(ch);
	if(ch == 0x0D) //If next is 0x0A, ignore that too
	{
		ReadByte(ch);

		if(ch == 0x0A)
			ReadByte(ch);

	}else if(ch == 0x0A)
			ReadByte(ch);

	//EOF?
	if(Eof())
		return false;

	//Clear
	line = "";

	//It would be a lot easier if we didn't used a buffer, and added directly to
	//line, but that is to slow.
	char buffer[BUFFSIZE];
	buffer[BUFFSIZE-1] = '\0';
	//Where to insert next character
	int bufpos = 0;

	//Read line
	while(ch != 0x0D && ch != 0x0A && !Eof())
	{
		//End of buffer?
		if(bufpos+1 >= BUFFSIZE)
		{
			//Add to line
			line.append(buffer, bufpos);
			bufpos=0;
		}

		buffer[bufpos] = ch;
		bufpos++;

		ReadByte(ch);
	};

	buffer[bufpos] = L'\0';
	line += buffer;
	
	//Save current character i extra buffer
	m_useExtraBuffer=true;
	m_extraBuffer_char=ch;

	return true;
}
