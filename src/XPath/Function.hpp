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
#if !defined(FUNCTION_HEADER_GUARD_1357924680)
#define FUNCTION_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <vector>



#include <XPath/XObject.hpp>
#include <XPath/XPathExecutionContext.hpp>



class XalanNode;



// Base class for all XPath function classes.
//
// These are all inline, even though
// there are virtual functions, because we expect that they will only be
// needed by the XPath class.
class XALAN_XPATH_EXPORT Function
{
public:

	explicit
	Function();

	virtual
	~Function();

	typedef XPathExecutionContext::XObjectArgVectorType	XObjectArgVectorType;

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object.
	 *
	 * @param executionContext executing context
	 * @param context          current context node
	 * @param opPos            current op position
	 * @param args             vector of pointers to XObject arguments
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			int								opPos,
			const XObjectArgVectorType&		args);

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object. Called if function has no parameters.
	 *
	 * @param executionContext executing context
	 * @param context          current context node	 
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context);

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object. Called if function has one parameter.
	 *
	 * @param executionContext executing context
	 * @param context          current context node
	 * @param arg              pointer to XObject argument
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(

			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg);

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object. Called if function has two parameters.
	 *
	 * @param executionContext executing context
	 * @param context          current context node
	 * @param arg1             pointer to XObject argument
	 * @param arg2             pointer to XObject argument
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(

			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		arg1,
			const XObjectPtr		arg2);

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object. Called if function has three parameters.
	 *
	 * @param executionContext executing context
	 * @param context          current context node
	 * @param arg1             pointer to XObject arguments
	 * @param arg2             pointer to XObject argument
	 * @param arg3             pointer to XObject argument
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const XObjectPtr		arg3);

	/**
	 * Create a copy of the function object.
	 *
	 * @return pointer to the new object
	 */
	virtual Function*
	clone() const = 0;

protected:

	/**
	 * Create a copy of the function object.
	 *
	 * @return string function name
	 */
	virtual const XalanDOMString
	getError() const;

private:

	// Not implemented...
	Function&
	operator=(const Function&);

	bool
	operator==(const Function&) const;
};



#endif	// FUNCTION_HEADER_GUARD_1357924680
