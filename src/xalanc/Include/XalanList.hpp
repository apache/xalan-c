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

#if !defined(XALANLIST_HEADER_GUARD_1357924680)
#define XALANLIST_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <cstddef>
#include <algorithm>
#include <cassert>
#include <new>
#include <iterator>
#include <stdexcept>



#include <xalanc/Include/XalanMemoryManagement.hpp>



XALAN_CPP_NAMESPACE_BEGIN



template <class Value>
struct XalanListIteratorTraits
{
	typedef Value	value_type;
	typedef Value&	reference;
	typedef Value*	pointer;
};

template <class Value>
struct XalanListConstIteratorTraits
{
	typedef Value	value_type;
	typedef const	Value&	reference;
	typedef const	Value*	pointer;
};

template<class XalanListTraits, class Node>
struct XalanListIteratorBase 
{
	typedef typename XalanListTraits::value_type	value_type;
	typedef typename XalanListTraits::reference		reference;
	typedef typename XalanListTraits::pointer		pointer;
	
	typedef ptrdiff_t		difference_type;

	typedef XALAN_STD_QUALIFIER bidirectional_iterator_tag iterator_category;

	typedef XalanListIteratorBase<XalanListIteratorTraits<value_type>, Node> iterator;

	XalanListIteratorBase(Node& node) : 
		currentNode(&node)
	{
	}

	XalanListIteratorBase(const iterator& theRhs) :
		currentNode(theRhs.currentNode)
	{
	}

	XalanListIteratorBase operator++()
	{
		currentNode = currentNode->next;
		return *this;
	}

	XalanListIteratorBase operator++(int)
	{
		Node& origNode = *currentNode;
		currentNode = currentNode->next;
		return XalanListIteratorBase(origNode);
	}

	XalanListIteratorBase operator--()
	{
		currentNode = currentNode->prev;
		return *this;
	}

	XalanListIteratorBase operator-(difference_type decrement) const 
	{
		Node* node = currentNode;
		while (decrement > 0)
		{
			node = node->prev;
			--decrement;
		};
		return XalanListIteratorBase(*node);
	}

	reference operator*() const
	{
		return currentNode->value;
	}

	pointer operator->() const
	{
		return &currentNode->value;
	}

	const XalanListIteratorBase & operator=(const XalanListIteratorBase& theRhs)
	{
		currentNode = theRhs.currentNode;
		return *this;
	}

	bool operator!=(const XalanListIteratorBase& theRhs) const 
	{
		return !operator==(theRhs);
	}

	bool operator==(const XalanListIteratorBase& theRhs) const 
	{
		return currentNode == theRhs.currentNode;
	}

	Node& node()
	{
		return *currentNode;
	}

	Node*	currentNode;
};



/**
 * Xalan implementation of a doubly linked list
 */
template <class Type>
class XalanList
{
public:


    typedef Type                value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;

    typedef XalanList<value_type>     ThisType;

	struct Node
	{
		Node(
				const value_type & theValue,
				Node& prevNode,
				Node& nextNode) : 
			value(theValue), 
			prev(&prevNode),
			next(&nextNode) 
		{
		}
	
		value_type	value;
		Node*		prev;
		Node*		next;
	};

	typedef XalanListIteratorBase<XalanListIteratorTraits<value_type>, Node>		iterator;
	
	typedef XalanListIteratorBase<XalanListConstIteratorTraits<value_type>, Node>	const_iterator;
			
#if defined(XALAN_HAS_STD_ITERATORS)
	typedef XALAN_STD_QUALIFIER reverse_iterator<iterator>	reverse_iterator_;
	typedef XALAN_STD_QUALIFIER reverse_iterator<const_iterator>	const_reverse_iterator_;
#elif defined(XALAN_RW_NO_CLASS_PARTIAL_SPEC)
	typedef XALAN_STD_QUALIFIER reverse_iterator<
		iterator,
		XALAN_STD_QUALIFIER bidirectional_iterator_tag,
		value_type> reverse_iterator_;
	typedef XALAN_STD_QUALIFIER reverse_iterator<
		const_iterator,
		XALAN_STD_QUALIFIER bidirectional_iterator_tag,
		const value_type> const_reverse_iterator_;
#else
	typedef XALAN_STD_QUALIFIER reverse_iterator<iterator, value_type>							reverse_iterator_;
	typedef XALAN_STD_QUALIFIER reverse_iterator<const_iterator, value_type, const_reference>	const_reverse_iterator_;
#endif

