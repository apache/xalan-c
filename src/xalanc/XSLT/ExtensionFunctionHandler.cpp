/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
 *
 * $ Id: $
 *
 */

#include "ExtensionFunctionHandler.hpp"



#include <cassert>



#include <xalanc/PlatformSupport/StringTokenizer.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar	ExtensionFunctionHandler::s_tokenDelimiterCharacters[] =
{
		XalanUnicode::charSpace,
		XalanUnicode::charHTab,
		XalanUnicode::charLF,
		XalanUnicode::charCR,
		0
};



ExtensionFunctionHandler::ExtensionFunctionHandler(const XalanDOMString&	namespaceUri) :
	m_namespaceUri(namespaceUri),
	m_scriptLang(),
	m_scriptSrc(),
	m_scriptSrcURL(),
	m_javaObject(0),
	m_functions(),
	m_componentStarted(false)
{
}



ExtensionFunctionHandler::~ExtensionFunctionHandler()
{
}



ExtensionFunctionHandler::ExtensionFunctionHandler (
			const XalanDOMString&	namespaceUri,
			const XalanDOMString&	funcNames,
			const XalanDOMString&	lang,
			const XalanDOMString&	srcURL,
			const XalanDOMString&	src) :
	m_namespaceUri(namespaceUri),
	m_scriptLang(lang),
	m_scriptSrc(src),
	m_scriptSrcURL(srcURL),
	m_javaObject(0),
	m_functions(),
	m_componentStarted(false)
{
	setFunctions (funcNames);
}



void
ExtensionFunctionHandler::setFunctions(const XalanDOMString&	funcNames) 
{
	if (isEmpty(funcNames)) 
	{
		return;
	}

	StringTokenizer		st(funcNames, s_tokenDelimiterCharacters, false);

	while (st.hasMoreTokens() == true)
	{
		m_functions.insert(st.nextToken());
	}
}


void
ExtensionFunctionHandler::setScript(
			const XalanDOMString&	lang,
			const XalanDOMString&	srcURL,
			const XalanDOMString&	scriptSrc)
{
	m_scriptLang = lang;
	m_scriptSrcURL = srcURL;
	m_scriptSrc = scriptSrc;
}



bool
ExtensionFunctionHandler::isFunctionAvailable (const XalanDOMString&	function) const
{
	return m_functions.find(function) != m_functions.end();
}



XObjectPtr
ExtensionFunctionHandler::callFunction(
			const XalanDOMString&	/* funcName */,
			const ArgVectorType&	/* args */)
{
	assert(0);	// @@ TODO: Not implemented

	return XObjectPtr();
}



void
ExtensionFunctionHandler::startupComponent()
{
}



XALAN_CPP_NAMESPACE_END
