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



#include <algorithm>
#include <cassert>
#include <functional>



#include <XalanDOM/XalanNamedNodeMap.hpp>
#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanNodeList.hpp>



#include "XPathExecutionContext.hpp"



MutableNodeRefList::MutableNodeRefList() :
	NodeRefList()
{
}



MutableNodeRefList::MutableNodeRefList(const MutableNodeRefList&	theSource) :
	NodeRefList(theSource)
{
}



MutableNodeRefList::MutableNodeRefList(const NodeRefListBase&	theSource) :
	NodeRefList(theSource)
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
MutableNodeRefList::operator=(const XalanNodeList*	theRHS)
{
	clear();

	if (theRHS != 0)
	{
		addNodes(*theRHS);
	}

	return *this;
}



void
MutableNodeRefList::addNode(XalanNode*	n)
{
	if (n != 0)
	{
		// ensureAllocation();

		m_nodeList.push_back(n);
	}
}



void
MutableNodeRefList::insertNode(
			XalanNode*		n,
			unsigned int	pos)
{
 	assert(m_nodeList.size() >= pos);

	if (n != 0)
	{
		// ensureAllocation();

		m_nodeList.insert(m_nodeList.begin() + pos, n);
	}
}



void
MutableNodeRefList::removeNode(const XalanNode*		n)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::find;
#endif

	NodeListVectorType::iterator	i =
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
	assert(pos < m_nodeList.size());

	m_nodeList.erase(m_nodeList.begin() + pos);
}



void
MutableNodeRefList::clear()
{
	m_nodeList.clear();
}



void
MutableNodeRefList::setNode(
			unsigned int	pos,
			XalanNode*		theNode)
{
	assert(pos < m_nodeList.size());

	m_nodeList[pos] = theNode;
}



inline void
MutableNodeRefList::ensureAllocation(NodeListVectorType::size_type	/* theSize */)
{
	// This code is commmented out for now, since it appears to actual hurt
	// performance, rather than help.
#if 0
	const unsigned int	theNodeListSize = m_nodeList.size();

	if (theSize > theNodeListSize)
	{
		const unsigned int	theNewSize = theNodeListSize * 2;

		if (theSize > theNewSize)
		{
			m_nodeList.reserve(theSize * 2);
		}
		else
		{
			m_nodeList.reserve(theNewSize);
		}
	}
	else
	{
		m_nodeList.reserve(eDefaultVectorSize);
	}
#endif
}



void
MutableNodeRefList::addNodes(const XalanNodeList&	nodelist)
{
	const unsigned int	theLength = nodelist.getLength();

	// ensureAllocation(m_nodeList.size() + theLength);

	for (unsigned int i = 0; i < theLength; i++)
	{
		addNode(nodelist.item(i));
	}
}



void
MutableNodeRefList::addNodes(const NodeRefListBase&		nodelist)
{
	const unsigned int	theLength = nodelist.getLength();

	// ensureAllocation(m_nodeList.size() + theLength);

	for (unsigned int i = 0; i < theLength; i++)
	{
		addNode(nodelist.item(i));
	}
}



void
MutableNodeRefList::addNodesInDocOrder(
			const XalanNodeList&	nodelist,
			XPathExecutionContext&	executionContext)
{
	const unsigned int	theLength = nodelist.getLength();

	// ensureAllocation(m_nodeList.size() + theLength);

	for(unsigned int i = 0; i < theLength; i++)
	{
		addNodeInDocOrder(nodelist.item(i), executionContext);
	}
}



void
MutableNodeRefList::addNodesInDocOrder(
			const NodeRefListBase&	nodelist,
			XPathExecutionContext&	executionContext)
{
	const unsigned int	theLength = nodelist.getLength();

	// ensureAllocation(m_nodeList.size() + theLength);

	for(unsigned int i = 0; i < theLength; i++)
	{
		addNodeInDocOrder(nodelist.item(i), executionContext);
	}
}



