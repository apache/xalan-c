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
#include "ElemCallTemplate.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "ElemTemplate.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemCallTemplate::ElemCallTemplate(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_CALL_TEMPLATE),
	m_templateName(0),
	m_template(0)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_NAME))
		{
			m_templateName = constructionContext.createXalanQName(
					atts.getValue(i),
					getStylesheet().getNamespaces(),
					getLocator());

			if (m_templateName->isValid() == false)
			{
				constructionContext.error(
						XalanMessageLoader::getMessage(
							XalanMessages::AttributeValueNotValidQName_2Param,
							Constants::ATTRNAME_NAME.c_str(),
							atts.getValue(i)),
						0,
						this);
			}
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}

	if (m_templateName == 0)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(
				XalanMessages::TemplateMustHaveAttribute_2Param,
				Constants::ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING,
				Constants::ATTRNAME_NAME),
			0,
			this);
	}
}



ElemCallTemplate::~ElemCallTemplate()
{
}



const XalanDOMString&
ElemCallTemplate::getElementName() const
{
	return Constants::ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING;
}



void
ElemCallTemplate::execute(StylesheetExecutionContext&	executionContext) const
{
	ElemTemplateElement::execute(executionContext);

	assert(m_template != 0);

	const StylesheetExecutionContext::ParamsPushPop		thePushPop(
				executionContext,
				*this);

	m_template->executeAsNamed(executionContext);
}



void
ElemCallTemplate::postConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler)
{
	assert(m_templateName != 0);

	m_template = getStylesheet().getStylesheetRoot().findNamedTemplate(*m_templateName);

	if(m_template == 0)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(XalanMessages::CannotFindNamedTemplate),
			0,
			this);
	}

	// OK, now we can chain-up...
	ElemTemplateElement::postConstruction(constructionContext, theParentHandler);
}



bool
ElemCallTemplate::childTypeAllowed(int	xslToken) const
{
	if (xslToken == StylesheetConstructionContext::ELEMNAME_WITH_PARAM)
	{
		return true;
	}
	else
	{
		return false;
	}
}



XALAN_CPP_NAMESPACE_END
