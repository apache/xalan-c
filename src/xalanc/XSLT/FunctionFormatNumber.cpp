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
#include "FunctionFormatNumber.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionFormatNumber::FunctionFormatNumber()
{
}



FunctionFormatNumber::~FunctionFormatNumber()
{
}



XObjectPtr
FunctionFormatNumber::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const LocatorType*		locator) const
{
	assert(arg1.null() == false && arg2.null() == false);	
	
	const double						theNumber = arg1->num();
	const XalanDOMString&				thePattern = arg2->str();
	
	typedef XPathExecutionContext::GetAndReleaseCachedString	GetAndReleaseCachedString;

	GetAndReleaseCachedString	theString(executionContext);

	executionContext.formatNumber(
			theNumber, 
			thePattern, 
			theString.get(), 
			context, 
			locator);	

	return executionContext.getXObjectFactory().createString(theString);
}



XObjectPtr
FunctionFormatNumber::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const XObjectPtr		arg3,
			const LocatorType*		locator) const
{
	assert(arg1.null() == false && arg2.null() == false && arg3.null() == false);
	
	const double						theNumber = arg1->num();
	const XalanDOMString&				thePattern = arg2->str();

	const XalanDOMString&				theDFSName = arg3->str();
	assert(length(theDFSName) != 0);
	
	typedef XPathExecutionContext::GetAndReleaseCachedString	GetAndReleaseCachedString;

	GetAndReleaseCachedString	theString(executionContext);

	executionContext.formatNumber(
		theNumber, 
		thePattern,
		theDFSName,
		theString.get(),
		context, 
		locator);

	return executionContext.getXObjectFactory().createString(theString);
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionFormatNumber*
#endif
FunctionFormatNumber::clone() const
{
	return new FunctionFormatNumber(*this);
}

const XalanDOMString
FunctionFormatNumber::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionTakesTwoOrThreeArguments_1Param,"format-number()");
}

XALAN_CPP_NAMESPACE_END
