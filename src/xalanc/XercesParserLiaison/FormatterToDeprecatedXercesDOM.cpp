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
#include "FormatterToDeprecatedXercesDOM.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_CDATASection.hpp>
#include <xercesc/dom/deprecated/DOM_Comment.hpp>
#include <xercesc/dom/deprecated/DOM_EntityReference.hpp>
#include <xercesc/dom/deprecated/DOM_ProcessingInstruction.hpp>
#include <xercesc/dom/deprecated/DOM_Text.hpp>
#else
#include <xercesc/dom/DOM_CDATASection.hpp>
#include <xercesc/dom/DOM_Comment.hpp>
#include <xercesc/dom/DOM_EntityReference.hpp>
#include <xercesc/dom/DOM_ProcessingInstruction.hpp>
#include <xercesc/dom/DOM_Text.hpp>
#endif


#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesDOMException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString	FormatterToDeprecatedXercesDOM::s_emptyString;



FormatterToDeprecatedXercesDOM::FormatterToDeprecatedXercesDOM(
			DOM_Document_Type&			doc,
			DOM_DocumentFragmentType&	docFrag,
			DOM_ElementType&			currentElement) :
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



FormatterToDeprecatedXercesDOM::FormatterToDeprecatedXercesDOM(
			DOM_Document_Type&	    doc,
			DOM_ElementType&		elem) :
	FormatterListener(OUTPUT_METHOD_DOM),
	m_doc(doc),
	m_docFrag(),
	m_currentElem(elem),
	m_elemStack(),
	m_buffer(),
	m_textBuffer()
{
	assert(m_doc != 0);
}

FormatterToDeprecatedXercesDOM::FormatterToDeprecatedXercesDOM(
			DOM_Document_Type&	    doc) :
	FormatterListener(OUTPUT_METHOD_DOM),
	m_doc(doc),
	m_docFrag(),
	m_currentElem(),
	m_elemStack(),
	m_buffer(),
	m_textBuffer()
{
	assert(m_doc != 0);
}



FormatterToDeprecatedXercesDOM::~FormatterToDeprecatedXercesDOM()
{
}



void
FormatterToDeprecatedXercesDOM::setDocumentLocator(const LocatorType* const	/* locator */)
{
	// No action for the moment.
}



void
FormatterToDeprecatedXercesDOM::startDocument()
{
	// No action for the moment.
}



void
FormatterToDeprecatedXercesDOM::endDocument()
{
	// No action for the moment.
}



void
FormatterToDeprecatedXercesDOM::startElement(
			const	XMLCh* const	name,
			AttributeListType&		attrs)
{
	try
	{
		processAccumulatedText();

		DOM_ElementType elem = createElement(name, attrs);

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
FormatterToDeprecatedXercesDOM::endElement(const	XMLCh* const	/* name */)
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
FormatterToDeprecatedXercesDOM::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	m_textBuffer.append(chars, length);
}



void
FormatterToDeprecatedXercesDOM::charactersRaw(
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
FormatterToDeprecatedXercesDOM::entityReference(const XMLCh* const	name)
{
	try
	{
		processAccumulatedText();

		DOM_EntityReferenceType  theXercesNode =
			m_doc.createEntityReference(name);

		assert(theXercesNode.isNull() == false);

		append(theXercesNode);
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToDeprecatedXercesDOM::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	try
	{
		processAccumulatedText();

		assign(m_buffer, chars, length);

		DOM_TextType theXercesNode = 
			m_doc.createTextNode(m_buffer.c_str());

		assert(theXercesNode.isNull() == false);

		append(theXercesNode);
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToDeprecatedXercesDOM::processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
{
	try
	{
		processAccumulatedText();

		DOM_ProcessingInstructionType theXercesNode = 
			m_doc.createProcessingInstruction(target, data);

		assert(theXercesNode.isNull() == false);

		append(theXercesNode);
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToDeprecatedXercesDOM::resetDocument()
{
}



void
FormatterToDeprecatedXercesDOM::comment(const XMLCh* const	data)
{
	try
	{
		processAccumulatedText();

		DOM_CommentType theXercesNode =
			m_doc.createComment(data);

		assert(theXercesNode.isNull() == false);

		append(theXercesNode);
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToDeprecatedXercesDOM::cdata(
			const XMLCh* const	ch,
			const unsigned int 	length)
{
	try
	{
		processAccumulatedText();

		assign(m_buffer, ch, length);

		DOM_CDATASectionType theXercesNode = 
			m_doc.createCDATASection(m_buffer.c_str());

		assert(theXercesNode.isNull() == false);

		append(theXercesNode);
	}
	catch(const XERCES_CPP_NAMESPACE_QUALIFIER DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
FormatterToDeprecatedXercesDOM::append(DOM_NodeType	&newNode)
{
	assert(newNode != 0);

	if(m_currentElem.isNull() == false)
	{
		m_currentElem.appendChild(newNode);
	}
	else if(m_docFrag.isNull() == false)
	{
		m_docFrag.appendChild(newNode);
	}
	else
	{
		m_doc.appendChild(newNode);
	}
}



DOM_ElementType
FormatterToDeprecatedXercesDOM::createElement(
			const XalanDOMChar*		theElementName,
			AttributeListType&		attrs)
{
	DOM_ElementType	theElement;

	if (m_prefixResolver == 0)
	{
		theElement = m_doc.createElement(theElementName);

		addAttributes(theElement, attrs);
	}
	else
	{
		// Check for the namespace...
		const XalanDOMString* const		theNamespace =
				DOMServices::getNamespaceForPrefix(theElementName, *m_prefixResolver, false , m_buffer);

		if (theNamespace == 0 || length(*theNamespace) == 0)
		{
			theElement = m_doc.createElement(theElementName);
		}
		else
		{
			theElement = m_doc.createElementNS(theNamespace->c_str(), theElementName);
		}

		addAttributes(theElement, attrs);
	}

	return theElement;
}



void
FormatterToDeprecatedXercesDOM::addAttributes(
			DOM_ElementType&	theElement,
			AttributeListType&	attrs)
{
	const unsigned int	nAtts = attrs.getLength();

	if (m_prefixResolver == 0)
	{
		for(unsigned int i = 0; i < nAtts; i++)
		{
			theElement.setAttribute(attrs.getName(i), attrs.getValue(i));
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
				theElement.setAttribute(theName, attrs.getValue(i));
			}
			else
			{
				theElement.setAttributeNS(theNamespace->c_str(), theName, attrs.getValue(i));
			}
		}
	}
}



void
FormatterToDeprecatedXercesDOM::processAccumulatedText()
{
	if (isEmpty(m_textBuffer) == false)
	{
		DOM_TextType theXercesNode =
			m_doc.createTextNode(m_textBuffer.c_str());
		
		assert(theXercesNode.isNull() == false);

		append(theXercesNode);

		clear(m_textBuffer);
	}
}



XALAN_CPP_NAMESPACE_END
