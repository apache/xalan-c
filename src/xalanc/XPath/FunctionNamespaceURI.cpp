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
#include "FunctionNamespaceURI.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionNamespaceURI::FunctionNamespaceURI()
{
}



FunctionNamespaceURI::~FunctionNamespaceURI()
{
}



static const XalanDOMString		theEmptyString;



XObjectPtr
FunctionNamespaceURI::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const LocatorType*		locator) const
{
	if (context == 0)
	{
		executionContext.error(
				XalanMessageLoader::getMessage(XalanMessages::FunctionRequiresNonNullContextNode_1Param, "namespace-uri()"),
				context,
				locator);

		// Dummy return value...
		return XObjectPtr(0);
	}
	else
	{
		return executionContext.getXObjectFactory().createStringReference(DOMServices::getNamespaceOfNode(*context));
	}
}



XObjectPtr
FunctionNamespaceURI::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,
			const XObjectPtr		arg1,
			const LocatorType*		locator) const
{
	assert(arg1.null() == false);	
	
	const NodeRefListBase&	theList = arg1->nodeset();

	if (theList.getLength() == 0)
	{
		return executionContext.getXObjectFactory().createStringReference(theEmptyString);
	}
	else
	{
		assert(theList.item(0) != 0);

		return execute(executionContext, theList.item(0), locator);
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionNamespaceURI*
#endif
FunctionNamespaceURI::clone() const
{
	return new FunctionNamespaceURI(*this);
}



const XalanDOMString
FunctionNamespaceURI::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionTakesZeroOrOneArg_1Param, "namespace-uri()");
}



XALAN_CPP_NAMESPACE_END
