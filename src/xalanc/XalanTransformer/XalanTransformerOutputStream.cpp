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
#include "XalanTransformerOutputStream.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanTransformerOutputStream::XalanTransformerOutputStream(
	void*						theOutputHandle, 
	XalanOutputHandlerType		theOutputHandler,
	XalanFlushHandlerType		theFlushHandler):
	m_outputHandle(theOutputHandle),
	m_outputHandler(theOutputHandler),
	m_flushHandler(theFlushHandler)
{
}



XalanTransformerOutputStream::~XalanTransformerOutputStream()
{
}



void
XalanTransformerOutputStream::doFlush()
{
	if(m_flushHandler != 0)
	{
		m_flushHandler(m_outputHandle);
	}
}



void
XalanTransformerOutputStream::writeData(
			const char*		theBuffer,
			size_type		theBufferLength)
{
	const size_t theBytesWritten = m_outputHandler(theBuffer, theBufferLength, m_outputHandle);		

	// We validate that the number of bytes written equals the number of bytes sent to
	// the output handler. Otherwise we will stop processing  and throw an exception. 
	// Thus the callback can alert us of memory allocation issues or buffer overflows.
	if(theBytesWritten != theBufferLength)
	{
		throw XalanOutputStreamException(
			XalanMessageLoader::getMessage(XalanMessages::NumberBytesWrittenDoesNotEqual),
			TranscodeFromLocalCodePage("UnknownOutputHandlerException"));
	}
}




XALAN_CPP_NAMESPACE_END
