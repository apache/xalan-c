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
	m_omitMetaTag(omitMetaTag)
{
	assert(s_elementFlags != 0 && s_dummyDesc != 0);

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



const FormatterToHTML::ElemDesc&
FormatterToHTML::getElemDesc(const XalanDOMChar*	name)
{
	assert(s_elementFlags != 0 && s_dummyDesc != 0);

	const ElementFlagsMapType::const_iterator	i =
		s_elementFlags->find(name);

	if (i == s_elementFlags->end())
	{
		return *s_dummyDesc;
	}
	else
	{
		return (*i).second;
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

		const ElemDesc&		elemDesc =
				getElemDesc(name);

		const bool	isBlockElement = elemDesc.is(ElemDesc::BLOCK);

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

		m_isRawStack.push_back(elemDesc.is(ElemDesc::RAW));

		accumContent(XalanUnicode::charLessThanSign);

		accumName(name);

		const unsigned int	nAttrs = attrs.getLength();

		for (unsigned int i = 0;  i < nAttrs ;  i++)
		{
			processAttribute(attrs.getName(i), attrs.getValue(i), elemDesc);
		}

		// Flag the current element as not yet having any children.
		openElementForChildren();

		m_currentIndent += m_indent;
    
		m_isprevtext = false;

		if (elemDesc.is(ElemDesc::HEADELEM) == true)
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

		m_isRawStack.pop_back();
		m_inScriptElemStack.pop_back();
    
		const ElemDesc&		elemDesc =
				getElemDesc(name);

		const bool	isBlockElement = elemDesc.is(ElemDesc::BLOCK);

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
			if(elemDesc.is(ElemDesc::EMPTY) == false)
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

		if (elemDesc.is(ElemDesc::WHITESPACESENSITIVE) == true)
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
	assert(ch != 0 && s_entitiesSize > 0);

	if(FormatterToXML::accumDefaultEntity(ch, i, chars, len, escLF) == true)
	{
		return true;
	}
	else
	{
		// Find the entity, if any...
		const EntityPair*	theFirst = s_entities;
		const EntityPair*	theLast = &s_entities[s_entitiesSize - 2];

		while(theFirst <= theLast)
		{
			const EntityPair*	theCurrent = theFirst + (theLast - theFirst) / 2;
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
				copyEntityIntoBuffer(theCurrent->m_string);

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
FormatterToHTML::copyEntityIntoBuffer(const XalanDOMChar*	s)
{
	const XalanDOMString::size_type		len = length(s);

    accumContent(XalanUnicode::charAmpersand);

    for(XalanDOMString::size_type i = 0; i < len; ++i)
    {
		accumContent(s[i]);
    }

    accumContent(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::copyEntityIntoBuffer(const XalanDOMString&		s)
{
	const XalanDOMString::size_type		len = length(s);

    accumContent(XalanUnicode::charAmpersand);

    for(XalanDOMString::size_type i = 0; i < len; ++i)
    {
		accumContent(charAt(s, i));
    }

    accumContent(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::processAttribute(
			const XalanDOMChar*		name,
			const XalanDOMChar*		value,
			const ElemDesc&			elemDesc)
{
    accumContent(XalanUnicode::charSpace);

    if((length(value) == 0 || equalsIgnoreCaseASCII(name, value)) &&
	   elemDesc.isAttrFlagSet(name, ElemDesc::ATTREMPTY) == true)
    {
		accumName(name);
    }
    else
    {
		accumName(name);
		accumContent(XalanUnicode::charEqualsSign);
		accumContent(XalanUnicode::charQuoteMark);

		if(elemDesc.isAttrFlagSet(name, ElemDesc::ATTRURL) == true)
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



typedef FormatterToHTML::ElementFlagsMapType	ElementFlagsMapType;
typedef FormatterToHTML::ElemDesc				ElemDesc;



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



static void
initializeElementFlagsMap1(ElementFlagsMapType&		theElementFlags)
{
#if defined(XALAN_NO_NAMESPACES)
	typedef pair<ElementFlagsMapType::iterator, bool>	PairType;
#else
	typedef std::pair<ElementFlagsMapType::iterator, bool>	PairType;
#endif

	// HTML 4.0 loose DTD
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BASEFONT")),
			ElemDesc(0|ElemDesc::EMPTY)));

	PairType	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("FRAME")),
			ElemDesc(0|ElemDesc::EMPTY|ElemDesc::BLOCK)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("SRC")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("FRAMESET")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("NOFRAMES")),
			ElemDesc(0|ElemDesc::BLOCK)));
 
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("ISINDEX")),
			ElemDesc(0|ElemDesc::EMPTY|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("APPLET")),
			ElemDesc(0|ElemDesc::WHITESPACESENSITIVE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("CENTER")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DIR")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("MENU")),
			ElemDesc(0|ElemDesc::BLOCK)));


	// HTML 4.0 strict DTD
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TT")),
			ElemDesc(0|ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("I")),
			ElemDesc(0|ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("B")),
			ElemDesc(0|ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BIG")),
			ElemDesc(0|ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SMALL")),
			ElemDesc(0|ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("EM")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("STRONG")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DFN")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("CODE")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SAMP")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("KBD")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("VAR")),
			ElemDesc(0|ElemDesc::PHRASE)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("CITE")),
			ElemDesc(0|ElemDesc::PHRASE)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("ABBR")),
			ElemDesc(0|ElemDesc::PHRASE)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("ACRONYM")),
			ElemDesc(0|ElemDesc::PHRASE)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SUP")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SUB")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SPAN")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BDO")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BR")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL|ElemDesc::EMPTY|ElemDesc::BLOCK)));
	
}



