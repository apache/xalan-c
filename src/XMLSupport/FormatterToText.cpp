/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
// Class header file
#include "FormatterToText.hpp"



#include <PlatformSupport/Writer.hpp>
#include <PlatformSupport/XalanOutputStream.hpp>
#include <PlatformSupport/XalanTranscodingServices.hpp>
#if defined(XALAN_NEWLINE_IS_CRLF)
#include <PlatformSupport/XalanUnicode.hpp>
#endif



FormatterToText::FormatterToText() :
	FormatterListener(OUTPUT_METHOD_TEXT),
	m_writer(0),
	m_maxCharacter(XalanDOMChar(~0)),
	m_encoding(),
	m_haveEncoding(false),
	m_normalize(true),
	m_handleIgnorableWhitespace(true)
{
}



FormatterToText::FormatterToText(
			Writer&		writer,
			bool		normalizeLinefeed,
			bool		handleIgnorableWhitespace) :
	FormatterListener(OUTPUT_METHOD_TEXT),
	m_writer(&writer),
	m_maxCharacter(XalanDOMChar(~0)),
	m_encoding(),
	m_haveEncoding(false),
	m_normalize(normalizeLinefeed),
	m_handleIgnorableWhitespace(handleIgnorableWhitespace)
{
}



FormatterToText::FormatterToText(
			Writer&					writer,
			const XalanDOMString&	encoding,
			bool					normalizeLinefeed,
			bool					handleIgnorableWhitespace) :
	FormatterListener(OUTPUT_METHOD_TEXT),
	m_writer(&writer),
	m_maxCharacter(0),
	m_encoding(isEmpty(encoding) == false ? encoding : XalanDOMString(XalanTranscodingServices::s_utf8String)),
	m_haveEncoding(true),
	m_normalize(normalizeLinefeed),
	m_handleIgnorableWhitespace(handleIgnorableWhitespace)
{
	update();
}



FormatterToText::~FormatterToText()
{
}



void
FormatterToText::clearEncoding()
{
	clear(m_encoding);

	m_maxCharacter = XalanDOMChar(~0);

	m_haveEncoding = false;
}



void
FormatterToText::setDocumentLocator(const Locator* const	/* locator */)
{
	// No action for the moment.
}



void
FormatterToText::startDocument()
{
	// No action for the moment.
}



void
FormatterToText::endDocument()
{
	assert(m_writer != 0);

	m_writer->flush();

	m_writer->close();
}



void
FormatterToText::startElement(
			const	XMLCh* const	/* name */,
			AttributeList&			/* attrs */)
{
	// No action for the moment.
}



void
FormatterToText::endElement(
			const	XMLCh* const	/* name */)
{
	// No action for the moment.
}



void
FormatterToText::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	assert(m_writer != 0);

	if (m_normalize == false && m_haveEncoding == false)
	{
		m_writer->write(chars, 0, length);
	}
	else
	{
		for (unsigned int i = 0; i < length; ++i)
		{
#if defined(XALAN_NEWLINE_IS_CRLF)
			if (m_normalize == true)
			{
				// Normalize LF to CR/LF...
				if (chars[i] == XalanUnicode::charLF &&
					(i == 0 ||
					 chars[i - 1] != XalanUnicode::charCR))
				{
					m_writer->write(XalanUnicode::charCR);
				}
			}
#endif
			if (chars[i] > m_maxCharacter)
			{
				//$$$ ToDo: Figure out what we're going to do here...
			}

			m_writer->write(chars[i]);
		}
	}
}



void
FormatterToText::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	characters(chars, length);
}


void
FormatterToText::entityReference(const XMLCh* const	/* name */)
{
	// No action for the moment.
}



void
FormatterToText::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if (m_handleIgnorableWhitespace == true)
	{
		characters(chars, length);
	}
}



void
FormatterToText::processingInstruction(
			const XMLCh* const	/* target */,
			const XMLCh* const	/* data */)
{
	// No action for the moment.
}



void
FormatterToText::resetDocument()
{
	// No action for the moment.
}



void
FormatterToText::comment(const XMLCh* const	/* data */)
{
	// No action for the moment.
}



void
FormatterToText::cdata(
			const XMLCh* const	ch,
			const unsigned int 	length)
{
	characters(ch, length);
}



void
FormatterToText::update()
{
	assert(m_writer != 0);

	XalanOutputStream* const	theStream = m_writer->getStream();

	if (theStream == 0)
	{
		// We're pretty much screwed here, since we can't transcode, so get the
		// maximum character for the local code page.
		m_maxCharacter = XalanTranscodingServices::getMaximumCharacterValue();
	}
	else
	{
		try
		{
			theStream->setOutputEncoding(m_encoding);
		}
		catch(const XalanOutputStream::UnsupportedEncodingException&)
		{
			const XalanDOMString	theUTF8String(XalanTranscodingServices::s_utf8String);

			// Default to UTF-8 if the requested encoding is not supported...
			theStream->setOutputEncoding(theUTF8String);

			m_encoding = theUTF8String;

			m_haveEncoding = true;
		}

		m_maxCharacter = XalanTranscodingServices::getMaximumCharacterValue(theStream->getOutputEncoding());
	}
}
