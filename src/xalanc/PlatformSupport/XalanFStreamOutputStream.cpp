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
#include "XalanFStreamOutputStream.hpp"



#include <cerrno>



#include "DOMStringHelper.hpp"
#include "XalanMessageLoader.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanFStreamOutputStream::XalanFStreamOutputStream(
			FILE*			theHandle,
			unsigned int	theBufferSize) :
	XalanOutputStream(theBufferSize),
	m_handle(theHandle)
{
	assert(m_handle != 0);
}



XalanFStreamOutputStream::~XalanFStreamOutputStream()
{
}



void
XalanFStreamOutputStream::doFlush()
{
}



void
XalanFStreamOutputStream::writeData(
			const char*		theBuffer,
			size_type		theBufferLength)
{
	const size_t	theBytesWritten =
		fwrite(theBuffer,
			   1,
			   theBufferLength,
			   m_handle);

	if (theBytesWritten != theBufferLength)
	{
		throw XalanFStreamOutputStreamWriteException(errno);
	}
}

XalanDOMString
FormatMessageLocal(
			const XalanDOMString&	theMessage,
			int				theErrorCode)
{
	XalanDOMString strErrorCode;
	LongToDOMString(theErrorCode, strErrorCode);

	XalanDOMString	theResult = theMessage + XalanMessageLoader::getMessage(XalanMessages::SystemErrorCode_1Param, strErrorCode);
	return theResult;
}



XalanFStreamOutputStream::XalanFStreamOutputStreamWriteException::XalanFStreamOutputStreamWriteException(int	theErrorCode) :
	XalanOutputStreamException(FormatMessageLocal(
				XalanMessageLoader::getMessage(XalanMessages::ErrorWritingFile_1Param,XalanDOMString("")),
				theErrorCode),
			TranscodeFromLocalCodePage("XalanFStreamOutputStreamWriteException"))
{
}



XalanFStreamOutputStream::XalanFStreamOutputStreamWriteException::~XalanFStreamOutputStreamWriteException()
{
}



XALAN_CPP_NAMESPACE_END
