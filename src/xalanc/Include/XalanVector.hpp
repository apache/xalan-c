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

/**
 * @author David Bertoni (david_n_bertoni@us.ibm.com)
 * @author Matthew Hoyt (mhoyt@ca.ibm.com)
 */

#if !defined(XALANVECTOR_HEADER_GUARD_1357924680)
#define XALANVECTOR_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>


#include <xalanc/Include/XalanMemoryManagement.hpp>
#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>


#include <cstddef>
#include <algorithm>
#include <cassert>
#include <new>
#include <iterator>
#include <stdexcept>





XALAN_CPP_NAMESPACE_BEGIN


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4100)
#endif



XALAN_USING_XERCES(MemoryManager)



template <class Type, class ConstructionTraits = MemoryManagedConstructionTraits<Type> >
class XalanVector
{
public:


    typedef Type                value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;

#if defined(XALAN_VCPP_USE_PTRIT)
    typedef std::_Ptrit<
                Type,
                ptrdiff_t,
                pointer,
                reference,
                pointer,
                reference>          iterator;

    typedef std::_Ptrit<
                Type,
                ptrdiff_t,
                const_pointer,
                const_reference,
                pointer,
                reference>          const_iterator;
#else
    typedef value_type*             iterator;
    typedef const value_type*       const_iterator;
#endif

#if defined(XALAN_HAS_STD_ITERATORS)
    typedef XALAN_STD_QUALIFIER reverse_iterator<iterator>          reverse_iterator_;
    typedef XALAN_STD_QUALIFIER reverse_iterator<const_iterator>    const_reverse_iterator_;
#elif defined(XALAN_RW_NO_CLASS_PARTIAL_SPEC)
    typedef XALAN_STD_QUALIFIER reverse_iterator<
        iterator,
        XALAN_STD_QUALIFIER random_access_iterator_tag,
        value_type> reverse_iterator_;
    typedef XALAN_STD_QUALIFIER reverse_iterator<
        const_iterator,
        XALAN_STD_QUALIFIER random_access_iterator_tag,
        const value_type> const_reverse_iterator_;
#else
    typedef XALAN_STD_QUALIFIER reverse_iterator<iterator, value_type>                          reverse_iterator_;
    typedef XALAN_STD_QUALIFIER reverse_iterator<const_iterator, value_type, const_reference>   const_reverse_iterator_;
#endif

    typedef reverse_iterator_           reverse_iterator;
    typedef const_reverse_iterator_     const_reverse_iterator;

    typedef XalanVector<value_type, ConstructionTraits>     ThisType;

    typedef typename ConstructionTraits::Constructor    Constructor;
    typedef typename Constructor::ConstructableType     ConstructibleType;

    XalanVector(
            MemoryManager&  theManager XALAN_DEFAULT_CONSTRUCTOR_MEMORY_MGR,
            size_type           initialAllocation = size_type(0)) :
        m_memoryManager(&theManager),
        m_size(0),
        m_allocation(initialAllocation),
        m_data(initialAllocation > 0 ? allocate(initialAllocation) : 0)
    {
        invariants();
    }

    static XalanVector*
    create(
        MemoryManager&  theManager,
        size_type       initialAllocation = size_type(0))
    {
        typedef XalanVector ThisType;

        XalanMemMgrAutoPtr<ThisType, false> theGuard( theManager , (ThisType*)theManager.allocate(sizeof(ThisType)));

        ThisType* theResult = theGuard.get();

        new (theResult) ThisType(theManager, initialAllocation);

        theGuard.release();

        return theResult;
    }

    XalanVector(
            const ThisType&     theSource,
            MemoryManager&      theManager XALAN_DEFAULT_CONSTRUCTOR_MEMORY_MGR,
            size_type           theInitialAllocation = size_type(0)) :
        m_memoryManager(&theManager),
        m_size(0),
        m_allocation(0),
        m_data(0)
    {
        if (theSource.m_size > 0)
        {
            ThisType    theTemp(theManager, local_max(theSource.m_size, theInitialAllocation));

            theTemp.insert(theTemp.begin(), theSource.begin(), theSource.end());

            swap(theTemp);
    
        }
        else if (theInitialAllocation > 0)
        {
            m_data = allocate(theInitialAllocation);

            m_allocation = theInitialAllocation;
        }

        invariants();
    }

