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
#include "FunctionGenerateID.hpp"



#include <XalanDOM/XalanDocument.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/NodeRefListBase.hpp>
#include <XPath/XObjectFactory.hpp>



const XalanDOMString	FunctionGenerateID::s_emptyString;



FunctionGenerateID::FunctionGenerateID() :
	Function(),
	m_prefix(XALAN_STATIC_UCODE_STRING("N")),
	m_prefixLength(length(m_prefix))
{
}



FunctionGenerateID::~FunctionGenerateID()
{
}



// Append the suffix to the provided string.
void
getSuffix(
		const XalanNode*	theNode,
		XalanDOMString&		theResult)
{
	assert(theNode != 0);
	assert(theNode->getOwnerDocument() != 0);

	const unsigned long		theIndex = theNode->getIndex();
	const unsigned long		theNumber = theNode->getOwnerDocument()->getNumber();

	if (theNumber != 0)
	{
		UnsignedLongToHexDOMString(theNumber, theResult);

		append(theResult, XalanDOMChar(XalanUnicode::charFullStop));
	}

	if (theIndex == 0)
	{
		// We're assuming here that each nodes has an implementation with a 
		// unique address that we can convert into a string
#if defined(XALAN_OLD_STYLE_CASTS)
		UnsignedLongToHexDOMString((unsigned long)theNode, theResult);
#else
		UnsignedLongToHexDOMString(reinterpret_cast<unsigned long>(theNode), theResult);
#endif
	}
	else
	{
		UnsignedLongToHexDOMString(theIndex, theResult);
	}
}



XObjectPtr
FunctionGenerateID::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const Locator*			locator) const
{
	if (context == 0)
	{
		executionContext.error(
				"The generate-id() function requires a non-null context node!",
				context,
				locator);

		return XObjectPtr();
	}
	else
	{
		XPathExecutionContext::GetAndReleaseCachedString	theID(executionContext);

		theID.get() = m_prefix;

		getSuffix(context, theID.get());

		return executionContext.getXObjectFactory().createString(theID);
	}
}



XObjectPtr
FunctionGenerateID::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,
			const XObjectPtr		arg1,
			const Locator*			locator) const
{
	assert(arg1.null() == false);

	const NodeRefListBase&	theNodeList = arg1->nodeset();

	if (theNodeList.getLength() == 0)
	{
		return executionContext.getXObjectFactory().createString(s_emptyString);
	}
	else
	{
		return execute(executionContext, theNodeList.item(0), locator);
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionGenerateID*
#endif
FunctionGenerateID::clone() const
{
	return new FunctionGenerateID(*this);
}



const XalanDOMString
FunctionGenerateID::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The generate-id function takes zero or one arguments!"));
}