bool
findInsertionPointBinarySearch(
			XalanNode*									node,
			MutableNodeRefList::NodeListIteratorType	begin,
			MutableNodeRefList::NodeListIteratorType	end,
			MutableNodeRefList::NodeListIteratorType&	insertionPoint)
{
	assert(node != 0);
	assert(node->getNodeType() == XalanNode::DOCUMENT_NODE ||
		   (node->getOwnerDocument() != 0 && node->getOwnerDocument()->isIndexed() == true));

	bool	fInsert = true;

	// At this point, we are guaranteed that the range is only for this
	// document, and that the range is indexed...
	const unsigned long		theIndex = node->getIndex();

	typedef MutableNodeRefList::NodeListIteratorType	NodeListIteratorType;

	// End points to one past the last valid point,
	// so subtract 1.
	NodeListIteratorType	last(end - 1);
	assert(*last != 0);

	// Do a quick check to see if we just need to append...
	if ((*last)->getIndex() < theIndex)
	{
		insertionPoint = end;
	}
	else
	{
		// Do a binary search for the insertion point...
		NodeListIteratorType	first(begin);
		NodeListIteratorType	current(end);

		unsigned long			theCurrentIndex = 0;

		while (first <= last)
		{
			current = first + (last - first) / 2;
			assert(*current != 0);

			theCurrentIndex = (*current)->getIndex();

			if (theIndex < theCurrentIndex)
			{
				last = current - 1;
			}
			else if (theIndex > theCurrentIndex)
			{
				first = current + 1;
			}
			else if (theIndex == theCurrentIndex)
			{
				// Duplicate, don't insert...
				fInsert = false;

				break;
			}
		}

		if (theIndex != theCurrentIndex)
		{
			if (current == end || first == end)
			{
				// We either didn't search, or we're
				// at the end...
				insertionPoint = end;
			}
			else if (theCurrentIndex < theIndex)
			{
				// We're inserting after the current position...
				assert((*current)->getIndex() < theIndex &&
					   (current + 1 == end || (*(current + 1))->getIndex() > theIndex));

				insertionPoint = current + 1;
			}
			else
			{
				// We're inserting before the current position...
				assert(theCurrentIndex > theIndex);
				assert((*current)->getIndex() > theIndex &&
					   (current == begin || (*(current))->getIndex() > theIndex));

				insertionPoint = current;
			}
		}
	}

	return fInsert;
}



template<class PredicateType>
bool
findInsertionPointLinearSearch(
			XalanNode*									node,
			MutableNodeRefList::NodeListIteratorType	begin,
			MutableNodeRefList::NodeListIteratorType	end,
			MutableNodeRefList::NodeListIteratorType&	insertionPoint,
			const PredicateType							isNodeAfterPredicate)
{
	assert(node != 0);

	bool	fInsert = true;

	typedef MutableNodeRefList::NodeListIteratorType	NodeListIteratorType;

	NodeListIteratorType	current(begin);

	// Loop, looking for the node, or for a
	// node that's before the one we're adding...
	while(current != end)
	{
		const XalanNode*	child = *current;
		assert(child != 0);

		if(child == node)
		{
			// Duplicate, don't insert...
			fInsert = false;

			break;
		}
		else if (isNodeAfterPredicate(*node, *child) == false)
		{
			// We found the insertion point...
			break;
		}
		else
		{
			++current;
		}
	}

	insertionPoint = current;

	return fInsert;
}



struct DocumentPredicate
{
	bool
	operator()(
			const XalanNode&	node1,
			const XalanNode&	node2) const
	{
		// Always order a document node, or a node from another
		// document after another node...
		return node1.getNodeType() == XalanNode::DOCUMENT_NODE &&
			   node2.getNodeType() == XalanNode::DOCUMENT_NODE ? true :
					node1.getOwnerDocument() != node2.getOwnerDocument() ? true : false;
	}
};



struct IndexPredicate
{
	bool
	operator()(
			const XalanNode&	node1,
			const XalanNode&	node2) const
	{
		assert(node1.getOwnerDocument() == node2.getOwnerDocument());

		return m_documentPredicate(node1, node2) == true ? true : node1.getIndex() > node2.getIndex() ? true : false;
	}

	DocumentPredicate	m_documentPredicate;
};




