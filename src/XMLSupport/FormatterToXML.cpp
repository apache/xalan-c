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
#include "FormatterToXML.hpp"



#include <sax/AttributeList.hpp>
#include <sax/SAXException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanOutputStream.hpp>
#include <PlatformSupport/XalanTranscodingServices.hpp>
#include <PlatformSupport/Writer.hpp>



static const XalanDOMChar 	theDefaultAttrSpecialChars[] =
{
	XalanUnicode::charLessThanSign,
	XalanUnicode::charGreaterThanSign,
	XalanUnicode::charAmpersand, 
	XalanUnicode::charQuoteMark,
	XalanUnicode::charCR,
	XalanUnicode::charLF,
	0
};



FormatterToXML::FormatterToXML(
			Writer& 				writer,
			const XalanDOMString&	version,
			bool					doIndent,
			int 					indent,
			const XalanDOMString&	encoding, 
			const XalanDOMString&	mediaType,
			const XalanDOMString&	doctypeSystem,
			const XalanDOMString&	doctypePublic,
			bool					xmlDecl,
			const XalanDOMString&	standalone,
			eFormat					format,
			bool					fBufferData) :
	FormatterListener(format),
	m_writer(&writer),
	m_stream(m_writer->getStream()),
	m_maxCharacter(0),
#if !defined(XALAN_NO_DEFAULT_BUILTIN_ARRAY_INITIALIZATION)
	m_attrCharsMap(),
	m_charsMap(),
#endif
	m_shouldWriteXMLHeader(xmlDecl),
	m_ispreserve(false),
	m_doIndent(doIndent),
	m_startNewLine(false),
	m_needToOutputDocTypeDecl(true),
	m_isprevtext(false),
	m_stripCData(false),
	m_nextIsRaw(false),
	m_inCData(false),
	m_encodingIsUTF(false),
	m_doctypeSystem(doctypeSystem),
	m_doctypePublic(doctypePublic),
	m_encoding(isEmpty(encoding) == false ? encoding : XalanDOMString(XalanTranscodingServices::s_utf8String)),
	m_currentIndent(0),
	m_indent(indent),
	m_preserves(),
	m_stringBuffer(),
	m_bytesEqualChars(false),
	m_shouldFlush(true),
	m_spaceBeforeClose(false),
	m_escapeCData(false),
	m_inEntityRef(false),
	m_version(version),
	m_standalone(standalone),
	m_mediaType(mediaType),
	m_attrSpecialChars(theDefaultAttrSpecialChars),
	m_charBuf(),
	m_pos(0),
	m_byteBuf(),
	m_elemStack(),
	m_accumNameCharFunction(0),
	m_accumNameStringFunction(0),
	m_accumNameDOMStringFunction(0),
	m_accumNameArrayFunction(0),
	m_accumContentCharFunction(0),
	m_accumContentStringFunction(0),
	m_accumContentDOMStringFunction(0),
	m_accumContentArrayFunction(0)
{
	assert(isEmpty(m_encoding) == false);

	if(isEmpty(m_doctypePublic) == false)
	{
		if(startsWith(
			m_doctypePublic,
			s_xhtmlDocType) == true)
		{
			m_spaceBeforeClose = true;
		}
	}

	if (m_stream != 0)
	{
		try
		{
			m_stream->setOutputEncoding(m_encoding);
		}
		catch(const XalanOutputStream::UnsupportedEncodingException&)
		{
			// Default to UTF-8 if the requested encoding is not supported...
			m_stream->setOutputEncoding(XalanDOMString(XalanTranscodingServices::s_utf8String));

			m_encoding = XalanTranscodingServices::s_utf8String;
		}
	}

	m_maxCharacter = XalanTranscodingServices::getMaximumCharacterValue(m_encoding);

	m_encodingIsUTF = XalanTranscodingServices::encodingIsUTF8(m_encoding) ||
					  XalanTranscodingServices::encodingIsUTF16(m_encoding);

#if 1
	if (m_encodingIsUTF == true)
	{
		if (fBufferData == false)
		{
			m_accumNameCharFunction = &FormatterToXML::accumCharUTFDirect;

			m_accumContentCharFunction = &FormatterToXML::accumCharUTFDirect;

			m_accumNameStringFunction = &FormatterToXML::accumStringUTFDirect;

			m_accumContentStringFunction = &FormatterToXML::accumStringUTFDirect;

			m_accumNameDOMStringFunction = &FormatterToXML::accumDOMStringUTFDirect;

			m_accumContentDOMStringFunction = &FormatterToXML::accumDOMStringUTFDirect;

			m_accumNameArrayFunction = &FormatterToXML::accumArrayUTFDirect;

			m_accumContentArrayFunction = &FormatterToXML::accumArrayUTFDirect;
		}
		else
		{
			m_charBuf.resize(s_maxBufferSize);

			m_accumNameCharFunction = &FormatterToXML::accumCharUTF;

			m_accumContentCharFunction = &FormatterToXML::accumCharUTF;

			m_accumNameStringFunction = &FormatterToXML::accumStringUTF;

			m_accumContentStringFunction = &FormatterToXML::accumStringUTF;

			m_accumNameDOMStringFunction = &FormatterToXML::accumDOMStringUTF;

			m_accumContentDOMStringFunction = &FormatterToXML::accumDOMStringUTF;

			m_accumNameArrayFunction = &FormatterToXML::accumArrayUTF;

			m_accumContentArrayFunction = &FormatterToXML::accumArrayUTF;
		}
	}
	else
	{
		if (fBufferData == false)
		{
			m_accumNameCharFunction = &FormatterToXML::accumNameAsCharDirect;

			m_accumContentCharFunction = &FormatterToXML::accumContentAsCharDirect;
		}
		else
		{
			m_charBuf.resize(s_maxBufferSize);

			m_accumNameCharFunction = &FormatterToXML::accumNameAsChar;

			m_accumContentCharFunction = &FormatterToXML::accumContentAsChar;
		}

		m_accumNameStringFunction = &FormatterToXML::accumNameString;

		m_accumContentStringFunction = &FormatterToXML::accumContentString;

		m_accumNameDOMStringFunction = &FormatterToXML::accumNameDOMString;

		m_accumContentDOMStringFunction = &FormatterToXML::accumContentDOMString;

		m_accumNameArrayFunction = &FormatterToXML::accumNameArray;

		m_accumContentArrayFunction = &FormatterToXML::accumContentArray;
	}

	m_flushFunction = &FormatterToXML::flushChars;
#else
	if (XalanTranscodingServices::getBytesEqualChars(m_encoding) == true)
	{
		m_bytesEqualChars = true;

		if (fBufferData == false)
		{
			m_accumNameCharFunction = &FormatterToXML::accumNameAsByteDirect;

			m_accumContentCharFunction = &FormatterToXML::accumContentAsByteDirect;
		}
		else
		{
			m_byteBuf.resize(s_maxBufferSize);

			m_accumNameCharFunction = &FormatterToXML::accumNameAsByte;

			m_accumContentCharFunction = &FormatterToXML::accumContentAsByte;
		}

		m_accumNameStringFunction = &FormatterToXML::accumNameString;

		m_accumContentStringFunction = &FormatterToXML::accumContentString;

		m_accumNameDOMStringFunction = &FormatterToXML::accumNameDOMString;

		m_accumContentDOMStringFunction = &FormatterToXML::accumContentDOMString;

		m_accumNameArrayFunction = &FormatterToXML::accumNameArray;

		m_accumContentArrayFunction = &FormatterToXML::accumContentArray;

		m_flushFunction = &FormatterToXML::flushBytes;
	}
	else
	{
		if (fBufferData == false)
		{
			m_accumNameCharFunction = &FormatterToXML::accumNameAsCharDirect;

			m_accumContentCharFunction = &FormatterToXML::accumContentAsCharDirect;
		}
		else
		{
			m_charBuf.resize(s_maxBufferSize);

			m_accumNameCharFunction = &FormatterToXML::accumNameAsChar;

			m_accumContentCharFunction = &FormatterToXML::accumContentAsChar;
		}

		m_flushFunction = &FormatterToXML::flushChars;
	}
#endif

	// Do this last so we initialize the map according to the value of
	// m_maxCharacter for the encoding.
	initCharsMap();
}



