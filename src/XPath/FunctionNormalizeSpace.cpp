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
#include <XPath/FunctionNormalizeSpace.hpp>



FunctionNormalizeSpace::FunctionNormalizeSpace()
{
}



FunctionNormalizeSpace::~FunctionNormalizeSpace()
{
}



XObjectPtr
FunctionNormalizeSpace::execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						/* context */,			
		const XObjectPtr				arg1)
{
	assert(arg1.null() == false);	
		
	return normalize(executionContext, arg1->str());
}



XObjectPtr
FunctionNormalizeSpace::execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context)
{
	if (context == 0)
	{
		executionContext.error("The normalize-space() function requires a non-null context node!",
							   context);

		return 0;
	}
	else
	{
		// The XPath standard says that if there
		// are no arguments, the default is to turn the contextNode
		// into a string-value, which really means using FunctionString,
		// but we don't need to do that, since our XObject classes
		// do the real work in turning themselves into strings.

		// A node set that contains the context node.
		XObjectPtr	theXObject(executionContext.createNodeSet(*context));

		return normalize(executionContext, theXObject->str());
	}
}



XObjectPtr
FunctionNormalizeSpace::normalize(
		XPathExecutionContext&	executionContext,
		const XalanDOMString&	theString)
{
	const unsigned int	theStringLength = length(theString);

	XalanDOMChar		thePreviousChar = 0;

	// A vector to contain the new characters.  We'll use it to construct
	// the result string.
#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanDOMChar>		VectorType;
#else
	typedef std::vector<XalanDOMChar>	VectorType;
#endif

	// A vector to contain the result.
	VectorType	theVector;

	// The result string can only be as large as the source string, so
	// just reserve the space now.
	theVector.reserve(theStringLength);

	// OK, strip out any multiple spaces...
	for (unsigned int i = 0; i < theStringLength; i++)
	{
		const XalanDOMChar	theCurrentChar = charAt(theString, i);

		if (isXMLWhitespace(theCurrentChar) == true)
		{
			// If the previous character wasn't a space, and we've
			// encountered some non-space characters, then push the
			// space.
			if (isXMLWhitespace(thePreviousChar) == false && theVector.size() > 0)
			{
				theVector.push_back(XalanDOMChar(XalanUnicode::charSpace));
			}
		}
		else
		{
			theVector.push_back(theCurrentChar);
		}

		thePreviousChar = theCurrentChar;
	}

	VectorType::size_type	theSize = theVector.size();

	if (theSize == 0)
	{
		return executionContext.getXObjectFactory().createString(XalanDOMString());
	}
	else
	{
		if (isXMLWhitespace(theVector.back()) == true)
		{
			// The last character is a space, so remove it...
			--theSize;
		}

		return executionContext.getXObjectFactory().createString(theVector.begin(), theSize);
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
	return XALAN_STATIC_UCODE_STRING(
		"The normalize-space() function takes zero arguments or one argument!");
}
