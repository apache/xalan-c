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
#include "FunctionFormatNumber.hpp"



#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanDecimalFormat.hpp>
#include <PlatformSupport/XalanDecimalFormatSymbols.hpp>



#include <XPath/XObjectFactory.hpp>



#include "Constants.hpp"



FunctionFormatNumber::FunctionFormatNumber()
{
}



FunctionFormatNumber::~FunctionFormatNumber()
{
}



XObjectPtr
FunctionFormatNumber::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



XObjectPtr
FunctionFormatNumber::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		/* arg1 */)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



XObjectPtr
FunctionFormatNumber::execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,			
		const XObjectPtr				arg1,
		const XObjectPtr				arg2)
{
	assert(arg1.null() == false && arg2.null() == false);	
	
	const double						theNumber = arg1->num();
	const XalanDOMString&				thePattern = arg2->str();
	
	const XalanDecimalFormatSymbols*	theDFS = 0;

	if (theDFS == 0)
	{
		theDFS = executionContext.getDecimalFormatSymbols(Constants::DEFAULT_DECIMAL_FORMAT);
	}

	typedef XPathExecutionContext::GetAndReleaseCachedString	GetAndReleaseCachedString;

	GetAndReleaseCachedString	theString(executionContext);

	doFormat(
			executionContext,
			context,
			theNumber,
			thePattern,
			theDFS,
			theString.get());

	return executionContext.getXObjectFactory().createString(theString);
}



XObjectPtr
FunctionFormatNumber::execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,			
		const XObjectPtr				arg1, 
		const XObjectPtr				arg2,
		const XObjectPtr				arg3)
{
	assert(arg1.null() == false && arg2.null() == false && arg3.null() == false);
	
	const double						theNumber = arg1->num();
	const XalanDOMString&				thePattern = arg2->str();

	const XalanDOMString&				theDecimalFormatName = arg3->str();
	assert(length(theDecimalFormatName) != 0);

	const XalanDecimalFormatSymbols*	theDFS =
			executionContext.getDecimalFormatSymbols(theDecimalFormatName);

	if (theDFS == 0)
	{
		executionContext.warn(s_warningNotFoundString,
							  context);

		theDFS = executionContext.getDecimalFormatSymbols(Constants::DEFAULT_DECIMAL_FORMAT);
	
	}	

	typedef XPathExecutionContext::GetAndReleaseCachedString	GetAndReleaseCachedString;

	GetAndReleaseCachedString	theString(executionContext);

	doFormat(
			executionContext,
			context,
			theNumber,
			thePattern,
			theDFS,
			theString.get());

	return executionContext.getXObjectFactory().createString(theString);
}



XObjectPtr
FunctionFormatNumber::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			int								/* opPos */,
			const XObjectArgVectorType&		/* args */)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



void
FunctionFormatNumber::doFormat(
			XPathExecutionContext&				executionContext,
			XalanNode*							context,
			double								theNumber,
			const XalanDOMString&				thePattern,
			const XalanDecimalFormatSymbols*	theDFS,
			XalanDOMString&						theResult)
{
	if (DoubleSupport::isNaN(theNumber) == true ||
		DoubleSupport::isNegativeInfinity(theNumber) == true ||
		DoubleSupport::isPositiveInfinity(theNumber) == true )
	{
		DoubleToDOMString(theNumber, theResult);
	}
	else
	{
		executionContext.warn(s_warningNotImplementedString, context);

		if (theDFS != 0)
		{
			XalanDecimalFormat	theFormatter(s_emptyString, *theDFS);

			theFormatter.applyLocalizedPattern(thePattern);

			theFormatter.format(theNumber, theResult);
		}
		else
		{
			XalanDecimalFormat	theFormatter(s_emptyString, m_decimalFormatSymbols);

			theFormatter.applyLocalizedPattern(thePattern);

			theFormatter.format(theNumber, theResult);
		}
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionFormatNumber*
#endif
FunctionFormatNumber::clone() const
{
	return new FunctionFormatNumber(*this);
}



const XalanDOMString
FunctionFormatNumber::getError() const
{
	return XALAN_STATIC_UCODE_STRING("The format-number() function takes two or three arguments!");
}



static XalanDOMString	s_warningNotImplementedString;
static XalanDOMString	s_warningNotFoundString;


const XalanDOMString&	FunctionFormatNumber::s_warningNotImplementedString =
		s_warningNotImplementedString;

const XalanDOMString&	FunctionFormatNumber::s_warningNotFoundString =
		s_warningNotFoundString;

const XalanDOMString	FunctionFormatNumber::s_emptyString;


void
FunctionFormatNumber::initialize()
{
	::s_warningNotImplementedString = XALAN_STATIC_UCODE_STRING("format-number() is not fully implemented!");

	::s_warningNotFoundString = XALAN_STATIC_UCODE_STRING("format-number: Specified decimal-format element not found!");
}



void
FunctionFormatNumber::terminate()
{
	releaseMemory(::s_warningNotImplementedString);

	releaseMemory(::s_warningNotFoundString);
}
