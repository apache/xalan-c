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


// Class header file.
#include "FormatterToXML_UTF8.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/Writer.hpp>
#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FormatterToXML_UTF8::FormatterToXML_UTF8(
			Writer&					writer,
			const XalanDOMString&	version,
			const XalanDOMString&	mediaType,
			const XalanDOMString&	doctypeSystem,
			const XalanDOMString&	doctypePublic,
			bool					xmlDecl,
			const XalanDOMString&	standalone) :
	FormatterToXMLBase(
		writer,
		version,
		mediaType,
		doctypeSystem,
		doctypePublic,
		xmlDecl,
		standalone),
	m_nameFunction(0),
	m_buffer(),
	m_bufferPosition(m_buffer),
	m_bufferRemaining(kBufferSize)
{
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
	return char((theChar >> 12) & 0x3F);
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
		throwInvalidCharacterException(theChar);
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
			write(
                firstChar,
                XalanDOMString::size_type(currentChar - firstChar));

			const XalanDOMChar	high = *currentChar;
			const XalanDOMChar	low = *(++currentChar);

			write(decodeUTF16SurrogatePair(high, low));

			++currentChar;

			firstChar = currentChar;
		}
	}

	write(
        firstChar,
        XalanDOMString::size_type(currentChar - firstChar));
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
FormatterToXML_UTF8::writeDoctypeDecl(const XalanDOMChar* 	name)
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

	outputNewline();
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
	else if (XalanUnicode::charCR == ch) 
	{
		write(s_carriageReturnNCRString, s_carriageReturnNCRStringLength);
	}
	else if (XalanUnicode::charQuoteMark == ch) 
	{
		write(s_quoteEntityString, s_quoteEntityStringLength);
	}
	else if (XalanUnicode::charHTab == ch) 
	{
		write(s_htabNCRString, s_htabNCRStringLength);
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
FormatterToXML_UTF8::writeParentTagEnd()
{
	if(markParentForChildren() == true)
	{
		write(char(XalanUnicode::charGreaterThanSign));
	}
}



void
FormatterToXML_UTF8::startElement(
			const XMLCh* const	name,
			AttributeListType&	attrs)
{
	generateDoctypeDecl(name);

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
FormatterToXML_UTF8::writeProcessingInstruction(
			const XMLCh*	target,
			const XMLCh*	data)
{
	writeParentTagEnd();

	write(char(XalanUnicode::charLessThanSign));
	write(char(XalanUnicode::charQuestionMark));
	writeName(target);

	const XalanDOMString::size_type		len = length(data);

	// We need to make sure there is a least one whitespace character
	// between the target and the data.
	if ( len > 0 && !isXMLWhitespace(data[0]))
	{
		write(char(XalanUnicode::charSpace));
	}

	writeNormalizedPIData(data, len);

	write(char(XalanUnicode::charQuestionMark));
	write(char(XalanUnicode::charGreaterThanSign));

	// If outside of an element, then put in a new line.  This whitespace
	// is not significant.
	if (outsideDocumentElement() == true)
	{
		outputNewline();
	}
}



void
FormatterToXML_UTF8::writeCharacters(
			const XMLCh*	chars,
			unsigned int	length)
{
	assert(length != 0);

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



void
FormatterToXML_UTF8::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	writeParentTagEnd();

	write(chars, length);
}



const XalanDOMString&
FormatterToXML_UTF8::getEncoding() const
{
	return s_utf8String;
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
		outputNewline();
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

	// enum for a cheezy little state machine.
	enum eState { eInitialState, eFirstRightSquareBracket, eSecondRightSquareBracket };

	eState	theCurrentState = eInitialState;

	while(i < length)
    {
		// If "]]>", which would close the CDATA appears in
		// the content, we have to put the first two characters
		// in the CDATA section, close the CDATA section, then
		// open a new one and add the last character.

		const XalanDOMChar	theChar = ch[i];

		if (theChar == XalanUnicode::charRightSquareBracket)
		{
			if (theCurrentState == eInitialState)
			{
				theCurrentState = eFirstRightSquareBracket;
			}
			else if (theCurrentState == eFirstRightSquareBracket)
			{
				theCurrentState = eSecondRightSquareBracket;
			}

			write(char(XalanUnicode::charRightSquareBracket));
		}
		else if (theChar == XalanUnicode::charGreaterThanSign)
		{
			if (theCurrentState != eInitialState)
			{
				if (theCurrentState == eFirstRightSquareBracket)
				{
					theCurrentState = eInitialState;
				}
				else
				{
					theCurrentState = eInitialState;

					write(s_cdataCloseString, s_cdataCloseStringLength);
					write(s_cdataOpenString, s_cdataOpenStringLength);
				}
			}

			write(char(XalanUnicode::charGreaterThanSign));
		}
		else
		{
			if (theCurrentState != eInitialState)
			{
				theCurrentState = eInitialState;
			}

			i = writeNormalizedChar(theChar, ch, i, length);
		}

		++i;
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
FormatterToXML_UTF8::writeCDATA(
			const XMLCh*	ch,
			unsigned int	length)
{
	assert(length != 0);

	writeParentTagEnd();

	write(s_cdataOpenString, s_cdataOpenStringLength);

	writeCDATAChars(ch, length);

	write(s_cdataCloseString, s_cdataCloseStringLength);
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
FormatterToXML_UTF8::writeXMLHeader()
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
}



void
FormatterToXML_UTF8::outputNewline()
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

const char	FormatterToXML_UTF8::s_carriageReturnNCRString[] =
{
	char(XalanUnicode::charAmpersand),
	char(XalanUnicode::charNumberSign),
	char(XalanUnicode::charDigit_1),
	char(XalanUnicode::charDigit_3),
	char(XalanUnicode::charSemicolon),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_carriageReturnNCRStringLength =
		FXML_SIZE(s_carriageReturnNCRString);

const char	FormatterToXML_UTF8::s_htabNCRString[] =
{
	char(XalanUnicode::charAmpersand),
	char(XalanUnicode::charNumberSign),
	char(XalanUnicode::charDigit_9),
	char(XalanUnicode::charSemicolon),
	char(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF8::s_htabNCRStringLength =
		FXML_SIZE(s_htabNCRString);



XALAN_CPP_NAMESPACE_END
