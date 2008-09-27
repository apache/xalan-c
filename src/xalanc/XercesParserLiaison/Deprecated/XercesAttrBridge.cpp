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


#include "XercesAttrBridge.hpp"



#include <cassert>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeHelper.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesElementBridge.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesAttrBridge::XercesAttrBridge(
            const DOM_AttrType&             theXercesAttr,
            const XercesBridgeNavigator&    theNavigator) :
    XalanAttr(),
    m_xercesNode(theXercesAttr),
    m_children(theXercesAttr.getChildNodes(),
               theNavigator),
    m_navigator(theNavigator)
{
}



XercesAttrBridge::~XercesAttrBridge()
{
}



const XalanDOMString&
XercesAttrBridge::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesAttrBridge::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesAttrBridge::NodeType
XercesAttrBridge::getNodeType() const
{
    return ATTRIBUTE_NODE;
}



XalanNode*
XercesAttrBridge::getParentNode() const
{
    return 0;
}



const XalanNodeList*
XercesAttrBridge::getChildNodes() const
{
    return &m_children;
}



XalanNode*
XercesAttrBridge::getFirstChild() const
{
    return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesAttrBridge::getLastChild() const
{
    return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesAttrBridge::getPreviousSibling() const
{
    return 0;
}



XalanNode*
XercesAttrBridge::getNextSibling() const
{
    return 0;
}



const XalanNamedNodeMap*
XercesAttrBridge::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesAttrBridge::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesAttrBridge*
#endif
XercesAttrBridge::cloneNode(bool    deep) const
{
    assert(m_navigator.getOwnerDocument() != 0);

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
    XercesAttrBridge* const     theBridge =
        static_cast<XercesAttrBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

    return theBridge;
#endif
}



XalanNode*
XercesAttrBridge::insertBefore(
            XalanNode*  newChild,
            XalanNode*  refChild)
{
    return m_navigator.insertBefore(m_xercesNode, newChild, refChild);
}



XalanNode*
XercesAttrBridge::replaceChild(
            XalanNode*  newChild,
            XalanNode*  oldChild)
{
    return m_navigator.replaceChild(m_xercesNode, newChild, oldChild);
}



XalanNode*
XercesAttrBridge::removeChild(XalanNode*    oldChild)
{
    return m_navigator.removeChild(m_xercesNode, oldChild);
}



XalanNode*
XercesAttrBridge::appendChild(XalanNode*    newChild)
{
    return m_navigator.appendChild(m_xercesNode, newChild);
}



bool
XercesAttrBridge::hasChildNodes() const
{
    return m_xercesNode.hasChildNodes();
}



void
XercesAttrBridge::setNodeValue(const XalanDOMString&    nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesAttrBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesNode);
}



bool
XercesAttrBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesNode.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesAttrBridge::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesAttrBridge::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesAttrBridge::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesAttrBridge::setPrefix(const XalanDOMString&   prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesAttrBridge::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesAttrBridge::IndexType
XercesAttrBridge::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesAttrBridge::getName() const
{
    return m_navigator.getPooledString(m_xercesNode.getName());
}



bool
XercesAttrBridge::getSpecified() const
{
    return m_xercesNode.getSpecified();
}



const XalanDOMString&
XercesAttrBridge::getValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getValue());
}



void
XercesAttrBridge::setValue(const XalanDOMString&    value)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, value);
}



XalanElement*
XercesAttrBridge::getOwnerElement() const
{
    return m_navigator.getOwnerElement(m_xercesNode);
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


