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
#include <strstream>



#include <Include/XalanAutoPtr.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>




XalanFileOutputStream::XalanFileOutputStream(const XalanDOMString&		theFileName) :
	XalanOutputStream(),
	m_fileName(theFileName),
	m_handle(0)
{
	const CharVectorType	theResult(TranscodeToLocalCodePage(theFileName));

	assert(theResult.size() > 0);

	const char* const	tmpName = &theResult[0];

	m_handle = fopen(tmpName, "wb");

    if (m_handle == 0)
	{
		throw XalanFileOutputStreamOpenException(theFileName,
												errno);
	}
}



XalanFileOutputStream::~XalanFileOutputStream()
{
    if (m_handle != 0)
	{
		fclose(m_handle);
	}
}



void
XalanFileOutputStream::doFlush()
{
	fflush(m_handle);
}



void
XalanFileOutputStream::writeData(
			const char*		theBuffer,
			unsigned long	theBufferLength)
{
	const size_t	theBytesWritten =
		fwrite(theBuffer,
			   1,
			   theBufferLength,
			   m_handle);

	if(theBytesWritten != theBufferLength)
	{
		throw XalanFileOutputStreamWriteException(m_fileName,
												 errno);
	}
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
