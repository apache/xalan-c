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
#include "ElemForwardCompatible.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemForwardCompatible::ElemForwardCompatible(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet& 					stylesheetTree,
			const XalanDOMChar*				name,
			const AttributeListType&		atts,
			int 							lineNumber,
			int 							columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_FORWARD_COMPATIBLE),
	m_elementName(constructionContext.getPooledString(name))
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if (!(isAttrOK(aname, atts, i, constructionContext) ||
			processSpaceAttr(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							m_elementName.c_str(),
							aname),
					0,
					this);
		}
	}
}



const XalanDOMString&
ElemForwardCompatible::getElementName() const
{
	return m_elementName;
}



ElemForwardCompatible::~ElemForwardCompatible()
{
}



void
ElemForwardCompatible::execute(StylesheetExecutionContext&		executionContext) const
{
	ElemTemplateElement::execute(executionContext);

	for (const ElemTemplateElement*	child = getFirstChildElem(); child != 0; child = child->getNextSiblingElem())
	{
		if(child->getXSLToken() == StylesheetConstructionContext::ELEMNAME_FALLBACK)
		{
			child->execute(executionContext);
		}
	}
}



XALAN_CPP_NAMESPACE_END
