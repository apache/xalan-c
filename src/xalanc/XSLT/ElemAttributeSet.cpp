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
#include "ElemAttributeSet.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetRoot.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemAttributeSet::ElemAttributeSet(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemUse(constructionContext,
			stylesheetTree,
			lineNumber,
			columnNumber,
			StylesheetConstructionContext::ELEMNAME_ATTRIBUTE_SET),
	m_qname(0)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_NAME))
		{
			m_qname = constructionContext.createXalanQName(
				atts.getValue(i),
				stylesheetTree.getNamespaces(),
				getLocator());
		}
		else if(!(processUseAttributeSets(constructionContext, aname, atts, i) ||
					isAttrOK(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}

	if(m_qname == 0)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(
				XalanMessages::TemplateMustHaveAttribute_2Param,
				Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING,
				Constants::ATTRNAME_NAME),
			0,
			this);
	}
	else if (m_qname->isValid() == false)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(XalanMessages::TemplateHasIllegalAttribute_2Param
				,Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING
				,Constants::ATTRNAME_NAME),
				0,
				this);
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
ElemAttributeSet::execute(StylesheetExecutionContext&	executionContext) const
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



void
ElemAttributeSet::addToStylesheet(
			StylesheetConstructionContext&	/* constructionContext */,
			Stylesheet&						theStylesheet)
{
	theStylesheet.getStylesheetRoot().addAttributeSet(*this);
}



bool
ElemAttributeSet::childTypeAllowed(int	xslToken) const
{
	bool	fResult = false;

	switch(xslToken)
	{
	case StylesheetConstructionContext::ELEMNAME_ATTRIBUTE:
		fResult = true;
		break;
		
	default:
		break;
	}

	return fResult;
}



XALAN_CPP_NAMESPACE_END
