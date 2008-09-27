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
#include "XalanSourceTreeElementA.hpp"


#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>


#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include "XalanSourceTreeAttr.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString     s_emptyString(XalanMemMgrs::getDummyMemMgr());



XalanSourceTreeElementA::XalanSourceTreeElementA(
            MemoryManager&          theManager,
            const XalanDOMString&       theTagName,
            XalanSourceTreeDocument*    theOwnerDocument,
            XalanSourceTreeAttr**       theAttributes,
            XalanSize_t                 theAttributeCount,
            XalanNode*                  theParentNode,
            XalanNode*                  thePreviousSibling,
            XalanNode*                  theNextSibling,
            IndexType                   theIndex) :
    XalanSourceTreeElement(
        theManager,
        theTagName,
        theOwnerDocument,
        theParentNode,
        thePreviousSibling,
        theNextSibling,
        theIndex),
    m_attributes(theAttributes),
    m_attributeCount(theAttributeCount)
{
}



XalanSourceTreeElementA::~XalanSourceTreeElementA()
{
}


const XalanNamedNodeMap*
XalanSourceTreeElementA::getAttributes() const
{
    return this;
}



const XalanDOMString&
XalanSourceTreeElementA::getNamespaceURI() const
{
    return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeElementA::getPrefix() const
{
    return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeElementA::getLocalName() const
{
    return m_tagName;
}



XalanNode*
XalanSourceTreeElementA::item(XalanSize_t   index) const
{
    return index < m_attributeCount ? m_attributes[index] : 0;
}



XalanNode*
XalanSourceTreeElementA::getNamedItem(const XalanDOMString&     name) const
{
    for (XalanSize_t i = 0; i < m_attributeCount; ++i)
    {
        assert(m_attributes[i] != 0);

        if (m_attributes[i]->getNodeName() == name)
        {
            return m_attributes[i];
        }
    }

    return 0;
}



XalanSize_t
XalanSourceTreeElementA::getLength() const
{
    return m_attributeCount;
}



XalanNode*
XalanSourceTreeElementA::getNamedItemNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName) const
{
    for (XalanSize_t i = 0; i < m_attributeCount; ++i)
    {
        assert(m_attributes[i] != 0);

        if (m_attributes[i]->getLocalName() == localName &&
            m_attributes[i]->getNamespaceURI() ==  namespaceURI)
        {
            return m_attributes[i];
        }
    }

    return 0;
}



XALAN_CPP_NAMESPACE_END