FormatterToXML::~FormatterToXML()
{
}



void
FormatterToXML::initAttrCharsMap()
{
	memset(m_attrCharsMap, 0, sizeof(m_attrCharsMap));

	const XalanDOMString::size_type		nSpecials = length(m_attrSpecialChars);

	{
		for(XalanDOMString::size_type i = 0; i < nSpecials; ++i)
		{
			m_attrCharsMap[charAt(m_attrSpecialChars, i)] = 'S';
		}
	}

	m_attrCharsMap[0x0A] = 'S';
	m_attrCharsMap[0x0D] = 'S';
}



void
FormatterToXML::initCharsMap()
{
	initAttrCharsMap();

	memset(m_charsMap, 0, sizeof(m_charsMap));

	m_charsMap[XalanUnicode::charLF] = 'S';
	m_charsMap[XalanUnicode::charLessThanSign] = 'S';
	m_charsMap[XalanUnicode::charGreaterThanSign] = 'S';
	m_charsMap[XalanUnicode::charAmpersand] = 'S';

	memset(m_charsMap, 'S', 20);

	// $$$ ToDo: I believe these are redundant...
	m_charsMap[0x0A] = 'S';
	m_charsMap[0x0D] = 'S';
	m_charsMap[9] = '\0';

	assert(m_maxCharacter != 0);

	for(XalanDOMChar i = m_maxCharacter; i < SPECIALSSIZE; ++i)
	{
		m_charsMap[i] = 'S';
	}
}



void
FormatterToXML::outputDocTypeDecl(const XalanDOMChar* 	name)
{
	accumName(s_doctypeHeaderStartString);	// "<!DOCTYPE "

	accumName(name);
	  
	if(length(m_doctypePublic) != 0)
	{
		accumName(s_doctypeHeaderPublicString); // " PUBLIC \""
		accumName(m_doctypePublic);
		accumName(XalanUnicode::charQuoteMark);
		accumName(XalanUnicode::charSpace);
		accumName(XalanUnicode::charQuoteMark);
	}
	else
	{
		accumName(s_doctypeHeaderSystemString); // " SYSTEM \""
	}

	accumName(m_doctypeSystem);
	accumName(XalanUnicode::charQuoteMark);
	accumName(XalanUnicode::charGreaterThanSign);

	outputLineSep();
}



