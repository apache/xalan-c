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
#include "FunctionSystemProperty.hpp"



#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanDocument.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/MutableNodeRefList.hpp>
#include <XPath/NodeRefListBase.hpp>
#include <XPath/XObjectFactory.hpp>



FunctionSystemProperty::FunctionSystemProperty() :
	Function()
{
}



FunctionSystemProperty::~FunctionSystemProperty()
{
}



XObjectPtr
FunctionSystemProperty::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		arg1,
			const Locator*			locator) const
{
	assert(arg1.null() == false);

	const XalanDOMString&				fullName = arg1->str();
	const XalanDOMString::size_type		fullNameLength = length(fullName);
	const XalanDOMString::size_type		indexOfNSSep = indexOf(fullName, XalanUnicode::charColon);

	bool			fNumberResult = false;

	XalanDOMString	result;

	double			numberResult = 0.0;

	if(indexOfNSSep < fullNameLength)
	{
		const XalanDOMString	prefix = substring(fullName, 0, indexOfNSSep);

		const XalanDOMString* const		nspace = executionContext.getNamespaceForPrefix(prefix);

		if (nspace != 0)
		{
			const XalanDOMString	propName = substring(fullName, indexOfNSSep + 1);

			if(startsWith(*nspace, XALAN_STATIC_UCODE_STRING("http://www.w3.org/1999/XSL/Transform")))
			{
				if(equals(propName, XALAN_STATIC_UCODE_STRING("version")))
				{
					numberResult = 1.0;

					fNumberResult = true;
				}
				else if(equals(propName, XALAN_STATIC_UCODE_STRING("vendor")))
				{
					result = XALAN_STATIC_UCODE_STRING("Apache Software Foundation");
				}
				else if(equals(propName, XALAN_STATIC_UCODE_STRING("vendor-url")))
				{
					result = XALAN_STATIC_UCODE_STRING("http://xml.apache.org/xalan-c");
				}
				else
				{
					executionContext.warn("XSL Property not supported: " + fullName);
				}
			}
			else
			{
				executionContext.warn(
					"Don't currently do anything with namespace " + *nspace + " in property: " + fullName,
					context,
					locator);

				result = TranscodeFromLocalCodePage(::getenv(c_str(TranscodeToLocalCodePage(propName))));
			}
		}
	}
	else
	{
		result = TranscodeFromLocalCodePage(::getenv(c_str(TranscodeToLocalCodePage(fullName))));
	}

	if (fNumberResult == true)
	{
		return executionContext.getXObjectFactory().createNumber(numberResult);
	}
	else
	{
		return executionContext.getXObjectFactory().createString(result);
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionSystemProperty*
#endif
FunctionSystemProperty::clone() const
{
	return new FunctionSystemProperty(*this);
}



const XalanDOMString
FunctionSystemProperty::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The system-property() function accepts one argument!"));
}
