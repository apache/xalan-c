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
#include "XObjectResultTreeFragProxyText.hpp"



#include <xalanc/XalanDOM/XalanNodeListDummy.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "XStringBase.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanNodeListDummy     s_emptyChildren;

static const XalanDOMString         s_emptyString(XalanMemMgrs::getDummyMemMgr());



XObjectResultTreeFragProxyText::XObjectResultTreeFragProxyText(
                const XObject&          theXObject,
                MemoryManager&          theManager,
                XPathExecutionContext*  theExecutionContext) :
    XalanText(),
    m_value(theXObject),
    m_memoryManager(theManager),
    m_executionContext(theExecutionContext)
{
}



XObjectResultTreeFragProxyText::~XObjectResultTreeFragProxyText()
{
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getNodeName() const
{
    return s_nameString;
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getNodeValue() const
{
    if (m_executionContext != 0)
    {
        return m_value.str(*m_executionContext);
    }
    else
    {
        return m_value.str();
    }
}



XObjectResultTreeFragProxyText::NodeType
XObjectResultTreeFragProxyText::getNodeType() const
{
    return TEXT_NODE;
}



XalanNode*
XObjectResultTreeFragProxyText::getParentNode() const
{
    return 0;
}



const XalanNodeList*
XObjectResultTreeFragProxyText::getChildNodes() const
{
    return &s_emptyChildren;
}



XalanNode*
XObjectResultTreeFragProxyText::getFirstChild() const
{
    return 0;
}



XalanNode*
XObjectResultTreeFragProxyText::getLastChild() const
{
    return 0;
}



XalanNode*
XObjectResultTreeFragProxyText::getPreviousSibling() const
{
    return 0;
}



XalanNode*
XObjectResultTreeFragProxyText::getNextSibling() const
{
    return 0;
}



const XalanNamedNodeMap*
XObjectResultTreeFragProxyText::getAttributes() const
{
    return 0;
}



XalanDocument*
XObjectResultTreeFragProxyText::getOwnerDocument() const
{
    return 0;
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getNamespaceURI() const
{
    return s_emptyString;
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getPrefix() const
{
    return s_emptyString;
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getLocalName() const
{
    return s_emptyString;
}



bool
XObjectResultTreeFragProxyText::isIndexed() const
{
    return false;
}



XObjectResultTreeFragProxyText::IndexType
XObjectResultTreeFragProxyText::getIndex() const
{
    return 0;
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getData() const
{
    return getNodeValue();
}



bool
XObjectResultTreeFragProxyText::isWhitespace() const
{
    return isXMLWhitespace(getNodeValue());
}



static XALAN_CPP_NAMESPACE_QUALIFIER XalanDOMString     s_localNameString(XalanMemMgrs::getDummyMemMgr());



const XalanDOMString&   XObjectResultTreeFragProxyText::s_nameString = s_localNameString;



void
XObjectResultTreeFragProxyText::initialize(MemoryManager&   theManager)
{
    XalanDOMString tmpString("#text", theManager);

    s_localNameString.swap(tmpString);
}



void
XObjectResultTreeFragProxyText::terminate()
{
    releaseMemory(s_localNameString, XalanMemMgrs::getDummyMemMgr());
}



XALAN_CPP_NAMESPACE_END
