/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ElemIf.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>



#include "Constants.hpp"
#include "SelectionEvent.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemIf::ElemIf(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_IF),
	m_test(0)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if (equals(aname, Constants::ATTRNAME_TEST))
		{
			m_test = constructionContext.createXPath(getLocator(), atts.getValue(i), *this);
		}
		else if(!(isAttrOK(aname, atts, i, constructionContext) || 
				 processSpaceAttr(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_IF_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}

	if(0 == m_test)
	{
		constructionContext.error(
				XalanMessageLoader::getMessage(
					XalanMessages::TemplateMustHaveAttribute_2Param,
					Constants::ELEMNAME_IF_WITH_PREFIX_STRING,
					Constants::ATTRNAME_TEST),
				0,
				this);
	}
}



const XalanDOMString&
ElemIf::getElementName() const
{
	return Constants::ELEMNAME_IF_WITH_PREFIX_STRING;
}



void
ElemIf::execute(StylesheetExecutionContext&		executionContext) const
{
	assert(m_test != 0);

	ElemTemplateElement::execute(executionContext);

	bool	fResult;

	m_test->execute(*this, executionContext, fResult);

	if(0 != executionContext.getTraceListeners())
	{
		executionContext.fireSelectEvent(
			SelectionEvent(executionContext,
			executionContext.getCurrentNode(),
			*this,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("test")),
			*m_test,
			fResult));
	}

	if(fResult == true)
	{
		executeChildren(executionContext);
	}
}



const XPath*
ElemIf::getXPath(unsigned int	index) const
{
	return index == 0 ? m_test : 0;
}



XALAN_CPP_NAMESPACE_END
