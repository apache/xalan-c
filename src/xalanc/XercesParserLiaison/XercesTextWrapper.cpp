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
#include "XercesTextWrapper.hpp"



#include <cassert>



#include <xercesc/dom/DOMText.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesTextWrapper::XercesTextWrapper(
            const DOMTextType*              theXercesText,
            const XercesWrapperNavigator&   theNavigator) :
    XalanText(),
    m_xercesNode(theXercesText),
    m_navigator(theNavigator)
{
    assert(m_xercesNode != 0);
}



XercesTextWrapper::~XercesTextWrapper()
{
}



const XalanDOMString&
XercesTextWrapper::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesTextWrapper::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesTextWrapper::NodeType
XercesTextWrapper::getNodeType() const
{
    return TEXT_NODE;
}



XalanNode*
XercesTextWrapper::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesTextWrapper::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesTextWrapper::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesTextWrapper::getLastChild() const
{
    return 0;
}



XalanNode*
XercesTextWrapper::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesTextWrapper::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesTextWrapper::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesTextWrapper::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



const XalanDOMString&
XercesTextWrapper::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesTextWrapper::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesTextWrapper::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



bool
XercesTextWrapper::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesTextWrapper::IndexType
XercesTextWrapper::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesTextWrapper::getData() const
{
    return m_navigator.getPooledString(m_xercesNode->getData());
}



bool
XercesTextWrapper::isWhitespace() const
{
    return isXMLWhitespace(getData());
}



XALAN_CPP_NAMESPACE_END