static void
initializeElementFlagsMap2(ElementFlagsMapType&		theElementFlags)
{
#if defined(XALAN_NO_NAMESPACES)
	typedef pair<ElementFlagsMapType::iterator, bool>	PairType;
#else
	typedef std::pair<ElementFlagsMapType::iterator, bool>	PairType;
#endif

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BODY")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("ADDRESS")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DIV")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));

	PairType	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("A")),
			ElemDesc(0|ElemDesc::SPECIAL)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("HREF")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("NAME")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("MAP")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL|ElemDesc::BLOCK)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("AREA")),
			ElemDesc(0|ElemDesc::EMPTY|ElemDesc::BLOCK)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("HREF")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("NOHREF")), ElemDesc::ATTREMPTY);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("LINK")),
			ElemDesc(0|ElemDesc::HEADMISC|ElemDesc::EMPTY|ElemDesc::BLOCK)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("IMG")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL|ElemDesc::EMPTY|ElemDesc::WHITESPACESENSITIVE)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("SRC")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("LONGDESC")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("USEMAP")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("ISMAP")), ElemDesc::ATTREMPTY);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("OBJECT")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL|ElemDesc::HEADMISC|ElemDesc::WHITESPACESENSITIVE)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CLASSID")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CODEBASE")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DATA")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("ARCHIVE")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("USEMAP")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DECLARE")), ElemDesc::ATTREMPTY);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("PARAM")),
			ElemDesc(0|ElemDesc::EMPTY)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("HR")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET|ElemDesc::EMPTY)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("P")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H1")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H2")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H3")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H4")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H5")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H6")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("PRE")),
			ElemDesc(0|ElemDesc::PREFORMATTED|ElemDesc::BLOCK)));

}



static void
initializeElementFlagsMap3(ElementFlagsMapType&		theElementFlags)
{
#if defined(XALAN_NO_NAMESPACES)
	typedef pair<ElementFlagsMapType::iterator, bool>	PairType;
#else
	typedef std::pair<ElementFlagsMapType::iterator, bool>	PairType;
#endif

	PairType	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("Q")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CITE")), ElemDesc::ATTRURL);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BLOCKQUOTE")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CITE")), ElemDesc::ATTRURL);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("INS")),
			ElemDesc(0)));
	
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CITE")), ElemDesc::ATTRURL);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DEL")),
			ElemDesc(0)));
	
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CITE")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DL")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DT")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DD")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("OL")),
			ElemDesc(0|ElemDesc::LIST|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("UL")),
			ElemDesc(0|ElemDesc::LIST|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("LI")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("FORM")),
			ElemDesc(0|ElemDesc::BLOCK)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("ACTION")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("LABEL")),
			ElemDesc(0|ElemDesc::FORMCTRL)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("INPUT")),
			ElemDesc(0|ElemDesc::FORMCTRL|ElemDesc::INLINELABEL|ElemDesc::EMPTY)));
	
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("SRC")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("USEMAP")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CHECKED")), ElemDesc::ATTREMPTY);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DISABLED")), ElemDesc::ATTREMPTY);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("ISMAP")), ElemDesc::ATTREMPTY);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("READONLY")), ElemDesc::ATTREMPTY);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SELECT")),
			ElemDesc(0|ElemDesc::FORMCTRL|ElemDesc::INLINELABEL)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DISABLED")), ElemDesc::ATTREMPTY);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("MULTIPLE")), ElemDesc::ATTREMPTY);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("OPTGROUP")),
			ElemDesc(0)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DISABLED")), ElemDesc::ATTREMPTY);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("OPTION")),
			ElemDesc(0)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DISABLED")), ElemDesc::ATTREMPTY);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("SELECTED")), ElemDesc::ATTREMPTY);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TEXTAREA")),
			ElemDesc(0|ElemDesc::FORMCTRL|ElemDesc::INLINELABEL)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DISABLED")), ElemDesc::ATTREMPTY);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("READONLY")), ElemDesc::ATTREMPTY);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("FIELDSET")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("LEGEND")),
			ElemDesc(0)));
}



