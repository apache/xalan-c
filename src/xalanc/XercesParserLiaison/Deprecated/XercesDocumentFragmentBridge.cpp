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
#include "XercesDocumentFragmentBridge.hpp"



#include <cassert>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesBridgeHelper.hpp"
#include "XercesDocumentBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesDocumentFragmentBridge::XercesDocumentFragmentBridge(
			const DOM_DocumentFragmentType&		theXercesDOMDocumentFragment,
			const XercesBridgeNavigator&		theNavigator) :
	XalanDocumentFragment(),
	m_xercesNode(theXercesDOMDocumentFragment),
	m_children(theXercesDOMDocumentFragment.getChildNodes(),
			   theNavigator),
	m_navigator(theNavigator)
{
}



XercesDocumentFragmentBridge::~XercesDocumentFragmentBridge()
{
}



const XalanDOMString&
XercesDocumentFragmentBridge::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesDocumentFragmentBridge::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesDocumentFragmentBridge::NodeType
XercesDocumentFragmentBridge::getNodeType() const
{
	return DOCUMENT_FRAGMENT_NODE;
}



XalanNode*
XercesDocumentFragmentBridge::getParentNode() const
{
	return 0;
}



const XalanNodeList*
XercesDocumentFragmentBridge::getChildNodes() const
{
	return &m_children;
}



XalanNode*
XercesDocumentFragmentBridge::getFirstChild() const
{
	return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesDocumentFragmentBridge::getLastChild() const
{
	return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesDocumentFragmentBridge::getPreviousSibling() const
{
	return 0;
}



XalanNode*
XercesDocumentFragmentBridge::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
XercesDocumentFragmentBridge::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesDocumentFragmentBridge::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesDocumentFragmentBridge*
#endif
XercesDocumentFragmentBridge::cloneNode(bool	deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
	XercesDocumentFragmentBridge* const	theBridge =
		static_cast<XercesDocumentFragmentBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

	return theBridge;
#endif
}



XalanNode*
XercesDocumentFragmentBridge::insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild)
{
	return m_navigator.insertBefore(m_xercesNode, newChild, refChild);
}



XalanNode*
XercesDocumentFragmentBridge::replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild)
{
	return m_navigator.replaceChild(m_xercesNode, newChild, oldChild);
}



XalanNode*
XercesDocumentFragmentBridge::removeChild(XalanNode*	oldChild)
{
	return m_navigator.removeChild(m_xercesNode, oldChild);
}



XalanNode*
XercesDocumentFragmentBridge::appendChild(XalanNode*	newChild)
{
	return m_navigator.appendChild(m_xercesNode, newChild);
}



bool
XercesDocumentFragmentBridge::hasChildNodes() const
{
	return m_xercesNode.hasChildNodes();
}



void
XercesDocumentFragmentBridge::setNodeValue(const XalanDOMString&	nodeValue)
{
	XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesDocumentFragmentBridge::normalize()
{
	XercesBridgeHelper::normalize(m_xercesNode);
}


bool
XercesDocumentFragmentBridge::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return m_xercesNode.isSupported(
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesDocumentFragmentBridge::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesDocumentFragmentBridge::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesDocumentFragmentBridge::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesDocumentFragmentBridge::setPrefix(const XalanDOMString&	prefix)
{
	XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesDocumentFragmentBridge::isIndexed() const
{
	// $$$ ToDo: Are document fragments ever indexed?
	// I don't think so...
	return false;
}



XercesDocumentFragmentBridge::IndexType
XercesDocumentFragmentBridge::getIndex() const
{
	return 0;
}



XALAN_CPP_NAMESPACE_END
