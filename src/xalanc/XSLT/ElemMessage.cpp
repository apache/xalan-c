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
#include "ElemMessage.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemMessage::ElemMessage(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_MESSAGE),
	m_terminate(false)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if (equals(aname, Constants::ATTRNAME_TERMINATE) == true)
		{
			const XalanDOMChar*	const	avalue = atts.getValue(i);

			if (equals(avalue, Constants::ATTRVAL_YES) == true)
			{
				m_terminate = true;
			}
			else if (equals(avalue, Constants::ATTRVAL_NO) == false)
			{
				constructionContext.error(
					XalanMessageLoader::getMessage(XalanMessages::AttributeHasIllegalValue_1Param,Constants::ATTRNAME_TERMINATE),
					0,
					this);
			}
		}
		else if(isAttrOK(aname, atts, i, constructionContext) == false ||
				processSpaceAttr(aname, atts, i, constructionContext))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_MESSAGE_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}
}



const XalanDOMString&
ElemMessage::getElementName() const
{
	return Constants::ELEMNAME_MESSAGE_WITH_PREFIX_STRING;
}



void
ElemMessage::execute(StylesheetExecutionContext&	executionContext) const
{
	ElemTemplateElement::execute(executionContext);

	StylesheetExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

	const XalanDOMString&	theString =
		childrenToString(
			executionContext,
			theResult.get());

	const LocatorType* const	theLocator = getLocator();

	executionContext.message(
			theString,
			executionContext.getCurrentNode(),
			theLocator);

	if (m_terminate == true)
	{
		if (theLocator != 0)
		{
			throw ElemMessageTerminateException(*theLocator, theString);
		}
		else
		{
			throw ElemMessageTerminateException(theString);
		}
	}
}



ElemMessage::ElemMessageTerminateException::ElemMessageTerminateException(const XalanDOMString&		theMessage) :
	XSLTProcessorException(
			theMessage,
			TranscodeFromLocalCodePage("ElemMessageTerminateException"))
{
}



ElemMessage::ElemMessageTerminateException::ElemMessageTerminateException(
			const LocatorType&		theLocator,
			const XalanDOMString&	theMessage) :
	XSLTProcessorException(
			theLocator,
			theMessage,
			TranscodeFromLocalCodePage("ElemMessageTerminateException"))
{
}



ElemMessage::ElemMessageTerminateException::~ElemMessageTerminateException()
{
}



XALAN_CPP_NAMESPACE_END
