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
#if !defined(STLHELPERS_HEADER_GUARD_1357924680)
#define STLHELPERS_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <algorithm>
#include <functional>



#include <xalanc/Include/XalanMap.hpp>



XALAN_CPP_NAMESPACE_BEGIN



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
                MemoryManagerType&  theMemoryManager)
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
                MemoryManagerType&  theMemoryManager)
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
#if defined(XALAN_NO_STD_NAMESPACE)
struct DeleteFunctor : public unary_function<const Type*, void>
#else
struct DeleteFunctor : public std::unary_function<const Type*, void>
#endif
{
#if defined(XALAN_NO_STD_NAMESPACE)
    typedef unary_function<const Type*, void>       BaseClassType;
#else
    typedef std::unary_function<const Type*, void>  BaseClassType;
#endif

    typedef typename BaseClassType::result_type     result_type;
    typedef typename BaseClassType::argument_type   argument_type;

    DeleteFunctor(MemoryManagerType&    theManager) :
        m_memoryManager(theManager)
    {
    }

    /**
     * Delete the object pointed to by argument.
     *
     * @param thePointer pointer to object to be deleted
     */
    result_type
    operator()(argument_type    thePointer) const
    {
        makeXalanDestroyFunctor(thePointer)(thePointer, m_memoryManager);
    }

private:

   MemoryManagerType&   m_memoryManager;
};



#if !defined(XALAN_SGI_BASED_STL)

/**
 * Functor to retrieve the key of a key-value pair in a map, used in STL
 * iteration algorithms.
 */
template <class PairType>
#if defined(XALAN_NO_STD_NAMESPACE)
struct select1st : public unary_function<PairType, PairType::first_type>
#else
struct select1st : public std::unary_function<PairType, typename PairType::first_type>
#endif
{
#if defined(XALAN_NO_STD_NAMESPACE)
    typedef unary_function<PairType, PairType::first_type>  BaseClassType;
#else
    typedef std::unary_function<PairType, typename PairType::first_type>    BaseClassType;
#endif

    typedef typename BaseClassType::result_type     result_type;
    typedef typename BaseClassType::argument_type   argument_type;

    typedef PairType                                value_type;

    /**
     * Retrieve the key of a key-value pair.
     *
     * @param thePair key-value pair
     * @return key
     */
    result_type
    operator()(const argument_type&     thePair) const
    {
        return thePair.first;
    }
};



/**
 * Functor to retrieve the value of a key-value pair in a map, used in STL
 * iteration algorithms.
 */
template <class PairType>
#if defined(XALAN_NO_STD_NAMESPACE)
struct select2nd : public unary_function<PairType, PairType::second_type>
#else
struct select2nd : public std::unary_function<PairType, typename PairType::second_type>
#endif
{
#if defined(XALAN_NO_STD_NAMESPACE)
    typedef unary_function<PairType, PairType::second_type> BaseClassType;
#else
    typedef std::unary_function<PairType, typename PairType::second_type>   BaseClassType;
#endif

    typedef typename BaseClassType::result_type     result_type;
    typedef typename BaseClassType::argument_type   argument_type;

    typedef PairType                                value_type;

    /**
     * Retrieve the value of a key-value pair.
     *
     * @param thePair key-value pair
     * @return value
     */
    result_type
    operator()(const argument_type&     thePair) const
    {
        return thePair.second;
    }
};

#endif



/**
 * Functor to call a clear() member function on its argument.
 */
template <class Type>
#if defined(XALAN_NO_STD_NAMESPACE)
struct ClearFunctor : public unary_function<Type, void>
#else
struct ClearFunctor : public std::unary_function<Type, void>
#endif
{
#if defined(XALAN_NO_STD_NAMESPACE)
    typedef unary_function<Type, void>      BaseClassType;
#else
    typedef std::unary_function<Type, void> BaseClassType;
#endif

    typedef typename BaseClassType::result_type     result_type;
    typedef typename BaseClassType::argument_type   argument_type;

    typedef Type                                    value_type;

    /**
     * Retrieve the value of a key-value pair.
     *
     * @param thePair key-value pair
     * @return value
     */
    result_type
    operator()(argument_type&   theArg) const
    {
        theArg.clear();
    }
};



/**
 * Functor to delete value objects in maps, used in STL iteration algorithms.
 */
