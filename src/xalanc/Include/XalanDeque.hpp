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
 * @author Matthew Hoyt (mhoyt@ca.ibm.com)
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
	typedef Value	        value_type;
	typedef Value&	        reference;
	typedef Value*	        pointer;
    typedef const Value&    const_reference;
};

template <class Value>
struct XalanDequeConstIteratorTraits
{
	typedef Value	        value_type;
	typedef const Value&	reference;
	typedef const Value*	pointer;
    typedef const Value&	const_reference;
};

template <class XalanDequeTraits, class XalanDeque>
struct  XalanDequeIterator
{
    typedef size_t  size_type;
    typedef typename XalanDequeTraits::value_type       value_type;
    typedef typename XalanDequeTraits::reference        reference;
    typedef typename XalanDequeTraits::pointer          pointer;
    typedef typename XalanDequeTraits::const_reference  const_reference;
    typedef ptrdiff_t		                            difference_type;

    typedef XalanDequeIterator<XalanDequeIteratorTraits<value_type>, XalanDeque> Iterator;

	typedef XALAN_STD_QUALIFIER random_access_iterator_tag iterator_category;

    XalanDequeIterator(XalanDeque*  deque,
                       size_type    pos) :
        m_deque(deque),
        m_pos(pos)
    {
    }

    XalanDequeIterator(const Iterator & iterator) :
        m_deque(iterator.m_deque),
        m_pos(iterator.m_pos)
    {
    }

    XalanDequeIterator& operator=(const Iterator & iterator)
    {
        m_deque = iterator.m_deque;
        m_pos = iterator.m_pos;
        return *this;
    }

    XalanDequeIterator& operator++()
    {
        ++m_pos;
        return *this;
    }

    XalanDequeIterator operator++(int)
    {
        XalanDequeIterator temp = *this;
        ++m_pos;
        return temp;
    }

    XalanDequeIterator& operator--()
    {   
        --m_pos;
        return *this;
    }

    pointer operator->()
    {
        return &(*m_deque[m_pos]);
    }

    reference operator*()
    {
        return (*m_deque)[m_pos];
    }

    const_reference operator*() const
    {
        return (*m_deque)[m_pos];
    }

    XalanDequeIterator operator+(difference_type difference) const
    {
        return XalanDequeIterator(m_deque, m_pos + difference);
    }

    XalanDequeIterator operator-(difference_type difference) const
    {
        return XalanDequeIterator(m_deque, m_pos - difference);
    }

    difference_type operator-(const XalanDequeIterator &theRhs) const
    {
        return m_pos - theRhs.m_pos;
    }

    bool operator==(const XalanDequeIterator & theRhs) const
    {
        return (theRhs.m_deque == m_deque)
            &&  theRhs.m_pos == m_pos;
    }

    bool operator!=(const XalanDequeIterator & theRhs) const
    {
        return !(theRhs == *this);
    }

    XalanDeque* m_deque;
    size_type   m_pos;
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

    typedef XalanVector<Type, ConstructionTraits>	    BlockType;

    typedef XalanVector<BlockType*> BlockIndexType;

    typedef XalanDeque<Type, ConstructionTraits>							ThisType;		
    typedef XalanDequeIterator<XalanDequeIteratorTraits<value_type>, ThisType> 		iterator;
    typedef XalanDequeIterator<XalanDequeConstIteratorTraits<value_type>, ThisType> 	const_iterator;

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

    XalanDeque(
    		MemoryManagerType& memoryManager,
            size_type initialSize = 0,
            size_type blockSize = 10) :
        m_memoryManager(&memoryManager),
        m_blockSize(blockSize),
        m_blockIndex(memoryManager,
                    initialSize / blockSize + (initialSize % blockSize == 0 ? 0 : 1)),                    
        m_freeBlockVector(memoryManager)
    {
        typename ConstructionTraits::Constructor::ConstructableType defaultValue(*m_memoryManager);

        XALAN_STD_QUALIFIER fill_n(XALAN_STD_QUALIFIER back_inserter(*this), initialSize, defaultValue.value);
    }

    XalanDeque(const XalanDeque& theRhs, MemoryManagerType& memoryManager) :
        m_memoryManager(&memoryManager),
        m_blockSize(theRhs.m_blockSize),
        m_blockIndex(*theRhs.m_memoryManager,
                    theRhs.size() / theRhs.m_blockSize + (theRhs.size() % theRhs.m_blockSize == 0 ? 0 : 1)),
        m_freeBlockVector(memoryManager)
    {
        XALAN_STD_QUALIFIER copy(theRhs.begin(), theRhs.end(), XALAN_STD_QUALIFIER back_inserter(*this));
    }
    
