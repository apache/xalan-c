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

#if !defined(ARENABLOCKBASE_INCLUDE_GUARD_1357924680)
#define ARENABLOCKBASE_INCLUDE_GUARD_1357924680


#include <cassert>
#include <functional>
//#include <memory>

#include <xalanc/Include/XalanMemoryManagement.hpp>


#if !defined(XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION)
#include <xalanc/PlatformSupport/XalanAllocator.hpp>
#endif


XALAN_CPP_NAMESPACE_BEGIN


#if defined(XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION)

template <class Type>
class ArenaBlockAllocator
{
public:

	typedef typename T		size_type;
	typedef ptrdiff_t		difference_type;
	typedef Type*			pointer;
	typedef const Type*		const_pointer;
	typedef Type&			reference;
	typedef const Type&		const_reference;
	typedef Type			value_type;

    ArenaBlockAllocator(MemoryManagerType&  theManager) :
        m_memoryManager(theManager)
	{
	}

	~ArenaBlockAllocator()
	{
	}

    MemoryManagerType&
    getMemoryManager()
    {
        return m_memoryManager;
    }

	pointer
	allocate(
			size_type		size,
			const void*		/* hint */ = 0)
	{
		return (pointer)m_memoryManager.allocate(size * sizeof(Type));
	}

	void
	deallocate(
				pointer		p,
				size_type	/* n */)
	{
        if(p != 0)
        {
            m_memoryManager.deallocate(p);
        }
	}

private:

    // not defined
	ArenaBlockAllocator(const ArenaBlockAllocator<Type>&);

    ArenaBlockAllocator<Type>&
    operator=(const ArenaBlockAllocator<Type>&);

    MemoryManagerType&  m_memoryManager;
};
#endif



template<class ObjectType,
#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
		 class SizeType>
#else
		 class SizeType = size_t>
#endif
class ArenaBlockBase
{
public:

	typedef ArenaBlockBase<ObjectType, SizeType>    ThisType;

#if defined(XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION)
	typedef ArenaBlockAllocator<ObjectType>     AllocatorType;
#else
	typedef XalanAllocator<ObjectType>			AllocatorType;
#endif

	typedef SizeType							size_type;

    MemoryManagerType&
    getMemoryManager()
    {
        return m_allocator.getMemoryManager();
    }

	/*
	 * Find out if there is a block available.
	 *
	 * @return true if one is available, false if not.
	 */
	bool
	blockAvailable() const
	{
		return m_objectCount < m_blockSize ? true : false;
	}

	/*
	 * Find out if there are any block is allocated
	 *
	 * @return true if one is available, false if not.
	 */
	bool
	isEmpty() const
	{
		return m_objectCount == 0 ? true : false;
	}

	/*
	 * Get the number of objects currently allocated in the
	 * block.
	 *
	 * @return The number of objects allocated.
	 */
	size_type
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
		return isInBorders(theObject, m_blockSize);
	}

protected:

	ArenaBlockBase(
                MemoryManagerType&  theManager,
                size_type	        theBlockSize) :	
		m_allocator(theManager),
		m_objectCount(0),
		m_blockSize(theBlockSize),
#if defined(XALAN_NEW_STD_ALLOCATOR)
		m_objectBlock(m_allocator.allocate(m_blockSize))
#else
		m_objectBlock(m_allocator.allocate(m_blockSize, 0))
#endif
	{
		assert(theBlockSize > 0);

		assert(m_objectBlock != 0);
	}

	~ArenaBlockBase()
	{
		// Release the memory...
		m_allocator.deallocate(m_objectBlock, m_blockSize);

	}

	/*
	 * Determine if this block is located between beginning of the array
	 * and the "rightBorder" array member (not included)
	 * @param theObject the address of the object
	 *		  rightBorder the right 
	 * @return true if we own the object block, false if not.
	 */
	bool
	isInBorders(
            const ObjectType*	theObject,
            size_type           rightBoundary) const
	{
		if ( rightBoundary > m_blockSize )
		{
			rightBoundary = m_blockSize;
		}

        // Use less<>, since it's guaranteed to do pointer
		// comparisons correctly...
		XALAN_STD_QUALIFIER less<const ObjectType*>		functor;

		if (functor(theObject, m_objectBlock) == false &&
			functor(theObject, m_objectBlock + rightBoundary) == true)
		{
			return true;
		}
		else
		{
			return false;
		}
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
		assert(size_type( (theObject - m_objectBlock) / sizeof(ObjectType) ) < m_blockSize);

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

	// data members...
	AllocatorType		m_allocator;
	
	size_type			m_objectCount;

	const size_type		m_blockSize;

	ObjectType*			m_objectBlock;

private:

	// Not implemented...
	ArenaBlockBase(const ThisType&);

	ThisType&
	operator=(const ThisType&);

	bool
	operator==(const ThisType&) const;
};

XALAN_CPP_NAMESPACE_END



#endif	// !defined(ARENABLOCKBASE_INCLUDE_GUARD_1357924680)
