/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
