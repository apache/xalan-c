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
#if !defined(MUTABLENODEREFLIST_HEADER_GUARD_1357924680)
#define MUTABLENODEREFLIST_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <XPath/NodeRefList.hpp>



class DOM_NodeList;



/**
 * Local implementation of MutableNodeRefList.  This class is for internal use only.
 */
class XPathSupport;



class XALAN_XPATH_EXPORT MutableNodeRefList : public NodeRefList
{
public:

	explicit
	MutableNodeRefList(XPathSupport*	theSupport = 0);

	MutableNodeRefList(const MutableNodeRefList&	theSource);

	explicit
	MutableNodeRefList(const NodeRefListBase&	theSource,
					   XPathSupport*			theSupport = 0);

	virtual
	~MutableNodeRefList();

	MutableNodeRefList&
	operator=(const MutableNodeRefList&		theRHS);

	MutableNodeRefList&
	operator=(const NodeRefList&		theRHS);

	MutableNodeRefList&
	operator=(const NodeRefListBase&	theRHS);

	MutableNodeRefList&
	operator=(const DOM_NodeList&	theRHS);

	virtual void
	addNode(const DOM_Node&		n);

	/**
	 * Insert a node at a given position.
	 */
	virtual void
	insertNode(
			const DOM_Node&		n,
			unsigned int		pos);

	/**
	 * Remove a node.
	 */
	virtual void
	removeNode(const DOM_Node&	n);

	virtual void
	removeNode(unsigned int		pos);

	/**
	 * Remove all nodes.
	 */
	virtual void
	clear();

	/**
	 * Set a item.
	 */
	virtual void
	setNode(unsigned int		pos,
			const DOM_Node&		n = DOM_Node());

	/**
	 * Copy NodeList members into this nodelist, adding in 
	 * document order.  If a node is null, don't add it.
	 */
	virtual void
	addNodes(const DOM_NodeList&	nodelist);

	/**
	 * Copy NodeList members into this nodelist, adding in 
	 * document order.  If a node is null, don't add it.
	 */
	virtual void
	addNodes(const NodeRefListBase&		nodelist);

	/**
	 * Copy NodeList members into this nodelist, adding in 
	 * document order.
	 */
	virtual void
	addNodesInDocOrder(const DOM_NodeList&	nodelist);
  
	/**
	 * Copy NodeList members into this nodelist, adding in 
	 * document order.
	 */
	virtual void
	addNodesInDocOrder(const NodeRefListBase&	nodelist);
  
	/**
	 * Add the node into a vector of nodes where it should occur in 
	 * document order.
	 * @param v Vector of nodes, presumably containing Nodes
	 * @param obj Node object.
	 * @param test true if we should test for doc order
	 */
	virtual void
	addNodeInDocOrder(
			const DOM_Node&		node,
			bool				test = false);

protected:

	virtual bool
	isNodeAfter(
			const DOM_Node&		node1,
			const DOM_Node&		node2) const;

	static bool
	isNodeAfterSibling(
			const DOM_Node&		parent,
			const DOM_Node&		child1,
			const DOM_Node&		child2);

	XPathSupport*
	getSupport() const
	{
		return m_support;
	}

private:

	XPathSupport*	m_support;
};



#endif	// MUTABLENODEREFLIST_HEADER_GUARD_1357924680