static void
initializeElementFlagsMap4(ElementFlagsMapType&		theElementFlags)
{
#if defined(XALAN_NO_NAMESPACES)
	typedef pair<ElementFlagsMapType::iterator, bool>	PairType;
#else
	typedef std::pair<ElementFlagsMapType::iterator, bool>	PairType;
#endif

	PairType	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BUTTON")),
			ElemDesc(0|ElemDesc::FORMCTRL|ElemDesc::INLINELABEL)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DISABLED")), ElemDesc::ATTREMPTY);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TABLE")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("CAPTION")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("THEAD")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TFOOT")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TBODY")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("COLGROUP")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("COL")),
			ElemDesc(0|ElemDesc::EMPTY|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TR")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TH")),
			ElemDesc(0)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TD")),
			ElemDesc(0)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("HEAD")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::HEADELEM)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("PROFILE")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TITLE")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BASE")),
			ElemDesc(0|ElemDesc::EMPTY|ElemDesc::BLOCK)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("HREF")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("META")),
			ElemDesc(0|ElemDesc::HEADMISC|ElemDesc::EMPTY|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("STYLE")),
			ElemDesc(0|ElemDesc::HEADMISC|ElemDesc::RAW|ElemDesc::BLOCK)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SCRIPT")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL|ElemDesc::HEADMISC|ElemDesc::RAW)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("SRC")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("FOR")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DEFER")), ElemDesc::ATTREMPTY);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("NOSCRIPT")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("HTML")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("FONT")),
			ElemDesc(0 | ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("S")),
			ElemDesc(0 | ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("STRIKE")),
			ElemDesc(0 | ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("U")),
			ElemDesc(0 | ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("NOBR")),
			ElemDesc(0 | ElemDesc::FONTSTYLE)));
}



void
FormatterToHTML::initializeElementFlagsMap(ElementFlagsMapType&		theElementFlags)
{
	initializeElementFlagsMap1(theElementFlags);
	initializeElementFlagsMap2(theElementFlags);
	initializeElementFlagsMap3(theElementFlags);
	initializeElementFlagsMap4(theElementFlags);
}



static XalanAutoPtr<FormatterToHTML::ElementFlagsMapType>	s_elementFlags;


const FormatterToHTML::ElementFlagsMapType*			FormatterToHTML::s_elementFlags = 0;


static XalanAutoPtr<FormatterToHTML::ElemDesc>	s_dummyDesc;

const FormatterToHTML::ElemDesc*	FormatterToHTML::s_dummyDesc = 0;



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


#if defined(XALAN_LSTRSUPPORT) && !defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)

#define FHTML_LSTR(str) XALAN_STATIC_UCODE_STRING(str)

FormatterToHTML::EntityPair		FormatterToHTML::s_entities[] =

#else

#define FHTML_LSTR(str) str

struct LocalEntityPair
{
	XalanDOMChar	m_char;

	const char*		m_string;
};

static const LocalEntityPair	theLocalEntities[] =

