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
#include "FunctionFunctionAvailable.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionFunctionAvailable::FunctionFunctionAvailable()
{
}



FunctionFunctionAvailable::~FunctionFunctionAvailable()
{
}



XObjectPtr
FunctionFunctionAvailable::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,
			const XObjectPtr		arg,
			const LocatorType*		locator) const
{
	assert(arg.null() == false);

	return executionContext.getXObjectFactory().createBoolean(executionContext.functionAvailable(arg->str(), locator));
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionFunctionAvailable*
#endif
FunctionFunctionAvailable::clone() const
{
	return new FunctionFunctionAvailable(*this);
}



const XalanDOMString
FunctionFunctionAvailable::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionRequiresNonNullContextNode_1Param,"function-available()");
}



XALAN_CPP_NAMESPACE_END
