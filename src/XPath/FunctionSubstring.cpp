/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
#include <XPath/FunctionSubstring.hpp>



FunctionSubstring::FunctionSubstring()
{
}



FunctionSubstring::~FunctionSubstring()
{
}



XObject*
FunctionSubstring::execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						/* context */,			
		const XObject*					arg1,
		const XObject*					arg2,
		const XObject*					arg3)
{
	assert(arg1 != 0 || arg2 != 0);

	const XalanDOMString&	theSourceString = arg1->str();
	const unsigned int		theSourceStringLength = length(theSourceString);

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanDOMChar>		VectorType;
#else
	typedef std::vector<XalanDOMChar>	VectorType;
#endif

	// This buffer will hold the output characters.
	VectorType	theBuffer;

	if (theSourceStringLength > 0)
	{
		// The substring can only be as long as the source string,
		// so reserve enough space now.  Also reserve space for
		// the terminating 0.
		theBuffer.reserve(theSourceStringLength);

		// $$$ ToDo: Add support for NaN.

		// Get the value of the second argument...
		const double	theSecondArgValue =
			FunctionRound::getRoundedValue(arg2->num());

		// Now, total the second and third arguments.  If
		// the third argument is missing, make the total
		// DBL_MAX.
		const double	theTotal =
				arg3 == 0 ? DBL_MAX :
								   FunctionRound::getRoundedValue(
									   theSecondArgValue + arg3->num());

		if (DoubleSupport::isNaN(theSecondArgValue) == false &&
			DoubleSupport::isNaN(theTotal) == false &&
			DoubleSupport::isNegativeInfinity(theTotal) == false)
		{
			// Start with 1, since strings are index from 1 in the XPath spec,
			for (unsigned int i = 1; i <= theSourceStringLength; i++)
			{
				// Is the index greater than or equal to the second argument?
				if (i >= theSecondArgValue)
				{
					// Is it less than the sum of the second and
					// third arguments?
					if (i < theTotal)
					{
						// It is, so include the character.
						theBuffer.push_back(charAt(theSourceString, i - 1));
					}
					else
					{
						// It's not, so stop.
						break;
					}
				}
			}
		}
	}

	const VectorType::size_type		theSize = theBuffer.size();

	if (theSize == 0)
	{
		return executionContext.getXObjectFactory().createString(XalanDOMString());
	}
	else
	{
		return executionContext.getXObjectFactory().createString(XalanDOMString(theBuffer.begin(), theSize));
	}
}



XObject*
FunctionSubstring::execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,			
		const XObject*					arg1,
		const XObject*					arg2)
{
	assert(arg1 != 0 || arg2 != 0);	

	return execute(executionContext, context, arg1, arg2, 0);
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
	return "The substring() function takes two or three arguments!";
}