   static XalanDeque*
   create(
            MemoryManagerType&  theManager,
            size_type initialSize = 0,
            size_type blockSize = 10)
    {
        typedef XalanDeque ThisType;

        XalanMemMgrAutoPtr<ThisType, false> theGuard( theManager , (ThisType*)theManager.allocate(sizeof(ThisType)));

        ThisType* theResult = theGuard.get();

        new (theResult) ThisType(theManager, initialSize, blockSize);

        theGuard.release();

        return theResult;
    }
     
    ~XalanDeque()
    {
        clear();
        typename BlockIndexType::iterator iter = m_freeBlockVector.begin();

        while (iter != m_freeBlockVector.end())
        {
            (*iter)->~XalanVector<Type, ConstructionTraits>();
            deallocate(*iter);
            ++iter;
        }
    }

    iterator begin()
    {
        return iterator(this, 0);
    }

    const_iterator begin() const
    {
        return const_iterator(const_cast<XalanDeque*>(this), 0);
    }

    iterator end()
    {
        return iterator(this, size());
    }

    const_iterator end() const
    {
         return const_iterator(const_cast<XalanDeque*>(this), size());
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    bool empty() const
    {
        return m_blockIndex.empty();
    }

    size_type size() const
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

    value_type& back()
    {
        return m_blockIndex.back()->back();
    }

    value_type& operator[](size_type index)
    {
        BlockType & block = *(m_blockIndex[index / m_blockSize]);
        return block[index % m_blockSize];
    }

    const value_type& operator[](size_type index) const
    {
        BlockType & block = *(m_blockIndex[index / m_blockSize]);
        return block[index % m_blockSize];
    }

    void clear()
    {
        typename BlockIndexType::iterator iter = m_blockIndex.begin();

        m_freeBlockVector.reserve(m_freeBlockVector.size() + m_blockIndex.size());

        while (iter != m_blockIndex.end())
        {
            (*iter)->clear();
            m_freeBlockVector.push_back(*iter);
            ++iter;
        }
        
        m_blockIndex.clear();
    }

    void push_back(const value_type & value)
    {
        if (m_blockIndex.empty() ||
            m_blockIndex.back()->size() >= m_blockSize)
        {
            m_blockIndex.push_back(getNewBlock());
        }

        m_blockIndex.back()->push_back(value);
    }

    void pop_back()
    {
        BlockType & lastBlock = *(m_blockIndex.back());
        lastBlock.pop_back();
        if (lastBlock.empty())
        {
            m_freeBlockVector.push_back(&lastBlock);
            m_blockIndex.pop_back();
        }
    }

    void resize(size_type newSize)
    {
        typename ConstructionTraits::Constructor::ConstructableType defaultValue(*m_memoryManager);
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

    void swap(XalanDeque& theRhs)
    {
        MemoryManagerType* tempMemoryManager = m_memoryManager;
        m_memoryManager = theRhs.m_memoryManager;
        theRhs.m_memoryManager = tempMemoryManager;

        theRhs.m_blockIndex.swap(m_blockIndex);
        theRhs.m_freeBlockVector.swap(m_freeBlockVector);
    }

    XalanDeque & operator=(const XalanDeque & theRhs) 
    {
        clear();
        XALAN_STD_QUALIFIER copy(theRhs.begin(), theRhs.end(), XALAN_STD_QUALIFIER back_inserter(*this));
        return *this;
    }

    MemoryManagerType&
    getMemoryManager()
    {
        assert (m_memoryManager != 0);

        return *m_memoryManager;
    }
protected:

    BlockType* getNewBlock()
    {
        BlockType * newBlock;

        if (m_freeBlockVector.empty())
        {
            newBlock = allocate(1);
            new (&*newBlock) BlockType(*m_memoryManager, m_blockSize);
        }
        else
        {
            newBlock = m_freeBlockVector.back();
            m_freeBlockVector.pop_back();
        }

        assert (newBlock != 0);

        return newBlock;
    }

    BlockType*
    allocate(size_type  size)
    {
        const size_type     theBytesNeeded = size * sizeof(BlockType);

        BlockType* pointer = (BlockType*)m_memoryManager->allocate(theBytesNeeded);
        
        assert(pointer != 0);
        
        return pointer;
     }

    void
    deallocate(BlockType*  pointer)
    {
    	m_memoryManager->deallocate(pointer);
    }

    MemoryManagerType*  m_memoryManager;
    const size_type     m_blockSize;

    BlockIndexType     m_blockIndex; 
    BlockIndexType     m_freeBlockVector;
    
private:
	// Not implemented
	XalanDeque();
	XalanDeque(const XalanDeque&);
	
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANDEQUE_HEADER_GUARD_1357924680