    XalanVector(
            const_iterator  theFirst, 
            const_iterator  theLast,
            MemoryManager&  theManager) :
        m_memoryManager(&theManager),
        m_size(0),
        m_allocation(0),
        m_data(0)

    {
        ThisType    theTemp(theManager);

        theTemp.insert(theTemp.begin(), theFirst, theLast);

        swap(theTemp);

        invariants();
    }

    static XalanVector*
    create(           
            const_iterator  theFirst, 
            const_iterator  theLast,
            MemoryManager&  theManager)
    {
        typedef XalanVector ThisType;

        XalanMemMgrAutoPtr<ThisType, false> theGuard( theManager , (ThisType*)theManager.allocate(sizeof(ThisType)));

        ThisType* theResult = theGuard.get();

        new (theResult) ThisType(theFirst, theLast, theManager);

        theGuard.release();

        return theResult;
    }

    XalanVector(
            size_type           theInsertSize,
            const value_type&   theData,
            MemoryManager&      theManager) :
        m_memoryManager(&theManager),
        m_size(0),
        m_allocation(0),
        m_data(0)
    {
        ThisType    theTemp(theManager);

        theTemp.insert(theTemp.begin(), theInsertSize, theData);

        swap(theTemp);

        invariants();
    }

    ~XalanVector()
    {
        invariants();

        if (m_allocation != 0)
        {
            destroy(begin(), end());

            deallocate(m_data);
        }
    }

    void
    push_back(const value_type&     data)
    {
        invariants();

        doPushBack(data);

        invariants();
    }

    void
    pop_back()
    {
        invariants();

        --m_size;

        destroy(m_data[m_size]);

        invariants();
    }

    iterator
    erase(
            iterator    theFirst,
            iterator    theLast)
    {
        invariants();

        if (theFirst != theLast)
        {
            XALAN_STD_QUALIFIER copy(
                theLast, 
                end(),
                theFirst);

            shrinkCount(local_distance(theFirst, theLast));
        }

        invariants();

        return theFirst;
    }

    iterator
    erase(iterator  position)
    {
        return erase(position, position + 1);
    }

