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
#include "FormatterToDOM.hpp"



#include <cassert>



#include <sax/AttributeList.hpp>



#include <XalanDOM/XalanCDATASection.hpp>
#include <XalanDOM/XalanComment.hpp>
#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanDocumentFragment.hpp>
#include <XalanDOM/XalanElement.hpp>
#include <XalanDOM/XalanEntityReference.hpp>
#include <XalanDOM/XalanProcessingInstruction.hpp>
#include <XalanDOM/XalanText.hpp>
#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/PrefixResolver.hpp>



const XalanDOMString	FormatterToDOM::s_emptyString;



FormatterToDOM::FormatterToDOM(
			XalanDocument*			doc,
			XalanDocumentFragment*	docFrag,
			XalanElement*			currentElement) :
	FormatterListener(OUTPUT_METHOD_DOM),
	m_doc(doc),
	m_docFrag(docFrag),
	m_currentElem(currentElement),
	m_elemStack(),
	m_buffer1(),
	m_buffer2()
{
	assert(m_doc != 0 && m_docFrag != 0);
}



FormatterToDOM::FormatterToDOM(
			XalanDocument*	doc,
			XalanElement*	elem) :
	FormatterListener(OUTPUT_METHOD_DOM),
	m_doc(doc),
	m_docFrag(0),
	m_currentElem(elem),
	m_elemStack(),
	m_buffer1(),
	m_buffer2()
{
	assert(m_doc != 0);
}



FormatterToDOM::~FormatterToDOM()
{
}



void
FormatterToDOM::setDocumentLocator(const Locator* const		/* locator */)
{
	// No action for the moment.
}



void
FormatterToDOM::startDocument()
{
	// No action for the moment.
}



void
FormatterToDOM::endDocument()
{
	// No action for the moment.
}



void
FormatterToDOM::startElement(
			const	XMLCh* const	name,
			AttributeList&			attrs)
{
	XalanElement* const		elem = createElement(name, attrs);
	assert(elem != 0);

	append(elem);

	m_elemStack.push_back(m_currentElem);

	m_currentElem = elem;
}



void
FormatterToDOM::endElement(
			const	XMLCh* const	/* name */)
{
	if(m_elemStack.empty() == false)
	{
		m_currentElem = m_elemStack.back();

		m_elemStack.pop_back();
	}
	else
	{
		m_currentElem = 0;
	}
}



void
FormatterToDOM::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	assign(m_buffer1, chars, length);

	append(m_doc->createTextNode(m_buffer1));
}



void
FormatterToDOM::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	append(m_doc->createProcessingInstruction(
				s_xsltNextIsRawString,
				s_formatterListenerString));

	characters(chars, length);
}		



void
FormatterToDOM::entityReference(const XMLCh* const	name)
{
	assign(m_buffer1, name);

	append(m_doc->createEntityReference(m_buffer1));
}



void
FormatterToDOM::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	assign(m_buffer1, chars, length);

	append(m_doc->createTextNode(m_buffer1));
}



void
FormatterToDOM::processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
{
	assign(m_buffer1, target);
	assign(m_buffer2, data);

	append(m_doc->createProcessingInstruction(m_buffer1, m_buffer2));
}



void
FormatterToDOM::resetDocument()
{
}



void
FormatterToDOM::comment(const XMLCh* const	data)
{
	assign(m_buffer1, data);

	append(m_doc->createComment(m_buffer1));
}



void
FormatterToDOM::cdata(
			const XMLCh* const	ch,
			const unsigned int 	length)
{
	assign(m_buffer1, ch, length);

	append(m_doc->createCDATASection(m_buffer1));
}



void
FormatterToDOM::append(XalanNode*	newNode)
{
	assert(newNode != 0);

	if(0 != m_currentElem)
	{
		m_currentElem->appendChild(newNode);
	}
	else if(0 != m_docFrag)
	{
		m_docFrag->appendChild(newNode);
	}
	else
	{
		m_doc->appendChild(newNode);
	}
}



XalanElement*
FormatterToDOM::createElement(
			const XalanDOMChar*		theElementName,
			AttributeList&			attrs)
{
	XalanElement*	theElement = 0;

	assign(m_buffer1, theElementName);

	if (m_prefixResolver == 0)
	{
		theElement = m_doc->createElement(m_buffer1);

		addAttributes(theElement, attrs);
	}
	else
	{
		// Check for the namespace...
		const XalanDOMString* const		theNamespace =
					getNamespaceForPrefix(theElementName, *m_prefixResolver, m_buffer2);

		if (theNamespace == 0 || length(*theNamespace) == 0)
		{
			theElement = m_doc->createElement(m_buffer1);
		}
		else
		{
			theElement = m_doc->createElementNS(*theNamespace, m_buffer1);
		}

		addAttributes(theElement, attrs);
	}

	return theElement;
}



void
FormatterToDOM::addAttributes(
			XalanElement*	theElement,
			AttributeList&	attrs)
{
	const unsigned int	nAtts = attrs.getLength();

	if (m_prefixResolver == 0)
	{
		for(unsigned int i = 0; i < nAtts; i++)
		{
			assign(m_buffer1, attrs.getName(i));
			assign(m_buffer2, attrs.getValue(i));

			theElement->setAttribute(m_buffer1, m_buffer2);
		}
	}
	else
	{
		for(unsigned int i = 0; i < nAtts; i++)
		{
			const XalanDOMChar* const	theName = attrs.getName(i);
			assert(theName != 0);

			// Check for the namespace...
			const XalanDOMString* const		theNamespace =
					getNamespaceForPrefix(theName, *m_prefixResolver, m_buffer2);

			assign(m_buffer1, theName);
			assign(m_buffer2, attrs.getValue(i));

			if (theNamespace == 0 || length(*theNamespace) == 0)
			{
				theElement->setAttribute(m_buffer1, m_buffer2);
			}
			else
			{
				theElement->setAttributeNS(*theNamespace, m_buffer1, m_buffer2);
			}
		}
	}
}



const XalanDOMString*
FormatterToDOM::getNamespaceForPrefix(
			const XalanDOMChar*		theName,
			const PrefixResolver&	thePrefixResolver,
			XalanDOMString&			thePrefix)
{
	const unsigned int	theLength = length(theName);
	const unsigned int	theColonIndex = indexOf(theName, XalanUnicode::charColon);

	if (theColonIndex == theLength)
	{
		clear(thePrefix);

		return thePrefixResolver.getNamespaceForPrefix(s_emptyString);
	}
	else
	{
		// Get the prefix from theName...
		assign(thePrefix, theName, theColonIndex);
		assert(length(thePrefix) != 0);

		return thePrefixResolver.getNamespaceForPrefix(thePrefix);
	}
}
