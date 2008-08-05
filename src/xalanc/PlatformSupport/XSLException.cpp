/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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



#include "LocalMsgIndex.hpp"
#include "XalanMessageLoader.hpp"
#include "XalanUnicode.hpp"
#include "DOMStringHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN

static const XalanDOMChar	s_dummy = 0;

XSLException::XSLException(
		const XalanDOMString&	theMessage,
		const XalanDOMString&	theURI,
		XalanFileLoc			theLineNumber,
		XalanFileLoc			theColumnNumber,
        MemoryManager&          theManager) :
	m_message(theMessage, theManager),
	m_uri(theURI, theManager),
	m_lineNumber(theLineNumber),
	m_columnNumber(theColumnNumber),
    m_memoryManager(theManager)
{
}



XSLException::XSLException(
			const LocatorType&		theLocator,
			const XalanDOMString&	theMessage,
            MemoryManager&          theManager) :
    m_memoryManager(theManager),
	m_message(theMessage, theManager),
	m_uri(
        theLocator.getSystemId() == 0 ? &s_dummy : theLocator.getSystemId(),
        theManager),
	m_lineNumber(theLocator.getLineNumber()),
	m_columnNumber(theLocator.getColumnNumber())
{
}



XSLException::XSLException(
		const XalanDOMString&	theMessage,
        MemoryManager&          theManager) :
    m_memoryManager(theManager),
	m_message(theMessage, theManager),
	m_uri(theManager),
	m_lineNumber(XalanLocator::getUnknownValue()),
	m_columnNumber(XalanLocator::getUnknownValue())
{
}



XSLException::XSLException(const XSLException&	other) :
    m_memoryManager(other.m_memoryManager),
	m_message(other.m_message, m_memoryManager),
	m_uri(other.m_uri, m_memoryManager),
	m_lineNumber(other.m_lineNumber),
	m_columnNumber(other.m_columnNumber)
{
}



XSLException::~XSLException()
{
}



void
XSLException::defaultFormat(XalanDOMString&		theBuffer) const
{
	defaultFormat(
        m_message,
        m_uri,
        m_lineNumber,
        m_columnNumber,
        getType(),
        theBuffer);
}


void
XSLException::defaultFormat(
			const XalanDOMChar*					theMessage,
			const XalanDOMString::size_type		theMessageLength,
			const XalanDOMChar*					theURI,
			const XalanDOMString::size_type		theURILength,
			XalanFileLoc			            theLineNumber,
			XalanFileLoc			            theColumnNumber,
			const XalanDOMChar*					theType,
			const XalanDOMString::size_type		theTypeLength,
			XalanDOMString&						theBuffer)
{
    assert(
        (theMessageLength == 0 || theMessage != 0) &&
        (theURILength == 0 || theURI != 0) &&
        (theTypeLength == 0 || theType != 0));

    if (theTypeLength != 0)
    {
	    theBuffer.append(theType, theTypeLength);
	    theBuffer += XalanDOMChar(XalanUnicode::charColon);
	    theBuffer += XalanDOMChar(XalanUnicode::charSpace);
    }

	theBuffer.append(theMessage, theMessageLength);
	theBuffer += XalanDOMChar(XalanUnicode::charSpace);
	theBuffer += XalanDOMChar(XalanUnicode::charLeftParenthesis);

    // Assume we have reasonable locator information.
    XalanMessages::Codes    theMessageID = XalanMessages::InEntity_3Param;

    XalanDOMString  theMessageBuffer(theBuffer.getMemoryManager());
    XalanDOMString  theLineNumberBuffer(theBuffer.getMemoryManager());
    XalanDOMString  theColumnNumberBuffer(theBuffer.getMemoryManager());

    if (XalanLocator::isUnknownValue(theLineNumber))
    {
        // We assume that if the line number is unknown,
        // that the column number will also be unknown.
        if (theURILength == 0)
        {
            theMessageID = XalanMessages::InUnknownEntity;
        }
        else
        {
            theMessageID = XalanMessages::InEntity_1Param;
        }
    }
    else
    {
        if (theURILength == 0)
        {
            theMessageID = XalanMessages::InUnknownEntity_2Param;
        }

        NumberToDOMString(theLineNumber, theLineNumberBuffer);
        NumberToDOMString(theColumnNumber, theColumnNumberBuffer);
    }

    XalanMessageLoader::getMessage(
        theMessageBuffer,
        theMessageID,
        theURI,
        theLineNumberBuffer.c_str(),
        theColumnNumberBuffer.c_str());

    theBuffer.append(theMessageBuffer);
	theBuffer += XalanDOMChar(XalanUnicode::charRightParenthesis);
}


XALAN_CPP_NAMESPACE_END