    void
    insert(
            iterator        thePosition,
            const_iterator  theFirst,
            const_iterator  theLast)
    {
        // Since we're using bare pointers for now, we can
        // assert this...
        assert(theFirst <= theLast);
        assert(thePosition >= begin());
        assert(thePosition <= end());

        invariants();

        const size_type     theInsertSize =
            local_distance(theFirst, theLast);

        if (theInsertSize == 0)
        {
            return;
        }

        const size_type     theTotalSize = size() + theInsertSize;

        if (thePosition == end())
        {
            pointer     thePointer = ensureCapacity(theTotalSize);

            while (theFirst != theLast)
            {
                Constructor::construct(thePointer, *theFirst, *m_memoryManager);

                ++thePointer;
                ++m_size;
                ++theFirst;
            }
        }
        else
        {
            if (theTotalSize > capacity())
            {
                assert (m_memoryManager != 0);

                ThisType    theTemp(*m_memoryManager, theTotalSize);

                // insert everything up to the position...
                theTemp.insert(theTemp.end(), begin(), thePosition);

                // insert the new stuff...
                theTemp.insert(theTemp.end(), theFirst, theLast);

                // insert everything from the position to the end...
                theTemp.insert(theTemp.end(), thePosition, end());

                swap(theTemp);
            }
            else
            {
                // insert into the middle of the vector that has enough capacity
                const iterator      theOriginalEnd = end();

                const size_type     theRightSplitSize =
                    local_distance(thePosition, theOriginalEnd);

                if (theRightSplitSize <= theInsertSize)
                {
                    // inserted range will go to or beyond edge of current vector
                    
                    // append from inserted range, all values that will extend 
                    // beyond the current vector
                    const const_iterator    toInsertSplit = theFirst + theRightSplitSize;
                    const_iterator          toInsertIter = toInsertSplit;

                    while (toInsertIter != theLast)
                    {
                        doPushBack(*toInsertIter);

                        ++toInsertIter;
                    }

                    // copy the "right" of the current vector to the end
                    toInsertIter = thePosition;
                    while (toInsertIter !=  theOriginalEnd)
                    {
                        doPushBack(*toInsertIter);

                        ++toInsertIter;
                    }

                    // copy the remaining part of inserted range into 
                    // the original vector spaces
                    XALAN_STD_QUALIFIER copy(theFirst, toInsertSplit, thePosition);
                }
                else
                {
                    // inserted range will not extend beyond edge of current vector
                    
                    // move end of current vector by insertion size
                    const_iterator  toMoveIter = end() - theInsertSize;

                    while (toMoveIter != theOriginalEnd)
                    {
                        doPushBack(*toMoveIter);

                        ++toMoveIter;
                    }

                    // reverse copy the remaining part of the "right" piece of the current vector
                    XALAN_STD_QUALIFIER copy_backward(thePosition, theOriginalEnd - theInsertSize, theOriginalEnd);

                    // insert into current vector
                    XALAN_STD_QUALIFIER copy(theFirst, theLast, thePosition);
                }
            }
        }

        invariants();
    }

    void
    insert(
            iterator            thePosition,
            size_type           theCount,
            const value_type&   theData)
    {
        invariants();

        const size_type     theTotalSize = size() + theCount;

        // Needs to be optimized
        if (thePosition == end())
        {
            pointer     thePointer = ensureCapacity(theTotalSize);

            for (size_type index = 0; index < theCount; ++index)
            {
                Constructor::construct(thePointer, theData, *m_memoryManager);

                ++thePointer;
                ++m_size;
            }
        }
        else
        {
            if (theTotalSize > capacity())
            {
                assert ( m_memoryManager != 0 );

                ThisType    theTemp(*m_memoryManager, theTotalSize);

                // insert everything up to the position...
                theTemp.insert(theTemp.end(), begin(), thePosition);

                // insert the new stuff...
                theTemp.insert(theTemp.end(), theCount, theData);
                
                // insert everything from the position to the end...
                theTemp.insert(theTemp.end(), thePosition, end());

                swap(theTemp);
            }
            else
            {
                // insert into the middle of the vector that has enough capacity            
                const iterator      theOriginalEnd = end();

                const size_type     theRightSplitSize =
                                            local_distance(thePosition, theOriginalEnd);

                if (theRightSplitSize <= theCount)
                {
                    // inserted range will go to or beyond edge of current vector
                    
                    // append all copies that will extend 
                    // beyond the current vector
                    for (size_type i = 0;  i < (theCount - theRightSplitSize); ++i)
                    {
                        doPushBack(theData);
                    }

                    // copy the "right" of the current vector to the end
                    iterator    toInsertIter = thePosition;
 
                    while (toInsertIter !=  theOriginalEnd)
                    {
                        doPushBack(*toInsertIter);

                        ++toInsertIter;
                    }

                    // copy the remaining part of inserted range into 
                    // the original vector spaces
                    XALAN_STD_QUALIFIER fill(thePosition, thePosition + theRightSplitSize, theData);
                }
                else
                {
                    // inserted range will not extend beyond edge of current vector
                    
                    // move end of current vector by insertion size
                    const_iterator  toMoveIter = end() - theCount;

                    while (toMoveIter != theOriginalEnd)
                    {
                        doPushBack(*toMoveIter);

                        ++toMoveIter;
                    }

                    // reverse copy the remaining part of the "right" piece of the current vector
                    XALAN_STD_QUALIFIER copy_backward(thePosition, theOriginalEnd - theCount, theOriginalEnd);

                    // insert into current vector
                    XALAN_STD_QUALIFIER fill(thePosition, thePosition + theCount, theData);
                }
            }
        }

        invariants();
    }

