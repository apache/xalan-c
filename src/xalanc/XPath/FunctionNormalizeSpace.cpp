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

#include "FunctionNormalizeSpace.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionNormalizeSpace::FunctionNormalizeSpace()
{
}



FunctionNormalizeSpace::~FunctionNormalizeSpace()
{
}



XObjectPtr
FunctionNormalizeSpace::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const LocatorType*		locator) const
{
	if (context == 0)
	{
		executionContext.error(
				XalanMessageLoader::getMessage(XalanMessages::FunctionRequiresNonNullContextNode_1Param, "normalize-space()"),
				context,
				locator);

		// Dummy return value...
		return XObjectPtr(0);
	}
	else
	{
		// The XPath standard says that if there are no arguments,
		// the default is to turn the context node into a string value.
		// DOMServices::getNodeData() will give us the data.

		// Get a cached string...
		XPathExecutionContext::GetAndReleaseCachedString	theData(executionContext);

		XalanDOMString&		theString = theData.get();

		DOMServices::getNodeData(*context, theString);

		return normalize(executionContext, theString);
	}
}



XObjectPtr
FunctionNormalizeSpace::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,
			const XObjectPtr		arg1,
			const LocatorType*		/* locator */) const
{
	assert(arg1.null() == false);	
		
	return normalize(executionContext, arg1);
}



XObjectPtr
FunctionNormalizeSpace::normalize(
			XPathExecutionContext&	executionContext,
			const XalanDOMString&	theString) const
{
	const XalanDOMString::size_type		theStringLength = length(theString);

	// A string contain the result...
	XPathExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

	XalanDOMString&		theNewString = theResult.get();
	assert(length(theNewString) == 0);

	// The result string can only be as large as the source string, so
	// just reserve the space now.
	reserve(theNewString, theStringLength);

	bool	fPreviousIsSpace = false;

	// OK, strip out any multiple spaces...
	for (unsigned int i = 0; i < theStringLength; i++)
	{
		const XalanDOMChar	theCurrentChar = charAt(theString, i);

		if (isXMLWhitespace(theCurrentChar) == true)
		{
			// If the previous character wasn't a space, and we've
			// encountered some non-space characters, and it's not
			// the last character in the string, then push the
			// space character (not the original character).
			if (fPreviousIsSpace == false)
			{
				if (length(theNewString) > 0 &&
					i < theStringLength - 1)
				{
					append(theNewString, XalanDOMChar(XalanUnicode::charSpace));
				}

				fPreviousIsSpace = true;
			}
		}
		else
		{
			append(theNewString, theCurrentChar);

			fPreviousIsSpace = false;
		}
	}

	const XalanDOMString::size_type		theNewStringLength = length(theNewString);

	if (theNewStringLength == 0)
	{
		return executionContext.getXObjectFactory().createString(XalanDOMString());
	}
	else
	{
		// We may have a space character at end, since we don't look ahead,
		// so removed it now...
		if (charAt(theNewString, theNewStringLength - 1) ==
				XalanDOMChar(XalanUnicode::charSpace))
		{
			theNewString.erase(theNewStringLength - 1, 1);
		}

		return executionContext.getXObjectFactory().createString(theResult);
	}
}



XObjectPtr
FunctionNormalizeSpace::normalize(
			XPathExecutionContext&	executionContext,
			const XObjectPtr&		theArg) const
{
	const XalanDOMString&	theString = theArg->str();

	if (needsNormalization(theString) == false)
	{
		if (theArg->getType() == XObject::eTypeString)
		{
			return theArg;
		}
		else
		{
			return executionContext.getXObjectFactory().createStringAdapter(theArg);
		}
	}
	else
	{
		return normalize(executionContext, theString);
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionNormalizeSpace*
#endif
FunctionNormalizeSpace::clone() const
{
	return new FunctionNormalizeSpace(*this);
}



const XalanDOMString
FunctionNormalizeSpace::getError() const
{
	
	return XalanMessageLoader::getMessage(XalanMessages::FunctionTakesZeroOrOneArg_1Param, "normalize-space()");

}



bool
FunctionNormalizeSpace::needsNormalization(const XalanDOMString&	theString) const
{
	const XalanDOMString::size_type		theStringLength = length(theString);

	bool	fNormalize = false;

	bool	fPreviousIsSpace = false;

	// OK, search for multiple spaces, or whitespace that is not the
	// space character...
	for (XalanDOMString::size_type i = 0; i < theStringLength && fNormalize == false; ++i)
	{
		const XalanDOMChar	theCurrentChar = charAt(theString, i);

		if (isXMLWhitespace(theCurrentChar) == false)
		{
			fPreviousIsSpace = false;
		}
		else
		{
			if (i == 0 ||
				i == theStringLength - 1 ||
				theCurrentChar != XalanDOMChar(XalanUnicode::charSpace) ||
				fPreviousIsSpace == true)
			{
				fNormalize = true;
			}
			else
			{
				fPreviousIsSpace = true;
			}
		}
	}

	return fNormalize;
}



XALAN_CPP_NAMESPACE_END
