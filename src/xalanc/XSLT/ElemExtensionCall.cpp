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
#include "ElemExtensionCall.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemExtensionCall::ElemExtensionCall(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMChar*				name,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber,
			ExtensionNSHandler&				ns) :
	ElemLiteralResult(constructionContext,
					  stylesheetTree,
					  name,
					  atts,
					  lineNumber,
					  columnNumber,
					  StylesheetConstructionContext::ELEMNAME_EXTENSION_CALL),
	m_qname(constructionContext.createXalanQName(name, getStylesheet().getNamespaces(), getLocator())),
	m_nsh(ns)
{
	assert(m_qname != 0);
}



void
ElemExtensionCall::execute(StylesheetExecutionContext&	executionContext) const
{
	ElemTemplateElement::execute(executionContext);

	executionContext.warn( 
		XalanMessageLoader::getMessage(XalanMessages::XalanHandleExtensions),
		0,
		getLocator());

	for (const ElemTemplateElement*	child = getFirstChildElem(); child != 0; child = child->getNextSiblingElem())
	{
		if(child->getXSLToken() == StylesheetConstructionContext::ELEMNAME_FALLBACK)
		{
			child->execute(executionContext);
		}
	}
}



bool
ElemExtensionCall::elementAvailable(StylesheetExecutionContext&		executionContext) const
{
	return executionContext.elementAvailable(*m_qname);
}



XALAN_CPP_NAMESPACE_END
