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
#include "FormatterToXML_UTF16.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/Writer.hpp>
#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FormatterToXML_UTF16::FormatterToXML_UTF16(
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
	m_buffer(),
	m_bufferPosition(m_buffer),
	m_bufferRemaining(kBufferSize)
{
	const XalanTranscodingServices::XalanXMLByte*	theProlog =
		XalanTranscodingServices::getStreamProlog(s_utf16String);
	assert(theProlog != 0);

	const size_t	theLength = XalanTranscodingServices::length(theProlog);
	assert(theLength == 2);

#if defined(XALAN_OLD_STYLE_CASTS)
	m_writer->write((const char*)theProlog, 0, theLength);
#else
	m_writer->write(reinterpret_cast<const char*>(theProlog), 0, theLength);
#endif
}



FormatterToXML_UTF16::~FormatterToXML_UTF16()
{
}



inline void
FormatterToXML_UTF16::flushBuffer()
{
	m_writer->write((const char*)m_buffer, 0, (m_bufferPosition - m_buffer) * sizeof m_buffer[0]);

	m_bufferPosition = m_buffer;
	m_bufferRemaining = kBufferSize;
}



inline void
FormatterToXML_UTF16::write(
			const XalanDOMChar*			theChars,
			XalanDOMString::size_type	theLength)
{
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

		for(XalanDOMString::size_type i = 0; i < theLength; ++i)
		{
			*m_bufferPosition = theChars[i];

			++m_bufferPosition;
		}

		m_bufferRemaining -= theLength;
	}
}



void
FormatterToXML_UTF16::writeName(const XalanDOMChar*		theChars)
{
	write(theChars, length(theChars));
}



void
FormatterToXML_UTF16::writeDoctypeDecl(const XalanDOMChar* 	name)
{
	// "<!DOCTYPE "
	write(s_doctypeHeaderStartString, s_doctypeHeaderStartStringLength);

	write(name);

	if(length(m_doctypePublic) != 0)
	{
		// " PUBLIC \""
		write(s_doctypeHeaderPublicString, s_doctypeHeaderPublicStringLength);
		writeName(m_doctypePublic.c_str());
		write(XalanUnicode::charQuoteMark);
		write(XalanUnicode::charSpace);
		write(XalanUnicode::charQuoteMark);
	}
	else
	{
		// " SYSTEM \""
		write(s_doctypeHeaderSystemString, s_doctypeHeaderSystemStringLength);
	}

	writeName(m_doctypeSystem.c_str());
	write(XalanUnicode::charQuoteMark);
	write(XalanUnicode::charGreaterThanSign);

	outputLineSep();
}



bool
FormatterToXML_UTF16::writeDefaultEntity(XalanDOMChar	ch)
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



void
FormatterToXML_UTF16::writeDefaultEscape(XalanDOMChar	ch)
{
	if(!writeDefaultEntity(ch))
	{
		writeNormalizedChar(ch);
	}
}



bool
FormatterToXML_UTF16::writeDefaultAttributeEntity(XalanDOMChar	ch)
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



void
FormatterToXML_UTF16::writeDefaultAttributeEscape(XalanDOMChar	ch)
{
	if(!writeDefaultAttributeEntity(ch))
	{
		writeNormalizedChar(ch);
	}
}



void
FormatterToXML_UTF16::writeParentTagEnd()
{
	if(markParentForChildren() == true)
	{
		write(char(XalanUnicode::charGreaterThanSign));
	}
}



