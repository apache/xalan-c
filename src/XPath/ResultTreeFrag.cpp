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



#include <cassert>



ResultTreeFrag::ResultTreeFrag(
			XalanDocument&	theOwnerDocument,
			XPathSupport&	theSupport) :
	ResultTreeFragBase(),
	m_document(&theOwnerDocument),
	m_children(&theSupport),
	m_surrogate(m_children)
{
}



ResultTreeFrag::ResultTreeFrag(const ResultTreeFrag&	theSource,
							   bool						deepClone) :
	ResultTreeFragBase(theSource),
	m_document(theSource.m_document),
	m_children(deepClone == false ? theSource.m_children : MutableNodeRefList()),
	m_surrogate(m_children)
{
	if (deepClone == true)
	{
		const int	theLength = theSource.m_children.getLength();

		for (int i = 0; i < theLength; i++)
		{
			m_children.addNode(theSource.m_children.item(i)->cloneNode(true));
		}
	}
}



ResultTreeFrag::~ResultTreeFrag()
{
}



XalanDOMString
ResultTreeFrag::getNodeName() const
{
	return XalanDOMString();
}



XalanDOMString
ResultTreeFrag::getNodeValue() const
{
	return XalanDOMString();
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
	return &m_surrogate;
}



XalanNode*
ResultTreeFrag::getFirstChild() const
{
	return m_children.getLength() == 0 ? 0 : m_children.item(0);
}



XalanNode*
ResultTreeFrag::getLastChild() const
{
	const unsigned int	theLength = m_children.getLength();
	

	return theLength == 0 ? 0 : m_children.item(theLength - 1);
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
	return m_document;
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
	const unsigned int	refIndex = 0 == refChild ? m_children.getLength() :
										   m_children.indexOf(refChild);

	assert(refIndex != m_children.npos);

	m_children.insertNode(newChild, refIndex);

	return newChild;
}



XalanNode*
ResultTreeFrag::replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild)
{
	assert(newChild != 0);

	const unsigned int	refIndex =
		0 == oldChild ? m_children.npos : m_children.indexOf(oldChild);

	if(refIndex != m_children.npos)
	{
		const unsigned int	newChildIndex = m_children.indexOf(newChild);

		// Set the new child first, then erase it from
		// the old position. if it's there.
		m_children.setNode(refIndex, newChild);

		if(newChildIndex != m_children.npos)
		{
			m_children.removeNode(newChildIndex);
		}
	}

	return oldChild;
}



XalanNode*
ResultTreeFrag::appendChild(XalanNode*	newChild)
{
	assert(newChild != 0);

	m_children.addNode(newChild);
	assert(m_children.item(m_children.getLength() - 1) == newChild);

	return newChild;
}



XalanNode*
ResultTreeFrag::removeChild(XalanNode*	oldChild)
{
	m_children.removeNode(oldChild);
	assert(m_children.indexOf(oldChild) == m_children.npos);

	return oldChild;
}


bool
ResultTreeFrag::hasChildNodes() const
{
	return m_children.getLength() > 0 ? true : false;
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



XalanDOMString
ResultTreeFrag::getNamespaceURI() const
{
	return XalanDOMString();
}


XalanDOMString
ResultTreeFrag::getPrefix() const
{
	return XalanDOMString();
}


XalanDOMString
ResultTreeFrag::getLocalName() const
{
	return XalanDOMString();
}


void
ResultTreeFrag::setPrefix(const XalanDOMString&		/* prefix */)
{
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
