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

// Class header file...
#include "Function.hpp"



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



Function::Function()
{
}



Function::~Function()
{
}



XObjectPtr
Function::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theArgCount = args.size();

	if (theArgCount == 0)
	{
		return execute(executionContext, context, locator);
	}
	else if (theArgCount == 1)
	{
		return execute(executionContext, context, args[0], locator);
	}
	else if (theArgCount == 2)
	{
		return execute(executionContext, context, args[0], args[1], locator);
	}
	else if (theArgCount == 3)
	{
		return execute(executionContext, context, args[0], args[1], args[2], locator);
	}
	else
	{
		executionContext.error(getError(), context, locator);

		return XObjectPtr(0);
	}
}



XObjectPtr
Function::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const LocatorType*		locator) const
{
	executionContext.error(getError(), context, locator);

	return XObjectPtr(0);
}



XObjectPtr
Function::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		/* arg1 */,
			const LocatorType*		locator) const
{
	executionContext.error(getError(), context, locator);

	return XObjectPtr(0);
}



XObjectPtr
Function::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		/* arg1 */,
			const XObjectPtr		/* arg2 */,
			const LocatorType*		locator) const
{
	executionContext.error(getError(), context, locator);

	return XObjectPtr(0);
}



XObjectPtr
Function::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		/* arg1 */,
			const XObjectPtr		/* arg2 */,
			const XObjectPtr		/* arg3 */,
			const LocatorType*		locator) const
{
	executionContext.error(getError(), context, locator);

	return XObjectPtr(0);
}



XALAN_CPP_NAMESPACE_END
