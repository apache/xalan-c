/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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


// Class header file.
#include "FormatterToXML_UTF8.hpp"



#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/SAXException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/Writer.hpp>
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar	FormatterToXML_UTF8::s_specialChars[kSpecialsSize] =
{
	kNotSpecial,		// 0
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kBothSpecial,		// 0xA -- linefeed  Special because we normalize as requested.
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,		// 0x10
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,		// 0x20
	kNotSpecial,
	kAttributeSpecial,	// 0x22 '"'
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kBothSpecial,		// 0x26 -- '&'
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,		// 0x30
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kBothSpecial,		// 0x3C '<'
	kNotSpecial,
	kBothSpecial		// 0x3E '>'
};



FormatterToXML_UTF8::FormatterToXML_UTF8(
			Writer&					writer,
			const XalanDOMString&	version,
			const XalanDOMString&	mediaType,
			const XalanDOMString&	doctypeSystem,
			const XalanDOMString&	doctypePublic,
			bool					xmlDecl,
			const XalanDOMString&	standalone) :
	FormatterListener(OUTPUT_METHOD_XML),
	m_writer(&writer),
	m_shouldWriteXMLHeader(xmlDecl),
	m_needToOutputDocTypeDecl(true),
	m_nextIsRaw(false),
	m_doctypeSystem(doctypeSystem),
	m_doctypePublic(doctypePublic),
	m_spaceBeforeClose(false),
	m_version(version),
	m_standalone(standalone),
	m_mediaType(mediaType),
	m_elemStack(),
	m_newlineString(0),
	m_newlineStringLength(0),
	m_nameFunction(0),
	m_buffer(),
	m_bufferPosition(m_buffer),
	m_bufferRemaining(kBufferSize)
{
	if(isEmpty(m_doctypePublic) == false)
	{
		if(startsWith(
			m_doctypePublic,
			s_xhtmlDocTypeString,
			s_xhtmlDocTypeStringLength) == true)
		{
			m_spaceBeforeClose = true;
		}
	}

	const XalanOutputStream* const	theStream = writer.getStream();

	if (theStream == 0)
	{
		m_newlineString = XalanOutputStream::defaultNewlineString();
	}
	else
	{
		m_newlineString = theStream->getNewlineString();
	}

	assert(m_newlineString != 0);

	m_newlineStringLength = length(m_newlineString);

	assert(m_newlineString != 0);

	if (m_version.empty() == true ||
		DoubleSupport::equal(DOMStringToDouble(m_version), 1.0) == true)
	{
		m_nameFunction = &FormatterToXML_UTF8::writeName1_0;
	}
	else
	{
		m_nameFunction = &FormatterToXML_UTF8::writeName1_1;
	}
}



FormatterToXML_UTF8::~FormatterToXML_UTF8()
{
}



inline void
FormatterToXML_UTF8::flushBuffer()
{
	m_writer->write(m_buffer, 0, m_bufferPosition - m_buffer);

	m_bufferPosition = m_buffer;
	m_bufferRemaining = kBufferSize;
}



inline void
FormatterToXML_UTF8::write(char		theChar)
{
	assert(theChar < 128);

	if (m_bufferRemaining == 0)
	{
		flushBuffer();
	}

	*m_bufferPosition = theChar;

	++m_bufferPosition;
	--m_bufferRemaining;
}



inline char
bits19to21(unsigned int	theChar)
{
	return char((theChar >> 18) & 0x7);
}



inline char
bits13to18(unsigned int	theChar)
{
	return char((theChar >> 12) & 0x1F);
}



inline char
bits13to16(unsigned int	theChar)
{
	return char((theChar >> 12) & 0xF);
}



inline char
bits7to12(unsigned int	theChar)
{
	return char((theChar >> 6) & 0x3f);
}



inline char
bits7to11(unsigned int	theChar)
{
	return char((theChar >> 6) & 0x1f);
}



inline char
bits1to6(unsigned int	theChar)
{
	return char(theChar & 0x3f);
}



inline char
leadingByteOf2(char		theBits)
{
	return char(0xC0 + theBits);
}



inline char
leadingByteOf3(char		theBits)
{
	return char(0xE0 + theBits);
}



inline char
leadingByteOf4(char		theBits)
{
	return char(0xF0 + theBits);
}



