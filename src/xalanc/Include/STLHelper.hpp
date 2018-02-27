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
#if !defined(STLHELPERS_HEADER_GUARD_1357924680)
#define STLHELPERS_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <algorithm>
#include <functional>



#include <xalanc/Include/XalanMap.hpp>



namespace XALAN_CPP_NAMESPACE {



template<class Type>
struct
XalanDestroyFunctor
{
    void
    operator()(Type&  theArg)
    {
        theArg.~Type();
    }

    void
    operator()(Type*  theArg)
    {
        theArg->~Type();
    }

    void
    operator()(const Type*    theArg)
    {
        (*this)(const_cast<Type*>(theArg));
    }

    void
    operator()(
                Type*               theArg,
                MemoryManager&  theMemoryManager)
    {
        if (theArg != 0)
        {
            (*this)(*theArg);

            theMemoryManager.deallocate(theArg);
        }
    }

    void
    operator()(
                const Type*         theArg,
                MemoryManager&  theMemoryManager)
    {
        (*this)(const_cast<Type*>(theArg), theMemoryManager);
    }
};



template<class Type>
XalanDestroyFunctor<Type>
makeXalanDestroyFunctor(const Type*     /* theType */)
{
    return XalanDestroyFunctor<Type>();
}



/**
 * Functor to delete objects, used in STL iteration algorithms.
 */
template <class Type>
struct DeleteFunctor
{
    DeleteFunctor(MemoryManager&    theManager) :
        m_memoryManager(theManager)
    {
    }

    /**
     * Delete the object pointed to by argument.
     *
     * @param thePointer pointer to object to be deleted
     */
    void
    operator()(const Type*    thePointer) const
    {
        return makeXalanDestroyFunctor(thePointer)(thePointer, m_memoryManager);
    }

private:

   MemoryManager&   m_memoryManager;
};


/**
 * Functor to call a clear() member function on its argument.
 */
template <class Type>
struct ClearFunctor
{
    /**
     * Retrieve the value of a key-value pair.
     *
     * @param thePair key-value pair
     * @return value
     */
    void
    operator()(Type&   theArg) const
    {
        return theArg.clear();
    }
};



/**
 * Functor to delete value objects in maps, used in STL iteration algorithms.
 */
template <class T>
struct MapValueDeleteFunctor
{
    MapValueDeleteFunctor(MemoryManager&    theManager) :
        m_memoryManager(theManager)
    {
    }

    /**
     * Delete the value object in a map value pair.  The value of the pair must
     * be of pointer type.
     *
     * @param thePair key-value pair
     */
    void
    operator()(const typename T::value_type&    thePair) const
    {
        return makeXalanDestroyFunctor(thePair.second)(thePair.second, m_memoryManager);
    }

private:

