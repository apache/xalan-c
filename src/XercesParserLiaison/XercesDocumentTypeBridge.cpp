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
#include "XercesDocumentTypeBridge.hpp"



#include <DOMSupport/DOMServices.hpp>



#include "XercesBridgeHelper.hpp"
#include "XercesDOMException.hpp"
#include "XercesDocumentBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesDocumentTypeBridge::XercesDocumentTypeBridge(
			const DOM_DocumentType_Type&	theXercesDOMDocumentType,
			const XercesBridgeNavigator&	theNavigator) :
	XalanDocumentType(),
	m_xercesNode(theXercesDOMDocumentType),
	m_entities(m_xercesNode.getEntities(),
			   theNavigator),
	m_notations(m_xercesNode.getNotations(),
			    theNavigator),
	m_navigator(theNavigator)
{
}



XercesDocumentTypeBridge::~XercesDocumentTypeBridge()
{
}



const XalanDOMString&
XercesDocumentTypeBridge::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesDocumentTypeBridge::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesDocumentTypeBridge::NodeType
XercesDocumentTypeBridge::getNodeType() const
{
	return DOCUMENT_TYPE_NODE;
}



XalanNode*
XercesDocumentTypeBridge::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesDocumentTypeBridge::getChildNodes() const
{
	return 0;
}



XalanNode*
XercesDocumentTypeBridge::getFirstChild() const
{
	return 0;
}



XalanNode*
XercesDocumentTypeBridge::getLastChild() const
{
	return 0;
}



XalanNode*
XercesDocumentTypeBridge::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesDocumentTypeBridge::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesDocumentTypeBridge::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesDocumentTypeBridge::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesDocumentTypeBridge*
#endif
XercesDocumentTypeBridge::cloneNode(bool	deep) const
{
	// $$$ ToDo: Fix this!! XercesDocumentType nodes can't be cloned yet...
	assert(false);

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
	XercesDocumentTypeBridge* const		theBridge =
		static_cast<XercesDocumentTypeBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

	return theBridge;
#endif
}



XalanNode*
XercesDocumentTypeBridge::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesDocumentTypeBridge::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesDocumentTypeBridge::removeChild(XalanNode*	/* oldChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesDocumentTypeBridge::appendChild(XalanNode*	 /* newChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



bool
XercesDocumentTypeBridge::hasChildNodes() const
{
	return false;
}



void
XercesDocumentTypeBridge::setNodeValue(const XalanDOMString&	nodeValue)
{
	XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesDocumentTypeBridge::normalize()
{
	XercesBridgeHelper::normalize(m_xercesNode);
}



bool
XercesDocumentTypeBridge::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return m_xercesNode.isSupported(
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesDocumentTypeBridge::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesDocumentTypeBridge::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesDocumentTypeBridge::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesDocumentTypeBridge::setPrefix(const XalanDOMString&	prefix)
{
	XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesDocumentTypeBridge::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



XercesDocumentTypeBridge::IndexType
XercesDocumentTypeBridge::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesDocumentTypeBridge::getName() const
{
	return m_navigator.getPooledString(m_xercesNode.getName());
}



const XalanNamedNodeMap*
XercesDocumentTypeBridge::getEntities() const
{
	return &m_entities;
}



const XalanNamedNodeMap*
XercesDocumentTypeBridge::getNotations() const
{
	return &m_notations;
}



const XalanDOMString&
XercesDocumentTypeBridge::getPublicId() const
{
	return m_navigator.getPooledString(m_xercesNode.getPublicId());
}



const XalanDOMString&
XercesDocumentTypeBridge::getSystemId() const
{
	return m_navigator.getPooledString(m_xercesNode.getSystemId());
}



const XalanDOMString&
XercesDocumentTypeBridge::getInternalSubset() const
{
	return m_navigator.getPooledString(m_xercesNode.getInternalSubset());
}



XALAN_CPP_NAMESPACE_END