inline char
trailingByte(char	theBits)
{
	return char(0x80 + theBits);
}



void
FormatterToXML_UTF8::write(unsigned int		theChar)
{
	if (theChar <= 0x7F)
	{
		write(char(theChar));
	}
	else if (theChar <= 0x7FF)
	{
		if (m_bufferRemaining < 2)
		{
			flushBuffer();
		}

		*m_bufferPosition = leadingByteOf2(bits7to11(theChar));
		++m_bufferPosition;
		*m_bufferPosition = trailingByte(bits1to6(theChar));
		++m_bufferPosition;

		m_bufferRemaining -= 2;
	}
	else if (theChar <= 0xFFFF)
	{
		// We should never get a high or low surrogate here...
		assert(theChar < 0xD800 || theChar > 0xDBFF);
		assert(theChar < 0xDC00 || theChar > 0xDFFF);

		if (m_bufferRemaining < 3)
		{
			flushBuffer();
		}

		*m_bufferPosition = leadingByteOf3(bits13to16(theChar));
		++m_bufferPosition;
		*m_bufferPosition = trailingByte(bits7to12(theChar));
		++m_bufferPosition;
		*m_bufferPosition = trailingByte(bits1to6(theChar));
		++m_bufferPosition;

		m_bufferRemaining -= 3;
	}
	else if (theChar <= 0x10FFFF)
	{
		if (m_bufferRemaining < 4)
		{
			flushBuffer();
		}

		*m_bufferPosition = leadingByteOf4(bits19to21(theChar));
		++m_bufferPosition;
		*m_bufferPosition = trailingByte(bits13to18(theChar));
		++m_bufferPosition;
		*m_bufferPosition = trailingByte(bits7to12(theChar));
		++m_bufferPosition;
		*m_bufferPosition = trailingByte(bits1to6(theChar));
		++m_bufferPosition;

		m_bufferRemaining -= 4;
	}
	else
	{
		XALAN_USING_XERCES(SAXException)

		const XalanDOMString	theMessage(TranscodeFromLocalCodePage("Invalid character detected: ") +
										   UnsignedLongToHexDOMString(theChar));

		throw SAXException(c_wstr(theMessage));
	}
}



inline void
FormatterToXML_UTF8::safeWriteContent(
			const XalanDOMChar*			theChars,
			XalanDOMString::size_type	theLength)
{
	for(size_type i = 0; i < theLength; ++i)
	{
		assert(isContentSpecial(theChars[i]) == false);

		write(char(theChars[i]));
	}
}



inline void
FormatterToXML_UTF8::write(
			const XalanDOMChar*			theChars,
			XalanDOMString::size_type	theLength)
{
	for(size_type i = 0; i < theLength; ++i)
	{
		write(theChars[i]);
	}
}



inline unsigned int
FormatterToXML_UTF8::decodeUTF16SurrogatePair(
			XalanDOMChar	theHighSurrogate,
			XalanDOMChar	theLowSurrogate)
{
	assert(isUTF16HighSurrogate(theHighSurrogate) == true);

	if (isUTF16LowSurrogate(theLowSurrogate) == false)
	{
		throwInvalidUTF16SurrogateException(theHighSurrogate, theLowSurrogate);
	}

	return ((theHighSurrogate - 0xD800u) << 10) + theLowSurrogate - 0xDC00u + 0x00010000u;
}



void
FormatterToXML_UTF8::writeName1_0(const XalanDOMChar*	theChars)
{
	for(const XalanDOMChar*	currentChar = theChars; *currentChar; ++currentChar)
	{
		assert(isUTF16HighSurrogate(*currentChar) == false);

		write(*currentChar);
	}
}



void
FormatterToXML_UTF8::writeName1_1(const XalanDOMChar*	theChars)
{
	const XalanDOMChar*		currentChar = theChars;
	const XalanDOMChar*		firstChar = theChars;

	while(*currentChar)
	{
		if (isUTF16HighSurrogate(*currentChar) == false)
		{
			++currentChar;
		}
		else
		{
			write(firstChar, currentChar - firstChar);

			const XalanDOMChar	high = *currentChar;
			const XalanDOMChar	low = *(++currentChar);

			write(decodeUTF16SurrogatePair(high, low));

			++currentChar;

			firstChar = currentChar;
		}
	}

	write(firstChar, currentChar - firstChar);
}



