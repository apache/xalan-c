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
#include "FormatterToXMLBase.hpp"



#include <xercesc/sax/SAXException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
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
	kAttributeSpecial,	// 0x9 -- horizontal tab.  Write as a numeric character reference in attribute values.
	kBothSpecial,		// 0xA -- linefeed  Normalize as requested, and write as a numeric character reference in attribute values.
	kNotSpecial,
	kNotSpecial,
	kAttributeSpecial,	// 0xD -- carriage return.  Write as a numeric character reference in attribute values.
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
	m_spaceBeforeClose(false),
	m_doctypeSystem(doctypeSystem),
	m_doctypePublic(doctypePublic),
	m_version(version),
	m_standalone(standalone),
	m_mediaType(mediaType),
	m_newlineString(0),
	m_newlineStringLength(0),
	m_needToOutputDoctypeDecl(false),
    // We must write the XML declaration if standalone is specified
    m_shouldWriteXMLHeader(xmlDecl == true ? true : standalone.length() == 0),
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
	const XalanDOMString	theMessage = XalanMessageLoader::getMessage(XalanMessages::InvalidUFT16Surrogate_2Param, UnsignedLongToHexDOMString(ch));

	throw SAXException(c_wstr(theMessage));
}



void
FormatterToXMLBase::throwInvalidUTF16SurrogateException(
			XalanDOMChar	ch,
			XalanDOMChar	next)
{
	const XalanDOMString	theMessage = XalanMessageLoader::getMessage(XalanMessages::InvalidUFT16Surrogate_2Param, UnsignedLongToHexDOMString(ch),UnsignedLongToHexDOMString(next));

	throw SAXException(c_wstr(theMessage));
}



void
FormatterToXMLBase::throwInvalidCharacterException(unsigned int		ch)
{
	const XalanDOMString	theMessage = XalanMessageLoader::getMessage(XalanMessages::InvalidCharDetected_1Param, UnsignedLongToHexDOMString(ch));

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

        // Write a newline here, so the DOCTYPE comes out on a separate line
        if (m_needToOutputDoctypeDecl == true)
        {
            outputNewline();
        }
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
