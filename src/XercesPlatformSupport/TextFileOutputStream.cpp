/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
// Class header file...
#include "TextFileOutputStream.hpp"



#include <sstream>



#if defined(_MSC_VER)
#include "windows.h"
#else
#define INVALID_HANDLE_VALUE 0
#endif



#include <util/Janitor.hpp>
#include <util/XMLString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>





const TextFileOutputStream::BufferType::size_type	TextFileOutputStream::s_bufferSize = 8192;



TextFileOutputStream::TextFileOutputStream(const DOMString&		theFileName) :
	TextOutputStream(),
	m_fileName(theFileName),
#if defined(_MSC_VER)
	m_handle(INVALID_HANDLE_VALUE),
#else
	m_handle(0),
#endif
	m_buffer()
{
	m_buffer.reserve(s_bufferSize + 1);

#if defined(_MSC_VER)

	// check the version.  Only NT allows  
	// wide character file paths
	OSVERSIONINFO verInfo;
	verInfo.dwOSVersionInfoSize=sizeof(verInfo);
	::GetVersionEx(&verInfo);

    if (verInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
        m_handle = ::CreateFileW(toCharArray(theFileName),
								 GENERIC_WRITE,
								 FILE_SHARE_WRITE,
								 0,
								 CREATE_ALWAYS,
								 FILE_FLAG_SEQUENTIAL_SCAN,
								 0);
    }
    else
    {
        char* const	tmpName = XMLString::transcode(toCharArray(theFileName));

		ArrayJanitor<char> janTmp(tmpName);

        m_handle = ::CreateFileA(tmpName,
								 GENERIC_WRITE,
								 FILE_SHARE_WRITE,
								 0,
								 CREATE_ALWAYS,
								 FILE_FLAG_SEQUENTIAL_SCAN,
								 0);
    }

#else
	char* const	tmpName = XMLString::transcode(toCharArray(theFileName));

	ArrayJanitor<char> janTmp(tmpName);

	m_handle = fopen(tmpName, "wt");
#endif

    if (m_handle == INVALID_HANDLE_VALUE)
	{
		throw TextFileOutputStreamOpenException(theFileName,
												errno);
	}
}



TextFileOutputStream::~TextFileOutputStream()
{
    if (m_handle != INVALID_HANDLE_VALUE)
	{
#if defined(_MSC_VER)
		CloseHandle(m_handle);
#else
		fclose(m_handle);
#endif
	}
}



void
TextFileOutputStream::flush()
{
	flushBuffer();

#if defined(_MSC_VER)
	FlushFileBuffers(m_handle);
#else
	fflush(m_handle);
#endif
}



void
TextFileOutputStream::write(const XMLCh* const	theBuffer)
{
	const BufferType::size_type		theLength =
		static_cast<BufferType::size_type>(length(theBuffer));

	if (theLength + m_buffer.size() > s_bufferSize)
	{
		flushBuffer();
	}

	if (theLength > s_bufferSize)
	{
		doWrite(theBuffer);
	}
	else
	{
		m_buffer.insert(m_buffer.end(),
						theBuffer,
						theBuffer + theLength);
	}
}



void
TextFileOutputStream::write(const char* const	theBuffer)
{
	flushBuffer();

	doWrite(theBuffer,
		    strlen(theBuffer));
}



void
TextFileOutputStream::write(
			const char*		theBuffer,
			unsigned long	theBufferLength)
{
	flushBuffer();

	doWrite(theBuffer,
			theBufferLength);
}



void
TextFileOutputStream::flushBuffer()
{
	if (m_buffer.size() > 0)
	{
		m_buffer.push_back(0);

		doWrite(m_buffer.begin());

		m_buffer.clear();
	}
}



void
TextFileOutputStream::doWrite(const XMLCh*	theBuffer)
{
    char* const	tmpVal = XMLString::transcode(theBuffer);

    ArrayJanitor<char> janTmp(tmpVal);

	doWrite(tmpVal,
			strlen(tmpVal));
}



void
TextFileOutputStream::doWrite(
			const char*		theBuffer,
			unsigned long	theBufferLength)
{
#if defined(_MSC_VER)
	DWORD	theBytesWritten;

	WriteFile(m_handle,
			  theBuffer,
			  theBufferLength,
			  &theBytesWritten,
			  0);

#else
	const size_t	theBytesWritten =
		fwrite(theBuffer,
			   1,
			   theBufferLength,
			   m_handle);
#endif

	if(theBytesWritten != theBufferLength)
	{
		throw TextFileOutputStreamWriteException(m_fileName,
												 errno);
	}
}



TextFileOutputStreamException::TextFileOutputStreamException(
		const DOMString&	theMessage,
		const DOMString&	theType) :
	XercesPlatformSupportException(theMessage,
								theType)
{
}



TextFileOutputStreamException::~TextFileOutputStreamException()
{
}



namespace
{

const DOMString		theOpenExceptionType("TextFileOutputStreamOpenException");
const DOMString		theWriteExceptionType("TextFileOutputStreamWriteException");



DOMString
FormatMessageLocal(
			const DOMString&	theMessage,
			const DOMString&	theFileName,
			int					theErrorCode)
{
	DOMString	theResult(clone(theMessage));

	theResult += theFileName;

	std::stringstream	theFormatter;

	theFormatter << ".  The error code was "
				 << theErrorCode
				 << ".";

	theResult += theFormatter.str().c_str();

	return theResult;
}

};





TextFileOutputStreamOpenException::TextFileOutputStreamOpenException(
		const DOMString&	theFileName,
		int					theErrorCode) :
	TextFileOutputStreamException(FormatMessageLocal("Error opening file: ",
													 theFileName,
													 theErrorCode),
								  theOpenExceptionType)
{
}



TextFileOutputStreamOpenException::~TextFileOutputStreamOpenException()
{
}





TextFileOutputStreamWriteException::TextFileOutputStreamWriteException(
		const DOMString&	theFileName,
		int					theErrorCode) :
	TextFileOutputStreamException(FormatMessageLocal("Error writing file: ",
													 theFileName,
													 theErrorCode),
								  theWriteExceptionType)
{
}



TextFileOutputStreamWriteException::~TextFileOutputStreamWriteException()
{
}