	typedef reverse_iterator_			reverse_iterator;
	typedef const_reverse_iterator_		const_reverse_iterator;

    typedef typename MemoryManagedConstructionTraits<value_type>::Constructor Constructor;

    XalanList(
            MemoryManagerType&  theManager) :
        m_memoryManager(&theManager),
        m_listHead(0),
		m_freeListHeadPtr(0)
    {
    }

    ~XalanList()
    {
		if (m_listHead != 0)
		{
			iterator pos = begin();
			while (pos != end())
			{
				destroyNode(pos++.node()); 
			}

			Node * freeNode = m_freeListHeadPtr;
			while (freeNode != 0)
			{
				Node * nextNode = freeNode->next;
				deallocate(freeNode);
				freeNode = nextNode;
			}

			deallocate(m_listHead);
		}
    }
    
    MemoryManagerType&
    getMemoryManager()
    {
    	assert(m_memoryManager != 0 );
    	
        return *m_memoryManager;
    }
    
    const MemoryManagerType&
    getMemoryManager() const
    {
    	assert(m_memoryManager != 0 );
    	
        return *m_memoryManager;
    }

	iterator
	begin()
	{
		return iterator(*(getListHead().next));
	}

	const_iterator
	begin() const
	{
		return const_iterator(*(getListHead().next));
	}

	iterator
	end()
	{
		return iterator(getListHead());
	}

	const_iterator
	end() const
	{
		return const_iterator(getListHead());
	}

	reverse_iterator
	rbegin()
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator
	rbegin() const
	{
		return const_reverse_iterator(end());
	}

	reverse_iterator
	rend()
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator
	rend() const
	{
		return const_reverse_iterator(begin());
	}

	reference
	front()
	{
		return *begin();
	}

	reference
	back()
	{
		return *(--end());
	}

	size_type
	size() const
	{
		size_type size = 0;
		const_iterator item = begin();
		while (item != end())
		{
			++size;
			++item;
		}
		return size;
	}

	bool
	empty() const
	{
		return (begin() == end()) != 0;
	}

    void 
	push_back(const value_type&     data)
    {
        constructNode(data, end());     
    }

	void
	push_front(const value_type&	data)
	{
		constructNode(data, begin());
	}

	void
	pop_front()
	{
		erase(begin());
	}

	void
	pop_back()
	{
		erase(--end());
	}

	iterator
	insert(const iterator& pos, const value_type& value)
	{
		return iterator(constructNode(value,pos));
	}

	void
	erase(iterator pos)
	{
		assert(pos != end());
		freeNode(pos.node());
	}

	void 
	splice(
            iterator    pos,
#if defined(NDEBUG)
            ThisType&   /* list */,
#else
            ThisType&   list,
#endif
            iterator    toInsert)
	{
		assert(m_memoryManager == list.m_memoryManager);

		if (pos != toInsert)
		{
			Node & posNode = pos.node();
			Node & toInsertNode = toInsert.node();

			toInsertNode.prev->next = toInsertNode.next;
			toInsertNode.next->prev = toInsertNode.prev;

			toInsertNode.prev = posNode.prev;
			toInsertNode.next = &posNode;

			posNode.prev->next = &toInsertNode;
			posNode.prev = &toInsertNode;
		}
	}

