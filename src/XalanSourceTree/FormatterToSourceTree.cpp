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



#include <xercesc/sax/AttributeList.hpp>



#include <XalanDOM/XalanDOMException.hpp>
#include <XalanDOM/XalanDocumentFragment.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/PrefixResolver.hpp>



#include <XMLSupport/FormatterToDOM.hpp>



#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeDocumentFragment.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeHelper.hpp"
#include "XalanSourceTreeProcessingInstruction.hpp"
#include "XalanSourceTreeText.hpp"



FormatterToSourceTree::FormatterToSourceTree(XalanSourceTreeDocument*	theDocument) :
	FormatterListener(OUTPUT_METHOD_DOM),
	m_document(theDocument),
	m_documentFragment(0),
	m_currentElement(0),
	m_elementStack(),
	m_lastChild(0),
	m_lastChildStack(),
	m_textBuffer()
{
}



FormatterToSourceTree::FormatterToSourceTree(
			XalanSourceTreeDocument*			theDocument,
			XalanSourceTreeDocumentFragment*	theDocumentFragment) :
	FormatterListener(OUTPUT_METHOD_DOM),
	m_document(theDocument),
	m_documentFragment(theDocumentFragment),
	m_currentElement(0),
	m_elementStack(),
	m_lastChild(0),
	m_lastChildStack(),
	m_textBuffer()
{
	assert(m_document != 0);
	assert(m_documentFragment != 0);
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
	m_currentElement = 0;

	m_elementStack.clear();

	m_lastChild = 0;

	m_lastChildStack.clear();

	m_lastChildStack.reserve(eDefaultStackSize);

	clear(m_textBuffer);

	reserve(m_textBuffer, eDefaultTextBufferSize);

	// Push a dummy value for the current element, so we
	// don't have to check for an empty stack in endElement().
	m_elementStack.push_back(ElementStackType::value_type(0));
}



void
FormatterToSourceTree::endDocument()
{
	// Pop off the dummy value that we pushed in 
	// startDocument()...
	m_elementStack.pop_back();

	assert(m_elementStack.empty() == true);
	assert(m_lastChildStack.empty() == true);
	assert(isEmpty(m_textBuffer) == true);
}



// A helper function to manage appending the new child.
template <class ParentNodeType, class ChildNodeType>
inline void
doAppendChildNode(
			ParentNodeType*		theParent,
			XalanNode*&			theLastChild,
			ChildNodeType		theNewChild)
{
	assert(theParent != 0);
	assert(theNewChild != 0);

	if (theLastChild == 0)
	{
		theParent->appendChildNode(theNewChild);
	}
	else
	{
		XalanSourceTreeHelper::appendSibling(theLastChild, theNewChild);
	}

	theLastChild = theNewChild;
}



// A helper function to manage appending the new child.
template <class ChildNodeType>
inline void
doAppendChildNode(
			XalanSourceTreeDocument*			theDocument,
			XalanSourceTreeDocumentFragment*	theDocumentFragment,
			XalanSourceTreeElement*				theCurrentElement,
			XalanNode*&							theLastChild,
			ChildNodeType						theNewChild)
{
	assert(theDocument != 0);
	assert(theNewChild != 0);

	if (theCurrentElement == 0)
	{
		if (theDocumentFragment != 0)
		{
			doAppendChildNode(theDocumentFragment, theLastChild, theNewChild);
		}
		else
		{
			// If there is no current element. it means we haven't
			// created the document element yet, so always append
			// to the document, rather than the last child.
			theDocument->appendChildNode(theNewChild);
		}
	}
	else
	{
		doAppendChildNode(theCurrentElement, theLastChild, theNewChild);
	}
}



void
FormatterToSourceTree::startElement(
			const	XMLCh* const	name,
			AttributeList&			attrs)
{
	processAccumulatedText();

	XalanSourceTreeElement* const	theNewElement =
		createElementNode(name, attrs, m_currentElement);

	doAppendChildNode(
			m_document,
			m_documentFragment,
			m_currentElement,
			m_lastChild,
			theNewElement);

	m_elementStack.push_back(theNewElement);

	m_lastChildStack.push_back(m_lastChild);

	m_currentElement = theNewElement;

	m_lastChild = 0;
}



void
FormatterToSourceTree::endElement(const	XMLCh* const	/* name */)
{
	processAccumulatedText();

	assert(m_elementStack.empty() == false);

	// Pop the element of the stack...
	m_elementStack.pop_back();

	assert(m_elementStack.empty() == false);

	// Get the element from the back of the
	// stack.
	m_currentElement = m_elementStack.back();

	assert(m_lastChildStack.empty() == false);

	m_lastChild = m_lastChildStack.back();

	// Pop the last child stack
	m_lastChildStack.pop_back();
}



void
FormatterToSourceTree::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if (m_documentFragment != 0)
	{
		doCharacters(chars, length);
	}
	else if (m_currentElement == 0)
	{
		if (isXMLWhitespace(chars) == false)
		{
			throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
		}
	}
	else
	{
		append(m_textBuffer, chars, length);
	}
}



void
FormatterToSourceTree::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	assert(m_document != 0);

	processAccumulatedText();

	doProcessingInstruction(c_wstr(s_xsltNextIsRawString), c_wstr(s_formatterListenerString));

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
	assert(m_document != 0);

	// Ignore any whitespace reported before the document element has been parsed.
	if (m_elementStack.size() > 1)
	{
		assert(m_documentFragment != 0 || m_document->getDocumentElement() != 0);

		processAccumulatedText();

		assert(m_currentElement != 0);

		doAppendChildNode(
			m_currentElement,
			m_lastChild,
			m_document->createTextIWSNode(chars, length, m_currentElement));
	}
	else if(m_documentFragment != 0)
	{
		processAccumulatedText();

		doAppendChildNode(
			m_documentFragment,
			m_lastChild,
			m_document->createTextIWSNode(chars, length, m_currentElement));
	}
}



void
FormatterToSourceTree::processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
{
	assert(m_document != 0);

	processAccumulatedText();

	doProcessingInstruction(target, data);
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

	doAppendChildNode(
			m_document,
			m_documentFragment,
			m_currentElement,
			m_lastChild,
			theNewComment);
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
			const XMLCh*				chars,
			XalanDOMString::size_type	length)
{
	if (m_currentElement != 0)
	{
		doAppendChildNode(
			m_currentElement,
			m_lastChild,
			m_document->createTextNode(chars, length, m_currentElement));
	}
	else if (m_documentFragment != 0)
	{
		doAppendChildNode(
			m_documentFragment,
			m_lastChild,
			m_document->createTextNode(chars, length, m_currentElement));
	}
	else
	{
		throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
	}
}



XalanSourceTreeElement*
FormatterToSourceTree::createElementNode(
			const XalanDOMChar*			name,
			AttributeList&				attrs,
			XalanSourceTreeElement*		theParentElement)
{
	if (m_prefixResolver != 0)
	{
		return m_document->createElementNode(name, attrs, *m_prefixResolver, theParentElement);
	}
	else
	{
		return m_document->createElementNode(name, attrs, theParentElement);
	}
}



void
FormatterToSourceTree::doProcessingInstruction(
			const XMLCh*	target,
			const XMLCh*	data)
{
	doAppendChildNode(
			m_document,
			m_documentFragment,
			m_currentElement,
			m_lastChild,
			m_document->createProcessingInstructionNode(target, data));
}
