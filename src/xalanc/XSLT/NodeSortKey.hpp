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

#if !defined(XALAN_NODESORTKEY_HEADER_GUARD)
#define XALAN_NODESORTKEY_HEADER_GUARD

/**
 * 
 * @author Scott Boag (scott_boag@lotus.com)
 * @author David N. Bertoni (david_n_bertoni@lotus.com)
 */



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



// $$$ ToDo: This is necessary while XalanDOMString is a typedef.
#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/XalanCollationServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class ExecutionContext;
class PrefixResolver;
class XPath;



/**
 * Simple data structure class for use by the NodeSorter class.
 */
class XALAN_XSLT_EXPORT NodeSortKey
{
public:

	/**
	 * Construct a node sort key.
	 *
	 * @param executionContext current execution context
	 * @param selectPat        XPath for selection
	 * @param treatAsNumbers   treat as numeric values if true
	 * @param descending       sort in descending order if true
	 * @param caseOrder        case sort order enum
	 * @param langValue        language
	 * @param resolver         resolver for namespace resolution
	 */
	NodeSortKey(
			ExecutionContext&					executionContext,
			const XPath*						selectPat,
			bool								treatAsNumbers, 
			bool								descending,
			XalanCollationServices::eCaseOrder	caseOrder,
			const XalanDOMString&				langValue, 
			const PrefixResolver&				resolver);

	NodeSortKey();

	NodeSortKey(const NodeSortKey&	theSource);

	~NodeSortKey();

	NodeSortKey&
	operator=(const NodeSortKey&	theRHS);

	/**
	 * Retrieve the selection XPath.
	 * 
	 * @return XPath for selection
	 */
	const XPath*
	getSelectPattern() const
	{
		return m_selectPat;
	}

	/**
	 * Whether to treat values as numbers.
	 * 
	 * @return treat as numeric values if true
	 */
	bool
	getTreatAsNumbers() const
	{
		return m_treatAsNumbers;
	}

	/**
	 * Whether to sort in descending order.
	 * 
	 * @return sort in descending order if true
	 */
	bool
	getDescending() const
	{
		return m_descending;
	}

	/**
	 * Get the enumeration value for the case order.
	 * 
	 * @return sort upper case before lower case if true
	 */
	XalanCollationServices::eCaseOrder
	getCaseOrder() const
	{
		return m_caseOrder;
	}

	/**
	 * Retrieve the resolver for namespaces.
	 * 
	 * @return object for namespace resolution
	 */
	const PrefixResolver*
	getPrefixResolver() const
	{
		return m_prefixResolver;
	}

	const XalanDOMString&
	getLanguageString() const
	{
		return *m_languageString;
	}

private:

	ExecutionContext*					m_executionContext; // needed for error reporting.

	const XPath*						m_selectPat;

	bool								m_treatAsNumbers;
	bool								m_descending;

	XalanCollationServices::eCaseOrder	m_caseOrder;	

	const PrefixResolver*				m_prefixResolver;

	const XalanDOMString*				m_languageString;
};



XALAN_CPP_NAMESPACE_END



#endif
