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
#include "XNumberAllocator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XNumberAllocator::XNumberAllocator(
            MemoryManager&  theMemoryManager,
            size_type       theBlockCount) :
    m_allocator(theMemoryManager, theBlockCount)
{
}



XNumberAllocator::~XNumberAllocator()
{
}



XNumberAllocator::number_type*
XNumberAllocator::createNumber(double   theNumber) 
{
    number_type* const  theBlock = m_allocator.allocateBlock();
    assert(theBlock != 0);

    number_type* const  theResult = new(theBlock) number_type(theNumber, m_allocator.getMemoryManager());

    m_allocator.commitAllocation(theBlock);

    return theResult;
}




bool
XNumberAllocator::destroy(number_type*  theNumber)
{
    return m_allocator.destroyObject(theNumber);
}



void 
XNumberAllocator::reset()
{
    m_allocator.reset();
}



XALAN_CPP_NAMESPACE_END
