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
#include "ElemTemplate.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XPath.hpp>



#include <xalanc/XPath/XalanQNameByValue.hpp>



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

			if (m_name->isValid() == false)
			{
				constructionContext.error(
						XalanMessageLoader::getMessage(
							XalanMessages::AttributeValueNotValidQName_2Param,
							aname,
							atts.getValue(i)),
						0,
						this);
			}
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

			if (m_mode->isValid() == false)
			{
				constructionContext.error(
						XalanMessageLoader::getMessage(
							XalanMessages::AttributeValueNotValidQName_2Param,
							aname,
							atts.getValue(i)),
						0,
						this);
			}
		}
		else if(!(isAttrOK(aname, atts, i, constructionContext) || 
				 processSpaceAttr(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_TEMPLATE_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}

	if(0 == m_matchPattern && m_name->isEmpty() == true)
	{
		constructionContext.error(
				XalanMessageLoader::getMessage(
					XalanMessages::RequiresEitherNameOrMatchAttribute_1Param,
					Constants::ELEMNAME_TEMPLATE_WITH_PREFIX_STRING),
				0,
				this);
	}

	assert(m_name->isEmpty() == true || m_name->isValid() == true);
	assert(m_mode->isEmpty() == true || m_mode->isValid() == true);
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
	ParentType::execute(executionContext);

	executeChildren(executionContext);
}



const XPath*
ElemTemplate::getXPath(unsigned int	index) const
{
	return index == 0 ? m_matchPattern : 0;
}



void
ElemTemplate::executeChildren(StylesheetExecutionContext&	executionContext) const
{
	StylesheetExecutionContext::PushAndPopCurrentTemplate	thePushAndPop(executionContext, this);

	ParentType::executeChildren(executionContext);
}



void
ElemTemplate::executeChildren(
		StylesheetExecutionContext&		executionContext,
		XalanNode*						sourceNode) const
{
	ParentType::executeChildren(executionContext, sourceNode);
}



XALAN_CPP_NAMESPACE_END