inline void
FormatterToXML_UTF8::write(
			const char*					theChars,
			XalanDOMString::size_type	theLength)
{
#if defined(NDEBUG)
	if (theLength > sizeof(m_buffer))
	{
		flushBuffer();

		m_writer->write(theChars, 0, theLength);
	}
	else
	{
		if (m_bufferRemaining < theLength)
		{
			flushBuffer();
		}

		for(size_type i = 0; i < theLength; ++i)
		{
			*m_bufferPosition = theChars[i];

			++m_bufferPosition;
		}

		m_bufferRemaining -= theLength;
	}
#else
	for(XalanDOMString::size_type i = 0; i < theLength; ++i)
	{
		write(theChars[i]);
	}
#endif
}



void
FormatterToXML_UTF8::outputDocTypeDecl(const XalanDOMChar* 	name)
{
	// "<!DOCTYPE "
	write(s_doctypeHeaderStartString, s_doctypeHeaderStartStringLength);

	write(name);

	if(length(m_doctypePublic) != 0)
	{
		// " PUBLIC \""
		write(s_doctypeHeaderPublicString, s_doctypeHeaderPublicStringLength);
		writeName(m_doctypePublic.c_str());
		write(char(XalanUnicode::charQuoteMark));
		write(char(XalanUnicode::charSpace));
		write(char(XalanUnicode::charQuoteMark));
	}
	else
	{
		// " SYSTEM \""
		write(s_doctypeHeaderSystemString, s_doctypeHeaderSystemStringLength);
	}

	writeName(m_doctypeSystem.c_str());
	write(char(XalanUnicode::charQuoteMark));
	write(char(XalanUnicode::charGreaterThanSign));

	outputLineSep();
}



XALAN_USING_XERCES(SAXException)

void
FormatterToXML_UTF8::throwInvalidUTF16SurrogateException(XalanDOMChar	ch)
{
	const XalanDOMString	theMessage(TranscodeFromLocalCodePage("Invalid UTF-16 surrogate detected: ") +
									   UnsignedLongToHexDOMString(ch) +
									   TranscodeFromLocalCodePage(" ?"));

	throw SAXException(c_wstr(theMessage));
}



void
FormatterToXML_UTF8::throwInvalidUTF16SurrogateException(
			XalanDOMChar	ch,
			XalanDOMChar	next)
{
	const XalanDOMString	theMessage(TranscodeFromLocalCodePage("Invalid UTF-16 surrogate detected: ") +
									   UnsignedLongToHexDOMString(ch) +
									   UnsignedLongToHexDOMString(next) +
									   TranscodeFromLocalCodePage(" ?"));

	throw SAXException(c_wstr(theMessage));
}



bool
FormatterToXML_UTF8::writeDefaultEntity(XalanDOMChar	ch)
{
	if (XalanUnicode::charLessThanSign == ch)
	{
		write(s_lessThanEntityString, s_lessThanEntityStringLength);
	}
	else if (XalanUnicode::charGreaterThanSign == ch)
	{
		write(s_greaterThanEntityString, s_greaterThanEntityStringLength);
	}
	else if (XalanUnicode::charAmpersand == ch)
	{
		write(s_ampersandEntityString, s_ampersandEntityStringLength);
	}
	else
	{
		return false;
	}

	return true;
}






XalanDOMString::size_type
FormatterToXML_UTF8::writeDefaultEscape(
			XalanDOMChar				ch,
			XalanDOMString::size_type	i,
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	len)
{
	if(!writeDefaultEntity(ch))
	{
		i = writeNormalizedChar(ch, chars, i, len);
	}

	return i;
}



bool
FormatterToXML_UTF8::writeDefaultAttributeEntity(XalanDOMChar	ch)
{
	if (writeDefaultEntity(ch) == true)
	{
		return true;
	}
	else if (XalanUnicode::charLF == ch) 
	{
		write(s_linefeedNCRString, s_linefeedNCRStringLength);
	}
	else if (XalanUnicode::charQuoteMark == ch) 
	{
		write(s_quoteEntityString, s_quoteEntityStringLength);
	}
	else
	{
		return false;
	}

	return true;
}



