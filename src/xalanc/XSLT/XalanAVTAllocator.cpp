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
#include "XalanAVTAllocator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanAVTAllocator::XalanAVTAllocator(MemoryManager&  theManager, size_type  theBlockCount) :
    m_allocator(theManager, theBlockCount)
{
}



XalanAVTAllocator::~XalanAVTAllocator()
{
}



XalanAVTAllocator::data_type*
XalanAVTAllocator::create(
            StylesheetConstructionContext&  constructionContext,
            const Locator*                  locator,
            const XalanDOMChar*             name,
            const XalanDOMChar*             stringedValue,
            const PrefixResolver&           resolver)
{
    data_type* const    theBlock = m_allocator.allocateBlock();
    assert(theBlock != 0);

    data_type* const    theResult =
        new(theBlock) data_type(
                constructionContext,
                locator,
                name,
                stringedValue,
                resolver);

    m_allocator.commitAllocation(theBlock);

    return theResult;
}



XALAN_CPP_NAMESPACE_END