    iterator
    insert(
            iterator            thePosition,
            const value_type&   theData)
    {
        if (m_allocation > m_size)
        {
            insert(thePosition, 1, theData);

            return thePosition;
        }
        else
        {
            const size_type     theDistance =
                local_distance(begin(), thePosition);

            insert(thePosition, 1, theData);

            return begin() + theDistance;
        }
    }

    void
    assign(
            const_iterator  theFirst,
            const_iterator  theLast)
    {
        clear();

        insert(
            begin(),
            theFirst,
            theLast);
    }

    void
    assign(
            iterator    theFirst,
            iterator    theLast)
    {
        assign(
            const_iterator(theFirst),
            const_iterator(theLast));
    }

    void
    assign(
            size_type           theCount,
            const value_type&   theData)
    {
        clear();

        insert(theCount, theData);
    }

    size_type
    size() const
    {
        invariants();

        return m_size;
    }

    size_type
    max_size() const
    {
        invariants();

        return ~size_type(0);
    }

    void
    resize(size_type   theSize)
    {
        const ConstructibleType     defaultValue(*m_memoryManager);

        resize(theSize, defaultValue.value);
    }

    void
    resize( size_type           theSize,
            const value_type&   theValue)
    {
        invariants();

        if (m_size > theSize)
        {
            shrinkToSize(theSize);
        }
        else if (m_size < theSize)
        {
            // Reserve memory up-front...
            reserve(theSize);

            assert(m_allocation >= theSize);

            const value_type* const     theEnd = m_data + theSize;

            // Fill the new area...
            for (value_type* data = endPointer();
                    data != theEnd;
                        ++data, ++m_size)
            {
                Constructor::construct(data, theValue, *m_memoryManager);
            }
        }

        assert(m_size == theSize);

        invariants();
    }

    size_type
    capacity() const
    {
        invariants();

        return m_allocation;
    }

    bool
    empty() const
    {
        invariants();

        return m_size == 0 ? true : false;
    }

    void
    reserve(size_type   theSize)
    {
        invariants();

        if (theSize > m_allocation)
        {
            doReserve(theSize);
        }

        invariants();
    }

    reference
    front()
    {
        invariants();

        return m_data[0];
    }

    const_reference
    front() const
    {
        invariants();

        return m_data[0];
    }

    reference
    back()
    {
        return m_data[m_size - 1];
    }

    const_reference
    back() const
    {
        return m_data[m_size - 1];
    }

    iterator
    begin()
    {
        invariants();

        return m_data;
    }

    const_iterator
    begin() const
    {
        invariants();

        return m_data;
    }

    iterator
    end()
    {
        invariants();

        return endPointer();
    }

    const_iterator
    end() const
    {
        invariants();

        return endPointer();
    }

    reverse_iterator
    rbegin()
    {
        invariants();

        return reverse_iterator(end());
    }

    const_reverse_iterator
    rbegin() const
    {
        invariants();

        return const_reverse_iterator(end());
    }

    reverse_iterator
    rend()
    {
        invariants();

        return reverse_iterator(begin());
    }

    const_reverse_iterator
    rend() const
    {
        invariants();

        return const_reverse_iterator(begin());
    }
    

    reference
    at(size_type    theIndex)
    {
        if (theIndex >= m_size)
        {
            outOfRange();
        }

        return m_data[theIndex];
    }

    const_reference
    at(size_type    theIndex) const
    {
        if (theIndex >= m_size)
        {
            outOfRange();
        }

        return m_data[theIndex];
    }

    reference
    operator[](size_type    theIndex)
    {
        assert (theIndex < m_size);

        return m_data[theIndex];
    }

