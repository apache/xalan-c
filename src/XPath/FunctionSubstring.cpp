/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#include "FunctionSubstring.hpp"



#include <PlatformSupport/DoubleSupport.hpp>



#include "XObjectFactory.hpp"



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
inline unsigned int
getStartIndex(double	theSecondArgValue)
{
	// We always subtract 1 for C-style index, since XPath indexes from 1.
	
	// Anything less than, or equal to 1 is 0.
	if (theSecondArgValue <= 1)
	{
		return 0;
	}
	else
	{
		return unsigned(DoubleSupport::round(theSecondArgValue)) - 1;
	}
}



/*
 * Get the length of the substring.
 */
inline unsigned int
getSubstringLength(
			unsigned int	theSourceStringLength,
			unsigned int	theStartIndex,
			double			theThirdArgValue)
{
	// The last index must be less than theThirdArgValue.  Since it has
	// already been rounded, subtracting 1 will do the job.
	const unsigned int	theLastIndex = unsigned(theThirdArgValue - 1);

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
			unsigned int		theSourceStringLength,
			double				theSecondArgValue,
			const XObjectPtr&	arg3)
{
	// Total the second and third arguments.  Ithe third argument is
	// missing, make it the length of the string + 1 (for XPath
	// indexing style).
	if (arg3.null() == true)
	{
		return double(theSourceStringLength + 1);
	}
	else
	{
		const double	theRoundedValue =
			DoubleSupport::round(theSecondArgValue + arg3->num());

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
	return executionContext.getXObjectFactory().createString(theEmptyString);
}



XObjectPtr
FunctionSubstring::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const Locator*			locator) const
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
			const Locator*			/* locator */) const
{
	assert(arg1.null() == false && arg2.null() == false);	

	const XalanDOMString&	theSourceString = arg1->str();
	const unsigned int		theSourceStringLength = length(theSourceString);

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
		const unsigned int	theStartIndex = getStartIndex(theSecondArgValue);

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
				theTotal < double(theStartIndex))
			{
				return createEmptyString(executionContext);
			}
			else
			{
				const unsigned int	theSubstringLength =
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
	return XALAN_STATIC_UCODE_STRING("The substring() function takes two or three arguments!");
}
