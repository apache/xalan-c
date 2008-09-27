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
#include "XalanQNameByValueAllocator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanQNameByValueAllocator::XalanQNameByValueAllocator(
            MemoryManager&  theManager,
            size_type       theBlockCount) :
    m_allocator(theManager, theBlockCount)
{
}



XalanQNameByValueAllocator::~XalanQNameByValueAllocator()
{
}



XalanQNameByValueAllocator::data_type*
XalanQNameByValueAllocator::create(const XalanQNameByValue& theSource)
{
    data_type* const    theBlock = m_allocator.allocateBlock();
    assert(theBlock != 0);

    data_type* const    theResult =
        new(theBlock) data_type(theSource, m_allocator.getMemoryManager());

    m_allocator.commitAllocation(theBlock);

    return theResult;
}



XalanQNameByValueAllocator::data_type*
XalanQNameByValueAllocator::create(
            const XalanDOMString&   theNamespaceURI,
            const XalanDOMString&   theLocalPart)
{
    data_type* const    theBlock = m_allocator.allocateBlock();
    assert(theBlock != 0);

    data_type* const    theResult =
        new(theBlock) data_type(theNamespaceURI, theLocalPart,m_allocator.getMemoryManager());

    m_allocator.commitAllocation(theBlock);

    return theResult;
}



XalanQNameByValueAllocator::data_type*
XalanQNameByValueAllocator::create(
            const XalanDOMString&       qname,
            const NamespacesStackType&  namespaces,
            const Locator*              locator,
            bool                        fUseDefault)
{
    data_type* const    theBlock = m_allocator.allocateBlock();
    assert(theBlock != 0);

    data_type* const    theResult =
        new(theBlock) data_type(qname, namespaces, m_allocator.getMemoryManager(),locator, fUseDefault);

    m_allocator.commitAllocation(theBlock);

    return theResult;
}



XalanQNameByValueAllocator::data_type*
XalanQNameByValueAllocator::create(
            const XalanDOMChar*         qname,
            const NamespacesStackType&  namespaces,
            const Locator*              locator,
            bool                        fUseDefault)
{
    data_type* const    theBlock = m_allocator.allocateBlock();
    assert(theBlock != 0);

    data_type* const    theResult =
        new(theBlock) data_type(qname, namespaces, m_allocator.getMemoryManager(),locator, fUseDefault);

    m_allocator.commitAllocation(theBlock);

    return theResult;
}



XalanQNameByValueAllocator::data_type*
XalanQNameByValueAllocator::create(
            const XalanDOMString&   qname,
            const PrefixResolver*   theResolver,
            const Locator*          locator)
{
    data_type* const    theBlock = m_allocator.allocateBlock();
    assert(theBlock != 0);

    data_type* const    theResult =
        new(theBlock) data_type(qname,m_allocator.getMemoryManager(), theResolver, locator);

    m_allocator.commitAllocation(theBlock);

    return theResult;
}



XALAN_CPP_NAMESPACE_END