#endif
{
	// These must always be in order by the character.
	// Otherwise, the binary search for them will fail.
	{ 160, FHTML_LSTR("nbsp") },
	{ 161, FHTML_LSTR("iexcl") },
	{ 162, FHTML_LSTR("cent") },
	{ 163, FHTML_LSTR("pound") },
	{ 164, FHTML_LSTR("curren") },
	{ 165, FHTML_LSTR("yen") },
	{ 166, FHTML_LSTR("brvbar") },
	{ 167, FHTML_LSTR("sect") },
	{ 168, FHTML_LSTR("uml") },
	{ 169, FHTML_LSTR("copy") },
	{ 170, FHTML_LSTR("ordf") },
	{ 171, FHTML_LSTR("laquo") },
	{ 172, FHTML_LSTR("not") },
	{ 173, FHTML_LSTR("shy") },
	{ 174, FHTML_LSTR("reg") },
	{ 175, FHTML_LSTR("macr") },
	{ 176, FHTML_LSTR("deg") },
	{ 177, FHTML_LSTR("plusmn") },
	{ 178, FHTML_LSTR("sup2") },
	{ 179, FHTML_LSTR("sup3") },
	{ 180, FHTML_LSTR("acute") },
	{ 181, FHTML_LSTR("micro") },
	{ 182, FHTML_LSTR("para") },
	{ 183, FHTML_LSTR("middot") },
	{ 184, FHTML_LSTR("cedil") },
	{ 185, FHTML_LSTR("sup1") },
	{ 186, FHTML_LSTR("ordm") },
	{ 187, FHTML_LSTR("raquo") },
	{ 188, FHTML_LSTR("frac14") },
	{ 189, FHTML_LSTR("frac12") },
	{ 190, FHTML_LSTR("frac34") },
	{ 191, FHTML_LSTR("iquest") },
	{ 192, FHTML_LSTR("Agrave") },
	{ 193, FHTML_LSTR("Aacute") },
	{ 194, FHTML_LSTR("Acirc") },
	{ 195, FHTML_LSTR("Atilde") },
	{ 196, FHTML_LSTR("Auml") },
	{ 197, FHTML_LSTR("Aring") },
	{ 198, FHTML_LSTR("AElig") },
	{ 199, FHTML_LSTR("Ccedil") },
	{ 200, FHTML_LSTR("Egrave") },
	{ 201, FHTML_LSTR("Eacute") },
	{ 202, FHTML_LSTR("Ecirc") },
	{ 203, FHTML_LSTR("Euml") },
	{ 204, FHTML_LSTR("Igrave") },
	{ 205, FHTML_LSTR("Iacute") },
	{ 206, FHTML_LSTR("Icirc") },
	{ 207, FHTML_LSTR("Iuml") },
	{ 208, FHTML_LSTR("ETH") },
	{ 209, FHTML_LSTR("Ntilde") },
	{ 210, FHTML_LSTR("Ograve") },
	{ 211, FHTML_LSTR("Oacute") },
	{ 212, FHTML_LSTR("Ocirc") },
	{ 213, FHTML_LSTR("Otilde") },
	{ 214, FHTML_LSTR("Ouml") },
	{ 215, FHTML_LSTR("times") },
	{ 216, FHTML_LSTR("Oslash") },
	{ 217, FHTML_LSTR("Ugrave") },
	{ 218, FHTML_LSTR("Uacute") },
	{ 219, FHTML_LSTR("Ucirc") },
	{ 220, FHTML_LSTR("Uuml") },
	{ 221, FHTML_LSTR("Yacute") },
	{ 222, FHTML_LSTR("THORN") },
	{ 223, FHTML_LSTR("szlig") },
	{ 224, FHTML_LSTR("agrave") },
	{ 225, FHTML_LSTR("aacute") },
	{ 226, FHTML_LSTR("acirc") },
	{ 227, FHTML_LSTR("atilde") },
	{ 228, FHTML_LSTR("auml") },
	{ 229, FHTML_LSTR("aring") },
	{ 230, FHTML_LSTR("aelig") },
	{ 231, FHTML_LSTR("ccedil") },
	{ 232, FHTML_LSTR("egrave") },
	{ 233, FHTML_LSTR("eacute") },
	{ 234, FHTML_LSTR("ecirc") },
	{ 235, FHTML_LSTR("euml") },
	{ 236, FHTML_LSTR("igrave") },
	{ 237, FHTML_LSTR("iacute") },
	{ 238, FHTML_LSTR("icirc") },
	{ 239, FHTML_LSTR("iuml") },
	{ 240, FHTML_LSTR("eth") },
	{ 241, FHTML_LSTR("ntilde") },
	{ 242, FHTML_LSTR("ograve") },
	{ 243, FHTML_LSTR("oacute") },
	{ 244, FHTML_LSTR("ocirc") },
	{ 245, FHTML_LSTR("otilde") },
	{ 246, FHTML_LSTR("ouml") },
	{ 247, FHTML_LSTR("divide") },
	{ 248, FHTML_LSTR("oslash") },
	{ 249, FHTML_LSTR("ugrave") },
	{ 250, FHTML_LSTR("uacute") },
	{ 251, FHTML_LSTR("ucirc") },
	{ 252, FHTML_LSTR("uuml") },
	{ 253, FHTML_LSTR("yacute") },
	{ 254, FHTML_LSTR("thorn") },
	{ 255, FHTML_LSTR("yuml") },
	{ 338, FHTML_LSTR("OElig") },
	{ 339, FHTML_LSTR("oelig") },
	{ 352, FHTML_LSTR("Scaron") },
	{ 353, FHTML_LSTR("scaron") },
	{ 376, FHTML_LSTR("Yuml") },
	{ 402, FHTML_LSTR("fnof") },
	{ 710, FHTML_LSTR("circ") },
	{ 732, FHTML_LSTR("tilde") },
	{ 913, FHTML_LSTR("Alpha") },
	{ 914, FHTML_LSTR("Beta") },
	{ 915, FHTML_LSTR("Gamma") },
	{ 916, FHTML_LSTR("Delta") },
	{ 917, FHTML_LSTR("Epsilon") },
	{ 918, FHTML_LSTR("Zeta") },
	{ 919, FHTML_LSTR("Eta") },
	{ 920, FHTML_LSTR("Theta") },
	{ 921, FHTML_LSTR("Iota") },
	{ 922, FHTML_LSTR("Kappa") },
	{ 923, FHTML_LSTR("Lambda") },
	{ 924, FHTML_LSTR("Mu") },
	{ 925, FHTML_LSTR("Nu") },
	{ 926, FHTML_LSTR("Xi") },
	{ 927, FHTML_LSTR("Omicron") },
	{ 928, FHTML_LSTR("Pi") },
	{ 929, FHTML_LSTR("Rho") },
	{ 931, FHTML_LSTR("Sigma") },
	{ 932, FHTML_LSTR("Tau") },
	{ 933, FHTML_LSTR("Upsilon") },
	{ 934, FHTML_LSTR("Phi") },
	{ 935, FHTML_LSTR("Chi") },
	{ 936, FHTML_LSTR("Psi") },
	{ 937, FHTML_LSTR("Omega") },
	{ 945, FHTML_LSTR("alpha") },
	{ 946, FHTML_LSTR("beta") },
	{ 947, FHTML_LSTR("gamma") },
	{ 948, FHTML_LSTR("delta") },
	{ 949, FHTML_LSTR("epsilon") },
	{ 950, FHTML_LSTR("zeta") },
	{ 951, FHTML_LSTR("eta") },
	{ 952, FHTML_LSTR("theta") },
	{ 953, FHTML_LSTR("iota") },
	{ 954, FHTML_LSTR("kappa") },
	{ 955, FHTML_LSTR("lambda") },
	{ 956, FHTML_LSTR("mu") },
	{ 957, FHTML_LSTR("nu") },
	{ 958, FHTML_LSTR("xi") },
	{ 959, FHTML_LSTR("omicron") },
	{ 960, FHTML_LSTR("pi") },
	{ 961, FHTML_LSTR("rho") },
	{ 962, FHTML_LSTR("sigmaf") },
	{ 963, FHTML_LSTR("sigma") },
	{ 964, FHTML_LSTR("tau") },
	{ 965, FHTML_LSTR("upsilon") },
	{ 966, FHTML_LSTR("phi") },
	{ 967, FHTML_LSTR("chi") },
	{ 968, FHTML_LSTR("psi") },
	{ 969, FHTML_LSTR("omega") },
	{ 977, FHTML_LSTR("thetasym") },
	{ 978, FHTML_LSTR("upsih") },
	{ 982, FHTML_LSTR("piv") },
	{ 8194, FHTML_LSTR("ensp") },
	{ 8195, FHTML_LSTR("emsp") },
	{ 8201, FHTML_LSTR("thinsp") },
	{ 8204, FHTML_LSTR("zwnj") },
	{ 8205, FHTML_LSTR("zwj") },
	{ 8206, FHTML_LSTR("lrm") },
	{ 8207, FHTML_LSTR("rlm") },
	{ 8211, FHTML_LSTR("ndash") },
	{ 8212, FHTML_LSTR("mdash") },
	{ 8216, FHTML_LSTR("lsquo") },
	{ 8217, FHTML_LSTR("rsquo") },
	{ 8218, FHTML_LSTR("sbquo") },
	{ 8220, FHTML_LSTR("ldquo") },
	{ 8221, FHTML_LSTR("rdquo") },
	{ 8222, FHTML_LSTR("bdquo") },
	{ 8224, FHTML_LSTR("dagger") },
	{ 8225, FHTML_LSTR("Dagger") },
	{ 8226, FHTML_LSTR("bull") },
	{ 8230, FHTML_LSTR("hellip") },
	{ 8240, FHTML_LSTR("permil") },
	{ 8242, FHTML_LSTR("prime") },
	{ 8243, FHTML_LSTR("Prime") },
	{ 8249, FHTML_LSTR("lsaquo") },
	{ 8250, FHTML_LSTR("rsaquo") },
	{ 8254, FHTML_LSTR("oline") },
	{ 8260, FHTML_LSTR("frasl") },
	{ 8364, FHTML_LSTR("euro") },
	{ 8465, FHTML_LSTR("image") },
	{ 8472, FHTML_LSTR("weierp") },
	{ 8476, FHTML_LSTR("real") },
	{ 8482, FHTML_LSTR("trade") },
	{ 8501, FHTML_LSTR("alefsym") },
	{ 8592, FHTML_LSTR("larr") },
	{ 8593, FHTML_LSTR("uarr") },
	{ 8594, FHTML_LSTR("rarr") },
	{ 8595, FHTML_LSTR("darr") },
	{ 8596, FHTML_LSTR("harr") },
	{ 8629, FHTML_LSTR("crarr") },
	{ 8656, FHTML_LSTR("lArr") },
	{ 8657, FHTML_LSTR("uArr") },
	{ 8658, FHTML_LSTR("rArr") },
	{ 8659, FHTML_LSTR("dArr") },
	{ 8660, FHTML_LSTR("hArr") },
	{ 8704, FHTML_LSTR("forall") },
	{ 8706, FHTML_LSTR("part") },
	{ 8707, FHTML_LSTR("exist") },
	{ 8709, FHTML_LSTR("empty") },
	{ 8711, FHTML_LSTR("nabla") },
	{ 8712, FHTML_LSTR("isin") },
	{ 8713, FHTML_LSTR("notin") },
	{ 8715, FHTML_LSTR("ni") },
	{ 8719, FHTML_LSTR("prod") },
	{ 8721, FHTML_LSTR("sum") },
	{ 8722, FHTML_LSTR("minus") },
	{ 8727, FHTML_LSTR("lowast") },
	{ 8730, FHTML_LSTR("radic") },
	{ 8733, FHTML_LSTR("prop") },
	{ 8734, FHTML_LSTR("infin") },
	{ 8736, FHTML_LSTR("ang") },
	{ 8743, FHTML_LSTR("and") },
	{ 8744, FHTML_LSTR("or") },
	{ 8745, FHTML_LSTR("cap") },
	{ 8746, FHTML_LSTR("cup") },
	{ 8747, FHTML_LSTR("int") },
	{ 8756, FHTML_LSTR("there4") },
	{ 8764, FHTML_LSTR("sim") },
	{ 8773, FHTML_LSTR("cong") },
	{ 8776, FHTML_LSTR("asymp") },
	{ 8800, FHTML_LSTR("ne") },
	{ 8801, FHTML_LSTR("equiv") },
	{ 8804, FHTML_LSTR("le") },
	{ 8805, FHTML_LSTR("ge") },
	{ 8834, FHTML_LSTR("sub") },
	{ 8835, FHTML_LSTR("sup") },
	{ 8836, FHTML_LSTR("nsub") },
	{ 8838, FHTML_LSTR("sube") },
	{ 8839, FHTML_LSTR("supe") },
	{ 8853, FHTML_LSTR("oplus") },
	{ 8855, FHTML_LSTR("otimes") },
	{ 8869, FHTML_LSTR("perp") },
	{ 8901, FHTML_LSTR("sdot") },
	{ 8968, FHTML_LSTR("lceil") },
	{ 8969, FHTML_LSTR("rceil") },
	{ 8970, FHTML_LSTR("lfloor") },
	{ 8971, FHTML_LSTR("rfloor") },
	{ 9001, FHTML_LSTR("lang") },
	{ 9002, FHTML_LSTR("rang") },
	{ 9674, FHTML_LSTR("loz") },
	{ 9824, FHTML_LSTR("spades") },
	{ 9827, FHTML_LSTR("clubs") },
	{ 9829, FHTML_LSTR("hearts") },
	{ 9830, FHTML_LSTR("diams") },
    { 0, 0 }
};


