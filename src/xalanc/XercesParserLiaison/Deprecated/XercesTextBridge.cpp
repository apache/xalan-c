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


#include "XercesTextBridge.hpp"



#include <cassert>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeHelper.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeNavigator.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesTextBridge::XercesTextBridge(
            const DOM_TextType&             theXercesText,
            const XercesBridgeNavigator&    theNavigator) :
    XalanText(),
    m_xercesNode(theXercesText),
    m_navigator(theNavigator)
{
    assert(m_xercesNode.isNull() == false);
}



XercesTextBridge::~XercesTextBridge()
{
}



const XalanDOMString&
XercesTextBridge::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesTextBridge::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesTextBridge::NodeType
XercesTextBridge::getNodeType() const
{
    return TEXT_NODE;
}



XalanNode*
XercesTextBridge::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesTextBridge::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesTextBridge::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesTextBridge::getLastChild() const
{
    return 0;
}



XalanNode*
XercesTextBridge::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesTextBridge::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesTextBridge::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesTextBridge::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesTextBridge*
#endif
XercesTextBridge::cloneNode(bool    deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
    XercesTextBridge* const theBridge =
        static_cast<XercesTextBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

    return theBridge;
#endif
}



XalanNode*
XercesTextBridge::insertBefore(
            XalanNode*  /* newChild */,
            XalanNode*  /* refChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesTextBridge::replaceChild(
            XalanNode*  /* newChild */,
            XalanNode*  /* oldChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesTextBridge::removeChild(XalanNode*    /* oldChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesTextBridge::appendChild(XalanNode*    /* newChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



bool
XercesTextBridge::hasChildNodes() const
{
    return false;
}


void
XercesTextBridge::setNodeValue(const XalanDOMString&    nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesTextBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesNode);
}



bool
XercesTextBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesNode.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesTextBridge::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesTextBridge::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode.getPrefixImpl());
}



const XalanDOMString&
XercesTextBridge::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode.getLocalNameImpl());
}



void
XercesTextBridge::setPrefix(const XalanDOMString&   prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesTextBridge::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesTextBridge::IndexType
XercesTextBridge::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesTextBridge::getData() const
{
    return m_navigator.getPooledString(m_xercesNode.getDataImpl());
}



unsigned int
XercesTextBridge::getLength() const
{
    return m_xercesNode.getLength();
}



XalanDOMString
XercesTextBridge::substringData(
            unsigned int    offset, 
            unsigned int    count) const
{
    return XercesBridgeHelper::substringData(m_xercesNode, offset, count);
}



void
XercesTextBridge::appendData(const XalanDOMString&  arg)
{
    XercesBridgeHelper::appendData(m_xercesNode, arg);
}



void
XercesTextBridge::insertData(
            unsigned int            offset,
            const XalanDOMString&   arg)
{
    XercesBridgeHelper::insertData(m_xercesNode, offset, arg);
}



void
XercesTextBridge::deleteData(
            unsigned int    offset, 
            unsigned int    count)
{
    XercesBridgeHelper::deleteData(m_xercesNode, offset, count);
}



void
XercesTextBridge::replaceData(
            unsigned int            offset, 
            unsigned int            count, 
            const XalanDOMString&   arg)
{
    XercesBridgeHelper::replaceData(m_xercesNode, offset, count, arg);
}



XalanText*
XercesTextBridge::splitText(unsigned int    offset)
{
    return m_navigator.splitText(m_xercesNode, offset);
}



bool
XercesTextBridge::isWhitespace() const
{
    return isXMLWhitespace(getData());
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


