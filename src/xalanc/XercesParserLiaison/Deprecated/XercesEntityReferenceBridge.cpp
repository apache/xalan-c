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


#include "XercesEntityReferenceBridge.hpp"



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesBridgeHelper.hpp"
#include "XercesDocumentBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesEntityReferenceBridge::XercesEntityReferenceBridge(
            const DOM_EntityReferenceType&  theXercesDOMEntityReference,
            const XercesBridgeNavigator&    theNavigator) :
    XalanEntityReference(),
    m_xercesNode(theXercesDOMEntityReference),
    m_navigator(theNavigator),
    m_children(theXercesDOMEntityReference.getChildNodes(),
               theNavigator)
{
}



XercesEntityReferenceBridge::~XercesEntityReferenceBridge()
{
}



const XalanDOMString&
XercesEntityReferenceBridge::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesEntityReferenceBridge::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesEntityReferenceBridge::NodeType
XercesEntityReferenceBridge::getNodeType() const
{
    return ENTITY_REFERENCE_NODE;
}



XalanNode*
XercesEntityReferenceBridge::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesEntityReferenceBridge::getChildNodes() const
{
    return &m_children;
}



XalanNode*
XercesEntityReferenceBridge::getFirstChild() const
{
    return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesEntityReferenceBridge::getLastChild() const
{
    return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesEntityReferenceBridge::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesEntityReferenceBridge::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesEntityReferenceBridge::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesEntityReferenceBridge::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesEntityReferenceBridge*
#endif
XercesEntityReferenceBridge::cloneNode(bool     deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
    XercesEntityReferenceBridge* const  theBridge =
        static_cast<XercesEntityReferenceBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

    return theBridge;
#endif
}



XalanNode*
XercesEntityReferenceBridge::insertBefore(
            XalanNode*  newChild,
            XalanNode*  refChild)
{
    return m_navigator.insertBefore(m_xercesNode, newChild, refChild);
}



XalanNode*
XercesEntityReferenceBridge::replaceChild(
            XalanNode*  newChild,
            XalanNode*  oldChild)
{
    return m_navigator.replaceChild(m_xercesNode, newChild, oldChild);
}



XalanNode*
XercesEntityReferenceBridge::removeChild(XalanNode* oldChild)
{
    return m_navigator.removeChild(m_xercesNode, oldChild);
}



XalanNode*
XercesEntityReferenceBridge::appendChild(XalanNode* newChild)
{
    return m_navigator.appendChild(m_xercesNode, newChild);
}



bool
XercesEntityReferenceBridge::hasChildNodes() const
{
    return m_xercesNode.hasChildNodes();
}



void
XercesEntityReferenceBridge::setNodeValue(const XalanDOMString& nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesEntityReferenceBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesNode);
}



bool
XercesEntityReferenceBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesNode.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesEntityReferenceBridge::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesEntityReferenceBridge::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesEntityReferenceBridge::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesEntityReferenceBridge::setPrefix(const XalanDOMString&    prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesEntityReferenceBridge::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesEntityReferenceBridge::IndexType
XercesEntityReferenceBridge::getIndex() const
{
    return m_navigator.getIndex();
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


