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



#include <XPath/XObject.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPath.hpp>



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
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						Constants::ELEMNAME_COPY_OF),
	m_selectPattern(0),
	m_isDot(false)
{
	const unsigned int	nAttrs = atts.getLength();
	
	for(unsigned int i = 0; i < nAttrs; ++i)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_SELECT))
		{
			const XalanDOMChar* const	avalue = atts.getValue(i);
			assert(avalue != 0);

			if (avalue[0] == XalanUnicode::charFullStop && avalue[1] == 0)
			{
				m_isDot = true;
			}

			m_selectPattern = constructionContext.createXPath(avalue, *this);
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(XalanDOMString("xsl:copy-of has an illegal attribute: ") + aname);
		}
	}

	if (m_selectPattern == 0)
	{
		constructionContext.error("xsl:copy-of must have a \"select\" attribute.");
	}
}



const XalanDOMString&
ElemCopyOf::getElementName() const
{
	return Constants::ELEMNAME_COPY_OF_WITH_PREFIX_STRING;
}



inline void
ElemCopyOf::cloneNodeSet(
			StylesheetExecutionContext&		executionContext,
			const NodeRefListBase&			theNodeList) const
{
	unsigned int	nChildren = theNodeList.getLength();

	for(unsigned int i = 0; i < nChildren; i++)
	{
		assert(theNodeList.item(i) != 0);
		cloneNode(executionContext, *theNodeList.item(i));
	}
}



inline void
ElemCopyOf::cloneNode(
			StylesheetExecutionContext&		executionContext,
			XalanNode&						theNode) const
{
	XalanNode*			pos = &theNode;

	while(pos != 0)
	{
		if(pos->getNodeType() != XalanNode::ATTRIBUTE_NODE)
		{
			executionContext.flushPending();
		}

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
				executionContext.endElement(c_wstr(pos->getNodeName()));
			}

			if(&theNode == pos)
				break;

			nextNode = pos->getNextSibling();

			if(nextNode == 0)
			{
				pos = pos->getParentNode();

				if(&theNode == pos)
				{
					if(XalanNode::ELEMENT_NODE == pos->getNodeType())
					{
						executionContext.endElement(c_wstr(pos->getNodeName()));
					}

					nextNode = 0;
					break;
				}
			}
		}

		pos = nextNode;
	}
}



void
ElemCopyOf::execute(StylesheetExecutionContext&		executionContext) const
{
	ElemTemplateElement::execute(executionContext);

	assert(m_selectPattern != 0);

	XalanNode* const	sourceNode = executionContext.getCurrentNode();
	assert(sourceNode != 0);

	if (m_isDot == true)
	{
		if(0 != executionContext.getTraceListeners())
		{
			executionContext.fireSelectEvent(
				SelectionEvent(
					executionContext,
					sourceNode,
					*this,
					StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("select")),
					*m_selectPattern,
					XObjectPtr()));
		}

		cloneNode(executionContext, *sourceNode);
	}
	else
	{
		const XObjectPtr	value(m_selectPattern->execute(sourceNode, *this, executionContext));
		assert(value.null() == false);

		if(0 != executionContext.getTraceListeners())
		{
			executionContext.fireSelectEvent(
				SelectionEvent(
					executionContext,
					sourceNode,
					*this,
					StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("select")),
					*m_selectPattern,
					value));
		}

		const XObject::eObjectType	type = value->getType();

		switch(type)
		{
		case XObject::eTypeBoolean:
		case XObject::eTypeNumber:
		case XObject::eTypeString:
			executionContext.characters(value);
			break;

		case XObject::eTypeNodeSet:
			cloneNodeSet(executionContext, value->nodeset());
			break;

		case XObject::eTypeResultTreeFrag:
			executionContext.outputResultTreeFragment(*value.get());
			break;

		default:
			executionContext.characters(value);
			break;
		}
	}
}