XalanDOMString::size_type
FormatterToXML_UTF8::writeDefaultAttributeEscape(
			XalanDOMChar				ch,
			XalanDOMString::size_type	i,
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	len)
{
	if(!writeDefaultAttributeEntity(ch))
	{
		i = writeNormalizedChar(ch, chars, i, len);
	}

	return i;
}



void
FormatterToXML_UTF8::flushWriter()
{
	m_writer->flush();
}



void
FormatterToXML_UTF8::setDocumentLocator(const LocatorType* const 	/* locator */)
{
}



void
FormatterToXML_UTF8::startDocument()
{
	m_needToOutputDocTypeDecl = true;

	if(m_shouldWriteXMLHeader == true)
	{
		// "<?xml version=\""
		write(s_xmlHeaderStartString, s_xmlHeaderStartStringLength);

		if (length(m_version) != 0)
		{
			write(m_version);
		}
		else
		{
			write(s_defaultVersionString, s_defaultVersionStringLength);
		}

		// "\" encoding=\""
		write(s_xmlHeaderEncodingString, s_xmlHeaderEncodingStringLength);

		write(s_utf8String);

		if (length(m_standalone) != 0)
		{
			write(s_xmlHeaderStandaloneString, s_xmlHeaderStandaloneStringLength);
			write(m_standalone);
		}

		write(s_xmlHeaderEndString, s_xmlHeaderEndStringLength);

		outputLineSep();
	}
}



void
FormatterToXML_UTF8::endDocument()
{
	flushBuffer();
}



void
FormatterToXML_UTF8::startElement(
			const XMLCh* const	name,
			AttributeListType&	attrs)
{
	if(true == m_needToOutputDocTypeDecl &&
	   isEmpty(m_doctypeSystem) == false)
	{
		outputDocTypeDecl(name);

		m_needToOutputDocTypeDecl = false;
	}

	writeParentTagEnd();

	write(char(XalanUnicode::charLessThanSign));
	writeName(name);

	const unsigned int	nAttrs = attrs.getLength();

	for (unsigned int i = 0;  i < nAttrs ;  i++)
	{
		processAttribute(attrs.getName(i), attrs.getValue(i));
	}

	// Flag the current element as not yet having any children.
	openElementForChildren();
}



void
FormatterToXML_UTF8::endElement(const XMLCh* const	name)
{
	const bool	hasChildNodes = childNodesWereAdded();

	if (hasChildNodes == true) 
	{
		write(char(XalanUnicode::charLessThanSign));
		write(char(XalanUnicode::charSolidus));
		writeName(name);
	}
	else
	{
		if(m_spaceBeforeClose == true)
		{
			write(char(XalanUnicode::charSpace));
		}

		write(char(XalanUnicode::charSolidus));
	}

	write(char(XalanUnicode::charGreaterThanSign));
}



void
FormatterToXML_UTF8::processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
{
	// Use a fairly nasty hack to tell if the next node is supposed to be 
	// unescaped text.
	if(equals(target, length(target), s_piTarget, s_piTargetLength) == true &&
	   equals(data, length(data), s_piData, s_piDataLength) == true)
	{
		m_nextIsRaw = true;
	}
	else	
	{
		writeParentTagEnd();

		write(char(XalanUnicode::charLessThanSign));
		write(char(XalanUnicode::charQuestionMark));
		writeName(target);

		const XalanDOMString::size_type		len = length(data);

		if ( len > 0 && !isXMLWhitespace(data[0]))
		{
			write(char(XalanUnicode::charSpace));
		}

		writeNormalizedPIData(data, len);

		write(char(XalanUnicode::charQuestionMark));
		write(char(XalanUnicode::charGreaterThanSign));

		// If outside of an element, then put in a new line.  This whitespace
		// is not significant.
		if (m_elemStack.empty() == true)
		{
			outputLineSep();
		}
	}
}



void
FormatterToXML_UTF8::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if(length != 0)
	{
		if(m_nextIsRaw)
		{
			m_nextIsRaw = false;

			charactersRaw(chars, length);
		}
		else
		{
			writeParentTagEnd();

			unsigned int	i = 0;
			unsigned int	firstIndex = 0;

			while(i < length) 
			{
				const XalanDOMChar	ch = chars[i];

				if (isContentSpecial(ch) == false)
				{
					++i;
				}
				else
				{
					safeWriteContent(chars + firstIndex, i - firstIndex);

					i = writeDefaultEscape(chars[i], i, chars, length);

					++i;

					firstIndex = i;
				}
			}

			safeWriteContent(chars + firstIndex, i - firstIndex);
		}
	}
}



