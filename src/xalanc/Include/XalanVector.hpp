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


#include <cstddef>
#include <algorithm>
#include <cassert>
#include <new>
#include <iterator>
#include <stdexcept>



#include <xercesc/framework/MemoryManager.hpp>


XALAN_CPP_NAMESPACE_BEGIN


#pragma warning(push)
#pragma warning(disable: 4100)

template <class Type>
class XalanVector
{
public:

    typedef XERCES_CPP_NAMESPACE_QUALIFIER MemoryManager    MemoryManagerType;

    typedef Type                value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;
    typedef ptrdiff_t           difference_type;
    
#if defined(_MSC_VER) && (_MSC_VER < 1300)
    typedef XALAN_STD_QUALIFIER reverse_iterator<iterator,value_type> reverse_iterator;
    typedef XALAN_STD_QUALIFIER reverse_iterator<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
    typedef XALAN_STD_QUALIFIER reverse_iterator<iterator> reverse_iterator;
    typedef XALAN_STD_QUALIFIER reverse_iterator<const_iterator> const_reverse_iterator;
#endif

    typedef XalanVector<value_type>     ThisType;

    XalanVector(
            MemoryManagerType*  theManager = 0,
            size_type           initialAllocation = size_type(0)) :
        m_memoryManager(theManager),
        m_size(0),
        m_allocation(initialAllocation),
        m_data(initialAllocation > 0 ? allocate(initialAllocation) : 0)
    {
        invariants();
    }

    XalanVector(
            const ThisType&     theSource,
            MemoryManagerType*  theManager = 0,
            size_type           initialAllocation = size_type(0)) :
        m_memoryManager(theManager != 0 ? theManager : theSource.m_memoryManager),
        m_size(0),
        m_allocation(0),
        m_data(0)
    {
        if (theSource.m_size > 0)
        {
            if (theSource.m_size > initialAllocation)
            {
                initialAllocation = theSource.m_size;
            }

            ThisType    theTemp(theManager, initialAllocation);

            theTemp.insert(theTemp.begin(), theSource.begin(), theSource.end());

            swap(theTemp);
    
        }
        else if (initialAllocation > 0)
        {
            m_data = allocate(initialAllocation);

            m_allocation = initialAllocation;
        }

        invariants();
    }

    XalanVector(
            const_iterator theStart, 
            const_iterator theEnd,
            MemoryManagerType*  theManager = 0) :
        m_memoryManager(theManager),
        m_size(0),
        m_allocation(0),
        m_data(0)

    {
        ThisType    theTemp(theManager);

        theTemp.insert(theTemp.begin(), theStart, theEnd);

        swap(theTemp);

        invariants();
    }

