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
#include "TextOutputStream.hpp"



#include <util/PlatformUtils.hpp>
#include <util/TransService.hpp>
#include <util/XMLString.hpp>



TextOutputStream::TextOutputStream(unsigned int		theTranscoderBlockSize) :
	m_transcoderBlockSize(theTranscoderBlockSize),
	m_encoding(),
	m_transcoder(0)
{
	assert(m_transcoderBlockSize > 0);
}



TextOutputStream::~TextOutputStream()
{
	delete m_transcoder;
}



void
TextOutputStream::transcode(
			const XalanDOMChar*		theBuffer,
			unsigned long			theBufferLength,
			TranscodeVectorType&	theDestination)
{
	if (m_transcoder == 0)
	{
		if (TranscodeToLocalCodePage(
				theBuffer,
				theDestination) == false)
		{
			throw TranscodingException();
		}
	}
	else
	{
		try
		{
			bool					fDone = false;

			// Keep track of the total bytes we've added to the
			// destination vector, and the total bytes we've
			// eaten from theBuffer.
			unsigned int			theTotalBytesFilled = 0;
			unsigned int			theTotalBytesEaten = 0;

			// Keep track of the current position in the input buffer,
			// and amount remaining in the buffer, since we may not be
			// able to transcode it all at once.
			const XalanDOMChar*		theBufferPosition = theBuffer;
			unsigned int			theRemainingBufferLength = theBufferLength;

			// Keep track of the destination size, and the target size, which is
			// the size of the destination that has not yet been filled with
			// transcoded characters...
			unsigned int			theDestinationSize = theBufferLength;
			unsigned int			theTargetSize = theDestinationSize;

			do
			{
				// Resize the buffer...
				theDestination.resize(theDestinationSize);

				unsigned int	theBytesEaten = 0;

				// Transcode and accumulate theTotalBytesFilled...
				theTotalBytesFilled +=
				m_transcoder->transcodeTo(
						theBufferPosition,
						theRemainingBufferLength,
#if defined(XALAN_OLD_STYLE_CASTS)
						(XMLByte*)&theDestination[0] + theTotalBytesFilled,
#else
						reinterpret_cast<XMLByte*>(&theDestination[0]) + theTotalBytesFilled,
#endif
						theTargetSize,
						theBytesEaten,
						XMLTranscoder::UnRep_Throw);

				theTotalBytesEaten += theBytesEaten;

				if (theTotalBytesEaten == theBufferLength)
				{
					fDone = true;
				}
				else
				{
					assert(theTotalBytesEaten < theBufferLength);

					// Update everything...
					theBufferPosition += theBytesEaten;
					theRemainingBufferLength -= theBytesEaten;

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
		catch(const XMLException&)
		{
			throw TranscodingException();
		}
	}
}



const XalanDOMString&
TextOutputStream::getOutputEncoding() const
{
	return m_encoding;
}



void
TextOutputStream::setOutputEncoding(const XalanDOMString&	theEncoding)
{
	// Disabled for now...
#if 0
	assert(XMLPlatformUtils::fgTransService != 0);

	delete m_transcoder;

	XMLTransService::Codes	theCode = XMLTransService::Ok;

	m_transcoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(
				c_wstr(theEncoding),
				theCode,
				m_transcoderBlockSize);

	if (theCode == XMLTransService::UnsupportedEncoding)
	{
		throw UnsupportedEncodingException(theEncoding);
	}
	else if (theCode != XMLTransService::Ok)
	{
		throw TranscoderInternalFailureException(theEncoding);
	}
	else
	{
		assert(m_transcoder != 0);

		m_encoding = theEncoding;
	}
#endif
}



TextOutputStream::TextOutputStreamException::TextOutputStreamException(
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theType) :
	XSLException(theMessage, theType)
{
}



TextOutputStream::TextOutputStreamException::~TextOutputStreamException()
{
}



TextOutputStream::UnknownEncodingException::UnknownEncodingException() :
	TextOutputStreamException(
			XALAN_STATIC_UCODE_STRING("Unknown error occurred while transcoding!"),
			XALAN_STATIC_UCODE_STRING("UnknownEncodingException"))
{
}



TextOutputStream::UnknownEncodingException::~UnknownEncodingException()
{
}



TextOutputStream::UnsupportedEncodingException::UnsupportedEncodingException(const XalanDOMString&	theEncoding) :
	TextOutputStreamException(
			XALAN_STATIC_UCODE_STRING("Unsupported encoding:") + theEncoding,
			XALAN_STATIC_UCODE_STRING("UnsupportedEncodingException")),
	m_encoding(theEncoding)
{
}



TextOutputStream::UnsupportedEncodingException::~UnsupportedEncodingException()
{
}



TextOutputStream::TranscoderInternalFailureException::TranscoderInternalFailureException(const XalanDOMString&	theEncoding) :
	TextOutputStreamException(
			XALAN_STATIC_UCODE_STRING("Unknown error occurred while transcoding to ") +
					theEncoding +
					XALAN_STATIC_UCODE_STRING("!"),
			XALAN_STATIC_UCODE_STRING("TranscoderInternalFailureException")),
	m_encoding(theEncoding)
{
}



TextOutputStream::TranscoderInternalFailureException::~TranscoderInternalFailureException()
{
}



TextOutputStream::TranscodingException::TranscodingException() :
	TextOutputStreamException(
			XALAN_STATIC_UCODE_STRING("An error occurred while transcoding!"),
			XALAN_STATIC_UCODE_STRING("TranscodingException"))
{
}



TextOutputStream::TranscodingException::~TranscodingException()
{
}
