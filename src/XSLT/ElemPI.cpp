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
#include "ElemPI.hpp"



#include <sax/AttributeList.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>


#include "AVT.hpp"
#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemPI::ElemPI(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						Constants::ELEMNAME_PI),
	m_nameAVT(0)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_NAME))
		{			
			m_nameAVT = new AVT(getLocator(), aname, atts.getType(i), atts.getValue(i),
				*this, constructionContext);
		}
		else if(isAttrOK(aname, atts, i, constructionContext) == false || processSpaceAttr(aname, atts, i))
		{
			constructionContext.error(
				"xsl:processing-instruction has an illegal attribute",
				0,
				this);
		}
	}

	if(0 == m_nameAVT)
	{
		constructionContext.error(
			"xsl:processing-instruction must have a 'name' attribute",
			0,
			this);
	}
}



ElemPI::~ElemPI()
{
#if defined(XALAN_CANNOT_DELETE_CONST)
	delete (AVT*)m_nameAVT;
#else
	delete m_nameAVT;
#endif
}



const XalanDOMString&
ElemPI::getElementName() const
{
	return Constants::ELEMNAME_PI_WITH_PREFIX_STRING;
}



void
ElemPI::execute(StylesheetExecutionContext&		executionContext) const
{
	ElemTemplateElement::execute(executionContext);
	
	XalanDOMString	piName;
	XalanNode* sourceNode = executionContext.getCurrentNode();
	
	m_nameAVT->evaluate(piName, sourceNode, *this, executionContext);

	if(equalsIgnoreCase(piName, Constants::ATTRVAL_OUTPUT_METHOD_XML))
	{
		executionContext.error("processing-instruction name can not be 'xml'", 0, this);
	}
	else if(!isValidNCName(piName))
	{
		executionContext.error("processing-instruction name must be a valid NCName", 0, this);
	}

	childrenToResultPI(
			executionContext,
			piName);
}



bool
ElemPI::childTypeAllowed(int	xslToken) const
{
	bool	fResult = false;
	
	switch(xslToken)
	{
	// char-instructions 
	case Constants::ELEMNAME_TEXTLITERALRESULT:
	case Constants::ELEMNAME_APPLY_TEMPLATES:
	case Constants::ELEMNAME_APPLY_IMPORTS:
	case Constants::ELEMNAME_CALLTEMPLATE:
	case Constants::ELEMNAME_FOREACH:
	case Constants::ELEMNAME_VALUEOF:
	case Constants::ELEMNAME_COPY_OF:
	case Constants::ELEMNAME_NUMBER:
	case Constants::ELEMNAME_CHOOSE:
	case Constants::ELEMNAME_IF:
	case Constants::ELEMNAME_TEXT:
	case Constants::ELEMNAME_COPY:
	case Constants::ELEMNAME_VARIABLE:
	case Constants::ELEMNAME_MESSAGE:
		fResult = true;
		break;
		
	default:
		break;
	}
	
	return fResult;
}
