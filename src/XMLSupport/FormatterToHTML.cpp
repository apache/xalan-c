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

 /**
 * @author David N. Bertoni <david_n_bertoni@lotus.com>
 */



// Class header file.
#include "FormatterToHTML.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <Include/XalanAutoPtr.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/PrefixResolver.hpp>
#include <PlatformSupport/Writer.hpp>
#include <PlatformSupport/XalanTranscodingServices.hpp>
#include <PlatformSupport/XalanUnicode.hpp>
#include <PlatformSupport/XalanXMLChar.hpp>



#include <DOMSupport/DOMServices.hpp>



const XalanDOMString	FormatterToHTML::s_emptyString;



FormatterToHTML::FormatterToHTML(
			Writer&					writer,
			const XalanDOMString&	encoding, 
			const XalanDOMString&	mediaType,
			const XalanDOMString&	doctypeSystem,
			const XalanDOMString&	doctypePublic,
			bool					doIndent,
			int						indent,
			bool					escapeURLs,
			bool					omitMetaTag) :
	FormatterToXML(
			writer,
			s_emptyString,
			doIndent,
			indent,
			encoding,
			mediaType,
			doctypeSystem,
			doctypePublic,
			false,
			s_emptyString,
			OUTPUT_METHOD_HTML),
	m_currentElementName(),
	m_inBlockElem(false),
	m_isRawStack(),
	m_isScriptOrStyleElem(false),
	m_inScriptElemStack(),
	m_escapeURLs(escapeURLs),
	m_isFirstElement(false),
	m_isUTF8(XalanTranscodingServices::encodingIsUTF8(m_encoding)),
	m_elementLevel(0),
	m_hasNamespaceStack(),
	m_omitMetaTag(omitMetaTag),
	m_elementPropertiesStack()
{
	initCharsMap();
}



FormatterToHTML::~FormatterToHTML()
{
}



void
FormatterToHTML::initAttrCharsMap()
{
	FormatterToXML::initAttrCharsMap();

	m_attrCharsMap[XalanUnicode::charLF] = 'S';

	// These should _not_ be escaped...
	m_attrCharsMap[XalanUnicode::charLessThanSign] = 0;
	m_attrCharsMap[XalanUnicode::charGreaterThanSign] = 0;

	for(XalanDOMString::size_type i = 160; i < SPECIALSSIZE; i++)
	{
		m_attrCharsMap[i] = 'S';
	}
}



void
FormatterToHTML::initCharsMap()
{
	initAttrCharsMap();

#if defined(XALAN_STRICT_ANSI_HEADERS)
	std::memset(m_charsMap, 0, sizeof(m_charsMap));
#else
	memset(m_charsMap, 0, sizeof(m_charsMap));
#endif

	m_charsMap[XalanUnicode::charLF] = 'S';
	m_charsMap[XalanUnicode::charLessThanSign] = 'S';
	m_charsMap[XalanUnicode::charGreaterThanSign] = 'S';
	m_charsMap[XalanUnicode::charAmpersand] = 'S';

#if defined(XALAN_STRICT_ANSI_HEADERS)
	std::memset(m_charsMap, 'S', 10);
#else
	memset(m_charsMap, 'S', 10);
#endif

	m_charsMap[0x0A] = 'S';
	m_charsMap[0x0D] = 'S';

	for(int i = 160; i < SPECIALSSIZE; ++i)
	{
		m_charsMap[i] = 'S';
	}

    for(int j = m_maxCharacter; j < SPECIALSSIZE; ++j)
    {
		m_charsMap[j] = 'S';
    }
}



void
FormatterToHTML::startDocument()
{
	// Clear the buffer, just in case...
	clear(m_stringBuffer);

	// Reset this, just in case...
	m_elementLevel = 0;

	m_isFirstElement = true;
    m_startNewLine = false;
	m_shouldWriteXMLHeader = false;
	m_isScriptOrStyleElem = false;

	m_isRawStack.clear();
	m_inScriptElemStack.push_back(false);
	m_hasNamespaceStack.clear();
	m_elementPropertiesStack.clear();

	const bool	isEmptySystem = isEmpty(m_doctypeSystem);

	const bool	isEmptyPublic = isEmpty(m_doctypePublic);

	// Output the header if either the System or Public attributes are
	// specified
	if(isEmptySystem == false || isEmptyPublic == false)
	{
		accumContent(s_doctypeHeaderStartString);

		if(isEmptyPublic == false)
		{
			accumContent(s_doctypeHeaderPublicString);
			accumContent(m_doctypePublic);
			accumContent(XalanUnicode::charQuoteMark);
		}

		if(isEmptySystem == false)
		{
			if(isEmptyPublic == true)
			{
				accumContent(s_doctypeHeaderSystemString);
			}

			accumContent(XalanUnicode::charSpace);
			accumContent(XalanUnicode::charQuoteMark);

			accumContent(m_doctypeSystem);
			accumContent(XalanUnicode::charQuoteMark);
		}

		accumContent(XalanUnicode::charGreaterThanSign);

		outputLineSep();
	}

	m_needToOutputDocTypeDecl = false;
}



void
FormatterToHTML::endDocument()
{
	assert(m_elementLevel == 0);

	m_inScriptElemStack.pop_back();

	FormatterToXML::endDocument();
}



