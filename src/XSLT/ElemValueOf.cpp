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
#include "ElemValueOf.hpp"



#include <sax/AttributeList.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/XObjectFactory.hpp>
#include <XPath/XPath.hpp>



#include "Constants.hpp"
#include "SelectionEvent.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"



ElemValueOf::ElemValueOf(
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
						Constants::ELEMNAME_VALUEOF),
	m_selectPattern(0),
	m_disableOutputEscaping(false),
	m_isDot(false)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		const int					tok =
			constructionContext.getAttrTok(aname);

		switch(tok)
		{
		case Constants::TATTRNAME_SELECT:
			{
				const XalanDOMChar* const	avalue = atts.getValue(i);
				assert(avalue != 0);

				if (avalue[0] == '.' && avalue[1] == 0)
				{
					m_isDot = true;
				}

				m_selectPattern = constructionContext.createXPath(avalue, 
																  *this);
			}
			break;

		case Constants::TATTRNAME_DISABLE_OUTPUT_ESCAPING:
			m_disableOutputEscaping =
						getStylesheet().getYesOrNo(aname, atts.getValue(i), constructionContext);
			break;

		case Constants::TATTRNAME_XMLSPACE:
			processSpaceAttr(atts, i);
			break; 

		default:
			if(!isAttrOK(aname, atts, i, constructionContext))
			{
				constructionContext.error(name + " has an illegal attribute: " + aname);
			} 
		}
	}

	if(0 == m_selectPattern)
	{
		constructionContext.error(name + " requires a select attribute.");
	}
}



ElemValueOf::~ElemValueOf()
{
}



void
ElemValueOf::execute(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const
{
	ElemTemplateElement::execute(executionContext, sourceTree, sourceNode, mode);

	XalanDOMString	theValue;

	if (m_isDot == true)
	{
		const XalanNode::NodeType	type = sourceNode->getNodeType();

		if(type == XalanNode::COMMENT_NODE ||
           type == XalanNode::PROCESSING_INSTRUCTION_NODE)
		{
			theValue = sourceNode->getNodeValue();
		}
		else
		{
			theValue = executionContext.getNodeData(*sourceNode);
		}

		if(0 != executionContext.getTraceListeners())
		{
			executionContext.fireSelectEvent(
				SelectionEvent(executionContext,
							   sourceNode,
							   *this,
							   XalanDOMString(XALAN_STATIC_UCODE_STRING("select")),
							   *m_selectPattern,
							   executionContext.getXObjectFactory().createString(theValue)));       
		}
	}
	else
	{
		const XObject* const	value =
			m_selectPattern->execute(sourceNode,
									 *this,
									 executionContext);

		if(0 != executionContext.getTraceListeners())
		{
			executionContext.fireSelectEvent(
				SelectionEvent(executionContext,
							   sourceNode,
							   *this,
							   XalanDOMString(XALAN_STATIC_UCODE_STRING("select")),
							   *m_selectPattern,
							   value));       
		}

		if(0 != value)
		{
			const int	type = value->getType();

			if (XObject::eTypeNull != type)
			{
				theValue = value->str();
			}
		}
	}

	const unsigned int		len = length(theValue);

	if(len > 0)
	{
		if(m_disableOutputEscaping == false)
		{
			executionContext.characters(toCharArray(theValue), 0, len);
		}
		else
		{
			executionContext.charactersRaw(toCharArray(theValue), 0, len);
		}
	}
}
