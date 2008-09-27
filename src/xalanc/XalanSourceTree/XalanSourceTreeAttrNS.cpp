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
#include "XalanSourceTreeAttrNS.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanSourceTreeAttrNS::XalanSourceTreeAttrNS(
            const XalanDOMString&       theName,
            const XalanDOMString&       theLocalName,
            const XalanDOMString&       theNamespaceURI,
            const XalanDOMString&       thePrefix,
            const XalanDOMString&       theValue,
            XalanSourceTreeElement*     theOwnerElement,
            IndexType                   theIndex) :
    XalanSourceTreeAttr(
        theName,
        theValue,
        theOwnerElement,
        theIndex),
    m_localName(theLocalName),
    m_prefix(thePrefix),
    m_namespaceURI(theNamespaceURI)
{
}



XalanSourceTreeAttrNS::~XalanSourceTreeAttrNS()
{
}


const XalanDOMString&
XalanSourceTreeAttrNS::getNamespaceURI() const
{
    return m_namespaceURI;
}



const XalanDOMString&
XalanSourceTreeAttrNS::getPrefix() const
{
    return m_prefix;
}



const XalanDOMString&
XalanSourceTreeAttrNS::getLocalName() const
{
    return m_localName;
}



XALAN_CPP_NAMESPACE_END