void
FormatterToXML_UTF8::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	writeParentTagEnd();

	write(chars, length);
}



Writer*
FormatterToXML_UTF8::getWriter() const
{
	return m_writer;
}



const XalanDOMString&
FormatterToXML_UTF8::getDoctypeSystem() const
{
	return m_doctypeSystem;
}



const XalanDOMString&
FormatterToXML_UTF8::getDoctypePublic() const
{
	return m_doctypePublic;
}



const XalanDOMString&
FormatterToXML_UTF8::getEncoding() const
{
	return s_utf8String;
}



const XalanDOMString&
FormatterToXML_UTF8::getMediaType() const
{
	return m_mediaType;
}



void
FormatterToXML_UTF8::writeAttrString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theStringLength)
{
	assert(theString != 0);

	XalanDOMString::size_type	i = 0;
	XalanDOMString::size_type	firstIndex = 0;

    while(i < theStringLength)
    {
		const XalanDOMChar	ch = theString[i];

		if (isAttributeSpecial(ch) == false)
		{
			++i;
		}
		else
		{
			safeWriteContent(theString + firstIndex, i - firstIndex);

			i = writeDefaultAttributeEscape(ch, i, theString, theStringLength);

			++i;

			firstIndex = i;
		}
    }

	safeWriteContent(theString + firstIndex, i - firstIndex);
}



void
FormatterToXML_UTF8::writeCommentData(const XalanDOMChar*	data)
{
	const XalanDOMString::size_type		len = length(data);
	XalanDOMChar						previousChar = 0;

	for (XalanDOMString::size_type i = 0; i < len; ++i)
	{
		const XalanDOMChar	currentChar = data[i];

		if (currentChar == XalanUnicode::charHyphenMinus &&
			previousChar == XalanUnicode::charHyphenMinus)
		{
			write(char(XalanUnicode::charSpace));
		}

		write(currentChar);

		previousChar = currentChar;
	}
		
	if (previousChar == XalanUnicode::charHyphenMinus)
	{
		write(char(XalanUnicode::charSpace));
	}
}



XalanDOMString::size_type
FormatterToXML_UTF8::writeNormalizedChar(
			XalanDOMChar				ch,
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length)
{
	if (XalanUnicode::charLF == ch)
	{
		outputLineSep();
	}
	else if (isUTF16HighSurrogate(ch) == true)
	{
		if (start + 1 >= length)
		{
			throwInvalidUTF16SurrogateException(ch);
		}
		else 
		{
			write(decodeUTF16SurrogatePair(ch, chars[++start]));
		}
	}
	else
	{
		write(ch);
	}

	return start;
}



void
FormatterToXML_UTF8::writeCDATAChars(
			const XalanDOMChar			ch[],
			XalanDOMString::size_type	length)
{
	XalanDOMString::size_type i = 0;

	while(i < length)
    {
		// If "]]>", which would close the CDATA appears in
		// the content, we have to put the first two characters
		// in the CDATA section, close the CDATA section, then
		// open a new one and add the last character.
		if (i < length - 2 &&
			XalanUnicode::charRightSquareBracket == ch[i] &&
            XalanUnicode::charRightSquareBracket == ch[i + 1] &&
			XalanUnicode::charGreaterThanSign == ch[ i + 2])
		{
			// "]]]]><![CDATA[>"
			write(char(XalanUnicode::charRightSquareBracket));
			write(char(XalanUnicode::charRightSquareBracket));

			write(s_cdataCloseString, s_cdataCloseStringLength);
			write(s_cdataOpenString, s_cdataOpenStringLength);

			write(char(XalanUnicode::charGreaterThanSign));

			i += 3;
		}
		else
		{
			i = writeNormalizedChar(ch[i], ch, i, length);

			++i;
		}
    }
}



void
FormatterToXML_UTF8::entityReference(const XMLCh* const	name)
{
	writeParentTagEnd();
	  
	write(char(XalanUnicode::charAmpersand));
	writeName(name);
	write(char(XalanUnicode::charSemicolon));
}