    const_reference
    operator[](size_type    theIndex) const
    {
        assert (theIndex < m_size);

        return m_data[theIndex];
    }

    void
    clear()
    {
        invariants();

        if (m_size > 0)
        {
            shrinkToSize(0);
        }

        invariants();
    }

    // Operators...
    ThisType&
    operator=(const ThisType&  theRHS)
    {
        invariants();

        if (&theRHS != this)
        {
            if (m_allocation < theRHS.m_size)
            {
                ThisType    theTemp(theRHS,*m_memoryManager);

                swap(theTemp);
            }
            else
            {
                const_iterator  theRHSCopyEnd = theRHS.end();

                if (m_size > theRHS.m_size)
                {
                    // Resize to the target size...
                    shrinkToSize(theRHS.m_size);
                }
                else if (m_size < theRHS.m_size)
                {
                    theRHSCopyEnd =
                        theRHS.begin() + m_size;

                    insert(
                        end(),
                        theRHSCopyEnd,
                        theRHS.end());
                }

                // Copy everything that already exists...
                XALAN_STD_QUALIFIER copy(
                    theRHS.begin(),
                    theRHSCopyEnd,
                    begin());
            }
        }

        invariants();

        return *this;
    }

    void
    swap(ThisType&  theOther)
    {
        invariants();

        MemoryManager* const    theTempManager = m_memoryManager;
        const size_type         theTempLength = m_size;
        const size_type         theTempAllocation = m_allocation;
        value_type* const       theTempData = m_data;

        m_memoryManager = theOther.m_memoryManager;
        m_size = theOther.m_size;
        m_allocation = theOther.m_allocation;
        m_data = theOther.m_data;

        theOther.m_memoryManager = theTempManager;
        theOther.m_size = theTempLength;
        theOther.m_allocation = theTempAllocation;
        theOther.m_data = theTempData;

        invariants();
    }

    const MemoryManager*
    getMemoryManager() const
    {
        return m_memoryManager;
    }

    MemoryManager&
    getMemoryManager()
    {
        assert (m_memoryManager != 0);

        return *m_memoryManager;
    }

    // Detaches the allocated memory from the vector, and returns
    // the pointer to the caller.  The caller then owns the memory
    // and must destroy any objects and deallocate it using the
    // the memory manager returned from getMemoryManager()
    pointer
    detach()
    {
        m_size = 0;
        m_allocation = 0;

        value_type* const   theTemp = m_data;

        m_data = 0;

        return theTemp;
    }

private:

#if defined(NDEBUG)
    void
    invariants() const
    {
    }
#else
    void
    invariants() const
    {
        assert(m_allocation >= m_size);
        assert(m_data == 0 && m_allocation == 0 || m_data != 0 && m_allocation != 0);
    }
#endif

    size_type
    local_distance(
            const_iterator  theFirst,
            const_iterator  theLast)
    {
        // Since we're using bare pointers for now, we can
        // assert this...
        assert(theFirst <= theLast);

#if defined(XALAN_HAS_STD_DISTANCE)
        return XALAN_STD_QUALIFIER distance(theFirst, theLast);
#else
        size_type   theDistance;

        XALAN_STD_QUALIFIER distance(theFirst, theLast, theDistance);

        return theDistance;
#endif
    }

    value_type*
    allocate(size_type  size)
    {
        const size_type     theBytesNeeded = size * sizeof(value_type);

        assert (m_memoryManager != 0);

        void*   pointer = m_memoryManager->allocate(theBytesNeeded);

        assert(pointer != 0);

        return (value_type*) pointer;
    }

    void
    deallocate(value_type*  pointer)
    {
        assert(m_memoryManager != 0);

        m_memoryManager->deallocate(pointer);

    }

    static void
    destroy(value_type&     theValue)
    {
        theValue.~Type();
    }

    static void
    destroy(
            iterator    theFirst,
            iterator    theLast)
    {
        for(; theFirst != theLast; ++theFirst)
        {
            destroy(*theFirst);
        }
    }

