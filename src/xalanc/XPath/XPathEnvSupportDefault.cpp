/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "XPathEnvSupportDefault.hpp"



#include <algorithm>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XObject.hpp"
#include "XObjectFactory.hpp"
#include "XPath.hpp"
#include "XPathExecutionContext.hpp"



XALAN_USING_STD(cerr)
XALAN_USING_STD(endl)



XALAN_CPP_NAMESPACE_BEGIN



XPathEnvSupportDefault::NamespaceFunctionTablesType		XPathEnvSupportDefault::s_externalFunctions;



void
XPathEnvSupportDefault::initialize()
{
}



void
XPathEnvSupportDefault::terminate()
{
	XALAN_USING_STD(for_each)

	// Clean up the extension namespaces vector
	for_each(s_externalFunctions.begin(),
			 s_externalFunctions.end(),
			 NamespaceFunctionTableDeleteFunctor());

	NamespaceFunctionTablesType().swap(s_externalFunctions);
}



XPathEnvSupportDefault::XPathEnvSupportDefault() :
	XPathEnvSupport(),
	m_sourceDocs(),
	m_externalFunctions()
{
}



XPathEnvSupportDefault::~XPathEnvSupportDefault()
{
	XALAN_USING_STD(for_each)

	// Clean up the extension namespaces vector
	for_each(m_externalFunctions.begin(),
			 m_externalFunctions.end(),
			 NamespaceFunctionTableDeleteFunctor());

	NamespaceFunctionTablesType().swap(m_externalFunctions);
}



void
XPathEnvSupportDefault::updateFunctionTable(
			NamespaceFunctionTablesType&	theTable,
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			const Function*					function)
{
	// See if there's a table for that namespace...
	const NamespaceFunctionTablesType::iterator		i =
		theTable.find(theNamespace);

	if (i == theTable.end())
	{
		// The namespace was not found.  If function is not
		// 0, then add a clone of the function.
		if (function != 0)
		{
			theTable[theNamespace][functionName] =
				function->clone();
		}
	}
	else
	{
		// There is already a table for the namespace,
		// so look for the function...
		const FunctionTableType::iterator	j =
			(*i).second.find(functionName);

		if (j == (*i).second.end())
		{
			// The function was not found.  If function is not
			// 0, then add a clone of the function.
			if (function != 0)
			{
				(*i).second[functionName] = function->clone();
			}
		}
		else
		{
			// Found it, so delete the function...
#if defined(XALAN_CANNOT_DELETE_CONST)
			delete (Function*)(*j).second;
#else
			delete (*j).second;
#endif

			// If function is not 0, then we update
			// the entry.  Otherwise, we erase it...
			if (function != 0)
			{
				// Update it...
				(*j).second = function->clone();
			}
			else
			{
				// Erase it...
				(*i).second.erase(j);
			}
		}
	}
}



void
XPathEnvSupportDefault::installExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName,
			const Function&			function)
{
	updateFunctionTable(s_externalFunctions, theNamespace, functionName, &function);
}



void
XPathEnvSupportDefault::uninstallExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName)
{
	updateFunctionTable(s_externalFunctions, theNamespace, functionName, 0);
}



void
XPathEnvSupportDefault::installExternalFunctionLocal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName,
			const Function&			function)
{
	updateFunctionTable(m_externalFunctions, theNamespace, functionName, &function);
}



void
XPathEnvSupportDefault::uninstallExternalFunctionLocal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName)
{
	updateFunctionTable(m_externalFunctions, theNamespace, functionName, 0);
}



void
XPathEnvSupportDefault::reset()
{
	m_sourceDocs.clear();
}



XalanDocument*
XPathEnvSupportDefault::parseXML(
			const XalanDOMString&	/* urlString */,
			const XalanDOMString&	/* base */)
{
	return 0;
}



XalanDocument*
XPathEnvSupportDefault::getSourceDocument(const XalanDOMString&		theURI) const
{
	const SourceDocsTableType::const_iterator	i =
			m_sourceDocs.find(theURI);

	if (i == m_sourceDocs.end())
	{
		return 0;
	}
	else
	{
		return (*i).second;
	}
}



void
XPathEnvSupportDefault::setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument)
{
	m_sourceDocs[theURI] = theDocument;
}