void
FormatterToXML::accumNameAsByte(XalanDOMChar	ch)
{
	if (ch > m_maxCharacter)
	{
		m_byteBuf[m_pos++] = char(XalanUnicode::charQuestionMark);
	}
	else
	{
		m_byteBuf[m_pos++] = char(ch);
	}

	if(m_pos == s_maxBufferSize)
	{
		flushBytes();
	}
}



void
FormatterToXML::accumNameAsByteDirect(XalanDOMChar	ch)
{
	assert(m_stream != 0);

	if (ch > m_maxCharacter)
	{
		m_stream->write(char(XalanUnicode::charQuestionMark));
	}
	else
	{
		m_stream->write(char(ch));
	}
}



void
FormatterToXML::accumContentAsByte(XalanDOMChar		ch)
{
	if (ch > m_maxCharacter)
	{
		writeNumberedEntityReference(ch);
	}
	else
	{
		m_byteBuf[m_pos++] = char(ch);
	}

	if(m_pos == s_maxBufferSize)
	{
		flushBytes();
	}
}



void
FormatterToXML::accumContentAsByteDirect(XalanDOMChar	ch)
{
	assert(m_stream != 0);

	if (ch > m_maxCharacter)
	{
		writeNumberedEntityReference(ch);
	}
	else
	{
		m_stream->write(char(ch));
	}
}



void
FormatterToXML::accumNameAsChar(XalanDOMChar	ch)
{
	if (ch > m_maxCharacter)
	{
		m_charBuf[m_pos++] = XalanUnicode::charQuestionMark;
	}
	else
	{
		m_charBuf[m_pos++] = ch;
	}

	if(m_pos == s_maxBufferSize)
	{
		flushChars();
	}
}



void
FormatterToXML::accumNameAsCharDirect(XalanDOMChar	ch)
{
	assert(m_stream != 0);

	if (ch > m_maxCharacter)
	{
		m_stream->write(XalanDOMChar(XalanUnicode::charQuestionMark));
	}
	else
	{
		m_stream->write(ch);
	}
}



void
FormatterToXML::accumContentAsChar(XalanDOMChar	ch)
{
	if (ch > m_maxCharacter)
	{
		writeNumberedEntityReference(ch);
	}
	else
	{
		m_charBuf[m_pos++] = ch;
	}

	if(m_pos == s_maxBufferSize)
	{
		flushChars();
	}
}



void
FormatterToXML::accumContentAsCharDirect(XalanDOMChar	ch)
{
	assert(m_stream != 0);

	if (ch > m_maxCharacter)
	{
		writeNumberedEntityReference(ch);
	}
	else
	{
		m_stream->write(ch);
	}
}



void
FormatterToXML::accumCharUTF(XalanDOMChar	ch)
{
	assert(m_maxCharacter >= 65535);
	assert(m_stream != 0);

	m_charBuf[m_pos++] = ch;

	if(m_pos == s_maxBufferSize)
	{
		flushChars();
	}
}



void
FormatterToXML::accumCharUTFDirect(XalanDOMChar	ch)
{
	assert(m_maxCharacter >= 65535);
	assert(m_stream != 0);

	m_stream->write(ch);
}



void
FormatterToXML::accumNameString(const XalanDOMChar*	chars)
{
	for(; *chars!= 0; ++chars)
	{
		accumName(*chars);
	}
}



void
FormatterToXML::accumStringUTF(const XalanDOMChar*	chars)
{
	for(; *chars!= 0; ++chars)
	{
		accumCharUTF(*chars);
	}
}



void
FormatterToXML::accumStringUTFDirect(const XalanDOMChar*	chars)
{
	assert(m_maxCharacter >= 65535);
	assert(m_stream != 0);

	m_stream->write(chars);
}



void
FormatterToXML::accumContentString(const XalanDOMChar*	chars)
{
	for(; *chars!= 0; ++chars)
	{
		accumContent(*chars);
	}
}



void
FormatterToXML::accumNameArray(
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length)
{
	const XalanDOMString::size_type		n = start + length;

	for(XalanDOMString::size_type i = start; i < n; ++i)
	{
		accumName(chars[i]);
	}
}



void
FormatterToXML::accumContentArray(
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length)
{
	const XalanDOMString::size_type		n = start + length;

	for(XalanDOMString::size_type i = start; i < n; ++i)
	{
		accumContent(chars[i]);
	}
}



void
FormatterToXML::accumArrayUTF(
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length)
{
	const XalanDOMString::size_type		n = start + length;

	for(XalanDOMString::size_type i = start; i < n; ++i)
	{
		accumCharUTF(chars[i]);
	}
}



void
FormatterToXML::accumArrayUTFDirect(
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length)
{
	assert(m_maxCharacter >= 65535);
	assert(m_stream != 0);

	m_stream->write(chars + start, length);
}



