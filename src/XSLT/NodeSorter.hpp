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
#if !defined(XALAN_NODESORTER_HEADER_GUARD)
#define XALAN_NODESORTER_HEADER_GUARD

/**
 * @author Scott Boag (scott_boag@lotus.com)
 * @author David N. Bertoni (david_n_bertoni@lotus.com)
 */

 

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <functional>
#include <map>
#include <vector>



#include "NodeSortKey.hpp"



class MutableNodeRefList;
class StylesheetExecutionContext;
class XalanNode;
class XPath;



/**
 * This class can sort vectors of DOM nodes according to a select pattern.
 */
 // TODO: Optimize this so it can reuse queries for each of the nodes.
class NodeSorter
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanNode*>			NodeVectorType;
	typedef vector<NodeSortKey>			NodeSortKeyVectorType;
#else
	typedef std::vector<XalanNode*>		NodeVectorType;
	typedef std::vector<NodeSortKey>	NodeSortKeyVectorType;
#endif

	/**
	 * Construct a NodeSorter, passing in the XSL Processor so it can know how
	 * to get the node data according to the proper whitespace rules.
	 */
	explicit
	NodeSorter();
  
	~NodeSorter();

	/**
	 * Given a list of nodes, sort each node according to the criteria in the
	 * keys.  The list is assumed to be in document order.
	 *
	 * @param executionContext current execution context
	 * @param v    list of Nodes
	 * @param keys vector of NodeSortKeys
	 */
	void
	sort(
			StylesheetExecutionContext&		executionContext,
			MutableNodeRefList&				theList,
			const NodeSortKeyVectorType&	keys) const;

	/*
	 * TODO: Optimize compare -- cache the getStringExpr results,
	 * key by m_selectPat + hash of node.
	 */

	/**
	 * Return the results of a compare of two nodes.
	 */
#if defined(XALAN_NO_NAMESPACES)
	struct NodeSortKeyCompare : public binary_function<XalanNode*, XalanNode*, bool>
#else
	struct NodeSortKeyCompare : public std::binary_function<XalanNode*, XalanNode*, bool>
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
				const MutableNodeRefList&		theList,
				const NodeVectorType&			theNodes,
				const NodeSortKeyVectorType&	theNodeSortKeys) :
			m_executionContext(executionContext),
			m_list(theList),
			m_nodes(theNodes),
			m_nodeSortKeys(theNodeSortKeys),
			m_numberResultsCache(),
			m_stringResultsCache()
		{
		}

	/**
	 * Compare two nodes
	 *
	 * @param executionContext current execution context
	 * @param theNodes        vector or nodes to be sorted
	 * @param theNodeSortKeys vector of keys upon which to sort
	 */
		result_type
		operator()(first_argument_type		theLHS,
				   second_argument_type		theRHS,
				   unsigned int				theKeyIndex = 0) const;

	protected:

		bool
		isNodeBefore(
				const XalanNode*	node1,
				const XalanNode*	node2) const;

		double
		getNumberResult(
				const NodeSortKey&	theKey,
				XalanNode*			node) const;

		const XalanDOMString
		getStringResult(
				const NodeSortKey&	theKey,
				XalanNode*			node) const;

	private:

		StylesheetExecutionContext&		m_executionContext;
		const MutableNodeRefList&		m_list;
		const NodeVectorType&			m_nodes;
		const NodeSortKeyVectorType&	m_nodeSortKeys;

#if defined(XALAN_NO_NAMESPACES)
		typedef	map<const XalanNode*, double>			NumberResultsNodeCacheMapType;
		typedef	map<const XalanNode*, XalanDOMString>	StringResultsNodeCacheMapType;

		typedef	map<const XPath*, NumberResultsNodeCacheMapType>	NumberResultsCacheMapType;
		typedef	map<const XPath*, StringResultsNodeCacheMapType>	StringResultsCacheMapType;
#else
		typedef	std::map<const XalanNode*, double>			NumberResultsNodeCacheMapType;
		typedef	std::map<const XalanNode*, XalanDOMString>	StringResultsNodeCacheMapType;

		typedef	std::map<const XPath*, NumberResultsNodeCacheMapType>	NumberResultsCacheMapType;
		typedef	std::map<const XPath*, StringResultsNodeCacheMapType>	StringResultsCacheMapType;
#endif

		mutable NumberResultsCacheMapType	m_numberResultsCache;
		mutable StringResultsCacheMapType	m_stringResultsCache;
	};

private:

	/**
	 * Given a vector of nodes, sort each node according to the criteria in the
	 * keys.
	 *
	 * @param executionContext current execution context
	 * @param theList the original node list.
	 * @param v    vector of Nodes
	 * @param keys vector of NodeSortKeys
	 */
	void
	sort(
			StylesheetExecutionContext&		executionContext,
			const MutableNodeRefList&		theList,
			NodeVectorType&					v,
			const NodeSortKeyVectorType&	keys) const;

	/**
	 * @@ TODO: Adjust this for locale.
	 * JMD: java: not used yet, placeholder
     */
  // NumberFormat m_formatter = NumberFormat.getNumberInstance();  
};



#endif	// XALAN_NODESORTER_HEADER_GUARD
