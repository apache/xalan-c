/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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



#include <xercesc/sax/AttributeList.hpp>



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



XALAN_CPP_NAMESPACE_BEGIN



ElemCopyOf::ElemCopyOf(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_COPY_OF),
	m_selectPattern(0)
{
	bool	isSelectCurrentNode = false;

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
				isSelectCurrentNode = true;
			}
			else
			{
				m_selectPattern = constructionContext.createXPath(getLocator(), avalue, *this);
			}
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(
				"xsl:copy-of has an illegal attribute",
				0,
				this);
		}
	}

	if (isSelectCurrentNode == false && m_selectPattern == 0)
	{
		constructionContext.error(
			"xsl:copy-of must have a 'select' attribute",
			0,
			this);
	}
}



const XalanDOMString&
ElemCopyOf::getElementName() const
{
	return Constants::ELEMNAME_COPY_OF_WITH_PREFIX_STRING;
}



void
ElemCopyOf::execute(StylesheetExecutionContext&		executionContext) const
{
	ElemTemplateElement::execute(executionContext);

	XalanNode* const	sourceNode = executionContext.getCurrentNode();
	assert(sourceNode != 0);

	if (m_selectPattern == 0)
	{
		if(0 != executionContext.getTraceListeners())
		{
			StylesheetExecutionContext::BorrowReturnMutableNodeRefList	theNodeList(executionContext);

			theNodeList->addNode(sourceNode);

			executionContext.fireSelectEvent(
				SelectionEvent(
					executionContext,
					sourceNode,
					*this,
					StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("select")),
					StaticStringToDOMString(XALAN_STATIC_UCODE_STRING(".")),
					executionContext.getXObjectFactory().createNodeSet(theNodeList)));
		}

		executionContext.cloneToResultTree(*sourceNode, this);
	}
	else
	{
		const XObjectPtr	value(m_selectPattern->execute(*this, executionContext));
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
			{
				const NodeRefListBase&				theNodeList = value->nodeset();
				const NodeRefListBase::size_type	nChildren = theNodeList.getLength();

				for(NodeRefListBase::size_type i = 0; i < nChildren; i++)
				{
					assert(theNodeList.item(i) != 0);

					executionContext.cloneToResultTree(*theNodeList.item(i), this);
				}
			}
			break;

		case XObject::eTypeResultTreeFrag:
			executionContext.outputResultTreeFragment(*value.get(), this);
			break;

		default:
			executionContext.characters(value);
			break;
		}
	}
}



XALAN_CPP_NAMESPACE_END
