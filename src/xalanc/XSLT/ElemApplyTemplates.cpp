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
#include "ElemApplyTemplates.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XalanQNameByValue.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetRoot.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "TracerEvent.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanQNameByValue	s_defaultMode;



ElemApplyTemplates::ElemApplyTemplates(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ParentType(
		constructionContext,
		stylesheetTree,
		lineNumber,
		columnNumber,
		StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES),
	m_mode(0)
{
	const unsigned	int nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if (equals(aname, Constants::ATTRNAME_SELECT))
		{
			m_selectPattern = constructionContext.createXPath(getLocator(), atts.getValue(i), *this);
		}
		else if (equals(aname, Constants::ATTRNAME_MODE))
		{
			m_mode = constructionContext.createXalanQName(atts.getValue(i), getStylesheet().getNamespaces(), getLocator());

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
		else if (!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_APPLY_TEMPLATES_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}

	if(0 == m_selectPattern)
	{
		m_selectPattern = constructionContext.createXPath(
			getLocator(),
			Constants::PSEUDONAME_NODE,
			*this);
	}

	if (m_mode == 0)
	{
		m_mode = &s_defaultMode;
	}

	assert(m_selectPattern != 0 && m_mode != 0);
}



ElemApplyTemplates::~ElemApplyTemplates()
{
}



const XalanDOMString&
ElemApplyTemplates::getElementName() const
{
	return Constants::ELEMNAME_APPLY_TEMPLATES_WITH_PREFIX_STRING;
}



void
ElemApplyTemplates::execute(StylesheetExecutionContext&		executionContext) const
{
	assert(m_selectPattern != 0 && m_mode != 0);

	if(0 != executionContext.getTraceListeners())
	{
		executionContext.fireTraceEvent(TracerEvent(
		  executionContext, *this));
	}

	ParentType::transformSelectedChildren(
			executionContext,
			0);
}



void
ElemApplyTemplates::transformChild(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						child) const
{
	assert(child != 0);

	const XalanNode::NodeType	nodeType = child->getNodeType();

	// Filter out any attributes nodes that are namespace declarations
	if (nodeType != XalanNode::ATTRIBUTE_NODE ||
#if defined(XALAN_OLD_STYLE_CASTS)
		DOMServices::isNamespaceDeclaration((const XalanAttr&)*child) == false)
#else
		DOMServices::isNamespaceDeclaration(static_cast<const XalanAttr&>(*child)) == false)
#endif
	{
		ParentType::transformChild(
			executionContext,
			xslInstruction,
			theTemplate,
			child,
			nodeType);
	}
}



void
ElemApplyTemplates::selectAndSortChildren(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement*		theTemplate,
			NodeSorter*						sorter,
			int								/* selectStackFrameIndex */) const
{
	int		savedStackFrameIndex;

	// Push the params & stack frame, but then execute the select
	// expression inside transformSelectedChildren, which must be
	// executed in the stack frame before the new stack frame.
	const StylesheetExecutionContext::ParamsPushPop		thePushPop(
			executionContext,
			*this,
			savedStackFrameIndex);

	const XalanQName* const		currentMode = executionContext.getCurrentMode();
	assert(currentMode != 0);

	if (isDefaultTemplate() == false &&
		!m_mode->equals(*currentMode))
	{
		executionContext.setCurrentMode(m_mode);

		ParentType::selectAndSortChildren(
				executionContext,
				theTemplate,
				sorter,
				savedStackFrameIndex);

		executionContext.setCurrentMode(currentMode);
	}
	else
	{
		ParentType::selectAndSortChildren(
				executionContext,
				theTemplate,
				sorter,
				savedStackFrameIndex);
	}
}



bool
ElemApplyTemplates::childTypeAllowed(int	xslToken) const
{
	bool	fResult = false;

	switch(xslToken)
	{
	case StylesheetConstructionContext::ELEMNAME_SORT:
	case StylesheetConstructionContext::ELEMNAME_WITH_PARAM:
		fResult = true;
		break;

	default:
		break;
	}

	return fResult;
}



XALAN_CPP_NAMESPACE_END