template <class T>
#if defined(XALAN_NO_STD_NAMESPACE)
struct MapValueDeleteFunctor : public unary_function<const typename T::value_type&, void>
#else
struct MapValueDeleteFunctor : public std::unary_function<const typename T::value_type&, void>
#endif
{
#if defined(XALAN_NO_STD_NAMESPACE)
    typedef unary_function<const typename T::value_type&, void>         BaseClassType;
#else
    typedef std::unary_function<const typename T::value_type&, void>    BaseClassType;
#endif

    typedef typename BaseClassType::result_type     result_type;
    typedef typename BaseClassType::argument_type   argument_type;

    MapValueDeleteFunctor(MemoryManagerType&    theManager) :
        m_memoryManager(theManager)
    {
    }

    /**
     * Delete the value object in a map value pair.  The value of the pair must
     * be of pointer type.
     *
     * @param thePair key-value pair
     */
    result_type
    operator()(argument_type    thePair) const
    {
        makeXalanDestroyFunctor(thePair.second)(thePair.second, m_memoryManager);
    }

private:

    MemoryManagerType&  m_memoryManager;
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
#if defined(XALAN_NO_STD_NAMESPACE)
struct less_null_terminated_arrays : public binary_function<const T*, const T*, bool>
#else
struct less_null_terminated_arrays : public std::binary_function<const T*, const T*, bool>
#endif
{
#if defined(XALAN_NO_STD_NAMESPACE)
    typedef binary_function<const T*, const T*, bool>           BaseClassType;
#else
    typedef std::binary_function<const T*, const T*, bool>      BaseClassType;
#endif

    typedef typename BaseClassType::result_type             result_type;
    typedef typename BaseClassType::first_argument_type     first_argument_type;
    typedef typename BaseClassType::second_argument_type    second_argument_type;

    /**
     * Compare the values of two objects.
     *
     *
     * @param theLHS first object to compare
     * @param theRHS second object to compare
     * @return true if objects are the same
     */
    result_type
    operator()(
            first_argument_type     theLHS,
            second_argument_type    theRHS) const
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
struct equal_null_terminated_arrays : public XALAN_STD_QUALIFIER binary_function<const T*, const T*, bool>
{
    typedef XALAN_STD_QUALIFIER binary_function<const T*, const T*, bool>       BaseClassType;

    typedef typename BaseClassType::result_type             result_type;
    typedef typename BaseClassType::first_argument_type     first_argument_type;
    typedef typename BaseClassType::second_argument_type    second_argument_type;
    /**
     * Compare the values of two objects.
     *
     *
     * @param theLHS first object to compare
     * @param theRHS second object to compare
     * @return true if objects are the same
     */
    result_type
    operator()(
            first_argument_type     theLHS,
            second_argument_type    theRHS) const
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



template <class T>
struct hash_null_terminated_arrays : public XALAN_STD_QUALIFIER unary_function<const T*, size_t>
{
    typedef XALAN_STD_QUALIFIER unary_function<const T*, size_t>        BaseClassType;

    typedef typename BaseClassType::result_type             result_type;
    typedef typename BaseClassType::argument_type       argument_type;

    result_type
    operator() (argument_type   theKey) const
    {
        const T*        theRawBuffer = theKey;

        result_type     theHashValue = 0; 

        while (*theRawBuffer)
        {
            theHashValue = 5 * theHashValue + *theRawBuffer;
            ++theRawBuffer;
        }

        return ++theHashValue;
    }
};



template<>
struct XalanMapKeyTraits<const XalanDOMChar*>
{
    typedef hash_null_terminated_arrays<XalanDOMChar>   Hasher;
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
#if !defined(XALAN_NO_STD_NAMESPACE)
            using std::for_each;
#endif

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
#if defined(XALAN_NO_STD_NAMESPACE)
struct pointer_equals : public binary_function<const T*, const T*, bool>
#else
struct pointer_equals : public std::binary_function<const T*, const T*, bool>
#endif
{
#if defined(XALAN_NO_STD_NAMESPACE)
    typedef binary_function<const T*, const T*, bool>           BaseClassType;
#else
    typedef std::binary_function<const T*, const T*, bool>      BaseClassType;
#endif

    typedef typename BaseClassType::result_type             result_type;
    typedef typename BaseClassType::first_argument_type     first_argument_type;
    typedef typename BaseClassType::second_argument_type    second_argument_type;

    result_type
    operator()(
        first_argument_type     theLHS,
        second_argument_type    theRHS) const
    {
        assert(theLHS != 0 && theRHS != 0);

        return *theLHS == *theRHS;
    }
};



template<class T>
#if defined(XALAN_NO_STD_NAMESPACE)
struct pointer_equals_predicate : public unary_function<const T*, bool>
#else
struct pointer_equals_predicate : public std::unary_function<const T*, bool>
#endif
{
#if defined(XALAN_NO_STD_NAMESPACE)
    typedef unary_function<const T*, bool>          BaseClassType;
#else
    typedef std::unary_function<const T*, bool>     BaseClassType;
#endif

    typedef typename BaseClassType::result_type     result_type;
    typedef typename BaseClassType::argument_type   argument_type;

    pointer_equals_predicate(argument_type  theArg) :
        m_arg(theArg)
    {
    }

    result_type
    operator()(
        argument_type   theOther) const
    {
        assert(theOther != 0);

        return *theOther == *m_arg;
    }

private:

    const argument_type     m_arg;
};



template<class T>
#if defined(XALAN_NO_STD_NAMESPACE)
struct pointer_less : public binary_function<const T*, const T*, bool>
#else
struct pointer_less : public std::binary_function<const T*, const T*, bool>
#endif
{
#if defined(XALAN_NO_STD_NAMESPACE)
    typedef binary_function<const T*, const T*, bool>           BaseClassType;
#else
    typedef std::binary_function<const T*, const T*, bool>      BaseClassType;
#endif

    typedef typename BaseClassType::result_type             result_type;
    typedef typename BaseClassType::first_argument_type     first_argument_type;
    typedef typename BaseClassType::second_argument_type    second_argument_type;

    result_type
    operator()(
        first_argument_type     theLHS,
        second_argument_type    theRHS) const
    {
        assert(theLHS != 0 && theRHS != 0);

#if !defined(XALAN_NO_STD_NAMESPACE)
        using std::less;
#endif

        return less<T>()(*theLHS, *theRHS);
    }
};



template<class T>
struct pointer_equal : public XALAN_STD_QUALIFIER binary_function<const T*, const T*, bool>
{
    typedef XALAN_STD_QUALIFIER binary_function<const T*, const T*, bool> BaseClassType;

    typedef typename BaseClassType::result_type             result_type;
    typedef typename BaseClassType::first_argument_type     first_argument_type;
    typedef typename BaseClassType::second_argument_type    second_argument_type;

    result_type
    operator()(
        first_argument_type     theLHS,
        second_argument_type    theRHS) const
    {
        assert(theLHS != 0 && theRHS != 0);
        return XALAN_STD_QUALIFIER equal_to<T>()(*theLHS, *theRHS);
    }
};




XALAN_CPP_NAMESPACE_END



#endif  // STLHELPERS_HEADER_GUARD_1357924680
