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
#include "FormatterToDOM.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/XalanDOM/XalanCDATASection.hpp>
#include <xalanc/XalanDOM/XalanComment.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanEntityReference.hpp>
#include <xalanc/XalanDOM/XalanProcessingInstruction.hpp>
#include <xalanc/XalanDOM/XalanText.hpp>
#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



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
FormatterToDOM::setDocumentLocator(const LocatorType* const		/* locator */)
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
			AttributeListType&		attrs)
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
	assign(m_buffer1, s_piTarget, s_piTargetLength);
	assign(m_buffer2, s_piData, s_piDataLength);

	append(m_doc->createProcessingInstruction(
				m_buffer1,
				m_buffer2));

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
			AttributeListType&		attrs)
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
					DOMServices::getNamespaceForPrefix(theElementName, *m_prefixResolver, false, m_buffer2);

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
			XalanElement*		theElement,
			AttributeListType&	attrs)
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
				DOMServices::getNamespaceForPrefix(theName, *m_prefixResolver, true, m_buffer2);

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



XALAN_CPP_NAMESPACE_END
