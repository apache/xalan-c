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
#include "ElemTemplate.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DoubleSupport.hpp>



#include <XPath/XPath.hpp>



#include <XPath/XalanQNameByValue.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanQNameByValue	s_empty;



ElemTemplate::ElemTemplate(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_TEMPLATE),
	m_matchPattern(0),
	m_name(&s_empty),
	m_mode(&s_empty),
	m_priority(XPath::getMatchScoreValue(XPath::eMatchScoreNone))
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if (equals(aname, Constants::ATTRNAME_MATCH))
		{
			m_matchPattern = constructionContext.createMatchPattern(getLocator(), atts.getValue(i), *this);
		}
		else if (equals(aname, Constants::ATTRNAME_NAME))
		{
			m_name = constructionContext.createXalanQName(
						atts.getValue(i),
						getStylesheet().getNamespaces(),
						getLocator());
		}
		else if (equals(aname, Constants::ATTRNAME_PRIORITY))
		{
			assert(atts.getValue(i) != 0);

			m_priority = DoubleSupport::toDouble(atts.getValue(i));
		}
		else if (equals(aname, Constants::ATTRNAME_MODE))
		{
			m_mode = constructionContext.createXalanQName(
						atts.getValue(i),
						getStylesheet().getNamespaces(),
						getLocator());
		}
		else if(!(isAttrOK(aname, atts, i, constructionContext) || 
				 processSpaceAttr(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
					"xsl:template has an illegal attribute",
					0,
					this);
		}
	}

	const bool	isEmptyName = m_name->isEmpty();

	if(0 == m_matchPattern && isEmptyName == true)
	{
		constructionContext.error(
				"xsl:template requires either a name or a match attribute",
				0,
				this);
	}
	else if (isEmptyName == false && m_name->isValid() == false)
	{
		constructionContext.error(
			"xsl:template has an invalid 'name' attribute",
			0,
			this);
	}
	else if (m_mode->isEmpty() == false && m_mode->isValid() == false)
	{
		constructionContext.error(
			"xsl:template has an invalid 'mode' attribute",
			0,
			this);
	}
}



ElemTemplate::~ElemTemplate()
{
}



void
ElemTemplate::addToStylesheet(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						theStylesheet)
{
	theStylesheet.addTemplate(this, constructionContext); 
}



const XalanQName&
ElemTemplate::getNameAttribute() const
{ 
	return *m_name;
}



const XalanDOMString&
ElemTemplate::getElementName() const
{
	return Constants::ELEMNAME_TEMPLATE_WITH_PREFIX_STRING;
}



void
ElemTemplate::execute(StylesheetExecutionContext&	executionContext) const
{
	ElemTemplateElement::execute(executionContext);

	executeChildren(executionContext);
}



void
ElemTemplate::executeChildren(StylesheetExecutionContext&	executionContext) const
{
	StylesheetExecutionContext::SetAndRestoreCurrentTemplate	theSetAndRestore(executionContext, this);

	ElemTemplateElement::executeChildren(executionContext);
}



void
ElemTemplate::executeChildren(
		StylesheetExecutionContext&		executionContext,
		XalanNode*						sourceNode) const
{
	ElemTemplateElement::executeChildren(executionContext, sourceNode);
}



XALAN_CPP_NAMESPACE_END
