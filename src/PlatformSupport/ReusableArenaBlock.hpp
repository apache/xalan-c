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

#if !defined(REUSABLEARENABLOCK_INCLUDE_GUARD_1357924680)
#define REUSABLEARENABLOCK_INCLUDE_GUARD_1357924680



#include <PlatformSupport/XalanBitmap.hpp>
#include <PlatformSupport/ArenaBlock.hpp>




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

		m_destroyFunction(*theObject);

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
					getBlockOffset(theObject);

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
				getBlockOffset(theObject);

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
				getBlockOffset(theObject);

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
			const unsigned long	theFreeListSize = m_freeList.getSize();

			for(unsigned long i = 0; i < theFreeListSize; ++i)
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
	unsigned long	m_freeBlockCount;
};



#endif	// !defined(REUSABLEARENABLOCK_INCLUDE_GUARD_1357924680)
