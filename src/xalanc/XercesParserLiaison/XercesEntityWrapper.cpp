/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "XercesEntityWrapper.hpp"



#include <xercesc/dom/DOMEntity.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesDOMWrapperException.hpp"
#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesEntityWrapper::XercesEntityWrapper(
			const DOMEntityType*			theXercesDOMEntity,
			const XercesWrapperNavigator&	theNavigator) :
	XalanEntity(),
	m_xercesNode(theXercesDOMEntity),
	m_navigator(theNavigator),
	m_children(m_xercesNode->getChildNodes(),
			   theNavigator)
{
	assert(theXercesDOMEntity != 0);
}



XercesEntityWrapper::~XercesEntityWrapper()
{
}



const XalanDOMString&
XercesEntityWrapper::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesEntityWrapper::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesEntityWrapper::NodeType
XercesEntityWrapper::getNodeType() const
{
	return ENTITY_NODE;
}



XalanNode*
XercesEntityWrapper::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesEntityWrapper::getChildNodes() const
{
	return &m_children;
}



XalanNode*
XercesEntityWrapper::getFirstChild() const
{
	return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesEntityWrapper::getLastChild() const
{
	return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesEntityWrapper::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesEntityWrapper::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesEntityWrapper::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesEntityWrapper::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesEntityWrapper*
#endif
XercesEntityWrapper::cloneNode(bool		/* deep */) const
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XercesEntityWrapper::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesEntityWrapper::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesEntityWrapper::removeChild(XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesEntityWrapper::appendChild(XalanNode*	/* newChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



bool
XercesEntityWrapper::hasChildNodes() const
{
	return m_xercesNode->hasChildNodes();
}



void
XercesEntityWrapper::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesEntityWrapper::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesEntityWrapper::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return XercesWrapperHelper::isSupported(m_xercesNode, feature, version);
}



const XalanDOMString&
XercesEntityWrapper::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesEntityWrapper::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesEntityWrapper::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



void
XercesEntityWrapper::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesEntityWrapper::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



XercesEntityWrapper::IndexType
XercesEntityWrapper::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesEntityWrapper::getPublicId() const
{
	return m_navigator.getPooledString(m_xercesNode->getPublicId());
}



const XalanDOMString&
XercesEntityWrapper::getSystemId() const
{
	return m_navigator.getPooledString(m_xercesNode->getSystemId());
}



const XalanDOMString&
XercesEntityWrapper::getNotationName() const
{
	return m_navigator.getPooledString(m_xercesNode->getNotationName());
}



XALAN_CPP_NAMESPACE_END
