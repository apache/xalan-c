/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 200-1999-2003 The Apache Software Foundation.  All rights 
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



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/MutableNodeRefList.hpp>
#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XalanQName.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;



FunctionSystemProperty::FunctionSystemProperty() :
	Function(),
	m_xsltNamespaceURI(XALAN_STATIC_UCODE_STRING("http://www.w3.org/1999/XSL/Transform")),
	m_versionPropertyString(XALAN_STATIC_UCODE_STRING("version")),
	m_vendorPropertyString(XALAN_STATIC_UCODE_STRING("vendor")),
	m_vendorURLPropertyString(XALAN_STATIC_UCODE_STRING("vendor-url")),
	m_vendorString(XALAN_STATIC_UCODE_STRING("Apache Software Foundation")),
	m_vendorURLString(XALAN_STATIC_UCODE_STRING("http://xml.apache.org/xalan-c"))
{
}



FunctionSystemProperty::~FunctionSystemProperty()
{
}



inline void
validateNCName(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const LocatorType*		locator,
			const XalanDOMString&	ncname)
{
	if (XalanQName::isValidNCName(ncname) == false)
	{
		executionContext.error(
			"system-property(): The property is not a valid QName",
			context,
			locator);
	}
}



XObjectPtr
FunctionSystemProperty::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const LocatorType*		locator) const
{
	assert(arg1.null() == false);

	const XalanDOMString&				fullName = arg1->str();
	const XalanDOMString::size_type		fullNameLength = length(fullName);
	const XalanDOMString::size_type		indexOfNSSep = indexOf(fullName, XalanUnicode::charColon);

	if(indexOfNSSep < fullNameLength)
	{
		XPathExecutionContext::GetAndReleaseCachedString	guard(executionContext);

		XalanDOMString&		theBuffer = guard.get();

		substring(fullName, theBuffer, 0, indexOfNSSep);

		validateNCName(executionContext, context, locator, theBuffer);

		const XalanDOMString* const		nspace = executionContext.getNamespaceForPrefix(theBuffer);

		if (nspace == 0)
		{
			executionContext.error(
					XalanMessageLoader::getMessage(XalanMessages::UndeclaredNamespacePrefix_1Param, theBuffer),
					context,
					locator);
		}
		else
		{
			if(*nspace == m_xsltNamespaceURI)
			{
				substring(fullName, theBuffer, indexOfNSSep + 1);

				validateNCName(executionContext, context, locator, theBuffer);

				if(equals(theBuffer, m_versionPropertyString))
				{
					return executionContext.getXObjectFactory().createNumber(1.0);
				}
				else if(equals(theBuffer, m_vendorPropertyString))
				{
					return executionContext.getXObjectFactory().createStringReference(m_vendorString);
				}
				else if(equals(theBuffer, m_vendorURLPropertyString))
				{
					return executionContext.getXObjectFactory().createStringReference(m_vendorURLString);
				}
				else
				{
					return executionContext.getXObjectFactory().createStringReference(s_emptyString);
				}
			}
		}
	}
	else
	{
		validateNCName(executionContext, context, locator, fullName);

		const char* const	theEnvString =
#if defined(XALAN_STRICT_ANSI_HEADERS)
			std::getenv(c_str(TranscodeToLocalCodePage(fullName)));
#else
			getenv(c_str(TranscodeToLocalCodePage(fullName)));
#endif

		if (theEnvString != 0)
		{
			XPathExecutionContext::GetAndReleaseCachedString	guard(executionContext);

			XalanDOMString&		result = guard.get();

			result = TranscodeFromLocalCodePage(theEnvString);

			return executionContext.getXObjectFactory().createString(result);
		}
	}

	return executionContext.getXObjectFactory().createStringReference(s_emptyString);
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
	return XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"system-property()");
}



XALAN_CPP_NAMESPACE_END
