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
#include "XercesAttrWrapper.hpp"



#include <cassert>



#include <xercesc/dom/DOMAttr.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesAttrWrapper::XercesAttrWrapper(
            const DOMAttrType*              theXercesAttr,
            const XercesWrapperNavigator&   theNavigator) :
    XalanAttr(),
    m_xercesNode(theXercesAttr),
    m_children(theXercesAttr->getChildNodes(),
               theNavigator),
    m_navigator(theNavigator)
{
    assert(theXercesAttr != 0);
}



XercesAttrWrapper::~XercesAttrWrapper()
{
}



const XalanDOMString&
XercesAttrWrapper::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesAttrWrapper::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesAttrWrapper::NodeType
XercesAttrWrapper::getNodeType() const
{
    return ATTRIBUTE_NODE;
}



XalanNode*
XercesAttrWrapper::getParentNode() const
{
    return 0;
}



const XalanNodeList*
XercesAttrWrapper::getChildNodes() const
{
    return &m_children;
}



XalanNode*
XercesAttrWrapper::getFirstChild() const
{
    return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesAttrWrapper::getLastChild() const
{
    return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesAttrWrapper::getPreviousSibling() const
{
    return 0;
}



XalanNode*
XercesAttrWrapper::getNextSibling() const
{
    return 0;
}



const XalanNamedNodeMap*
XercesAttrWrapper::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesAttrWrapper::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



const XalanDOMString&
XercesAttrWrapper::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesAttrWrapper::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesAttrWrapper::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



bool
XercesAttrWrapper::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesAttrWrapper::IndexType
XercesAttrWrapper::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesAttrWrapper::getName() const
{
    return m_navigator.getPooledString(m_xercesNode->getName());
}



bool
XercesAttrWrapper::getSpecified() const
{
    return m_xercesNode->getSpecified();
}



const XalanDOMString&
XercesAttrWrapper::getValue() const
{
    return m_navigator.getPooledString(m_xercesNode->getValue());
}



XalanElement*
XercesAttrWrapper::getOwnerElement() const
{
    return m_navigator.getOwnerElement(m_xercesNode);
}



XALAN_CPP_NAMESPACE_END