    MemoryManager&  m_memoryManager;
};



template<class MapType>
MapValueDeleteFunctor<MapType>
makeMapValueDeleteFunctor(MapType&   theMap)
{
    return MapValueDeleteFunctor<MapType>(theMap.getMemoryManager());
}



/**
 * This functor is designed to compare 0-terminated arrays.  It substitutes
 * for the default less<type*> so that pointers to arrays can be compared,
 * rather than copies of arrays.  For example, you might want to use C-style
 * strings as keys in a map, rather than string objects.  The default
 * algorithm less<const char*> would just compare the pointers, and not the
 * vector of characters to which it points.  Using this algorithm instead of
 * the default will allow the map to work as expected.
 */
template<class T>
struct less_null_terminated_arrays
{
    /**
     * Compare the values of two objects.
     *
     *
     * @param theLHS first object to compare
     * @param theRHS second object to compare
     * @return true if objects are the same
     */
    bool
    operator()(
            const T*    theLHS,
            const T*    theRHS) const
    {
        while(*theLHS && *theRHS)
        {
            if (*theLHS != *theRHS)
            {
                break;
            }
            else
            {
                theLHS++;
                theRHS++;
            }
        }

        return *theLHS < *theRHS ? true : false;
    }
};



template<class T>
struct equal_null_terminated_arrays
{
    /**
     * Compare the values of two objects.
     *
     *
     * @param theLHS first object to compare
     * @param theRHS second object to compare
     * @return true if objects are the same
     */
    bool
    operator()(
            const T*    theLHS,
            const T*    theRHS) const
    {
        while(*theLHS && *theRHS)
        {
            if (*theLHS != *theRHS)
            {
                return false;
            }
            else
            {
                ++theLHS;
                ++theRHS;
            }
        }

        if (*theLHS || *theRHS)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
};



template <class ScalarType>
inline size_t
XalanScalarHash(
            ScalarType  theValue,
            size_t      theResult)
{
    return (theResult * 37) + (theResult >> 24) + size_type(theValue);
}



template <class T>
struct hash_non_terminated_array
{
    size_t
    operator() (
        const T*    theKey,
        size_t      theLength,
        size_t      theInitialValue = 0) const
    {
        size_t      theHashValue = theInitialValue; 

        const T*    theEnd =
                theKey + theLength;

        while (theKey != theEnd)
        {
            theHashValue += XalanScalarHash(*theKey, theHashValue);

            ++theKey;
        }

        return ++theHashValue;
    }
};



template <class T>
struct hash_null_terminated_array
{
    size_t
    operator() (
        const T*   theKey,
        size_t     theInitialValue = 0) const
    {
        size_t     theHashValue = theInitialValue; 

        while (*theKey)
        {
            theHashValue += XalanScalarHash(*theKey, theHashValue);

            ++theKey;
        }

        return ++theHashValue;
    }
};



template<>
struct XalanMapKeyTraits<const XalanDOMChar*>
{
    typedef hash_null_terminated_array<XalanDOMChar>   Hasher;
    typedef equal_null_terminated_arrays<XalanDOMChar>  Comparator;
};



template<class CollectionType>
class CollectionClearGuard
{
public:

    CollectionClearGuard(CollectionType&    theCollection) :
        m_collection(&theCollection)
    {
    }

    ~CollectionClearGuard()
    {
        if (m_collection != 0)
        {
            m_collection->clear();
        }
    }

    void
    release()
    {
        m_collection = 0;
    }

private:

    // Not implemented...
    CollectionClearGuard(const CollectionClearGuard<CollectionType>&);

    CollectionClearGuard<CollectionType>&
    operator=(const CollectionClearGuard<CollectionType>&);

    // Data members...
    CollectionType*     m_collection;
};



template<class CollectionType, class DeleteFunctorType>
class CollectionDeleteGuard
{
public:

    CollectionDeleteGuard(CollectionType&   theCollection) :
        m_collection(&theCollection)
    {
    }

    ~CollectionDeleteGuard()
    {
        if (m_collection != 0)
        {
            using std::for_each;

            // Delete all of the objects in the temp vector.
            for_each(m_collection->begin(),
                     m_collection->end(),
                     DeleteFunctorType(m_collection->getMemoryManager()));
        }
    }

    void
    release()
    {
        m_collection = 0;
    }

private:

    // Not implemented...
    CollectionDeleteGuard(const CollectionDeleteGuard<CollectionType, DeleteFunctorType>&);

    CollectionDeleteGuard<CollectionType, DeleteFunctorType>&
    operator=(const CollectionDeleteGuard<CollectionType, DeleteFunctorType>&);

    // Data members...
    CollectionType*     m_collection;
};



template<class T>
struct pointer_equals
{
    bool
    operator()(
        const T*    theLHS,
        const T*    theRHS) const
    {
        assert(theLHS != 0 && theRHS != 0);

        return *theLHS == *theRHS;
    }
};



template<class T>
struct pointer_equals_predicate
{
    pointer_equals_predicate(const T*    theArg) :
        m_arg(theArg)
    {
    }

    bool
    operator()(
        const T*   theOther) const
    {
        assert(theOther != 0);

        return *theOther == *m_arg;
    }

private:

    const T*    m_arg;
};



template<class T>
struct pointer_less
{
    bool
    operator()(
        const T*    theLHS,
        const T*    theRHS) const
    {
        assert(theLHS != 0 && theRHS != 0);

        using std::less;

        return less<T>()(*theLHS, *theRHS);
    }
};



template<class T>
struct pointer_equal
{
    bool
    operator()(
        const T*    theLHS,
        const T*    theRHS) const
    {
        assert(theLHS != 0 && theRHS != 0);
        return std::equal_to<T>()(*theLHS, *theRHS);
    }
};




}



#endif  // STLHELPERS_HEADER_GUARD_1357924680
