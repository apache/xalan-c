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
#include "XalanSourceTreeElementANS.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanSourceTreeElementANS::XalanSourceTreeElementANS(
            MemoryManager&              theManager,
            const XalanDOMString&       theTagName,
            const XalanDOMString&       theLocalName,
            const XalanDOMString&       theNamespaceURI,
            const XalanDOMString&       thePrefix,
            XalanSourceTreeDocument*    theOwnerDocument,
            XalanSourceTreeAttr**       theAttributes,
            XalanSize_t                 theAttributeCount,
            XalanNode*                  theParentNode,
            XalanNode*                  thePreviousSibling,
            XalanNode*                  theNextSibling,
            IndexType                   theIndex) :
    XalanSourceTreeElementA(
        theManager,
        theTagName,
        theOwnerDocument,
        theAttributes,
        theAttributeCount,
        theParentNode,
        thePreviousSibling,
        theNextSibling,
        theIndex),
    m_localName(theLocalName),
    m_prefix(thePrefix),
    m_namespaceURI(theNamespaceURI)
{
}



XalanSourceTreeElementANS::~XalanSourceTreeElementANS()
{
}


const XalanDOMString&
XalanSourceTreeElementANS::getNamespaceURI() const
{
    return m_namespaceURI;
}



const XalanDOMString&
XalanSourceTreeElementANS::getPrefix() const
{
    return m_prefix;
}



const XalanDOMString&
XalanSourceTreeElementANS::getLocalName() const
{
    return m_localName;
}



XALAN_CPP_NAMESPACE_END
