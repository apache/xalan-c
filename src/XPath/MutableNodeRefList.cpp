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
// Class header file.
#include "MutableNodeRefList.hpp"



#include <cassert>
#include <algorithm>



#include <dom/DOM_NamedNodeMap.hpp>
#include <dom/DOM_NodeList.hpp>



#include <Include/DOMHelper.hpp>



#include "XPathSupport.hpp"



MutableNodeRefList::MutableNodeRefList(XPathSupport*	theSupport) :
	NodeRefList(),
	m_support(theSupport)
{
}



MutableNodeRefList::MutableNodeRefList(const MutableNodeRefList&	theSource) :
	NodeRefList(theSource),
	m_support(theSource.m_support)
{
}



MutableNodeRefList::MutableNodeRefList(const NodeRefListBase&	theSource,
									   XPathSupport*			theSupport) :
	NodeRefList(theSource),
	m_support(theSupport)
{
}


	
MutableNodeRefList::~MutableNodeRefList()
{
}



MutableNodeRefList&
MutableNodeRefList::operator=(const MutableNodeRefList&		theRHS)
{
	if (this != &theRHS)
	{
		// Chain up...
		NodeRefList::operator=(theRHS);
	}

	return *this;
}



MutableNodeRefList&
MutableNodeRefList::operator=(const NodeRefList&		theRHS)
{
	if (this != &theRHS)
	{
		// Chain up...
		NodeRefList::operator=(theRHS);
	}

	return *this;
}



MutableNodeRefList&
MutableNodeRefList::operator=(const NodeRefListBase&	theRHS)
{
	if (this != &theRHS)
	{
		// Chain up...
		NodeRefList::operator=(theRHS);
	}

	return *this;
}



MutableNodeRefList&
MutableNodeRefList::operator=(const DOM_NodeList&	theRHS)
{
	clear();

	addNodes(theRHS);

	return *this;
}



void
MutableNodeRefList::addNode(const DOM_Node&		n)
{
	if (n != 0)
	{
		m_nodeList.push_back(n);
	}
}



void
MutableNodeRefList::insertNode(
			const DOM_Node&		n,
			unsigned int		pos)
{
 	assert(getLength() >= pos);

	if (n != 0)
	{
		m_nodeList.insert(&m_nodeList[pos], n);
	}
}



void
MutableNodeRefList::removeNode(const DOM_Node&	n)
{
	using std::vector;
	using std::find;

	std::vector<DOM_Node>::iterator		i =
		find(m_nodeList.begin(),
			 m_nodeList.end(),
			 n);

	if (i != m_nodeList.end())
	{
		m_nodeList.erase(i);
	}
}



void
MutableNodeRefList::removeNode(unsigned int		pos)
{
	assert(pos < getLength());

	m_nodeList.erase(&m_nodeList[pos]);
}



void
MutableNodeRefList::clear()
{
	m_nodeList.clear();
}



void
MutableNodeRefList::setNode(
			unsigned int		pos,
			const DOM_Node&		theNode)
{
	assert(pos < getLength());

	if (theNode != 0)
	{
		m_nodeList[pos] = theNode;
	}
}



void
MutableNodeRefList::addNodes(const DOM_NodeList&	nodelist)
{
	if (nodelist != 0)
	{
		const unsigned int	theLength = nodelist.getLength();

		for (unsigned int i = 0; i < theLength; i++)
		{
			addNode(nodelist.item(i));
		}
	}
}



void
MutableNodeRefList::addNodes(const NodeRefListBase&		nodelist)
{
	const unsigned int	theLength = nodelist.getLength();

	for (unsigned int i = 0; i < theLength; i++)
	{
		addNode(nodelist.item(i));
	}
}



void
MutableNodeRefList::addNodesInDocOrder(const DOM_NodeList&	nodelist)
{
	const unsigned int	nChildren = nodelist.getLength();

	for(unsigned int i = 0; i < nChildren; i++)
	{
		addNodeInDocOrder(nodelist.item(i));
	}
}



void
MutableNodeRefList::addNodesInDocOrder(const NodeRefListBase&	nodelist)
{
	const unsigned int	nChildren = nodelist.getLength();

	for(unsigned int i = 0; i < nChildren; i++)
	{
		addNodeInDocOrder(nodelist.item(i),
						  true);
	}
}



void
MutableNodeRefList::addNodeInDocOrder(
			const DOM_Node&		node,
			bool				test)
{
	if (node != 0)
	{
		if (test == false || m_support == 0)
		{
			addNode(node);
		}
		else
		{
			const unsigned int	size = getLength();

			unsigned int		i = size - 1;

			// When wrap-around happens, i will be > than size...
			for(; i < size; i--)
			{
				const DOM_Node&		child = m_nodeList[i];

				if(child == node)
				{
					// Duplicate, don't insert...
					i = size;

					break;
				}
				else if (isNodeAfter(node, child) == false)
				{
					break;
				}
			}

			if (i < size)
			{
				insertNode(node, i + 1);
			}
		}
	}
}