void
FormatterToXML::accumNameDOMString(const XalanDOMString&	str)
{
	accumName(c_wstr(str), 0, length(str));
}



void
FormatterToXML::accumContentDOMString(const XalanDOMString&		str)
{
	accumContent(c_wstr(str), 0, length(str));
}



void
FormatterToXML::accumDOMStringUTF(const XalanDOMString&		str)
{
	accumArrayUTF(c_wstr(str), 0, length(str));
}



void
FormatterToXML::accumDOMStringUTFDirect(const XalanDOMString&	str)
{
	assert(m_maxCharacter >= 65535);
	assert(m_stream != 0);

	m_stream->write(c_wstr(str), length(str));
}



void
FormatterToXML::throwInvalidUTF16SurrogateException(XalanDOMChar	ch)
{
	const XalanDOMString	theMessage(TranscodeFromLocalCodePage("Invalid UTF-16 surrogate detected: ") +
									   UnsignedLongToHexDOMString(ch) +
									   TranscodeFromLocalCodePage(" ?"));

	throw SAXException(c_wstr(theMessage));
}



void
FormatterToXML::throwInvalidUTF16SurrogateException(
			XalanDOMChar	ch,
			XalanDOMChar	next)
{
	const XalanDOMString	theMessage(TranscodeFromLocalCodePage("Invalid UTF-16 surrogate detected: ") +
									   UnsignedLongToHexDOMString(ch) +
									   UnsignedLongToHexDOMString(next) +
									   TranscodeFromLocalCodePage(" ?"));

	throw SAXException(c_wstr(theMessage));
}



void
FormatterToXML::accumDefaultEscape(
			XalanDOMChar				ch,
			XalanDOMString::size_type	i,
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	len,
			bool						escLF)
{
	if(!accumDefaultEntity(ch, i, chars, len, escLF))
	{
		if (0xd800 <= ch && ch < 0xdc00) 
		{
			// UTF-16 surrogate
			XalanDOMChar	next = 0;

			if (i + 1 >= len) 
			{
				throwInvalidUTF16SurrogateException(ch);
			}
			else 
			{
				next = chars[++i];

				if (!(0xdc00u <= next && next < 0xe000u))
				{
					throwInvalidUTF16SurrogateException(ch, next);
				}

				next = XalanDOMChar(((ch - 0xd800u) << 10) + next - 0xdc00u + 0x00010000u);
			}

			writeNumberedEntityReference(next);
		}
		else 
		{
			if(ch > m_maxCharacter || (ch < SPECIALSSIZE && m_attrCharsMap[ch] == 'S'))
			{
				writeNumberedEntityReference(ch);
			}
			else
			{
				accumContent(ch);
			}
		}
	}
}



bool
FormatterToXML::accumDefaultEntity(
			XalanDOMChar				ch,
			XalanDOMString::size_type	i,
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	len,
			bool						escLF)
{
	if (escLF == false &&
		XalanUnicode::charCR == ch &&
		i + 1 < len &&
		XalanUnicode::charLF == chars[i + 1]) 
	{
		outputLineSep();

		i++;
	}
	else if (escLF == false && XalanUnicode::charLF == ch) 
	{
		outputLineSep();
	}
	else if (XalanUnicode::charLessThanSign == ch) 
	{
		accumContent(XalanUnicode::charAmpersand);
		accumContent(XalanUnicode::charLetter_l);
		accumContent(XalanUnicode::charLetter_t);
		accumContent(XalanUnicode::charSemicolon);
	}
	else if (XalanUnicode::charGreaterThanSign == ch) 
	{
		accumContent(XalanUnicode::charAmpersand);
		accumContent(XalanUnicode::charLetter_g);
		accumContent(XalanUnicode::charLetter_t);
		accumContent(XalanUnicode::charSemicolon);
	}
	else if (XalanUnicode::charAmpersand == ch) 
	{
		accumContent(XalanUnicode::charAmpersand);
		accumContent(XalanUnicode::charLetter_a);
		accumContent(XalanUnicode::charLetter_m);
		accumContent(XalanUnicode::charLetter_p);
		accumContent(XalanUnicode::charSemicolon);
	}
	else if (XalanUnicode::charQuoteMark == ch) 
	{
		accumContent(XalanUnicode::charAmpersand);
		accumContent(XalanUnicode::charLetter_q);
		accumContent(XalanUnicode::charLetter_u);
		accumContent(XalanUnicode::charLetter_o);
		accumContent(XalanUnicode::charLetter_t);
		accumContent(XalanUnicode::charSemicolon);
	}
	else if (XalanUnicode::charApostrophe == ch) 
	{
		accumContent(XalanUnicode::charAmpersand);
		accumContent(XalanUnicode::charLetter_a);
		accumContent(XalanUnicode::charLetter_p);
		accumContent(XalanUnicode::charLetter_o);
		accumContent(XalanUnicode::charLetter_s);
		accumContent(XalanUnicode::charSemicolon);
	}
	else
	{
		return false;
	}

	return true;
}



