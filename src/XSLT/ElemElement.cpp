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
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemElement::ElemElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMString&			name,
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
	m_namespaceAVT(0)	
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
			constructionContext.error(name + " has an illegal attribute: " + aname);
		}
	}

	if(0 == m_nameAVT)
	{
		constructionContext.error(name + " must have a name attribute.");
	}
	
}



ElemElement::~ElemElement()
{
	delete m_nameAVT;

	delete m_namespaceAVT;
}



void
ElemElement::execute(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const
{	
	XalanDOMString	elemName; 

	assert(m_nameAVT != 0);

	m_nameAVT->evaluate(elemName, sourceNode, *this, executionContext.getXPathExecutionContext());

	if(!isEmpty(elemName))
	{
		if(0 != m_namespaceAVT)
		{
			XalanDOMString	elemNameSpace;

			m_namespaceAVT->evaluate(elemNameSpace, sourceNode, 
				*this, executionContext.getXPathExecutionContext());

			if(!isEmpty(elemNameSpace))
			{
				XalanDOMString	prefix = executionContext.getResultPrefixForNamespace(elemNameSpace);

				if(isEmpty(prefix))
				{
					prefix = executionContext.getUniqueNameSpaceValue();

					const XalanDOMString	nsDecl =
						DOMServices::s_XMLNamespaceWithSeparator + prefix;

					executionContext.addResultAttribute(nsDecl, elemNameSpace);
				}

				elemName = prefix + XALAN_STATIC_UCODE_STRING(":") + elemName;
			}
		}

		executionContext.startElement(toCharArray(elemName));   

		ElemUse::execute(executionContext, sourceTree, sourceNode, mode);

		executeChildren(executionContext, sourceTree, sourceNode, mode);

		executionContext.endElement(toCharArray(elemName));
	}
}
