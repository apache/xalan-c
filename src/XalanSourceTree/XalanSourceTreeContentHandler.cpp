/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.	All rights
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
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache\@apache.org.
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
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#include "XalanSourceTreeContentHandler.hpp"



#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeElement.hpp"



XalanSourceTreeContentHandler::XalanSourceTreeContentHandler(
			XalanSourceTreeDocument*	theDocument,
			bool						fAccumulateText) :
	ContentHandler(),
	DTDHandler(),
	m_document(theDocument),
	m_currentElement(0),
	m_elementStack(),
	m_ownsDocument(false),
	m_accumulateText(fAccumulateText),
	m_textBuffer()
{
}



XalanSourceTreeContentHandler::~XalanSourceTreeContentHandler()
{
	if (m_ownsDocument == true)
	{
		delete m_document;
	}
}



void
XalanSourceTreeContentHandler::characters(
			const	XMLCh* const	chars,
			const unsigned int		length)
{
	if (m_currentElement == 0)
	{
		if (isXMLWhitespace(chars) == false)
		{
			throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
		}
	}
	else if (m_accumulateText == true)
	{
		append(m_textBuffer, chars, length);
	}
	else
	{
		doCharacters(chars, length);
	}
}



void
XalanSourceTreeContentHandler::endDocument()
{
	assert(m_elementStack.empty() == true);

	assert(isEmpty(m_textBuffer) == true);
}



void
XalanSourceTreeContentHandler::endElement(
			const XMLCh* const	/* uri */, 
			const XMLCh* const	/* localname */, 
			const XMLCh* const	/* qname */)
{
	processAccumulatedText();

	assert(m_elementStack.empty() == false);

	// Pop the element of the stack...
	m_elementStack.pop_back();

	// Get the element from the back of the
	// stack, if any...
	if (m_elementStack.empty() == false)
	{
		m_currentElement = m_elementStack.back();
	}
	else
	{
		m_currentElement = 0;
	}
}



void
XalanSourceTreeContentHandler::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if (m_currentElement == 0)
	{
		throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
	}
	else
	{
		processAccumulatedText();

		m_currentElement->appendChildNode(m_document->createTextIWSNode(chars, length, m_currentElement));
	}
}



void
XalanSourceTreeContentHandler::processingInstruction(
		const XMLCh* const	target,
		const XMLCh* const	data)
{
	if (m_currentElement == 0)
	{
		assert(m_document != 0);

		m_document->appendChildNode(m_document->createProcessingInstructionNode(target, data));
	}
	else
	{
		processAccumulatedText();

		m_currentElement->appendChildNode(m_document->createProcessingInstructionNode(target, data, m_currentElement));
	}
}



void
XalanSourceTreeContentHandler::setDocumentLocator(const Locator* const	/* locator */)
{
}



void
XalanSourceTreeContentHandler::startDocument()
{
	if (m_document == 0)
	{
		m_document = new XalanSourceTreeDocument;
	}
	else if (m_ownsDocument == true)
	{
		delete m_document;

		m_document = new XalanSourceTreeDocument;
	}

	m_elementStack.clear();

	m_elementStack.reserve(eDefaultStackSize);

	if (m_accumulateText == true)
	{
		clear(m_textBuffer);

		reserve(m_textBuffer, eDefaultTextBufferSize);
	}
}



void
XalanSourceTreeContentHandler::startElement(
			const XMLCh* const	uri,
			const XMLCh* const	localname,
			const XMLCh* const	qname,
			const Attributes& 	attrs)
{
	processAccumulatedText();

	XalanSourceTreeElement* const	theNewElement =
		createElement(uri, localname, qname, attrs, m_currentElement);

	if (m_currentElement == 0)
	{
		if (m_document->getDocumentElement() != 0)
		{
			throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
		}
		else
		{
			m_document->setDocumentElement(theNewElement);
		}
	}
	else
	{
		m_currentElement->appendChildNode(theNewElement);
	}

	m_elementStack.push_back(theNewElement);

	m_currentElement = theNewElement;
}



void
XalanSourceTreeContentHandler::startPrefixMapping(
		const XMLCh* const	/* prefix */,
		const XMLCh* const	/* uri */)
{
}



void
XalanSourceTreeContentHandler::endPrefixMapping(const XMLCh* const	/* prefix */)
{
}



void
XalanSourceTreeContentHandler::skippedEntity(const XMLCh* const		/* name */)
{
}



void
XalanSourceTreeContentHandler::notationDecl(
			const XMLCh* const    name,
			const XMLCh* const    publicId,
			const XMLCh* const    systemId)
{
}



void
XalanSourceTreeContentHandler::unparsedEntityDecl(
			const XMLCh* const    name,
			const XMLCh* const    publicId,
			const XMLCh* const    systemId,
			const XMLCh* const    notationName)
{
	assert(m_document != 0);

	m_document->unparsedEntityDecl(name, publicId, systemId, notationName);
}



void
XalanSourceTreeContentHandler::resetDocType()
{
}



void
XalanSourceTreeContentHandler::setDocument(XalanSourceTreeDocument*	theDocument)
{
	if (m_ownsDocument == true)
	{
		delete m_document;
	}

	m_document = theDocument;
}



XalanSourceTreeDocument*
XalanSourceTreeContentHandler::detachDocument()
{
	XalanSourceTreeDocument* const	theDocument = m_document;

	m_document = 0;

	m_ownsDocument = false;

	return theDocument;
}



XalanSourceTreeElement*
XalanSourceTreeContentHandler::createElement(
			const XMLCh* const			uri,
			const XMLCh* const			localname,
			const XMLCh* const			qname,
			const Attributes& 			attrs,
			XalanSourceTreeElement*		theOwnerElement)
{
#if 1
	if (equals(uri, XALAN_STATIC_UCODE_STRING("EMPTY")) == false)
#else
	if (uri != 0 && length(uri) != 0)
#endif
	{
		return m_document->createElementNode(uri, localname, qname, attrs, theOwnerElement);
	}
	else
	{
		return m_document->createElementNode(qname, attrs, theOwnerElement);
	}
}



void
XalanSourceTreeContentHandler::processAccumulatedText()
{
	if (isEmpty(m_textBuffer) == false)
	{
		doCharacters(c_wstr(m_textBuffer), length(m_textBuffer));

		clear(m_textBuffer);
	}
}



void
XalanSourceTreeContentHandler::doCharacters(
			const XMLCh*	chars,
			unsigned int	length)
{
	m_currentElement->appendChildNode(m_document->createTextNode(chars, length, m_currentElement));
}
