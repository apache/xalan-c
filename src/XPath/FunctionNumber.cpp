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
#include "FunctionNumber.hpp"



#include <DOMSupport/DOMServices.hpp>



#include "XObjectFactory.hpp"



FunctionNumber::FunctionNumber()
{
}



FunctionNumber::~FunctionNumber()
{
}



XObjectPtr
FunctionNumber::execute(
		XPathExecutionContext&	executionContext,
		XalanNode*				context)
{
	if (context == 0)
	{
		executionContext.error("The number() function requires a non-null context node!");

		// Dummy return value...
		return XObjectPtr(0);
	}
	else
	{
		// The XPath standard says that if there are no arguments,
		// the argument defaults to a node set with the context node
		// as the only member.  The number value of a node set is the
		// string value of the first node in the node set, converted to
		// a number.  DOMServices::getNodeData() will give us the data.

		// Get a cached string...
		XPathExecutionContext::GetAndReleaseCachedString	theData(executionContext);

		DOMServices::getNodeData(*context, theData);

		return executionContext.getXObjectFactory().createNumber(DOMStringToDouble(theData));
	}
}



XObjectPtr
FunctionNumber::execute(
		XPathExecutionContext&	executionContext,
		XalanNode*				/* context */,			
		const XObjectPtr		arg1)
{
	assert(arg1.null() == false);	

	if (arg1->getType() == XObject::eTypeNumber)
	{
		// Since XObjects are reference counted, just return the
		// argument.
		return arg1;
	}
	else
	{
		return executionContext.getXObjectFactory().createNumber(arg1->num());
	}
}



XObjectPtr
FunctionNumber::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		/* arg1 */,
			const XObjectPtr		/* arg2 */)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



XObjectPtr
FunctionNumber::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		/* arg1 */,
			const XObjectPtr		/* arg2 */,
			const XObjectPtr		/* arg3 */)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



XObjectPtr
FunctionNumber::execute(
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
FunctionNumber*
#endif
FunctionNumber::clone() const
{
	return new FunctionNumber(*this);
}



const XalanDOMString
FunctionNumber::getError() const
{
	return XALAN_STATIC_UCODE_STRING(
		"The number() function takes zero arguments or one argument!");
}

