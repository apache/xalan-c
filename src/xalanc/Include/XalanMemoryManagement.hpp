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



#include <cstddef>
#include <new>



#include <xercesc/framework/MemoryManager.hpp>




XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER MemoryManager    MemoryManagerType;
XALAN_USING_XERCES(MemoryManager)



class XalanAllocationGuard
{
public:

#if defined(XALAN_STRICT_ANSI_HEADERS)
    typedef std::size_t     size_type;
#else
    typedef size_t          size_type;
#endif

    XalanAllocationGuard(
                MemoryManager&  theMemoryManager,
                void*           thePointer) :
        m_memoryManager(theMemoryManager),
        m_pointer(thePointer)
    {
    }

    XalanAllocationGuard(
                MemoryManager&  theMemoryManager,
                size_type       theSize) :
        m_memoryManager(theMemoryManager),
        m_pointer(theMemoryManager.allocate(theSize))
    {
    }

    ~XalanAllocationGuard()
    {
        if (m_pointer != 0)
        {
            m_memoryManager.deallocate(m_pointer);
        }
    }

    void*
    get() const
    {
        return m_pointer;
    }

    void
    release()
    {
        m_pointer = 0;
    }

private:

    // Data members...
    MemoryManager&  m_memoryManager;

    void*               m_pointer;
};



template<class Type>
void
XalanDestroy(Type&  theArg)
{
    theArg.~Type();
}



template<class Type>
void
XalanDestroy(Type*  theArg)
{
    if (theArg != 0)
    {
        theArg->~Type();
    }
}



template<class Type>
void
XalanDestroy(
            MemoryManager&  theMemoryManager,
            Type*           theArg)
{
    if (theArg != 0)
    {
        XalanDestroy(*theArg);

        theMemoryManager.deallocate(theArg);
    }
}



template<class Type>
void
XalanDestroy(
            MemoryManager&  theMemoryManager,
            Type&           theArg)
{
    XalanDestroy(theArg);

    theMemoryManager.deallocate(&theArg);
}



template<class Type>
Type*
XalanConstruct(
            MemoryManager&  theMemoryManager,
            Type*&          theInstance)
{
    XalanAllocationGuard    theGuard(
                                theMemoryManager,
                                sizeof(Type));

    theInstance =
        new (theGuard.get()) Type;

    theGuard.release();

    return theInstance;
}



template<
    class Type,
    class Param1Type>
Type*
XalanConstruct(
            MemoryManager&      theMemoryManager,
            Type*&              theInstance,
            const Param1Type&   theParam1)
{
    XalanAllocationGuard    theGuard(
                                theMemoryManager,
                                sizeof(Type));

    theInstance =
        new (theGuard.get()) Type(theParam1);

    theGuard.release();

    return theInstance;
}



template<
    class Type,
    class Param1Type>
Type*
XalanConstruct(
            MemoryManager&  theMemoryManager,
            Type*&          theInstance,
            Param1Type&     theParam1)
{
    XalanAllocationGuard    theGuard(
                                theMemoryManager,
                                sizeof(Type));

    theInstance =
        new (theGuard.get()) Type(theParam1);

    theGuard.release();

    return theInstance;
}



template<
    class Type,
    class Param1Type,
    class Param2Type>
Type*
XalanConstruct(
            MemoryManager&      theMemoryManager,
            Type*&              theInstance,
            Param1Type&         theParam1,
            const Param2Type&   theParam2)
{
    XalanAllocationGuard    theGuard(
                                theMemoryManager,
                                sizeof(Type));

    theInstance =
        new (theGuard.get()) Type(theParam1, theParam2);

    theGuard.release();

    return theInstance;
}



template<
    class Type,
    class Param1Type,
    class Param2Type,
    class Param3Type>
Type*
XalanConstruct(
            MemoryManager&      theMemoryManager,
            Type*&              theInstance,
            Param1Type&         theParam1,
            const Param2Type&   theParam2,
            Param3Type&         theParam3)
{
    XalanAllocationGuard    theGuard(
                                theMemoryManager,
                                sizeof(Type));

    theInstance =
        new (theGuard.get()) Type(theParam1, theParam2, theParam3);

    theGuard.release();

    return theInstance;
}



template<
    class Type,
    class Param1Type,
    class Param2Type,
    class Param3Type,
    class Param4Type,
    class Param5Type>
