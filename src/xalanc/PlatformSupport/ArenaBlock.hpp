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


#include <xalanc/PlatformSupport/ArenaBlockBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN


template<class ObjectType,
#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
		 class size_Type>
#else
		 class size_Type  = size_t >
#endif
class ArenaBlock : public ArenaBlockBase<ObjectType,size_Type>
{
public:
	typedef ArenaBlockBase<ObjectType,size_Type>		BaseClassType;
	
	typedef typename BaseClassType::size_type		size_type;


	/*
	 * Construct an ArenaBlock of the specified size
	 * of objects.
	 *
	 * @param theBlockSize The size of the block (the number of objects it can contain).
	 */
	ArenaBlock(size_type	theBlockSize) :	
	BaseClassType(theBlockSize)
	{
	}

	 
	~ArenaBlock()
	{	
		assert( this->m_objectCount <= this->m_blockSize );

		
		for ( size_type i = 0; i < this->m_objectCount  ; ++i )
		{
			this->m_objectBlock[i].~ObjectType();
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
		// Any space left?
		if (this->m_objectCount == this->m_blockSize)
		{
			return 0;
		}
		else
		{
			assert(this->m_objectBlock != 0);

			return this->m_objectBlock + this->m_objectCount;
		}
	}

	/*
	 * Commit the previous allocation.
	 *
	 * @param theBlock the address that was returned by allocateBlock()
	 */
	void
#if defined (NDEBUG)
	commitAllocation(ObjectType*	/* theBlock */)
#else
	commitAllocation(ObjectType*	theBlock)
#endif
	{
		assert(theBlock == this->m_objectBlock + this->m_objectCount);
		assert(this->m_objectCount < this->m_blockSize);

		this->m_objectCount++;
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
		return isInBorders(theObject, this->m_objectCount);
	}
private:

	// Not implemented...
	ArenaBlock(const ArenaBlock<ObjectType>&);

	ArenaBlock<ObjectType>&
	operator=(const ArenaBlock<ObjectType>&);

	bool
	operator==(const ArenaBlock<ObjectType>&) const;

};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(ARENABLOCK_INCLUDE_GUARD_1357924680)
