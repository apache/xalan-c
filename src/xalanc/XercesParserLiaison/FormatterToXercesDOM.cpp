/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#include "FormatterToXercesDOM.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <xercesc/dom/DOMCDATASection.hpp>
#include <xercesc/dom/DOMComment.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentFragment.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMEntityReference.hpp>
#include <xercesc/dom/DOMProcessingInstruction.hpp>
#include <xercesc/dom/DOMText.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>



#include "XercesDOMException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString	FormatterToXercesDOM::s_emptyString;



FormatterToXercesDOM::FormatterToXercesDOM(
			DOMDocument_Type*			doc,
			DOMDocumentFragmentType*	docFrag,
			DOMElementType*				currentElement) :
	FormatterListener(OUTPUT_METHOD_DOM),
	m_doc(doc),
	m_docFrag(docFrag),
	m_currentElem(currentElement),
	m_elemStack(),
	m_buffer(),
	m_textBuffer()
{
	assert(m_doc != 0 && m_docFrag != 0);
}



FormatterToXercesDOM::FormatterToXercesDOM(
			DOMDocument_Type*	doc,
			DOMElementType*		elem) :
	FormatterListener(OUTPUT_METHOD_DOM),
	m_doc(doc),
	m_docFrag(0),
	m_currentElem(elem),
	m_elemStack(),
	m_buffer(),
	m_textBuffer()
{
	assert(m_doc != 0);
}



FormatterToXercesDOM::~FormatterToXercesDOM()
{
}



void
FormatterToXercesDOM::setDocumentLocator(const LocatorType* const	/* locator */)
{
	// No action for the moment.
}



void
FormatterToXercesDOM::startDocument()
{
	// No action for the moment.
}



void
FormatterToXercesDOM::endDocument()
{
	// No action for the moment.
}



void
FormatterToXercesDOM::startElement(
			const	XMLCh* const	name,
			AttributeListType&		attrs)
{
	try
	{
		processAccumulatedText();

		DOMElementType* const	elem = createElement(name, attrs);
		assert(elem != 0);

		append(elem);

		m_elemStack.push_back(m_currentElem);

		m_currentElem = elem;
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToXercesDOM::endElement(const	XMLCh* const	/* name */)
{
	try
	{
		processAccumulatedText();

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
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToXercesDOM::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	m_textBuffer.append(chars, length);
}



void
FormatterToXercesDOM::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	try
	{
		processAccumulatedText();

		cdata(chars, length);
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}		



void
FormatterToXercesDOM::entityReference(const XMLCh* const	name)
{
	try
	{
		processAccumulatedText();

		append(m_doc->createEntityReference(name));
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToXercesDOM::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	try
	{
		processAccumulatedText();

		assign(m_buffer, chars, length);

		append(m_doc->createTextNode(m_buffer.c_str()));
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToXercesDOM::processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
{
	try
	{
		processAccumulatedText();

		append(m_doc->createProcessingInstruction(target, data));
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToXercesDOM::resetDocument()
{
}



void
FormatterToXercesDOM::comment(const XMLCh* const	data)
{
	try
	{
		processAccumulatedText();

		append(m_doc->createComment(data));
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToXercesDOM::cdata(
			const XMLCh* const	ch,
			const unsigned int 	length)
{
	try
	{
		processAccumulatedText();

		assign(m_buffer, ch, length);

		append(m_doc->createCDATASection(m_buffer.c_str()));
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToXercesDOM::append(DOMNodeType*	newNode)
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



DOMElementType*
FormatterToXercesDOM::createElement(
			const XalanDOMChar*		theElementName,
			AttributeListType&		attrs)
{
	DOMElementType*	theElement = 0;

	if (m_prefixResolver == 0)
	{
		theElement = m_doc->createElement(theElementName);

		addAttributes(theElement, attrs);
	}
	else
	{
		// Check for the namespace...
		const XalanDOMString* const		theNamespace =
				DOMServices::getNamespaceForPrefix(theElementName, *m_prefixResolver, false, m_buffer);

		if (theNamespace == 0 || length(*theNamespace) == 0)
		{
			theElement = m_doc->createElement(theElementName);
		}
		else
		{
			theElement = m_doc->createElementNS(theNamespace->c_str(), theElementName);
		}

		addAttributes(theElement, attrs);
	}

	return theElement;
}



void
FormatterToXercesDOM::addAttributes(
			DOMElementType*		theElement,
			AttributeListType&	attrs)
{
	const unsigned int	nAtts = attrs.getLength();

	if (m_prefixResolver == 0)
	{
		for(unsigned int i = 0; i < nAtts; i++)
		{
			theElement->setAttribute(attrs.getName(i), attrs.getValue(i));
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
					DOMServices::getNamespaceForPrefix(theName, *m_prefixResolver, true, m_buffer);

			if (theNamespace == 0 || length(*theNamespace) == 0)
			{
				theElement->setAttribute(theName, attrs.getValue(i));
			}
			else
			{
				theElement->setAttributeNS(theNamespace->c_str(), theName, attrs.getValue(i));
			}
		}
	}
}



void
FormatterToXercesDOM::processAccumulatedText()
{
	if (isEmpty(m_textBuffer) == false)
	{
		append(m_doc->createTextNode(m_textBuffer.c_str()));

		clear(m_textBuffer);
	}
}



XALAN_CPP_NAMESPACE_END