void
FormatterToHTML::startElement(
			const XMLCh* const	name,
			AttributeList&		attrs)
{
	if (pushHasNamespace(name) == true)
	{
		FormatterToXML::startElement(name, attrs);
	}
	else
	{
		writeParentTagEnd();

		const XalanHTMLElementsProperties::ElementProperties&	elemProperties =
			XalanHTMLElementsProperties::find(name);
		assert(elemProperties.null() == false);

		// Push a copy onto the stack for endElement().  Don't worry --
		// the copy is cheap!
		m_elementPropertiesStack.push_back(elemProperties);

		const bool	isBlockElement = elemProperties.is(XalanHTMLElementsProperties::BLOCK);

		if (equalsIgnoreCaseASCII(name, c_wstr(s_scriptString)) == true)
		{
			m_isScriptOrStyleElem = true;

			m_inScriptElemStack.push_back(true);
		}
		else
		{
			if (equalsIgnoreCaseASCII(name, c_wstr(s_styleString)) == true)
			{
				m_isScriptOrStyleElem = true;
			}

			m_inScriptElemStack.push_back(m_inScriptElemStack.back());
		}

		// Increment the level...
		++m_elementLevel;

		if(m_ispreserve == true)
		{
			m_ispreserve = false;
		}
		else if(m_doIndent &&
				m_elementLevel > 0 && m_isFirstElement == false &&
				(m_inBlockElem == false || isBlockElement == true))
		{
			m_startNewLine = true;

			indent(m_currentIndent);
		}

		m_inBlockElem = !isBlockElement;

		m_isRawStack.push_back(elemProperties.is(XalanHTMLElementsProperties::RAW));

		accumContent(XalanUnicode::charLessThanSign);

		accumName(name);

		const unsigned int	nAttrs = attrs.getLength();

		for (unsigned int i = 0;  i < nAttrs ;  i++)
		{
			processAttribute(attrs.getName(i), attrs.getValue(i), elemProperties);
		}

		// Flag the current element as not yet having any children.
		openElementForChildren();

		m_currentIndent += m_indent;
    
		m_isprevtext = false;

		if (elemProperties.is(XalanHTMLElementsProperties::HEADELEM) == true)
		{
			writeParentTagEnd();

			if (m_omitMetaTag == false)
			{
				if (m_doIndent)
					indent(m_currentIndent);

				accumContent(s_metaString);
				accumContent(getEncoding());      
				accumContent(XalanUnicode::charQuoteMark);
				accumContent(XalanUnicode::charGreaterThanSign);
			}
		}

		// We've written the first element, so turn off the flag...
		if (m_isFirstElement == true)
		{
			m_isFirstElement = false;
		}

		assert(m_elementLevel > 0);
	}
}



void
FormatterToHTML::endElement(const XMLCh* const	name)
{
	if (popHasNamespace() == true)
	{
		FormatterToXML::endElement(name);
	}
	else
	{
		m_currentIndent -= m_indent;

		const bool	hasChildNodes = childNodesWereAdded();

		assert(m_isRawStack.empty() == false);
		assert(m_inScriptElemStack.empty() == false);
		assert(m_elementPropertiesStack.empty() == false);

		m_isRawStack.pop_back();
		m_inScriptElemStack.pop_back();

		const XalanHTMLElementsProperties::ElementProperties	elemProperties =
				m_elementPropertiesStack.back();
		assert(elemProperties.null() == false);

		m_elementPropertiesStack.pop_back();

		const bool	isBlockElement = elemProperties.is(XalanHTMLElementsProperties::BLOCK);

		bool shouldIndent = false;

		if(m_ispreserve == true)
		{
			m_ispreserve = false;
		}
		else if(m_doIndent == true && (m_inBlockElem == false || isBlockElement == true))
		{
			m_startNewLine = true;

			shouldIndent = true;
		}

		m_inBlockElem = !isBlockElement;

		if (hasChildNodes) 
		{
			if (shouldIndent == true)
			{
				indent(m_currentIndent);
			}

			accumContent(XalanUnicode::charLessThanSign);
			accumContent(XalanUnicode::charSolidus);
			accumName(name);
			accumContent(XalanUnicode::charGreaterThanSign);
		}
		else
		{
			if(elemProperties.is(XalanHTMLElementsProperties::EMPTY) == false)
			{
				accumContent(XalanUnicode::charGreaterThanSign);

				accumContent(XalanUnicode::charLessThanSign);
				accumContent(XalanUnicode::charSolidus);
				accumName(name);
				accumContent(XalanUnicode::charGreaterThanSign);
			}
			else
			{
				accumContent(XalanUnicode::charGreaterThanSign);
			}
		}

		if (elemProperties.is(XalanHTMLElementsProperties::WHITESPACESENSITIVE) == true)
		{
			m_ispreserve = true;
		}

		if (hasChildNodes == true)
		{
			if (m_preserves.empty() == false)
			{
				m_preserves.pop_back();
			}
		}

		m_isprevtext = false;

		// Decrement the level...
		--m_elementLevel;
	}
}



void
FormatterToHTML::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if(length != 0)
	{
		if(m_inCData == true)
		{
			cdata(chars, length);
		}
		else if(m_nextIsRaw)
		{
			m_nextIsRaw = false;

			charactersRaw(chars, length);
		}
		else if (m_inScriptElemStack.back() == true)
		{
			charactersRaw(chars, length);
		}
		else if (m_isRawStack.empty() == false &&
				 m_isRawStack.back() == true)
		{
			writeParentTagEnd();

			m_ispreserve = true;

			if (shouldIndent() == true)
			{
				indent(m_currentIndent);
			}

			writeNormalizedChars(chars, 0, length, false);
		}
		else
		{
			writeParentTagEnd();

			m_ispreserve = true;

			writeCharacters(chars, length);
		}
	}

	if (m_isprevtext == false)
	{
		m_isprevtext = true;
	}
}



