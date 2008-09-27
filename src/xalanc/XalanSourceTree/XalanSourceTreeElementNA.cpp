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
#include "XalanSourceTreeElementNA.hpp"


#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>


#include <xalanc/XalanDOM/XalanDOMException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XalanEmptyNamedNodeMap    XalanSourceTreeElementNA::s_emptyAttributes;



XalanSourceTreeElementNA::XalanSourceTreeElementNA(
            MemoryManager&          theManager,
            const XalanDOMString&       theTagName,
            XalanSourceTreeDocument*    theOwnerDocument,
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
        theIndex)
{
}



XalanSourceTreeElementNA::~XalanSourceTreeElementNA()
{
}



const XalanNamedNodeMap*
XalanSourceTreeElementNA::getAttributes() const
{
    return &s_emptyAttributes;
}



const XalanDOMString&
XalanSourceTreeElementNA::getNamespaceURI() const
{
    return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeElementNA::getPrefix() const
{
    return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeElementNA::getLocalName() const
{
    return m_tagName;
}



XALAN_CPP_NAMESPACE_END