struct ExecutionContextPredicate
{
	ExecutionContextPredicate(XPathExecutionContext&	executionContext) :
		m_executionContext(executionContext)
	{
	}

	bool
	operator()(
			const XalanNode&	node1,
			const XalanNode&	node2) const
	{
		if (m_documentPredicate(node1, node2) == true)
		{
			return true;
		}
		else
		{
			assert(node1.getOwnerDocument() == node2.getOwnerDocument());
			assert(node1.getNodeType() != XalanNode::DOCUMENT_NODE &&
				   node2.getNodeType() != XalanNode::DOCUMENT_NODE);

			return  m_executionContext.isNodeAfter(node1, node2);
		}
	}

	XPathExecutionContext&	m_executionContext;

	DocumentPredicate		m_documentPredicate;
};




void
MutableNodeRefList::addNodeInDocOrder(
			XalanNode*				node,
			XPathExecutionContext&	executionContext)
{
	if (node != 0)
	{
		const unsigned int	size = m_nodeList.size();

		// ensureAllocation(size + 1);

		if (size == 0)
		{
			addNode(node);
		}
		else
		{
			assert(m_nodeList[0] != 0);

			// Do some quick optimizations, since we tend to append
			// the same node a lot.
			const XalanNode* const	theLastNode = m_nodeList.back();
			assert(theLastNode != 0);

			// Is it a duplicate?
			if (theLastNode != node)
			{
				bool					fInsert = false;

				NodeListIteratorType	insertionPoint;

				const XalanNode* const	theFirstNode = m_nodeList.front();
				assert(theFirstNode != 0);

				// Normalize so that if we have a document node, it owns
				// itself, which is not how DOM works...
				const XalanNode* const	theFirstNodeOwner =
					theFirstNode->getNodeType() == XalanNode::DOCUMENT_NODE ?
							theFirstNode : theFirstNode->getOwnerDocument();
				assert(theFirstNodeOwner != 0);

				if (node->isIndexed() == true &&
					node->getOwnerDocument() == theFirstNodeOwner)
				{
					// If it's indexed, then see if the entire list consists of
					// nodes from the same document.
					// Normalize so that if we have a document node, it owns
					// itself, which is not how DOM works...
					const XalanNode* const	theLastNodeOwner =
						theLastNode->getNodeType() == XalanNode::DOCUMENT_NODE ?
								theLastNode : theLastNode->getOwnerDocument();
					assert(theLastNodeOwner != 0);

					// If the owner document is 0, then it's a document node, so there's not
					// much we can do except a linear search...
					if (theFirstNodeOwner == theLastNodeOwner)
					{
						fInsert =
							findInsertionPointBinarySearch(
									node,
									m_nodeList.begin(),
									m_nodeList.end(),
									insertionPoint);
					}
					else
					{
						fInsert =
							findInsertionPointLinearSearch(
									node,
									m_nodeList.begin(),
									m_nodeList.end(),
									insertionPoint,
									IndexPredicate());
					}
				}
				else
				{
					fInsert =
							findInsertionPointLinearSearch(
									node,
									m_nodeList.begin(),
									m_nodeList.end(),
									insertionPoint,
									ExecutionContextPredicate(executionContext));
				}

				if (fInsert == true)
				{
					m_nodeList.insert(insertionPoint, node);
				}
			}
		}
	}
}



void
MutableNodeRefList::clearNulls()
{
#if 1
#if !defined(XALAN_NO_NAMESPACES)
	using std::remove;
#endif

	m_nodeList.erase(
		remove(
			m_nodeList.begin(),
			m_nodeList.end(), 
			NodeListVectorType::value_type(0)),
		m_nodeList.end());

#else
	NodeListVectorType::iterator	i =
		m_nodeList.begin();

	while(i != m_nodeList.end())
	{
		if (*i == 0)
		{
			NodeListVectorType::iterator	j = i + 1;

			while(j != m_nodeList.end() && *j == 0)
			{
				++j;
			}

			i = m_nodeList.erase(i, j);
		}
		else
		{
			++i;
		}
	}
#endif

	assert(checkForDuplicates() == false);
}
