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


#include "XercesEntityBridge.hpp"



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesBridgeHelper.hpp"
#include "XercesBridgeNavigator.hpp"
#include "XercesDocumentBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesEntityBridge::XercesEntityBridge(
            const DOM_EntityType&           theXercesDOMEntity,
            const XercesBridgeNavigator&    theNavigator) :
    XalanEntity(),
    m_xercesNode(theXercesDOMEntity),
    m_navigator(theNavigator),
    m_children(m_xercesNode.getChildNodes(),
               theNavigator)
{
}



XercesEntityBridge::~XercesEntityBridge()
{
}



const XalanDOMString&
XercesEntityBridge::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesEntityBridge::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesEntityBridge::NodeType
XercesEntityBridge::getNodeType() const
{
    return ENTITY_NODE;
}



XalanNode*
XercesEntityBridge::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesEntityBridge::getChildNodes() const
{
    return &m_children;
}



XalanNode*
XercesEntityBridge::getFirstChild() const
{
    return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesEntityBridge::getLastChild() const
{
    return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesEntityBridge::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesEntityBridge::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesEntityBridge::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesEntityBridge::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesEntityBridge*
#endif
XercesEntityBridge::cloneNode(bool  deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
    XercesEntityBridge* const   theBridge =
        static_cast<XercesEntityBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

    return theBridge;
#endif
}



XalanNode*
XercesEntityBridge::insertBefore(
            XalanNode*  newChild,
            XalanNode*  refChild)
{
    return m_navigator.insertBefore(m_xercesNode, newChild, refChild);
}



XalanNode*
XercesEntityBridge::replaceChild(
            XalanNode*  newChild,
            XalanNode*  oldChild)
{
    return m_navigator.replaceChild(m_xercesNode, newChild, oldChild);
}



XalanNode*
XercesEntityBridge::removeChild(XalanNode*  oldChild)
{
    return m_navigator.removeChild(m_xercesNode, oldChild);
}



XalanNode*
XercesEntityBridge::appendChild(XalanNode*  newChild)
{
    return m_navigator.appendChild(m_xercesNode, newChild);
}



bool
XercesEntityBridge::hasChildNodes() const
{
    return m_xercesNode.hasChildNodes();
}



void
XercesEntityBridge::setNodeValue(const XalanDOMString&  nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesEntityBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesNode);
}


bool
XercesEntityBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesNode.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesEntityBridge::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesEntityBridge::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesEntityBridge::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesEntityBridge::setPrefix(const XalanDOMString&     prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesEntityBridge::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesEntityBridge::IndexType
XercesEntityBridge::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesEntityBridge::getPublicId() const
{
    return m_navigator.getPooledString(m_xercesNode.getPublicId());
}



const XalanDOMString&
XercesEntityBridge::getSystemId() const
{
    return m_navigator.getPooledString(m_xercesNode.getSystemId());
}



const XalanDOMString&
XercesEntityBridge::getNotationName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNotationName());
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


