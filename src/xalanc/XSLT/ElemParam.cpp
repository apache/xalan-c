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
#include "ElemParam.hpp"



#include <xalanc/XPath/XObjectFactory.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"
#include "TracerEvent.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemParam::ElemParam(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemVariable(constructionContext,
				 stylesheetTree,
				 atts,
				 lineNumber,
				 columnNumber,
				 StylesheetConstructionContext::ELEMNAME_PARAM)
{
}



const XalanDOMString&
ElemParam::getElementName() const
{
	return Constants::ELEMNAME_PARAM_WITH_PREFIX_STRING;
}



void
ElemParam::execute(StylesheetExecutionContext&		executionContext) const
{
	assert(m_qname != 0);

	const XObjectPtr	obj = executionContext.getParamVariable(*m_qname);

	if (obj.null() == true)
	{
		ElemVariable::execute(executionContext);
	}
	else
	{
		if(0 != executionContext.getTraceListeners())
		{
			executionContext.fireTraceEvent(
				TracerEvent(
					executionContext,					
					*this));
		}
	}
}



XALAN_CPP_NAMESPACE_END