void
FormatterToXML::flushChars()
{
	assert(m_charBuf.size() > 0 && m_charBuf.size() >= m_pos);

	m_writer->write(&m_charBuf[0], 0, m_pos);

	m_pos = 0;
}



void
FormatterToXML::flushBytes()
{
	assert(m_byteBuf.size() > 0 && m_byteBuf.size() >= m_pos);

	m_writer->write(&m_byteBuf[0], 0, m_pos);

	m_pos = 0;
}



void
FormatterToXML::flushWriter()
{
	assert(m_writer != 0);

	m_writer->flush();
}



void
FormatterToXML::setDocumentLocator(const Locator* const 	/* locator */)
{
	// I don't do anything with this yet.
}



void
FormatterToXML::startDocument()
{
	// Clear the buffer, just in case...
	clear(m_stringBuffer);

	if(m_inEntityRef == false)
	{
		m_needToOutputDocTypeDecl = true;
		m_startNewLine = false;

		if(m_shouldWriteXMLHeader == true)
		{
			accumName(s_xmlHeaderStartString);	// "<?xml version=\""

			if (length(m_version) != 0)
			{
				accumName(m_version);
			}
			else
			{
				accumName(s_defaultVersionString);
			}

			accumName(s_xmlHeaderEncodingString);	// "\" encoding=\""

			accumName(m_encoding);

			if (length(m_standalone) != 0)
			{
				accumName(s_xmlHeaderStandaloneString);
				accumName(m_standalone);
			}

			accumName(s_xmlHeaderEndString);

			outputLineSep();
		}	   
	}
}



void
FormatterToXML::endDocument()
{
	assert(m_flushFunction != 0);

	if(m_doIndent == true && m_isprevtext == false)
	{
		outputLineSep();
	}

	(this->*m_flushFunction)();

	flushWriter();
}



void
FormatterToXML::startElement(
			const XMLCh* const	name,
			AttributeList&		attrs)
{
    if(m_inEntityRef == false)
	{
		if(true == m_needToOutputDocTypeDecl &&
		   isEmpty(m_doctypeSystem) == false)
		{
			outputDocTypeDecl(name);

			m_needToOutputDocTypeDecl = false;
		}

		writeParentTagEnd();

		m_ispreserve = false;

		if (shouldIndent() == true &&
			m_startNewLine == true) 
		{
			indent(m_currentIndent);
		}

		m_startNewLine = true;

		accumName(XalanUnicode::charLessThanSign);
		accumName(name);

		const unsigned int	nAttrs = attrs.getLength();

		for (unsigned int i = 0;  i < nAttrs ;  i++)
		{
			processAttribute(attrs.getName(i), attrs.getValue(i));
		}

		// Flag the current element as not yet having any children.
		openElementForChildren();

		m_currentIndent += m_indent;

		m_isprevtext = false;
	}
}



void
FormatterToXML::endElement(const XMLCh* const	name)
{
	m_currentIndent -= m_indent;

	const bool	hasChildNodes = childNodesWereAdded();

	if (hasChildNodes == true) 
	{
		if (shouldIndent() == true)
		{
			indent(m_currentIndent);
		}

		accumName(XalanUnicode::charLessThanSign);
		accumName(XalanUnicode::charSolidus);
		accumName(name);
	}
	else
	{
		if(m_spaceBeforeClose == true)
		{
			accumName(XalanUnicode::charSpace);
		}

		accumName(XalanUnicode::charSolidus);
	}

	accumName(XalanUnicode::charGreaterThanSign);

	if (hasChildNodes == true) 
	{
		if (m_preserves.size() == 0)
		{
			m_ispreserve = false;
		}
		else
		{
			m_ispreserve = m_preserves.back();

			m_preserves.pop_back();
		}
	}

	m_isprevtext = false;
}



void
FormatterToXML::processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
{
	if(m_inEntityRef == false)
	{
		// Use a fairly nasty hack to tell if the next node is supposed to be 
		// unescaped text.
		if(equals(target, c_wstr(s_xsltNextIsRawString)) == true
			&& equals(data, c_wstr(s_formatterListenerString)) == true)
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

			accumName(XalanUnicode::charLessThanSign);
			accumName(XalanUnicode::charQuestionMark);
			accumName(target);

			const XalanDOMString::size_type		len = length(data);

			if ( len > 0 && !isXMLWhitespace(data[0]))
			{
				accumName(XalanUnicode::charSpace);
			}

			accumNormalizedPIData(data, len);

			accumName(XalanUnicode::charQuestionMark);
			accumName(XalanUnicode::charGreaterThanSign);

			// If outside of an element, then put in a new line.  This whitespace
			// is not significant.
			if (m_elemStack.empty() == true)
			{
				outputLineSep();
			}

			m_startNewLine = true;
		}
	}
}



void
FormatterToXML::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if(m_inEntityRef == false && length != 0)
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
		else
		{
			writeParentTagEnd();

			m_ispreserve = true;

			for (unsigned int i = 0; i < length; ++i) 
			{
				const XalanDOMChar	ch = chars[i];

				if((ch < SPECIALSSIZE &&
					m_charsMap[ch] == 'S') ||
					ch > m_maxCharacter)
				{
					accumDefaultEscape(ch, i, chars, length, false);
				}
				else
				{
					accumContent(ch);
				}
			}

			if (m_isprevtext == false)
			{
				m_isprevtext = true;
			}
		}
	}
}



