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
#include "XercesNotationBridge.hpp"



#include <DOMSupport/DOMServices.hpp>



#include "XercesBridgeHelper.hpp"
#include "XercesDocumentBridge.hpp"



XercesNotationBridge::XercesNotationBridge(
			const DOM_Notation&				theXercesDOMNotation,
			const XercesBridgeNavigator&	theNavigator) :
	XalanNotation(),
	m_xercesNode(theXercesDOMNotation),
	m_navigator(theNavigator)
{
}



XercesNotationBridge::~XercesNotationBridge()
{
}



const XalanDOMString&
XercesNotationBridge::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesNotationBridge::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesNotationBridge::NodeType
XercesNotationBridge::getNodeType() const
{
	return NOTATION_NODE;
}



XalanNode*
XercesNotationBridge::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesNotationBridge::getChildNodes() const
{
	return 0;
}



XalanNode*
XercesNotationBridge::getFirstChild() const
{
	return 0;
}



XalanNode*
XercesNotationBridge::getLastChild() const
{
	return 0;
}



XalanNode*
XercesNotationBridge::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesNotationBridge::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesNotationBridge::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesNotationBridge::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesNotationBridge*
#endif
XercesNotationBridge::cloneNode(bool	deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
	XercesNotationBridge* const		theBridge =
		static_cast<XercesNotationBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

	return theBridge;
#endif
}



XalanNode*
XercesNotationBridge::insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild)
{
	return m_navigator.insertBefore(m_xercesNode, newChild, refChild);
}



XalanNode*
XercesNotationBridge::replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild)
{
	return m_navigator.replaceChild(m_xercesNode, newChild, oldChild);
}



XalanNode*
XercesNotationBridge::removeChild(XalanNode*	oldChild)
{
	return m_navigator.removeChild(m_xercesNode, oldChild);
}



XalanNode*
XercesNotationBridge::appendChild(XalanNode*	newChild)
{
	return m_navigator.appendChild(m_xercesNode, newChild);
}



bool
XercesNotationBridge::hasChildNodes() const
{
	return false;
}



void
XercesNotationBridge::setNodeValue(const XalanDOMString&	nodeValue)
{
	XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesNotationBridge::normalize()
{
	XercesBridgeHelper::normalize(m_xercesNode);
}


bool
XercesNotationBridge::supports(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return m_xercesNode.supports(
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesNotationBridge::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesNotationBridge::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesNotationBridge::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesNotationBridge::setPrefix(const XalanDOMString&	prefix)
{
	XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesNotationBridge::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



unsigned long
XercesNotationBridge::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesNotationBridge::getPublicId() const
{
	return m_navigator.getPooledString(m_xercesNode.getPublicId());
}



const XalanDOMString&
XercesNotationBridge::getSystemId() const
{
	return m_navigator.getPooledString(m_xercesNode.getSystemId());
}