void
FormatterToXML_UTF8::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if (length > 0)
	{
		characters(chars, length);
	}
}



void
FormatterToXML_UTF8::resetDocument()
{
	// I don't do anything with this yet.
}



void
FormatterToXML_UTF8::comment(const XMLCh* const	data)
{
	writeParentTagEnd();

	write(char(XalanUnicode::charLessThanSign));
	write(char(XalanUnicode::charExclamationMark));
	write(char(XalanUnicode::charHyphenMinus));
	write(char(XalanUnicode::charHyphenMinus));

	writeCommentData(data);

	write(char(XalanUnicode::charHyphenMinus));
	write(char(XalanUnicode::charHyphenMinus));
	write(char(XalanUnicode::charGreaterThanSign));
}



void
FormatterToXML_UTF8::cdata(
			const XMLCh* const	ch,
			const unsigned int	length)
{
	if(m_nextIsRaw == true)
	{
		m_nextIsRaw = false;

		charactersRaw(ch, length);
	}
	else
	{
		writeParentTagEnd();

		if(length > 0)
		{
			write(s_cdataOpenString, s_cdataOpenStringLength);

			writeCDATAChars(ch, length);

			write(s_cdataCloseString, s_cdataCloseStringLength);
		}
	}
}



void
FormatterToXML_UTF8::writeParentTagEnd()
{
	if(!m_elemStack.empty())
	{
		// See if the parent element has already been flagged as having children.
		if(false == m_elemStack.back())
		{
			write(char(XalanUnicode::charGreaterThanSign));

			m_elemStack.back() = true;
		}
	}
}



void
FormatterToXML_UTF8::openElementForChildren()
{
	m_elemStack.push_back(false);
}



bool
FormatterToXML_UTF8::childNodesWereAdded()
{
	bool	fResult = false;

	if (m_elemStack.empty() == false)
	{
		fResult = m_elemStack.back();

		m_elemStack.pop_back();
	}

	return fResult;
}



void
FormatterToXML_UTF8::processAttribute(
			const XalanDOMChar* 	name,
			const XalanDOMChar* 	value)
{
	// We add a fake attribute to the source tree to
	// declare the xml prefix, so we filter it back out
	// here...
	// $$$ ToDo: It would be better if we didn't have to do
	// this here.
	if (equals(name, DOMServices::s_XMLNamespacePrefix) == false)
	{
		write(char(XalanUnicode::charSpace));
		writeName(name);
		write(char(XalanUnicode::charEqualsSign));
		write(char(XalanUnicode::charQuoteMark));
		writeAttrString(value, length(value));
		write(char(XalanUnicode::charQuoteMark));
	}
}



void
FormatterToXML_UTF8::outputLineSep()
{
	assert(m_newlineString != 0 && length(m_newlineString) == m_newlineStringLength);

	write(m_newlineString, m_newlineStringLength);
}



void
FormatterToXML_UTF8::writeNormalizedPIData(
			const XalanDOMChar*			theData,
			XalanDOMString::size_type	theLength)
{
	// If there are any "?>" pairs in the string,
	// we have to normalize them to "? >", so they
	// won't be confused with the end tag.

	for (XalanDOMString::size_type i = 0; i < theLength; ++i)
	{
		const XalanDOMChar	theChar = theData[i];

		if (theChar == XalanUnicode::charQuestionMark &&
			i + 1 < theLength &&
			theData[i + 1] == XalanUnicode::charGreaterThanSign)
		{
			write(char(XalanUnicode::charQuestionMark));
			write(char(XalanUnicode::charSpace));
		}
		else
		{
			write(theChar);
		}
	}
}



static XalanDOMString	s_localUTF8String;



const XalanDOMString&	FormatterToXML_UTF8::s_utf8String = s_localUTF8String;



void
FormatterToXML_UTF8::initialize()
{
	s_localUTF8String = XalanTranscodingServices::s_utf8String;
}



void
FormatterToXML_UTF8::terminate()
{
	XalanDOMString().swap(s_localUTF8String);
}



#define FXML_SIZE(str)	((sizeof(str) / sizeof(str[0]) - 1))

