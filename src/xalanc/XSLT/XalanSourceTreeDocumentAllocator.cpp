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
#include "XalanSourceTreeDocumentAllocator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanSourceTreeDocumentAllocator::XalanSourceTreeDocumentAllocator(MemoryManager&  theManager, size_type    theBlockCount) :
    m_allocator(theManager, theBlockCount)
{
}



XalanSourceTreeDocumentAllocator::~XalanSourceTreeDocumentAllocator()
{
}




XalanSourceTreeDocumentAllocator::data_type*
XalanSourceTreeDocumentAllocator::create(
            bool                fPoolAllText,
            block_size_type     theNamesStringPoolBlockSize,
            bucket_count_type   theNamesStringPoolBucketCount,
            bucket_size_type    theNamesStringPoolBucketSize,
            block_size_type     theValuesStringPoolBlockSize,
            bucket_count_type   theValuesStringPoolBucketCount,
            bucket_size_type    theValuesStringPoolBucketSize)
{
    data_type* const    theBlock = m_allocator.allocateBlock();
    assert(theBlock != 0);

    data_type* const    theResult = new(theBlock) data_type(
                m_allocator.getMemoryManager(),
                fPoolAllText,
                theNamesStringPoolBlockSize,
                theNamesStringPoolBucketCount,
                theNamesStringPoolBucketSize,
                theValuesStringPoolBlockSize,
                theValuesStringPoolBucketCount,
                theValuesStringPoolBucketSize);

    m_allocator.commitAllocation(theBlock);

    return theResult;
}



XalanSourceTreeDocumentAllocator::data_type*
XalanSourceTreeDocumentAllocator::create(
            size_type   theAttributeBlockSize,
            size_type   theAttributeNSBlockSize,
            size_type   theCommentBlockSize,
            size_type   theElementBlockSize,
            size_type   theElementNSBlockSize,
            size_type   thePIBlockSize,
            size_type   theTextBlockSize,
            size_type   theTextIWSBlockSize,
            bool        fPoolAllText)
{
    data_type* const    theBlock = m_allocator.allocateBlock();
    assert(theBlock != 0);

    data_type* const    theResult = new(theBlock) data_type(
                m_allocator.getMemoryManager(),
                theAttributeBlockSize,
                theAttributeNSBlockSize,
                theCommentBlockSize,
                theElementBlockSize,
                theElementNSBlockSize,
                thePIBlockSize,
                theTextBlockSize,
                theTextIWSBlockSize,
                fPoolAllText);

    m_allocator.commitAllocation(theBlock);

    return theResult;
}



bool
XalanSourceTreeDocumentAllocator::destroy(data_type*    theObject)
{
    return m_allocator.destroyObject(theObject);
}



void 
XalanSourceTreeDocumentAllocator::reset()
{
    m_allocator.reset();
}



XALAN_CPP_NAMESPACE_END
