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
#include "XercesLiaisonXalanDOMStringPool.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesLiaisonXalanDOMStringPool::XercesLiaisonXalanDOMStringPool(MemoryManager& theManager) :
    ParentType(theManager),
    m_mutex(&theManager)
{
}



XercesLiaisonXalanDOMStringPool*
XercesLiaisonXalanDOMStringPool::    create(MemoryManager& theManager)
{
    typedef XercesLiaisonXalanDOMStringPool ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(theManager);

    theGuard.release();

    return theResult;
}



XercesLiaisonXalanDOMStringPool::~XercesLiaisonXalanDOMStringPool()
{
}



void
XercesLiaisonXalanDOMStringPool::clear()
{
    XMLMutexLockType    theLock(&m_mutex);

    ParentType::clear();
}



size_t
XercesLiaisonXalanDOMStringPool::size() const
{
    XMLMutexLockType    theLock(&m_mutex);

    return ParentType::size();
}



const XalanDOMString&
XercesLiaisonXalanDOMStringPool::get(const XalanDOMString&  theString)
{
    XMLMutexLockType    theLock(&m_mutex);

    return ParentType::get(theString);
}



const XalanDOMString&
XercesLiaisonXalanDOMStringPool::get(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength)
{
    XMLMutexLockType    theLock(&m_mutex);

    return ParentType::get(theString, theLength);
}



XALAN_CPP_NAMESPACE_END
