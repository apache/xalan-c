/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000-2002 The Apache Software Foundation.  All rights 
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

#include "FunctionNormalizeSpace.hpp"



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
				"The normalize-space() function requires a non-null context node!",
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
		return theArg;
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
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The normalize-space() function takes zero arguments or one argument!"));
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
