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
#if !defined(XALAN_OBJECTCACHE_HEADER_GUARD)
#define XALAN_OBJECTCACHE_HEADER_GUARD



#include <algorithm>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/STLHelper.hpp>




XALAN_CPP_NAMESPACE_BEGIN



template<class ObjectType>
class DefaultCacheCreateFunctor
{
public:

    ObjectType*
    operator()(MemoryManagerType& theManager) const
    {
        typedef ObjectType ThisType;
        
        XalanMemMgrAutoPtr<ThisType, false> theGuard( theManager , (ThisType*)theManager.allocate(sizeof(ThisType)));

        ThisType* theResult = theGuard.get();

        new (theResult) ThisType();

        theGuard.release();

        return theResult;
    }
};


template<class ObjectType>
class DefaultCacheCreateFunctorMemMgr
{
public:

    ObjectType*
    operator()(MemoryManagerType& theManager) const
    {
        typedef ObjectType ThisType;
        
        XalanMemMgrAutoPtr<ThisType, false> theGuard( theManager , (ThisType*)theManager.allocate(sizeof(ThisType)));

        ThisType* theResult = theGuard.get();

        new (theResult) ThisType(theManager);

        theGuard.release();

        return theResult;
    }
};


template<class ObjectType>
class DefaultCacheResetFunctor
{
public:

    void
    operator()(ObjectType*) const
    {
    }
};



template<class ObjectType>
class ClearCacheResetFunctor
{
public:

    void
    operator()(ObjectType*  theInstance) const
    {
        theInstance->clear();
    }
};



#if defined(XALAN_OBJECT_CACHE_KEEP_BUSY_LIST)

template<
class ObjectType,
#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
class CreateFunctorType,
class DeleteFunctorType,
class ResetFunctorType>
#else
class CreateFunctorType = DefaultCacheCreateFunctor<ObjectType>,
class DeleteFunctorType = DeleteFunctor<ObjectType>,
class ResetFunctorType = DefaultCacheResetFunctor<ObjectType> >
#endif
class XalanObjectCache
{
public:

    typedef XalanVector<ObjectType*>            VectorType;

    typedef ObjectType  CacheObjectType;

    explicit
    XalanObjectCache(
                MemoryManagerType&  theManager,
                unsigned int        initialListSize = 0) :
        m_availableList(theManager),
        m_busyList(theManager)
    {
        m_availableList.reserve(initialListSize);

        m_busyList.reserve(initialListSize);
    }

    ~XalanObjectCache()
    {
        reset();

#if !defined(XALAN_NO_STD_NAMESPACE)
        using std::for_each;
#endif

        for_each(
                m_availableList.begin(),
                m_availableList.end(),
                m_deleteFunctor(theManager));
    }

    ObjectType*
    get()
    {
        // We'll always return the back of the free list, since
        // that's the cheapest thing.
        if (m_availableList.empty() == true)
        {
            ObjectType* const   theNewObject = m_createFunctor(m_availableList.getMemoryManager());

            m_busyList.push_back(theNewObject);

            return theNewObject;
        }
        else
        {
            ObjectType* const   theObject = m_availableList.back();

            m_busyList.push_back(theObject);

            m_availableList.pop_back();

            return theObject;
        }
    }

    bool
    release(ObjectType*     theInstance)
    {
#if !defined(XALAN_NO_STD_NAMESPACE)
        using std::find;
#endif

        typedef typename VectorType::iterator   IteratorType;

        const IteratorType  i =
            find(
                m_busyList.begin(),
                m_busyList.end(),
                theInstance);

        if (i == m_busyList.end())
        {
            return false;
        }
        else
        {
            m_resetFunctor(theInstance);

            m_availableList.push_back(theInstance);

            m_busyList.erase(i);

            return true;
        }
    }

    void
    reset()
    {
        while (m_busyList.empty() == false)
        {
            ObjectType* const   theInstance = m_busyList.back();

            m_resetFunctor(theInstance);

            m_availableList.push_back(theInstance);

            m_busyList.pop_back();
        }
    }

    // Functors for various operations...
    CreateFunctorType   m_createFunctor;

    DeleteFunctorType   m_deleteFunctor;

    ResetFunctorType    m_resetFunctor;

private:

    // There are not defined...
    XalanObjectCache(const XalanObjectCache<ObjectType, CreateFunctorType, DeleteFunctorType, ResetFunctorType>&    theRHS);

    XalanObjectCache<ObjectType, CreateFunctorType, DeleteFunctorType, ResetFunctorType>&
    operator=(const XalanObjectCache<ObjectType, CreateFunctorType, DeleteFunctorType, ResetFunctorType>&   theRHS);


    // Data members...
    VectorType          m_availableList;

    VectorType          m_busyList;
};



#else



