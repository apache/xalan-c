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
// Class header file...
#include "XSLException.hpp"



#include "XalanUnicode.hpp"
#include "DOMStringHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XSLException::XSLException(
		const XalanDOMString&	theMessage,
		const XalanDOMString&	theURI,
		int						theLineNumber,
		int						theColumnNumber,
		const XalanDOMString&	theType) :
	m_message(theMessage),
	m_uri(theURI),
	m_lineNumber(theLineNumber),
	m_columnNumber(theColumnNumber),
	m_type(theType)
{
}



static const XalanDOMChar	s_dummy = 0;



XSLException::XSLException(
			const LocatorType&		theLocator,
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theType) :
	m_message(theMessage),
	m_uri(theLocator.getSystemId() == 0 ? &s_dummy : theLocator.getSystemId()),
	m_lineNumber(theLocator.getLineNumber()),
	m_columnNumber(theLocator.getColumnNumber()),
	m_type(theType)
{
}



XSLException::XSLException(
		const XalanDOMString&	theMessage,
		const XalanDOMString&	theType) :
	m_message(theMessage),
	m_uri(),
	m_lineNumber(XalanLocator::getUnknownValue()),
	m_columnNumber(XalanLocator::getUnknownValue()),
	m_type(theType)
{
}



XSLException::~XSLException()
{
}



XalanDOMString
XSLException::defaultFormat() const
{
	XalanDOMString	theBuffer;

	defaultFormat(theBuffer);

	return theBuffer;
}



void
XSLException::defaultFormat(XalanDOMString&		theBuffer) const
{
	defaultFormat(m_message, m_uri, m_lineNumber, m_columnNumber, m_type, theBuffer);
}



static
XalanDOMChar	colonString[] =
{
	XalanUnicode::charColon,
	XalanUnicode::charSpace,
	0
};



static
XalanDOMChar	lineString[] =
{
	XalanUnicode::charComma,
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_e,
	XalanUnicode::charSpace,
	0
};



static
XalanDOMChar	columnString[] =
{
	XalanUnicode::charComma,
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_n,
	XalanUnicode::charSpace,
	0
};



void
XSLException::defaultFormat(
			const XalanDOMChar*					theMessage,
			const XalanDOMString::size_type		theMessageLength,
			const XalanDOMChar*					theURI,
			const XalanDOMString::size_type		theURILength,
			size_type							theLineNumber,
			size_type							theColumnNumber,
			const XalanDOMChar*					theType,
			const XalanDOMString::size_type		theTypeLength,
			XalanDOMString&						theBuffer)
{
	theBuffer.append(theType, theTypeLength);
	theBuffer.append(colonString);
	theBuffer.append(theMessage, theMessageLength);
	theBuffer += XalanDOMChar(XalanUnicode::charSpace);
	theBuffer += XalanDOMChar(XalanUnicode::charLeftParenthesis);
	theBuffer.append(theURI, theURILength);
	theBuffer += lineString;
	LongToDOMString(theLineNumber, theBuffer);
	theBuffer += columnString;
	LongToDOMString(theColumnNumber, theBuffer);
	theBuffer += XalanDOMChar(XalanUnicode::charRightParenthesis);
}



XALAN_CPP_NAMESPACE_END
