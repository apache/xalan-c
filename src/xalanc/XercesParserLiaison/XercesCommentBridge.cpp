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
#include "XercesCommentBridge.hpp"



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesBridgeHelper.hpp"
#include "XercesDOMException.hpp"
#include "XercesDocumentBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesCommentBridge::XercesCommentBridge(
			const DOM_CommentType&			theXercesComment,
			const XercesBridgeNavigator&	theNavigator) :
	XalanComment(),
	m_xercesNode(theXercesComment),
	m_navigator(theNavigator)
{
}



XercesCommentBridge::~XercesCommentBridge()
{
}



const XalanDOMString&
XercesCommentBridge::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesCommentBridge::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesCommentBridge::NodeType
XercesCommentBridge::getNodeType() const
{
	return COMMENT_NODE;
}



XalanNode*
XercesCommentBridge::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesCommentBridge::getChildNodes() const
{
	return 0;
}



XalanNode*
XercesCommentBridge::getFirstChild() const
{
	return 0;
}



XalanNode*
XercesCommentBridge::getLastChild() const
{
	return 0;
}



XalanNode*
XercesCommentBridge::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesCommentBridge::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesCommentBridge::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesCommentBridge::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesCommentBridge*
#endif
XercesCommentBridge::cloneNode(bool	deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
	XercesCommentBridge* const	theBridge =
		static_cast<XercesCommentBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

	return theBridge;
#endif
}



XalanNode*
XercesCommentBridge::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCommentBridge::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCommentBridge::removeChild(XalanNode*	/* oldChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCommentBridge::appendChild(XalanNode*	 /* newChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



bool
XercesCommentBridge::hasChildNodes() const
{
	return false;
}



void
XercesCommentBridge::setNodeValue(const XalanDOMString&	nodeValue)
{
	XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesCommentBridge::normalize()
{
	XercesBridgeHelper::normalize(m_xercesNode);
}


bool
XercesCommentBridge::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return m_xercesNode.isSupported(
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesCommentBridge::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesCommentBridge::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesCommentBridge::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesCommentBridge::setPrefix(const XalanDOMString&	prefix)
{
	XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesCommentBridge::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



XercesCommentBridge::IndexType
XercesCommentBridge::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesCommentBridge::getData() const
{
	return m_navigator.getPooledString(m_xercesNode.getData());
}



unsigned int
XercesCommentBridge::getLength() const
{
	return m_xercesNode.getLength();
}



XalanDOMString
XercesCommentBridge::substringData(
			unsigned int	offset, 
			unsigned int	count) const
{
	return XercesBridgeHelper::substringData(m_xercesNode, offset, count);
}



void
XercesCommentBridge::appendData(const XalanDOMString&	arg)
{
	XercesBridgeHelper::appendData(m_xercesNode, arg);
}



void
XercesCommentBridge::insertData(
			unsigned int			offset,
			const  XalanDOMString& 	arg)
{
	XercesBridgeHelper::insertData(m_xercesNode, offset, arg);
}



void
XercesCommentBridge::deleteData(
			unsigned int	offset, 
			unsigned int	count)
{
	XercesBridgeHelper::deleteData(m_xercesNode, offset, count);
}



void
XercesCommentBridge::replaceData(
			unsigned int			offset, 
			unsigned int			count, 
			const XalanDOMString&	arg)
{
	XercesBridgeHelper::replaceData(m_xercesNode, offset, count, arg);
}



XALAN_CPP_NAMESPACE_END
