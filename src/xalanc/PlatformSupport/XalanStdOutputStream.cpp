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
// Class header file...
#include "XalanStdOutputStream.hpp"



#include <cerrno>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include "DOMStringHelper.hpp"
#include "XalanMessageLoader.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanStdOutputStream::XalanStdOutputStream(StreamType&	theOutputStream) :
	XalanOutputStream(),
#if !defined(XALAN_NEWLINE_IS_CRLF)
	m_outputStream(theOutputStream)
#else
	m_outputStream(theOutputStream),
	m_newlineString(0),
	m_newlineStringLength(0)
#endif
{
	// This will make sure that cerr is not buffered...
	if (&m_outputStream == &XALAN_STD_QUALIFIER cerr)
	{
		setBufferSize(0);

#if defined(XALAN_NEWLINE_IS_CRLF)
		m_newlineString = s_nlString;
		m_newlineStringLength = s_nlStringLength;
#endif
	}
#if defined(XALAN_NEWLINE_IS_CRLF)
	else if (&m_outputStream == &XALAN_STD_QUALIFIER cout)
	{
		m_newlineString = s_nlString;
		m_newlineStringLength = s_nlStringLength;
	}
	else
	{
		m_newlineString = s_nlCRString;
		m_newlineStringLength = s_nlCRStringLength;
	}
#endif
}



XalanStdOutputStream::~XalanStdOutputStream()
{
}



#if defined(XALAN_NEWLINE_IS_CRLF)
void
XalanStdOutputStream::newline()
{
	assert(m_newlineString != 0 && length(m_newlineString) == m_newlineStringLength);

	write(m_newlineString, m_newlineStringLength);
}



const XalanDOMChar*
XalanStdOutputStream::getNewlineString() const
{
	assert(m_newlineString != 0);

	return m_newlineString;
}
#endif



void
XalanStdOutputStream::doFlush()
{
	// Don't try to flush if the stream is in a bad state...
	if(m_outputStream)
	{
		m_outputStream.flush();

		if(!m_outputStream)
		{
#if defined(XALAN_STRICT_ANSI_HEADERS)
			using namespace std;
#endif

			throw XalanStdOutputStreamWriteException(errno);
		}
	}
}



void
XalanStdOutputStream::writeData(
			const char*		theBuffer,
			size_type		theBufferLength)
{
	assert(StreamSizeType(theBufferLength) == theBufferLength);

	m_outputStream.write(theBuffer, StreamSizeType(theBufferLength));

	if(!m_outputStream)
	{
#if defined(XALAN_STRICT_ANSI_HEADERS)
		using namespace std;
#endif

		throw XalanStdOutputStreamWriteException(errno);
	}
}



static XalanDOMString
FormatMessageLocal(
			const XalanDOMString&	theMessage,
			int						theErrorCode)
{
	XalanDOMString	theResult(theMessage);

	XalanDOMString  theStrErrCode;
	LongToDOMString(theErrorCode, theStrErrCode);

	return theResult + XalanMessageLoader::getMessage(XalanMessages::MessageErrorCodeWas_1Param,theStrErrCode);
}



XalanStdOutputStream::XalanStdOutputStreamWriteException::XalanStdOutputStreamWriteException(
		int					theErrorCode) :
	XalanOutputStreamException(FormatMessageLocal(XalanMessageLoader::getMessage(XalanMessages::ErrorWritingToStdStream),
													   theErrorCode),
								    TranscodeFromLocalCodePage("XercesStdOutputStreamWriteException"))
{
}



XalanStdOutputStream::XalanStdOutputStreamWriteException::~XalanStdOutputStreamWriteException()
{
}



XALAN_CPP_NAMESPACE_END