void
FormatterToXML::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	if(m_inEntityRef == false)
	{
		writeParentTagEnd();

		m_ispreserve = true;

		accumContent(chars, 0, length);
	}
}



void
FormatterToXML::writeAttrString(const XalanDOMChar*		theString)
{
    const XalanDOMString::size_type		len = length(theString);

    for (XalanDOMString::size_type i = 0;  i < len;  i ++) 
    {
		const XalanDOMChar	ch = theString[i];

		if((ch < SPECIALSSIZE &&
		    m_attrCharsMap[ch] == 'S') ||
			ch > m_maxCharacter)
		{
			accumDefaultEscape(ch, i, theString, len, true);
		}
		else
		{
			accumName(ch);
		}
    }
}



void
FormatterToXML::accumCommentData(const XalanDOMChar*	data)
{
	accumName(data);
}



void
FormatterToXML::writeNormalizedChars(
			const XalanDOMChar	ch[],
			unsigned int		start,
			unsigned int		length,
			bool				isCData)
{
    unsigned int	end = start + length;

    for(unsigned int i = start; i < end; i++)
    {
		const XalanDOMChar	c = ch[i];

		if (XalanUnicode::charCR == c &&
			i + 1 < end &&
			XalanUnicode::charLF == ch[i + 1])
		{
			outputLineSep();

			i++;
		}
		else if(XalanUnicode::charLF == c)
		{
			outputLineSep();
		}
		else if(isCData == true && c > m_maxCharacter)
		{
			if(i != 0)
			{
				accumContent(s_dtdCDATACloseString);
			}

			// This needs to go into a function... 
			if (0xd800u <= unsigned(c) && unsigned(c) < 0xdc00) 
			{
				// UTF-16 surrogate
				XalanDOMChar	next = 0;

				if (i + 1 >= end) 
				{
					throwInvalidUTF16SurrogateException(c);
				}
				else 
				{
					next = ch[++i];

					if (!(0xdc00 <= next && next < 0xe000))
					{
						throwInvalidUTF16SurrogateException(c, next);
					}

					next = XalanDOMChar(((c - 0xd800) << 10) + next - 0xdc00 + 0x00010000);
				}

				writeNumberedEntityReference(next);
			}
			else
			{
				writeNumberedEntityReference(c);
			}

			if(i != 0 && i < end - 1)
			{
				// "<![CDATA["
				accumContent(XalanUnicode::charLessThanSign);
				accumContent(XalanUnicode::charExclamationMark);
				accumContent(XalanUnicode::charLeftSquareBracket);
				accumContent(XalanUnicode::charLetter_C);
				accumContent(XalanUnicode::charLetter_D);
				accumContent(XalanUnicode::charLetter_A);
				accumContent(XalanUnicode::charLetter_T);
				accumContent(XalanUnicode::charLetter_A);
				accumContent(XalanUnicode::charLeftSquareBracket);
			}
		}
		else if(isCData == true &&
				i < end - 2 &&
				XalanUnicode::charRightSquareBracket == c &&
                XalanUnicode::charRightSquareBracket == ch[i + 1] &&
				XalanUnicode::charGreaterThanSign == ch[ i + 2])
		{
			// "]]]]><![CDATA[>"
			accumContent(XalanUnicode::charRightSquareBracket);
			accumContent(XalanUnicode::charRightSquareBracket);
			accumContent(XalanUnicode::charRightSquareBracket);
			accumContent(XalanUnicode::charRightSquareBracket);
			accumContent(XalanUnicode::charGreaterThanSign);
			accumContent(XalanUnicode::charLessThanSign);
			accumContent(XalanUnicode::charExclamationMark);
			accumContent(XalanUnicode::charLeftSquareBracket);
			accumContent(XalanUnicode::charLetter_C);
			accumContent(XalanUnicode::charLetter_D);
			accumContent(XalanUnicode::charLetter_A);
			accumContent(XalanUnicode::charLetter_T);
			accumContent(XalanUnicode::charLetter_A);
			accumContent(XalanUnicode::charLeftSquareBracket);
			accumContent(XalanUnicode::charGreaterThanSign);

			i += 2;
		}
		else
		{
			if(c <= m_maxCharacter)
			{
				accumContent(c);
			}
			// This needs to go into a function...
			else if (0xd800 <= c && c < 0xdc00)
			{
				// UTF-16 surrogate
				XalanDOMChar	next = 0;

				if (i + 1 >= end) 
				{
					throwInvalidUTF16SurrogateException(c);
				}
				else
				{
					next = ch[++i];

					if (!(0xdc00 <= next && next < 0xe000))
					{
						throwInvalidUTF16SurrogateException(c, next);
					}

					next = XalanDOMChar(((c - 0xd800) << 10) + next - 0xdc00 + 0x00010000);
				}

				writeNumberedEntityReference(next);
			}
			else
			{
				writeNumberedEntityReference(c);
			}
		}
    }
}



