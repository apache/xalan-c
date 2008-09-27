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
#include "XPathFactoryBlock.hpp"



#include <algorithm>



XALAN_CPP_NAMESPACE_BEGIN



XPathFactoryBlock::XPathFactoryBlock(
            MemoryManager&  theManager,
            XalanSize_t         theBlockSize) :
    XPathFactory(),
    m_allocator(theManager, theBlockSize)
{
}



XPathFactoryBlock::~XPathFactoryBlock()
{
}



void
XPathFactoryBlock::reset()
{
    m_allocator.reset();
}



bool
XPathFactoryBlock::doReturnObject(
            const XPath*    theXPath,
            bool            /* fInReset */)
{
    // We never really delete anything, but return
    // false if our allocator didn't create the instance.
    return m_allocator.ownsObject(theXPath);
}



XPath*
XPathFactoryBlock::create()
{
    return m_allocator.create();
}



XALAN_CPP_NAMESPACE_END
