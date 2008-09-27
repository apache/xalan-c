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
#include "XercesElementWrapper.hpp"



#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>



#include <xalanc/XalanDOM/XalanAttr.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesElementWrapper::XercesElementWrapper(
            const DOMElementType*           theXercesElement,
            const XercesWrapperNavigator&   theNavigator) :
    XalanElement(),
    m_xercesNode(theXercesElement),
    m_navigator(theNavigator),
    m_attributes(theXercesElement->getAttributes(),
                 theNavigator)
{
    assert(theXercesElement != 0);
}



XercesElementWrapper::~XercesElementWrapper()
{
}



const XalanDOMString&
XercesElementWrapper::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesElementWrapper::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesElementWrapper::NodeType
XercesElementWrapper::getNodeType() const
{
    return ELEMENT_NODE;
}



XalanNode*
XercesElementWrapper::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesElementWrapper::getChildNodes() const
{
    return this;
}



XalanNode*
XercesElementWrapper::getFirstChild() const
{
    return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesElementWrapper::getLastChild() const
{
    return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesElementWrapper::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesElementWrapper::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesElementWrapper::getAttributes() const
{
    return &m_attributes;
}



XalanDocument*
XercesElementWrapper::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



const XalanDOMString&
XercesElementWrapper::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesElementWrapper::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesElementWrapper::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



bool
XercesElementWrapper::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesElementWrapper::IndexType
XercesElementWrapper::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesElementWrapper::getTagName() const
{
    return m_navigator.getPooledString(m_xercesNode->getTagName());
}



XalanNode*
XercesElementWrapper::item(XalanSize_t  index) const
{
    assert(index < getLength());

    if (m_navigator.getOwnerDocument()->getMappingMode() == true)
    {
        assert(m_xercesNode->getChildNodes()->item(index));

        return m_navigator.mapNode(m_xercesNode->getChildNodes()->item(index));
    }
    else
    {
        XalanNode*  child = getFirstChild();
        assert(child != 0);

        for(XalanSize_t i = 0; i < index; ++i)
        {
            child = child->getNextSibling();
            assert(child != 0);
        }

        return child;
    }
}



XalanSize_t
XercesElementWrapper::getLength() const
{
    return m_xercesNode->getChildNodes()->getLength();
}



XALAN_CPP_NAMESPACE_END
