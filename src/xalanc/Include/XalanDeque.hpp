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

#if !defined(XALANDEQUE_HEADER_GUARD_1357924680)
#define XALANDEQUE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanMemoryManagement.hpp> 



XALAN_CPP_NAMESPACE_BEGIN



template <class Value>
struct XalanDequeIteratorTraits
{
    typedef Value           value_type;
    typedef Value&          reference;
    typedef Value*          pointer;
    typedef const Value&    const_reference;
};

template <class Value>
struct XalanDequeConstIteratorTraits
{
    typedef Value           value_type;
    typedef const Value&    reference;
    typedef const Value*    pointer;
    typedef const Value&    const_reference;
};

template <class Traits, class XalanDeque>
class XalanDequeIterator
{
public:

    typedef size_t                              size_type;
    typedef typename Traits::value_type         value_type;
    typedef typename Traits::reference          reference;
    typedef typename Traits::pointer            pointer;
    typedef typename Traits::const_reference    const_reference;
    typedef ptrdiff_t                           difference_type;

    typedef XALAN_STD_QUALIFIER random_access_iterator_tag  iterator_category;

    // The non-const iterator type.  In the case of the non-const instatiation, this
    // is the same type.
    typedef XalanDequeIterator<XalanDequeIteratorTraits<value_type>, XalanDeque>    Iterator;

    // The const version needs access to our private data members for copy construction and
    // assignment.  For the const instantiation, this is a superfluous friend declaration,
    // since it's the same type as the class itself.
    friend class XalanDequeIterator<XalanDequeConstIteratorTraits<value_type>, XalanDeque>;

    XalanDequeIterator(
                XalanDeque*     deque,
                size_type       pos) :
        m_deque(deque),
        m_pos(pos)
    {
    }

    // This is standard copy-construction for the non-const iterator type.  For the
    // const iterator type, this is copy construction from the non-const type, and the
    // compiler will generate the standard copy constructor.
    XalanDequeIterator(const Iterator&  iterator) :
        m_deque(iterator.m_deque),
        m_pos(iterator.m_pos)
    {
    }

    // This is the standard assignment operator for the non-const iterator type.
    // For the const iterator type, this is the assignment operator from the
    // non-const type, and the compiler will generate the standard assignment
    // operator.
    XalanDequeIterator&
    operator=(const Iterator&   iterator)
    {
        m_deque = iterator.m_deque;
        m_pos = iterator.m_pos;

        return *this;
    }

    XalanDequeIterator&
    operator++()
    {
        ++m_pos;

        return *this;
    }

    XalanDequeIterator
    operator++(int)
    {
        XalanDequeIterator temp = *this;
        ++m_pos;

        return temp;
    }

    XalanDequeIterator&
    operator--()
    {   
        --m_pos;

        return *this;
    }

    pointer
    operator->()
    {
        return &(*m_deque[m_pos]);
    }

    reference
    operator*()
    {
        return (*m_deque)[m_pos];
    }

    const_reference
    operator*() const
    {
        return (*m_deque)[m_pos];
    }

    XalanDequeIterator
    operator+(difference_type   difference) const
    {
        return XalanDequeIterator(m_deque, m_pos + difference);
    }

    XalanDequeIterator
    operator-(difference_type   difference) const
    {
        return XalanDequeIterator(m_deque, m_pos - difference);
    }

    difference_type
    operator-(const XalanDequeIterator&     theRHS) const
    {
        return m_pos - theRHS.m_pos;
    }

    bool
    operator==(const XalanDequeIterator&    theRHS) const
    {
        return theRHS.m_deque == m_deque &&
               theRHS.m_pos == m_pos;
    }

    bool
    operator!=(const XalanDequeIterator&   theRHS) const
    {
        return !(theRHS == *this);
    }

    bool
    operator<(const XalanDequeIterator&     theRHS) const
    {
        return m_pos < theRHS.m_pos;
    }

private:

    XalanDeque*     m_deque;

    size_type       m_pos;
};

/**
 * Xalan implementation of deque
 */
template <class Type, class ConstructionTraits = MemoryManagedConstructionTraits<Type> >
class XalanDeque
{
public:

    typedef size_t  size_type;

    typedef Type            value_type;
    typedef Type&           reference;
    typedef const Type&     const_reference;

