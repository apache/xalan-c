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
#include "ElemEmpty.hpp"



#include <cassert>


#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemEmpty::ElemEmpty(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
			const XalanDOMString*			elementName) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						StylesheetConstructionContext::ELEMNAME_UNDEFINED,
						XalanDOMString(),
						lineNumber,
						columnNumber),
	m_elementName(elementName)
{
}



ElemEmpty::ElemEmpty(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMString*			elementName) :
	ElemTemplateElement(
			constructionContext,
			stylesheetTree,
			StylesheetConstructionContext::ELEMNAME_UNDEFINED),
	m_elementName(elementName)
{
}



ElemEmpty::~ElemEmpty()
{
	assert(getNextSiblingElem() == 0 && getFirstChildElem() == 0);
}



const XalanDOMString&
ElemEmpty::getElementName() const
{
	return m_elementName != 0 ? *m_elementName : s_emptyString;
}



void
ElemEmpty::execute(StylesheetExecutionContext&		executionContext) const
{
	assert(false);	// really shouldn't be executing empty nodes

	ElemTemplateElement::execute(executionContext);
}



bool
ElemEmpty::childTypeAllowed(int		/* xslToken */) const
{
	return false;
}



XALAN_CPP_NAMESPACE_END
