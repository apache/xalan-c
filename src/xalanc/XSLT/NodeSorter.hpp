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
#if !defined(XALAN_NODESORTER_HEADER_GUARD)
#define XALAN_NODESORTER_HEADER_GUARD

/**
 * @author David N. Bertoni (david_n_bertoni@lotus.com)
 */

 

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <functional>



#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/XPath/XObject.hpp>



#include <xalanc/XSLT/NodeSortKey.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class MutableNodeRefList;
class StylesheetExecutionContext;
class XalanNode;
class XPath;


typedef	XalanVector<double>			NumberVectorTypeDecl;
XALAN_USES_MEMORY_MANAGER(NumberVectorTypeDecl)

typedef	XalanVector<XalanDOMString>	StringVectorTypeDecl;
XALAN_USES_MEMORY_MANAGER(StringVectorTypeDecl)


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

	typedef XalanVector<VectorEntry>			NodeVectorType;
	typedef XalanVector<NodeSortKey>			NodeSortKeyVectorType;

	explicit
	NodeSorter(MemoryManagerType& theManager);
  
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

	typedef	NumberVectorTypeDecl		NumberVectorType;
	typedef	XalanVector<XObjectPtr>		XObjectVectorType;
	typedef	StringVectorTypeDecl    	StringVectorType;

	typedef XalanVector<NumberVectorType>	NumberCacheType;
	typedef XalanVector<XObjectVectorType>	XObjectCacheType;
	typedef XalanVector<StringVectorType>	StringCacheType;

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
