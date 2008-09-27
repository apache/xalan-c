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


#include "XercesCDATASectionBridge.hpp"



#include <cassert>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeHelper.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesCDATASectionBridge::XercesCDATASectionBridge(
            const DOM_CDATASectionType&     theXercesCDATASection,
            const XercesBridgeNavigator&    theNavigator) :
    XalanCDATASection(),
    m_xercesNode(theXercesCDATASection),
    m_navigator(theNavigator)
{
}



XercesCDATASectionBridge::~XercesCDATASectionBridge()
{
}



const XalanDOMString&
XercesCDATASectionBridge::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesCDATASectionBridge::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesCDATASectionBridge::NodeType
XercesCDATASectionBridge::getNodeType() const
{
    return CDATA_SECTION_NODE;
}



XalanNode*
XercesCDATASectionBridge::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesCDATASectionBridge::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesCDATASectionBridge::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesCDATASectionBridge::getLastChild() const
{
    return 0;
}



XalanNode*
XercesCDATASectionBridge::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesCDATASectionBridge::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesCDATASectionBridge::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesCDATASectionBridge::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesCDATASectionBridge*
#endif
XercesCDATASectionBridge::cloneNode(bool    deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
    XercesCDATASectionBridge* const theBridge =
        static_cast<XercesCDATASectionBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

    return theBridge;
#endif
}



XalanNode*
XercesCDATASectionBridge::insertBefore(
            XalanNode*  /* newChild */,
            XalanNode*  /* refChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesCDATASectionBridge::replaceChild(
            XalanNode*  /* newChild */,
            XalanNode*  /* oldChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesCDATASectionBridge::removeChild(XalanNode*    /* oldChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesCDATASectionBridge::appendChild(XalanNode*     /* newChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



bool
XercesCDATASectionBridge::hasChildNodes() const
{
    return false;
}



void
XercesCDATASectionBridge::setNodeValue(const XalanDOMString&    nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesCDATASectionBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesNode);
}



bool
XercesCDATASectionBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesNode.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesCDATASectionBridge::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesCDATASectionBridge::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesCDATASectionBridge::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesCDATASectionBridge::setPrefix(const XalanDOMString&   prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}




bool
XercesCDATASectionBridge::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesCDATASectionBridge::IndexType
XercesCDATASectionBridge::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesCDATASectionBridge::getData() const
{
    return m_navigator.getPooledString(m_xercesNode.getData());
}



unsigned int
XercesCDATASectionBridge::getLength() const
{
    return m_xercesNode.getLength();
}



XalanDOMString
XercesCDATASectionBridge::substringData(
            unsigned int    offset, 
            unsigned int    count) const
{
    return XercesBridgeHelper::substringData(m_xercesNode, offset, count);
}



void
XercesCDATASectionBridge::appendData(const XalanDOMString&  arg)
{
    XercesBridgeHelper::appendData(m_xercesNode, arg);
}



void
XercesCDATASectionBridge::insertData(
            unsigned int            offset,
            const  XalanDOMString&  arg)
{
    XercesBridgeHelper::insertData(m_xercesNode, offset, arg);
}



void
XercesCDATASectionBridge::deleteData(
            unsigned int    offset, 
            unsigned int    count)
{
    XercesBridgeHelper::deleteData(m_xercesNode, offset, count);
}



void
XercesCDATASectionBridge::replaceData(
            unsigned int            offset, 
            unsigned int            count, 
            const XalanDOMString&   arg)
{
    XercesBridgeHelper::replaceData(m_xercesNode, offset, count, arg);
}



XalanText*
XercesCDATASectionBridge::splitText(unsigned int    offset)
{
    return m_navigator.splitText(m_xercesNode, offset);
}



bool
XercesCDATASectionBridge::isWhitespace() const
{
    return isXMLWhitespace(getData());
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


