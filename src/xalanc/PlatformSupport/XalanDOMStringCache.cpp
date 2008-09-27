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
#include "XalanDOMStringCache.hpp"



#include <xalanc/Include/STLHelper.hpp>



#include "DOMStringHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanDOMStringCache::XalanDOMStringCache(
            MemoryManager&     theManager,
            XalanSize_t             theMaximumSize) :
    m_availableList(theManager),
    m_busyList(theManager),
    m_maximumSize(theMaximumSize),
    m_allocator(theManager, XalanDOMStringReusableAllocator::eDefaultBlockSize)
{
}



XalanDOMStringCache::~XalanDOMStringCache()
{
    clear();
}



XalanDOMString&
XalanDOMStringCache::get()
{
    if (m_availableList.empty() == true)
    {
        XalanDOMString&     theString = m_allocator.create();

        m_busyList.push_back(&theString);

        return theString;
    }
    else
    {
        XalanDOMString* const   theString = m_availableList.back();
        assert(theString != 0);

        m_availableList.pop_back();

        m_busyList.push_back(theString);

        return *theString;
    }
}



bool
XalanDOMStringCache::release(XalanDOMString&    theString)
{
    XALAN_USING_STD(find)

    StringListType::iterator    i =
        find(m_busyList.begin(),
             m_busyList.end(),
             &theString);

    if (i == m_busyList.end())
    {
        return false;
    }
    else
    {
        if (m_availableList.size() > m_maximumSize)
        {
            m_allocator.destroy(theString);
        }
        else
        {
            theString.erase();

            m_availableList.push_back(*i);
        }

        m_busyList.erase(i);

        return true;
    }
}



void
XalanDOMStringCache::clear()
{
    m_busyList.clear();

    m_availableList.clear();

    m_allocator.reset();
}



void
XalanDOMStringCache::reset()
{
    const StringListType::size_type     theSize =
        m_availableList.size();

    while(m_busyList.empty() == false)
    {
        assert(m_busyList.back() != 0);

        if (theSize > m_maximumSize)
        {
            m_allocator.destroy(*m_busyList.back());
        }
        else
        {
            m_busyList.back()->clear();

            m_availableList.push_back(m_busyList.back());
        }

        m_busyList.pop_back();
    }
}



XALAN_CPP_NAMESPACE_END
