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
#include "FunctionNormalizeSpace.hpp"
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



XPathFunctionTable::XPathFunctionTable(bool		fCreateTable) :
	m_FunctionCollection(),
	m_FunctionNameIndex()
{
	if (fCreateTable == true)
	{
		CreateTable();
	}
}



XPathFunctionTable::~XPathFunctionTable()
{
	DestroyTable();
}



void
XPathFunctionTable::InstallFunction(
			const XalanDOMString&	theFunctionName,
			const Function&			theFunction)
{
	assert(length(theFunctionName) != 0);

	// See if a function of that name is already installed...
	const FunctionNameIndexMapType::iterator	i =
		m_FunctionNameIndex.find(theFunctionName);

	if (i != m_FunctionNameIndex.end())
	{
		assert(CollectionType::size_type((*i).second) < m_FunctionCollection.size());

		// It is, so delete the old one, and add the new one...
#if defined(XALAN_CANNOT_DELETE_CONST)
		delete (Function*)m_FunctionCollection[(*i).second];
#else
		delete m_FunctionCollection[(*i).second];
#endif

		m_FunctionCollection[(*i).second] = theFunction.clone();
	}
	else
	{
		const CollectionType::size_type		theIndex = m_FunctionCollection.size();

		m_FunctionCollection.push_back(theFunction.clone());

		m_FunctionNameIndex[theFunctionName] = int(theIndex);
	}
}



bool
XPathFunctionTable::UninstallFunction(const XalanDOMString&		theFunctionName)
{
	assert(length(theFunctionName) != 0);

	// See if a function of that name is installed...
	const FunctionNameIndexMapType::iterator	i =
		m_FunctionNameIndex.find(theFunctionName);

	if (i == m_FunctionNameIndex.end())
	{
		return false;
	}
	else
	{
		assert(CollectionType::size_type((*i).second) < m_FunctionCollection.size());

#if !defined(XALAN_NO_NAMESPACES)
		using std::find;
#endif

		// Delete the function...
#if defined(XALAN_CANNOT_DELETE_CONST)
		delete (Function*)m_FunctionCollection[(*i).second];
#else
		delete m_FunctionCollection[(*i).second];
#endif

		// Set the entry in the table to 0...
		m_FunctionCollection[(*i).second] = 0;

		return true;
	}
}



void
XPathFunctionTable::CreateTable()
{
	try
	{
		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("last")),
						FunctionLast());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("position")),
						FunctionPosition());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("count")),
						FunctionCount());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("id")),
						FunctionID());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("local-name")),
						FunctionLocalName());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("namespace-uri")),
						FunctionNamespaceURI());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("name")),
						FunctionName());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("string")),
						FunctionString());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("concat")),
						FunctionConcat());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("starts-with")),
						FunctionStartsWith());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("contains")),
						FunctionContains());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("substring-before")),
						FunctionSubstringBefore());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("substring-after")),
						FunctionSubstringAfter());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("substring")),
						FunctionSubstring());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("string-length")),
						FunctionStringLength());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("normalize-space")),
						FunctionNormalizeSpace());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("translate")),
						FunctionTranslate());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("boolean")),
						FunctionBoolean());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("not")),
						FunctionNot());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("true")),
						FunctionTrue());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("false")),
						FunctionFalse());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("lang")),
						FunctionLang());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("number")),
						FunctionNumber());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("sum")),
						FunctionSum());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("floor")),
						FunctionFloor());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("ceiling")),
						FunctionCeiling());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("round")),
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

		CollectionType().swap(m_FunctionCollection);

		FunctionNameIndexMapType().swap(m_FunctionNameIndex);
	}
	catch(...)
	{
	}
}



XPathExceptionFunctionNotAvailable::XPathExceptionFunctionNotAvailable(int	theFunctionNumber) :
	XalanXPathException(TranscodeFromLocalCodePage("The specified function number is not available: ") + LongToDOMString(theFunctionNumber))
{
}



XPathExceptionFunctionNotAvailable::XPathExceptionFunctionNotAvailable(const XalanDOMString&	theFunctionName) :
	XalanXPathException(TranscodeFromLocalCodePage("The specified function is not available: ") + theFunctionName)
{
}



XPathExceptionFunctionNotAvailable::XPathExceptionFunctionNotAvailable(
			int				theFunctionNumber,
			const Locator&	theLocator) :
	XalanXPathException(
					theLocator,
					TranscodeFromLocalCodePage("The specified function number is not available: ") + LongToDOMString(theFunctionNumber))
{
}



XPathExceptionFunctionNotAvailable::XPathExceptionFunctionNotAvailable(
			const XalanDOMString&	theFunctionName,
			const Locator&			theLocator) :
	XalanXPathException(
					theLocator,
					TranscodeFromLocalCodePage("The specified function is not available: ") + theFunctionName)
{
}



XPathExceptionFunctionNotAvailable::~XPathExceptionFunctionNotAvailable()
{
}
