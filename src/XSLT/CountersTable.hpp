/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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
  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
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
#if !defined(XALAN_COUNTERSTABLE_HEADER_GUARD_1357924680)
#define XALAN_ELEMNUMBER_HEADER_GUARD_1357924680 

/**
 * $Id$
 * 
 * $State$
 * 
 */

// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <map>
#include <vector>



#include "XPath/MutableNodeRefList.hpp"



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
#if defined(XALAN_NO_NAMESPACES)
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
	int					m_countNodesStartCount;

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
	 * @returns The count of the node, or -1 if not found.
	 */
	int
	getPreviouslyCounted(
			StylesheetExecutionContext&		support,
			const XalanNode*				node) const;

	/**
	 * Get the last node in the list.
	 */
	XalanNode*
	getLast() const
	{
		return m_countNodes.size() == 0 ? 0 : m_countNodes.back();
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

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<Counter>					CounterVectorType;
	typedef map<const ElemNumber*,
				CounterVectorType,
				less<const ElemNumber*> >	ElemToCounterVectorMapType;
#else
	typedef std::vector<Counter>			CounterVectorType;
	typedef std::map<const ElemNumber*,
					 CounterVectorType>		ElemToCounterVectorMapType;
#endif

	typedef Counter::NodeVectorType			NodeVectorType;

	/**
	 * Construct a CountersTable.
	 */
	CountersTable() :
		m_counterMap(),
		m_newFound()
	{
	};


	/**
	 * Count forward until the given node is found, or until 
	 * we have looked to the given amount.
	 *
	 * @executionContext The current execution context;
	 * @numberElem The executing ElemNumber
	 * @node The node to count.
	 * @return The node count, or 0 if not found.
	 */
	int
	countNode(
			StylesheetExecutionContext&		executionContext,
			const ElemNumber*				numberElem,
			XalanNode*						node);

	/**
	 * Clear all cached data from the table.
	 */
	void
	reset()
	{
		m_newFound.clear();

		m_counterMap.clear();
	}

private:

	/**
	 * Add a list of counted nodes that were built in backwards document 
	 * order, or a list of counted nodes that are in forwards document 
	 * order.
	 */
	void
	appendBtoFList(
			NodeVectorType&		flist,
			NodeVectorType&		blist);

	/**
	 * A map which holds counters for ElemNumber instances.
	 */
	ElemToCounterVectorMapType	m_counterMap;


	/**
	 * A vector to use as a temporary buffer.
	 */
	NodeVectorType				m_newFound;
};


#endif	// !defined(XALAN_COUNTERSTABLE_HEADER_GUARD_1357924680)
