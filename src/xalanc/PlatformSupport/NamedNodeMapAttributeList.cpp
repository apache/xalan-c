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

#include "NamedNodeMapAttributeList.hpp"


#include <xalanc/XalanDOM/XalanAttr.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



#include "DOMStringHelper.hpp"
#include "XalanUnicode.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar  NamedNodeMapAttributeList::s_typeString[] = 
{
    XalanUnicode::charLetter_C,
    XalanUnicode::charLetter_D,
    XalanUnicode::charLetter_A,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_A,
    0
};



NamedNodeMapAttributeList::NamedNodeMapAttributeList(
            const XalanNamedNodeMap&    theMap,
            MemoryManager&              theManager) :
    ParentType(),
    m_nodeMap(theMap),
    m_length(theMap.getLength()),
    m_memoryManager(theManager)
{
    assert(length(s_typeString) > 0);
}



NamedNodeMapAttributeList::~NamedNodeMapAttributeList()
{
}



XalanSize_t
NamedNodeMapAttributeList::getLength() const
{
    return m_nodeMap.getLength();
}



const XMLCh*
NamedNodeMapAttributeList::getName(const XalanSize_t    index) const
{
    if (index >= m_length)
    {
        return 0;
    }
    else
    {
        const XalanNode* const  theAttribute = m_nodeMap.item(m_length - 1 - index);
        assert(theAttribute != 0);

        return theAttribute->getNodeName().c_str();
    }
}



const XMLCh*
NamedNodeMapAttributeList::getType(const XalanSize_t    index) const
{
    if (index >= m_length)
    {
        return 0;
    }
    else
    {
        return s_typeString;
    }
}



const XMLCh*
NamedNodeMapAttributeList::getValue(const XalanSize_t   index) const
{
    if (index >= m_length)
    {
        return 0;
    }
    else
    {
        const XalanNode* const  theAttribute = m_nodeMap.item(m_length - 1 - index);
        assert(theAttribute != 0);

        return theAttribute->getNodeValue().c_str();
    }
}



const XMLCh*
NamedNodeMapAttributeList::getType(const XMLCh* const   name) const
{
    const XalanNode*    theNode = m_nodeMap.getNamedItem(XalanDOMString(name, m_memoryManager));

    if (theNode == 0)
    {
        return 0;
    }
    else
    {
        return s_typeString;
    }
}



const XMLCh*
NamedNodeMapAttributeList::getValue(const XMLCh* const  name) const
{
    const XalanNode*    theNode = m_nodeMap.getNamedItem(XalanDOMString(name, m_memoryManager));

    if (theNode == 0)
    {
        return 0;
    }
    else
    {
        return theNode->getNodeValue().c_str();
    }
}



const XMLCh* 
NamedNodeMapAttributeList::getValue(const char* const name) const
{
    XalanDOMString  theBuffer(m_memoryManager);

    return getValue(TranscodeFromLocalCodePage(name, theBuffer).c_str());
}



XALAN_CPP_NAMESPACE_END
