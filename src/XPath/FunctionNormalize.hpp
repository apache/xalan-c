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
#if !defined(FUNCTIONNORMALIZE_HEADER_GUARD_1357924680)
#define FUNCTIONNORMALIZE_HEADER_GUARD_1357924680

// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



// Base class header file...
#include <XPath/FunctionDefaultStringArgument.hpp>



#include <XPath/NodeRefListBase.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathExecutionContext.hpp>



/**
 * XPath implementation of "normalize-space" function.
 */
//
// These are all inline, even though
// there are virtual functions, because we expect that they will only be
// needed by the XPath class.
class XALAN_XPATH_EXPORT FunctionNormalizeSpace : public FunctionDefaultStringArgument
{
public:

	// These methods are inherited from FunctionDefaultStringArgument ...

	virtual XObject*
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			int								/* opPos */,
			const XObjectArgVectorType&		args)
	{
		XalanDOMString	theSourceString;

		if(args.size() > 2)
		{
			executionContext.error("The normalize-space() function takes zero arguments or one argument!",
								   context);

			// Dummy return...
			return 0;
		}
		else if (args.size() == 1)
		{
			return normalize(executionContext, args[0]->str());
		}
		else if (context == 0)
		{
			executionContext.error("The normalize-space() function requires a non-null context node!",
								   context);

			// Dummy return...
			return 0;
		}
		else
		{
			return normalize(executionContext, getDefaultStringArgument(executionContext, *context));
		}
	}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual FunctionNormalizeSpace*
#endif
	clone() const
	{
		return new FunctionNormalizeSpace(*this);
	}

private:

	XObject*
	normalize(
			XPathExecutionContext&	executionContext,
			const XalanDOMString&	theString)
	{
		const unsigned int		theStringLength = length(theString);

		XalanDOMChar			thePreviousChar = 0;

		// A vector to contain the new characters.  We'll use it to construct
		// the result string.
#if !defined(XALAN_NO_NAMESPACES)
		using std::vector;
#endif

#if defined(XALAN_NO_NAMESPACES)
		typedef vector<XalanDOMChar>		VectorType;
#else
		typedef std::vector<XalanDOMChar>	VectorType;
#endif

		// A vector to contain the result.
		VectorType				theVector;

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
				// The last character is a space, so remove it
				--theSize;
			}

			return executionContext.getXObjectFactory().createString(XalanDOMString(theVector.begin(), theSize));
		}
	}

	// Not implemented...
	FunctionNormalizeSpace&
	operator=(const FunctionNormalizeSpace&);

	bool
	operator==(const FunctionNormalizeSpace&) const;
};



#endif	// FUNCTIONNORMALIZE_HEADER_GUARD_1357924680
