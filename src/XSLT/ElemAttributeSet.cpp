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
#include "ElemAttributeSet.hpp"



#include <sax/AttributeList.hpp>
#include <sax/SAXException.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemAttributeSet::ElemAttributeSet(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemUse(constructionContext,
			stylesheetTree,
			lineNumber,
			columnNumber,
			Constants::ELEMNAME_DEFINEATTRIBUTESET),
	m_QName()
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname,Constants::ATTRNAME_NAME))
		{
			m_QName = QNameByValue(atts.getValue(i), stylesheetTree.getNamespaces());

			stylesheetTree.addAttributeSet(m_QName, this);
		}
		else if(!(processUseAttributeSets(constructionContext, aname, atts, i) ||
					isAttrOK(aname, atts, i, constructionContext)))
		{
			constructionContext.error(Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING + " has an illegal attribute: " + aname);
		}
	}

	if(isEmpty(m_QName.getLocalPart()))
	{
		constructionContext.error(Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING + " must have a name attribute.");
	}
}



ElemAttributeSet::~ElemAttributeSet()
{
}



const XalanDOMString&
ElemAttributeSet::getElementName() const
{
	return Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING;
}



void
ElemAttributeSet::adopt(ElemAttributeSet&	theSource)
{
	ElemTemplateElement* node = theSource.getFirstChildElem();

	while(node != 0) 
	{
		assert(node->getXSLToken() == Constants::ELEMNAME_ATTRIBUTE);

		theSource.removeChild(node);

		XalanAutoPtr<ElemTemplateElement>	theGuard(node);

		appendChild(node);

		theGuard.release();

		node = theSource.getFirstChildElem();
	}

	copyQNames(theSource);
}



void
ElemAttributeSet::execute(StylesheetExecutionContext&		executionContext) const
{
	typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex		SetAndRestoreCurrentStackFrameIndex;
	typedef StylesheetExecutionContext::ElementRecursionStackPusher				ElementRecursionStackPusher;

	// This will push and pop the stack automatically...
	ElementRecursionStackPusher		thePusher(executionContext, this);

	// Make sure only global variables are visible during execution...
	SetAndRestoreCurrentStackFrameIndex		theSetAndRestore(
					executionContext, 
					executionContext.getGlobalStackFrameIndex());

	ElemUse::execute(executionContext);

	const ElemTemplateElement*	attr = getFirstChildElem();

	while(0 != attr)
	{
		attr->execute(executionContext);

		attr = attr->getNextSiblingElem();
	}
}



bool
ElemAttributeSet::childTypeAllowed(int	xslToken) const
{
	bool	fResult = false;

	switch(xslToken)
	{
	case Constants::ELEMNAME_ATTRIBUTE:
		fResult = true;
		break;
		
	default:
		break;
	}

	return fResult;
}
