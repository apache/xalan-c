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
#include "ResultTreeFrag.hpp"



#include <algorithm>
#include <cassert>



#include <DOMSupport/DOMServices.hpp>



const XalanDOMString	ResultTreeFrag::s_emptyString;



ResultTreeFrag::ResultTreeFrag() :
	ResultTreeFragBase(),
	XalanNodeList()
{
}



ResultTreeFrag::ResultTreeFrag(const ResultTreeFrag&	theSource,
							   bool						deepClone) :
	ResultTreeFragBase(theSource),
	XalanNodeList(),
	m_children(deepClone == false ? theSource.m_children : NodeVectorType())
{
	if (deepClone == true)
	{
		const unsigned int	theLength = theSource.m_children.size();

		m_children.reserve(theLength);

		for (unsigned int i = 0; i < theLength; ++i)
		{
			assert(theSource.m_children[i] != 0);

			m_children.push_back(theSource.m_children[i]->cloneNode(true));
		}
	}
}



ResultTreeFrag::~ResultTreeFrag()
{
}



void
ResultTreeFrag::clear()
{
	m_children.clear();
}



const XalanDOMString&
ResultTreeFrag::getNodeName() const
{
	return s_emptyString;
}



const XalanDOMString&
ResultTreeFrag::getNodeValue() const
{
	return s_emptyString;
}



ResultTreeFrag::NodeType
ResultTreeFrag::getNodeType() const
{
	return DOCUMENT_FRAGMENT_NODE;
}



XalanNode*
ResultTreeFrag::getParentNode() const
{
	return 0;
}



const XalanNodeList*
ResultTreeFrag::getChildNodes() const
{
	return this;
}



XalanNode*
ResultTreeFrag::getFirstChild() const
{
	return m_children.size() == 0 ? 0 : m_children[0];
}



XalanNode*
ResultTreeFrag::getLastChild() const
{
	const unsigned int	theLength = m_children.size();
	

	return theLength == 0 ? 0 : m_children.back();
}



XalanNode*
ResultTreeFrag::getPreviousSibling() const
{
	return 0;
}



XalanNode*
ResultTreeFrag::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
ResultTreeFrag::getAttributes() const
{
	return 0;
}



XalanDocument*
ResultTreeFrag::getOwnerDocument() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
ResultTreeFrag*
#endif
ResultTreeFrag::cloneNode(bool	deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return new ResultTreeFrag(*this,
							  deep);
#else
	return clone(deep);
#endif
}



XalanNode*
ResultTreeFrag::insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::find;
#endif

	const NodeVectorType::iterator	i =
		0 == refChild ? m_children.end() :
						find(m_children.begin(),
							 m_children.end(),
							 refChild);

	assert(0 == refChild || i != m_children.end());

	m_children.insert(i, newChild);

	return newChild;
}



XalanNode*
ResultTreeFrag::replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild)
{
	assert(newChild != 0);

#if !defined(XALAN_NO_NAMESPACES)
	using std::find;
#endif

	// Look for the old child...
	NodeVectorType::iterator	i =
		0 == oldChild ? m_children.end() :
						find(m_children.begin(),
							 m_children.end(),
							 oldChild);

	// Did we find it?
	if(i != m_children.end())
	{
		// First, look for an occurrence of the
		// new child, because we'll have to remove
		// it if it's there...
		const NodeVectorType::iterator	j =
				find(m_children.begin(),
					 m_children.end(),
					 newChild);

		// OK, if the newChild is already in the list,
		// and it's after the new position, then we
		// can just erase and set the newChild.  If it's
		// not, then we have to erase and set the new
		// child at the previous iterator position, since
		// we've erased a node from the vector, and the indices
		// are now off...
		if(j != m_children.end())
		{
			if (j < i)
			{
				// It's less, so decrement...
				--i;
			}

			m_children.erase(j);
		}

		assert((*i) == oldChild);

		(*i) = newChild;
	}

	return oldChild;
}



XalanNode*
ResultTreeFrag::appendChild(XalanNode*	newChild)
{
	assert(newChild != 0);

	m_children.push_back(newChild);

	return newChild;
}



XalanNode*
ResultTreeFrag::removeChild(XalanNode*	oldChild)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::find;
#endif

	// Look for the old child...
	const NodeVectorType::iterator	i =
		find(m_children.begin(),
			 m_children.end(),
			 oldChild);

	m_children.erase(i);

	return oldChild;
}


bool
ResultTreeFrag::hasChildNodes() const
{
	return m_children.size() > 0 ? true : false;
}



void
ResultTreeFrag::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
}



void
ResultTreeFrag::normalize()
{
}



bool
ResultTreeFrag::supports(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
ResultTreeFrag::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
ResultTreeFrag::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
ResultTreeFrag::getLocalName() const
{
	return s_emptyString;
}



void
ResultTreeFrag::setPrefix(const XalanDOMString&		/* prefix */)
{
}



bool
ResultTreeFrag::isIndexed() const
{
	return false;
}



unsigned long
ResultTreeFrag::getIndex() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
ResultTreeFragBase*
#else
ResultTreeFrag*
#endif
ResultTreeFrag::clone(bool	deep) const
{
	return new ResultTreeFrag(*this, deep);
}



XalanNode*
ResultTreeFrag::item(unsigned int	index) const
{
	assert(index < m_children.size());

	return m_children[index];
}



unsigned int
ResultTreeFrag::getLength() const
{
	return m_children.size();
}
