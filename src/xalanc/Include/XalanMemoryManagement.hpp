/*
 * Copyright 1999-2004 The Apache Software Foundation.
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
#if !defined(XALANMEMORYMANAGEMENT_HEADER_GUARD_1357924680)
#define XALANMEMORYMANAGEMENT_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>

#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <cassert>


XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER MemoryManager		MemoryManagerType;



template <class C>
struct ConstructWithNoMemoryManager
{
    static C* construct(C* address, MemoryManagerType& /* mgr */)
    {
        return (C*) new (address) C;
    }

    static C* construct(C* address, const C& theRhs, MemoryManagerType& /* mgr */)
    {
        return (C*) new (address) C(theRhs);
    }
};

template <class C>
struct ConstructWithMemoryManager
{
    static C* construct(C* address, MemoryManagerType& mgr)
    {
        return (C*) new (address) C(mgr);
    }

    static C* construct(C* address, const C& theRhs, MemoryManagerType& mgr)
    {
        return (C*) new (address) C(theRhs, mgr);
    }
};

template <class C>
struct MemoryManagedConstructionTraits
{
    typedef ConstructWithNoMemoryManager<C> Constructor;
};

#define  XALAN_USES_MEMORY_MANAGER(Type)  \
XALAN_USING_XALAN(MemoryManagedConstructionTraits) \
XALAN_USING_XALAN(ConstructWithMemoryManager) \
template<> \
struct MemoryManagedConstructionTraits<Type> \
    { \
        typedef ConstructWithMemoryManager<Type> Constructor; \
    };



XALAN_CPP_NAMESPACE_END



#endif  // XALANMEMORYMANAGEMENT_HEADER_GUARD_1357924680


