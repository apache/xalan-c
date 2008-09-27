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
// Class header file.
#include "NodeRefList.hpp"



#include <cassert>
#include <algorithm>

#if !defined(NDEBUG)
#include <xalanc/Include/XalanSet.hpp>
#endif



XALAN_CPP_NAMESPACE_BEGIN



NodeRefList::NodeRefList(MemoryManager& theManager) :
    NodeRefListBase(),
    m_nodeList(theManager)
{
}



NodeRefList::NodeRefList(const NodeRefList&     theSource,
                         MemoryManager& theManager) :
    NodeRefListBase(theSource),
    m_nodeList(theSource.m_nodeList, theManager)
{
}



NodeRefList::NodeRefList(const NodeRefListBase&     theSource,
                         MemoryManager& theManager) :
    NodeRefListBase(theSource),
    m_nodeList(theManager)
{
    *this = theSource;
}



NodeRefList::~NodeRefList()
{
}



NodeRefList&
NodeRefList::operator=(const NodeRefListBase&   theRHS)
{
    if (&theRHS != this)
    {
        m_nodeList.clear();

        const size_type     theLength = theRHS.getLength();

        m_nodeList.reserve(theLength);

        for(size_type i = 0; i < theLength; i++)
        {
            XalanNode* const    theNode = theRHS.item(i);

            if (theNode != 0)
            {
                m_nodeList.push_back(theNode);
            }
        }

        // Chain up...
        NodeRefListBase::operator=(theRHS);
    }

    return *this;
}



NodeRefList&
NodeRefList::operator=(const NodeRefList&   theRHS)
{
    if (&theRHS != this)
    {
        m_nodeList = theRHS.m_nodeList;

        // Chain up...
        NodeRefListBase::operator=(theRHS);
    }

    return *this;
}



XalanNode*
NodeRefList::item(size_type     index) const
{
    assert(index < m_nodeList.size());

    return m_nodeList[index];
}



NodeRefList::size_type
NodeRefList::getLength() const
{
    assert(size_type(m_nodeList.size()) == m_nodeList.size());

    return size_type(m_nodeList.size());
}



NodeRefList::size_type
NodeRefList::indexOf(const XalanNode*   theNode) const
{
    XALAN_USING_STD(find)

    // Look for the node in the list.
    NodeListVectorType::const_iterator  i =
        find(
            m_nodeList.begin(),
            m_nodeList.end(),
            theNode);

    // If not found, return npos.  Otherwise, subtract the iterator
    // from the first iterator to get the distance between them.
    return i == m_nodeList.end() ? npos : i - m_nodeList.begin();
}



#if !defined(NDEBUG)
bool
NodeRefList::checkForDuplicates(MemoryManager& theManager) const
{
    typedef XalanSet<const XalanNode*>  NodeSetType;

    bool    fResult = false;

    const size_type     theLength = getLength();

    if (theLength > 0)
    {
        NodeSetType     theNodes(theManager);

        for (size_type i = 0; i < theLength && fResult == false; ++i)
        {
            const XalanNode* const  theNode = item(i);

            if (theNodes.find(theNode) != theNodes.end())
            {
                fResult = true;
            }
            else
            {
                theNodes.insert(theNode);
            }
        }
    }

    return fResult;
}
#endif



XALAN_CPP_NAMESPACE_END
