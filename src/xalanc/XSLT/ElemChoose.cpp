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
#include "ElemChoose.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>



#include "Constants.hpp"
#include "ElemWhen.hpp"
#include "SelectionEvent.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemChoose::ElemChoose(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_CHOOSE)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(isAttrOK(aname, atts, i, constructionContext) == false ||
		   processSpaceAttr(aname, atts, i, constructionContext))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_CHOOSE_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}	
}



const XalanDOMString&
ElemChoose::getElementName() const
{
	return Constants::ELEMNAME_CHOOSE_WITH_PREFIX_STRING;
}



void
ElemChoose::execute(StylesheetExecutionContext&		executionContext) const
{
	ElemTemplateElement::execute(executionContext);

	XalanNode* sourceNode = executionContext.getCurrentNode();

    for (const ElemTemplateElement* node = getFirstChildElem();
			node != 0;
				node = node->getNextSiblingElem()) 
    {
		const int	type = node->getXSLToken();

		if(StylesheetConstructionContext::ELEMNAME_WHEN == type)
		{

			const XPath* const		theXPath = node->getXPath();
			assert(theXPath != 0);

			bool	test;

			theXPath->execute(*this, executionContext, test);

			if(0 != executionContext.getTraceListeners())
			{
				executionContext.fireSelectEvent(
					SelectionEvent(executionContext,
					sourceNode,
					*node,
					Constants::ATTRNAME_TEST,
					*theXPath,
					test));
			}

			if(test == true)
			{
				node->execute(executionContext);

				break;
			}
		}
		else
		{
			// xsl:otherwise
			node->execute(executionContext);
		}
    }
}



bool
ElemChoose::childTypeAllowed(int	xslToken) const
{
	bool	fResult = false;
	
	switch(xslToken)
	{
	// char-instructions 
    case StylesheetConstructionContext::ELEMNAME_WHEN:
    case StylesheetConstructionContext::ELEMNAME_OTHERWISE:
		fResult = true;
		break;
		
	default:
		break;
	}
	
	return fResult;
}



XALAN_CPP_NAMESPACE_END
