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
#include "XercesWrapperNavigator.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>



#include "XercesAttrWrapper.hpp"
#include "XercesDocumentWrapper.hpp"
#include "XercesTextWrapper.hpp"
#include "XercesDOMWrapperException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString    XercesWrapperNavigator::s_emptyString(XalanMemMgrs::getDummyMemMgr());



XercesWrapperNavigator::XercesWrapperNavigator(
            XercesDocumentWrapper*  theOwnerDocument) :
    m_ownerDocument(theOwnerDocument),
    m_parentNode(0),
    m_previousSibling(0),
    m_nextSibling(0),
    m_firstChild(0),
    m_lastChild(0),
    m_index(0)
{
    assert(theOwnerDocument != 0);
}



XercesWrapperNavigator::XercesWrapperNavigator(const XercesWrapperNavigator&    theSource) :
    m_ownerDocument(theSource.m_ownerDocument),
    m_parentNode(theSource.m_parentNode),
    m_previousSibling(theSource.m_previousSibling),
    m_nextSibling(theSource.m_nextSibling),
    m_firstChild(theSource.m_firstChild),
    m_lastChild(theSource.m_lastChild),
    m_index(theSource.m_index)
{
}



XercesWrapperNavigator::~XercesWrapperNavigator()
{
}



XalanNode*
XercesWrapperNavigator::mapNode(const DOMNodeType*  theXercesNode) const
{
    return m_ownerDocument->mapNode(theXercesNode);
}



XalanAttr*
XercesWrapperNavigator::mapNode(const DOMAttrType*  theXercesNode) const
{
    return m_ownerDocument->mapNode(theXercesNode);
}



const DOMNodeType*
XercesWrapperNavigator::mapNode(XalanNode*  theXalanNode) const
{
    return m_ownerDocument->mapNode(theXalanNode);
}



XalanNode*
XercesWrapperNavigator::getParentNode(const DOMNodeType*    theXercesNode) const
{
    if (m_parentNode == 0)
    {
        return m_ownerDocument->mapNode(theXercesNode->getParentNode());
    }
    else
    {
        return m_parentNode;
    }
}



XalanNode*
XercesWrapperNavigator::getPreviousSibling(const DOMNodeType*   theXercesNode) const
{
    if (m_previousSibling == 0)
    {
        return m_ownerDocument->mapNode(theXercesNode->getPreviousSibling());
    }
    else
    {
        return m_previousSibling;
    }
}



XalanNode*
XercesWrapperNavigator::getNextSibling(const DOMNodeType*   theXercesNode) const
{
    if (m_nextSibling == 0)
    {
        return m_ownerDocument->mapNode(theXercesNode->getNextSibling());
    }
    else
    {
        return m_nextSibling;
    }
}



XalanNode*
XercesWrapperNavigator::getFirstChild(const DOMNodeType*    theXercesNode) const
{
    if (m_firstChild == 0)
    {
        return m_ownerDocument->mapNode(theXercesNode->getFirstChild());
    }
    else
    {
        return m_firstChild;
    }
}



XalanNode*
XercesWrapperNavigator::getLastChild(const DOMNodeType*     theXercesNode) const
{
    if (m_lastChild == 0)
    {
        return m_ownerDocument->mapNode(theXercesNode->getLastChild());
    }
    else
    {
        return m_lastChild;
    }
}



XalanElement*
XercesWrapperNavigator::getOwnerElement(const DOMAttrType*  theXercesAttr) const
{
    assert(theXercesAttr != 0);

    if (m_parentNode != 0)
    {
        assert(m_parentNode->getNodeType() == XalanNode::ELEMENT_NODE);

        return static_cast<XalanElement*>(m_parentNode);
    }
    else
    {
        return m_ownerDocument->mapNode(theXercesAttr->getOwnerElement());
    }
}



const XalanDOMString&
XercesWrapperNavigator::getPooledString(const XMLCh*    theString) const
{
    if (theString == 0)
    {
        return s_emptyString;
    }
    else
    {
        return m_ownerDocument->getPooledString(theString, XalanDOMString::length(theString));
    }
}



XALAN_CPP_NAMESPACE_END
