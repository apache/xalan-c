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
#include "XalanFileOutputStream.hpp"



#include <cerrno>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <strstream.h>
#else
#include <strstream>
#endif



#include <Include/XalanAutoPtr.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



static  XalanFileOutputStream::HandleType
openFile(const XalanDOMString&	theFileName)
{
	typedef XalanFileOutputStream::HandleType	HandleType;

#if defined(WIN32)
	HandleType	theFileHandle = CreateFileW(
			c_wstr(theFileName),
			GENERIC_WRITE,
			0,
			0,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			0);

	if (theFileHandle != INVALID_HANDLE_VALUE && theFileHandle != 0)
	{
		return theFileHandle;
	}
	else
	{
		const CharVectorType	theResult(TranscodeToLocalCodePage(theFileName));

		if (theResult.size() == 0)
		{
			return INVALID_HANDLE_VALUE;
		}
		else
		{
			const char* const	tmpName = &theResult[0];

			if (tmpName == 0)
			{
				return INVALID_HANDLE_VALUE;
			}
			else
			{
				return CreateFile(
							tmpName,
							GENERIC_WRITE,
							0,
							0,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							0);
			}
		}
	}
#else
	const CharVectorType	theResult(TranscodeToLocalCodePage(theFileName));

	if (theResult.size() == 0)
	{
		return 0;
	}
	else
	{
		const char* const	tmpName = &theResult[0];

		if (tmpName == 0)
		{
			return 0;
		}
		else
		{
			return fopen(tmpName, "wb");
		}
	}
#endif
}



XalanFileOutputStream::XalanFileOutputStream(
			const XalanDOMString&	theFileName,
			unsigned int			theBufferSize) :
	XalanOutputStream(theBufferSize),
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
			size_type		theBufferLength)
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



static XalanDOMString
FormatMessageLocal(
			const char*				theMessage,
			const XalanDOMString&	theFileName,
			int						theErrorCode)
{
	XalanDOMString	theResult(TranscodeFromLocalCodePage(theMessage));

	theResult += theFileName;

#if !defined(XALAN_NO_NAMESPACES)
using std::ostrstream;
#endif

	ostrstream	theFormatter;

	theFormatter << ".  The error code was "
				 << theErrorCode << "." << '\0';

	append(theResult, theFormatter.str());

	delete theFormatter.str();

	return theResult;
}



XalanFileOutputStream::XalanFileOutputStreamOpenException::XalanFileOutputStreamOpenException(
		const XalanDOMString&	theFileName,
		int					theErrorCode) :
	XalanOutputStreamException(FormatMessageLocal(
				"Error opening file: ",
				theFileName,
				theErrorCode),
			TranscodeFromLocalCodePage("XalanFileOutputStreamOpenException"))
{
}



XalanFileOutputStream::XalanFileOutputStreamOpenException::~XalanFileOutputStreamOpenException()
{
}



XalanFileOutputStream::XalanFileOutputStreamWriteException::XalanFileOutputStreamWriteException(
		const XalanDOMString&	theFileName,
		int					theErrorCode) :
	XalanOutputStreamException(FormatMessageLocal(
				"Error writing file: ",
				theFileName,
				theErrorCode),
			TranscodeFromLocalCodePage("XalanFileOutputStreamWriteException"))
{
}



XalanFileOutputStream::XalanFileOutputStreamWriteException::~XalanFileOutputStreamWriteException()
{
}