template<
class ObjectType,
#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
class CreateFunctorType,
class DeleteFunctorType,
class ResetFunctorType>
#else
class CreateFunctorType = DefaultCacheCreateFunctor<ObjectType>,
class DeleteFunctorType = DeleteFunctor<ObjectType>,
class ResetFunctorType = DefaultCacheResetFunctor<ObjectType> >
#endif
class XalanObjectCache
{
public:

    typedef XalanVector<ObjectType*>            VectorType;

    typedef ObjectType  CacheObjectType;

    explicit
    XalanObjectCache(MemoryManagerType& theManager,
                    unsigned int    initialListSize = 0) :
        m_deleteFunctor(theManager),
        m_availableList(theManager)
    {
        m_availableList.reserve(initialListSize);
    }

    ~XalanObjectCache()
    {
        reset();

#if !defined(XALAN_NO_STD_NAMESPACE)
        using std::for_each;
#endif

        for_each(
                m_availableList.begin(),
                m_availableList.end(),
                m_deleteFunctor);
    }

    ObjectType*
    get()
    {
        // We'll always return the back of the free list, since
        // that's the cheapest thing.
        if (m_availableList.empty() == true)
        {
            return m_createFunctor(m_availableList.getMemoryManager());
        }
        else
        {
            ObjectType* const   theObject = m_availableList.back();

            m_availableList.pop_back();

            return theObject;
        }
    }

    bool
    release(ObjectType*     theInstance)
    {
        m_resetFunctor(theInstance);

        m_availableList.push_back(theInstance);

        return true;
    }

    void
    reset()
    {
    }

    // Functors for various operations...
    CreateFunctorType   m_createFunctor;

    DeleteFunctorType   m_deleteFunctor;

    ResetFunctorType    m_resetFunctor;

private:

    // These are not defined...
    XalanObjectCache(const XalanObjectCache<ObjectType, CreateFunctorType, DeleteFunctorType, ResetFunctorType>&    theRHS);

    XalanObjectCache<ObjectType, CreateFunctorType, DeleteFunctorType, ResetFunctorType>&
    operator=(const XalanObjectCache<ObjectType, CreateFunctorType, DeleteFunctorType, ResetFunctorType>&   theRHS);


    // Data members...
    VectorType          m_availableList;
};



#endif



template<class XalanObjectCacheType>
class GuardCachedObject
{
public:

    typedef typename XalanObjectCacheType::CacheObjectType  CacheObjectType;

    GuardCachedObject(XalanObjectCacheType& theCache) :
        m_cache(theCache),
        m_cachedObject(theCache.get())
    {
    }

    ~GuardCachedObject()
    {
        if (m_cachedObject != 0)
        {
            m_cache.release(m_cachedObject);
        }
    }

    CacheObjectType*
    get() const
    {
        return m_cachedObject;
    }

    CacheObjectType*
    release()
    {
        CacheObjectType* const  temp = m_cachedObject;

        m_cachedObject = 0;

        return temp;
    }

private:

    // Not implemented...
    GuardCachedObject(const GuardCachedObject<XalanObjectCacheType>&);


    // Data members...
    XalanObjectCacheType&   m_cache;

    CacheObjectType*        m_cachedObject;
};



template<class ObjectType>
class XalanObjectCacheDefault :
            public XalanObjectCache<
                        ObjectType,
                        DefaultCacheCreateFunctor<ObjectType>,
                        DeleteFunctor<ObjectType>,
                        DefaultCacheResetFunctor<ObjectType> >
{
public:

    typedef XalanObjectCache<
                ObjectType,
                DefaultCacheCreateFunctor<ObjectType>,
                DeleteFunctor<ObjectType>,
                DefaultCacheResetFunctor<ObjectType> >  BaseClassType;

    explicit
    XalanObjectCacheDefault(
                MemoryManagerType&  theManager,
                unsigned int        initialListSize = 0) :
        BaseClassType(theManager, initialListSize)
    {
    }
};



template<class ObjectType>
class XalanMemoryManagerObjectCacheDefault :
            public XalanObjectCache<
                        ObjectType,
                        DefaultCacheCreateFunctorMemMgr<ObjectType>,
                        DeleteFunctor<ObjectType>,
                        DefaultCacheResetFunctor<ObjectType> >
{
public:

    typedef XalanObjectCache<
                ObjectType,
                DefaultCacheCreateFunctorMemMgr<ObjectType>,
                DeleteFunctor<ObjectType>,
                DefaultCacheResetFunctor<ObjectType> >  BaseClassType;

    explicit
    XalanMemoryManagerObjectCacheDefault(
                MemoryManagerType&  theManager,
                unsigned int        initialListSize = 0) :
        BaseClassType(theManager, initialListSize)
    {
    }
};



XALAN_CPP_NAMESPACE_END



#endif
