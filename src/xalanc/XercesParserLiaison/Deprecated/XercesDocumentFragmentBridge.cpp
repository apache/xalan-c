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


#include "XercesDocumentFragmentBridge.hpp"



#include <cassert>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesBridgeHelper.hpp"
#include "XercesDocumentBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesDocumentFragmentBridge::XercesDocumentFragmentBridge(
            const DOM_DocumentFragmentType&     theXercesDOMDocumentFragment,
            const XercesBridgeNavigator&        theNavigator) :
    XalanDocumentFragment(),
    m_xercesNode(theXercesDOMDocumentFragment),
    m_children(theXercesDOMDocumentFragment.getChildNodes(),
               theNavigator),
    m_navigator(theNavigator)
{
}



XercesDocumentFragmentBridge::~XercesDocumentFragmentBridge()
{
}



const XalanDOMString&
XercesDocumentFragmentBridge::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesDocumentFragmentBridge::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesDocumentFragmentBridge::NodeType
XercesDocumentFragmentBridge::getNodeType() const
{
    return DOCUMENT_FRAGMENT_NODE;
}



XalanNode*
XercesDocumentFragmentBridge::getParentNode() const
{
    return 0;
}



const XalanNodeList*
XercesDocumentFragmentBridge::getChildNodes() const
{
    return &m_children;
}



XalanNode*
XercesDocumentFragmentBridge::getFirstChild() const
{
    return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesDocumentFragmentBridge::getLastChild() const
{
    return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesDocumentFragmentBridge::getPreviousSibling() const
{
    return 0;
}



XalanNode*
XercesDocumentFragmentBridge::getNextSibling() const
{
    return 0;
}



const XalanNamedNodeMap*
XercesDocumentFragmentBridge::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesDocumentFragmentBridge::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesDocumentFragmentBridge*
#endif
XercesDocumentFragmentBridge::cloneNode(bool    deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
    XercesDocumentFragmentBridge* const theBridge =
        static_cast<XercesDocumentFragmentBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

    return theBridge;
#endif
}



XalanNode*
XercesDocumentFragmentBridge::insertBefore(
            XalanNode*  newChild,
            XalanNode*  refChild)
{
    return m_navigator.insertBefore(m_xercesNode, newChild, refChild);
}



XalanNode*
XercesDocumentFragmentBridge::replaceChild(
            XalanNode*  newChild,
            XalanNode*  oldChild)
{
    return m_navigator.replaceChild(m_xercesNode, newChild, oldChild);
}



XalanNode*
XercesDocumentFragmentBridge::removeChild(XalanNode*    oldChild)
{
    return m_navigator.removeChild(m_xercesNode, oldChild);
}



XalanNode*
XercesDocumentFragmentBridge::appendChild(XalanNode*    newChild)
{
    return m_navigator.appendChild(m_xercesNode, newChild);
}



bool
XercesDocumentFragmentBridge::hasChildNodes() const
{
    return m_xercesNode.hasChildNodes();
}



void
XercesDocumentFragmentBridge::setNodeValue(const XalanDOMString&    nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesDocumentFragmentBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesNode);
}


bool
XercesDocumentFragmentBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesNode.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesDocumentFragmentBridge::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesDocumentFragmentBridge::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesDocumentFragmentBridge::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesDocumentFragmentBridge::setPrefix(const XalanDOMString&   prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesDocumentFragmentBridge::isIndexed() const
{
    // $$$ ToDo: Are document fragments ever indexed?
    // I don't think so...
    return false;
}



XercesDocumentFragmentBridge::IndexType
XercesDocumentFragmentBridge::getIndex() const
{
    return 0;
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


