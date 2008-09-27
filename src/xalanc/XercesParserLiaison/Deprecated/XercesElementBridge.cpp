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


#include "XercesElementBridge.hpp"



#include <xalanc/XalanDOM/XalanAttr.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeHelper.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeNavigator.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesElementBridge::XercesElementBridge(
            const DOM_ElementType&          theXercesElement,
            const XercesBridgeNavigator&    theNavigator) :
    XalanElement(),
    m_xercesNode(theXercesElement),
    m_navigator(theNavigator),
    m_children(theXercesElement.getChildNodes(),
               theNavigator),
    m_attributes(theXercesElement.getAttributes(),
                 theNavigator)
{
}



XercesElementBridge::~XercesElementBridge()
{
}



const XalanDOMString&
XercesElementBridge::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeNameImpl());
}



const XalanDOMString&
XercesElementBridge::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeValueImpl());
}



XercesElementBridge::NodeType
XercesElementBridge::getNodeType() const
{
    return ELEMENT_NODE;
}



XalanNode*
XercesElementBridge::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesElementBridge::getChildNodes() const
{
    return &m_children;
}



XalanNode*
XercesElementBridge::getFirstChild() const
{
    return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesElementBridge::getLastChild() const
{
    return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesElementBridge::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesElementBridge::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesElementBridge::getAttributes() const
{
    return &m_attributes;
}



XalanDocument*
XercesElementBridge::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesElementBridge*
#endif
XercesElementBridge::cloneNode(bool deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
    XercesElementBridge* const  theBridge =
        static_cast<XercesElementBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

    return theBridge;
#endif
}



XalanNode*
XercesElementBridge::insertBefore(
            XalanNode*  newChild,
            XalanNode*  refChild)
{
    return m_navigator.insertBefore(m_xercesNode, newChild, refChild);
}



XalanNode*
XercesElementBridge::replaceChild(
            XalanNode*  newChild,
            XalanNode*  oldChild)
{
    return m_navigator.replaceChild(m_xercesNode, newChild, oldChild);
}



XalanNode*
XercesElementBridge::removeChild(XalanNode* oldChild)
{
    return m_navigator.removeChild(m_xercesNode, oldChild);
}



XalanNode*
XercesElementBridge::appendChild(XalanNode* newChild)
{
    return m_navigator.appendChild(m_xercesNode, newChild);
}



bool
XercesElementBridge::hasChildNodes() const
{
    return m_xercesNode.hasChildNodes();
}



void
XercesElementBridge::setNodeValue(const XalanDOMString&     nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesElementBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesNode);
}


bool
XercesElementBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesNode.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesElementBridge::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode.getNamespaceURIImpl());
}



const XalanDOMString&
XercesElementBridge::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode.getPrefixImpl());
}



const XalanDOMString&
XercesElementBridge::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode.getLocalNameImpl());
}



void
XercesElementBridge::setPrefix(const XalanDOMString&    prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesElementBridge::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesElementBridge::IndexType
XercesElementBridge::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesElementBridge::getTagName() const
{
    return m_navigator.getPooledString(m_xercesNode.getTagNameImpl());
}



const XalanDOMString&
XercesElementBridge::getAttribute(const XalanDOMString&     name) const
{
    return m_navigator.getPooledString(m_xercesNode.getAttributeImpl(name.c_str()));
}



XalanAttr*
XercesElementBridge::getAttributeNode(const XalanDOMString&     name) const
{
    return static_cast<XalanAttr*>(m_attributes.getNamedItem(name));
}



XalanNodeList*
XercesElementBridge::getElementsByTagName(const XalanDOMString&     /* name */) const
{
    // Not supported...
    return 0;
}



void
XercesElementBridge::setAttribute(
            const XalanDOMString&   name, 
            const XalanDOMString&   value)
{
    try
    {
        m_xercesNode.setAttribute(name.c_str(), value.c_str());
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



XalanAttr*
XercesElementBridge::setAttributeNode(XalanAttr*    newAttr)
{
    assert(newAttr != 0);

    const DOM_AttrType  theXercesAttrNode =
            m_navigator.mapNode(newAttr);
    assert(theXercesAttrNode.isNull() == false);

    XalanAttr*  theXalanAttrResult = 0;

    try
    {
        const DOM_AttrType  theXercesAttrResult =
            m_xercesNode.setAttributeNode(theXercesAttrNode);

        theXalanAttrResult = m_navigator.mapNode(theXercesAttrResult);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theXalanAttrResult;
}



XalanAttr*
XercesElementBridge::removeAttributeNode(XalanAttr*     oldAttr)
{
    assert(oldAttr != 0);

    const DOM_AttrType  theXercesAttrNode =
            m_navigator.mapNode(oldAttr);
    assert(theXercesAttrNode.isNull() == false);

    XalanAttr*  theXalanAttrResult = 0;

    try
    {
        const DOM_AttrType  theXercesAttrResult =
            m_xercesNode.removeAttributeNode(theXercesAttrNode);

        theXalanAttrResult = m_navigator.mapNode(theXercesAttrResult);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theXalanAttrResult;
}



void
XercesElementBridge::removeAttribute(const XalanDOMString&  name)
{
    try
    {
        m_xercesNode.removeAttribute(name.c_str());
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



const XalanDOMString&
XercesElementBridge::getAttributeNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName) const
{
    return m_navigator.getPooledString(m_xercesNode.getAttributeNSImpl(namespaceURI.c_str(), localName.c_str()));
}



void
XercesElementBridge::setAttributeNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   qualifiedName,
            const XalanDOMString&   value)
{
    try
    {
        m_xercesNode.setAttributeNS(namespaceURI.c_str(), qualifiedName.c_str(), value.c_str());
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



void
XercesElementBridge::removeAttributeNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName)
{
    try
    {
        m_xercesNode.removeAttributeNS(namespaceURI.c_str(), localName.c_str());
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



XalanAttr*
XercesElementBridge::getAttributeNodeNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName) const
{
    const DOM_AttrType  theAttrNode =
        m_xercesNode.getAttributeNodeNS(namespaceURI.c_str(), localName.c_str());

    if (theAttrNode.isNull() == true)
    {
        return 0;
    }
    else
    {
        return m_navigator.mapNode(theAttrNode);
    }
}



XalanAttr*
XercesElementBridge::setAttributeNodeNS(XalanAttr*  newAttr)
{
    assert(newAttr != 0);


    const DOM_AttrType  theXercesAttrNode =
            m_navigator.mapNode(newAttr);
    assert(theXercesAttrNode.isNull() == false);

    XalanAttr*  theXalanAttrResult = 0;

    try
    {
        const DOM_AttrType  theXercesAttrResult =
            m_xercesNode.setAttributeNodeNS(theXercesAttrNode);

        theXalanAttrResult = m_navigator.mapNode(theXercesAttrResult);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theXalanAttrResult;
}



XalanNodeList*
XercesElementBridge::getElementsByTagNameNS(
            const XalanDOMString&   /* namespaceURI */,
            const XalanDOMString&   /* localName */) const
{
    // Not supported...
    return 0;
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


