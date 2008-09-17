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
#include "XalanDOMStringHashTable.hpp"



#include <algorithm>



#include "DOMStringHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanDOMStringHashTable::XalanDOMStringHashTable(
            MemoryManager&  theManager,
            size_t          theBucketCount,
            size_t          theBucketSize) :

    m_bucketCount(theBucketCount),
    m_bucketSize(theBucketSize),
    m_buckets(theManager),
    m_count(0),
    m_collisions(0)
{
    m_buckets.resize(theBucketCount);
}



void
XalanDOMStringHashTable::clear()
{
    for(size_t i = 0; i < m_bucketCount; ++i)
    {
        m_buckets[i].clear();
    }

#if !defined(NDEBUG)
    m_collisions = 0;
#endif
    m_count = 0;
}



void
XalanDOMStringHashTable::getBucketCounts(BucketCountsType&  theVector) const
{
    for (size_t i = 0; i < m_bucketCount; ++i)
    {
        const bucket_size_type  size = m_buckets[i].size();

        theVector.push_back(size);
    }
}



struct
equalsXalanDOMString
{
    equalsXalanDOMString(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength) :
        m_string(theString),
        m_length(theLength)
    {
    }

    bool
    operator()(const XalanDOMString*    theString) const
    {
        assert(theString != 0);

        if (m_length != theString->length())
        {
            return false;
        }
        else
        {
            return equals(m_string, theString->c_str(), m_length);
        }
    }

private:

    const XalanDOMChar* const           m_string;

    const XalanDOMString::size_type     m_length;
};



const XalanDOMString*
XalanDOMStringHashTable::find(
            const XalanDOMString&   theString,
            size_t*                 theBucketIndex) const
{
    return find(
                theString.c_str(),
                theString.length(),
                theBucketIndex);
}



const XalanDOMString*
XalanDOMStringHashTable::find(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength,
            size_t*                     theBucketIndex) const
{
    assert(theString != 0);

    const XalanDOMString::size_type     theActualLength =
        theLength == XalanDOMString::npos ? length(theString) : theLength;

    const size_t    theHash = XalanDOMString::hash(theString, theActualLength);

    const size_t    theLocalBucketIndex = theHash % m_bucketCount;

    assert(theLocalBucketIndex < m_bucketCount);

    const BucketType&   theBucket = m_buckets[theLocalBucketIndex];

    if (theBucketIndex != 0)
    {
        *theBucketIndex = theLocalBucketIndex;
    }

    XALAN_USING_STD(find_if)

    const BucketType::const_iterator    i =
        find_if(
                theBucket.begin(),
                theBucket.end(),
                equalsXalanDOMString(theString, theActualLength));

    if (i == theBucket.end())
    {
        return 0;
    }
    else
    {
        return *i;
    }
}



void
XalanDOMStringHashTable::insert(const XalanDOMString&   theString)
{
    const size_t    theHash = theString.hash();

    const size_t    theBucketIndex = theHash % m_bucketCount;

    assert(theBucketIndex < m_bucketCount);

    BucketType& theBucket = m_buckets[theBucketIndex];

#if !defined(NDEBUG)
    if (theBucket.empty() == false)
    {
        ++m_collisions;
    }
#endif

    theBucket.reserve(m_bucketSize);

    theBucket.push_back(&theString);

    ++m_count;
}



void
XalanDOMStringHashTable::insert(
            const XalanDOMString&   theString,
            size_t                  theBucketIndex)
{
    assert(theBucketIndex == theString.hash() % m_bucketCount);
    assert(theBucketIndex < m_bucketCount);

    BucketType&     theBucket = m_buckets[theBucketIndex];

#if !defined(NDEBUG)
    if (theBucket.empty() == false)
    {
        ++m_collisions;
    }
#endif

    theBucket.reserve(m_bucketSize);

    theBucket.push_back(&theString);

    ++m_count;
}



XALAN_CPP_NAMESPACE_END
