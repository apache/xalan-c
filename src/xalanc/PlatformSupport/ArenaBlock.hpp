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

#if !defined(ARENABLOCK_INCLUDE_GUARD_1357924680)
#define ARENABLOCK_INCLUDE_GUARD_1357924680



#include <algorithm>
#include <cassert>
#include <set>
#include <memory>



#if defined(XALAN_NO_STD_ALLOCATORS) && !defined(XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION)
#include <xalanc/PlatformSupport/XalanAllocator.hpp>
#endif



XALAN_CPP_NAMESPACE_BEGIN



#if defined(XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION)

template <class Type>
class ArenaBlockAllocator
{
public:

	typedef size_t			size_type;
	typedef ptrdiff_t		difference_type;
	typedef Type*			pointer;
	typedef const Type*		const_pointer;
	typedef Type&			reference;
	typedef const Type&		const_reference;
	typedef Type			value_type;

	ArenaBlockAllocator()
	{
	}

	ArenaBlockAllocator(const ArenaBlockAllocator<Type>&)
	{
	};

	~ArenaBlockAllocator()
	{
	}

	pointer
	allocate(
			size_type		size,
			const void*		/* hint */ = 0)
	{
		return (pointer)operator new(size * sizeof(Type));
	}

	void
	deallocate(
				pointer		p,
				size_type	/* n */)
	{
		operator delete(p);
	}
};
#endif



template<class ObjectType>
class ArenaBlockDestroy
{
public:

	void
	operator()(ObjectType&	theObject) const
	{
#if defined(XALAN_EXPLICIT_SCOPE_IN_TEMPLATE_BUG)
		theObject.~ObjectType();
#else
		theObject.ObjectType::~ObjectType();
#endif
	}
};



template<class ObjectType>
class ArenaBlock
{
public:

#if defined(XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION)
	typedef ArenaBlockAllocator<ObjectType>		AllocatorType;
#elif defined(XALAN_NO_STD_ALLOCATORS)
	typedef XalanAllocator<ObjectType>			AllocatorType;
#else
	typedef std::allocator<ObjectType>			AllocatorType;
#endif

	typedef ArenaBlockDestroy<ObjectType>		DestroyFunctionType;

	typedef typename AllocatorType::size_type	size_type;

	/*
	 * Construct an ArenaBlock of the specified size
	 * of objects.
	 *
	 * @param theBlockSize The size of the block (the number of objects it can contain).
	 */
	ArenaBlock(size_type	theBlockSize) :
		m_destroyFunction(DestroyFunctionType()),
		m_objectCount(0),
		m_blockSize(theBlockSize),
		m_objectBlock(0),
		m_allocator()
	{
		assert(theBlockSize > 0);
	}

	virtual 
	~ArenaBlock()
	{
		destroyAll();

		// Release the memory...
		m_allocator.deallocate(m_objectBlock, m_blockSize);
	}

	/*
	 * Allocate a block.  Once the object is constructed, you must call
	 * commitAllocation().
	 *
	 * @return a pointer to the new block.
	 */
	virtual ObjectType*
	allocateBlock()
	{
		// Any space left?
		if (m_objectCount == m_blockSize)
		{
			return 0;
		}
		else
		{
			// If no memory has yet been allocated, then allocate it...
			if (m_objectBlock == 0)
			{
#if defined(XALAN_NEW_STD_ALLOCATOR)
				m_objectBlock = m_allocator.allocate(m_blockSize);
#else
				m_objectBlock = m_allocator.allocate(m_blockSize, 0);
#endif
			}
			assert(m_objectBlock != 0);

			return m_objectBlock + m_objectCount;
		}
	}

	/*
	 * Commit the previous allocation.
	 *
	 * @param theBlock the address that was returned by allocateBlock()
	 */
	virtual void
#if defined (NDEBUG)
	commitAllocation(ObjectType*	/* theBlock */)
#else
	commitAllocation(ObjectType*	theBlock)
#endif
	{
		assert(theBlock == m_objectBlock + m_objectCount);
		assert(m_objectCount < m_blockSize);

		m_objectCount++;
	}

	/*
	 * Find out if there is a block available.
	 *
	 * @return true if one is available, false if not.
	 */
	virtual bool
	blockAvailable() const
	{
		return m_objectCount < m_blockSize ? true : false;
	}

