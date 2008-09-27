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

// Class header file...
#include "XalanDOMStringPool.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString    XalanDOMStringPool::s_emptyString(XalanMemMgrs::getDummyMemMgr());



XalanDOMStringPool::XalanDOMStringPool(
            MemoryManager&      theManager,
            block_size_type     theBlockSize,
            bucket_count_type   theBucketCount,
            bucket_size_type    theBucketSize) :
    m_stringAllocator(theManager, theBlockSize),
    m_stringCount(0),
    m_hashTable(theManager, theBucketCount, theBucketSize)
{
}



XalanDOMStringPool*
XalanDOMStringPool::create(
            MemoryManager&      theManager,
            block_size_type     theBlockSize,
            bucket_count_type   theBucketCount,
            bucket_size_type    theBucketSize)
{
    typedef XalanDOMStringPool ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(theManager, theBlockSize, theBucketCount, theBucketSize);

    theGuard.release();

    return theResult;
}



XalanDOMStringPool::~XalanDOMStringPool()
{
}



void
XalanDOMStringPool::clear()
{
    m_stringAllocator.reset();

    m_hashTable.clear();

    m_stringCount = 0;
}



size_t
XalanDOMStringPool::size() const
{
    assert(m_stringCount == m_hashTable.size());

    return m_stringCount;
}



const XalanDOMString&
XalanDOMStringPool::get(const XalanDOMString&   theString)
{
    return get(theString.c_str(), theString.length());
}



const XalanDOMString&
XalanDOMStringPool::get(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength)
{
    assert(m_stringCount == m_hashTable.size());

    if (theString == 0 || *theString == 0)
    {
        return s_emptyString;
    }
    else
    {
        const XalanDOMString::size_type     theActualLength = theLength == XalanDOMString::npos ? length(theString) : theLength;

        size_t  theBucketIndex;

        const XalanDOMString*   theTableString = m_hashTable.find(theString, theActualLength, &theBucketIndex);

        if (theTableString != 0)
        {
            return *theTableString;
        }
        else
        {
            // Not found, so insert the string...
            XalanDOMString* const   theNewString =
                m_stringAllocator.create(theString, theActualLength);
            assert(theNewString != 0);

            assert(theActualLength == theNewString->length());

            ++m_stringCount;

            // Insert the string into the hash table...
            m_hashTable.insert(*theNewString, theBucketIndex);

            assert(m_stringCount == m_hashTable.size());

            return *theNewString;
        }
    }
}



XALAN_CPP_NAMESPACE_END
