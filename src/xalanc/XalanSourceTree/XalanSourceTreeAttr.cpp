/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#include "XalanSourceTreeAttr.hpp"



#include <cassert>



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include "XalanSourceTreeElement.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;



XalanSourceTreeAttr::XalanSourceTreeAttr(
			const XalanDOMString&		theName,
			const XalanDOMString&		theValue,
			XalanSourceTreeElement*		theOwnerElement,
			IndexType					theIndex) :
	XalanAttr(),
	m_name(theName),
	m_value(theValue),
	m_ownerElement(theOwnerElement),
	m_index(theIndex)
{
}



XalanSourceTreeAttr::~XalanSourceTreeAttr()
{
}



XalanSourceTreeAttr::XalanSourceTreeAttr(
			const XalanSourceTreeAttr&		theSource,
			bool							/* deep */) :
	XalanAttr(theSource),
	m_name(theSource.m_name),
	m_value(theSource.m_value),
	m_ownerElement(0),
	m_index(0)
{
	// Deep copy is the same as shallow copy -- The owning element
	// must deal with the hookup of the siblings.
}



const XalanDOMString&
XalanSourceTreeAttr::getNodeName() const
{
	return m_name;
}



const XalanDOMString&
XalanSourceTreeAttr::getNodeValue() const
{
	return m_value;
}



XalanSourceTreeAttr::NodeType
XalanSourceTreeAttr::getNodeType() const
{
	return ATTRIBUTE_NODE;
}



XalanNode*
XalanSourceTreeAttr::getParentNode() const
{
	return 0;
}



const XalanNodeList*
XalanSourceTreeAttr::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeAttr::getFirstChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeAttr::getLastChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeAttr::getPreviousSibling() const
{
	return 0;
}



XalanNode*
XalanSourceTreeAttr::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
XalanSourceTreeAttr::getAttributes() const
{
	return 0;
}



XalanDocument*
XalanSourceTreeAttr::getOwnerDocument() const
{
	assert(m_ownerElement != 0);

	return m_ownerElement->getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeAttr*
#endif
XalanSourceTreeAttr::cloneNode(bool		/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeAttr::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeAttr::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeAttr::removeChild(XalanNode*		/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeAttr::appendChild(XalanNode*		/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XalanSourceTreeAttr::hasChildNodes() const
{
	return false;
}



void
XalanSourceTreeAttr::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeAttr::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeAttr::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeAttr::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeAttr::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeAttr::getLocalName() const
{
	return s_emptyString;
}



void
XalanSourceTreeAttr::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeAttr::isIndexed() const
{
	return true;
}



XalanSourceTreeAttr::IndexType
XalanSourceTreeAttr::getIndex() const
{
	return m_index;
}



const XalanDOMString&
XalanSourceTreeAttr::getName() const
{
	return m_name;
}



bool
XalanSourceTreeAttr::getSpecified() const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeAttr::getValue() const
{
	return m_value;
}



void
XalanSourceTreeAttr::setValue(const XalanDOMString&		/* value */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanElement*
XalanSourceTreeAttr::getOwnerElement() const
{
	return m_ownerElement;
}



XALAN_CPP_NAMESPACE_END