	void
	splice(
            iterator    pos,
#if defined(NDEBUG)
            ThisType&   /* list */,
#else
            ThisType&   list,
#endif
            iterator    toInsertFirst,
            iterator    toInsertLast)
	{
		assert(m_memoryManager == list.m_memoryManager);

		if (toInsertFirst != toInsertLast)
		{
			Node & posNode = pos.node();
			Node & toInsertFirstNode = toInsertFirst.node();
			Node & toInsertLastNode = *(toInsertLast.node().prev);

			toInsertFirstNode.prev->next = toInsertLastNode.next;
			toInsertLastNode.next->prev = toInsertFirstNode.prev;

			toInsertFirstNode.prev = posNode.prev;
			toInsertLastNode.next = &posNode;

			posNode.prev->next = &toInsertFirstNode;
			posNode.prev = &toInsertLastNode;
		}
	}
	
	void
	clear()
	{
		iterator pos = begin();
		while (pos != end())
		{
			freeNode(pos++.node());
		}
	}

	void swap(ThisType& theRHS)
	{
	#if defined(XALAN_NO_STD_NAMESPACE)
		::swap(m_memoryManager, theRHS.m_memoryManager);
		::swap(m_listHead, theRHS.m_listHead);
		::swap(m_freeListHeadPtr, theRHS.m_freeListHeadPtr);
	#else
		XALAN_STD_QUALIFIER swap(m_memoryManager, theRHS.m_memoryManager);
		XALAN_STD_QUALIFIER swap(m_listHead, theRHS.m_listHead);
		XALAN_STD_QUALIFIER swap(m_freeListHeadPtr, theRHS.m_freeListHeadPtr);
	#endif
	}


protected:

	Node& constructNode(const value_type& data, iterator pos)
	{
		Node * newNode = 0;
        Node * nextFreeNode = 0;
        
        if (m_freeListHeadPtr != 0)
        {
            newNode = m_freeListHeadPtr;
            nextFreeNode = m_freeListHeadPtr->next;
		}
		else
		{
           m_freeListHeadPtr = allocate(1);
           newNode = m_freeListHeadPtr;
		}

        Constructor::construct(&newNode->value, data, *m_memoryManager);
        new (&newNode->prev) Node*(pos.node().prev);
        new (&newNode->next) Node*(&(pos.node()));

		pos.node().prev->next = newNode;
		pos.node().prev = newNode;
		
        m_freeListHeadPtr = nextFreeNode;
		
		return *newNode;
	}

	void freeNode(Node& node)
	{
		node.prev->next = node.next;
		node.next->prev = node.prev;

		node.~Node();
		node.prev = 0;
		node.next = m_freeListHeadPtr;
		m_freeListHeadPtr = &node;
	}

	void destroyNode(Node& node)
	{
		assert(&node != m_listHead);
		node.~Node();
		deallocate(&node);
	}

	Node& getListHead()
	{
		if (0 == m_listHead)
		{
			m_listHead = allocate(1);
			m_listHead->next = m_listHead;
			m_listHead->prev = m_listHead;
		}

		return *m_listHead;
	}

	Node& getListHead() const
	{
		return const_cast<XalanList*>(this)->getListHead();
	}

	Node*
	allocate(size_type  size)
	{
		const size_type     theBytesNeeded = size * sizeof(Node);
		
		assert(m_memoryManager != 0);

		void* pointer = m_memoryManager->allocate(theBytesNeeded);

		assert( pointer != 0 );

		return (Node*) pointer;
	}

	
	void
	deallocate(Node*  pointer)
	{
		assert(m_memoryManager != 0);
		
		m_memoryManager->deallocate(pointer);
	}

	MemoryManagerType *	m_memoryManager;

	Node*				m_listHead;

	Node*				m_freeListHeadPtr;

private:
    // not defined
    XalanList();
	XalanList(const XalanList& theRhs);

	XalanList& operator=(const XalanList& theRhs);

};



XALAN_CPP_NAMESPACE_END

#endif  // XALANLIST_HEADER_GUARD_1357924680