const char	FormatterToXML_UTF8::s_doctypeHeaderStartString[] =
{
	char(XalanUnicode::charLessThanSign),
	char(XalanUnicode::charExclamationMark),
	char(XalanUnicode::charLetter_D),
	char(XalanUnicode::charLetter_O),
	char(XalanUnicode::charLetter_C),
	char(XalanUnicode::charLetter_T),
	char(XalanUnicode::charLetter_Y),
	char(XalanUnicode::charLetter_P),
	char(XalanUnicode::charLetter_E),
	char(XalanUnicode::charSpace),
	char(0)
};

const XalanDOMString::size_type	FormatterToXML_UTF8::s_doctypeHeaderStartStringLength =
		FXML_SIZE(s_doctypeHeaderStartString);

const char	FormatterToXML_UTF8::s_doctypeHeaderPublicString[] =
{
	char(XalanUnicode::charSpace),
	char(XalanUnicode::charLetter_P),
	char(XalanUnicode::charLetter_U),
	char(XalanUnicode::charLetter_B),
	char(XalanUnicode::charLetter_L),
	char(XalanUnicode::charLetter_I),
	char(XalanUnicode::charLetter_C),
	char(XalanUnicode::charSpace),
	char(XalanUnicode::charQuoteMark),
	char(0)
};

const XalanDOMString::size_type	FormatterToXML_UTF8::s_doctypeHeaderPublicStringLength =
		FXML_SIZE(s_doctypeHeaderPublicString);

