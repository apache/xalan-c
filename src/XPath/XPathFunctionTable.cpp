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
// Base class header file
#include "XPathFunctionTable.hpp"



#include <PlatformSupport/DOMStringHelper.hpp>



#include "FunctionBoolean.hpp"
#include "FunctionCeiling.hpp"
#include "FunctionConcat.hpp"
#include "FunctionContains.hpp"
#include "FunctionCount.hpp"
#include "FunctionFalse.hpp"
#include "FunctionFloor.hpp"
#include "FunctionID.hpp"
#include "FunctionLang.hpp"
#include "FunctionLast.hpp"
#include "FunctionLocalName.hpp"
#include "FunctionName.hpp"
#include "FunctionNamespaceURI.hpp"
#include "FunctionNormalize.hpp"
#include "FunctionNot.hpp"
#include "FunctionNumber.hpp"
#include "FunctionPosition.hpp"
#include "FunctionRound.hpp"
#include "FunctionStartsWith.hpp"
#include "FunctionString.hpp"
#include "FunctionStringLength.hpp"
#include "FunctionSubstring.hpp"
#include "FunctionSubstringAfter.hpp"
#include "FunctionSubstringBefore.hpp"
#include "FunctionSum.hpp"
#include "FunctionTranslate.hpp"
#include "FunctionTrue.hpp"



XPathFunctionTable::XPathFunctionTable() :
	m_FunctionCollection()
{
	CreateTable();
}



XPathFunctionTable::~XPathFunctionTable()
{
	DestroyTable();
}


void
XPathFunctionTable::InstallFunction(
			const XalanDOMString&	theFunctionName,
			const Function&		theFunction)
{
	assert(length(theFunctionName) != 0);

	// See if a function of that name is already installed...
	const CollectionType::iterator	i =
		m_FunctionCollection.find(theFunctionName);

	if (i != m_FunctionCollection.end())
	{
		// It is, so delete the old one, and add the new one...
		delete i->second;

		i->second = theFunction.clone();
	}
	else
	{
		// It's not, so clone the function and add it to the collection.
		m_FunctionCollection[theFunctionName] = theFunction.clone();
	}
}



void
XPathFunctionTable::CreateTable()
{
	try
	{
		InstallFunction(XALAN_STATIC_UCODE_STRING("last"),
						FunctionLast());

		InstallFunction(XALAN_STATIC_UCODE_STRING("position"),
						FunctionPosition());

		InstallFunction(XALAN_STATIC_UCODE_STRING("count"),
						FunctionCount());

		InstallFunction(XALAN_STATIC_UCODE_STRING("id"),
						FunctionID());

		InstallFunction(XALAN_STATIC_UCODE_STRING("local-name"),
						FunctionLocalName());

		InstallFunction(XALAN_STATIC_UCODE_STRING("namespace-uri"),
						FunctionNamespaceURI());

		InstallFunction(XALAN_STATIC_UCODE_STRING("name"),
						FunctionName());

		InstallFunction(XALAN_STATIC_UCODE_STRING("string"),
						FunctionString());

		InstallFunction(XALAN_STATIC_UCODE_STRING("concat"),
						FunctionConcat());

		InstallFunction(XALAN_STATIC_UCODE_STRING("starts-with"),
						FunctionStartsWith());

		InstallFunction(XALAN_STATIC_UCODE_STRING("contains"),
						FunctionContains());

		InstallFunction(XALAN_STATIC_UCODE_STRING("substring-before"),
						FunctionSubstringBefore());

		InstallFunction(XALAN_STATIC_UCODE_STRING("substring-after"),
						FunctionSubstringAfter());

		InstallFunction(XALAN_STATIC_UCODE_STRING("substring"),
						FunctionSubstring());

		InstallFunction(XALAN_STATIC_UCODE_STRING("string-length"),
						FunctionStringLength());

		InstallFunction(XALAN_STATIC_UCODE_STRING("normalize-space"),
						FunctionNormalizeSpace());

		InstallFunction(XALAN_STATIC_UCODE_STRING("translate"),
						FunctionTranslate());

		InstallFunction(XALAN_STATIC_UCODE_STRING("boolean"),
						FunctionBoolean());

		InstallFunction(XALAN_STATIC_UCODE_STRING("not"),
						FunctionNot());

		InstallFunction(XALAN_STATIC_UCODE_STRING("true"),
						FunctionTrue());

		InstallFunction(XALAN_STATIC_UCODE_STRING("false"),
						FunctionFalse());

		InstallFunction(XALAN_STATIC_UCODE_STRING("lang"),
						FunctionLang());

		InstallFunction(XALAN_STATIC_UCODE_STRING("number"),
						FunctionNumber());

		InstallFunction(XALAN_STATIC_UCODE_STRING("sum"),
						FunctionSum());

		InstallFunction(XALAN_STATIC_UCODE_STRING("floor"),
						FunctionFloor());

		InstallFunction(XALAN_STATIC_UCODE_STRING("ceiling"),
						FunctionCeiling());

		InstallFunction(XALAN_STATIC_UCODE_STRING("round"),
						FunctionRound());

	}
	catch(...)
	{
		DestroyTable();

		throw;
	}
}



void
XPathFunctionTable::DestroyTable()
{
	try
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::for_each;
#endif

		for_each(m_FunctionCollection.begin(),
				 m_FunctionCollection.end(),
				 DeleteFunctorType());
	}
	catch(...)
	{
	}
}



XPathFunctionTableException::XPathFunctionTableException(const XalanDOMString&	theMessage) :
		XPathException(theMessage)
{
}



XPathFunctionTableException::~XPathFunctionTableException()
{
}



XPathFunctionTableInvalidFunctionException::XPathFunctionTableInvalidFunctionException(const XalanDOMString&		theFunctionName) :
	XPathFunctionTableException(FormatMessage(theFunctionName))
{
}



XPathFunctionTableInvalidFunctionException::~XPathFunctionTableInvalidFunctionException()
{
}



XalanDOMString
XPathFunctionTableInvalidFunctionException::FormatMessage(const XalanDOMString&	theFunctionName)
{
	const char*		theMessage =
				"Invalid function name detected: ";

	return XalanDOMString(theMessage) + theFunctionName;
}
