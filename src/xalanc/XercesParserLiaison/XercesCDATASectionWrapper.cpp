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
#include "XercesCDATASectionWrapper.hpp"



#include <cassert>



#include <xercesc/dom/DOMCDATASection.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesCDATASectionWrapper::XercesCDATASectionWrapper(
			const DOMCDATASectionType*		theXercesCDATASection,
			const XercesWrapperNavigator&	theNavigator) :
	XalanCDATASection(),
	m_xercesNode(theXercesCDATASection),
	m_navigator(theNavigator)
{
	assert(theXercesCDATASection != 0);
}



XercesCDATASectionWrapper::~XercesCDATASectionWrapper()
{
}



const XalanDOMString&
XercesCDATASectionWrapper::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesCDATASectionWrapper::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesCDATASectionWrapper::NodeType
XercesCDATASectionWrapper::getNodeType() const
{
	return CDATA_SECTION_NODE;
}



XalanNode*
XercesCDATASectionWrapper::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesCDATASectionWrapper::getChildNodes() const
{
	return 0;
}



XalanNode*
XercesCDATASectionWrapper::getFirstChild() const
{
	return 0;
}



XalanNode*
XercesCDATASectionWrapper::getLastChild() const
{
	return 0;
}



XalanNode*
XercesCDATASectionWrapper::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesCDATASectionWrapper::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesCDATASectionWrapper::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesCDATASectionWrapper::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesCDATASectionWrapper*
#endif
XercesCDATASectionWrapper::cloneNode(bool	/* deep */) const
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XercesCDATASectionWrapper::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCDATASectionWrapper::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCDATASectionWrapper::removeChild(XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCDATASectionWrapper::appendChild(XalanNode*	 /* newChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



bool
XercesCDATASectionWrapper::hasChildNodes() const
{
	return false;
}



void
XercesCDATASectionWrapper::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesCDATASectionWrapper::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesCDATASectionWrapper::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return XercesWrapperHelper::isSupported(m_xercesNode, feature, version);
}



const XalanDOMString&
XercesCDATASectionWrapper::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesCDATASectionWrapper::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesCDATASectionWrapper::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



void
XercesCDATASectionWrapper::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesCDATASectionWrapper::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



XercesCDATASectionWrapper::IndexType
XercesCDATASectionWrapper::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesCDATASectionWrapper::getData() const
{
	return m_navigator.getPooledString(m_xercesNode->getData());
}



unsigned int
XercesCDATASectionWrapper::getLength() const
{
	return m_xercesNode->getLength();
}



XalanDOMString
XercesCDATASectionWrapper::substringData(
			unsigned int	offset, 
			unsigned int	count) const
{
	return XercesWrapperHelper::substringData(m_xercesNode, offset, count);
}



void
XercesCDATASectionWrapper::appendData(const XalanDOMString&		/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesCDATASectionWrapper::insertData(
			unsigned int			/* offset */,
			const  XalanDOMString& 	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesCDATASectionWrapper::deleteData(
			unsigned int	/* offset */,
			unsigned int	/* count */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesCDATASectionWrapper::replaceData(
			unsigned int			/* offset */,
			unsigned int			/* count */,
			const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanText*
XercesCDATASectionWrapper::splitText(unsigned int	/* offset */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



bool
XercesCDATASectionWrapper::isIgnorableWhitespace() const
{
	return isXMLWhitespace(getData());
}



XALAN_CPP_NAMESPACE_END
