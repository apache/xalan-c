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
#include "NodeRefList.hpp"



#include <cassert>
#include <algorithm>

#if !defined(NDEBUG)
#include <set>
#endif



NodeRefList::NodeRefList() :
	NodeRefListBase(),
	m_nodeList()
{
}



NodeRefList::NodeRefList(const NodeRefList&		theSource) :
	NodeRefListBase(theSource),
	m_nodeList(theSource.m_nodeList)
{
}



NodeRefList::NodeRefList(const NodeRefListBase&		theSource) :
	NodeRefListBase(theSource),
	m_nodeList()
{
	*this = theSource;
}



NodeRefList::~NodeRefList()
{
}



NodeRefList&
NodeRefList::operator=(const NodeRefListBase&	theRHS)
{
	if (&theRHS != this)
	{
		m_nodeList.clear();

		const unsigned int	theLength = theRHS.getLength();

		ensureAllocation(theLength);

		for(unsigned int i = 0; i < theLength; i++)
		{
			XalanNode* const	theNode = theRHS.item(i);

			if (theNode != 0)
			{
				m_nodeList.push_back(theNode);
			}
		}

		// Chain up...
		NodeRefListBase::operator=(theRHS);
	}

	return *this;
}



NodeRefList&
NodeRefList::operator=(const NodeRefList&	theRHS)
{
	if (&theRHS != this)
	{
		m_nodeList = theRHS.m_nodeList;

		// Chain up...
		NodeRefListBase::operator=(theRHS);
	}

	return *this;
}



XalanNode*
NodeRefList::item(unsigned int	index) const
{
	assert(index < m_nodeList.size());

	return m_nodeList[index];
}



unsigned int
NodeRefList::getLength() const
{
	return m_nodeList.size();
}



unsigned int
NodeRefList::indexOf(const XalanNode*	theNode) const
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::find;
#endif

	// Look for the node in the list.
	NodeListVectorType::const_iterator	i =
		find(m_nodeList.begin(),
			 m_nodeList.end(),
			 theNode);

	// If not found, return npos.  Otherwise, subtract the iterator
	// from the first iterator to get the distance between them.
	return i == m_nodeList.end() ? npos : i - m_nodeList.begin();
}



XPathSupport*
NodeRefList::getSupport() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
NodeRefListBase*
#else
NodeRefList*
#endif
NodeRefList::clone() const
{
	return new NodeRefList(*this);
}



#if !defined(NDEBUG)
bool
NodeRefList::checkForDuplicates() const
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::set;
#endif

	bool	fResult = false;

	const unsigned int	theLength = getLength();

	if (theLength > 0)
	{
		set<const XalanNode*>	theNodes;

		for (unsigned i = 0; i < theLength && fResult == false; ++i)
		{
			const XalanNode* const	theNode = item(i);

			if (theNodes.find(theNode) != theNodes.end())
			{
				fResult = true;
			}
			else
			{
				theNodes.insert(theNode);
			}
		}
	}

	return fResult;
}
#endif
