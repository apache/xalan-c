/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#include "XercesCDATASectionBridge.hpp"



#include <cassert>



#include <DOMSupport/DOMServices.hpp>



#include "XercesBridgeHelper.hpp"
#include "XercesDOMException.hpp"
#include "XercesDocumentBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesCDATASectionBridge::XercesCDATASectionBridge(
			const DOM_CDATASectionType&		theXercesCDATASection,
			const XercesBridgeNavigator&	theNavigator) :
	XalanCDATASection(),
	m_xercesNode(theXercesCDATASection),
	m_navigator(theNavigator)
{
}



XercesCDATASectionBridge::~XercesCDATASectionBridge()
{
}



const XalanDOMString&
XercesCDATASectionBridge::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesCDATASectionBridge::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesCDATASectionBridge::NodeType
XercesCDATASectionBridge::getNodeType() const
{
	return CDATA_SECTION_NODE;
}



XalanNode*
XercesCDATASectionBridge::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesCDATASectionBridge::getChildNodes() const
{
	return 0;
}



XalanNode*
XercesCDATASectionBridge::getFirstChild() const
{
	return 0;
}



XalanNode*
XercesCDATASectionBridge::getLastChild() const
{
	return 0;
}



XalanNode*
XercesCDATASectionBridge::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesCDATASectionBridge::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesCDATASectionBridge::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesCDATASectionBridge::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesCDATASectionBridge*
#endif
XercesCDATASectionBridge::cloneNode(bool	deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
	XercesCDATASectionBridge* const	theBridge =
		static_cast<XercesCDATASectionBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

	return theBridge;
#endif
}



XalanNode*
XercesCDATASectionBridge::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCDATASectionBridge::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCDATASectionBridge::removeChild(XalanNode*	/* oldChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCDATASectionBridge::appendChild(XalanNode*	 /* newChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



bool
XercesCDATASectionBridge::hasChildNodes() const
{
	return false;
}



void
XercesCDATASectionBridge::setNodeValue(const XalanDOMString&	nodeValue)
{
	XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesCDATASectionBridge::normalize()
{
	XercesBridgeHelper::normalize(m_xercesNode);
}



bool
XercesCDATASectionBridge::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return m_xercesNode.isSupported(
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesCDATASectionBridge::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesCDATASectionBridge::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesCDATASectionBridge::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesCDATASectionBridge::setPrefix(const XalanDOMString&	prefix)
{
	XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}




bool
XercesCDATASectionBridge::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



XercesCDATASectionBridge::IndexType
XercesCDATASectionBridge::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesCDATASectionBridge::getData() const
{
	return m_navigator.getPooledString(m_xercesNode.getData());
}



unsigned int
XercesCDATASectionBridge::getLength() const
{
	return m_xercesNode.getLength();
}



XalanDOMString
XercesCDATASectionBridge::substringData(
			unsigned int	offset, 
			unsigned int	count) const
{
	return XercesBridgeHelper::substringData(m_xercesNode, offset, count);
}



void
XercesCDATASectionBridge::appendData(const XalanDOMString&	arg)
{
	XercesBridgeHelper::appendData(m_xercesNode, arg);
}



void
XercesCDATASectionBridge::insertData(
			unsigned int			offset,
			const  XalanDOMString& 	arg)
{
	XercesBridgeHelper::insertData(m_xercesNode, offset, arg);
}



void
XercesCDATASectionBridge::deleteData(
			unsigned int	offset, 
			unsigned int	count)
{
	XercesBridgeHelper::deleteData(m_xercesNode, offset, count);
}



void
XercesCDATASectionBridge::replaceData(
			unsigned int			offset, 
			unsigned int			count, 
			const XalanDOMString&	arg)
{
	XercesBridgeHelper::replaceData(m_xercesNode, offset, count, arg);
}



XalanText*
XercesCDATASectionBridge::splitText(unsigned int	offset)
{
	return m_navigator.splitText(m_xercesNode, offset);
}



bool
XercesCDATASectionBridge::isIgnorableWhitespace() const
{
	return isXMLWhitespace(getData());
}



XALAN_CPP_NAMESPACE_END