void
FormatterToXML_UTF16::startElement(
			const XMLCh* const	name,
			AttributeListType&	attrs)
{
	generateDoctypeDecl(name);

	writeParentTagEnd();

	write(XalanUnicode::charLessThanSign);
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
FormatterToXML_UTF16::endElement(const XMLCh* const	name)
{
	const bool	hasChildNodes = childNodesWereAdded();

	if (hasChildNodes == true) 
	{
		write(XalanUnicode::charLessThanSign);
		write(XalanUnicode::charSolidus);
		writeName(name);
	}
	else
	{
		if(m_spaceBeforeClose == true)
		{
			write(XalanUnicode::charSpace);
		}

		write(XalanUnicode::charSolidus);
	}

	write(XalanUnicode::charGreaterThanSign);
}



void
FormatterToXML_UTF16::writeProcessingInstruction(
			const XMLCh*	target,
			const XMLCh*	data)
{
	writeParentTagEnd();

	write(XalanUnicode::charLessThanSign);
	write(XalanUnicode::charQuestionMark);

	writeName(target);

	const XalanDOMString::size_type		len = length(data);

	// We need to make sure there is a least one whitespace character
	// between the target and the data.
	if ( len > 0 && !isXMLWhitespace(data[0]))
	{
		write(XalanUnicode::charSpace);
	}

	writeNormalizedPIData(data, len);

	write(XalanUnicode::charQuestionMark);
	write(XalanUnicode::charGreaterThanSign);

	// If outside of the document element, then put in a new line.  This whitespace
	// is not significant.
	if (outsideDocumentElement() == true)
	{
		outputLineSep();
	}
}



void
FormatterToXML_UTF16::writeCharacters(
			const XMLCh*	chars,
			unsigned int	length)
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
			write(chars + firstIndex, i - firstIndex);

			writeDefaultEscape(chars[i++]);

			firstIndex = i;
		}
	}

	write(chars + firstIndex, i - firstIndex);
}



void
FormatterToXML_UTF16::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	writeParentTagEnd();

	write(chars, length);
}



const XalanDOMString&
FormatterToXML_UTF16::getEncoding() const
{
	return s_utf16String;
}



void
FormatterToXML_UTF16::writeAttrString(
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
			write(theString + firstIndex, i - firstIndex);

			writeDefaultAttributeEscape(ch);

			++i;

			firstIndex = i;
		}
    }

	write(theString + firstIndex, i - firstIndex);
}



void
FormatterToXML_UTF16::writeCommentData(const XalanDOMChar*	data)
{
	const XalanDOMString::size_type		len = length(data);
	XalanDOMChar						previousChar = 0;

	for (XalanDOMString::size_type i = 0; i < len; ++i)
	{
		const XalanDOMChar	currentChar = data[i];

		if (currentChar == XalanUnicode::charHyphenMinus &&
			previousChar == XalanUnicode::charHyphenMinus)
		{
			write(XalanUnicode::charSpace);
		}

		write(currentChar);

		previousChar = currentChar;
	}
		
	if (previousChar == XalanUnicode::charHyphenMinus)
	{
		write(XalanUnicode::charSpace);
	}
}



void
FormatterToXML_UTF16::writeNormalizedChar(XalanDOMChar	ch)
{
	if(XalanUnicode::charLF == ch)
	{
		outputLineSep();
	}
	else
	{
		write(ch);
	}
}



void
FormatterToXML_UTF16::writeCDATAChars(
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

			write(XalanUnicode::charRightSquareBracket);
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

			write(XalanUnicode::charGreaterThanSign);
		}
		else
		{
			if (theCurrentState != eInitialState)
			{
				theCurrentState = eInitialState;
			}

			writeNormalizedChar(theChar);
		}

		++i;
    }
}



void
FormatterToXML_UTF16::entityReference(const XMLCh* const	name)
{
	writeParentTagEnd();
	  
	write(XalanUnicode::charAmpersand);
	writeName(name);
	write(XalanUnicode::charSemicolon);
}



