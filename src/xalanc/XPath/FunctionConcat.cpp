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
#include "FunctionConcat.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionConcat::FunctionConcat()
{
}



FunctionConcat::~FunctionConcat()
{
}



XObjectPtr
FunctionConcat::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,			
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const LocatorType*		/* locator */) const
{
	assert(arg1.null() == false && arg2.null() == false);	

	XPathExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

	XalanDOMString&		theString = theResult.get();

	arg1->str(theString);
	arg2->str(theString);

	return executionContext.getXObjectFactory().createString(theResult);
}



XObjectPtr
FunctionConcat::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,			
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const XObjectPtr		arg3,
			const LocatorType*		/* locator */) const
{
	assert(arg1.null() == false && arg2.null() == false && arg3.null() == false);	

	XPathExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

	XalanDOMString&			theString = theResult.get();

	arg1->str(theString);
	arg2->str(theString);
	arg3->str(theString);

	return executionContext.getXObjectFactory().createString(theResult);
}



XObjectPtr
FunctionConcat::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						/* context */,
			const XObjectArgVectorType&		args,
			const LocatorType*				/* locator */) const
{
	const XObjectArgVectorType::const_iterator	theEnd = args.end();

	XPathExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

	XalanDOMString&		theString = theResult.get();

	XObjectArgVectorType::const_iterator	i = args.begin();

	for(; i != theEnd; ++i)
	{
		(*i)->str(theString);
	}

	return executionContext.getXObjectFactory().createString(theResult);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionConcat*
#endif
FunctionConcat::clone() const
{
	return new FunctionConcat(*this);
}



const XalanDOMString
FunctionConcat::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionTakes2ArgsAtLeast_1Param, "concat()");
}



XALAN_CPP_NAMESPACE_END
