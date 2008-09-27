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
#include "XalanSourceTreeDOMSupport.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanAttr.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeParserLiaison.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString     s_emptyString(XalanMemMgrs::getDummyMemMgr());



XalanSourceTreeDOMSupport::XalanSourceTreeDOMSupport() :
    DOMSupport(),
    m_parserLiaison(0)
{
}



XalanSourceTreeDOMSupport::XalanSourceTreeDOMSupport(const XalanSourceTreeParserLiaison&    theParserLiaison) :
    DOMSupport(),
    m_parserLiaison(&theParserLiaison)
{
}



XalanSourceTreeDOMSupport::~XalanSourceTreeDOMSupport()
{
}



void
XalanSourceTreeDOMSupport::reset()
{
}



const XalanDOMString&
XalanSourceTreeDOMSupport::getUnparsedEntityURI(
            const XalanDOMString&   theName,
            const XalanDocument&    theDocument) const
{
    if (m_parserLiaison != 0)
    {
        const XalanSourceTreeDocument* const    theXSTDocument =
            m_parserLiaison->mapDocument(&theDocument);

        if (theXSTDocument != 0)
        {
            return theXSTDocument->getUnparsedEntityURI(theName);
            
        }
    }

    return s_emptyString;
}



bool
XalanSourceTreeDOMSupport::isNodeAfter(
            const XalanNode&    node1,
            const XalanNode&    node2) const
{
    assert(node1.isIndexed() == true && node2.isIndexed() == true);

    return node1.getIndex() > node2.getIndex() ? true : false;
}



XALAN_CPP_NAMESPACE_END
