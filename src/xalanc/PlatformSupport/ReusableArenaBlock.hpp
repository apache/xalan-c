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


#include <xalanc/PlatformSupport/ArenaBlockBase.hpp>


XALAN_CPP_NAMESPACE_BEGIN

template<bool> struct CompileTimeError;

template<> struct CompileTimeError<true>{};

#define XALAN_STATIC_CHECK(expr) CompileTimeError<bool(expr)>()


template <class ObjectType,
#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
		 class size_Type>
#else
		 class size_Type  = unsigned short >
#endif

class ReusableArenaBlock : public ArenaBlockBase<ObjectType, size_Type>
{

public:

	typedef ArenaBlockBase<ObjectType, size_Type>	BaseClassType;

	typedef typename BaseClassType::size_type		size_type;

	struct NextBlock
	{
        enum { VALID_OBJECT_STAMP = 0xffddffdd };

		size_type		next;
		const int		verificationStamp;
		
		NextBlock( size_type _next):
			next(_next),
			verificationStamp(VALID_OBJECT_STAMP)
		{
		}

		bool
		isValidFor( size_type  rightBorder ) const
		{
			return ( ( verificationStamp == int(VALID_OBJECT_STAMP)) &&
				( next <= rightBorder ) ) ? true : false ;
		}
	};



	/*
	 * Construct an ArenaBlock of the specified size
	 * of objects.
	 *
	 * @param theBlockSize The size of the block (the number of objects it can contain).
	 */
	ReusableArenaBlock(size_type	theBlockSize) :
		BaseClassType(theBlockSize),
		m_firstFreeBlock(0),
		m_nextFreeBlock(0)

	{
		XALAN_STATIC_CHECK(sizeof(ObjectType) >= sizeof(NextBlock));
		
		for( size_type i = 0; i < this->m_blockSize; ++i )
		{
			new ( reinterpret_cast<NextBlock*>(&(this->m_objectBlock[i])) ) NextBlock( (size_type)(i + 1) );
		}
	}

	~ReusableArenaBlock()
	{
		size_type removedObjects = 0;

		NextBlock* pStruct = 0;

		for ( size_type i = 0 ; i < this->m_blockSize && (removedObjects < this->m_objectCount) ; ++i )
		{
			pStruct = reinterpret_cast<NextBlock*>(&(this->m_objectBlock[i]));

			if ( isOccupiedBlock(pStruct) )
			{
				this->m_objectBlock[i].~ObjectType();

				++removedObjects;
			}
		}
	}


	/*
	 * Allocate a block.  Once the object is constructed, you must call
	 * commitAllocation().
	 *
	 * @return a pointer to the new block.
	 */
	ObjectType*
	allocateBlock()
	{
		if ( this->m_objectCount == this->m_blockSize )
		{
			assert ( this->m_firstFreeBlock == (this->m_blockSize + 1) );

			return 0;
		}
		else
		{
			assert( this->m_objectCount < this->m_blockSize );

			ObjectType*		theResult = 0;

			assert ( this->m_firstFreeBlock <= this->m_blockSize );
			assert ( this->m_nextFreeBlock <= this->m_blockSize );

			// check if any part was allocated but not commited
			if( this->m_firstFreeBlock != this->m_nextFreeBlock)
			{
				// return then againg the previouse allocated block and wait for commitment
				theResult = this->m_objectBlock + this->m_firstFreeBlock;
			}
			else
			{
				theResult = this->m_objectBlock + this->m_firstFreeBlock;

				assert(size_type( theResult - this->m_objectBlock ) < this->m_blockSize);

				this->m_nextFreeBlock = (reinterpret_cast<NextBlock*>(theResult))->next;

				assert ( ( reinterpret_cast<NextBlock*>(theResult ))->isValidFor( this->m_blockSize ) );
				assert ( this->m_nextFreeBlock <= this->m_blockSize );

				++this->m_objectCount;
			}

			return theResult;
		}
	}

	/*
	 * Commit the previous allocation.
	 *
	 * @param theBlock the address that was returned by allocateBlock()
	 */
	void
	commitAllocation(ObjectType* /*	theBlock */)
	{
		this->m_firstFreeBlock = this->m_nextFreeBlock;

		assert ( this->m_objectCount <= this->m_blockSize );
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
		// check if any uncommited block is there, add it to the list
		if ( this->m_firstFreeBlock != this->m_nextFreeBlock )
		{
			// return it to pull of the free blocks
			void* const     p = this->m_objectBlock + this->m_firstFreeBlock;

			new (p) NextBlock(this->m_nextFreeBlock);

			this->m_nextFreeBlock = this->m_firstFreeBlock;
		}

		assert(ownsObject(theObject) == true);
		assert(shouldDestroyBlock(theObject));

		theObject->~ObjectType();

		new (theObject) NextBlock(this->m_firstFreeBlock);

		m_firstFreeBlock = this->m_nextFreeBlock = size_type(theObject - this->m_objectBlock);

		assert (this->m_firstFreeBlock <= this->m_blockSize);

		--this->m_objectCount;
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
	bool
	ownsObject(const ObjectType*	theObject) const
	{
		assert ( theObject != 0 );

		return isOccupiedBlock( reinterpret_cast<const NextBlock*>(theObject) );
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
	bool
	shouldDestroyBlock(const ObjectType*	theObject) const
	{
		assert( size_type(theObject - this->m_objectBlock) < this->m_blockSize);

        return !isOnFreeList(theObject);
	}

	bool
	isOccupiedBlock(const NextBlock*    block)const
	{
		assert( block !=0 );

		return !( ownsBlock(reinterpret_cast<const ObjectType*>(block)) && block->isValidFor(this->m_blockSize) );
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
		if ( this->m_objectCount == 0 )
		{
			return false;
		}
		else
		{
			ObjectType*     pRunPtr = this->m_objectBlock + this->m_firstFreeBlock;

			for ( size_type i = 0; i < (this->m_blockSize - this->m_objectCount); ++i)
			{
				assert ( ownsBlock( pRunPtr ) );

				if (pRunPtr == theObject)
				{
					return true;
				}
				else
				{
					NextBlock* const    p = reinterpret_cast<NextBlock*>(pRunPtr);

					assert( p->isValidFor( this->m_blockSize ) );

					pRunPtr = this->m_objectBlock + p->next;
				}
			}

			return false;
		}
	}


    // Data members...
	size_type   m_firstFreeBlock;

	size_type	m_nextFreeBlock;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(REUSABLEARENABLOCK_INCLUDE_GUARD_1357924680)
