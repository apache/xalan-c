/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#include "MsgFileOutputStream.hpp"

#include <cerrno>

#include <xercesc/util/PlatformUtils.hpp>

#include <xercesc/util/XMLString.hpp>

#include <cassert>

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

	assert(size_t(DWORD(theBufferLength)) == theBufferLength);

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



static const char*
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
	if(m_pMessage)
	{
		delete const_cast<char*> (m_pMessage);
	}
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
	if(m_pMessage)
	{
		delete const_cast<char*>(m_pMessage);
	}
}


void 	XalanFileOutputStream::write(const UTF16Ch*	theString, unsigned int		theLength)
{
	assert ( theString != 0 );
	writeData((const char*)theString,theLength * 2 );
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
	const char* szString = XMLString::transcode(theString);
	writeData( szString, theLengts );
	XMLString::release(const_cast<char**>(&szString));

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

void 	XalanFileOutputStream::writeUTFprefix( void )
{
	write(s_UTF16ByteOrderMark,1);
}



