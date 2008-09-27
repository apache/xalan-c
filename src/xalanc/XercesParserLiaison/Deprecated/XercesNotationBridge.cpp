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


#include "XercesNotationBridge.hpp"



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesBridgeHelper.hpp"
#include "XercesDocumentBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesNotationBridge::XercesNotationBridge(
            const DOM_NotationType&         theXercesDOMNotation,
            const XercesBridgeNavigator&    theNavigator) :
    XalanNotation(),
    m_xercesNode(theXercesDOMNotation),
    m_navigator(theNavigator)
{
}



XercesNotationBridge::~XercesNotationBridge()
{
}



const XalanDOMString&
XercesNotationBridge::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesNotationBridge::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesNotationBridge::NodeType
XercesNotationBridge::getNodeType() const
{
    return NOTATION_NODE;
}



XalanNode*
XercesNotationBridge::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesNotationBridge::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesNotationBridge::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesNotationBridge::getLastChild() const
{
    return 0;
}



XalanNode*
XercesNotationBridge::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesNotationBridge::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesNotationBridge::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesNotationBridge::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesNotationBridge*
#endif
XercesNotationBridge::cloneNode(bool    deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
    XercesNotationBridge* const     theBridge =
        static_cast<XercesNotationBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

    return theBridge;
#endif
}



XalanNode*
XercesNotationBridge::insertBefore(
            XalanNode*  newChild,
            XalanNode*  refChild)
{
    return m_navigator.insertBefore(m_xercesNode, newChild, refChild);
}



XalanNode*
XercesNotationBridge::replaceChild(
            XalanNode*  newChild,
            XalanNode*  oldChild)
{
    return m_navigator.replaceChild(m_xercesNode, newChild, oldChild);
}



XalanNode*
XercesNotationBridge::removeChild(XalanNode*    oldChild)
{
    return m_navigator.removeChild(m_xercesNode, oldChild);
}



XalanNode*
XercesNotationBridge::appendChild(XalanNode*    newChild)
{
    return m_navigator.appendChild(m_xercesNode, newChild);
}



bool
XercesNotationBridge::hasChildNodes() const
{
    return false;
}



void
XercesNotationBridge::setNodeValue(const XalanDOMString&    nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesNotationBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesNode);
}


bool
XercesNotationBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesNode.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesNotationBridge::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesNotationBridge::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesNotationBridge::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesNotationBridge::setPrefix(const XalanDOMString&   prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesNotationBridge::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesNotationBridge::IndexType
XercesNotationBridge::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesNotationBridge::getPublicId() const
{
    return m_navigator.getPooledString(m_xercesNode.getPublicId());
}



const XalanDOMString&
XercesNotationBridge::getSystemId() const
{
    return m_navigator.getPooledString(m_xercesNode.getSystemId());
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


