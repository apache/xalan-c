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
#include "XercesProcessingInstructionWrapper.hpp"



#include <xercesc/dom/DOMProcessingInstruction.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesDOMWrapperException.hpp"
#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesProcessingInstructionWrapper::XercesProcessingInstructionWrapper(
			const DOMProcessingInstructionType*		theXercesDOMProcessingInstruction,
			const XercesWrapperNavigator&			theNavigator) :
	XalanProcessingInstruction(),
	m_xercesNode(theXercesDOMProcessingInstruction),
	m_navigator(theNavigator)
{
	assert(theXercesDOMProcessingInstruction != 0);
}



XercesProcessingInstructionWrapper::~XercesProcessingInstructionWrapper()
{
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesProcessingInstructionWrapper::NodeType
XercesProcessingInstructionWrapper::getNodeType() const
{
	return PROCESSING_INSTRUCTION_NODE;
}



XalanNode*
XercesProcessingInstructionWrapper::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesProcessingInstructionWrapper::getChildNodes() const
{
	return 0;
}



XalanNode*
XercesProcessingInstructionWrapper::getFirstChild() const
{
	return 0;
}



XalanNode*
XercesProcessingInstructionWrapper::getLastChild() const
{
	return 0;
}



XalanNode*
XercesProcessingInstructionWrapper::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesProcessingInstructionWrapper::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesProcessingInstructionWrapper::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesProcessingInstructionWrapper::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesProcessingInstructionWrapper*
#endif
XercesProcessingInstructionWrapper::cloneNode(bool		/* deep */) const
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XercesProcessingInstructionWrapper::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesProcessingInstructionWrapper::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesProcessingInstructionWrapper::removeChild(XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesProcessingInstructionWrapper::appendChild(XalanNode*	/* newChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



bool
XercesProcessingInstructionWrapper::hasChildNodes() const
{
	return false;
}



void
XercesProcessingInstructionWrapper::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesProcessingInstructionWrapper::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesProcessingInstructionWrapper::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return XercesWrapperHelper::isSupported(m_xercesNode, feature, version);
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



void
XercesProcessingInstructionWrapper::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesProcessingInstructionWrapper::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



XercesProcessingInstructionWrapper::IndexType
XercesProcessingInstructionWrapper::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getTarget() const
{
	return m_navigator.getPooledString(m_xercesNode->getTarget());
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getData() const
{
	return m_navigator.getPooledString(m_xercesNode->getData());
}



void
XercesProcessingInstructionWrapper::setData(const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XALAN_CPP_NAMESPACE_END
