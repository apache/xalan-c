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



#include <XalanDOM/XalanNamedNodeMap.hpp>
#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanNodeList.hpp>



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



MutableNodeRefList::MutableNodeRefList(const NodeRefListBase&	theSource) :
	NodeRefList(theSource),
	m_support(theSource.getSupport())
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
		m_nodeList.push_back(n);
	}
}



void
MutableNodeRefList::insertNode(
			XalanNode*		n,
			unsigned int	pos)
{
 	assert(getLength() >= pos);

	if (n != 0)
	{
		m_nodeList.insert(&m_nodeList[pos], n);
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
			unsigned int	pos,
			XalanNode*		theNode)
{
	assert(pos < getLength());

	if (theNode != 0)
	{
		m_nodeList[pos] = theNode;
	}
}



void
MutableNodeRefList::addNodes(const XalanNodeList&	nodelist)
{
	const unsigned int	theLength = nodelist.getLength();

	for (unsigned int i = 0; i < theLength; i++)
	{
		addNode(nodelist.item(i));
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
MutableNodeRefList::addNodesInDocOrder(const XalanNodeList&		nodelist)
{
	const unsigned int	nChildren = nodelist.getLength();

	for(unsigned int i = 0; i < nChildren; i++)
	{
		addNodeInDocOrder(nodelist.item(i), false);
	}
}



void
MutableNodeRefList::addNodesInDocOrder(const NodeRefListBase&	nodelist)
{
	const unsigned int	nChildren = nodelist.getLength();

	for(unsigned int i = 0; i < nChildren; i++)
	{
		addNodeInDocOrder(nodelist.item(i), true);
	}
}



void
MutableNodeRefList::addNodeInDocOrder(
			XalanNode*	node,
			bool		test)
{
	if (node != 0)
	{
		const unsigned int	size = getLength();

		if (test == false || m_support == 0 || size == 0)
		{
			addNode(node);
		}
		else
		{

			unsigned int	i = size - 1;

			// When wrap-around happens, i will be > than size...
			for(; i < size; i--)
			{
				const XalanNode*	child = m_nodeList[i];
				assert(child != 0);

				if(child == node)
				{
					// Duplicate, don't insert...
					i = size;

					break;
				}
				else if (m_support->isNodeAfter(*node, *child) == false)
				{
					break;
				}
			}

			if (i != size)
			{
				insertNode(node, i + 1);
			}
		}
	}
}



XPathSupport*
MutableNodeRefList::getSupport() const
{
	return m_support;
}
