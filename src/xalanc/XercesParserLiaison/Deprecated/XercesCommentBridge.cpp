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


#include "XercesCommentBridge.hpp"



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeHelper.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesCommentBridge::XercesCommentBridge(
            const DOM_CommentType&          theXercesComment,
            const XercesBridgeNavigator&    theNavigator) :
    XalanComment(),
    m_xercesNode(theXercesComment),
    m_navigator(theNavigator)
{
}



XercesCommentBridge::~XercesCommentBridge()
{
}



const XalanDOMString&
XercesCommentBridge::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesCommentBridge::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesCommentBridge::NodeType
XercesCommentBridge::getNodeType() const
{
    return COMMENT_NODE;
}



XalanNode*
XercesCommentBridge::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesCommentBridge::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesCommentBridge::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesCommentBridge::getLastChild() const
{
    return 0;
}



XalanNode*
XercesCommentBridge::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesCommentBridge::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesCommentBridge::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesCommentBridge::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesCommentBridge*
#endif
XercesCommentBridge::cloneNode(bool deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
    XercesCommentBridge* const  theBridge =
        static_cast<XercesCommentBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

    return theBridge;
#endif
}



XalanNode*
XercesCommentBridge::insertBefore(
            XalanNode*  /* newChild */,
            XalanNode*  /* refChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesCommentBridge::replaceChild(
            XalanNode*  /* newChild */,
            XalanNode*  /* oldChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesCommentBridge::removeChild(XalanNode* /* oldChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesCommentBridge::appendChild(XalanNode*  /* newChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



bool
XercesCommentBridge::hasChildNodes() const
{
    return false;
}



void
XercesCommentBridge::setNodeValue(const XalanDOMString& nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesCommentBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesNode);
}


bool
XercesCommentBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesNode.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesCommentBridge::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesCommentBridge::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesCommentBridge::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesCommentBridge::setPrefix(const XalanDOMString&    prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesCommentBridge::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesCommentBridge::IndexType
XercesCommentBridge::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesCommentBridge::getData() const
{
    return m_navigator.getPooledString(m_xercesNode.getData());
}



unsigned int
XercesCommentBridge::getLength() const
{
    return m_xercesNode.getLength();
}



XalanDOMString
XercesCommentBridge::substringData(
            unsigned int    offset, 
            unsigned int    count) const
{
    return XercesBridgeHelper::substringData(m_xercesNode, offset, count);
}



void
XercesCommentBridge::appendData(const XalanDOMString&   arg)
{
    XercesBridgeHelper::appendData(m_xercesNode, arg);
}



void
XercesCommentBridge::insertData(
            unsigned int            offset,
            const  XalanDOMString&  arg)
{
    XercesBridgeHelper::insertData(m_xercesNode, offset, arg);
}



void
XercesCommentBridge::deleteData(
            unsigned int    offset, 
            unsigned int    count)
{
    XercesBridgeHelper::deleteData(m_xercesNode, offset, count);
}



void
XercesCommentBridge::replaceData(
            unsigned int            offset, 
            unsigned int            count, 
            const XalanDOMString&   arg)
{
    XercesBridgeHelper::replaceData(m_xercesNode, offset, count, arg);
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