void
FormatterToXML::writeNumberedEntityReference(unsigned long	theNumber)
{
	accumContent(XalanUnicode::charAmpersand);
	accumContent(XalanUnicode::charNumberSign);

	accumContent(UnsignedLongToDOMString(theNumber, m_stringBuffer));
	clear(m_stringBuffer);

	accumContent(XalanUnicode::charSemicolon);
}



void
FormatterToXML::entityReference(const XMLCh* const	name)
{
	writeParentTagEnd();
	  
	if (shouldIndent() == true)  
	{
		indent(m_currentIndent);
	}

	accumName(XalanUnicode::charAmpersand);
	accumName(name);
	accumName(XalanUnicode::charSemicolon);
}



void
FormatterToXML::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
#if 1
	// We need to do normalization, which is slower,
	// but there you have it...
	if (length > 0)
	{
		characters(chars, length);
	}
#else
	// We'ed like to be able to do this...
	if(m_inEntityRef == false && length != 0)
	{
		assert(isXMLWhitespace(chars, 0, length));

		if(m_inCData == true)
		{
			cdata(chars, length);
		}
		else if(m_nextIsRaw)
		{
			m_nextIsRaw = false;

			charactersRaw(chars, length);
		}
		else
		{
			writeParentTagEnd();

			m_ispreserve = true;

			accumContent(chars, 0, length);

			if (m_isprevtext == false)
			{
				m_isprevtext = true;
			}
		}
	}
#endif
}



void
FormatterToXML::resetDocument()
{
	// I don't do anything with this yet.
}



void
FormatterToXML::comment(const XMLCh* const	data)
{
	if(m_inEntityRef == false)
	{
		writeParentTagEnd();

		if (shouldIndent() == true)  
		{
			indent(m_currentIndent);
		}

		accumName(XalanUnicode::charLessThanSign);
		accumName(XalanUnicode::charExclamationMark);
		accumName(XalanUnicode::charHyphenMinus);
		accumName(XalanUnicode::charHyphenMinus);

		accumCommentData(data);

		accumName(XalanUnicode::charHyphenMinus);
		accumName(XalanUnicode::charHyphenMinus);
		accumName(XalanUnicode::charGreaterThanSign);

		m_startNewLine = true;
	}
}



void
FormatterToXML::cdata(
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
		if(m_escapeCData) // Should normally always be false.
		{
			characters(ch, length);
		}
		else
		{
			writeParentTagEnd();

			m_ispreserve = true;

			if (shouldIndent() == true)
			{
				indent(m_currentIndent);
			}

			if(m_stripCData == false)
			{
				if(length >= 1 &&
				   ch[0] <= m_maxCharacter)
				{
					// "<![CDATA["
					accumContent(XalanUnicode::charLessThanSign);
					accumContent(XalanUnicode::charExclamationMark);
					accumContent(XalanUnicode::charLeftSquareBracket);
					accumContent(XalanUnicode::charLetter_C);
					accumContent(XalanUnicode::charLetter_D);
					accumContent(XalanUnicode::charLetter_A);
					accumContent(XalanUnicode::charLetter_T);
					accumContent(XalanUnicode::charLetter_A);
					accumContent(XalanUnicode::charLeftSquareBracket);
				}
			}

			writeNormalizedChars(ch, 0, length, !m_stripCData);

			if(m_stripCData == false)
			{
				if(length >= 1 &&
				   ch[length - 1] <= m_maxCharacter)
				{
					accumContent(XalanUnicode::charRightSquareBracket);
					accumContent(XalanUnicode::charRightSquareBracket);
					accumContent(XalanUnicode::charGreaterThanSign);
				}
			}
		}
	}
}



void
FormatterToXML::writeParentTagEnd()
{
	if(!m_elemStack.empty())
	{
		// See if the parent element has already been flagged as having children.
		if(false == m_elemStack.back())
		{
			accumContent(XalanUnicode::charGreaterThanSign);

			m_isprevtext = false;

			m_elemStack.back() = true;

			m_preserves.push_back(m_ispreserve);
		}
	}
}



void
FormatterToXML::openElementForChildren()
{
	m_elemStack.push_back(false);
}



bool
FormatterToXML::childNodesWereAdded()
{
	bool	fResult = false;

	if (m_elemStack.size() > 0)
	{
		fResult = m_elemStack.back();

		m_elemStack.pop_back();
	}

	return fResult;
}



void
FormatterToXML::processAttribute(
			const XalanDOMChar* 	name,
			const XalanDOMChar* 	value)
{
	accumContent(XalanUnicode::charSpace);
	accumName(name);
	accumContent(XalanUnicode::charEqualsSign);
	accumContent(XalanUnicode::charQuoteMark);
	writeAttrString(value);
	accumContent(XalanUnicode::charQuoteMark);
}



void
FormatterToXML::outputLineSep()
{
#if defined(XALAN_NEWLINE_IS_CRLF)
	accumContent(XalanUnicode::charCR);
	accumContent(XalanUnicode::charLF);
#else
	accumContent(XalanUnicode::charLF);
#endif
}



