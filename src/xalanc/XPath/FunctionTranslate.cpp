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
#include "FunctionTranslate.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionTranslate::FunctionTranslate()
{
}



FunctionTranslate::~FunctionTranslate()
{
}



XObjectPtr
FunctionTranslate::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,			
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const XObjectPtr		arg3,
			const LocatorType*		/* locator */) const
{
	assert(arg1.null() == false && arg2.null() == false && arg3.null() == false);

	const XalanDOMString&	theFirstString = arg1->str();
	const XalanDOMString&	theSecondString = arg2->str();
	const XalanDOMString&	theThirdString = arg3->str();

	const XalanDOMString::size_type		theFirstStringLength = length(theFirstString);
	const XalanDOMString::size_type		theSecondStringLength = length(theSecondString);
	const XalanDOMString::size_type		theThirdStringLength = length(theThirdString);

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<XalanDOMChar>		VectorType;
#else
	typedef std::vector<XalanDOMChar>	VectorType;
#endif

	// A string to hold the result.
	XPathExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

	XalanDOMString&		theString = theResult.get();

	// The result string can only be as large as the first string, so
	// just reserve the space now.  Also reserve space for the
	// terminating 0.
	reserve(theString, theFirstStringLength + 1);

	for (XalanDOMString::size_type i = 0; i < theFirstStringLength; i++)
	{
		const XalanDOMChar					theCurrentChar = charAt(theFirstString, i);

		const XalanDOMString::size_type		theIndex = indexOf(theSecondString, theCurrentChar);

		if (theIndex >= theSecondStringLength)
		{
			// Didn't find the character in the second string, so it
			// is not translated.
			append(theString, theCurrentChar);
		}
		else if (theIndex < theThirdStringLength)
		{
			// OK, there's a corresponding character in the
			// third string, so do the translation...
			append(theString, charAt(theThirdString, theIndex));
		}
		else
		{
			// There's no corresponding character in the
			// third string, since it's shorter than the
			// second string.  In this case, the character
			// is removed from the output string, so don't
			// do anything.
		}
	}

	return executionContext.getXObjectFactory().createString(theResult);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionTranslate*
#endif
FunctionTranslate::clone() const
{
	return new FunctionTranslate(*this);
}



const XalanDOMString
FunctionTranslate::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionTakesThreeArguments_1Param, "translate()");
}



XALAN_CPP_NAMESPACE_END
