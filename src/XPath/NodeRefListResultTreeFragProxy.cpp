/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights 
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
#include "NodeRefListResultTreeFragProxy.hpp"



#include <XalanDOM/XalanDOMException.hpp>



#include "NodeRefListBase.hpp"



const XalanDOMString	NodeRefListResultTreeFragProxy::s_emptyString;



NodeRefListResultTreeFragProxy::NodeRefListResultTreeFragProxy(const NodeRefListBase&	theValue) :
	ResultTreeFragBase(),
	XalanNodeList(),
	m_value(theValue)
{
}



NodeRefListResultTreeFragProxy::~NodeRefListResultTreeFragProxy()
{
}



void
NodeRefListResultTreeFragProxy::clear()
{
	assert(false);
}



const XalanDOMString&
NodeRefListResultTreeFragProxy::getNodeName() const
{
	return s_emptyString;
}



const XalanDOMString&
NodeRefListResultTreeFragProxy::getNodeValue() const
{
	return s_emptyString;
}



NodeRefListResultTreeFragProxy::NodeType
NodeRefListResultTreeFragProxy::getNodeType() const
{
	return DOCUMENT_FRAGMENT_NODE;
}



XalanNode*
NodeRefListResultTreeFragProxy::getParentNode() const
{
	return 0;
}



const XalanNodeList*
NodeRefListResultTreeFragProxy::getChildNodes() const
{
	return this;
}



XalanNode*
NodeRefListResultTreeFragProxy::getFirstChild() const
{
	return m_value.getLength() == 0 ? 0 : m_value.item(0);
}



XalanNode*
NodeRefListResultTreeFragProxy::getLastChild() const
{
	const NodeRefListBase::size_type	theLength = m_value.getLength();
	
	return theLength == 0 ? 0 : m_value.item(theLength - 1);
}



XalanNode*
NodeRefListResultTreeFragProxy::getPreviousSibling() const
{
	return 0;
}



XalanNode*
NodeRefListResultTreeFragProxy::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
NodeRefListResultTreeFragProxy::getAttributes() const
{
	return 0;
}



XalanDocument*
NodeRefListResultTreeFragProxy::getOwnerDocument() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
NodeRefListResultTreeFragProxy*
#endif
NodeRefListResultTreeFragProxy::cloneNode(bool	/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
NodeRefListResultTreeFragProxy::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
NodeRefListResultTreeFragProxy::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
NodeRefListResultTreeFragProxy::appendChild(XalanNode*	/* newChild */ )
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
NodeRefListResultTreeFragProxy::removeChild(XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}


bool
NodeRefListResultTreeFragProxy::hasChildNodes() const
{
	return getLength() > 0 ? true : false;
}



void
NodeRefListResultTreeFragProxy::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
NodeRefListResultTreeFragProxy::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
NodeRefListResultTreeFragProxy::supports(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
NodeRefListResultTreeFragProxy::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
NodeRefListResultTreeFragProxy::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
NodeRefListResultTreeFragProxy::getLocalName() const
{
	return s_emptyString;
}



void
NodeRefListResultTreeFragProxy::setPrefix(const XalanDOMString&		/* prefix */)
{
}



bool
NodeRefListResultTreeFragProxy::isIndexed() const
{
	return false;
}



unsigned long
NodeRefListResultTreeFragProxy::getIndex() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
ResultTreeFragBase*
#else
NodeRefListResultTreeFragProxy*
#endif
NodeRefListResultTreeFragProxy::clone(bool	/* deep */) const
{
	assert(false);

	return 0;
}



XalanNode*
NodeRefListResultTreeFragProxy::item(unsigned int	index) const
{
	assert(index < m_value.getLength());

	return m_value.item(index);
}



unsigned int
NodeRefListResultTreeFragProxy::getLength() const
{
	assert(unsigned(m_value.getLength()) == m_value.getLength());

	return unsigned(m_value.getLength());
}