void
FormatterToXML_UTF16::comment(const XMLCh* const	data)
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
FormatterToXML_UTF16::writeCDATA(
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
FormatterToXML_UTF16::processAttribute(
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
FormatterToXML_UTF16::writeXMLHeader()
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

	write(s_utf16String);

	if (length(m_standalone) != 0)
	{
		write(s_xmlHeaderStandaloneString, s_xmlHeaderStandaloneStringLength);
		write(m_standalone);
	}

	write(s_xmlHeaderEndString, s_xmlHeaderEndStringLength);

	outputLineSep();
}



void
FormatterToXML_UTF16::outputLineSep()
{
	assert(m_newlineString != 0 && length(m_newlineString) == m_newlineStringLength);

	write(m_newlineString, m_newlineStringLength);
}



void
FormatterToXML_UTF16::writeNormalizedPIData(
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



static XalanDOMString	s_localUTF16String;



const XalanDOMString&	FormatterToXML_UTF16::s_utf16String = s_localUTF16String;



void
FormatterToXML_UTF16::initialize()
{
	s_localUTF16String = XalanTranscodingServices::s_utf16String;
}



void
FormatterToXML_UTF16::terminate()
{
	XalanDOMString().swap(s_localUTF16String);
}



#define FXML_SIZE(str)	((sizeof(str) / sizeof(str[0]) - 1))

const XalanDOMChar	FormatterToXML_UTF16::s_doctypeHeaderStartString[] =
{
	XalanUnicode::charLessThanSign,
	XalanUnicode::charExclamationMark,
	XalanUnicode::charLetter_D,
	XalanUnicode::charLetter_O,
	XalanUnicode::charLetter_C,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_Y,
	XalanUnicode::charLetter_P,
	XalanUnicode::charLetter_E,
	XalanUnicode::charSpace,
	XalanDOMChar(0)
};

const XalanDOMString::size_type	FormatterToXML_UTF16::s_doctypeHeaderStartStringLength =
		FXML_SIZE(s_doctypeHeaderStartString);

const XalanDOMChar	FormatterToXML_UTF16::s_doctypeHeaderPublicString[] =
{
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_P,
	XalanUnicode::charLetter_U,
	XalanUnicode::charLetter_B,
	XalanUnicode::charLetter_L,
	XalanUnicode::charLetter_I,
	XalanUnicode::charLetter_C,
	XalanUnicode::charSpace,
	XalanUnicode::charQuoteMark,
	XalanDOMChar(0)
};

const XalanDOMString::size_type	FormatterToXML_UTF16::s_doctypeHeaderPublicStringLength =
		FXML_SIZE(s_doctypeHeaderPublicString);

const XalanDOMChar	FormatterToXML_UTF16::s_doctypeHeaderSystemString[] =
{
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_S,
	XalanUnicode::charLetter_Y,
	XalanUnicode::charLetter_S,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_E,
	XalanUnicode::charLetter_M,
	XalanUnicode::charSpace,
	XalanUnicode::charQuoteMark,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_doctypeHeaderSystemStringLength =
		FXML_SIZE(s_doctypeHeaderSystemString);

const XalanDOMChar	FormatterToXML_UTF16::s_xmlHeaderStartString[] =
{
	XalanUnicode::charLessThanSign,
	XalanUnicode::charQuestionMark,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_l,
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_v,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	XalanUnicode::charEqualsSign,
	XalanUnicode::charQuoteMark,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_xmlHeaderStartStringLength =
		FXML_SIZE(s_xmlHeaderStartString);

const XalanDOMChar	FormatterToXML_UTF16::s_xmlHeaderEncodingString[] =
{
	XalanUnicode::charQuoteMark,
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	XalanUnicode::charEqualsSign,
	XalanUnicode::charQuoteMark,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_xmlHeaderEncodingStringLength =
		FXML_SIZE(s_xmlHeaderEncodingString);

const XalanDOMChar	FormatterToXML_UTF16::s_xmlHeaderStandaloneString[] =
{
	XalanUnicode::charQuoteMark,
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_e,
	XalanUnicode::charEqualsSign,
	XalanUnicode::charQuoteMark,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_xmlHeaderStandaloneStringLength =
		FXML_SIZE(s_xmlHeaderStandaloneString);

const XalanDOMChar	FormatterToXML_UTF16::s_xmlHeaderEndString[] =
{
	XalanUnicode::charQuoteMark,
	XalanUnicode::charQuestionMark,
	XalanUnicode::charGreaterThanSign,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_xmlHeaderEndStringLength =
		FXML_SIZE(s_xmlHeaderEndString);

const XalanDOMChar	FormatterToXML_UTF16::s_defaultVersionString[] =
{
	XalanUnicode::charDigit_1,
	XalanUnicode::charFullStop,
	XalanUnicode::charDigit_0,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_defaultVersionStringLength =
		FXML_SIZE(s_defaultVersionString);

const XalanDOMChar	FormatterToXML_UTF16::s_cdataOpenString[] =
{
	XalanUnicode::charLessThanSign,
	XalanUnicode::charExclamationMark,
	XalanUnicode::charLeftSquareBracket,
	XalanUnicode::charLetter_C,
	XalanUnicode::charLetter_D,
	XalanUnicode::charLetter_A,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_A,
	XalanUnicode::charLeftSquareBracket,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_cdataOpenStringLength =
		FXML_SIZE(s_cdataOpenString);

const XalanDOMChar	FormatterToXML_UTF16::s_cdataCloseString[] =
{
	XalanUnicode::charRightSquareBracket,
	XalanUnicode::charRightSquareBracket,
	XalanUnicode::charGreaterThanSign,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_cdataCloseStringLength =
		FXML_SIZE(s_cdataCloseString);


const XalanDOMChar	FormatterToXML_UTF16::s_xhtmlDocTypeString[] =
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

const XalanDOMString::size_type		FormatterToXML_UTF16::s_xhtmlDocTypeStringLength =
		FXML_SIZE(s_xhtmlDocTypeString);

const XalanDOMChar	FormatterToXML_UTF16::s_lessThanEntityString[] =
{
	XalanUnicode::charAmpersand,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_t,
	XalanUnicode::charSemicolon,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_lessThanEntityStringLength =
		FXML_SIZE(s_lessThanEntityString);

const XalanDOMChar	FormatterToXML_UTF16::s_greaterThanEntityString[] =
{
	XalanUnicode::charAmpersand,
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_t,
	XalanUnicode::charSemicolon,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_greaterThanEntityStringLength =
		FXML_SIZE(s_greaterThanEntityString);

const XalanDOMChar	FormatterToXML_UTF16::s_ampersandEntityString[] =
{
	XalanUnicode::charAmpersand,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_p,
	XalanUnicode::charSemicolon,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_ampersandEntityStringLength =
		FXML_SIZE(s_ampersandEntityString);

const XalanDOMChar	FormatterToXML_UTF16::s_quoteEntityString[] =
{
	XalanUnicode::charAmpersand,
	XalanUnicode::charLetter_q,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_t,
	XalanUnicode::charSemicolon,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_quoteEntityStringLength =
		FXML_SIZE(s_quoteEntityString);

const XalanDOMChar	FormatterToXML_UTF16::s_linefeedNCRString[] =
{
	XalanUnicode::charAmpersand,
	XalanUnicode::charNumberSign,
	XalanUnicode::charDigit_1,
	XalanUnicode::charDigit_0,
	XalanUnicode::charSemicolon,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_linefeedNCRStringLength =
		FXML_SIZE(s_linefeedNCRString);

const XalanDOMChar	FormatterToXML_UTF16::s_htabNCRString[] =
{
	XalanUnicode::charAmpersand,
	XalanUnicode::charNumberSign,
	XalanUnicode::charDigit_9,
	XalanUnicode::charSemicolon,
	XalanDOMChar(0)
};

const XalanDOMString::size_type		FormatterToXML_UTF16::s_htabNCRStringLength =
		FXML_SIZE(s_htabNCRString);


XALAN_CPP_NAMESPACE_END