XalanDOMString
XPathEnvSupportDefault::findURIFromDoc(const XalanDocument*		owner) const
{
	SourceDocsTableType::const_iterator	i =
			m_sourceDocs.begin();

	bool	fFound = false;

	while(i != m_sourceDocs.end() && fFound == false)
	{
		if ((*i).second == owner)
		{
			fFound = true;
		}
		else
		{
			++i;
		}
	}

	return fFound == false ? XalanDOMString() : (*i).first;
}



bool
XPathEnvSupportDefault::elementAvailable(
			const XalanDOMString&	/* theNamespace */,
			const XalanDOMString&	/* elementName */) const
{
	return false;
}



bool
XPathEnvSupportDefault::functionAvailable(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName) const
{
	bool	theResult = false;

	// Any function without a namespace prefix is considered
	// to be an intrinsic function.
	if (isEmpty(theNamespace) == true)
	{
		theResult = XPath::isInstalledFunction(functionName);
	}
	else
	{
		const Function* const	theFunction =
			findFunction(
				theNamespace,
				functionName);

		if (theFunction != 0)
		{
			theResult = true;
		}
	}

	return theResult;
}



const Function*
XPathEnvSupportDefault::findFunction(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName) const
{
	// First, look locally...
	const Function*		theFunction = findFunction(
			m_externalFunctions,
			theNamespace,
			functionName);

	if (theFunction == 0)
	{
		// Not found, so look in the global space...
		theFunction = findFunction(
			s_externalFunctions,
			theNamespace,
			functionName);
	}

	return theFunction;
}



const Function*
XPathEnvSupportDefault::findFunction(
			const NamespaceFunctionTablesType&	theTable,
			const XalanDOMString&				theNamespace,
			const XalanDOMString&				functionName) const
{
	const Function*	theFunction = 0;

	// See if there's a table for that namespace...
	const NamespaceFunctionTablesType::const_iterator	i =
		theTable.find(theNamespace);

	if (i != theTable.end())
	{
		// There is a table for the namespace,
		// so look for the function...
		const FunctionTableType::const_iterator		j =
			(*i).second.find(functionName);

		if (j != (*i).second.end())
		{
			// Found the function...
			assert((*j).second != 0);

			theFunction = (*j).second;
		}
	}

	return theFunction;
}



XObjectPtr
XPathEnvSupportDefault::extFunction(
			XPathExecutionContext&			executionContext,
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec,
			const LocatorType*				locator) const
{
	const Function* const	theFunction = findFunction(theNamespace, functionName);

	if (theFunction != 0)
	{
		return theFunction->execute(
					executionContext,
					context,
					argVec,
					locator);
	}
	else
	{
		XalanDOMString	theFunctionName;

		if(length(theNamespace) > 0)
		{
			theFunctionName += theNamespace;
			theFunctionName += DOMServices::s_XMLNamespaceSeparatorString;
		}

		theFunctionName += functionName;

		if (locator != 0)
		{
			throw XPathExceptionFunctionNotAvailable(theFunctionName, *locator);
		}
		else
		{
			throw XPathExceptionFunctionNotAvailable(theFunctionName);
		}

		// dummy return value...
		return XObjectPtr();
	}
}



bool
XPathEnvSupportDefault::problem(
			eSource					/* where */,
/**
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
			eClassification			classification,
			const PrefixResolver*	/* resolver */,
			const XalanNode*		/* sourceNode */,
			const XalanDOMString&	msg,
			const XalanDOMChar*		uri,
			int						lineNo,
			int						charOffset) const
{
	cerr << msg;

	if (uri != 0)
	{
		cerr << ",in " << uri;
	}

	cerr << ", at line number "
		 << lineNo
		 << " at offset "
		 << charOffset
		 << endl;

	return classification == XPathEnvSupport::eError ? true : false;
}



void
XPathEnvSupportDefault::NamespaceFunctionTableDeleteFunctor::operator()(const NamespaceFunctionTablesInnerType::value_type&	thePair) const
{
#if defined(XALAN_CANNOT_DELETE_CONST)
	FunctionTableInnerType::const_iterator	i = thePair.second.begin();

	while(i != thePair.second.end())
	{
		delete (Function*) (*i).second;

		++i;
	}
#else
	XALAN_USING_STD(for_each)

	// Clean up the extension namespaces vector
	for_each(thePair.second.begin(),
			 thePair.second.end(),
			 MapValueDeleteFunctor<FunctionTableInnerType>());
#endif
}



XALAN_CPP_NAMESPACE_END
