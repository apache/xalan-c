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
#include "XalanFileOutputStream.hpp"



#if !defined(WIN32)
#include <cerrno>
#endif



#include "DOMStringHelper.hpp"
#include "XalanMessageLoader.hpp"



XALAN_CPP_NAMESPACE_BEGIN



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

		if (theResult.empty() == true)
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

	if (theResult.empty() == true)
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
	{
		throw XalanFileOutputStreamOpenException(
					theFileName,
					GetLastError());
	}
#else
    if (m_handle == 0)
	{
		throw XalanFileOutputStreamOpenException(
					theFileName,
					errno);
	}
#endif
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


/*
static XalanDOMString
FormatMessageLocal(
			const XalanDOMString&	theMessage,

			int						theErrorCode)
{
	return XalanMessageLoader::getMessage(
#if defined(WIN32)
				XalanMessages::WindowsErrorCodeIs_3Params,
#else
				XalanMessages::CPPRunTimeErrorCode_3Params,
#endif
				theMessage,
				theFileName,
				LongToDOMString(theErrorCode));
}
*/
extern XalanDOMString
FormatMessageLocal(
			const XalanDOMString&	theMessage,
			int				theErrorCode);

XalanFileOutputStream::XalanFileOutputStreamOpenException::XalanFileOutputStreamOpenException(
		const XalanDOMString&	theFileName,
		int					theErrorCode) :
	XalanOutputStreamException(FormatMessageLocal(
				XalanMessageLoader::getMessage(XalanMessages::ErrorOpeningFile_1Param, theFileName ),
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
				XalanMessageLoader::getMessage(XalanMessages::ErrorWritingFile_1Param,theFileName),
				theErrorCode),
			TranscodeFromLocalCodePage("XalanFileOutputStreamWriteException"))
{
}



XalanFileOutputStream::XalanFileOutputStreamWriteException::~XalanFileOutputStreamWriteException()
{
}



XALAN_CPP_NAMESPACE_END
