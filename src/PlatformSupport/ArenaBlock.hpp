/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#if !defined(ARENABLOCK_INCLUDE_GUARD_1357924680)
#define ARENABLOCK_INCLUDE_GUARD_1357924680



#include <algorithm>
#include <cassert>
#include <set>
#include <memory>



#if defined(XALAN_NO_STD_ALLOCATORS) && !defined(XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION)
#include <PlatformSupport/XalanAllocator.hpp>
#endif


#define XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION
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
		theObject.ObjectType::~ObjectType();
	}
};



template<class ObjectType>
class ArenaBlock
{
public:

#if defined(XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION)
	typedef ArenaBlockAllocator<ObjectType>	AllocatorType;
#elif defined(XALAN_NO_STD_ALLOCATORS)
	typedef XalanAllocator<ObjectType>		AllocatorType;
#else
	typedef std::allocator<ObjectType>		AllocatorType;
#endif

	typedef ArenaBlockDestroy<ObjectType>	DestroyFunctionType;

	typedef AllocatorType::size_type		size_type;

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
		m_allocator.deallocate(m_objectBlock, 0);
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
		// If no memory has yet been allocated, then allocate it...
		if (m_objectBlock == 0)
		{
			m_objectBlock = m_allocator.allocate(m_blockSize, 0);
		}
		assert(m_objectBlock != 0);

		// Any space left?
		if (m_objectCount == m_blockSize)
		{
			return 0;
		}
		else
		{
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
#if !defined(XALAN_NO_NAMESPACES)
		using std::less;
#endif

		// Use less<>, since it's guaranteed to do pointer
		// comparisons correctly...
		less<const ObjectType*>		functor;

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
#if !defined(XALAN_NO_NAMESPACES)
		using std::less;
#endif

		// Use less<>, since it's guaranteed to do pointer
		// comparisons correctly...
		less<const ObjectType*>		functor;

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
#if !defined(XALAN_NO_NAMESPACES)
		using std::for_each;
#endif

		// Destroy all existing objects...
		for_each(m_objectBlock,
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
		assert(theObject - m_objectBlock < m_blockSize);

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

	const DestroyFunctionType		m_destroyFunction;

private:

	// Not implemented...
	ArenaBlock(const ArenaBlock<ObjectType>&);

	ArenaBlock<ObjectType>&
	operator=(const ArenaBlock<ObjectType>&);

	bool
	operator==(const ArenaBlock<ObjectType>&) const;


	// data members...
	size_type				m_objectCount;

	const size_type			m_blockSize;

	ObjectType*				m_objectBlock;

	AllocatorType			m_allocator;
};



#endif	// !defined(ARENABLOCK_INCLUDE_GUARD_1357924680)