#if defined(XALAN_LSTRSUPPORT) && !defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
#else

FormatterToHTML::EntityPair		FormatterToHTML::s_entities[sizeof(theLocalEntities) / sizeof(theLocalEntities[0])];

void
initializeEntities(
			const LocalEntityPair			theLocalEntities[],
			FormatterToHTML::EntityPair		theEntities[])
{
	const LocalEntityPair*			theCurrentLocalEntity = theLocalEntities;
	FormatterToHTML::EntityPair*	theCurrentEntity = theEntities;

#if defined(XALAN_NON_ASCII_PLATFORM)
	XalanDOMCharVectorType			theTempVector;
#endif

	while(theCurrentLocalEntity->m_char != 0)
	{
		theCurrentEntity->m_char = theCurrentLocalEntity->m_char;

		const XalanDOMString::size_type		theLength =
			XalanDOMString::length(theCurrentLocalEntity->m_string);

		assert(theLength <= FormatterToHTML::EntityPair::eMaxLength);

#if defined(XALAN_NON_ASCII_PLATFORM)
		theTempVector.clear();

		TranscodeFromLocalCodePage(
				theCurrentLocalEntity->m_string,
				theTempVector,
				false);
		assert(theTempVector.size() == theLength);

		const XalanDOMChar* const	theString = &theTempVector[0];
#else
		const char* const	theString = theCurrentLocalEntity->m_string;
#endif

		for(XalanDOMString::size_type i = 0; i < theLength; ++i)
		{
			theCurrentEntity->m_string[i] = theString[i];
		}

		++theCurrentLocalEntity;
		++theCurrentEntity;
	}
}

