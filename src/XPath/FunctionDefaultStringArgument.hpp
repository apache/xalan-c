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
#if !defined(FUNCTIONDEFAULTSTRINGARGUMENT_HEADER_GUARD_1357924680)
#define FUNCTIONDEFAULTSTRINGARGUMENT_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



// Base class header file...
#include <XPath/Function.hpp>



#include <XPath/FunctionString.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathExecutionContext.hpp>



/**
 * XPath FunctionDefaultStringArgument implementation. This is a base class
 * for all string functions which have a default argument.  It is NOT a
 * concrete class.
 */
//
// These are all inline, even though
// there are virtual functions, because we expect that they will only be
// needed by the XPath class.
//
class XALAN_XPATH_EXPORT FunctionDefaultStringArgument : public Function
{
public:

	/**
	 * Execute an XPath function object.  The function must return 
	 * a valid object.
	 *
	 * @param executionContext current context
	 * @param opPos            current op position
	 * @param args             list of XObject arguments
	 * @return valid XObject
	 */
	virtual XObject*
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			int								opPos,
			const XObjectArgVectorType&		args) = 0;

	// These methods are inherited from Function ...

	virtual Function*
	clone() const = 0;

protected:

	virtual XalanDOMString
	getDefaultStringArgument(
			XPathExecutionContext&		executionContext,
			XalanNode&					context,
			int							opPos)
	{
		// This is complicated.  The XPath standard says that if there
		// are no arguments, the default is to turn the contextNode
		// into a string-value, which really means using FunctionString.
		// 
		// So we have to create a context for calling FunctionString
		// with the context node.  We shroud the temporary XObjects in
		// XString in FactoryObjectAutoPointers because they can be returned
		// once we've converted the context node to an XObject.

		// A vector for the args.  The size will always be one.
		XObjectArgVectorType	theNewArgs(1);

		// A node set that contains the context node.
		FactoryObjectAutoPointer<XObject>		theArg(&executionContext.getXObjectFactory(),
													   executionContext.getXObjectFactory().createNodeSet(context));

		// Put the argument into the vector...
		theNewArgs[0] = theArg.get();

		// This is our string functor.
		FunctionString		theStringFunctor;

		// Get the result...
		FactoryObjectAutoPointer<XObject>		theXString(&executionContext.getXObjectFactory(),
														   theStringFunctor.execute(executionContext,
																					&context,
																					opPos,
																					theNewArgs));

		// Now, get the string from the XObject.
			return theXString->str();
	}

private:

	// Not implemented...
	FunctionDefaultStringArgument&
	operator=(const FunctionDefaultStringArgument&);

	bool
	operator==(const FunctionDefaultStringArgument&) const;
};



#endif	// FUNCTIONDEFAULTSTRINGARGUMENT_HEADER_GUARD_1357924680
