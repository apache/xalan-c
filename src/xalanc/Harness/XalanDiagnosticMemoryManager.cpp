/*
 * Copyright 1999-2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "XalanDiagnosticMemoryManager.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanDiagnosticMemoryManager::XalanDiagnosticMemoryManager(
            MemoryManager&  theMemoryManager,
            bool            fAssertErrors) :
    m_memoryManager(theMemoryManager),
    m_assertErrors(fAssertErrors),
    m_locked(false),
    m_sequence(0),
    m_highWaterMark(0),
    m_currentAllocated(0),
    m_allocations(theMemoryManager)
{
}



XalanDiagnosticMemoryManager::~XalanDiagnosticMemoryManager()
{
    if (m_allocations.size() > 0)
    {
//        std::cerr << "Detected memory leaks. "
//                  << m_allocations.size()
//                  << " blocks are still allocated.";
    }
}



void*
XalanDiagnosticMemoryManager::allocate(size_type  size)
{
    void*   theResult = 0;

    if (m_locked == true)
    {
        throw LockException();
    }
    else
    {
        theResult =
            m_memoryManager.allocate(size);
        assert(theResult != 0);

        assert(m_allocations.find(theResult) == m_allocations.end());

        m_currentAllocated += size;

        if (m_currentAllocated > m_highWaterMark)
        {
            m_highWaterMark = m_currentAllocated;
        }

        m_allocations.insert(MapType::value_type(theResult, Data(size, m_sequence++)));
    }

    return theResult;
}



void
XalanDiagnosticMemoryManager::deallocate(void*    pointer)
{
    if (m_locked == true)
    {
        throw LockException();
    }
    else
    {
        if (pointer != 0)
        {
            MapType::iterator   i =
                m_allocations.find(pointer);
            
            if (i != m_allocations.end())
            {
                m_memoryManager.deallocate(pointer);
                
                assert(m_currentAllocated >= i->second.m_size);
                
                m_currentAllocated -= i->second.m_size;
                
                m_allocations.erase(i);
            }
            else
            {
                //std::cerr << "Attempt to free unallocated pointer "
                //    << pointer
                //    << ".\n";

                assert(!m_assertErrors);
            }
        }
    }
}


XALAN_CPP_NAMESPACE_END