bool
MutableNodeRefList::isNodeAfter(
			const DOM_Node&		node1,
			const DOM_Node&		node2) const
{
	assert(m_support != 0);
	assert(node1 != 0 && node2 != 0);

	bool		isNodeAfter = false;

	DOM_Node	parent1 = m_support->getParentOfNode(node1);
	DOM_Node	parent2 = m_support->getParentOfNode(node2);

	// Optimize for most common case
	if(parent1 == parent2) // then we know they are siblings
	{
		isNodeAfter = isNodeAfterSibling(parent1,
										 node1,
										 node2);
	}
	else
	{
		// General strategy: Figure out the lengths of the two 
		// ancestor chains, and walk up them looking for the 
		// first common ancestor, at which point we can do a 
		// sibling compare.  Edge condition where one is the 
		// ancestor of the other.
	  
		// Count parents, so we can see if one of the chains 
		// needs to be equalized.
		int nParents1 = 2;
		int nParents2 = 2; // count node & parent obtained above

		while(parent1 != 0)
		{
			nParents1++;
			parent1 = m_support->getParentOfNode(parent1);
		}

		while(parent2 != 0)
		{
			nParents2++;
			parent2 = m_support->getParentOfNode(parent2);
		}

		// adjustable starting points
		DOM_Node	startNode1 = node1;
		DOM_Node	startNode2 = node2;

		// Do I have to adjust the start point in one of 
		// the ancesor chains?
		if(nParents1 < nParents2)
		{
			// adjust startNode2
			const int	adjust = nParents2 - nParents1;

			for(int i = 0; i < adjust; i++)
			{
				startNode2 = m_support->getParentOfNode(startNode2);
			}
		}
		else if(nParents1 > nParents2)
		{
			// adjust startNode1
			const int	adjust = nParents1 - nParents2;

			for(int i = 0; i < adjust; i++)
			{
				startNode1 = m_support->getParentOfNode(startNode1);
			}
		}

		// so we can "back up"
		DOM_Node	prevChild1;
		DOM_Node	prevChild2;
	  
		// Loop up the ancestor chain looking for common parent.
		while(0 != startNode1)
		{
			if(startNode1 == startNode2) // common parent?
			{
				if(0 == prevChild1) // first time in loop?
				{
					// Edge condition: one is the ancestor of the other.
					isNodeAfter = (nParents1 < nParents2) ? true : false;

					break; // from while loop
				}
				else
				{
					isNodeAfter = isNodeAfterSibling(startNode1,
													 prevChild1,
													 prevChild2);

					break; // from while loop
				}
			}

			prevChild1 = startNode1;
			startNode1 = m_support->getParentOfNode(startNode1);
			prevChild2 = startNode2;
			startNode2 = m_support->getParentOfNode(startNode2);
		}
	}
	
	/* -- please do not remove... very useful for diagnostics --
	System.out.println("node1 = "+node1.getNodeName()+"("+node1.getNodeType()+")"+
	", node2 = "+node2.getNodeName()
	+"("+node2.getNodeType()+")"+
	", isNodeAfter = "+isNodeAfter); */

	return isNodeAfter;
}



bool
MutableNodeRefList::isNodeAfterSibling(
			const DOM_Node&		parent,
			const DOM_Node&		child1,
			const DOM_Node&		child2)
{
	bool	isNodeAfterSibling = false;

	const int	child1type = child1.getNodeType();
	const int	child2type = child2.getNodeType();

	if(DOM_Node::ATTRIBUTE_NODE != child1type &&
	   DOM_Node::ATTRIBUTE_NODE == child2type)
	{
		// always sort attributes before non-attributes.
		isNodeAfterSibling = false;
	}
	else if(DOM_Node::ATTRIBUTE_NODE == child1type &&
			DOM_Node::ATTRIBUTE_NODE != child2type)
	{
		// always sort attributes before non-attributes.
		isNodeAfterSibling = true;
	}
	else if(DOM_Node::ATTRIBUTE_NODE == child1type)
	{
		DOM_NamedNodeMap	children = parent.getAttributes();
	  
		const unsigned int	nNodes = children.getLength();

		bool				found1 = false;
		bool				found2 = false;

		for(unsigned int i = 0; i < nNodes; i++)
		{
			const DOM_Node	child = children.item(i);

			if(child1 == child)
			{
				if(found2 == true)
				{
					isNodeAfterSibling = false;
					break;
				}
		  
				found1 = true;
			}
			else if(child2 == child)
			{
				if(found1 == true)
				{
					isNodeAfterSibling = true;
					break;
				}
		  
				found2 = true;
			}
		}
	}
	else
	{
		DOM_NodeList	children = parent.getChildNodes();

		const int		nNodes = children.getLength();

		bool			found1 = false;
		bool			found2 = false;

		for(int i = 0; i < nNodes; i++)
		{
			const DOM_Node	child = children.item(i);

			if(child1 == child)
			{
				if(found2 == true)
				{
					isNodeAfterSibling = false;
					break;
				}

				found1 = true;
			}
			else if(child2 == child)
			{
				if(found1 == true)
				{
					isNodeAfterSibling = true;
					break;
				}

				found2 = true;
			}
		}
	}

	return isNodeAfterSibling;
}
