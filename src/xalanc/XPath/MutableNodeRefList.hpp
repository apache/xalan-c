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
#if !defined(MUTABLENODEREFLIST_HEADER_GUARD_1357924680)
#define MUTABLENODEREFLIST_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XPath/NodeRefList.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XPathExecutionContext;
class XalanDocument;
class XalanNodeList;



/**
 * Local implementation of MutableNodeRefList.  This class is for internal use
 * only.
 */
class XALAN_XPATH_EXPORT MutableNodeRefList : public NodeRefList
{
public:

	/**
	 * Construct an empty mutable node list.
	 */
	explicit
	MutableNodeRefList(MemoryManagerType& theManager);

    static MutableNodeRefList*
    create(MemoryManagerType& theManager);
	/**
	 * Construct a mutable node list from another list.
	 * 
	 * @param theSource source list
	 */
	MutableNodeRefList(const MutableNodeRefList&	theSource,
                        MemoryManagerType& theManager);

	/**
	 * Construct a mutable node list from another list.
	 * 
	 * @param theSource  source list
	 */
	explicit
	MutableNodeRefList(const NodeRefListBase&	theSource,
                        MemoryManagerType& theManager);

	virtual
	~MutableNodeRefList();

	MutableNodeRefList&
	operator=(const MutableNodeRefList&		theRHS);

	MutableNodeRefList&
	operator=(const NodeRefList&		theRHS);

	MutableNodeRefList&
	operator=(const NodeRefListBase&	theRHS);

	MutableNodeRefList&
	operator=(const XalanNodeList*	theRHS);

	/**
	 * Add a node at to the list.
	 * 
	 * @param n node to add
	 */
	void
	addNode(XalanNode*	n);

	/**
	 * Insert a node at a given position.
	 * 
	 * @param n   node to insert
	 * @param pos position of insertion
	 */
	void
	insertNode(
			XalanNode*	n,
			size_type	pos);

	/**
	 * Remove a node from the list.
	 * 
	 * @param n   node to insert
	 */
	void
	removeNode(const XalanNode*		n);

	/**
	 * Remove a node from the list.
	 * 
	 * @param pos position of node in list
	 */
	void
	removeNode(size_type	pos);

	/**
	 * Remove all nodes.
	 */
	void
	clear();

	/**
	 * Set a item.
	 * 
	 * @param pos position of node to modify
	 * @param n   node to insert, default is empty node
	 */
	void
	setNode(
			size_type	pos,
			XalanNode*	n = 0);

	/**
	 * Copy NodeList members into this nodelist, adding in document order.  If
	 * a node is null, don't add it.
	 * 
	 * @param nodelist node list to add
	 */
	void
	addNodes(const XalanNodeList&	nodelist);

	/**
	 * Copy NodeList members into this nodelist, adding in document order.  If
	 * a node is null, don't add it.
	 * 
	 * @param nodelist node list to add
	 */
	void
	addNodes(const NodeRefListBase&		nodelist);

	/**
	 * Copy NodeList members into this nodelist, adding in document order.
	 * 
	 * @param nodelist node list to add
	 * @param executionContext the current execution context
	 */
	void
	addNodesInDocOrder(
			const XalanNodeList&	nodelist,
			XPathExecutionContext&	executionContext);
  
	/**
	 * Copy NodeList members into this nodelist, adding in document order.
	 * 
	 * @param nodelist node list to add
	 * @param executionContext the current execution context
	 */
	void
	addNodesInDocOrder(
			const NodeRefListBase&	nodelist,
			XPathExecutionContext&	executionContext);

	/**
	 * Copy NodeList members into this nodelist, adding in document order.
	 * 
	 * @param nodelist node list to add
	 * @param executionContext the current execution context
	 */
	void
	addNodesInDocOrder(
			const MutableNodeRefList&	nodelist,
			XPathExecutionContext&		executionContext);
  
	/**
	 * Add a node into list where it should occur in document order.
	 *
	 * @param node node object to add
	 * @param executionContext the current execution context
	 */
	void
	addNodeInDocOrder(
			XalanNode*				node,
			XPathExecutionContext&	executionContext);

	/**
	 * Clear any null entries in the node list.
	 */
	void
	clearNulls();

	/**
	 * Reverse the nodes in the list.
	 */
	void
	reverse();

	/**
	 * Reserve space for the supplied number of nodes.
	 * This is taken as an optimization, and may be
	 * ignored.  You might want to call this when you
	 * know the number of nodes you're about to add to
	 * this list.
	 *
	 * Remember to take into account the current size of
	 * the list when calling this.  That means you will
	 * probably want to add the result of getLength() to
	 * the number of nodes you're planning to add.
	 *
	 * @param theCount the number of nodes to reserve space for
	 */
	void
	reserve(size_type	theCount)
	{
		m_nodeList.reserve(theCount);
	}

	/**
	 * See if the order of the nodes is an unknown order.
	 */
	bool
	getUnknownOrder() const
	{
		return m_order == eUnknownOrder ? true : false;
	}

	void
	setUnknownOrder()
	{
		m_order = eUnknownOrder;
	}

	/**
	 * See if the order of the nodes is document order.
	 */
	bool
	getDocumentOrder() const
	{
		return m_order == eDocumentOrder ? true : false;
	}

	/**
	 * Set the known order of the nodes.  This should
	 * only be done when the order is known. Otherwise,
	 * disaster will ensue.
	 */
	void
	setDocumentOrder()
	{
		m_order = eDocumentOrder;
	}

	/**
	 * See if the order of the nodes is reverse document order.
	 */
	bool
	getReverseDocumentOrder() const
	{
		return m_order == eReverseDocumentOrder ? true : false;
	}

	/**
	 * Set the known order of the nodes.  This should
	 * only be done when the order is known. Otherwise,
	 * disaster will ensue.
	 */
	void
	setReverseDocumentOrder()
	{
		m_order = eReverseDocumentOrder;
	}

	typedef NodeListVectorType::iterator	NodeListIteratorType;

	class addNodeInDocOrderFunctor
	{
	public:

		addNodeInDocOrderFunctor(
				MutableNodeRefList&		theList,
				XPathExecutionContext&	theExecutionContext) :
			m_list(theList),
			m_executionContext(theExecutionContext)
		{
		}

		void
		operator()(XalanNode*	theNode) const
		{
			m_list.addNodeInDocOrder(theNode, m_executionContext);
		}

	private:

		MutableNodeRefList&		m_list;

		XPathExecutionContext&	m_executionContext;
	};

	void
	swap(MutableNodeRefList&	theOther)
	{
		NodeRefList::swap(theOther);

		const eOrder	temp = m_order;

		m_order = theOther.m_order;

		theOther.m_order = temp;
	}

private:
    //not defined
   	MutableNodeRefList(const MutableNodeRefList&	theSource);

	// An enum to determine what the order of the nodes is...
	enum eOrder { eUnknownOrder, eDocumentOrder, eReverseDocumentOrder };

	eOrder	m_order;
};

XALAN_USES_MEMORY_MANAGER(MutableNodeRefList)

XALAN_CPP_NAMESPACE_END



#endif	// MUTABLENODEREFLIST_HEADER_GUARD_1357924680