    XalanVector(
            size_type theInsertSize,
            value_type data,
            MemoryManagerType*  theManager = 0) :
        m_memoryManager(theManager),
        m_size(0),
        m_allocation(theInsertSize),
        m_data(0)
    {
        ThisType    theTemp(theManager);

        theTemp.insert(theTemp.begin(), theInsertSize, data);

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
    push_back(value_type    data)
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
    erase(iterator  position)
    {
        return erase(position, position + 1);
    }

    iterator
    erase(
            iterator    start,
            iterator    end)
    {
        invariants();

        if (start != end)
        {
            XALAN_STD_QUALIFIER copy(
                end, 
                endPointer(),
                start);

            shrink(size() - (end - start));
        }

        invariants();

        return start;
    }

    void
    insert(
            iterator        thePosition,
            const_iterator  theStart,
            const_iterator  theEnd)
    {
        invariants();

        const size_type     theInsertSize = theEnd - theStart;

        if (theInsertSize == 0)
        {
            return;
        }

        const size_type     theTotalSize = size() + theInsertSize;

        if (thePosition == end())
        {
            pointer     thePointer = ensureCapacity(theTotalSize);

            while (theStart != theEnd)
            {
                construct(thePointer, *theStart);

                ++thePointer;
                ++m_size;
                ++theStart;
            }
        }
        else
        {
            if (theTotalSize > capacity())
            {
                ThisType    theTemp(m_memoryManager, theTotalSize);

                // insert everything up to the position...
                theTemp.insert(theTemp.end(), begin(), thePosition);

                // insert the new stuff...
                theTemp.insert(theTemp.end(), theStart, theEnd);

                // insert everything from the position to the end...
                theTemp.insert(theTemp.end(), thePosition, end());

                swap(theTemp);
            }
            else
            {
                // insert into the middle of the vector that has enough capacity
                iterator theOriginalEnd = end();
                size_type theRightSplitSize = end() - thePosition;
                
                if (theRightSplitSize <= theInsertSize)
                {
                    // inserted range will go to or beyond edge of current vector
                    
                    // append from inserted range, all values that will extend 
                    // beyond the current vector
                    const_iterator toInsertSplit = theStart + theRightSplitSize;
                    const_iterator toInsertIter = toInsertSplit;
                    while (toInsertIter != theEnd)
                    {
                        construct(endPointer(),*toInsertIter);
                        ++m_size;
                        ++toInsertIter;
                    }

                    // copy the "right" of the current vector to the end
                    toInsertIter = thePosition;
                    while (toInsertIter !=  theOriginalEnd)
                    {
                        construct(endPointer(),*toInsertIter);
                        ++m_size;
                        ++toInsertIter;
                    }

                    // copy the remaining part of inserted range into 
                    // the original vector spaces
                    XALAN_STD_QUALIFIER copy(theStart, toInsertSplit, thePosition);
                }
                else
                {
                    // inserted range will not extend beyond edge of current vector
                    
                    // move end of current vector by insertion size
                    const_iterator toMoveIter = end() - theInsertSize;

                    while (toMoveIter != theOriginalEnd)
                    {
                        construct(endPointer(),*toMoveIter);
                        ++m_size;
                        ++toMoveIter;
                    }

                    // reverse copy the remaining part of the "right" piece of the current vector
                    XALAN_STD_QUALIFIER copy_backward(thePosition, theOriginalEnd - theInsertSize, theOriginalEnd);

                    // insert into current vector
                    XALAN_STD_QUALIFIER copy(theStart, theEnd, thePosition);
                }
            }
        }

        invariants();
    }

    void
    insert(
            iterator    thePosition,
            size_type   theInsertSize,
            value_type  data)
    {
        invariants();

        const size_type     theTotalSize = size() + theInsertSize;

        // Needs to be optimized
        if (thePosition == end())
        {
            pointer     thePointer = ensureCapacity(theTotalSize);

            size_type index = 0; 
            while (index < theInsertSize)
            {
                construct(thePointer, data);

                ++thePointer;
                ++m_size;
                ++index;
            }
        }
        else
        {
            if (theTotalSize > capacity())
            {
                ThisType    theTemp(m_memoryManager, theTotalSize);

                // insert everything up to the position...
                theTemp.insert(theTemp.end(), begin(), thePosition);

                // insert the new stuff...
                theTemp.insert(theTemp.end(), theInsertSize, data);
                
                // insert everything from the position to the end...
                theTemp.insert(theTemp.end(), thePosition, end());

                swap(theTemp);
            }
            else
            {
                // insert into the middle of the vector that has enough capacity            
                iterator theOriginalEnd = end();
                size_type theRightSplitSize = end() - thePosition;
                
                if (theRightSplitSize <= theInsertSize)
                {
                    // inserted range will go to or beyond edge of current vector
                    
                    // append all copies that will extend 
                    // beyond the current vector                
                    size_type i = 0;
                    for (;  i < (theInsertSize - theRightSplitSize); ++i)
                    {
                        construct(endPointer(),data);
                        ++m_size;
                    }

                    // copy the "right" of the current vector to the end
                    iterator toInsertIter = thePosition;
                    while (toInsertIter !=  theOriginalEnd)
                    {
                        construct(endPointer(),*toInsertIter);
                        ++m_size;
                        ++toInsertIter;
                    }

                    // copy the remaining part of inserted range into 
                    // the original vector spaces
                    XALAN_STD_QUALIFIER fill(thePosition, thePosition + theRightSplitSize, data);
                }
                else
                {
                    // inserted range will not extend beyond edge of current vector
                    
                    // move end of current vector by insertion size
                    const_iterator toMoveIter = end() - theInsertSize;

                    while (toMoveIter != theOriginalEnd)
                    {
                        construct(endPointer(),*toMoveIter);
                        ++m_size;
                        ++toMoveIter;
                    }

                    // reverse copy the remaining part of the "right" piece of the current vector
                    XALAN_STD_QUALIFIER copy_backward(thePosition, theOriginalEnd - theInsertSize, theOriginalEnd);

                    // insert into current vector
                    XALAN_STD_QUALIFIER fill(thePosition, thePosition + theInsertSize, data);
                
                }
            }
        }

        invariants();
    }

    void
    insert(
            iterator thePosition,
            value_type data)
    {
        insert(thePosition,1,data);
    }
            
    void
    assign(const_iterator theStart, const_iterator theEnd)
    {
        clear();

        insert(0,theStart,theEnd);
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
    resize(
            size_type   theSize,
            value_type  theValue = value_type())
    {
        invariants();

        if (m_size > theSize)
        {
            shrink(theSize);
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
                construct(data, theValue);
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
            range_error();
        }

        return m_data[theIndex];
    }

    const_reference
    at(size_type    theIndex) const
    {
        if (theIndex >= m_size)
        {
            rangeError();
        }

        return m_data[theIndex];
    }

    reference
    operator[](size_type    theIndex)
    {
        return m_data[theIndex];
    }

    const_reference
    operator[](size_type    theIndex) const
    {
        return m_data[theIndex];
    }

    void
    clear()
    {
        invariants();

        m_size = 0;

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
                XalanVector<Type>   theTemp(theRHS);

                swap(theTemp);
            }
            else
            {
                const_iterator  theRHSEnd = theRHS.end();

                if (m_size > theRHS.m_size)
                {
                    // Resize to the target size...
                    shrink(theRHS.m_size);
                }
                else if (m_size < theRHS.m_size)
                {
                    insert(
                        end(),
                        theRHS.begin() + m_size,
                        theRHS.end());
                }

                // Copy everything that already exists...
                XALAN_STD_QUALIFIER copy(
                    theRHS.begin(),
                    theRHSEnd,
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

        MemoryManagerType* const    theTempManager = m_memoryManager;
        const size_type             theTempLength = m_size;
        const size_type             theTempAllocation = m_allocation;
        value_type* const           theTempData = m_data;

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

    const MemoryManagerType*
    getMemoryManager() const
    {
        return m_memoryManager;
    }

    MemoryManagerType*
    getMemoryManager()
    {
        return m_memoryManager;
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

    value_type*
    allocate(size_type  size)
    {
        const size_type     theBytesNeeded = size * sizeof(Type);

        void*   pointer = m_memoryManager == 0 ?
            ::operator new (theBytesNeeded) :
            m_memoryManager->allocate(theBytesNeeded);

        assert(pointer != 0);

        return (value_type*) pointer;
    }

    void
    deallocate(value_type*  pointer)
    {
        if (m_memoryManager == 0)
        {
            ::operator delete(pointer);
        }
        else
        {
            m_memoryManager->deallocate(pointer);
        }
    }

    static void
    construct(
            value_type*         theAddress,
            const value_type&   theValue)
    {
        new (theAddress) Type(theValue);
    }

    static void
    construct(
            value_type*         start,
            value_type*         end,
            const value_type*   values)
    {
        for(; start != end; ++start, ++values)
        {
            construct(*values);
        }
    }

    static void
    destroy(value_type&     theValue)
    {
        theValue.~Type();
    }

    static void
    destroy(
            iterator    start,
            iterator    end)
    {
        for(; start != end; ++start)
        {
            destroy(*start);
        }
    }

    void
    doPushBack(value_type   data)
    {
        invariants();

        if (m_size < m_allocation)
        {
            construct(endPointer(), data);

            ++m_size;
        }
        else
        {
            assert(m_size == m_allocation);

            const size_type     theNewSize = m_size == 0 ? 1 : size_type((m_size * 1.6) + 0.5);
            assert(theNewSize > m_size);

            ThisType    theTemp(*this, m_memoryManager, theNewSize);

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

        ThisType    theTemp(*this, m_memoryManager, theSize);

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

    void
    rangeError()
    {
        throw XALAN_STD_QUALIFIER out_of_range("");
    }

    void
    shrink(size_type    theSize)
    {
        assert(m_size > theSize);

        do
        {
            pop_back();
        } while (m_size > theSize);
    }

    // Data members...
    MemoryManagerType*  m_memoryManager;

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
    return theLHS.swap(theRHS);
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
    return !(y < x);
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
    return !(x < y);
}



#pragma warning(pop)


XALAN_CPP_NAMESPACE_END



#endif  // XALANVECTOR_HEADER_GUARD_1357924680
