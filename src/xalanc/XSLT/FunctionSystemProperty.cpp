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
#include "FunctionSystemProperty.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/MutableNodeRefList.hpp>
#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XalanQName.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;



FunctionSystemProperty::FunctionSystemProperty() :
	Function(),
	m_xsltNamespaceURI(XALAN_STATIC_UCODE_STRING("http://www.w3.org/1999/XSL/Transform")),
	m_versionPropertyString(XALAN_STATIC_UCODE_STRING("version")),
	m_vendorPropertyString(XALAN_STATIC_UCODE_STRING("vendor")),
	m_vendorURLPropertyString(XALAN_STATIC_UCODE_STRING("vendor-url")),
	m_vendorString(XALAN_STATIC_UCODE_STRING("Apache Software Foundation")),
	m_vendorURLString(XALAN_STATIC_UCODE_STRING("http://xml.apache.org/xalan-c"))
{
}



FunctionSystemProperty::~FunctionSystemProperty()
{
}



inline void
validateNCName(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const LocatorType*		locator,
			const XalanDOMString&	ncname)
{
	if (XalanQName::isValidNCName(ncname) == false)
	{
		executionContext.error(
			"system-property(): The property is not a valid QName",
			context,
			locator);
	}
}



XObjectPtr
FunctionSystemProperty::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const LocatorType*		locator) const
{
	assert(arg1.null() == false);

	const XalanDOMString&				fullName = arg1->str();
	const XalanDOMString::size_type		fullNameLength = length(fullName);
	const XalanDOMString::size_type		indexOfNSSep = indexOf(fullName, XalanUnicode::charColon);

	if(indexOfNSSep < fullNameLength)
	{
		XPathExecutionContext::GetAndReleaseCachedString	guard(executionContext);

		XalanDOMString&		theBuffer = guard.get();

		substring(fullName, theBuffer, 0, indexOfNSSep);

		validateNCName(executionContext, context, locator, theBuffer);

		const XalanDOMString* const		nspace = executionContext.getNamespaceForPrefix(theBuffer);

		if (nspace == 0)
		{
			executionContext.error(
					XalanMessageLoader::getMessage(XalanMessages::UndeclaredNamespacePrefix_1Param, theBuffer),
					context,
					locator);
		}
		else
		{
			if(*nspace == m_xsltNamespaceURI)
			{
				substring(fullName, theBuffer, indexOfNSSep + 1);

				validateNCName(executionContext, context, locator, theBuffer);

				if(equals(theBuffer, m_versionPropertyString))
				{
					return executionContext.getXObjectFactory().createNumber(1.0);
				}
				else if(equals(theBuffer, m_vendorPropertyString))
				{
					return executionContext.getXObjectFactory().createStringReference(m_vendorString);
				}
				else if(equals(theBuffer, m_vendorURLPropertyString))
				{
					return executionContext.getXObjectFactory().createStringReference(m_vendorURLString);
				}
				else
				{
					return executionContext.getXObjectFactory().createStringReference(s_emptyString);
				}
			}
		}
	}
	else
	{
		validateNCName(executionContext, context, locator, fullName);

		const char* const	theEnvString =
#if defined(XALAN_STRICT_ANSI_HEADERS)
			std::getenv(c_str(TranscodeToLocalCodePage(fullName)));
#else
			getenv(c_str(TranscodeToLocalCodePage(fullName)));
#endif

		if (theEnvString != 0)
		{
			XPathExecutionContext::GetAndReleaseCachedString	guard(executionContext);

			XalanDOMString&		result = guard.get();

			result = TranscodeFromLocalCodePage(theEnvString);

			return executionContext.getXObjectFactory().createString(result);
		}
	}

	return executionContext.getXObjectFactory().createStringReference(s_emptyString);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionSystemProperty*
#endif
FunctionSystemProperty::clone() const
{
	return new FunctionSystemProperty(*this);
}



const XalanDOMString
FunctionSystemProperty::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"system-property()");
}



XALAN_CPP_NAMESPACE_END
