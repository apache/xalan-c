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
#if !defined(XALAN_COUNTERSTABLE_HEADER_GUARD_1357924680)
#define XALAN_COUNTERSTABLE_HEADER_GUARD_1357924680

/**
 * $Id$
 * 
 * $State$
 * 
 */

// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <vector>



#include "xalanc/XPath/MutableNodeRefList.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class ElemNumber;
class StylesheetExecutionContext;



/**
 * <meta name="usage" content="internal"/>
 * A class that does incremental counting for support of xsl:number.
 * This class stores a cache of counted nodes (m_countNodes). 
 * It tries to cache the counted nodes in document order... 
 * the node count is based on its position in the cache list 
 */
struct Counter
{
	typedef unsigned long	CountType;

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<XalanNode*>			NodeVectorType;
#else
	typedef std::vector<XalanNode*>		NodeVectorType;
#endif


	/**
	 * The start count from where m_countNodes counts 
	 * from.  In other words, the count of a given node 
	 * in the m_countNodes vector is node position + 
	 * m_countNodesStartCount.
	 */
	CountType			m_countNodesStartCount;

	/**
	 * A vector of all nodes counted so far.
	 */
	NodeVectorType		m_countNodes;

	/**
	 * The node from where the counting starts.  This is needed to 
	 * find a counter if the node being counted is not immediatly
	 * found in the m_countNodes vector.
	 */
	const XalanNode*	m_fromNode;

	/**
	 * The owning xsl:number element.
	 */
	const ElemNumber*	m_numberElem;

	/**
	 * Construct a counter object.
	 */
	Counter(
			const ElemNumber*	numberElem,
			NodeVectorType&		countNodes) :
		m_countNodesStartCount(0),
		m_countNodes(countNodes),
		m_fromNode(0),
		m_numberElem(numberElem)
	{
	}

	/**
	 * Construct a counter object.
	 */
	Counter(const ElemNumber*	numberElem = 0) :
		m_countNodesStartCount(0),
		m_countNodes(),
		m_fromNode(0),
		m_numberElem(numberElem)
	{
	}

	/**
	 * Try to find a node that was previously counted. If found, return a
	 * positive integer that corresponds to the count.
	 * @param node The node to be counted.
	 * @returns The count of the node, or 0 if not found.
	 */
	CountType
	getPreviouslyCounted(
			StylesheetExecutionContext&		support,
			const XalanNode*				node) const;

	/**
	 * Get the last node in the list.
	 */
	XalanNode*
	getLast() const
	{
		return m_countNodes.empty() == true ? 0 : m_countNodes.back();
	}
};



/**
 * <meta name="usage" content="internal"/>
 * This is a table of counters, keyed by ElemNumber objects, each 
 * of which has a list of Counter objects.  This really isn't a true 
 * table, it is more like a list of lists (there must be a technical 
 * term for that...).
 */
class CountersTable
{
public:

	typedef Counter::CountType	CountType;

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<Counter>					CounterVectorType;
	typedef vector<CounterVectorType>		ElemCounterVectorVectorType;
#else
	typedef std::vector<Counter>			CounterVectorType;
	typedef std::vector<CounterVectorType>	ElemCounterVectorVectorType;
#endif

	typedef Counter::NodeVectorType			NodeVectorType;

	/**
	 * Construct a CountersTable.
	 */
	CountersTable(unsigned long		theSize = 0) :
		m_countersVector(),
		m_newFound()
	{
		resize(theSize);
	};


	/**
	 * Resize the table.  The must be done prior
	 * to using the table, if the size was not past
	 * in the constructor.
	 *
	 * @theSize The new size
	 */
	void
	resize(unsigned long	theSize)
	{
		m_countersVector.resize(theSize);
	}

	/**
	 * Count forward until the given node is found, or until 
	 * we have looked to the given amount.
	 *
	 * @executionContext The current execution context;
	 * @numberElem The executing ElemNumber
	 * @node The node to count.
	 * @return The node count, or 0 if not found.
	 */
	CountType
	countNode(
			StylesheetExecutionContext&		executionContext,
			const ElemNumber&				numberElem,
			XalanNode*						node);

	/**
	 * Clear all cached data from the table.
	 */
	void
	reset()
	{
		m_newFound.clear();

		m_countersVector.clear();
	}

private:

	/**
	 * A vector which holds counters for ElemNumber instances.
	 */
	ElemCounterVectorVectorType		m_countersVector;


	/**
	 * A vector to use as a temporary buffer.
	 */
	NodeVectorType					m_newFound;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALAN_COUNTERSTABLE_HEADER_GUARD_1357924680)
