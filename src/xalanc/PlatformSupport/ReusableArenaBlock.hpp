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

#if !defined(REUSABLEARENABLOCK_INCLUDE_GUARD_1357924680)
#define REUSABLEARENABLOCK_INCLUDE_GUARD_1357924680



#include <xalanc/PlatformSupport/XalanBitmap.hpp>
#include <xalanc/PlatformSupport/ArenaBlock.hpp>




XALAN_CPP_NAMESPACE_BEGIN



template<class ObjectType>
class ReusableArenaBlock : public ArenaBlock<ObjectType>
{
public:

	typedef ArenaBlock<ObjectType>				BaseClassType;

	typedef typename BaseClassType::size_type	size_type;

	/*
	 * Construct an ArenaBlock of the specified size
	 * of objects.
	 *
	 * @param theBlockSize The size of the block (the number of objects it can contain).
	 */
	ReusableArenaBlock(size_type	theBlockSize) :
		BaseClassType(theBlockSize),
		m_freeList(theBlockSize),
		m_freeBlockCount(0)
	{
	}

	~ReusableArenaBlock()
	{
		// Note that this-> is required by template lookup rules.
		this->destroyAll();
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
		if (m_freeBlockCount == 0)
		{
			return BaseClassType::allocateBlock();
		}
		else
		{
			return getNextFromFreeList();
		}
	}

	/*
	 * Commit the previous allocation.
	 *
	 * @param theBlock the address that was returned by allocateBlock()
	 */
	virtual void
	commitAllocation(ObjectType*	theBlock)
	{
		assert(theBlock != 0);
		assert(m_freeBlockCount == 0 ||
			   theBlock == getNextFromFreeList());

		if (m_freeBlockCount == 0)
		{
			BaseClassType::commitAllocation(theBlock);
		}
		else
		{
			removeFromFreeList(theBlock);
		}
	}

	/*
	 * Find out if there is a block available.
	 *
	 * @return true if one is available, false if not.
	 */
	virtual bool
	blockAvailable() const
	{
		return m_freeBlockCount != 0 ? true : BaseClassType::blockAvailable();
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
		return BaseClassType::getCountAllocated() - m_freeBlockCount;
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
		return BaseClassType::ownsObject(theObject) && !isOnFreeList(theObject);
	}

	/*
	 * Destroy the object, and return the block to the free list.
	 * The behavior is undefined if the object pointed to is not
	 * owned by the block.
	 *
	 * @param theObject the address of the object.
	 */
	void
	destroyObject(ObjectType*	theObject)
	{
		assert(ownsObject(theObject) == true);

		this->m_destroyFunction(*theObject);

		addToFreeList(theObject);
	}

protected:

	/*
	 * Determine if the block should be destroyed.  Returns true,
	 * unless the object is on the free list.  The behavior is
	 * undefined if the object pointed to is not owned by the
	 * block.
	 *
	 * @param theObject the address of the object
	 * @return true if block should be destroyed, false if not.
	 */
	virtual bool
	shouldDestroyBlock(const ObjectType*	theObject) const
	{
		return !isOnFreeList(theObject);
	}

private:

	// Not implemented...
	ReusableArenaBlock(const ReusableArenaBlock<ObjectType>&);

	ReusableArenaBlock<ObjectType>&
	operator=(const ReusableArenaBlock<ObjectType>&);

	bool
	operator==(const ReusableArenaBlock<ObjectType>&) const;


	/*
	 * Determine if the block is on the free list.  The behavior is
	 * undefined if the object pointed to is not owned by the
	 * block.
	 *
	 * @param theObject the address of the object
	 * @return true if block is on the free list, false if not.
	 */
	bool
	isOnFreeList(const ObjectType*	theObject) const
	{
		if (m_freeBlockCount == 0)
		{
			return false;
		}
		else
		{
			const size_type		theOffset =
					this->getBlockOffset(theObject);

			return m_freeList.isSet(theOffset);
		}
	}

	/*
	 * Add a block to the free list.  The behavior is
	 * undefined if the object pointed to is not owned by the
	 * block.
	 *
	 * @param theObject the address of the object
	 */
	void
	addToFreeList(const ObjectType*		theObject)
	{
		const size_type		theOffset =
				this->getBlockOffset(theObject);

		m_freeList.set(theOffset);

		++m_freeBlockCount;
	}

	/*
	 * Remove a block from the free list.  The behavior is
	 * undefined if the object pointed to is not owned by the
	 * block.
	 *
	 * @param theObject the address of the object
	 */
	void
	removeFromFreeList(const ObjectType*	theObject)
	{
		const size_type		theOffset =
				this->getBlockOffset(theObject);

		m_freeList.clear(theOffset);

		--m_freeBlockCount;
	}

	/*
	 * Get the next block from the free list.  Returns 0 if
	 * the free list is empty.
	 *
	 * @return the address of the block
	 */
	ObjectType*
	getNextFromFreeList()
	{
		ObjectType*		theResult = 0;

		if (m_freeBlockCount > 0)
		{
			const size_type		theFreeListSize = m_freeList.getSize();

			for(size_type i = 0; i < theFreeListSize; ++i)
			{
				if (m_freeList.isSet(i) == true)
				{
					// Note that this-> is required by template lookup rules.
					theResult = this->getBlockAddress(i);

					break;
				}
			}
		}

		return theResult;
	}

	// Bitmap which tracks which blocks are not in use
	// and that should not be destroyed.
	XalanBitmap		m_freeList;

	// The number of blocks on the free list.)
	size_type		m_freeBlockCount;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(REUSABLEARENABLOCK_INCLUDE_GUARD_1357924680)
