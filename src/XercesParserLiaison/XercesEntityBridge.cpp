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
#include "XercesEntityBridge.hpp"



#include "XercesBridgeNavigator.hpp"
#include "XercesDOMException.hpp"
#include "XercesDocumentBridge.hpp"



XercesEntityBridge::XercesEntityBridge(
			const DOM_Entity&				theXercesDOMEntity,
			const XercesBridgeNavigator&	theNavigator) :
	XalanEntity(),
	m_xercesNode(theXercesDOMEntity),
	m_navigator(theNavigator),
	m_children(m_xercesNode.getChildNodes(),
			   theNavigator)
{
}



XercesEntityBridge::~XercesEntityBridge()
{
}



XalanDOMString
XercesEntityBridge::getNodeName() const
{
	return m_xercesNode.getNodeName();
}



XalanDOMString
XercesEntityBridge::getNodeValue() const
{
	return m_xercesNode.getNodeValue();
}



XercesEntityBridge::NodeType
XercesEntityBridge::getNodeType() const
{
	return ENTITY_NODE;
}



XalanNode*
XercesEntityBridge::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesEntityBridge::getChildNodes() const
{
	return &m_children;
}



XalanNode*
XercesEntityBridge::getFirstChild() const
{
	return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesEntityBridge::getLastChild() const
{
	return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesEntityBridge::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesEntityBridge::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesEntityBridge::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesEntityBridge::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesEntityBridge*
#endif
XercesEntityBridge::cloneNode(bool	deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
	XercesEntityBridge* const	theBridge =
		static_cast<XercesEntityBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

	return theBridge;
#endif
}



XalanNode*
XercesEntityBridge::insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild)
{
	return m_navigator.insertBefore(m_xercesNode, newChild, refChild);
}



XalanNode*
XercesEntityBridge::replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild)
{
	return m_navigator.replaceChild(m_xercesNode, newChild, oldChild);
}



XalanNode*
XercesEntityBridge::removeChild(XalanNode*	oldChild)
{
	return m_navigator.removeChild(m_xercesNode, oldChild);
}



XalanNode*
XercesEntityBridge::appendChild(XalanNode*	newChild)
{
	return m_navigator.appendChild(m_xercesNode, newChild);
}



bool
XercesEntityBridge::hasChildNodes() const
{
	return m_xercesNode.hasChildNodes();
}



void
XercesEntityBridge::setNodeValue(const XalanDOMString&	nodeValue)
{
	try
	{
		m_xercesNode.setNodeValue(nodeValue);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
XercesEntityBridge::normalize()
{
	try
	{
		m_xercesNode.normalize();
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}


bool
XercesEntityBridge::supports(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return m_xercesNode.supports(feature, version);
}



XalanDOMString
XercesEntityBridge::getNamespaceURI() const
{
	return m_xercesNode.getNamespaceURI();
}



XalanDOMString
XercesEntityBridge::getPrefix() const
{
	return m_xercesNode.getPrefix();
}



XalanDOMString
XercesEntityBridge::getLocalName() const
{
	return m_xercesNode.getLocalName();
}



void
XercesEntityBridge::setPrefix(const XalanDOMString&		prefix)
{
	try
	{
		m_xercesNode.setPrefix(prefix);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



XalanDOMString
XercesEntityBridge::getPublicId() const
{
	return m_xercesNode.getPublicId();
}



XalanDOMString
XercesEntityBridge::getSystemId() const
{
	return m_xercesNode.getSystemId();
}



XalanDOMString
XercesEntityBridge::getNotationName() const
{
	return m_xercesNode.getNotationName();
}
