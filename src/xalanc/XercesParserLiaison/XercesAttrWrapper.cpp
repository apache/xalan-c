/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#include "XercesAttrWrapper.hpp"



#include <cassert>



#include <xercesc/dom/DOMAttr.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesAttrWrapper::XercesAttrWrapper(
			const DOMAttrType*				theXercesAttr,
			const XercesWrapperNavigator&	theNavigator) :
	XalanAttr(),
	m_xercesNode(theXercesAttr),
	m_children(theXercesAttr->getChildNodes(),
			   theNavigator),
	m_navigator(theNavigator)
{
	assert(theXercesAttr != 0);
}



XercesAttrWrapper::~XercesAttrWrapper()
{
}



const XalanDOMString&
XercesAttrWrapper::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesAttrWrapper::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesAttrWrapper::NodeType
XercesAttrWrapper::getNodeType() const
{
	return ATTRIBUTE_NODE;
}



XalanNode*
XercesAttrWrapper::getParentNode() const
{
	return 0;
}



const XalanNodeList*
XercesAttrWrapper::getChildNodes() const
{
	return &m_children;
}



XalanNode*
XercesAttrWrapper::getFirstChild() const
{
	return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesAttrWrapper::getLastChild() const
{
	return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesAttrWrapper::getPreviousSibling() const
{
	return 0;
}



XalanNode*
XercesAttrWrapper::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
XercesAttrWrapper::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesAttrWrapper::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesAttrWrapper*
#endif
XercesAttrWrapper::cloneNode(bool	/* deep */) const
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XercesAttrWrapper::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XercesAttrWrapper::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XercesAttrWrapper::removeChild(XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XercesAttrWrapper::appendChild(XalanNode*	/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XercesAttrWrapper::hasChildNodes() const
{
	return m_xercesNode->hasChildNodes();
}



void
XercesAttrWrapper::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesAttrWrapper::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesAttrWrapper::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return XercesWrapperHelper::isSupported(m_xercesNode, feature, version);
}



const XalanDOMString&
XercesAttrWrapper::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesAttrWrapper::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesAttrWrapper::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



void
XercesAttrWrapper::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesAttrWrapper::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



XercesAttrWrapper::IndexType
XercesAttrWrapper::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesAttrWrapper::getName() const
{
	return m_navigator.getPooledString(m_xercesNode->getName());
}



bool
XercesAttrWrapper::getSpecified() const
{
	return m_xercesNode->getSpecified();
}



const XalanDOMString&
XercesAttrWrapper::getValue() const
{
	return m_navigator.getPooledString(m_xercesNode->getValue());
}



void
XercesAttrWrapper::setValue(const XalanDOMString&	/* value */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanElement*
XercesAttrWrapper::getOwnerElement() const
{
	return m_navigator.getOwnerElement(m_xercesNode);
}



XALAN_CPP_NAMESPACE_END
