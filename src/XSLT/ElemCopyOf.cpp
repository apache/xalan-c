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
#include "ElemCopyOf.hpp"



#include <sax/AttributeList.hpp>



#include <XPath/XPath.hpp>
#include <XPath/XObject.hpp>



#include "Constants.hpp"
#include "ElemWhen.hpp"
#include "SelectionEvent.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"



ElemCopyOf::ElemCopyOf(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMString&			name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						name,
						lineNumber,
						columnNumber,
						Constants::ELEMNAME_COPY_OF),
	m_pSelectPattern(0)
{
	const unsigned int	nAttrs = atts.getLength();
	
	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_SELECT))
		{
			m_pSelectPattern = constructionContext.createXPath(atts.getValue(i), *this);
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(name + " has an illegal attribute: " + aname);
		}
	}
}



void
ElemCopyOf::execute(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const
{
	ElemTemplateElement::execute(executionContext, sourceTree, sourceNode, mode);

	assert(m_pSelectPattern != 0);

	const XObject* const	pValue =
		m_pSelectPattern->execute(sourceNode, *this, executionContext.getXPathExecutionContext());
	assert(pValue != 0);

	if(0 != getStylesheet().getStylesheetRoot().getTraceListeners())
	{
		getStylesheet().getStylesheetRoot().fireSelectedEvent(
			SelectionEvent(executionContext, sourceNode,
				*this, XalanDOMString(XALAN_STATIC_UCODE_STRING("select")), *m_pSelectPattern, pValue));
	}

	const int	type = pValue->getType();

	XalanDOMString s;

	switch(type)
	{
	case XObject::eTypeBoolean:
	case XObject::eTypeNumber:
	case XObject::eTypeString:
		s = pValue->str();
		executionContext.characters(toCharArray(s), 0, length(s));
			break;

	case XObject::eTypeNodeSet:
	{
		const NodeRefListBase&	nl = pValue->nodeset();

		unsigned int			nChildren = nl.getLength();

		for(unsigned int i = 0; i < nChildren; i++)
		{
			XalanNode*			pos = nl.item(i);
			XalanNode* const	top = pos;

			while(pos != 0)
			{
				executionContext.flushPending();

				executionContext.cloneToResultTree(
						*pos,
						false,
						false,
						true);

				XalanNode*	nextNode = pos->getFirstChild();

				while(nextNode == 0)
				{
					if(XalanNode::ELEMENT_NODE == pos->getNodeType())
					{
						s = pos->getNodeName();

						executionContext.endElement(toCharArray(s));
					}

					if(top == pos)
						break;

					nextNode = pos->getNextSibling();

					if(nextNode == 0)
					{
						pos = pos->getParentNode();

						if(top == pos)
						{
							if(XalanNode::ELEMENT_NODE == pos->getNodeType())
							{
								s = pos->getNodeName();

								executionContext.endElement(toCharArray(s));
							}

							nextNode = 0;
							break;
						}
					}
				}

				pos = nextNode;
			}
		}
		break;
	}

	case XObject::eTypeResultTreeFrag:
		executionContext.outputResultTreeFragment(*pValue);
		break;

	default:
		s = pValue->str();

		if (!isEmpty(s))
		{
			executionContext.characters(toCharArray(s), 0, s.length());
		}

		break;
	}
}
