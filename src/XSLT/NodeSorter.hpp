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
#include <vector>




//#include <dom/DOM_Node.hpp>



#include "NodeSortKey.hpp"


class DOM_Node;
class MutableNodeRefList;
class XPath;
class XPathExecutionContext;



/**
 * This class can sort vectors of DOM nodes according to a select pattern.
 * TODO: Optimize this so it can reuse queries for each of the nodes.
 */
class NodeSorter
{
public:

	/**
	 * Construct a NodeSorter, passing in the XSL Processor 
	 * so it can know how to get the node data according to 
	 * the proper whitespace rules.
	 */
	NodeSorter(XPathExecutionContext&	executionContext);
  
	~NodeSorter();

	/**
	 * Given a vector of nodes, sort each node according to 
	 * the criteria in the keys.
	 * @param v an vector of Nodes.
	 * @param keys a vector of NodeSortKeys.
	 */
	void
	sort(
			std::vector<DOM_Node>&				v,
			const std::vector<NodeSortKey>&		keys);
/*
     throws XSLProcessorException, 
           java.net.MalformedURLException, 
           java.io.FileNotFoundException, 
           java.io.IOException
*/

	/**
	 * Given a vector of nodes, sort each node according to 
	 * the criteria in the keys.
	 * @param v an vector of Nodes.
	 * @param keys a vector of NodeSortKeys.
	 */
	void
	sort(
			MutableNodeRefList&					theList,
			const std::vector<NodeSortKey>&		keys);

	/**
	 * Return the results of a compare of two nodes.
	 * TODO: Optimize compare -- cache the getStringExpr results,
	 * key by m_selectPat + hash of node.
	 */
	struct NodeSortKeyCompare : public std::binary_function<const DOM_Node&, const DOM_Node&, bool>
	{
	public:

		NodeSortKeyCompare(XPathExecutionContext&			executionContext,
						   const std::vector<DOM_Node>&		theNodes,
						   const std::vector<NodeSortKey>&	theNodeSortKeys) :
			m_executionContext(executionContext),
			m_nodes(theNodes),
			m_nodeSortKeys(theNodeSortKeys)
		{
		}

		result_type
		operator()(first_argument_type		theLHS,
				   second_argument_type		theRHS,
				   unsigned int				theKeyIndex = 0) const;

		XPathExecutionContext&				m_executionContext;
		const std::vector<DOM_Node>&		m_nodes;
		const std::vector<NodeSortKey>&		m_nodeSortKeys;
	};

private:

	XPathExecutionContext&		m_executionContext;

	std::vector<NodeSortKey>	m_keys; // vector of NodeSortKeys
  /**
   * @@ TODO: Adjust this for locale.
	* JMD: java: not used yet, placeholder
   */
  // NumberFormat m_formatter = NumberFormat.getNumberInstance();
  
};



#endif	// XALAN_NODESORTER_HEADER_GUARD