bool
FormatterToHTML::accumDefaultEntity(
		XalanDOMChar				ch,
		XalanDOMString::size_type	i,
		const XalanDOMChar			chars[],
		XalanDOMString::size_type	len,
		bool						escLF)
{
	assert(ch != 0);

	if(FormatterToXML::accumDefaultEntity(ch, i, chars, len, escLF) == true)
	{
		return true;
	}
	else
	{
		// Find the entity, if any...
		const Entity*	theFirst = s_entities;
		const Entity*	theLast = s_lastEntity;

		while(theFirst <= theLast)
		{
			const Entity* const		theCurrent = theFirst + (theLast - theFirst) / 2;
			assert(theCurrent->m_char != 0);

			if (ch < theCurrent->m_char)
			{
				theLast = theCurrent - 1;
			}
			else if (ch > theCurrent->m_char)
			{
				theFirst = theCurrent + 1;
			}
			else
			{
				copyEntityIntoBuffer(theCurrent->m_string, theCurrent->m_length);

				return true;
			}
		}

		return false;
	}
}



void
FormatterToHTML::entityReference(const XMLCh* const		name)
{
	accumContent(XalanUnicode::charAmpersand);
	accumName(name);
	accumContent(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::cdata(
			const XMLCh* const	ch,
			const unsigned int 	length)
{
	if(m_isScriptOrStyleElem == true)
	{
		writeParentTagEnd();

		m_ispreserve = true;

		if (shouldIndent() == true)
		{
			indent(m_currentIndent);
		}

		writeNormalizedChars(ch, 0, length, true);
	}
	else if(m_stripCData == true)
	{
		writeParentTagEnd();

		m_ispreserve = true;

		if (shouldIndent() == true)
		{
			indent(m_currentIndent);
		}

		accumContent(ch, 0, length);
	}
	else
	{
		FormatterToXML::cdata(ch, length);
	}
}



void
FormatterToHTML::processingInstruction(
		const XMLCh* const	target,
		const XMLCh* const	data)

{
	// Use a fairly nasty hack to tell if the next node is supposed to be 
	// unescaped text.
	if(equals(target, s_xsltNextIsRawString) == true &&
	   equals(data, s_formatterListenerString) == true)
	{
		m_nextIsRaw = true;
	}
	else
	{
		writeParentTagEnd();

		if (shouldIndent() == true)
		{
			indent(m_currentIndent);
		}

		accumContent(XalanUnicode::charLessThanSign);
		accumContent(XalanUnicode::charQuestionMark);
		accumName(target);

		if (length(data) > 0)
		{
			if(isXMLWhitespace(data[0]) == false)
			{
				accumContent(XalanUnicode::charSpace);
			}

			writeCharacters(data);
		}

		accumContent(XalanUnicode::charGreaterThanSign); // different from XML

		// If outside of an element, then put in a new line.  This whitespace
		// is not significant.
		if (m_elementLevel == 0)
		{
			outputLineSep();
		}

		m_startNewLine = true;
	}
}



void
FormatterToHTML::writeCharacters(const XalanDOMString&	theString)
{
	writeCharacters(toCharArray(theString), length(theString));
}



void
FormatterToHTML::writeCharacters(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength)
{
	assert(theString != 0);

	if (theLength == unsigned(-1))
	{
		theLength = length(theString);
	}

	for (XalanDOMString::size_type i = 0; i < theLength; ++i) 
	{
		const XalanDOMChar	ch = theString[i];

		if(ch < SPECIALSSIZE && m_charsMap[ch] != 'S')
		{
			accumContent(ch);
		}
		else if (XalanUnicode::charLF == ch) // sta this can be removed?
		{
			outputLineSep();
		}	
		else if (accumDefaultEntity(ch, i, theString, theLength, true) == false)
		{
			if (m_isUTF8 == true && 0xd800 <= ch && ch < 0xdc00)
			{
				// UTF-16 surrogate
				XalanDOMChar	next = 0;

				if (i + 1 >= theLength) 
				{
					throwInvalidUTF16SurrogateException(ch);
				}
				else
				{
					next = theString[++i];

					if (!(0xdc00 <= next && next < 0xe000))
					{
						throwInvalidUTF16SurrogateException(ch, next);
					}

					next = XalanDOMChar(((ch - 0xd800) << 10) + next - 0xdc00 + 0x00010000);
				}

				writeNumberedEntityReference(next);
			}
			else if(ch >= 0x007Fu && ch <= m_maxCharacter)
			{
				// Hope this is right...
				accumContent(ch);
			}
			else
			{
				writeNumberedEntityReference(ch);
			}
		}
	}
}



void
FormatterToHTML::writeAttrString(const XalanDOMChar*	theString)
{
	assert(theString != 0);

    const XalanDOMString::size_type		theLength = length(theString);

    for (XalanDOMString::size_type i = 0;  i < theLength;  i ++)
    {
		const XalanDOMChar	ch = theString[i];

		if(ch < SPECIALSSIZE && m_attrCharsMap[ch] != 'S')
		{
			accumContent(ch);
		}
		else if(XalanUnicode::charAmpersand == ch &&
				i + 1 < theLength &&
				XalanUnicode::charLeftCurlyBracket == theString[i + 1])
		{
			accumContent(ch); // no escaping in this case, as specified in 15.2
		}
		else if (accumDefaultEntity(ch, i, theString, theLength, true) == false)
		{
			if (0xd800 <= ch && ch < 0xdc00) 
			{
				// UTF-16 surrogate

				XalanDOMChar	next = 0;

				if (i + 1 >= theLength) 
				{
					throwInvalidUTF16SurrogateException(ch);
				}
				else 
				{
					next = theString[++i];

					if (!(0xdc00 <= next && next < 0xe000))
					{
						throwInvalidUTF16SurrogateException(ch, next);
					}

					next = XalanDOMChar(((ch - 0xd800) << 10) + next -0xdc00 + 0x00010000);
				}

				accumContent(XalanUnicode::charAmpersand);
				accumContent(XalanUnicode::charNumberSign);

				accumContent(UnsignedLongToDOMString(next, m_stringBuffer));
				clear(m_stringBuffer);

				accumContent(XalanUnicode::charSemicolon);
			}
			else
			{
				writeNumberedEntityReference(ch);
			}
		}
    }
}



void
FormatterToHTML::accumCommentData(const XalanDOMChar*	data)
{
	accumName(data);
}



void
FormatterToHTML::copyEntityIntoBuffer(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theLength)
{
	assert(s != 0);

    accumName(XalanUnicode::charAmpersand);

	accumName(s, 0, theLength);

    accumName(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::copyEntityIntoBuffer(const XalanDOMString&		s)
{
    accumName(XalanUnicode::charAmpersand);

	accumName(s);

    accumName(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::processAttribute(
			const XalanDOMChar*										name,
			const XalanDOMChar*										value,
			const XalanHTMLElementsProperties::ElementProperties&	elemProperties)
{
    accumContent(XalanUnicode::charSpace);

    if((length(value) == 0 || equalsIgnoreCaseASCII(name, value)) &&
	   elemProperties.isAttribute(name, XalanHTMLElementsProperties::ATTREMPTY) == true)
    {
		accumName(name);
    }
    else
    {
		accumName(name);
		accumContent(XalanUnicode::charEqualsSign);
		accumContent(XalanUnicode::charQuoteMark);

		if(elemProperties.isAttribute(name, XalanHTMLElementsProperties::ATTRURL) == true)
		{
			writeAttrURI(value);
		}
		else
		{
			writeAttrString(value);
		}

		accumContent(XalanUnicode::charQuoteMark);
    }
}



void
FormatterToHTML::writeAttrURI(const XalanDOMChar*	theString)
{
	assert(theString != 0);

	// http://www.ietf.org/rfc/rfc2396.txt says:
	// A URI is always in an "escaped" form, since escaping or unescaping a
	// completed URI might change its semantics.  Normally, the only time
	// escape encodings can safely be made is when the URI is being created
	// from its component parts; each component may have its own set of
	// characters that are reserved, so only the mechanism responsible for
	// generating or interpreting that component can determine whether or
	// not escaping a character will change its semantics. Likewise, a URI
	// must be separated into its components before the escaped characters
	// within those components can be safely decoded.
	//
	// ...So we do our best to do limited escaping of the URL, without 
	// causing damage.	If the URL is already properly escaped, in theory, this 
	// function should not change the string value.

	const XalanDOMString::size_type		len = length(theString);

    for (XalanDOMString::size_type i = 0; i < len; ++i)
    {
		const XalanDOMChar	ch = theString[i];

		if (ch < 33 || ch > 126)
		{
			if (m_escapeURLs == true)
			{
				// For the gory details of encoding these characters as
				// UTF-8 hex, see:
				// 
				// Unicode, A Primer, by Tony Graham, p. 92.
				//
				if (ch == XalanUnicode::charSpace)
				{
					accumContent(ch);
				}
				else if(ch <= 0x7F)
				{
					accumHexNumber(ch);
				}
				else if(ch <= 0x7FF)
				{
					const XalanDOMChar	highByte = XalanDOMChar((ch >> 6) | 0xC0);
					const XalanDOMChar	lowByte = XalanDOMChar((ch & 0x3F) | 0x80);

					accumHexNumber(highByte);

					accumHexNumber(lowByte);
				}
				else if(isUTF16Surrogate(ch) == true) // high surrogate
				{
					// I'm sure this can be done in 3 instructions, but I choose 
					// to try and do it exactly like it is done in the book, at least 
					// until we are sure this is totally clean.  I don't think performance 
					// is a big issue with this particular function, though I could be 
					// wrong.  Also, the stuff below clearly does more masking than 
					// it needs to do.
            
					// Clear high 6 bits.
					const XalanDOMChar	highSurrogate = XalanDOMChar(ch & 0x03FF);

					// Middle 4 bits (wwww) + 1
					// "Note that the value of wwww from the high surrogate bit pattern
					// is incremented to make the uuuuu bit pattern in the scalar value 
					// so the surrogate pair don't address the BMP."
					const XalanDOMChar	wwww = XalanDOMChar((highSurrogate & 0x03C0) >> 6);
					const XalanDOMChar	uuuuu = XalanDOMChar(wwww + 1);  

					// next 4 bits
					const XalanDOMChar	zzzz = XalanDOMChar((highSurrogate & 0x003C) >> 2);
            
					// low 2 bits
					const XalanDOMChar	temp = XalanDOMChar(((highSurrogate & 0x0003) << 4) & 0x30);
            
					// Get low surrogate character.
					const XalanDOMChar	nextChar = theString[++i];
            
					// Clear high 6 bits.
					const XalanDOMChar	lowSurrogate = XalanDOMChar(nextChar & 0x03FF);
            
					// put the middle 4 bits into the bottom of yyyyyy (byte 3)
					const XalanDOMChar	yyyyyy = XalanDOMChar(temp | ((lowSurrogate & 0x03C0) >> 6));
            
					// bottom 6 bits.
					const XalanDOMChar	xxxxxx = XalanDOMChar(lowSurrogate & 0x003F);
            
					const XalanDOMChar	byte1 = XalanDOMChar(0xF0 | (uuuuu >> 2)); // top 3 bits of uuuuu
					const XalanDOMChar	byte2 = XalanDOMChar(0x80 | (((uuuuu & 0x03) << 4) & 0x30) | zzzz);
					const XalanDOMChar	byte3 = XalanDOMChar(0x80 | yyyyyy);
					const XalanDOMChar	byte4 = XalanDOMChar(0x80 | xxxxxx);
            
					accumHexNumber(byte1);

					accumHexNumber(byte2);

					accumHexNumber(byte3);

					accumHexNumber(byte4);
				}
				else
				{
					const XalanDOMChar	highByte = XalanDOMChar((ch >> 12) | 0xE0);
					const XalanDOMChar	middleByte = XalanDOMChar(((ch & 0x0FC0) >> 6) | 0x80);
					const XalanDOMChar	lowByte = XalanDOMChar((ch & 0x3F) | 0x80);

					accumHexNumber(highByte);

					accumHexNumber(middleByte);

					accumHexNumber(lowByte);
				}
			}
			else if (ch < m_maxCharacter)
			{
				accumContent(ch);
			}
			else
			{
				accumContent(XalanUnicode::charAmpersand);
				accumContent(XalanUnicode::charNumberSign);
    
				accumContent(UnsignedLongToDOMString(ch, m_stringBuffer));
				clear(m_stringBuffer);

				accumContent(XalanUnicode::charSemicolon);
			}
		}
		// Since http://www.ietf.org/rfc/rfc2396.txt refers to the URI grammar as
		// not allowing quotes in the URI proper syntax, nor in the fragment 
		// identifier, we believe that double quotes should be escaped.
		else if (ch == XalanUnicode::charQuoteMark)
		{
			if (m_escapeURLs == true)
			{
				accumContent(XalanUnicode::charPercentSign);
				accumContent(XalanUnicode::charDigit_2);
				accumContent(XalanUnicode::charDigit_2);
			}
			else
			{
				accumDefaultEntity(ch, i, theString, len, true);
			}
		}
		else if (ch == XalanUnicode::charAmpersand)
		{
			accumDefaultEntity(ch, i, theString, len, true);
		}
		else
		{
			accumContent(ch);
		}
	}
}



void
FormatterToHTML::accumHexNumber(const XalanDOMChar	theChar)
{
	accumContent(XalanUnicode::charPercentSign);

	assert(length(m_stringBuffer) == 0);

	UnsignedLongToHexDOMString(theChar, m_stringBuffer);

	if (length(m_stringBuffer) == 1)
	{
		accumContent(XalanUnicode::charDigit_0);
	}

	accumContent(m_stringBuffer);

	clear(m_stringBuffer);
}



bool
FormatterToHTML::popHasNamespace()
{
	if (m_hasNamespaceStack.empty() == true)
	{
		return false;
	}
	else
	{
		const bool	theValue = m_hasNamespaceStack.back();

		m_hasNamespaceStack.pop_back();
			
		return theValue;
	}
}



bool
FormatterToHTML::pushHasNamespace(const XalanDOMChar*	theElementName)
{
	bool	fHasNamespace = false;

	if (m_prefixResolver != 0)
	{
		const XalanDOMString::size_type		theLength = length(theElementName);
		const XalanDOMString::size_type		theColonIndex = indexOf(theElementName, XalanUnicode::charColon);

		const XalanDOMString*	thePrefix = &s_emptyString;

		if (theColonIndex < theLength)
		{
			substring(theElementName, m_stringBuffer, 0, theColonIndex);

			thePrefix = &m_stringBuffer;
		}

		assert(thePrefix != 0);

		// Check for the namespace...
		const XalanDOMString* const		theNamespace =
				m_prefixResolver->getNamespaceForPrefix(*thePrefix);

		if (theNamespace != 0 && length(*theNamespace) != 0)
		{
			m_hasNamespaceStack.push_back(true);

			fHasNamespace = true;
		}
	}

	return fHasNamespace;
}



static XalanDOMString	s_doctypeHeaderStartString;

static XalanDOMString	s_doctypeHeaderPublicString;

static XalanDOMString	s_doctypeHeaderSystemString;

static XalanDOMString	s_scriptString;

static XalanDOMString	s_styleString;

static XalanDOMString	s_ampString;

static XalanDOMString	s_metaString;


const XalanDOMString&	FormatterToHTML::s_doctypeHeaderStartString =
			::s_doctypeHeaderStartString;

const XalanDOMString&	FormatterToHTML::s_doctypeHeaderPublicString =
			::s_doctypeHeaderPublicString;

const XalanDOMString&	FormatterToHTML::s_doctypeHeaderSystemString =
			::s_doctypeHeaderSystemString;

const XalanDOMString&	FormatterToHTML::s_scriptString =
			::s_scriptString;

const XalanDOMString&	FormatterToHTML::s_styleString =
			::s_styleString;

const XalanDOMString&	FormatterToHTML::s_metaString =
			::s_metaString;


const FormatterToHTML::Entity	FormatterToHTML::s_entities[] =
{
	// These must always be in order by the character.
	// Otherwise, the binary search for them will fail.
	{ 160, 4, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_b, XalanUnicode::charLetter_s, XalanUnicode::charLetter_p, 0 } },
	{ 161, 5, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_e, XalanUnicode::charLetter_x, XalanUnicode::charLetter_c, XalanUnicode::charLetter_l, 0 } },
	{ 162, 4, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_n, XalanUnicode::charLetter_t, 0 } },
	{ 163, 5, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_o, XalanUnicode::charLetter_u, XalanUnicode::charLetter_n, XalanUnicode::charLetter_d, 0 } },
	{ 164, 6, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, XalanUnicode::charLetter_e, XalanUnicode::charLetter_n, 0 } },
	{ 165, 3, { XalanUnicode::charLetter_y, XalanUnicode::charLetter_e, XalanUnicode::charLetter_n, 0 } },
	{ 166, 6, { XalanUnicode::charLetter_b, XalanUnicode::charLetter_r, XalanUnicode::charLetter_v, XalanUnicode::charLetter_b, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, 0 } },
	{ 167, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_e, XalanUnicode::charLetter_c, XalanUnicode::charLetter_t, 0 } },
	{ 168, 3, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 169, 4, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_o, XalanUnicode::charLetter_p, XalanUnicode::charLetter_y, 0 } },
	{ 170, 4, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, XalanUnicode::charLetter_d, XalanUnicode::charLetter_f, 0 } },
	{ 171, 5, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
	{ 172, 3, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, 0 } },
	{ 173, 3, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_h, XalanUnicode::charLetter_y, 0 } },
	{ 174, 3, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_e, XalanUnicode::charLetter_g, 0 } },
	{ 175, 4, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_r, 0 } },
	{ 176, 3, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, XalanUnicode::charLetter_g, 0 } },
	{ 177, 6, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_l, XalanUnicode::charLetter_u, XalanUnicode::charLetter_s, XalanUnicode::charLetter_m, XalanUnicode::charLetter_n, 0 } },
	{ 178, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charDigit_2, 0 } },
	{ 179, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charDigit_3, 0 } },
	{ 180, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 181, 5, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_c, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, 0 } },
	{ 182, 4, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, 0 } },
	{ 183, 6, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_d, XalanUnicode::charLetter_d, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, 0 } },
	{ 184, 5, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
	{ 185, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charDigit_1, 0 } },
	{ 186, 4, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, XalanUnicode::charLetter_d, XalanUnicode::charLetter_m, 0 } },
	{ 187, 5, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
	{ 188, 6, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charDigit_1, XalanUnicode::charDigit_4, 0 } },
	{ 189, 6, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charDigit_1, XalanUnicode::charDigit_2, 0 } },
	{ 190, 6, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charDigit_3, XalanUnicode::charDigit_4, 0 } },
	{ 191, 6, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_e, XalanUnicode::charLetter_s, XalanUnicode::charLetter_t, 0 } },
	{ 192, 6, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
	{ 193, 6, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 194, 5, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
	{ 195, 6, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
	{ 196, 4, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 197, 5, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
	{ 198, 5, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_E, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, 0 } },
	{ 199, 6, { XalanUnicode::charLetter_C, XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
	{ 200, 6, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
	{ 201, 6, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 202, 5, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
	{ 203, 4, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 204, 6, { XalanUnicode::charLetter_I, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
	{ 205, 6, { XalanUnicode::charLetter_I, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 206, 5, { XalanUnicode::charLetter_I, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
	{ 207, 4, { XalanUnicode::charLetter_I, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 208, 3, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_T, XalanUnicode::charLetter_H, 0 } },
	{ 209, 6, { XalanUnicode::charLetter_N, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
	{ 210, 6, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
	{ 211, 6, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 212, 5, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
	{ 213, 6, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
	{ 214, 4, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 215, 5, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, XalanUnicode::charLetter_s, 0 } },
	{ 216, 6, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_s, XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_h, 0 } },
	{ 217, 6, { XalanUnicode::charLetter_U, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
	{ 218, 6, { XalanUnicode::charLetter_U, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 219, 5, { XalanUnicode::charLetter_U, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
	{ 220, 4, { XalanUnicode::charLetter_U, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 221, 6, { XalanUnicode::charLetter_Y, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 222, 5, { XalanUnicode::charLetter_T, XalanUnicode::charLetter_H, XalanUnicode::charLetter_O, XalanUnicode::charLetter_R, XalanUnicode::charLetter_N, 0 } },
	{ 223, 5, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_z, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, 0 } },
	{ 224, 6, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
	{ 225, 6, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 226, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
	{ 227, 6, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
	{ 228, 4, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 229, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
	{ 230, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_e, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, 0 } },
	{ 231, 6, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
	{ 232, 6, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
	{ 233, 6, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 234, 5, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
	{ 235, 4, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 236, 6, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
	{ 237, 6, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 238, 5, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
	{ 239, 4, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 240, 3, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, 0 } },
	{ 241, 6, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
	{ 242, 6, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
	{ 243, 6, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 244, 5, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
	{ 245, 6, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
	{ 246, 4, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 247, 6, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_v, XalanUnicode::charLetter_i, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
	{ 248, 6, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_s, XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_h, 0 } },
	{ 249, 6, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
	{ 250, 6, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 251, 5, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
	{ 252, 4, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 253, 6, { XalanUnicode::charLetter_y, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
	{ 254, 5, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, XalanUnicode::charLetter_n, 0 } },
	{ 255, 4, { XalanUnicode::charLetter_y, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 338, 5, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_E, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, 0 } },
	{ 339, 5, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_e, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, 0 } },
	{ 352, 6, { XalanUnicode::charLetter_S, XalanUnicode::charLetter_c, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
	{ 353, 6, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_c, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
	{ 376, 4, { XalanUnicode::charLetter_Y, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
	{ 402, 4, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_n, XalanUnicode::charLetter_o, XalanUnicode::charLetter_f, 0 } },
	{ 710, 4, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
	{ 732, 5, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
	{ 913, 5, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_l, XalanUnicode::charLetter_p, XalanUnicode::charLetter_h, XalanUnicode::charLetter_a, 0 } },
	{ 914, 4, { XalanUnicode::charLetter_B, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 915, 5, { XalanUnicode::charLetter_G, XalanUnicode::charLetter_a, XalanUnicode::charLetter_m, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, 0 } },
	{ 916, 5, { XalanUnicode::charLetter_D, XalanUnicode::charLetter_e, XalanUnicode::charLetter_l, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 917, 7, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
	{ 918, 4, { XalanUnicode::charLetter_Z, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 919, 3, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 920, 5, { XalanUnicode::charLetter_T, XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 921, 4, { XalanUnicode::charLetter_I, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 922, 5, { XalanUnicode::charLetter_K, XalanUnicode::charLetter_a, XalanUnicode::charLetter_p, XalanUnicode::charLetter_p, XalanUnicode::charLetter_a, 0 } },
	{ 923, 6, { XalanUnicode::charLetter_L, XalanUnicode::charLetter_a, XalanUnicode::charLetter_m, XalanUnicode::charLetter_b, XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, 0 } },
	{ 924, 2, { XalanUnicode::charLetter_M, XalanUnicode::charLetter_u, 0 } },
	{ 925, 2, { XalanUnicode::charLetter_N, XalanUnicode::charLetter_u, 0 } },
	{ 926, 2, { XalanUnicode::charLetter_X, XalanUnicode::charLetter_i, 0 } },
	{ 927, 7, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_c, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
	{ 928, 2, { XalanUnicode::charLetter_P, XalanUnicode::charLetter_i, 0 } },
	{ 929, 3, { XalanUnicode::charLetter_R, XalanUnicode::charLetter_h, XalanUnicode::charLetter_o, 0 } },
	{ 931, 5, { XalanUnicode::charLetter_S, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, 0 } },
	{ 932, 3, { XalanUnicode::charLetter_T, XalanUnicode::charLetter_a, XalanUnicode::charLetter_u, 0 } },
	{ 933, 7, { XalanUnicode::charLetter_U, XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
	{ 934, 3, { XalanUnicode::charLetter_P, XalanUnicode::charLetter_h, XalanUnicode::charLetter_i, 0 } },
	{ 935, 3, { XalanUnicode::charLetter_C, XalanUnicode::charLetter_h, XalanUnicode::charLetter_i, 0 } },
	{ 936, 3, { XalanUnicode::charLetter_P, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, 0 } },
	{ 937, 5, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, XalanUnicode::charLetter_g, XalanUnicode::charLetter_a, 0 } },
	{ 945, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_l, XalanUnicode::charLetter_p, XalanUnicode::charLetter_h, XalanUnicode::charLetter_a, 0 } },
	{ 946, 4, { XalanUnicode::charLetter_b, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 947, 5, { XalanUnicode::charLetter_g, XalanUnicode::charLetter_a, XalanUnicode::charLetter_m, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, 0 } },
	{ 948, 5, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, XalanUnicode::charLetter_l, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 949, 7, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
	{ 950, 4, { XalanUnicode::charLetter_z, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 951, 3, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 952, 5, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 953, 4, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
	{ 954, 5, { XalanUnicode::charLetter_k, XalanUnicode::charLetter_a, XalanUnicode::charLetter_p, XalanUnicode::charLetter_p, XalanUnicode::charLetter_a, 0 } },
	{ 955, 6, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_m, XalanUnicode::charLetter_b, XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, 0 } },
	{ 956, 2, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_u, 0 } },
	{ 957, 2, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_u, 0 } },
	{ 958, 2, { XalanUnicode::charLetter_x, XalanUnicode::charLetter_i, 0 } },
	{ 959, 7, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_c, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
	{ 960, 2, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_i, 0 } },
	{ 961, 3, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_h, XalanUnicode::charLetter_o, 0 } },
	{ 962, 6, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, XalanUnicode::charLetter_f, 0 } },
	{ 963, 5, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, 0 } },
	{ 964, 3, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, XalanUnicode::charLetter_u, 0 } },
	{ 965, 7, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
	{ 966, 3, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_h, XalanUnicode::charLetter_i, 0 } },
	{ 967, 3, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_h, XalanUnicode::charLetter_i, 0 } },
	{ 968, 3, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, 0 } },
	{ 969, 5, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, XalanUnicode::charLetter_g, XalanUnicode::charLetter_a, 0 } },
	{ 977, 8, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_y, XalanUnicode::charLetter_m, 0 } },
	{ 978, 5, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_h, 0 } },
	{ 982, 3, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_i, XalanUnicode::charLetter_v, 0 } },
	{ 8194, 4, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_n, XalanUnicode::charLetter_s, XalanUnicode::charLetter_p, 0 } },
	{ 8195, 4, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_m, XalanUnicode::charLetter_s, XalanUnicode::charLetter_p, 0 } },
	{ 8201, 6, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_s, XalanUnicode::charLetter_p, 0 } },
	{ 8204, 4, { XalanUnicode::charLetter_z, XalanUnicode::charLetter_w, XalanUnicode::charLetter_n, XalanUnicode::charLetter_j, 0 } },
	{ 8205, 3, { XalanUnicode::charLetter_z, XalanUnicode::charLetter_w, XalanUnicode::charLetter_j, 0 } },
	{ 8206, 3, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_r, XalanUnicode::charLetter_m, 0 } },
	{ 8207, 3, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_l, XalanUnicode::charLetter_m, 0 } },
	{ 8211, 5, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_h, 0 } },
	{ 8212, 5, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_h, 0 } },
	{ 8216, 5, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_s, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
	{ 8217, 5, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_s, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
	{ 8218, 5, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_b, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
	{ 8220, 5, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
	{ 8221, 5, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_d, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
	{ 8222, 5, { XalanUnicode::charLetter_b, XalanUnicode::charLetter_d, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
	{ 8224, 6, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, XalanUnicode::charLetter_g, XalanUnicode::charLetter_g, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, 0 } },
	{ 8225, 6, { XalanUnicode::charLetter_D, XalanUnicode::charLetter_a, XalanUnicode::charLetter_g, XalanUnicode::charLetter_g, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, 0 } },
	{ 8226, 4, { XalanUnicode::charLetter_b, XalanUnicode::charLetter_u, XalanUnicode::charLetter_l, XalanUnicode::charLetter_l, 0 } },
	{ 8230, 6, { XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_l, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_p, 0 } },
	{ 8240, 6, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
	{ 8242, 5, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_r, XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, 0 } },
	{ 8243, 5, { XalanUnicode::charLetter_P, XalanUnicode::charLetter_r, XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, 0 } },
	{ 8249, 6, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_s, XalanUnicode::charLetter_a, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
	{ 8250, 6, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_s, XalanUnicode::charLetter_a, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
	{ 8254, 5, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_e, 0 } },
	{ 8260, 5, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_l, 0 } },
	{ 8364, 4, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_u, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, 0 } },
	{ 8465, 5, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, XalanUnicode::charLetter_g, XalanUnicode::charLetter_e, 0 } },
	{ 8472, 6, { XalanUnicode::charLetter_w, XalanUnicode::charLetter_e, XalanUnicode::charLetter_i, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, XalanUnicode::charLetter_p, 0 } },
	{ 8476, 4, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_e, XalanUnicode::charLetter_a, XalanUnicode::charLetter_l, 0 } },
	{ 8482, 5, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
	{ 8501, 7, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_l, XalanUnicode::charLetter_e, XalanUnicode::charLetter_f, XalanUnicode::charLetter_s, XalanUnicode::charLetter_y, XalanUnicode::charLetter_m, 0 } },
	{ 8592, 4, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
	{ 8593, 4, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
	{ 8594, 4, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
	{ 8595, 4, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
	{ 8596, 4, { XalanUnicode::charLetter_h, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
	{ 8629, 5, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
	{ 8656, 4, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
	{ 8657, 4, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
	{ 8658, 4, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
	{ 8659, 4, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
	{ 8660, 4, { XalanUnicode::charLetter_h, XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
	{ 8704, 6, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_l, XalanUnicode::charLetter_l, 0 } },
	{ 8706, 4, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_t, 0 } },
	{ 8707, 5, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_x, XalanUnicode::charLetter_i, XalanUnicode::charLetter_s, XalanUnicode::charLetter_t, 0 } },
	{ 8709, 5, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_m, XalanUnicode::charLetter_p, XalanUnicode::charLetter_t, XalanUnicode::charLetter_y, 0 } },
	{ 8711, 5, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_a, XalanUnicode::charLetter_b, XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, 0 } },
	{ 8712, 4, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, 0 } },
	{ 8713, 5, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, 0 } },
	{ 8715, 2, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_i, 0 } },
	{ 8719, 4, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_d, 0 } },
	{ 8721, 3, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, 0 } },
	{ 8722, 5, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_u, XalanUnicode::charLetter_s, 0 } },
	{ 8727, 6, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_w, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_t, 0 } },
	{ 8730, 5, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_c, 0 } },
	{ 8733, 4, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_p, 0 } },
	{ 8734, 5, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_f, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, 0 } },
	{ 8736, 3, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
	{ 8743, 3, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_n, XalanUnicode::charLetter_d, 0 } },
	{ 8744, 2, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, 0 } },
	{ 8745, 3, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_a, XalanUnicode::charLetter_p, 0 } },
	{ 8746, 3, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, 0 } },
	{ 8747, 3, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_t, 0 } },
	{ 8756, 6, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, XalanUnicode::charLetter_e, XalanUnicode::charDigit_4, 0 } },
	{ 8764, 3, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, 0 } },
	{ 8773, 4, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
	{ 8776, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_y, XalanUnicode::charLetter_m, XalanUnicode::charLetter_p, 0 } },
	{ 8800, 2, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_e, 0 } },
	{ 8801, 5, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_i, XalanUnicode::charLetter_v, 0 } },
	{ 8804, 2, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_e, 0 } },
	{ 8805, 2, { XalanUnicode::charLetter_g, XalanUnicode::charLetter_e, 0 } },
	{ 8834, 3, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_b, 0 } },
	{ 8835, 3, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, 0 } },
	{ 8836, 4, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_b, 0 } },
	{ 8838, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_b, XalanUnicode::charLetter_e, 0 } },
	{ 8839, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charLetter_e, 0 } },
	{ 8853, 5, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_p, XalanUnicode::charLetter_l, XalanUnicode::charLetter_u, XalanUnicode::charLetter_s, 0 } },
	{ 8855, 6, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, XalanUnicode::charLetter_s, 0 } },
	{ 8869, 4, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, XalanUnicode::charLetter_p, 0 } },
	{ 8901, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_d, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, 0 } },
	{ 8968, 5, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
	{ 8969, 5, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
	{ 8970, 6, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_f, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, 0 } },
	{ 8971, 6, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_f, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, 0 } },
	{ 9001, 4, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
	{ 9002, 4, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
	{ 9674, 3, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_z, 0 } },
	{ 9824, 6, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_p, XalanUnicode::charLetter_a, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, XalanUnicode::charLetter_s, 0 } },
	{ 9827, 5, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_l, XalanUnicode::charLetter_u, XalanUnicode::charLetter_b, XalanUnicode::charLetter_s, 0 } },
	{ 9829, 6, { XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_t, XalanUnicode::charLetter_s, 0 } },
	{ 9830, 5, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_a, XalanUnicode::charLetter_m, XalanUnicode::charLetter_s, 0 } }
};

const FormatterToHTML::Entity* const	FormatterToHTML::s_lastEntity = 
	FormatterToHTML::s_entities + (sizeof(s_entities) / sizeof (s_entities[0])) - 1;



void
FormatterToHTML::initialize()
{
	::s_doctypeHeaderStartString = XALAN_STATIC_UCODE_STRING("<!DOCTYPE HTML");

	::s_doctypeHeaderPublicString = XALAN_STATIC_UCODE_STRING(" PUBLIC \"");

	::s_doctypeHeaderSystemString = XALAN_STATIC_UCODE_STRING(" SYSTEM");

	::s_scriptString = XALAN_STATIC_UCODE_STRING("SCRIPT");

	::s_styleString = XALAN_STATIC_UCODE_STRING("STYLE");

	::s_metaString = XALAN_STATIC_UCODE_STRING("<META http-equiv=\"Content-Type\" content=\"text/html; charset=");
}



void
FormatterToHTML::terminate()
{
	releaseMemory(::s_doctypeHeaderStartString);

	releaseMemory(::s_doctypeHeaderPublicString);

	releaseMemory(::s_doctypeHeaderSystemString);

	releaseMemory(::s_scriptString);

	releaseMemory(::s_styleString);

	releaseMemory(::s_metaString);		
}
