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
#include "XercesProcessingInstructionBridge.hpp"



#include <DOMSupport/DOMServices.hpp>



#include "XercesBridgeHelper.hpp"
#include "XercesDOMException.hpp"
#include "XercesDocumentBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesProcessingInstructionBridge::XercesProcessingInstructionBridge(
			const DOM_ProcessingInstructionType&	theXercesDOMProcessingInstruction,
			const XercesBridgeNavigator&			theNavigator) :
	XalanProcessingInstruction(),
	m_xercesNode(theXercesDOMProcessingInstruction),
	m_navigator(theNavigator)
{
}



XercesProcessingInstructionBridge::~XercesProcessingInstructionBridge()
{
}



const XalanDOMString&
XercesProcessingInstructionBridge::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesProcessingInstructionBridge::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesProcessingInstructionBridge::NodeType
XercesProcessingInstructionBridge::getNodeType() const
{
	return PROCESSING_INSTRUCTION_NODE;
}



XalanNode*
XercesProcessingInstructionBridge::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesProcessingInstructionBridge::getChildNodes() const
{
	return 0;
}



XalanNode*
XercesProcessingInstructionBridge::getFirstChild() const
{
	return 0;
}



XalanNode*
XercesProcessingInstructionBridge::getLastChild() const
{
	return 0;
}



XalanNode*
XercesProcessingInstructionBridge::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesProcessingInstructionBridge::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesProcessingInstructionBridge::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesProcessingInstructionBridge::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesProcessingInstructionBridge*
#endif
XercesProcessingInstructionBridge::cloneNode(bool	deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
	XercesProcessingInstructionBridge* const	theBridge =
		static_cast<XercesProcessingInstructionBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

	return theBridge;
#endif
}



XalanNode*
XercesProcessingInstructionBridge::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesProcessingInstructionBridge::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesProcessingInstructionBridge::removeChild(XalanNode*	/* oldChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesProcessingInstructionBridge::appendChild(XalanNode*	/* newChild */)
{
	throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

	return 0;
}



bool
XercesProcessingInstructionBridge::hasChildNodes() const
{
	return false;
}



void
XercesProcessingInstructionBridge::setNodeValue(const XalanDOMString&	nodeValue)
{
	XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesProcessingInstructionBridge::normalize()
{
	XercesBridgeHelper::normalize(m_xercesNode);
}


bool
XercesProcessingInstructionBridge::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return m_xercesNode.isSupported(
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesProcessingInstructionBridge::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesProcessingInstructionBridge::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesProcessingInstructionBridge::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesProcessingInstructionBridge::setPrefix(const XalanDOMString&	prefix)
{
	XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesProcessingInstructionBridge::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



unsigned long
XercesProcessingInstructionBridge::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesProcessingInstructionBridge::getTarget() const
{
	return m_navigator.getPooledString(m_xercesNode.getTarget());
}



const XalanDOMString&
XercesProcessingInstructionBridge::getData() const
{
	return m_navigator.getPooledString(m_xercesNode.getData());
}



void
XercesProcessingInstructionBridge::setData(const XalanDOMString&	data)
{
	XercesBridgeHelper::setNodeValue(m_xercesNode, data);
}



XALAN_CPP_NAMESPACE_END
