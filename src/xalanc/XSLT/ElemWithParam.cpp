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
#include "ElemWithParam.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemWithParam::ElemWithParam(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_WITH_PARAM),
	m_selectPattern(0),
	m_qname(0)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_SELECT))
		{
			m_selectPattern = constructionContext.createXPath(getLocator(), atts.getValue(i), *this);
		}
		else if(equals(aname, Constants::ATTRNAME_NAME))
		{
			m_qname = constructionContext.createXalanQName(
						atts.getValue(i),
						stylesheetTree.getNamespaces(),
						getLocator());

			if (m_qname->isValid() == false)
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
						Constants::ELEMNAME_WITHPARAM_WITH_PREFIX_STRING.c_str(),
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
				Constants::ELEMNAME_WITHPARAM_WITH_PREFIX_STRING,
				Constants::ATTRNAME_NAME),
			0,
			this);
	}
}



ElemWithParam::~ElemWithParam()
{
}



const XalanDOMString&
ElemWithParam::getElementName() const
{
	return Constants::ELEMNAME_WITHPARAM_WITH_PREFIX_STRING;
}



const XPath*
ElemWithParam::getXPath(unsigned int	index) const
{
	return index == 0 ? m_selectPattern : 0;
}



XALAN_CPP_NAMESPACE_END
