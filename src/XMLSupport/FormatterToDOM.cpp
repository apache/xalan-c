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



#include <dom/DOMString.hpp>
#include <sax/AttributeList.hpp>



#include <Include/DOMHelper.hpp>



FormatterToDOM::FormatterToDOM(
			const DOM_Document&				doc,
			const DOM_DocumentFragment&		docFrag,
			const DOM_Element&				currentElement) :
	m_docFrag(docFrag),
	m_doc(doc),
	m_currentElem(currentElement),
	m_elemStack()
{
}

FormatterToDOM::FormatterToDOM(
			const DOM_Document&			doc,
			const DOM_Element&			elem) :
	m_docFrag(),
	m_doc(doc),
	m_currentElem(elem),
	m_elemStack()
{
}



FormatterToDOM::~FormatterToDOM()
{
}



void
FormatterToDOM::setDocumentLocator(Locator* const		/* locator */)
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
	DOM_Element		elem = m_doc.createElement(name);

	const int		nAtts = attrs.getLength();

	for(int i = 0; i < nAtts; i++)
	{
		elem.setAttribute(attrs.getName(i), attrs.getValue(i));
	}

	append(elem);

	m_elemStack.push(elem);

	m_currentElem = elem;
}



void
FormatterToDOM::endElement(
			const	XMLCh* const	/* name */)
{
	m_elemStack.pop();

	if(m_elemStack.empty() == false)
	{
		m_currentElem = m_elemStack.top();
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
	append(m_doc.createTextNode(DOMString(chars, length)));
}

void
FormatterToDOM::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	append(m_doc.createProcessingInstruction("xslt-next-is-raw",
				"formatter-to-dom"));
	append(m_doc.createTextNode(DOMString(chars, length)));
}		



void
FormatterToDOM::entityReference(const XMLCh* const	name)
{
	append(m_doc.createEntityReference(name));
}



void
FormatterToDOM::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	append(m_doc.createTextNode(DOMString(chars, length)));
}



void
FormatterToDOM::processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
{
	append(m_doc.createProcessingInstruction(DOMString(target), DOMString(data)));
}



void
FormatterToDOM::resetDocument()
{
}



void
FormatterToDOM::comment(const XMLCh* const	data)
{
	append(m_doc.createComment(DOMString(data)));
}



void
FormatterToDOM::cdata(
			const XMLCh* const	ch,
			const unsigned int 	length)
{
	append(m_doc.createCDATASection(DOMString(ch, length)));
}



void
FormatterToDOM::append(const DOM_Node&	newNode)
{
	if(0 != m_currentElem)
	{
		m_currentElem.appendChild(newNode);
	}
	else if(0 != m_docFrag)
	{
		m_docFrag.appendChild(newNode);
	}
	else
	{
		m_doc.appendChild(newNode);
	}
}
