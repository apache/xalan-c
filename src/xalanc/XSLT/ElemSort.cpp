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
#include "ElemSort.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemSort::ElemSort(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_SORT),
	m_selectPattern(0),
	m_langAVT(0),
	m_dataTypeAVT(0),
	m_orderAVT(0),
	m_caseOrderAVT(0)
{
	const unsigned int nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_SELECT))
		{
			m_selectPattern 
				= constructionContext.createXPath(getLocator(), atts.getValue(i), *this);
		}
		else if(equals(aname, Constants::ATTRNAME_LANG))
		{			
			m_langAVT =
					constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
		}
		else if(equals(aname, Constants::ATTRNAME_DATATYPE))
		{
			m_dataTypeAVT =
					constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
		}
		else if(equals(aname, Constants::ATTRNAME_ORDER))
		{
			m_orderAVT =
					constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
		}
		else if(equals(aname, Constants::ATTRNAME_CASEORDER))
		{
			m_caseOrderAVT =
					constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_SORT_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}

	if(0 == m_dataTypeAVT)
	{
		m_dataTypeAVT =
			constructionContext.createAVT(
				getLocator(),
				c_wstr(Constants::ATTRNAME_DATATYPE),
				c_wstr(Constants::ATTRVAL_DATATYPE_TEXT),
				*this);
	}

	if(0 == m_orderAVT)
	{
		m_orderAVT =
			constructionContext.createAVT(
				getLocator(),
				c_wstr(Constants::ATTRNAME_ORDER),
				c_wstr(Constants::ATTRVAL_ORDER_ASCENDING),
				*this);
	}
}


ElemSort::~ElemSort()
{
}



const XalanDOMString&
ElemSort::getElementName() const
{
	return Constants::ELEMNAME_SORT_WITH_PREFIX_STRING;
}



const XPath*
ElemSort::getXPath(unsigned int		index) const
{
	return index == 0 ? m_selectPattern : 0;
}



XALAN_CPP_NAMESPACE_END
