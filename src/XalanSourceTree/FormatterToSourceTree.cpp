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
// Class header file.
#include "FormatterToSourceTree.hpp"



#include <cassert>



#include <sax/AttributeList.hpp>



#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeProcessingInstruction.hpp"
#include "XalanSourceTreeText.hpp"



FormatterToSourceTree::FormatterToSourceTree(XalanSourceTreeDocument*	theDocument) :
	FormatterListener(OUTPUT_METHOD_DOM),
	m_document(theDocument),
	m_currentElement(0),
	m_elementStack(),
	m_textBuffer()
{
	assert(m_document != 0);
}



FormatterToSourceTree::~FormatterToSourceTree()
{
}



void
FormatterToSourceTree::setDocumentLocator(const Locator* const		/* locator */)
{
}



void
FormatterToSourceTree::startDocument()
{
}



void
FormatterToSourceTree::endDocument()
{
	assert(m_elementStack.empty() == true);

	assert(isEmpty(m_textBuffer) == true);
}



void
FormatterToSourceTree::startElement(
			const	XMLCh* const	name,
			AttributeList&			attrs)
{
	XalanSourceTreeElement* const	theNewElement =
		m_document->createElementNode(name, attrs, m_currentElement);

	if (m_currentElement == 0)
	{
		m_document->appendChildNode(theNewElement);
	}
	else
	{
		m_currentElement->appendChildNode(theNewElement);
	}

	m_elementStack.push_back(theNewElement);

	m_currentElement = theNewElement;
}



void
FormatterToSourceTree::endElement(const	XMLCh* const	/* name */)
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
FormatterToSourceTree::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if (m_currentElement == 0)
	{
		if (isXMLWhitespace(chars) == false)
		{
			throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
		}
	}
	else
	{
		append(m_textBuffer, chars, length);
		m_currentElement->appendChildNode(m_document->createTextNode(chars, length, m_currentElement));
	}
}



void
FormatterToSourceTree::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	assert(m_document != 0);

	processAccumulatedText();

	characters(chars, length);
}



void
FormatterToSourceTree::entityReference(const XMLCh* const	/* name */)
{
}



void
FormatterToSourceTree::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	// Ignore any whitespace reported before the document element has been parsed.
	if (m_elementStack.empty() == false)
	{
		processAccumulatedText();

		m_currentElement->appendChildNode(m_document->createTextIWSNode(chars, length, m_currentElement));
	}
}



void
FormatterToSourceTree::processingInstruction(
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
FormatterToSourceTree::resetDocument()
{
}



void
FormatterToSourceTree::comment(const XMLCh* const	data)
{
	assert(m_document != 0);

	processAccumulatedText();

	XalanSourceTreeComment* const	theNewComment =
		m_document->createCommentNode(data, length(data), m_currentElement);

	if (m_currentElement != 0)
	{
		m_currentElement->appendChildNode(theNewComment);
	}
	else
	{
		m_document->appendChildNode(theNewComment);
	}
}



void
FormatterToSourceTree::cdata(
			const XMLCh* const	/* ch */,
			const unsigned int 	/* length */)
{
}



void
FormatterToSourceTree::processAccumulatedText()
{
	if (isEmpty(m_textBuffer) == false)
	{
		doCharacters(c_wstr(m_textBuffer), length(m_textBuffer));

		clear(m_textBuffer);
	}
}



void
FormatterToSourceTree::doCharacters(
			const XMLCh*	chars,
			unsigned int	length)
{
	m_currentElement->appendChildNode(m_document->createTextNode(chars, length, m_currentElement));
}
