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
#include "XercesAttrBridge.hpp"



#include <cassert>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesBridgeHelper.hpp"
#include "XercesDOMException.hpp"
#include "XercesDocumentBridge.hpp"
#include "XercesElementBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesAttrBridge::XercesAttrBridge(
			const DOM_AttrType&				theXercesAttr,
			const XercesBridgeNavigator&	theNavigator) :
	XalanAttr(),
	m_xercesNode(theXercesAttr),
	m_children(theXercesAttr.getChildNodes(),
			   theNavigator),
	m_navigator(theNavigator)
{
}



XercesAttrBridge::~XercesAttrBridge()
{
}



const XalanDOMString&
XercesAttrBridge::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesAttrBridge::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesAttrBridge::NodeType
XercesAttrBridge::getNodeType() const
{
	return ATTRIBUTE_NODE;
}



XalanNode*
XercesAttrBridge::getParentNode() const
{
	return 0;
}



const XalanNodeList*
XercesAttrBridge::getChildNodes() const
{
	return &m_children;
}



XalanNode*
XercesAttrBridge::getFirstChild() const
{
	return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesAttrBridge::getLastChild() const
{
	return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesAttrBridge::getPreviousSibling() const
{
	return 0;
}



XalanNode*
XercesAttrBridge::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
XercesAttrBridge::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesAttrBridge::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesAttrBridge*
#endif
XercesAttrBridge::cloneNode(bool	deep) const
{
	assert(m_navigator.getOwnerDocument() != 0);

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
	XercesAttrBridge* const		theBridge =
		static_cast<XercesAttrBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

	return theBridge;
#endif
}



XalanNode*
XercesAttrBridge::insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild)
{
	return m_navigator.insertBefore(m_xercesNode, newChild, refChild);
}



XalanNode*
XercesAttrBridge::replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild)
{
	return m_navigator.replaceChild(m_xercesNode, newChild, oldChild);
}



XalanNode*
XercesAttrBridge::removeChild(XalanNode*	oldChild)
{
	return m_navigator.removeChild(m_xercesNode, oldChild);
}



XalanNode*
XercesAttrBridge::appendChild(XalanNode*	newChild)
{
	return m_navigator.appendChild(m_xercesNode, newChild);
}



bool
XercesAttrBridge::hasChildNodes() const
{
	return m_xercesNode.hasChildNodes();
}



void
XercesAttrBridge::setNodeValue(const XalanDOMString&	nodeValue)
{
	XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesAttrBridge::normalize()
{
	XercesBridgeHelper::normalize(m_xercesNode);
}



bool
XercesAttrBridge::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return m_xercesNode.isSupported(
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesAttrBridge::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesAttrBridge::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesAttrBridge::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesAttrBridge::setPrefix(const XalanDOMString&	prefix)
{
	XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesAttrBridge::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



XercesAttrBridge::IndexType
XercesAttrBridge::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesAttrBridge::getName() const
{
	return m_navigator.getPooledString(m_xercesNode.getName());
}



bool
XercesAttrBridge::getSpecified() const
{
	return m_xercesNode.getSpecified();
}



const XalanDOMString&
XercesAttrBridge::getValue() const
{
	return m_navigator.getPooledString(m_xercesNode.getValue());
}



void
XercesAttrBridge::setValue(const XalanDOMString&	value)
{
	XercesBridgeHelper::setNodeValue(m_xercesNode, value);
}



XalanElement*
XercesAttrBridge::getOwnerElement() const
{
	return m_navigator.getOwnerElement(m_xercesNode);
}



XALAN_CPP_NAMESPACE_END
