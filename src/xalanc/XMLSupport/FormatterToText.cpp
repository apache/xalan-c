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
// Class header file
#include "FormatterToText.hpp"



#include <xalanc/PlatformSupport/Writer.hpp>
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>
#if defined(XALAN_NEWLINE_IS_CRLF)
#include <xalanc/PlatformSupport/XalanUnicode.hpp>
#endif



XALAN_CPP_NAMESPACE_BEGIN



FormatterToText::FormatterToText() :
	FormatterListener(OUTPUT_METHOD_TEXT),
	m_writer(0),
	m_maxCharacter(XalanDOMChar(~0)),
	m_encoding(),
	m_haveEncoding(false),
	m_normalize(true),
	m_handleIgnorableWhitespace(true),
	m_newlineString(0),
	m_newlineStringLength(0)
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
	m_handleIgnorableWhitespace(handleIgnorableWhitespace),
	m_newlineString(0),
	m_newlineStringLength(0)
{
	update(true);
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
	m_handleIgnorableWhitespace(handleIgnorableWhitespace),
	m_newlineString(0),
	m_newlineStringLength(0)
{
	update(false);
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
FormatterToText::setDocumentLocator(const LocatorType* const	/* locator */)
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
}



void
FormatterToText::startElement(
			const	XMLCh* const	/* name */,
			AttributeListType&		/* attrs */)
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
		unsigned int i = 0;

		while(i < length)
		{
			if (chars[i] > m_maxCharacter)
			{
				//$$$ ToDo: Figure out what we're going to do here...
			}

#if defined(XALAN_NEWLINE_IS_CRLF)
			if (m_normalize == false)
			{
				m_writer->write(chars[i]);
			}
			else
			{
				// Normalize LF and CR/LF to the appropriate line ending sequence.
				if (chars[i] == XalanUnicode::charLF)
				{
					m_writer->write(m_newlineString, 0, m_newlineStringLength);
				}
				else if (chars[i] == XalanUnicode::charCR &&
					(i + 1 < length &&
					 chars[i + 1] == XalanUnicode::charLF))
				{
					m_writer->write(m_newlineString, 0, m_newlineStringLength);

					++i;
				}
				else
				{
					m_writer->write(chars[i]);
				}
			}
#else
			m_writer->write(chars[i]);
#endif

			++i;
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
FormatterToText::update(bool	fNormalizationOnly)
{
	assert(m_writer != 0);

	XalanOutputStream* const	theStream = m_writer->getStream();

	if (theStream == 0)
	{
		m_newlineString = XalanOutputStream::defaultNewlineString();
		m_newlineStringLength = length(m_newlineString);

		if (fNormalizationOnly == false)
		{
			// We're pretty much screwed here, since we can't transcode, so get the
			// maximum character for the local code page.
			m_maxCharacter = XalanTranscodingServices::getMaximumCharacterValue();
		}
	}
	else
	{
		m_newlineString = theStream->getNewlineString();
		assert(m_newlineString != 0);

		m_newlineStringLength = length(m_newlineString);

		if (fNormalizationOnly == false)
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
}



XALAN_CPP_NAMESPACE_END
