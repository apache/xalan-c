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
#include "ElemUse.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/StringTokenizer.hpp>



#include "Constants.hpp"
#include "ElemAttributeSet.hpp"
#include "StylesheetRoot.hpp"
#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



/**
 * This acts as a superclass for ElemCopy, ElemAttributeSet,
 * ElemElement, and ElemLiteralResult, on order to implement 
 * shared behavior the use-attribute-sets attribute.
 */
ElemUse::ElemUse(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
			int								xslToken) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						xslToken),
	m_attributeSetsNames(0),
	m_attributeSetsNamesCount(0)
{
}



ElemUse::~ElemUse()
{
}



const XalanDOMString&
ElemUse::getElementName() const
{
	return s_emptyString;
}



void
ElemUse::postConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler)
{
	if (m_attributeSetsNamesCount > 0)
	{
		canGenerateAttributes(true);
	}

	// OK, now we can chain-up...
	ElemTemplateElement::postConstruction(constructionContext, theParentHandler);
}



void
ElemUse::execute(StylesheetExecutionContext&	executionContext) const
{
	doExecute(executionContext, true);
}



void
ElemUse::doExecute(
			StylesheetExecutionContext&		executionContext,
			bool							applyAttributeSets) const
{
	assert(m_attributeSetsNamesCount == 0 || m_attributeSetsNames != 0);

	ElemTemplateElement::execute(executionContext);

	if(applyAttributeSets == true && m_attributeSetsNamesCount > 0)
	{
		assert(canGenerateAttributes() == true);

		const StylesheetRoot&		theStylesheetRoot = getStylesheet().getStylesheetRoot();
		const LocatorType* const	theLocator = getLocator();

		for(size_type i = 0; i < m_attributeSetsNamesCount; ++i)
		{
			theStylesheetRoot.executeAttributeSet(executionContext, *m_attributeSetsNames[i], theLocator);
		}
	}
}



bool
ElemUse::processUseAttributeSets(
			StylesheetConstructionContext&	constructionContext,
			const XalanDOMChar*				attrName,
			const AttributeListType&		atts,
			int								which)
{
	bool isUAS = false;

	if(StylesheetConstructionContext::ELEMNAME_LITERAL_RESULT == getXSLToken())
	{
		isUAS = constructionContext.isXSLUseAttributeSetsAttribute(
			attrName,
			getStylesheet(),
			getLocator());
	}
	else
	{
		isUAS = equals(attrName, Constants::ATTRNAME_USEATTRIBUTESETS);
	}

	if(isUAS == true)
	{
		m_attributeSetsNames =
			constructionContext.tokenizeQNames(
				m_attributeSetsNamesCount,
				atts.getValue(which),
				getStylesheet().getNamespaces(),
				getLocator());
		assert(m_attributeSetsNamesCount == 0 || m_attributeSetsNames != 0);
	}

	return isUAS;
}



XALAN_CPP_NAMESPACE_END
