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
#include "XercesDocumentTypeWrapper.hpp"



#include <xercesc/dom/DOMDocumentType.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesDocumentTypeWrapper::XercesDocumentTypeWrapper(
			const DOMDocumentType_Type*		theXercesDOMDocumentType,
			const XercesWrapperNavigator&	theNavigator) :
	XalanDocumentType(),
	m_xercesNode(theXercesDOMDocumentType),
	m_entities(m_xercesNode->getEntities(),
			   theNavigator),
	m_notations(m_xercesNode->getNotations(),
			    theNavigator),
	m_navigator(theNavigator)
{
	assert(theXercesDOMDocumentType != 0);
}



XercesDocumentTypeWrapper::~XercesDocumentTypeWrapper()
{
}



const XalanDOMString&
XercesDocumentTypeWrapper::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesDocumentTypeWrapper::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesDocumentTypeWrapper::NodeType
XercesDocumentTypeWrapper::getNodeType() const
{
	return DOCUMENT_TYPE_NODE;
}



XalanNode*
XercesDocumentTypeWrapper::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesDocumentTypeWrapper::getChildNodes() const
{
	return 0;
}



XalanNode*
XercesDocumentTypeWrapper::getFirstChild() const
{
	return 0;
}



XalanNode*
XercesDocumentTypeWrapper::getLastChild() const
{
	return 0;
}



XalanNode*
XercesDocumentTypeWrapper::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesDocumentTypeWrapper::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesDocumentTypeWrapper::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesDocumentTypeWrapper::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesDocumentTypeWrapper*
#endif
XercesDocumentTypeWrapper::cloneNode(bool	/* deep */) const
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XercesDocumentTypeWrapper::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesDocumentTypeWrapper::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesDocumentTypeWrapper::removeChild(XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesDocumentTypeWrapper::appendChild(XalanNode*	 /* newChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



bool
XercesDocumentTypeWrapper::hasChildNodes() const
{
	return false;
}



void
XercesDocumentTypeWrapper::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesDocumentTypeWrapper::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesDocumentTypeWrapper::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return XercesWrapperHelper::isSupported(m_xercesNode, feature, version);
}



const XalanDOMString&
XercesDocumentTypeWrapper::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesDocumentTypeWrapper::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesDocumentTypeWrapper::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



void
XercesDocumentTypeWrapper::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesDocumentTypeWrapper::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



XercesDocumentTypeWrapper::IndexType
XercesDocumentTypeWrapper::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesDocumentTypeWrapper::getName() const
{
	return m_navigator.getPooledString(m_xercesNode->getName());
}



const XalanNamedNodeMap*
XercesDocumentTypeWrapper::getEntities() const
{
	return &m_entities;
}



const XalanNamedNodeMap*
XercesDocumentTypeWrapper::getNotations() const
{
	return &m_notations;
}



const XalanDOMString&
XercesDocumentTypeWrapper::getPublicId() const
{
	return m_navigator.getPooledString(m_xercesNode->getPublicId());
}



const XalanDOMString&
XercesDocumentTypeWrapper::getSystemId() const
{
	return m_navigator.getPooledString(m_xercesNode->getSystemId());
}



const XalanDOMString&
XercesDocumentTypeWrapper::getInternalSubset() const
{
	return m_navigator.getPooledString(m_xercesNode->getInternalSubset());
}



XALAN_CPP_NAMESPACE_END