    void
    doPushBack(const value_type&   data)
    {
        invariants();

        if (m_size < m_allocation)
        {
            Constructor::construct(endPointer(), data, *m_memoryManager);

            ++m_size;
        }
        else
        {
            assert(m_size == m_allocation);

            const size_type     theNewSize = m_size == 0 ? 1 : size_type((m_size * 1.6) + 0.5);
            assert(theNewSize > m_size);

            ThisType    theTemp(*this, *m_memoryManager, theNewSize);

            theTemp.doPushBack(data);

            swap(theTemp);
        }

        invariants();
    }

    pointer
    ensureCapacity(size_type    theSize)
    {
        if (theSize > capacity())
        {
            doReserve(theSize);
        }

        return endPointer();
    }

    void
    doReserve(size_type     theSize)
    {
        invariants();

        assert(theSize > m_allocation);

        ThisType    theTemp(*this, *m_memoryManager, theSize);

        swap(theTemp);

        invariants();
    }

    pointer
    endPointer()
    {
        return m_data + m_size;
    }

    const_pointer
    endPointer() const
    {
        return m_data + m_size;
    }

    static void
    outOfRange()
    {
        throw XALAN_STD_QUALIFIER out_of_range("");
    }

    void
    shrinkToSize(size_type    theSize)
    {
        assert(m_size > theSize);

        do
        {
            pop_back();
        } while (m_size > theSize);
    }

    void
    shrinkCount(size_type   theCount)
    {
        assert(m_size >= theCount);

        while (theCount > 0)
        {
            pop_back();

            --theCount;
        }
    }

    size_type
    local_max(
            size_type   theLHS,
            size_type   theRHS)
    {
        return theLHS > theRHS ? theLHS : theRHS;
    }

#if defined(XALAN_DEVELOPMENT)
    //not implemented
    XalanVector(const     XalanVector&);
    XalanVector();
#endif

    // Data members...
    MemoryManager*  m_memoryManager;

    size_type           m_size;

    size_type           m_allocation;

    value_type*         m_data;
};



template <class Type>
inline void
swap(
            XalanVector<Type>&  theLHS,
            XalanVector<Type>&  theRHS)
{
    theLHS.swap(theRHS);
}



template <class Type>
inline bool
operator==(
            const XalanVector<Type>&    theLHS,
            const XalanVector<Type>&    theRHS)
{
    if (theLHS.size() != theRHS.size())
    {
        return false;
    }
    else if (theLHS.size() == 0)
    {
        return true;
    }
    else
    {
        return XALAN_STD_QUALIFIER equal(theLHS.begin(), theLHS.end(), theRHS.begin());
    }
}



template <class Type>
inline bool
operator!=(
            const XalanVector<Type>&    theLHS,
            const XalanVector<Type>&    theRHS)
{
    return !(theLHS == theRHS);
}



template <class Type>
inline bool
operator<(
            const XalanVector<Type>&    theLHS,
            const XalanVector<Type>&    theRHS)
{
    return XALAN_STD_QUALIFIER lexicographical_compare(
                theLHS.begin(),
                theLHS.end(),
                theRHS.begin(),
                theRHS.end());
}



template <class Type>
inline bool
operator<=(
            const XalanVector<Type>&    theLHS,
            const XalanVector<Type>&    theRHS)
{
    return !(theRHS < theLHS);
}



template <class Type>
inline bool
operator>(
            const XalanVector<Type>&    theLHS,
            const XalanVector<Type>&    theRHS)
{
    return theRHS < theLHS;
}



template <class Type>
inline bool
operator>=(
            const XalanVector<Type>&    theLHS,
            const XalanVector<Type>&    theRHS)
{
    return !(theLHS < theRHS);
}



#if defined(_MSC_VER)
#pragma warning(pop)
#endif



XALAN_CPP_NAMESPACE_END



#endif  // XALANVECTOR_HEADER_GUARD_1357924680
