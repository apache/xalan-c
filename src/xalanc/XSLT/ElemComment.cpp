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
#include "ElemComment.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemComment::ElemComment(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_COMMENT)
{
	const unsigned int nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(isAttrOK(aname, atts, i, constructionContext) == false ||
		   processSpaceAttr(aname, atts, i, constructionContext))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_COMMENT_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}	
}



ElemComment::~ElemComment()
{
}



const XalanDOMString&
ElemComment::getElementName() const
{
	return Constants::ELEMNAME_COMMENT_WITH_PREFIX_STRING;
}



void
ElemComment::execute(StylesheetExecutionContext&	executionContext) const
{
	ElemTemplateElement::execute(executionContext);

	StylesheetExecutionContext::SetAndRestoreCopyTextNodesOnly	theSetAndRestore(executionContext, true);

    childrenToResultComment(executionContext);
}



bool
ElemComment::childTypeAllowed(int	xslToken) const
{
	bool	fResult = false;
	
	switch(xslToken)
	{
	// char-instructions 
	case StylesheetConstructionContext::ELEMNAME_TEXT_LITERAL_RESULT:
	case StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES:
	case StylesheetConstructionContext::ELEMNAME_APPLY_IMPORTS:
	case StylesheetConstructionContext::ELEMNAME_CALL_TEMPLATE:
	case StylesheetConstructionContext::ELEMNAME_FOR_EACH:
	case StylesheetConstructionContext::ELEMNAME_VALUE_OF:
	case StylesheetConstructionContext::ELEMNAME_COPY_OF:
	case StylesheetConstructionContext::ELEMNAME_NUMBER:
	case StylesheetConstructionContext::ELEMNAME_CHOOSE:
	case StylesheetConstructionContext::ELEMNAME_IF:
	case StylesheetConstructionContext::ELEMNAME_TEXT:
	case StylesheetConstructionContext::ELEMNAME_COPY:
	case StylesheetConstructionContext::ELEMNAME_VARIABLE:
	case StylesheetConstructionContext::ELEMNAME_MESSAGE:
		fResult = true;
		break;
		
	default:
		break;
	}
	
	return fResult;
}



XALAN_CPP_NAMESPACE_END
