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
 */
#if !defined(XALAN_NODESORTER_HEADER_GUARD)
#define XALAN_NODESORTER_HEADER_GUARD

/**
 * @author David N. Bertoni (david_n_bertoni@lotus.com)
 */

 

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <functional>
#include <vector>



#include <xalanc/XPath/XObject.hpp>



#include <xalanc/XSLT/NodeSortKey.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class MutableNodeRefList;
class StylesheetExecutionContext;
class XalanNode;
class XPath;

#define XALAN_NODESORTER_CACHE_XOBJECTS

/**
 * This class can sort vectors of nodes according to a select pattern.
 */
class XALAN_XSLT_EXPORT NodeSorter
{
public:

	struct XALAN_XSLT_EXPORT VectorEntry
	{
	public:

		VectorEntry(
			XalanNode*		theNode = 0,
			unsigned int	thePosition = 0) :
			m_node(theNode),
			m_position(thePosition)
		{
		}

		XalanNode*		m_node;
		unsigned int	m_position;
	};

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<VectorEntry>			NodeVectorType;
	typedef vector<NodeSortKey>			NodeSortKeyVectorType;
#else
	typedef std::vector<VectorEntry>	NodeVectorType;
	typedef std::vector<NodeSortKey>	NodeSortKeyVectorType;
#endif

	explicit
	NodeSorter();
  
	~NodeSorter();

	NodeSortKeyVectorType&
	getSortKeys()
	{
		return m_keys;
	}

	/**
	 * Given a list of nodes, sort each node according to the criteria in the
	 * keys.  The list is assumed to be in document order.
	 *
	 * @param executionContext current execution context
	 * @param v    list of Nodes
	 */
	void
	sort(
			StylesheetExecutionContext&		executionContext,
			MutableNodeRefList&				theList);

	/**
	 * Return the results of a compare of two nodes.
	 */
#if defined(XALAN_NO_STD_NAMESPACE)
	struct XALAN_XSLT_EXPORT NodeSortKeyCompare : public binary_function<const NodeVectorType::value_type&, const NodeVectorType::value_type&, bool>
#else
	struct XALAN_XSLT_EXPORT NodeSortKeyCompare : public std::binary_function<const NodeVectorType::value_type&, const NodeVectorType::value_type&, bool>
#endif
	{
	public:

	/**
	 * Construct a NodeSortKeyCompare object, to perform the sort
	 *
	 * @param executionContext current execution context
	 * @param theNodes        vector or nodes to be sorted
	 * @param theNodeSortKeys vector of keys upon which to sort
	 */
		NodeSortKeyCompare(
				StylesheetExecutionContext&		executionContext,
				NodeSorter&						theSorter,
				const NodeVectorType&			theNodes,
				const NodeSortKeyVectorType&	theNodeSortKeys) :
			m_executionContext(executionContext),
			m_sorter(theSorter),
			m_nodes(theNodes),
			m_nodeSortKeys(theNodeSortKeys)
		{
		}

		/**
		 * Compare two nodes, returning a value to indicate the
		 * result
		 *
		 * @param theLHS the first node to compare
		 * @param theRHS the second node to compare
		 * @param theKeyIndex the index of the key to use
		 * @result < 0 if theLHS is less than theRHS, 0 if they are equal, and > 0 if theLHS is greater than theRHS
		 */
		int
		compare(
				first_argument_type		theLHS,
				second_argument_type	theRHS,
				unsigned int			theKeyIndex = 0) const;

		/**
		 * Compare two nodes as a less predicate.
		 *
		 * @param theLHS the first node to compare
		 * @param theRHS the second node to compare
		 * @param theKeyIndex the index of the key to use
		 * @return true if theLHS is less than theRHS
		 */
		result_type
		operator()(
				first_argument_type		theLHS,
				second_argument_type	theRHS,
				unsigned int			theKeyIndex = 0) const
		{
			return compare(theLHS, theRHS, theKeyIndex) < 0 ? true : false;
		}

	protected:

		double
		getNumberResult(
				const NodeSortKey&		theKey,
				unsigned int			theKeyIndex,
				first_argument_type		theEntry) const;

		const XalanDOMString&
		getStringResult(
				const NodeSortKey&		theKey,
				unsigned int			theKeyIndex,
				first_argument_type		theEntry) const;

	private:

		StylesheetExecutionContext&		m_executionContext;
		NodeSorter&						m_sorter;
		const NodeVectorType&			m_nodes;
		const NodeSortKeyVectorType&	m_nodeSortKeys;
	};

	friend struct NodeSortKeyCompare;

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef	vector<double>			NumberVectorType;
	typedef	vector<XObjectPtr>		XObjectVectorType;
	typedef	vector<XalanDOMString>	StringVectorType;

	typedef vector<NumberVectorType>	NumberCacheType;
	typedef vector<XObjectVectorType>	XObjectCacheType;
	typedef vector<StringVectorType>	StringCacheType;
#else
	typedef	std::vector<double>			NumberVectorType;
	typedef	std::vector<XObjectPtr>		XObjectVectorType;
	typedef	std::vector<XalanDOMString>	StringVectorType;

	typedef std::vector<NumberVectorType>	NumberCacheType;
	typedef std::vector<XObjectVectorType>	XObjectCacheType;
	typedef std::vector<StringVectorType>	StringCacheType;
#endif

	typedef NumberCacheType		NumberResultsCacheType;

#if defined(XALAN_NODESORTER_CACHE_XOBJECTS)
	typedef XObjectCacheType	StringResultsCacheType;
#else
	typedef StringCacheType		StringResultsCacheType;
#endif

private:

	/**
	 * Given a vector of nodes, sort each node according to the criteria in the
	 * keys.
	 *
	 * @param executionContext current execution context
	 */
	void
	sort(StylesheetExecutionContext&	executionContext);

	// Data members...
	NumberResultsCacheType	m_numberResultsCache;

	StringResultsCacheType	m_stringResultsCache;

	NodeSortKeyVectorType	m_keys;

	NodeVectorType			m_scratchVector;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_NODESORTER_HEADER_GUARD