	/*
	 * Get the number of objects currently allocated in the
	 * block.
	 *
	 * @return The number of objects allocated.
	 */
	virtual size_type
	getCountAllocated() const
	{
		return m_objectCount;
	}

	/*
	 * Get the block size, that is, the number
	 * of objects in each block.
	 *
	 * @return The size of the block
	 */
	size_type
	getBlockSize() const
	{
		return m_blockSize;
	}

	/*
	 * Determine if this block owns the specified object.  Note
	 * that even if the object address is within our block, this
	 * call will return false if no object currently occupies the
	 * block.  See also ownsBlock().
	 *
	 * @param theObject the address of the object.
	 * @return true if we own the object, false if not.
	 */
	virtual bool
	ownsObject(const ObjectType*	theObject) const
	{
		// Use less<>, since it's guaranteed to do pointer
		// comparisons correctly...
		XALAN_STD_QUALIFIER less<const ObjectType*>		functor;

		if (functor(theObject, m_objectBlock) == false &&
			functor(theObject, m_objectBlock + m_objectCount) == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/*
	 * Determine if this block owns the specified object block.
	 * Note that, unlike ownsObject(), there does not need to
	 * be an object at the address.
	 *
	 * @param theObject the address of the object
	 * @return true if we own the object block, false if not.
	 */
	bool
	ownsBlock(const ObjectType*		theObject) const
	{
		// Use less<>, since it's guaranteed to do pointer
		// comparisons correctly...
		XALAN_STD_QUALIFIER less<const ObjectType*>		functor;

		if (functor(theObject, m_objectBlock) == false &&
			functor(theObject, m_objectBlock + m_blockSize) == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/*
	 * Destroy all objects in the block.  You can then reuse the
	 * block.
	 */
	void
	destroyAll()
	{
		// Destroy all existing objects...
		XALAN_STD_QUALIFIER for_each(m_objectBlock,
				 m_objectBlock + m_objectCount,
				 DeleteFunctor(*this, m_destroyFunction));

		m_objectCount = 0;
	}

protected:

	/*
	 * Determine if the block should be destroyed.  Called by
	 * an instance of DeleteFunctor, this function is for
	 * deriving classes that might want to control the destruction
	 * of things.
	 *
	 * @param theObject the address of the object
	 * @return true if block should be destroyed, false if not.
	 */
	virtual bool
	shouldDestroyBlock(const ObjectType*	/* theObject */) const
	{
		return true;
	}

	/*
	 * Determine the offset into the block for the given address.
	 * Behavior is undefined if the address is not within our
	 * block
	 *
	 * @param theObject the address of the object
	 * @return the offset
	 */
	size_type
	getBlockOffset(const ObjectType*	theObject) const
	{
		assert(size_type(theObject - m_objectBlock) < m_blockSize);

		return theObject - m_objectBlock;
	}

	/*
	 * Determine the address within our block of the object
	 * at the specified offset.
	 * Behavior is undefined if the offset is greater than the
	 * block size.
	 *
	 * @param theObject the address of the object
	 * @return the offset
	 */
	ObjectType*
	getBlockAddress(size_type	theOffset) const
	{
		assert(theOffset < m_blockSize);

		return m_objectBlock + theOffset;
	}

	struct DeleteFunctor
	{
		DeleteFunctor(
				const ArenaBlock<ObjectType>&	theArenaBlock,
				const DestroyFunctionType&		theDestroyFunction) :
			m_arenaBlock(theArenaBlock),
			m_destroyFunction(theDestroyFunction)
		{
		}

		void
		operator()(ObjectType&	theObject) const
		{
			if (m_arenaBlock.shouldDestroyBlock(&theObject) == true)
			{
				m_destroyFunction(theObject);
			}
		}

	private:

		const ArenaBlock<ObjectType>&	m_arenaBlock;
		const DestroyFunctionType&		m_destroyFunction;
	};

	friend struct DeleteFunctor;

	const DestroyFunctionType	m_destroyFunction;

private:

	// Not implemented...
	ArenaBlock(const ArenaBlock<ObjectType>&);

	ArenaBlock<ObjectType>&
	operator=(const ArenaBlock<ObjectType>&);

	bool
	operator==(const ArenaBlock<ObjectType>&) const;


	// data members...
	size_type			m_objectCount;

	const size_type		m_blockSize;

	ObjectType*			m_objectBlock;

	AllocatorType		m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(ARENABLOCK_INCLUDE_GUARD_1357924680)