#endif

const unsigned long		FormatterToHTML::s_entitiesSize =
				sizeof(s_entities) / sizeof (s_entities[0]);


void
FormatterToHTML::initialize()
{
	// Make sure there's nothing there first...
	::s_elementFlags.reset();
	::s_dummyDesc.reset();

	// New everything in a local so that an exception will clean up everything...
	XalanAutoPtr<ElementFlagsMapType>	theElementFlags(new ElementFlagsMapType);
	XalanAutoPtr<ElemDesc>				theDummyDesc(new ElemDesc(ElemDesc::BLOCK));

	// Do initialization...
	initializeElementFlagsMap(*theElementFlags.get());

	::s_doctypeHeaderStartString = XALAN_STATIC_UCODE_STRING("<!DOCTYPE HTML");

	::s_doctypeHeaderPublicString = XALAN_STATIC_UCODE_STRING(" PUBLIC \"");

	::s_doctypeHeaderSystemString = XALAN_STATIC_UCODE_STRING(" SYSTEM");

	::s_scriptString = XALAN_STATIC_UCODE_STRING("SCRIPT");

	::s_styleString = XALAN_STATIC_UCODE_STRING("STYLE");

	::s_metaString = XALAN_STATIC_UCODE_STRING("<META http-equiv=\"Content-Type\" content=\"text/html; charset=");

	// Everythings cool, so let the globals own the objects...
	::s_elementFlags = theElementFlags;
	::s_dummyDesc = theDummyDesc;

	// Update the class members...
	s_elementFlags = ::s_elementFlags.get();
	s_dummyDesc = ::s_dummyDesc.get();

#if defined(XALAN_LSTRSUPPORT) && !defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
#else
	initializeEntities(theLocalEntities, FormatterToHTML::s_entities);
#endif
}



void
FormatterToHTML::terminate()
{
	s_elementFlags = 0;
	s_dummyDesc = 0;

	::s_elementFlags.reset();
	::s_dummyDesc.reset();

	releaseMemory(::s_doctypeHeaderStartString);

	releaseMemory(::s_doctypeHeaderPublicString);

	releaseMemory(::s_doctypeHeaderSystemString);

	releaseMemory(::s_scriptString);

	releaseMemory(::s_styleString);

	releaseMemory(::s_metaString);		
}
