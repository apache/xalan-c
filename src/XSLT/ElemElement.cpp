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
#include "ElemElement.hpp"



#include <sax/AttributeList.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemElement::ElemElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMChar*				name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemUse(constructionContext,
			stylesheetTree,
			name,
			lineNumber,
			columnNumber,
			Constants::ELEMNAME_ELEMENT),
	m_nameAVT(0),
	m_namespaceAVT(0),
	m_namespacesHandler(stylesheetTree.getNamespacesHandler(),
						stylesheetTree.getNamespaces(),
						stylesheetTree.getXSLTNamespaceURI())	
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_NAME))
		{
			m_nameAVT = new AVT(aname,	atts.getType(i), atts.getValue(i),
				*this, constructionContext);
		}
		else if(equals(aname, Constants::ATTRNAME_NAMESPACE))
		{
			m_namespaceAVT = new AVT(aname, atts.getType(i), atts.getValue(i),
				*this, constructionContext); 
		}
		else if(!(processUseAttributeSets(constructionContext, aname, atts, i) || processSpaceAttr(aname, atts, i) ||
			isAttrOK(aname, atts, i, constructionContext)))
		{
			constructionContext.error(XalanDOMString(name) + " has an illegal attribute: " + aname);
		}
	}

	if(0 == m_nameAVT)
	{
		constructionContext.error(XalanDOMString(name) + " must have a name attribute.");
	}
	
}



ElemElement::~ElemElement()
{
#if defined(XALAN_CANNOT_DELETE_CONST)
	delete (AVT*)m_nameAVT;

	delete (AVT*)m_namespaceAVT;
#else
	delete m_nameAVT;

	delete m_namespaceAVT;
#endif
}



void
ElemElement::postConstruction(const NamespacesHandler&	theParentHandler)
{
	m_namespacesHandler.postConstruction(getElementName(), &theParentHandler);

	ElemUse::postConstruction(m_namespacesHandler);
}



const NamespacesHandler&
ElemElement::getNamespacesHandler() const
{
	return m_namespacesHandler;
}



void
ElemElement::execute(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const
{
	StylesheetExecutionContext::GetAndReleaseCachedString	elemNameGuard(executionContext);

	XalanDOMString&		elemName = elemNameGuard.get();

	m_nameAVT->evaluate(elemName, sourceNode, *this, executionContext);

	bool					isIllegalElement = false;

	unsigned int			len = length(elemName);

	const unsigned int		indexOfNSSep = indexOf(elemName, XalanUnicode::charColon);

	const bool				haveNamespace = indexOfNSSep == len ? false : true;

	const XalanDOMString*	ns = 0;

	if(haveNamespace == true)
	{
		if (indexOfNSSep + 1 == len ||
			isValidNCName(substring(elemName, indexOfNSSep + 1)) == false)
		{
			isIllegalElement = true;
		}
	}
	else if(len == 0 || isValidNCName(elemName) == false)
	{
		isIllegalElement = true;
	}

	if (isIllegalElement == true)
	{
		executionContext.warn("Illegal element name!", sourceNode, this);

		clear(elemName);

		len = 0;
	}
	else if (haveNamespace == true)
	{
		ns = &getNamespaceForPrefixInternal(substring(elemName, 0, indexOfNSSep), false);
	}

	const unsigned int	nsLength = ns == 0 ? 0 : length(*ns);

	if (len != 0 && (haveNamespace == false || nsLength != 0))
	{
		if(0 != m_namespaceAVT)
		{
			StylesheetExecutionContext::GetAndReleaseCachedString	elemNameSpaceGuard(executionContext);

			XalanDOMString&		elemNameSpace = elemNameSpaceGuard.get();

			m_namespaceAVT->evaluate(elemNameSpace, sourceNode, *this, executionContext);

			if(!isEmpty(elemNameSpace))
			{
				if(indexOfNSSep < len)
				{
					assign(elemName, substring(elemName, indexOfNSSep + 1));
				}

				const XalanDOMString&	prefix = executionContext.getResultPrefixForNamespace(elemNameSpace);

				if(isEmpty(prefix) == false)
				{
					reserve(
						elemName,
						length(elemName) + DOMServices::s_XMLNamespaceSeparatorStringLength + length(prefix) + 1);

					insert(elemName, 0, DOMServices::s_XMLNamespaceSeparatorString);
					insert(elemName, 0, prefix);
				}
				else
				{
					StylesheetExecutionContext::GetAndReleaseCachedString	newPrefixGuard(executionContext);

					XalanDOMString&		newPrefix = newPrefixGuard.get();

					executionContext.getUniqueNamespaceValue(newPrefix);

					StylesheetExecutionContext::GetAndReleaseCachedString	nsDeclGuard(executionContext);

					XalanDOMString&		nsDecl = nsDeclGuard.get();

					reserve(nsDecl, DOMServices::s_XMLNamespaceWithSeparatorLength + length(newPrefix) + 1);

					assign(nsDecl, DOMServices::s_XMLNamespaceWithSeparator);

					append(nsDecl, newPrefix);

					executionContext.addResultAttribute(nsDecl, elemNameSpace);

					reserve(
						elemName,
						length(elemName) + DOMServices::s_XMLNamespaceSeparatorStringLength + length(newPrefix) + 1);

					insert(elemName, 0, DOMServices::s_XMLNamespaceSeparatorString);
					insert(elemName, 0, newPrefix);
				}
			}
		}

		executionContext.startElement(toCharArray(elemName));   
	}

	ElemUse::execute(executionContext, sourceTree, sourceNode, mode);

	m_namespacesHandler.outputResultNamespaces(executionContext);

	executeChildren(executionContext, sourceTree, sourceNode, mode);

	if (len != 0 && (haveNamespace == false || nsLength != 0))
	{
		executionContext.endElement(toCharArray(elemName));
	}
}