Type*
XalanConstruct(
            MemoryManager&      theMemoryManager,
            Type*&              theInstance,
            Param1Type&         theParam1,
            Param2Type&         theParam2,
            const Param3Type&   theParam3,
            const Param4Type&   theParam4,
            const Param5Type&   theParam5)
{
    XalanAllocationGuard    theGuard(
                                theMemoryManager,
                                sizeof(Type));

    theInstance =
        new (theGuard.get()) Type(theParam1, theParam2, theParam3, theParam4, theParam5);

    theGuard.release();

    return theInstance;
}



template<
    class Type,
    class Param1Type,
    class Param2Type,
    class Param3Type,
    class Param4Type,
    class Param5Type,
    class Param6Type>
Type*
XalanConstruct(
            MemoryManager&      theMemoryManager,
            Type*&              theInstance,
            Param1Type&         theParam1,
            Param2Type&         theParam2,
            const Param3Type&   theParam3,
            const Param4Type&   theParam4,
            const Param5Type&   theParam5,
            const Param6Type&   theParam6)
{
    XalanAllocationGuard    theGuard(
                                theMemoryManager,
                                sizeof(Type));

    theInstance =
        new (theGuard.get()) Type(theParam1, theParam2, theParam3, theParam4, theParam5, theParam6);

    theGuard.release();

    return theInstance;
}



template<class Type>
Type*
XalanCopyConstruct(
            MemoryManager&  theMemoryManager,
            const Type&     theSource)
{
    XalanAllocationGuard    theGuard(
                                theMemoryManager,
                                sizeof(Type));

    Type* const     theInstance =
        new (theGuard.get()) Type(theSource);

    theGuard.release();

    return theInstance;
}



template<
    class Type,
    class Param1Type>
Type*
XalanCopyConstruct(
            MemoryManager&  theMemoryManager,
            const Type&     theSource,
            Param1Type&     theParam1)
{
    XalanAllocationGuard    theGuard(
                                theMemoryManager,
                                sizeof(Type));

    Type* const     theInstance =
        new (theGuard.get()) Type(theSource, theParam1);

    theGuard.release();

    return theInstance;
}



class XALAN_PLATFORM_EXPORT XalanMemMgrs
{
public:

    static MemoryManager&
    getDummyMemMgr();

    static MemoryManager&
    getDefaultXercesMemMgr();

    static MemoryManager&
    getDefault()
    {
        return getDefaultXercesMemMgr();
    }
};




#if defined (XALAN_DEVELOPMENT)
#define XALAN_DEFAULT_CONSTRUCTOR_MEMORY_MGR
#define XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR
#define XALAN_DEFAULT_MEMMGR = XalanMemMgrs::getDummyMemMgr()
#else
#define XALAN_DEFAULT_CONSTRUCTOR_MEMORY_MGR = XalanMemMgrs::getDefaultXercesMemMgr()
#define XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR XALAN_DEFAULT_CONSTRUCTOR_MEMORY_MGR
#define XALAN_DEFAULT_MEMMGR = XalanMemMgrs::getDefaultXercesMemMgr()
#endif



template <class C>
struct ConstructValueWithNoMemoryManager 
{ 
    ConstructValueWithNoMemoryManager(MemoryManager& /*mgr*/) :
        value()
    {
    }

    C value;
};

template <class C>
struct ConstructValueWithMemoryManager
{   
    ConstructValueWithMemoryManager(MemoryManager& mgr) :
        value(mgr)
    {
    }

    C value;
};

template <class C>
struct ConstructWithNoMemoryManager
{
    typedef ConstructValueWithNoMemoryManager<C>   ConstructableType;

    static C* construct(C* address, MemoryManager& /* mgr */)
    {
        return (C*) new (address) C;
    }

    static C* construct(C* address, const C& theRhs, MemoryManager& /* mgr */)
    {
        return (C*) new (address) C(theRhs);
    }
};

template <class C>
struct ConstructWithMemoryManager
{
    typedef ConstructValueWithMemoryManager<C>    ConstructableType;

    static C* construct(C* address, MemoryManager& mgr)
    {
        return (C*) new (address) C(mgr);
    }

    static C* construct(C* address, const C& theRhs, MemoryManager& mgr)
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
template<> \
struct MemoryManagedConstructionTraits<Type> \
    { \
        typedef ConstructWithMemoryManager<Type> Constructor; \
    };

template <class C>
struct ConstructWithMemoryManagerTraits 
{
    typedef ConstructWithMemoryManager<C>       Constructor;
};

template <class C>
struct ConstructWithNoMemoryManagerTraits
{
    typedef ConstructWithNoMemoryManager<C> Constructor;
};




XALAN_CPP_NAMESPACE_END



#endif  // XALANMEMORYMANAGEMENT_HEADER_GUARD_1357924680


