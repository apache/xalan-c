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
#include "FunctionTranslate.hpp"



#include "XObjectFactory.hpp"



FunctionTranslate::FunctionTranslate()
{
}



FunctionTranslate::~FunctionTranslate()
{
}



XObjectPtr
FunctionTranslate::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



XObjectPtr
FunctionTranslate::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		/* arg1 */)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



XObjectPtr
FunctionTranslate::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		/* arg1 */,
			const XObjectPtr		/* arg2 */)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



XObjectPtr
FunctionTranslate::execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						/* context */,			
		const XObjectPtr				arg1,
		const XObjectPtr				arg2,
		const XObjectPtr				arg3)
{
	assert(arg1.null() == false && arg2.null() == false && arg3.null() == false);

	const XalanDOMString&	theFirstString = arg1->str();
	const XalanDOMString&	theSecondString = arg2->str();
	const XalanDOMString&	theThirdString = arg3->str();

	const unsigned int		theFirstStringLength = length(theFirstString);
	const unsigned int		theSecondStringLength = length(theSecondString);
	const unsigned int		theThirdStringLength = length(theThirdString);

#if defined(XALAN_NO_NAMESPACES)
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

	for (unsigned int i = 0; i < theFirstStringLength; i++)
	{
		const XalanDOMChar		theCurrentChar = charAt(theFirstString, i);

		const unsigned int		theIndex = indexOf(theSecondString, theCurrentChar);

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



XObjectPtr
FunctionTranslate::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			int								/* opPos */,
			const XObjectArgVectorType&		/* args */)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
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
	return XALAN_STATIC_UCODE_STRING("The translate() function takes three arguments!");
}