    typedef XalanVector<Type, ConstructionTraits>   BlockType;
    typedef XalanVector<BlockType*>                 BlockIndexType;

    typedef XalanDeque<Type, ConstructionTraits>    ThisType;

    typedef XalanDequeIterator<XalanDequeIteratorTraits<value_type>, ThisType>          iterator;
    typedef XalanDequeIterator<XalanDequeConstIteratorTraits<value_type>, ThisType>     const_iterator;

#if defined(XALAN_HAS_STD_ITERATORS)
    typedef XALAN_STD_QUALIFIER reverse_iterator<iterator>          reverse_iterator_;
    typedef XALAN_STD_QUALIFIER reverse_iterator<const_iterator>    const_reverse_iterator_;
#elif defined(XALAN_RW_NO_CLASS_PARTIAL_SPEC)
    typedef typename iterator::iterator_category    iterator_category;

    // This is a specific case for the Rogue Wave STL on Solaris.
    typedef XALAN_STD_QUALIFIER reverse_iterator<
        iterator,
        iterator_category,
        value_type> reverse_iterator_;

    typedef XALAN_STD_QUALIFIER reverse_iterator<
        const_iterator,
        iterator_category,
        const value_type> const_reverse_iterator_;
#else
    typedef XALAN_STD_QUALIFIER reverse_iterator<
        iterator,
        value_type>        reverse_iterator_;

    typedef XALAN_STD_QUALIFIER reverse_iterator<
        const_iterator,
        value_type,
        const_reference>   const_reverse_iterator_;
#endif

    typedef reverse_iterator_           reverse_iterator;
    typedef const_reverse_iterator_     const_reverse_iterator;

    typedef typename ConstructionTraits::Constructor    Constructor;
    typedef typename Constructor::ConstructableType     ConstructableType;

    XalanDeque(
            MemoryManager&  memoryManager,
            size_type       initialSize = 0,
            size_type       blockSize = 10) :
        m_memoryManager(&memoryManager),
        m_blockSize(blockSize),
        m_blockIndex(memoryManager,
                    initialSize / blockSize + (initialSize % blockSize == 0 ? 0 : 1)),                    
        m_freeBlockVector(memoryManager)
    {
        const ConstructableType     defaultValue(*m_memoryManager);

        XALAN_USING_STD(fill_n)
        XALAN_USING_STD(back_inserter)

        fill_n(
            back_inserter(*this),
            initialSize, 
            defaultValue.value);
    }

    XalanDeque(
                const XalanDeque&   theRHS,
                MemoryManager&      theMemoryManager) :
        m_memoryManager(&theMemoryManager),
        m_blockSize(theRHS.m_blockSize),
        m_blockIndex(*theRHS.m_memoryManager,
                    theRHS.size() / theRHS.m_blockSize + (theRHS.size() % theRHS.m_blockSize == 0 ? 0 : 1)),
        m_freeBlockVector(theMemoryManager)
    {
        XALAN_USING_STD(copy)
        XALAN_USING_STD(back_inserter)

        copy(
            theRHS.begin(),
            theRHS.end(), 
            back_inserter(*this));
    }

   static XalanDeque*
   create(
            MemoryManager&  theManager,
            size_type       initialSize = 0,
            size_type       blockSize = 10)
    {
        XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

        ThisType* const     theResult =
            new (theGuard.get()) ThisType(theManager, initialSize, blockSize);

        theGuard.release();

        return theResult;
    }

    ~XalanDeque()
    {
        destroyBlockList(m_freeBlockVector);

        destroyBlockList(m_blockIndex);
    }

    iterator
    begin()
    {
        return iterator(this, 0);
    }

    const_iterator
    begin() const
    {
        return const_iterator(const_cast<XalanDeque*>(this), 0);
    }

    iterator
    end()
    {
        return iterator(this, size());
    }

    const_iterator 
    end() const
    {
         return const_iterator(const_cast<XalanDeque*>(this), size());
    }

    const_reverse_iterator 
    rbegin() const
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator 
    rend() const
    {
        return const_reverse_iterator(begin());
    }

    bool 
    empty() const
    {
        return m_blockIndex.empty();
    }

