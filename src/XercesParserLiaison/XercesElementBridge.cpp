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
#include "XercesElementBridge.hpp"



#include <XalanDOM/XalanAttr.hpp>



#include "XercesDOMException.hpp"
#include "XercesBridgeNavigator.hpp"
#include "XercesDocumentBridge.hpp"



XercesElementBridge::XercesElementBridge(
			const DOM_Element&				theXercesElement,
			const XercesBridgeNavigator&	theNavigator) :
	XalanElement(),
	m_xercesNode(theXercesElement),
	m_navigator(theNavigator),
	m_children(theXercesElement.getChildNodes(),
			   theNavigator),
	m_attributes(theXercesElement.getAttributes(),
				 theNavigator),
	m_cachedNodeLists(theXercesElement,
					  theNavigator)
{
}



XercesElementBridge::~XercesElementBridge()
{
}



XalanDOMString
XercesElementBridge::getNodeName() const
{
	return m_xercesNode.getNodeName();
}



XalanDOMString
XercesElementBridge::getNodeValue() const
{
	return m_xercesNode.getNodeValue();
}



XercesElementBridge::NodeType
XercesElementBridge::getNodeType() const
{
	return ELEMENT_NODE;
}



XalanNode*
XercesElementBridge::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesElementBridge::getChildNodes() const
{
	return &m_children;
}



XalanNode*
XercesElementBridge::getFirstChild() const
{
	return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesElementBridge::getLastChild() const
{
	return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesElementBridge::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesElementBridge::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesElementBridge::getAttributes() const
{
	return &m_attributes;
}



XalanDocument*
XercesElementBridge::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanElementBridge*
#endif
XercesElementBridge::cloneNode(bool deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
	XalanElementBridge* const	theBridge =
		static_cast<XalanElementBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

	return theBridge;
#endif
}



XalanNode*
XercesElementBridge::insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild)
{
	return m_navigator.insertBefore(m_xercesNode, newChild, refChild);
}



XalanNode*
XercesElementBridge::replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild)
{
	return m_navigator.replaceChild(m_xercesNode, newChild, oldChild);
}



XalanNode*
XercesElementBridge::removeChild(XalanNode*	oldChild)
{
	return m_navigator.removeChild(m_xercesNode, oldChild);
}



XalanNode*
XercesElementBridge::appendChild(XalanNode*	newChild)
{
	return m_navigator.appendChild(m_xercesNode, newChild);
}



bool
XercesElementBridge::hasChildNodes() const
{
	return m_xercesNode.hasChildNodes();
}



void
XercesElementBridge::setNodeValue(const XalanDOMString&		nodeValue)
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
XercesElementBridge::normalize()
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
XercesElementBridge::supports(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return m_xercesNode.supports(feature, version);
}



XalanDOMString
XercesElementBridge::getNamespaceURI() const
{
	return m_xercesNode.getNamespaceURI();
}



XalanDOMString
XercesElementBridge::getPrefix() const
{
	return m_xercesNode.getPrefix();
}



XalanDOMString
XercesElementBridge::getLocalName() const
{
	return m_xercesNode.getLocalName();
}



void
XercesElementBridge::setPrefix(const XalanDOMString&	prefix)
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
XercesElementBridge::getTagName() const
{
	return m_xercesNode.getTagName();
}



XalanDOMString
XercesElementBridge::getAttribute(const XalanDOMString&	name) const
{
	return m_xercesNode.getAttribute(name);
}



XalanAttr*
XercesElementBridge::getAttributeNode(const XalanDOMString&	name) const
{
	return static_cast<XalanAttr*>(m_attributes.getNamedItem(name));
}



XalanNodeList*
XercesElementBridge::getElementsByTagName(const XalanDOMString&		name) const
{
	return m_cachedNodeLists.getElementsByTagName(name);
}



void
XercesElementBridge::setAttribute(
			const XalanDOMString&	name, 
			const XalanDOMString&	value)
{
	try
	{
		m_xercesNode.setAttribute(name, value);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



XalanAttr*
XercesElementBridge::setAttributeNode(XalanAttr*	newAttr)
{
	assert(newAttr != 0);

	const DOM_Attr	theXercesAttrNode =
			m_navigator.mapNode(newAttr);
	assert(theXercesAttrNode.isNull() == false);

	XalanAttr*	theXalanAttrResult = 0;

	try
	{
		const DOM_Attr	theXercesAttrResult =
			m_xercesNode.setAttributeNode(theXercesAttrNode);

		theXalanAttrResult = m_navigator.mapNode(theXercesAttrResult);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theXalanAttrResult;
}



XalanAttr*
XercesElementBridge::removeAttributeNode(XalanAttr*		oldAttr)
{
	assert(oldAttr != 0);

	const DOM_Attr	theXercesAttrNode =
			m_navigator.mapNode(oldAttr);
	assert(theXercesAttrNode.isNull() == false);

	XalanAttr*	theXalanAttrResult = 0;

	try
	{
		const DOM_Attr	theXercesAttrResult =
			m_xercesNode.removeAttributeNode(theXercesAttrNode);

		theXalanAttrResult = m_navigator.mapNode(theXercesAttrResult);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theXalanAttrResult;
}



void
XercesElementBridge::removeAttribute(const XalanDOMString&	name)
{
	try
	{
		m_xercesNode.removeAttribute(name);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



XalanDOMString
XercesElementBridge::getAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	return m_xercesNode.getAttributeNS(namespaceURI, localName);
}



void
XercesElementBridge::setAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	qualifiedName,
			const XalanDOMString&	value)
{
	try
	{
		m_xercesNode.setAttributeNS(namespaceURI, qualifiedName, value);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
XercesElementBridge::removeAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName)
{
	try
	{
		m_xercesNode.removeAttributeNS(namespaceURI, localName);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



XalanAttr*
XercesElementBridge::getAttributeNodeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	const DOM_Attr	theAttrNode =
		m_xercesNode.getAttributeNodeNS(namespaceURI, localName);

	return m_navigator.mapNode(theAttrNode);
}



XalanAttr*
XercesElementBridge::setAttributeNodeNS(XalanAttr*	newAttr)
{
	assert(newAttr != 0);


	const DOM_Attr	theXercesAttrNode =
			m_navigator.mapNode(newAttr);
	assert(theXercesAttrNode.isNull() == false);

	XalanAttr*	theXalanAttrResult = 0;

	try
	{
		const DOM_Attr	theXercesAttrResult =
			m_xercesNode.setAttributeNodeNS(theXercesAttrNode);

		theXalanAttrResult = m_navigator.mapNode(theXercesAttrResult);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theXalanAttrResult;
}



XalanNodeList*
XercesElementBridge::getElementsByTagNameNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	return m_cachedNodeLists.getElementsByTagNameNS(namespaceURI, localName);
}