void
FormatterToXML::printSpace(int n)
{
	for (int i = 0;  i < n;  i ++)
	{
		accumContent(XalanUnicode::charSpace);
	}
}



void
FormatterToXML::indent(int 	n)
{
	if(m_startNewLine == true)
	{
		outputLineSep();
	}

	if(m_doIndent == true)
	{
		printSpace(n);
	}
}



void
FormatterToXML::accumNormalizedPIData(
			const XalanDOMChar*		theData,
			unsigned int			theLength)
{
	// If there are any "?>" pairs in the string,
	// we have to normalize them to "? >", so they
	// won't be confused with the end tag.

	for (unsigned int i = 0; i < theLength; ++i)
	{
		const XalanDOMChar	theChar = theData[i];

		if (theChar == XalanUnicode::charQuestionMark &&
			i + 1 < theLength &&
			theData[i + 1] == XalanUnicode::charGreaterThanSign)
		{
			accumContent(XalanUnicode::charQuestionMark);
			accumContent(XalanUnicode::charSpace);
		}
		else
		{
			accumContent(theChar);
		}
	}
}



static XalanDOMString	s_defaultMIMEEncoding;

static XalanDOMString	s_doctypeHeaderStartString;

static XalanDOMString	s_doctypeHeaderPublicString;

static XalanDOMString	s_doctypeHeaderSystemString;

static XalanDOMString	s_defaultVersionString;

static XalanDOMString	s_xmlHeaderStartString;

static XalanDOMString	s_xmlHeaderEncodingString;

static XalanDOMString	s_xmlHeaderStandaloneString;

static XalanDOMString	s_xmlHeaderEndString;

static XalanDOMString	s_xhtmlDocType;

static XalanDOMString	s_dtdCDATACloseString;


const XalanDOMString&	FormatterToXML::s_defaultMIMEEncoding = ::s_defaultMIMEEncoding;

const XalanDOMString&	FormatterToXML::s_doctypeHeaderStartString = ::s_doctypeHeaderStartString;

const XalanDOMString&	FormatterToXML::s_doctypeHeaderPublicString = ::s_doctypeHeaderPublicString;

const XalanDOMString&	FormatterToXML::s_doctypeHeaderSystemString = ::s_doctypeHeaderSystemString;

const XalanDOMString&	FormatterToXML::s_defaultVersionString = ::s_defaultVersionString;

const XalanDOMString&	FormatterToXML::s_xmlHeaderStartString = ::s_xmlHeaderStartString;

const XalanDOMString&	FormatterToXML::s_xmlHeaderEncodingString = ::s_xmlHeaderEncodingString;

const XalanDOMString&	FormatterToXML::s_xmlHeaderStandaloneString = ::s_xmlHeaderStandaloneString;

const XalanDOMString&	FormatterToXML::s_xmlHeaderEndString = ::s_xmlHeaderEndString;

const XalanDOMString&			FormatterToXML::s_xhtmlDocType = ::s_xhtmlDocType;

const XalanDOMString&			FormatterToXML::s_dtdCDATACloseString = ::s_dtdCDATACloseString;

const FormatterToXML::DOMCharBufferType::size_type	FormatterToXML::s_maxBufferSize = 512;



void
FormatterToXML::initialize()
{
	::s_defaultMIMEEncoding = XALAN_STATIC_UCODE_STRING("UTF-8");

	::s_doctypeHeaderStartString = XALAN_STATIC_UCODE_STRING("<!DOCTYPE ");

	::s_doctypeHeaderPublicString = XALAN_STATIC_UCODE_STRING(" PUBLIC \"");

	::s_doctypeHeaderSystemString = XALAN_STATIC_UCODE_STRING(" SYSTEM \"");

	::s_defaultVersionString = XALAN_STATIC_UCODE_STRING("1.0");

	::s_xmlHeaderStartString = XALAN_STATIC_UCODE_STRING("<?xml version=\"");

	::s_xmlHeaderEncodingString = XALAN_STATIC_UCODE_STRING("\" encoding=\"");

	::s_xmlHeaderStandaloneString = XALAN_STATIC_UCODE_STRING("\" standalone=\"");

	::s_xmlHeaderEndString = XALAN_STATIC_UCODE_STRING("\"?>");

	::s_xhtmlDocType = XALAN_STATIC_UCODE_STRING("-//W3C//DTD XHTML");
				
	::s_dtdCDATACloseString = XALAN_STATIC_UCODE_STRING("]]>");
}



void
FormatterToXML::terminate()
{
	releaseMemory(::s_defaultMIMEEncoding);

	releaseMemory(::s_doctypeHeaderStartString);

	releaseMemory(::s_doctypeHeaderPublicString);

	releaseMemory(::s_doctypeHeaderSystemString);

	releaseMemory(::s_defaultVersionString);

	releaseMemory(::s_xmlHeaderStartString);

	releaseMemory(::s_xmlHeaderEncodingString);

	releaseMemory(::s_xmlHeaderStandaloneString);

	releaseMemory(::s_xmlHeaderEndString);

	releaseMemory(::s_xhtmlDocType);

	releaseMemory(::s_dtdCDATACloseString);
}
