/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999, 2000 The Apache Software Foundation.  All rights 
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
#include "XalanOutputStream.hpp"



#include <util/TransService.hpp>
#include <util/XMLException.hpp>



#include "XalanTranscodingServices.hpp"



XalanOutputStream::XalanOutputStream(
			BufferType::size_type			theBufferSize,
			TranscodeVectorType::size_type	theTranscoderBlockSize,
			bool							fThrowTranscodeException) :
	m_transcoderBlockSize(theTranscoderBlockSize),
	m_transcoder(0),
	m_bufferSize(theBufferSize),
	m_buffer(),
	m_encoding(),
	m_writeAsUTF16(false),
	m_throwTranscodeException(fThrowTranscodeException),
	m_transcodingBuffer()
{
	if (m_bufferSize == 0)
	{
		m_bufferSize = 1;
	}

	m_buffer.reserve(theBufferSize + 1);
}



XalanOutputStream::~XalanOutputStream()
{
	XalanTranscodingServices::destroyTranscoder(m_transcoder);
}



void
XalanOutputStream::write(
			const XalanDOMChar*		theBuffer,
			size_t					theBufferLength)
{
	assert(theBuffer != 0);

	if (theBufferLength + m_buffer.size() > m_bufferSize)
	{
		flushBuffer();
	}

	if (theBufferLength > m_bufferSize)
	{
		doWrite(theBuffer, theBufferLength);
	}
	else
	{
		m_buffer.insert(m_buffer.end(),
						theBuffer,
						theBuffer + theBufferLength);
	}
}



void
XalanOutputStream::transcode(
			const XalanDOMChar*		theBuffer,
			size_t					theBufferLength,
			TranscodeVectorType&	theDestination)
{
	if (m_transcoder == 0)
	{
		if (TranscodeToLocalCodePage(
				theBuffer,
				theBufferLength,
				theDestination) == false)
		{
			if (m_throwTranscodeException == true)
			{
				throw TranscodingException();
			}
			else
			{
			}
		}
	}
	else
	{
		bool					fDone = false;

		// Keep track of the total bytes we've added to the
		// destination vector, and the total bytes we've
		// eaten from theBuffer.
		size_t	theTotalBytesFilled = 0;
		size_t	theTotalBytesEaten = 0;

		// Keep track of the current position in the input buffer,
		// and amount remaining in the buffer, since we may not be
		// able to transcode it all at once.
		const XalanDOMChar*		theBufferPosition = theBuffer;
		size_t					theRemainingBufferLength = theBufferLength;

		// Keep track of the destination size, and the target size, which is
		// the size of the destination that has not yet been filled with
		// transcoded characters.  Double the buffer size, in case we're
		// transcoding to a 16-bit encoding.
		// $$$ ToDo: We need to know the size of an encoding, so we can
		// do the right thing with the destination size.
		size_t	theDestinationSize = theBufferLength * 2;
		size_t	theTargetSize = theDestinationSize;

		do
		{
			// Resize the buffer...
			theDestination.resize(theDestinationSize + 1);

			size_t	theSourceBytesEaten = 0;
			size_t	theTargetBytesEaten = 0;

			XalanTranscodingServices::eCode		theResult =
				m_transcoder->transcode(
						theBufferPosition,
						theRemainingBufferLength,
#if defined(XALAN_OLD_STYLE_CASTS)
						(XMLByte*)&theDestination[0] + theTotalBytesFilled,
#else
						reinterpret_cast<XMLByte*>(&theDestination[0]) + theTotalBytesFilled,
#endif
						theTargetSize,
						theSourceBytesEaten,
						theTargetBytesEaten);

			if(theResult != XalanTranscodingServices::OK)
			{
				if (m_throwTranscodeException == true)
				{
					throw TranscodingException();
				}
				else
				{
				}
			}

			theTotalBytesFilled += theTargetBytesEaten;
			theTotalBytesEaten += theSourceBytesEaten;

			if (theTotalBytesEaten == theBufferLength)
			{
				fDone = true;
			}
			else
			{
				assert(theTotalBytesEaten < theBufferLength);

				// Update everything...
				theBufferPosition += theSourceBytesEaten;
				theRemainingBufferLength -= theSourceBytesEaten;

				// The new target size will always be the
				// current destination size, since we
				// grow by a factor of 2.  This will
				// need to change if the factor is
				// every changed.
				theTargetSize = theDestinationSize;

				// Grow the destination by a factor of
				// two 2.  See the previous comment if
				// you want to change this.
				theDestinationSize = theDestinationSize * 2;
			}
		} while(fDone == false);

		// Resize things, if there are any extra bytes...
		if (theDestination.size() != theTotalBytesFilled)
		{
			theDestination.resize(theTotalBytesFilled);
		}
	}
}



