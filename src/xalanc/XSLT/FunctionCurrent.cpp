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
#include "FunctionCurrent.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionCurrent::FunctionCurrent()
{
}



FunctionCurrent::~FunctionCurrent()
{
}



XObjectPtr
FunctionCurrent::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,
			const LocatorType*		/* locator */) const
{
	assert(executionContext.getCurrentNode() != 0);

	return executionContext.getXObjectFactory().createNodeSet(executionContext.getCurrentNode());
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionCurrent*
#endif
FunctionCurrent::clone() const
{
	return new FunctionCurrent(*this);
}



const XalanDOMString
FunctionCurrent::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionDoesNotAcceptAnyArguments_1Param,"current()");
}



XALAN_CPP_NAMESPACE_END
