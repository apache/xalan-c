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
#include "FunctionLang.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionLang::FunctionLang() :
	m_attributeName(XALAN_STATIC_UCODE_STRING("lang"))
{
}



FunctionLang::~FunctionLang()
{
}



XObjectPtr
FunctionLang::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const LocatorType*		/* locator */) const
{
	assert(arg1.null() == false);	

	const XalanNode*		parent = context;

	bool					fMatch = false;

	const XalanDOMString&	lang = arg1->str();

	while(0 != parent)
	{
		if(XalanNode::ELEMENT_NODE == parent->getNodeType())
		{
			const XalanElement* const	theElementNode =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanElement*)parent;
#else
				static_cast<const XalanElement*>(parent);
#endif

			const XalanDOMString		langVal =
				theElementNode->getAttributeNS(DOMServices::s_XMLNamespaceURI, m_attributeName);

			if(0 != length(langVal))
			{
				if(startsWith(toLowerCaseASCII(langVal), toLowerCaseASCII(lang)))
				{
					const XalanDOMString::size_type		valLen = length(lang);

					if(length(langVal) == valLen ||
					   charAt(langVal, valLen) == XalanUnicode::charHyphenMinus)
					{
						fMatch = true;

						break;
					}
				}
			}
		}

		parent = DOMServices::getParentOfNode(*parent);
	}

	return executionContext.getXObjectFactory().createBoolean(fMatch);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionLang*
#endif
FunctionLang::clone() const
{
	return new FunctionLang(*this);
}



const XalanDOMString
FunctionLang::getError() const
{


	return XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param, "lang()");

}



XALAN_CPP_NAMESPACE_END
