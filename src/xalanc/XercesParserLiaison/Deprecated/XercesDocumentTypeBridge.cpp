/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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


#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)


#include "XercesDocumentTypeBridge.hpp"



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeHelper.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesDocumentTypeBridge::XercesDocumentTypeBridge(
            const DOM_DocumentType_Type&    theXercesDOMDocumentType,
            const XercesBridgeNavigator&    theNavigator) :
    XalanDocumentType(),
    m_xercesNode(theXercesDOMDocumentType),
    m_entities(m_xercesNode.getEntities(),
               theNavigator),
    m_notations(m_xercesNode.getNotations(),
                theNavigator),
    m_navigator(theNavigator)
{
}



XercesDocumentTypeBridge::~XercesDocumentTypeBridge()
{
}



const XalanDOMString&
XercesDocumentTypeBridge::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesDocumentTypeBridge::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesDocumentTypeBridge::NodeType
XercesDocumentTypeBridge::getNodeType() const
{
    return DOCUMENT_TYPE_NODE;
}



XalanNode*
XercesDocumentTypeBridge::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesDocumentTypeBridge::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesDocumentTypeBridge::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesDocumentTypeBridge::getLastChild() const
{
    return 0;
}



XalanNode*
XercesDocumentTypeBridge::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesDocumentTypeBridge::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesDocumentTypeBridge::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesDocumentTypeBridge::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesDocumentTypeBridge*
#endif
XercesDocumentTypeBridge::cloneNode(bool    deep) const
{
    // $$$ ToDo: Fix this!! XercesDocumentType nodes can't be cloned yet...
    assert(false);

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
    XercesDocumentTypeBridge* const     theBridge =
        static_cast<XercesDocumentTypeBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

    return theBridge;
#endif
}



XalanNode*
XercesDocumentTypeBridge::insertBefore(
            XalanNode*  /* newChild */,
            XalanNode*  /* refChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesDocumentTypeBridge::replaceChild(
            XalanNode*  /* newChild */,
            XalanNode*  /* oldChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesDocumentTypeBridge::removeChild(XalanNode*    /* oldChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesDocumentTypeBridge::appendChild(XalanNode*     /* newChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



bool
XercesDocumentTypeBridge::hasChildNodes() const
{
    return false;
}



void
XercesDocumentTypeBridge::setNodeValue(const XalanDOMString&    nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesDocumentTypeBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesNode);
}



bool
XercesDocumentTypeBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesNode.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesDocumentTypeBridge::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesDocumentTypeBridge::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesDocumentTypeBridge::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesDocumentTypeBridge::setPrefix(const XalanDOMString&   prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesDocumentTypeBridge::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesDocumentTypeBridge::IndexType
XercesDocumentTypeBridge::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesDocumentTypeBridge::getName() const
{
    return m_navigator.getPooledString(m_xercesNode.getName());
}



const XalanNamedNodeMap*
XercesDocumentTypeBridge::getEntities() const
{
    return &m_entities;
}



const XalanNamedNodeMap*
XercesDocumentTypeBridge::getNotations() const
{
    return &m_notations;
}



const XalanDOMString&
XercesDocumentTypeBridge::getPublicId() const
{
    return m_navigator.getPooledString(m_xercesNode.getPublicId());
}



const XalanDOMString&
XercesDocumentTypeBridge::getSystemId() const
{
    return m_navigator.getPooledString(m_xercesNode.getSystemId());
}



const XalanDOMString&
XercesDocumentTypeBridge::getInternalSubset() const
{
    return m_navigator.getPooledString(m_xercesNode.getInternalSubset());
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


