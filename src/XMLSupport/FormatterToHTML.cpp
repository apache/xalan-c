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

 /**
 * @author David N. Bertoni <david_n_bertoni@lotus.com>
 */



// Class header file.
#include "FormatterToHTML.hpp"



#include <cassert>
//#include <map>



#include <sax/AttributeList.hpp>



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
			const XalanDOMString&	version,
			const XalanDOMString&	standalone,
			bool					xmlDecl) :
	FormatterToXML(
			writer,
			version,
			doIndent,
			indent,
			encoding,
			mediaType,
			doctypeSystem,
			doctypePublic,
			xmlDecl,
			standalone,
			OUTPUT_METHOD_HTML),
	m_currentElementName(),
	m_inBlockElem(false),
	m_isRawStack(),
	m_isScriptOrStyleElem(false),
	m_inScriptElemStack(),
	m_escapeURLs(true),
	m_isFirstElement(false),
	m_isUTF8(XalanTranscodingServices::encodingIsUTF8(m_encoding)),
	m_elementLevel(0),
	m_hasNamespaceStack()
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

	for(unsigned int i = 160; i < SPECIALSSIZE; i++)
	{
		m_attrCharsMap[i] = 'S';
	}
}



void
FormatterToHTML::initCharsMap()
{
	initAttrCharsMap();

	memset(m_charsMap, 0, sizeof(m_charsMap));

	m_charsMap[XalanUnicode::charLF] = 'S';
	m_charsMap[XalanUnicode::charLessThanSign] = 'S';
	m_charsMap[XalanUnicode::charGreaterThanSign] = 'S';
	m_charsMap[XalanUnicode::charAmpersand] = 'S';

	memset(m_charsMap, 'S', 10);

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
	const ElementFlagsMapType::const_iterator	i =
		s_elementFlags.find(name);

	if (i == s_elementFlags.end())
	{
		return s_dummyDesc;
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

			if (m_doIndent)
				indent(m_currentIndent);

			accumContent(s_metaString);
			accumContent(getEncoding());      
			accumContent(XalanUnicode::charQuoteMark);
			accumContent(XalanUnicode::charGreaterThanSign);
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
		XalanDOMChar		ch,
		unsigned int		i,
		const XalanDOMChar	chars[],
		unsigned int		len,
		bool				escLF)
{
	if(FormatterToXML::accumDefaultEntity(ch, i, chars, len, escLF) == false)
	{	
		const XalanEntityReferenceMapType::const_iterator	theIterator = s_xalanHTMLEntities.find(ch);

		if (theIterator == s_xalanHTMLEntitiesIteratorEnd)	
		{
			return false;
		}
		else
		{
			copyEntityIntoBuffer((*theIterator).second);			
		}
	}

	return true;
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
			const XalanDOMChar*		theString,
			unsigned int			theLength)
{
	assert(theString != 0);

	if (theLength == unsigned(-1))
	{
		theLength = length(theString);
	}

	for (unsigned int i = 0; i < theLength; ++i) 
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
				unsigned int	next = 0;

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

					next = ((ch - 0xd800) << 10) + next - 0xdc00 + 0x00010000;
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

    const unsigned int	theLength = length(theString);

    for (unsigned int i = 0;  i < theLength;  i ++)
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

				unsigned int next = 0;

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

					next = ((ch - 0xd800) << 10) + next -0xdc00 + 0x00010000;
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
	const unsigned int	len = length(s);

    accumContent(XalanUnicode::charAmpersand);

    for(unsigned int i = 0; i < len; ++i)
    {
		accumContent(s[i]);
    }

    accumContent(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::copyEntityIntoBuffer(const XalanDOMString&		s)
{
	const unsigned int	len = length(s);

    accumContent(XalanUnicode::charAmpersand);

    for(unsigned int i = 0; i < len; ++i)
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

	const unsigned int	len = length(theString);

    for (unsigned int i = 0; i < len; ++i)
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
				if(ch <= 0x7F)
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
			else if (ch == XalanUnicode::charSpace)
			{
				accumHexNumber(ch);
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
		const unsigned int	theLength = length(theElementName);
		const unsigned int	theColonIndex = indexOf(theElementName, XalanUnicode::charColon);

		const XalanDOMString*	thePrefix = &s_emptyString;

		if (theColonIndex < theLength)
		{
			m_stringBuffer = substring(theElementName, 0, theColonIndex);

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


typedef FormatterToHTML::XalanEntityReferenceMapType	XalanEntityReferenceMapType;



static void
initializeXalanEntityReferenceMap1(XalanEntityReferenceMapType&	theMap)
{
	typedef XalanEntityReferenceMapType::value_type	value_type;

    //#    
    //# Character markup-significant  
    //#  
	// currently handled by FormatterToXML::accumDefaultEntity
    //theMap[34] = XALAN_STATIC_UCODE_STRING("quot");
    //theMap[38] = XALAN_STATIC_UCODE_STRING("amp");
    //theMap[60] = XALAN_STATIC_UCODE_STRING("lt");
    //theMap[62] = XALAN_STATIC_UCODE_STRING("gt");
    theMap[160] = XALAN_STATIC_UCODE_STRING("nbsp");
    //#    
    //# Character ISO 8859-1 characters
    //#    
    theMap[161] = XALAN_STATIC_UCODE_STRING("iexcl");
    theMap[162] = XALAN_STATIC_UCODE_STRING("cent");
    theMap[163] = XALAN_STATIC_UCODE_STRING("pound");
    theMap[164] = XALAN_STATIC_UCODE_STRING("curren");
    theMap[165] = XALAN_STATIC_UCODE_STRING("yen");
    theMap[166] = XALAN_STATIC_UCODE_STRING("brvbar");
    theMap[167] = XALAN_STATIC_UCODE_STRING("sect");
    theMap[168] = XALAN_STATIC_UCODE_STRING("uml");
    theMap[169] = XALAN_STATIC_UCODE_STRING("copy");
    theMap[170] = XALAN_STATIC_UCODE_STRING("ordf");
    theMap[171] = XALAN_STATIC_UCODE_STRING("laquo");
    theMap[172] = XALAN_STATIC_UCODE_STRING("not");
    theMap[173] = XALAN_STATIC_UCODE_STRING("shy");
    theMap[174] = XALAN_STATIC_UCODE_STRING("reg");
    theMap[175] = XALAN_STATIC_UCODE_STRING("macr");
    theMap[176] = XALAN_STATIC_UCODE_STRING("deg");
    theMap[177] = XALAN_STATIC_UCODE_STRING("plusmn");
    theMap[178] = XALAN_STATIC_UCODE_STRING("sup2");
    theMap[179] = XALAN_STATIC_UCODE_STRING("sup3");
    theMap[180] = XALAN_STATIC_UCODE_STRING("acute");
    theMap[181] = XALAN_STATIC_UCODE_STRING("micro");
    theMap[182] = XALAN_STATIC_UCODE_STRING("para");
    theMap[183] = XALAN_STATIC_UCODE_STRING("middot");
    theMap[184] = XALAN_STATIC_UCODE_STRING("cedil");
    theMap[185] = XALAN_STATIC_UCODE_STRING("sup1");
    theMap[186] = XALAN_STATIC_UCODE_STRING("ordm");
    theMap[187] = XALAN_STATIC_UCODE_STRING("raquo");
    theMap[188] = XALAN_STATIC_UCODE_STRING("frac14");
    theMap[189] = XALAN_STATIC_UCODE_STRING("frac12");
    theMap[190] = XALAN_STATIC_UCODE_STRING("frac34");
    theMap[191] = XALAN_STATIC_UCODE_STRING("iquest");
    theMap[192] = XALAN_STATIC_UCODE_STRING("Agrave");
    theMap[193] = XALAN_STATIC_UCODE_STRING("Aacute");
    theMap[194] = XALAN_STATIC_UCODE_STRING("Acirc");
    theMap[195] = XALAN_STATIC_UCODE_STRING("Atilde");
    theMap[196] = XALAN_STATIC_UCODE_STRING("Auml");
    theMap[197] = XALAN_STATIC_UCODE_STRING("Aring");
    theMap[198] = XALAN_STATIC_UCODE_STRING("AElig");
    theMap[199] = XALAN_STATIC_UCODE_STRING("Ccedil");
    theMap[200] = XALAN_STATIC_UCODE_STRING("Egrave");
    theMap[201] = XALAN_STATIC_UCODE_STRING("Eacute");
    theMap[202] = XALAN_STATIC_UCODE_STRING("Ecirc");
    theMap[203] = XALAN_STATIC_UCODE_STRING("Euml");
    theMap[204] = XALAN_STATIC_UCODE_STRING("Igrave");
    theMap[205] = XALAN_STATIC_UCODE_STRING("Iacute");
    theMap[206] = XALAN_STATIC_UCODE_STRING("Icirc");
    theMap[207] = XALAN_STATIC_UCODE_STRING("Iuml");
    theMap[208] = XALAN_STATIC_UCODE_STRING("ETH");
    theMap[209] = XALAN_STATIC_UCODE_STRING("Ntilde");
    theMap[210] = XALAN_STATIC_UCODE_STRING("Ograve");
    theMap[211] = XALAN_STATIC_UCODE_STRING("Oacute");
    theMap[212] = XALAN_STATIC_UCODE_STRING("Ocirc");
    theMap[213] = XALAN_STATIC_UCODE_STRING("Otilde");
    theMap[214] = XALAN_STATIC_UCODE_STRING("Ouml");
    theMap[215] = XALAN_STATIC_UCODE_STRING("times");
    theMap[216] = XALAN_STATIC_UCODE_STRING("Oslash");
    theMap[217] = XALAN_STATIC_UCODE_STRING("Ugrave");
    theMap[218] = XALAN_STATIC_UCODE_STRING("Uacute");
    theMap[219] = XALAN_STATIC_UCODE_STRING("Ucirc");
    theMap[220] = XALAN_STATIC_UCODE_STRING("Uuml");
    theMap[221] = XALAN_STATIC_UCODE_STRING("Yacute");
    theMap[222] = XALAN_STATIC_UCODE_STRING("THORN");
    theMap[223] = XALAN_STATIC_UCODE_STRING("szlig");
    theMap[224] = XALAN_STATIC_UCODE_STRING("agrave");
    theMap[225] = XALAN_STATIC_UCODE_STRING("aacute");
    theMap[226] = XALAN_STATIC_UCODE_STRING("acirc");
    theMap[227] = XALAN_STATIC_UCODE_STRING("atilde");
    theMap[228] = XALAN_STATIC_UCODE_STRING("auml");
    theMap[229] = XALAN_STATIC_UCODE_STRING("aring");
    theMap[230] = XALAN_STATIC_UCODE_STRING("aelig");
    theMap[231] = XALAN_STATIC_UCODE_STRING("ccedil");
    theMap[232] = XALAN_STATIC_UCODE_STRING("egrave");
    theMap[233] = XALAN_STATIC_UCODE_STRING("eacute");
    theMap[234] = XALAN_STATIC_UCODE_STRING("ecirc");
    theMap[235] = XALAN_STATIC_UCODE_STRING("euml");
    theMap[236] = XALAN_STATIC_UCODE_STRING("igrave");
    theMap[237] = XALAN_STATIC_UCODE_STRING("iacute");
    theMap[238] = XALAN_STATIC_UCODE_STRING("icirc");
    theMap[239] = XALAN_STATIC_UCODE_STRING("iuml");
    theMap[240] = XALAN_STATIC_UCODE_STRING("eth");
    theMap[241] = XALAN_STATIC_UCODE_STRING("ntilde");
    theMap[242] = XALAN_STATIC_UCODE_STRING("ograve");
    theMap[243] = XALAN_STATIC_UCODE_STRING("oacute");
    theMap[244] = XALAN_STATIC_UCODE_STRING("ocirc");
    theMap[245] = XALAN_STATIC_UCODE_STRING("otilde");
    theMap[246] = XALAN_STATIC_UCODE_STRING("ouml");
    theMap[247] = XALAN_STATIC_UCODE_STRING("divide");
    theMap[248] = XALAN_STATIC_UCODE_STRING("oslash");
    theMap[249] = XALAN_STATIC_UCODE_STRING("ugrave");
    theMap[250] = XALAN_STATIC_UCODE_STRING("uacute");
    theMap[251] = XALAN_STATIC_UCODE_STRING("ucirc");
    theMap[252] = XALAN_STATIC_UCODE_STRING("uuml");
    theMap[253] = XALAN_STATIC_UCODE_STRING("yacute");
    theMap[254] = XALAN_STATIC_UCODE_STRING("thorn");
    theMap[255] = XALAN_STATIC_UCODE_STRING("yuml");
}



static void
initializeXalanEntityReferenceMap2(XalanEntityReferenceMapType&	theMap)
{
	typedef XalanEntityReferenceMapType::value_type	value_type;

    //#    
    //# Character symbols, mathematical symbols,
    //#    
    //# Latin   
    theMap[402] = XALAN_STATIC_UCODE_STRING("fnof");
    //#    
    //# Greek   
    theMap[913] = XALAN_STATIC_UCODE_STRING("Alpha");
    theMap[914] = XALAN_STATIC_UCODE_STRING("Beta");
    theMap[915] = XALAN_STATIC_UCODE_STRING("Gamma");
    theMap[916] = XALAN_STATIC_UCODE_STRING("Delta");
    theMap[917] = XALAN_STATIC_UCODE_STRING("Epsilon");
    theMap[918] = XALAN_STATIC_UCODE_STRING("Zeta");
    theMap[919] = XALAN_STATIC_UCODE_STRING("Eta");
    theMap[920] = XALAN_STATIC_UCODE_STRING("Theta");
    theMap[921] = XALAN_STATIC_UCODE_STRING("Iota");
    theMap[922] = XALAN_STATIC_UCODE_STRING("Kappa");
    theMap[923] = XALAN_STATIC_UCODE_STRING("Lambda");
    theMap[924] = XALAN_STATIC_UCODE_STRING("Mu");
    theMap[925] = XALAN_STATIC_UCODE_STRING("Nu");
    theMap[926] = XALAN_STATIC_UCODE_STRING("Xi");
    theMap[927] = XALAN_STATIC_UCODE_STRING("Omicron");
    theMap[928] = XALAN_STATIC_UCODE_STRING("Pi");
    theMap[929] = XALAN_STATIC_UCODE_STRING("Rho");
    theMap[931] = XALAN_STATIC_UCODE_STRING("Sigma");
    theMap[932] = XALAN_STATIC_UCODE_STRING("Tau");
    theMap[933] = XALAN_STATIC_UCODE_STRING("Upsilon");
    theMap[934] = XALAN_STATIC_UCODE_STRING("Phi");
    theMap[935] = XALAN_STATIC_UCODE_STRING("Chi");
    theMap[936] = XALAN_STATIC_UCODE_STRING("Psi");
    theMap[937] = XALAN_STATIC_UCODE_STRING("Omega");
    theMap[945] = XALAN_STATIC_UCODE_STRING("alpha");
    theMap[946] = XALAN_STATIC_UCODE_STRING("beta");
    theMap[947] = XALAN_STATIC_UCODE_STRING("gamma");
    theMap[948] = XALAN_STATIC_UCODE_STRING("delta");
    theMap[949] = XALAN_STATIC_UCODE_STRING("epsilon");
    theMap[950] = XALAN_STATIC_UCODE_STRING("zeta");
    theMap[951] = XALAN_STATIC_UCODE_STRING("eta");
    theMap[952] = XALAN_STATIC_UCODE_STRING("theta");
    theMap[953] = XALAN_STATIC_UCODE_STRING("iota");
    theMap[954] = XALAN_STATIC_UCODE_STRING("kappa");
    theMap[955] = XALAN_STATIC_UCODE_STRING("lambda");
    theMap[956] = XALAN_STATIC_UCODE_STRING("mu");
    theMap[957] = XALAN_STATIC_UCODE_STRING("nu");
    theMap[958] = XALAN_STATIC_UCODE_STRING("xi");
    theMap[959] = XALAN_STATIC_UCODE_STRING("omicron");
    theMap[960] = XALAN_STATIC_UCODE_STRING("pi");
    theMap[961] = XALAN_STATIC_UCODE_STRING("rho");
    theMap[962] = XALAN_STATIC_UCODE_STRING("sigmaf");
    theMap[963] = XALAN_STATIC_UCODE_STRING("sigma");
    theMap[964] = XALAN_STATIC_UCODE_STRING("tau");
    theMap[965] = XALAN_STATIC_UCODE_STRING("upsilon");
    theMap[966] = XALAN_STATIC_UCODE_STRING("phi");
    theMap[967] = XALAN_STATIC_UCODE_STRING("chi");
    theMap[968] = XALAN_STATIC_UCODE_STRING("psi");
    theMap[969] = XALAN_STATIC_UCODE_STRING("omega");
    theMap[977] = XALAN_STATIC_UCODE_STRING("thetasym");
    theMap[978] = XALAN_STATIC_UCODE_STRING("upsih");
    theMap[982] = XALAN_STATIC_UCODE_STRING("piv");
}



static void
initializeXalanEntityReferenceMap3(XalanEntityReferenceMapType&	theMap)
{
	typedef XalanEntityReferenceMapType::value_type	value_type;

    //#    
    //# General   
    theMap[8226] = XALAN_STATIC_UCODE_STRING("bull");
    theMap[8230] = XALAN_STATIC_UCODE_STRING("hellip");
    theMap[8242] = XALAN_STATIC_UCODE_STRING("prime");
    theMap[8243] = XALAN_STATIC_UCODE_STRING("Prime");
    theMap[8254] = XALAN_STATIC_UCODE_STRING("oline");
    theMap[8260] = XALAN_STATIC_UCODE_STRING("frasl");
    //#    
    //# Letterlike   
    theMap[8472] = XALAN_STATIC_UCODE_STRING("weierp");
    theMap[8465] = XALAN_STATIC_UCODE_STRING("image");
    theMap[8476] = XALAN_STATIC_UCODE_STRING("real");
    theMap[8482] = XALAN_STATIC_UCODE_STRING("trade");
    theMap[8501] = XALAN_STATIC_UCODE_STRING("alefsym");
    //#    
    //# Arrows   
    theMap[8592] = XALAN_STATIC_UCODE_STRING("larr");
    theMap[8593] = XALAN_STATIC_UCODE_STRING("uarr");
    theMap[8594] = XALAN_STATIC_UCODE_STRING("rarr");
    theMap[8595] = XALAN_STATIC_UCODE_STRING("darr");
    theMap[8596] = XALAN_STATIC_UCODE_STRING("harr");
    theMap[8629] = XALAN_STATIC_UCODE_STRING("crarr");
    theMap[8656] = XALAN_STATIC_UCODE_STRING("lArr");
    theMap[8657] = XALAN_STATIC_UCODE_STRING("uArr");
    theMap[8658] = XALAN_STATIC_UCODE_STRING("rArr");
    theMap[8659] = XALAN_STATIC_UCODE_STRING("dArr");
    theMap[8660] = XALAN_STATIC_UCODE_STRING("hArr");
    //#    
    //# Mathematical   
    theMap[8704] = XALAN_STATIC_UCODE_STRING("forall");
    theMap[8706] = XALAN_STATIC_UCODE_STRING("part");
    theMap[8707] = XALAN_STATIC_UCODE_STRING("exist");
    theMap[8709] = XALAN_STATIC_UCODE_STRING("empty");
    theMap[8711] = XALAN_STATIC_UCODE_STRING("nabla");
    theMap[8712] = XALAN_STATIC_UCODE_STRING("isin");
    theMap[8713] = XALAN_STATIC_UCODE_STRING("notin");
    theMap[8715] = XALAN_STATIC_UCODE_STRING("ni");
    theMap[8719] = XALAN_STATIC_UCODE_STRING("prod");
    theMap[8721] = XALAN_STATIC_UCODE_STRING("sum");
    theMap[8722] = XALAN_STATIC_UCODE_STRING("minus");
    theMap[8727] = XALAN_STATIC_UCODE_STRING("lowast");
    theMap[8730] = XALAN_STATIC_UCODE_STRING("radic");
    theMap[8733] = XALAN_STATIC_UCODE_STRING("prop");
    theMap[8734] = XALAN_STATIC_UCODE_STRING("infin");
    theMap[8736] = XALAN_STATIC_UCODE_STRING("ang");
    theMap[8743] = XALAN_STATIC_UCODE_STRING("and");
    theMap[8744] = XALAN_STATIC_UCODE_STRING("or");
    theMap[8745] = XALAN_STATIC_UCODE_STRING("cap");
    theMap[8746] = XALAN_STATIC_UCODE_STRING("cup");
    theMap[8747] = XALAN_STATIC_UCODE_STRING("int");
    theMap[8756] = XALAN_STATIC_UCODE_STRING("there4");
    theMap[8764] = XALAN_STATIC_UCODE_STRING("sim");
    theMap[8773] = XALAN_STATIC_UCODE_STRING("cong");
    theMap[8776] = XALAN_STATIC_UCODE_STRING("asymp");
    theMap[8800] = XALAN_STATIC_UCODE_STRING("ne");
    theMap[8801] = XALAN_STATIC_UCODE_STRING("equiv");
    theMap[8804] = XALAN_STATIC_UCODE_STRING("le");
    theMap[8805] = XALAN_STATIC_UCODE_STRING("ge");
    theMap[8834] = XALAN_STATIC_UCODE_STRING("sub");
    theMap[8835] = XALAN_STATIC_UCODE_STRING("sup");
    theMap[8836] = XALAN_STATIC_UCODE_STRING("nsub");
    theMap[8838] = XALAN_STATIC_UCODE_STRING("sube");
    theMap[8839] = XALAN_STATIC_UCODE_STRING("supe");
    theMap[8853] = XALAN_STATIC_UCODE_STRING("oplus");
    theMap[8855] = XALAN_STATIC_UCODE_STRING("otimes");
    theMap[8869] = XALAN_STATIC_UCODE_STRING("perp");
    theMap[8901] = XALAN_STATIC_UCODE_STRING("sdot");
}



static void
initializeXalanEntityReferenceMap4(XalanEntityReferenceMapType&	theMap)
{
	typedef XalanEntityReferenceMapType::value_type	value_type;

    //#    
    //# Miscellaneous   
    theMap[8968] = XALAN_STATIC_UCODE_STRING("lceil");
    theMap[8969] = XALAN_STATIC_UCODE_STRING("rceil");
    theMap[8970] = XALAN_STATIC_UCODE_STRING("lfloor");
    theMap[8971] = XALAN_STATIC_UCODE_STRING("rfloor");
    theMap[9001] = XALAN_STATIC_UCODE_STRING("lang");
    theMap[9002] = XALAN_STATIC_UCODE_STRING("rang");
    //#    
    //# Geometric   
    theMap[9674] = XALAN_STATIC_UCODE_STRING("loz");
    //#    
    //# Miscellaneous   
    theMap[9824] = XALAN_STATIC_UCODE_STRING("spades");
    theMap[9827] = XALAN_STATIC_UCODE_STRING("clubs");
    theMap[9829] = XALAN_STATIC_UCODE_STRING("hearts");
    theMap[9830] = XALAN_STATIC_UCODE_STRING("diams");
    //#    
    //# Character internationalization characters 
    //#    
    //# Latin   
    theMap[338] = XALAN_STATIC_UCODE_STRING("OElig");
    theMap[339] = XALAN_STATIC_UCODE_STRING("oelig");
    //#    
    //# May not be supported Comment out???
    theMap[352] = XALAN_STATIC_UCODE_STRING("Scaron");
    theMap[353] = XALAN_STATIC_UCODE_STRING("scaron");
    theMap[376] = XALAN_STATIC_UCODE_STRING("Yuml");
    //#    
    //# Spacing   
    theMap[710] = XALAN_STATIC_UCODE_STRING("circ");
    theMap[732] = XALAN_STATIC_UCODE_STRING("tilde");
    //#    
    //# General   
    theMap[8194] = XALAN_STATIC_UCODE_STRING("ensp");
    theMap[8195] = XALAN_STATIC_UCODE_STRING("emsp");
    theMap[8201] = XALAN_STATIC_UCODE_STRING("thinsp");
    theMap[8204] = XALAN_STATIC_UCODE_STRING("zwnj");
    theMap[8205] = XALAN_STATIC_UCODE_STRING("zwj");
    theMap[8206] = XALAN_STATIC_UCODE_STRING("lrm");
    theMap[8207] = XALAN_STATIC_UCODE_STRING("rlm");
    theMap[8211] = XALAN_STATIC_UCODE_STRING("ndash");
    theMap[8212] = XALAN_STATIC_UCODE_STRING("mdash");
    theMap[8216] = XALAN_STATIC_UCODE_STRING("lsquo");
    theMap[8217] = XALAN_STATIC_UCODE_STRING("rsquo");
    theMap[8218] = XALAN_STATIC_UCODE_STRING("sbquo");
    theMap[8220] = XALAN_STATIC_UCODE_STRING("ldquo");
    theMap[8221] = XALAN_STATIC_UCODE_STRING("rdquo");
    theMap[8222] = XALAN_STATIC_UCODE_STRING("bdquo");
    theMap[8224] = XALAN_STATIC_UCODE_STRING("dagger");
    theMap[8225] = XALAN_STATIC_UCODE_STRING("Dagger");
    theMap[8240] = XALAN_STATIC_UCODE_STRING("permil");
    theMap[8249] = XALAN_STATIC_UCODE_STRING("lsaquo");
    theMap[8250] = XALAN_STATIC_UCODE_STRING("rsaquo");
    theMap[8364] = XALAN_STATIC_UCODE_STRING("euro");
}



void
FormatterToHTML::initializeXalanEntityReferenceMap(XalanEntityReferenceMapType&	theMap)
{
	initializeXalanEntityReferenceMap1(theMap);
	initializeXalanEntityReferenceMap2(theMap);
	initializeXalanEntityReferenceMap3(theMap);
	initializeXalanEntityReferenceMap4(theMap);
}



static FormatterToHTML::ElementFlagsMapType			s_elementFlags;


const FormatterToHTML::ElementFlagsMapType&			FormatterToHTML::s_elementFlags = ::s_elementFlags;


static FormatterToHTML::XalanEntityReferenceMapType	s_xalanHTMLEntities;


const FormatterToHTML::XalanEntityReferenceMapType&	FormatterToHTML::s_xalanHTMLEntities = ::s_xalanHTMLEntities;


static FormatterToHTML::XalanEntityReferenceMapType::const_iterator s_xalanHTMLEntitiesIteratorEnd; 


const  FormatterToHTML::XalanEntityReferenceMapType::const_iterator& FormatterToHTML::s_xalanHTMLEntitiesIteratorEnd = ::s_xalanHTMLEntitiesIteratorEnd;


const FormatterToHTML::ElemDesc						FormatterToHTML::s_dummyDesc(FormatterToHTML::ElemDesc::BLOCK);


static XalanDOMString	s_doctypeHeaderStartString;

static XalanDOMString	s_doctypeHeaderPublicString;

static XalanDOMString	s_doctypeHeaderSystemString;

static XalanDOMString	s_scriptString;

static XalanDOMString	s_styleString;

static XalanDOMString	s_ampString;

static XalanDOMString	s_fnofString;

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

const XalanDOMString&	FormatterToHTML::s_fnofString =
			::s_fnofString;

const XalanDOMString&	FormatterToHTML::s_metaString =
			::s_metaString;



void
FormatterToHTML::initialize()
{
	initializeElementFlagsMap(::s_elementFlags);

	initializeXalanEntityReferenceMap(::s_xalanHTMLEntities);
	
	::s_xalanHTMLEntitiesIteratorEnd = ::s_xalanHTMLEntities.end();

	::s_doctypeHeaderStartString = XALAN_STATIC_UCODE_STRING("<!DOCTYPE HTML");

	::s_doctypeHeaderPublicString = XALAN_STATIC_UCODE_STRING(" PUBLIC \"");

	::s_doctypeHeaderSystemString = XALAN_STATIC_UCODE_STRING(" SYSTEM");

	::s_scriptString = XALAN_STATIC_UCODE_STRING("SCRIPT");

	::s_styleString = XALAN_STATIC_UCODE_STRING("STYLE");

	::s_fnofString = XALAN_STATIC_UCODE_STRING("fnof");

	::s_metaString = XALAN_STATIC_UCODE_STRING("<META http-equiv=\"Content-Type\" content=\"text/html; charset=");
}



void
FormatterToHTML::terminate()
{
	ElementFlagsMapType().swap(::s_elementFlags);

	XalanEntityReferenceMapType().swap(::s_xalanHTMLEntities);

	releaseMemory(::s_doctypeHeaderStartString);

	releaseMemory(::s_doctypeHeaderPublicString);

	releaseMemory(::s_doctypeHeaderSystemString);

	releaseMemory(::s_scriptString);

	releaseMemory(::s_styleString);

	releaseMemory(::s_fnofString);

	releaseMemory(::s_metaString);		
}