    size_type 
    size() const
    {
       if (m_blockIndex.empty())
       {
           return 0;
       }
       else
       {
           return (m_blockIndex.size() - 1) * m_blockSize
               +  m_blockIndex.back()->size();
       }
    }

    value_type&
    back()
    {
        return m_blockIndex.back()->back();
    }

    value_type&
    operator[](size_type    index)
    {
        BlockType&  block = *m_blockIndex[index / m_blockSize];

        return block[index % m_blockSize];
    }

    const value_type&
    operator[](size_type    index) const
    {
        BlockType&  block = *m_blockIndex[index / m_blockSize];

        return block[index % m_blockSize];
    }

    void
    clear()
    {
        typename BlockIndexType::iterator   iter = m_blockIndex.begin();

        m_freeBlockVector.reserve(m_freeBlockVector.size() + m_blockIndex.size());

        while (iter != m_blockIndex.end())
        {
            (*iter)->clear();
            m_freeBlockVector.push_back(*iter);
            ++iter;
        }

        m_blockIndex.clear();
    }

    void
    push_back(const value_type&     value)
    {
        if (m_blockIndex.empty() ||
            m_blockIndex.back()->size() >= m_blockSize)
        {
            pushNewIndexBlock();
        }

        m_blockIndex.back()->push_back(value);
    }

    void
    pop_back()
    {
        assert(!empty());

        BlockType&  lastBlock = *m_blockIndex.back();
        lastBlock.pop_back();

        if (lastBlock.empty())
        {
            m_freeBlockVector.push_back(&lastBlock);
            m_blockIndex.pop_back();
        }
    }

    void
    resize(size_type    newSize)
    {
        const ConstructableType     defaultValue(*m_memoryManager);

        if (newSize > size())
        {
            for (size_type i = 0; i < newSize - size(); ++i)
            {
                push_back(defaultValue.value);
            }
        }
        else
        {
            for (size_type i = 0; i < size() - newSize; ++i)
            {
                pop_back();
            }
        }
    }

    void
    swap(XalanDeque&    theRHS)
    {
        MemoryManager* const temp = m_memoryManager;
        m_memoryManager = theRHS.m_memoryManager;
        theRHS.m_memoryManager = temp;

        theRHS.m_blockIndex.swap(m_blockIndex);
        theRHS.m_freeBlockVector.swap(m_freeBlockVector);
    }

    XalanDeque&
    operator=(const XalanDeque&     theRHS) 
    {
        if (this != &theRHS)
        {
            XALAN_USING_STD(copy)
            XALAN_USING_STD(back_inserter)

            clear();

            copy(
                theRHS.begin(),
                theRHS.end(),
                back_inserter(*this));
        }

        return *this;
    }

    MemoryManager&
    getMemoryManager()
    {
        assert (m_memoryManager != 0);

        return *m_memoryManager;
    }

private:

    void
    pushNewIndexBlock()
    {
        // Allocate space first, so we don't have to worry
        // about an out-of-memory error once we've constructed
        // the new block.
        m_blockIndex.push_back(0);

        if (m_freeBlockVector.empty())
        {
            XalanConstruct(
                *m_memoryManager,
                m_blockIndex.back(),
                *m_memoryManager,
                m_blockSize);
        }
        else
        {
            m_blockIndex.back() = m_freeBlockVector.back();

            // Now that ownership has been transfered, pop
            // it off the free list.
            m_freeBlockVector.pop_back();
        }

        assert(m_blockIndex.back() != 0);
    }

    void
    destroyBlockList(BlockIndexType&    theBlockIndex)
    {
        typename BlockIndexType::iterator iter =
            theBlockIndex.begin();

        while (iter != theBlockIndex.end())
        {
            // Normally, we should be able to just call
            // the version of XalanDestroy() that accepts
            // a pointer, but Visual Studio 6 has issues
            // with partial ordering, so we're stuck with
            // this for now.
            if (*iter != 0)
            {
                XalanDestroy(*m_memoryManager, **iter);
            }

            ++iter;
        }
    }

    MemoryManager*      m_memoryManager;

    const size_type     m_blockSize;

    BlockIndexType	    m_blockIndex; 
    BlockIndexType	    m_freeBlockVector;


    // These are not implemented
    XalanDeque();

    XalanDeque(const XalanDeque&);
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANDEQUE_HEADER_GUARD_1357924680