void
XalanOutputStream::setOutputEncoding(const XalanDOMString&	theEncoding)
{
	// Flush, just in case.  This should probably be an error...
	flushBuffer();

	XalanTranscodingServices::destroyTranscoder(m_transcoder);

	XalanTranscodingServices::eCode		theCode = XalanTranscodingServices::OK;

	// This turns on an optimization that we can only do if
	// XalanDOMChar == sizeof(ushort).  See doWrite().
#if !defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
	if (XalanTranscodingServices::encodingIsUTF16(theEncoding) == true)
	{
		m_writeAsUTF16 = true;
	}
	else
#endif
	{
		m_transcoder = XalanTranscodingServices::makeNewTranscoder(
					theEncoding,
					theCode,
					m_transcoderBlockSize);

		if (theCode == XalanTranscodingServices::UnsupportedEncoding)
		{
			throw UnsupportedEncodingException(theEncoding);
		}
		else if (theCode != XalanTranscodingServices::OK)
		{
			throw TranscoderInternalFailureException(theEncoding);
		}

		assert(m_transcoder != 0);
	}

	m_encoding = theEncoding;

	const XalanTranscodingServices::XalanXMLByte*	theProlog =
		XalanTranscodingServices::getStreamProlog(theEncoding);
	assert(theProlog != 0);

	const size_t	theLength = XalanTranscodingServices::length(theProlog);

	if (theLength > 0)
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		write((const char*)theProlog, theLength);
#else
		write(reinterpret_cast<const char*>(theProlog), theLength);
#endif
	}
}



void
XalanOutputStream::flushBuffer()
{
	if (m_buffer.size() > 0)
	{
		doWrite(&*m_buffer.begin(), m_buffer.size());

		m_buffer.clear();
	}
}



void
XalanOutputStream::doWrite(
			const XalanDOMChar*		theBuffer,
			size_t					theBufferLength)
{
	assert(theBuffer != 0);

	try
	{
		if (m_writeAsUTF16 == true)
		{
			assert(sizeof(XalanDOMChar) == sizeof(char) * 2);

			// This is a hack to write UTF-16 through as if it
			// were just chars.  Saves lots of time "transcoding."
#if defined(XALAN_OLD_STYLE_CASTS)
			writeData((const char*)theBuffer, theBufferLength * 2);
#else
			writeData(reinterpret_cast<const char*>(theBuffer), theBufferLength * 2);
#endif
		}
		else
		{
			transcode(theBuffer, theBufferLength, m_transcodingBuffer);

			assert(&m_transcodingBuffer[0] != 0);

			writeData(&m_transcodingBuffer[0],
					  m_transcodingBuffer.size());
		}
	}
	catch(const XalanOutputStreamException&)
	{
		// Have to catch this error and flush any output remaining...
		m_buffer.clear();

		throw;
	}
}



void
XalanOutputStream::setBufferSize(BufferType::size_type	theBufferSize)
{
	flushBuffer();

	if (theBufferSize == 0)
	{
		m_bufferSize = 1;
	}
	else
	{
		m_bufferSize = theBufferSize;
	}

	if (m_buffer.size() < m_bufferSize)
	{
		// Enlarge the buffer...
		m_buffer.reserve(theBufferSize + 1);
	}
	else if (m_buffer.size() > m_bufferSize)
	{
		// Shrink the buffer.

		// Create a temp buffer and make it
		// the correct size.
		BufferType	temp;
		
		temp.reserve(theBufferSize + 1);
		
		// Swap temp with m_buffer so that
		// m_buffer is now the correct size.
		temp.swap(m_buffer);
	}
}



XalanOutputStream::XalanOutputStreamException::XalanOutputStreamException(
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theType) :
	XSLException(theMessage, theType)
{
}



XalanOutputStream::XalanOutputStreamException::~XalanOutputStreamException()
{
}



XalanOutputStream::UnknownEncodingException::UnknownEncodingException() :
	XalanOutputStreamException(
			TranscodeFromLocalCodePage("Unknown error occurred while transcoding!"),
			TranscodeFromLocalCodePage("UnknownEncodingException"))
{
}



XalanOutputStream::UnknownEncodingException::~UnknownEncodingException()
{
}



XalanOutputStream::UnsupportedEncodingException::UnsupportedEncodingException(const XalanDOMString&	theEncoding) :
	XalanOutputStreamException(
			TranscodeFromLocalCodePage("Unsupported encoding: ") + theEncoding,
			TranscodeFromLocalCodePage("UnsupportedEncodingException")),
	m_encoding(theEncoding)
{
}



XalanOutputStream::UnsupportedEncodingException::~UnsupportedEncodingException()
{
}



XalanOutputStream::TranscoderInternalFailureException::TranscoderInternalFailureException(const XalanDOMString&	theEncoding) :
	XalanOutputStreamException(
			TranscodeFromLocalCodePage("Unknown error occurred while transcoding to ") +
					theEncoding +
					TranscodeFromLocalCodePage("!"),
			TranscodeFromLocalCodePage("TranscoderInternalFailureException")),
	m_encoding(theEncoding)
{
}



XalanOutputStream::TranscoderInternalFailureException::~TranscoderInternalFailureException()
{
}



XalanOutputStream::TranscodingException::TranscodingException() :
	XalanOutputStreamException(
			TranscodeFromLocalCodePage("An error occurred while transcoding!"),
			TranscodeFromLocalCodePage("TranscodingException"))
{
}



XalanOutputStream::TranscodingException::~TranscodingException()
{
}
