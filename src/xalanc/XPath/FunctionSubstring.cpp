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
#include "FunctionSubstring.hpp"



#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XObjectPtr		FunctionSubstring::s_nullXObjectPtr;



FunctionSubstring::FunctionSubstring()
{
}



FunctionSubstring::~FunctionSubstring()
{
}



/*
 * Get the value for the start index (C-style, not XPath).
 */
inline XalanDOMString::size_type
getStartIndex(
			double						theSecondArgValue,
			XalanDOMString::size_type	theStringLength)
{
	// We always subtract 1 for C-style index, since XPath indexes from 1.
	
	// Anything less than, or equal to 1 is 0.
	if (theSecondArgValue <= 1 ||
		DoubleSupport::isNaN(theSecondArgValue) == true)
	{
		return 0;
	}
	else if (DoubleSupport::isPositiveInfinity(theSecondArgValue) == true)
	{
		return theStringLength;
	}
	else
	{
		return XalanDOMString::size_type(DoubleSupport::round(theSecondArgValue)) - 1;
	}
}



/*
 * Get the length of the substring.
 */
inline XalanDOMString::size_type
getSubstringLength(
			XalanDOMString::size_type	theSourceStringLength,
			XalanDOMString::size_type	theStartIndex,
			double						theThirdArgValue)
{
	// The last index must be less than theThirdArgValue.  Since it has
	// already been rounded, subtracting 1 will do the job.
	const XalanDOMString::size_type		theLastIndex = XalanDOMString::size_type(theThirdArgValue - 1);

	if (theLastIndex >= theSourceStringLength)
	{
		return theSourceStringLength - theStartIndex;
	}
	else
	{
		return theLastIndex - theStartIndex;
	}
}



/*
 * Get the total of the second and third arguments.
 */
inline double
getTotal(
			XalanDOMString::size_type	theSourceStringLength,
			double						theSecondArgValue,
			const XObjectPtr&			arg3)
{
	// Total the second and third arguments.  If the third argument is
	// missing, make it the length of the string + 1 (for XPath
	// indexing style).
	if (arg3.null() == true)
	{
		return double(theSourceStringLength + 1);
	}
	else
	{
		const double	theRoundedValue =
			DoubleSupport::round(DoubleSupport::add(theSecondArgValue, arg3->num()));

		// If there's overflow, then we should return the length of the string + 1.
		if (DoubleSupport::isPositiveInfinity(theRoundedValue) == true)
		{
			return double(theSourceStringLength + 1);
		}
		else
		{
			return theRoundedValue;
		}
	}
}



static const XalanDOMString		theEmptyString;


inline XObjectPtr
createEmptyString(XPathExecutionContext&	executionContext)
{
	return executionContext.getXObjectFactory().createStringReference(theEmptyString);
}



XObjectPtr
FunctionSubstring::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const LocatorType*		locator) const
{
	assert(arg1.null() == false && arg2.null() == false);

	return execute(executionContext, context, arg1, arg2, s_nullXObjectPtr, locator);
}



XObjectPtr
FunctionSubstring::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const XObjectPtr		arg3,
			const LocatorType*		/* locator */) const
{
	assert(arg1.null() == false && arg2.null() == false);	

	const XalanDOMString&				theSourceString = arg1->str();
	const XalanDOMString::size_type		theSourceStringLength = length(theSourceString);

	if (theSourceStringLength == 0)
	{
		return createEmptyString(executionContext);
	}
	else
	{
		// Get the value of the second argument...
		const double	theSecondArgValue =
			DoubleSupport::round(arg2->num());

		// XPath indexes from 1, so this is the first XPath index....
		const XalanDOMString::size_type		theStartIndex = getStartIndex(theSecondArgValue, theSourceStringLength);

		if (theStartIndex >= theSourceStringLength)
		{
			return createEmptyString(executionContext);
		}
		else
		{
			const double	theTotal =
				getTotal(theSourceStringLength, theSecondArgValue, arg3);

			if (DoubleSupport::isNaN(theSecondArgValue) == true ||
				DoubleSupport::isNaN(theTotal) == true ||
				DoubleSupport::isNegativeInfinity(theTotal) == true ||
				theTotal == 0.0 ||
				theTotal < double(theStartIndex))
			{
				return createEmptyString(executionContext);
			}
			else
			{
				const XalanDOMString::size_type		theSubstringLength =
					getSubstringLength(
						theSourceStringLength,
						theStartIndex,
						theTotal);

				XPathExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

				XalanDOMString&		theString = theResult.get();

				assign(
						theString,
						toCharArray(theSourceString) + theStartIndex,
						theSubstringLength);

				return executionContext.getXObjectFactory().createString(theResult);
			}
		}
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionSubstring*
#endif
FunctionSubstring::clone() const
{
	return new FunctionSubstring(*this);
}



const XalanDOMString
FunctionSubstring::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionTakesTwoOrThreeArguments_1Param, "substring()");
}



XALAN_CPP_NAMESPACE_END
