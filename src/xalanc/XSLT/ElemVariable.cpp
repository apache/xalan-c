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
#include "ElemVariable.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>



#include "SelectionEvent.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemVariable::ElemVariable(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ParentType(
		constructionContext,
		stylesheetTree,
		lineNumber,
		columnNumber,
		StylesheetConstructionContext::ELEMNAME_VARIABLE),
	m_qname(0),
	m_selectPattern(0),
	m_isTopLevel(false),
	m_value(0),
	m_varContext(0)
{
	init(constructionContext, stylesheetTree, atts);
}



ElemVariable::ElemVariable(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber,
			int								xslToken) :
	ParentType(
		constructionContext,
		stylesheetTree,
		lineNumber,
		columnNumber,
		xslToken),
	m_qname(0),
	m_selectPattern(0),
	m_isTopLevel(false),
	m_value(0),
	m_varContext(0)
{
	init(constructionContext, stylesheetTree, atts);
}



ElemVariable::~ElemVariable()
{
}



void
ElemVariable::init(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts)
{
	const unsigned int	nAttrs = atts.getLength();
	
	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if (equals(aname, Constants::ATTRNAME_SELECT))
		{
			m_selectPattern = constructionContext.createXPath(getLocator(), atts.getValue(i), *this);
		}
		else if (equals(aname, Constants::ATTRNAME_NAME))
		{
			m_qname = constructionContext.createXalanQName(
						atts.getValue(i),
						stylesheetTree.getNamespaces(),
						getLocator());
		}
		else if(!(isAttrOK(aname, atts, i, constructionContext) || 
				 processSpaceAttr(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
					"xsl:variable has an illegal attribute",
					0,
					this);
		}
	}

	if(m_qname == 0)
	{
		constructionContext.error(
			"xsl:variable must have a 'name' attribute",
			0,
			this);
	}
	else if (m_qname->isValid() == false)
	{
		constructionContext.error(
			"xsl:variable has an invalid 'name' attribute",
			0,
			this);
	}
}



const XalanQName&
ElemVariable::getNameAttribute() const
{
	assert(m_qname != 0);

	return *m_qname;
}



void
ElemVariable::addToStylesheet(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						theStylesheet)
{
	// Processing a top-level element only...
	if (&theStylesheet != &getStylesheet())
	{
		constructionContext.error(
			"The ElemVariable instance was added to wrong stylesheet.",
			0,
			this);
	}
	else if (getParentNode() != 0)
	{
		constructionContext.error(
			"The ElemVariable instance is already parented and cannot be a top-level element.",
			0,
			this);
	}
	else
	{
		theStylesheet.setTopLevelVariable(this);

		m_isTopLevel = true;
	}
}



const XalanDOMString&
ElemVariable::getElementName() const
{
	return Constants::ELEMNAME_VARIABLE_WITH_PREFIX_STRING;
}



void
ElemVariable::setParentNodeElem(ElemTemplateElement*	theParent)
{
	if (m_isTopLevel == true)
	{
		throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
	}
	else
	{
		ParentType::setParentNodeElem(theParent);
	}
}



void
ElemVariable::execute(StylesheetExecutionContext&	executionContext) const
{
	assert(m_qname != 0);

	ParentType::execute(executionContext);

	const XObjectPtr	theValue(getValue(executionContext, executionContext.getCurrentNode()));

	if (theValue.null() == false)
	{
		executionContext.pushVariable(
				*m_qname,
				theValue,
				getParentNodeElem());
	}
	else
	{
		executionContext.pushVariable(
				*m_qname,
				this,
				getParentNodeElem());
	}
}



const XObjectPtr
ElemVariable::getValue(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceNode) const
{
	if(m_selectPattern == 0)
	{
		if (getFirstChild() == 0)
		{
			return executionContext.getXObjectFactory().createStringReference(s_emptyString);
		}
		else
		{
			return executionContext.createXResultTreeFrag(*this, sourceNode);
		}
	}
	else
	{
		XObjectPtr	theValue;

		XalanNode* const	theCurrentNode = executionContext.getCurrentNode();
		
		if (theCurrentNode == sourceNode)
		{
			theValue = m_selectPattern->execute(*this, executionContext);
		}
		else
		{
			const XPathExecutionContext::CurrentNodeSetAndRestore	theCurrentNodeSetAndRestore(executionContext, theCurrentNode, sourceNode);

			theValue = m_selectPattern->execute(*this, executionContext);
		}

		if(0 != executionContext.getTraceListeners())
		{
			executionContext.fireSelectEvent(
				SelectionEvent(
					executionContext,
					sourceNode,
					*this,
					StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("select")),
					*m_selectPattern,
					theValue));
		}

		return theValue;
	}
}



XALAN_CPP_NAMESPACE_END
