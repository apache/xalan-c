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

#include "XercesNamedNodeMapAttributeList.hpp"


#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNode.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XMLCh     XercesNamedNodeMapAttributeList::s_typeString[] = 
{
    XalanUnicode::charLetter_C,
    XalanUnicode::charLetter_D,
    XalanUnicode::charLetter_A,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_A,
    0
};



XercesNamedNodeMapAttributeList::XercesNamedNodeMapAttributeList(const DOMNamedNodeMapType*     theMap) :
    ParentType(),
    m_nodeMap(theMap),
    m_attrCount(theMap->getLength())
{
}



XercesNamedNodeMapAttributeList::~XercesNamedNodeMapAttributeList()
{
}



XalanSize_t
XercesNamedNodeMapAttributeList::getLength() const
{
    return m_attrCount;
}



const XMLCh*
XercesNamedNodeMapAttributeList::getName(const XalanSize_t  index) const
{
    if (m_attrCount == 0)
        return 0;

    if (index >= m_attrCount)
        return 0;

    const DOMNodeType* const    theAttribute = m_nodeMap->item(index);
    return theAttribute->getNodeName();
}



const XMLCh*
XercesNamedNodeMapAttributeList::getType(const XalanSize_t  /* index */) const
{
    assert(length(s_typeString) > 0);

    return s_typeString;
}



const XMLCh*
XercesNamedNodeMapAttributeList::getValue(const XalanSize_t     index) const
{
    if (m_attrCount == 0)
        return 0;

    if (index >= m_attrCount)
        return 0;

    const DOMNodeType* const    theAttribute = m_nodeMap->item(index);

    return theAttribute->getNodeValue();
}



const XMLCh*
XercesNamedNodeMapAttributeList::getType(const XMLCh* const /* name */) const
{
    assert(length(s_typeString) > 0);

    return s_typeString;
}



const XMLCh*
XercesNamedNodeMapAttributeList::getValue(const XMLCh* const name) const
{
    const DOMNodeType*  theNode = m_nodeMap->getNamedItem(name);

    if (theNode == 0)
    {
        return 0;
    }
    else
    {
        return theNode->getNodeValue();
    }
}

const XMLCh* 
XercesNamedNodeMapAttributeList::getValue(const char* const /*name*/) const
{
    assert(false);

    return 0;
}




XALAN_CPP_NAMESPACE_END
