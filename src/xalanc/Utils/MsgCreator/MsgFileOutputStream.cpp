/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MsgFileOutputStream.hpp"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include <cassert>
#include <cerrno>
#include <cstdio>


static  XalanFileOutputStream::HandleType
openFile(const char*	theFileName)
{
	typedef XalanFileOutputStream::HandleType	HandleType;

#if defined(WIN32)

	
			return CreateFile(
					theFileName,
					GENERIC_WRITE,
					0,
					0,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					0);
				
				
#else

			return fopen(theFileName, "wb");

#endif
}



XalanFileOutputStream::XalanFileOutputStream(
			const char*	theFileName) :
			m_fileName(theFileName),
			m_handle(openFile(theFileName))
{
#if defined(WIN32)
    if (m_handle == INVALID_HANDLE_VALUE)
#else
    if (m_handle == 0)
#endif
	{
		throw XalanFileOutputStreamOpenException(
					theFileName,
					errno);
	}
}



XalanFileOutputStream::~XalanFileOutputStream()
{
#if defined(WIN32)
    if (m_handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_handle);
	}
#else
    if (m_handle != 0)
	{
		fclose(m_handle);
	}
#endif
}



void
XalanFileOutputStream::doFlush()
{
#if !defined(WIN32)
	if (fflush(m_handle) != 0)
	{
		throw XalanFileOutputStreamWriteException(
			m_fileName,
			errno);
	}
#endif
}



void
XalanFileOutputStream::writeData(
			const char*		theBuffer,
			unsigned int		theBufferLength)
{
#if defined(WIN32)
	DWORD			theBytesWritten;

	if (WriteFile(m_handle, theBuffer, DWORD(theBufferLength), &theBytesWritten, 0) == false ||
	    theBytesWritten != theBufferLength)
	{
		throw XalanFileOutputStreamWriteException(
			m_fileName,
			GetLastError());
	}
#else
	const size_t	theBytesWritten =
		fwrite(theBuffer,
			   1,
			   theBufferLength,
			   m_handle);

	if (theBytesWritten != theBufferLength)
	{
		throw XalanFileOutputStreamWriteException(
			m_fileName,
			errno);
	}
#endif
}



static char*
FormatMessageLocal(
			const char*				theMessage,
			const char*				theFileName,
			int						theErrorCode)
{
	assert( theMessage != 0 );

	assert( theFileName != 0);

	char*		strtoBuild = new char[256];
	strtoBuild[0] = '\0';

	strncat(strtoBuild,theMessage, 100);
	strtoBuild[100] = 0; // in case that lenght(theMessage) > 100 

	strcat(strtoBuild,".  The C++ run-time error code (errno) is ");

	char szNumb[20];
	szNumb[0] = 0;
	sprintf(szNumb,"%d",theErrorCode);

	strncat(strtoBuild,szNumb,18);

	strcat(strtoBuild,".");

	return strtoBuild;
}



XalanFileOutputStream::XalanFileOutputStreamOpenException::XalanFileOutputStreamOpenException(
		const char*		theFileName,
		int					theErrorCode) :
		m_pMessage( FormatMessageLocal(
				"Error opening file: ",
				theFileName,
				theErrorCode))
			
{
}



XalanFileOutputStream::XalanFileOutputStreamOpenException::~XalanFileOutputStreamOpenException()
{
	delete [] m_pMessage;
}



XalanFileOutputStream::XalanFileOutputStreamWriteException::XalanFileOutputStreamWriteException(
		const char*		theFileName,
		int				theErrorCode) :
		m_pMessage(FormatMessageLocal(
					"Error writing file: ",
					theFileName,
					theErrorCode))
{
}



XalanFileOutputStream::XalanFileOutputStreamWriteException::~XalanFileOutputStreamWriteException()
{
	delete [] m_pMessage;
}


void 	XalanFileOutputStream::write(const UTF16Ch*	theString, unsigned int		theLength)
{
	assert ( theString != 0 );
	writeData((const char*)theString,theLength * sizeof(UTF16Ch) );
}

void 	XalanFileOutputStream::write(const char*	theString, unsigned int		theLength)
{
	assert ( theString != 0 );

	UTF16Ch* theUTFString = XMLString::transcode(theString);

	write(theUTFString,theLength);

	XMLString::release(&theUTFString);
}

void XalanFileOutputStream::writeAsASCII(const UTF16Ch*	theString, unsigned int		theLengts)
{
	char* szString = XMLString::transcode(theString);
	writeData( szString, theLengts );
	XMLString::release(&szString);

}

void XalanFileOutputStream::writeAsASCII(const char*	theString, unsigned int		theLengts)
{
	writeData( theString, theLengts );
}

const UTF16Ch	s_UTF16ByteOrderMark[] =
{
	UTF16Ch(0xFEFF),
	UTF16Ch(0)
};

void 	XalanFileOutputStream::writeUTFprefix()
{
	write(s_UTF16ByteOrderMark,1);
}
