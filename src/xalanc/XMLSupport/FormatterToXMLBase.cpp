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
#include "FormatterToXMLBase.hpp"



#include <xercesc/sax/SAXException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/Writer.hpp>
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar	FormatterToXMLBase::s_specialChars[kSpecialsSize] =
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



FormatterToXMLBase::FormatterToXMLBase(
			Writer&					writer,
			const XalanDOMString&	version,
			const XalanDOMString&	mediaType,
			const XalanDOMString&	doctypeSystem,
			const XalanDOMString&	doctypePublic,
			bool					xmlDecl,
			const XalanDOMString&	standalone) :
	FormatterListener(OUTPUT_METHOD_XML),
	m_writer(&writer),
	m_nextIsRaw(false),
	m_doctypeSystem(doctypeSystem),
	m_doctypePublic(doctypePublic),
	m_spaceBeforeClose(false),
	m_version(version),
	m_standalone(standalone),
	m_mediaType(mediaType),
	m_newlineString(0),
	m_newlineStringLength(0),
	m_needToOutputDoctypeDecl(false),
	m_shouldWriteXMLHeader(xmlDecl),
	m_elemStack()
{
	if(isEmpty(m_doctypePublic) == false)
	{
		if(startsWith(
			m_doctypePublic,
			s_xhtmlDocTypeString) == true)
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
}



FormatterToXMLBase::~FormatterToXMLBase()
{
}



unsigned int
FormatterToXMLBase::decodeUTF16SurrogatePair(
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



XALAN_USING_XERCES(SAXException)

void
FormatterToXMLBase::throwInvalidUTF16SurrogateException(XalanDOMChar	ch)
{
	const XalanDOMString	theMessage(TranscodeFromLocalCodePage("Invalid UTF-16 surrogate detected: ") +
									   UnsignedLongToHexDOMString(ch) +
									   TranscodeFromLocalCodePage(" ?"));

	throw SAXException(c_wstr(theMessage));
}



void
FormatterToXMLBase::throwInvalidUTF16SurrogateException(
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
FormatterToXMLBase::throwInvalidCharacterException(unsigned int		ch)
{
	const XalanDOMString	theMessage(TranscodeFromLocalCodePage("Invalid character detected: ") +
									   UnsignedLongToHexDOMString(ch));

	throw SAXException(c_wstr(theMessage));
}



void
FormatterToXMLBase::flushWriter()
{
	m_writer->flush();
}



void
FormatterToXMLBase::setDocumentLocator(const LocatorType* const 	/* locator */)
{
}



void
FormatterToXMLBase::startDocument()
{
	if (m_doctypeSystem.empty() == false)
	{
		m_needToOutputDoctypeDecl = true;
	}

	if(m_shouldWriteXMLHeader == true)
	{
		writeXMLHeader();
	}
}



void
FormatterToXMLBase::endDocument()
{
	m_needToOutputDoctypeDecl = false;

	flushBuffer();
}



void
FormatterToXMLBase::characters(
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
			writeCharacters(chars, length);
		}
	}
}



void
FormatterToXMLBase::cdata(
			const XMLCh* const	ch,
			const unsigned int	length)
{
	if (length != 0)
	{
		if(m_nextIsRaw == true)
		{
			m_nextIsRaw = false;

			charactersRaw(ch, length);
		}
		else
		{
			writeCDATA(ch, length);
		}
	}
}



void
FormatterToXMLBase::processingInstruction(
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
		writeProcessingInstruction(target, data);
	}
}



void
FormatterToXMLBase::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if (length > 0)
	{
		characters(chars, length);
	}
}



Writer*
FormatterToXMLBase::getWriter() const
{
	return m_writer;
}



const XalanDOMString&
FormatterToXMLBase::getDoctypeSystem() const
{
	return m_doctypeSystem;
}



const XalanDOMString&
FormatterToXMLBase::getDoctypePublic() const
{
	return m_doctypePublic;
}



const XalanDOMString&
FormatterToXMLBase::getMediaType() const
{
	return m_mediaType;
}



void
FormatterToXMLBase::resetDocument()
{
	// I don't do anything with this yet.
}



#define FXML_SIZE(str)	((sizeof(str) / sizeof(str[0]) - 1))

const XalanDOMChar	FormatterToXMLBase::s_xhtmlDocTypeString[] =
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

const XalanDOMString::size_type		FormatterToXMLBase::s_xhtmlDocTypeStringLength =
		FXML_SIZE(s_xhtmlDocTypeString);


XALAN_CPP_NAMESPACE_END