const char	FormatterToXML_UTF8::s_doctypeHeaderSystemString[] =
{
	char(XalanUnicode::charSpace),
	char(XalanUnicode::charLetter_S),
	char(XalanUnicode::charLetter_Y),
	char(XalanUnicode::charLetter_S),
	char(XalanUnicode::charLetter_T),
	char(XalanUnicode::charLetter_E),
	char(XalanUnicode::charLetter_M),
	char(XalanUnicode::charSpace),
	char(XalanUnicode::charQuoteMark),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_doctypeHeaderSystemStringLength =
		FXML_SIZE(s_doctypeHeaderSystemString);

const char	FormatterToXML_UTF8::s_xmlHeaderStartString[] =
{
	char(XalanUnicode::charLessThanSign),
	char(XalanUnicode::charQuestionMark),
	char(XalanUnicode::charLetter_x),
	char(XalanUnicode::charLetter_m),
	char(XalanUnicode::charLetter_l),
	char(XalanUnicode::charSpace),
	char(XalanUnicode::charLetter_v),
	char(XalanUnicode::charLetter_e),
	char(XalanUnicode::charLetter_r),
	char(XalanUnicode::charLetter_s),
	char(XalanUnicode::charLetter_i),
	char(XalanUnicode::charLetter_o),
	char(XalanUnicode::charLetter_n),
	char(XalanUnicode::charEqualsSign),
	char(XalanUnicode::charQuoteMark),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_xmlHeaderStartStringLength =
		FXML_SIZE(s_xmlHeaderStartString);

const char	FormatterToXML_UTF8::s_xmlHeaderEncodingString[] =
{
	char(XalanUnicode::charQuoteMark),
	char(XalanUnicode::charSpace),
	char(XalanUnicode::charLetter_e),
	char(XalanUnicode::charLetter_n),
	char(XalanUnicode::charLetter_c),
	char(XalanUnicode::charLetter_o),
	char(XalanUnicode::charLetter_d),
	char(XalanUnicode::charLetter_i),
	char(XalanUnicode::charLetter_n),
	char(XalanUnicode::charLetter_g),
	char(XalanUnicode::charEqualsSign),
	char(XalanUnicode::charQuoteMark),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_xmlHeaderEncodingStringLength =
		FXML_SIZE(s_xmlHeaderEncodingString);

const char	FormatterToXML_UTF8::s_xmlHeaderStandaloneString[] =
{
	char(XalanUnicode::charQuoteMark),
	char(XalanUnicode::charSpace),
	char(XalanUnicode::charLetter_s),
	char(XalanUnicode::charLetter_t),
	char(XalanUnicode::charLetter_a),
	char(XalanUnicode::charLetter_n),
	char(XalanUnicode::charLetter_d),
	char(XalanUnicode::charLetter_a),
	char(XalanUnicode::charLetter_l),
	char(XalanUnicode::charLetter_o),
	char(XalanUnicode::charLetter_n),
	char(XalanUnicode::charLetter_e),
	char(XalanUnicode::charEqualsSign),
	char(XalanUnicode::charQuoteMark),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_xmlHeaderStandaloneStringLength =
		FXML_SIZE(s_xmlHeaderStandaloneString);

const char	FormatterToXML_UTF8::s_xmlHeaderEndString[] =
{
	char(XalanUnicode::charQuoteMark),
	char(XalanUnicode::charQuestionMark),
	char(XalanUnicode::charGreaterThanSign),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_xmlHeaderEndStringLength =
		FXML_SIZE(s_xmlHeaderEndString);

const char	FormatterToXML_UTF8::s_defaultVersionString[] =
{
	char(XalanUnicode::charDigit_1),
	char(XalanUnicode::charFullStop),
	char(XalanUnicode::charDigit_0),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_defaultVersionStringLength =
		FXML_SIZE(s_defaultVersionString);

const char	FormatterToXML_UTF8::s_cdataOpenString[] =
{
	char(XalanUnicode::charLessThanSign),
	char(XalanUnicode::charExclamationMark),
	char(XalanUnicode::charLeftSquareBracket),
	char(XalanUnicode::charLetter_C),
	char(XalanUnicode::charLetter_D),
	char(XalanUnicode::charLetter_A),
	char(XalanUnicode::charLetter_T),
	char(XalanUnicode::charLetter_A),
	char(XalanUnicode::charLeftSquareBracket),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_cdataOpenStringLength =
		FXML_SIZE(s_cdataOpenString);

const char	FormatterToXML_UTF8::s_cdataCloseString[] =
{
	char(XalanUnicode::charRightSquareBracket),
	char(XalanUnicode::charRightSquareBracket),
	char(XalanUnicode::charGreaterThanSign),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_cdataCloseStringLength =
		FXML_SIZE(s_cdataCloseString);


const XalanDOMChar	FormatterToXML_UTF8::s_xhtmlDocTypeString[] =
{
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_W,
	XalanUnicode::charDigit_3,
	XalanUnicode::charLetter_C,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_D,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_D,
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_X,
	XalanUnicode::charLetter_H,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_M,
	XalanUnicode::charLetter_L,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_xhtmlDocTypeStringLength =
		FXML_SIZE(s_xhtmlDocTypeString);

const char	FormatterToXML_UTF8::s_lessThanEntityString[] =
{
	char(XalanUnicode::charAmpersand),
	char(XalanUnicode::charLetter_l),
	char(XalanUnicode::charLetter_t),
	char(XalanUnicode::charSemicolon),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_lessThanEntityStringLength =
		FXML_SIZE(s_lessThanEntityString);

const char	FormatterToXML_UTF8::s_greaterThanEntityString[] =
{
	char(XalanUnicode::charAmpersand),
	char(XalanUnicode::charLetter_g),
	char(XalanUnicode::charLetter_t),
	char(XalanUnicode::charSemicolon),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_greaterThanEntityStringLength =
		FXML_SIZE(s_greaterThanEntityString);

const char	FormatterToXML_UTF8::s_ampersandEntityString[] =
{
	char(XalanUnicode::charAmpersand),
	char(XalanUnicode::charLetter_a),
	char(XalanUnicode::charLetter_m),
	char(XalanUnicode::charLetter_p),
	char(XalanUnicode::charSemicolon),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_ampersandEntityStringLength =
		FXML_SIZE(s_ampersandEntityString);

const char	FormatterToXML_UTF8::s_quoteEntityString[] =
{
	char(XalanUnicode::charAmpersand),
	char(XalanUnicode::charLetter_q),
	char(XalanUnicode::charLetter_u),
	char(XalanUnicode::charLetter_o),
	char(XalanUnicode::charLetter_t),
	char(XalanUnicode::charSemicolon),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_quoteEntityStringLength =
		FXML_SIZE(s_quoteEntityString);

const char	FormatterToXML_UTF8::s_linefeedNCRString[] =
{
	char(XalanUnicode::charAmpersand),
	char(XalanUnicode::charNumberSign),
	char(XalanUnicode::charDigit_1),
	char(XalanUnicode::charDigit_0),
	char(XalanUnicode::charSemicolon),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_linefeedNCRStringLength =
		FXML_SIZE(s_linefeedNCRString);



XALAN_CPP_NAMESPACE_END
