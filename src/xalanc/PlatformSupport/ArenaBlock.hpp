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
		 class SizeType>
#else
		 class SizeType = size_t>
#endif
class ArenaBlock : public ArenaBlockBase<ObjectType, SizeType>
{
public:

	typedef ArenaBlockBase<ObjectType, SizeType>	BaseClassType;

    typedef ArenaBlock<ObjectType, SizeType>        ThisType;

	typedef typename BaseClassType::size_type		size_type;

	/*
	 * Construct an ArenaBlock of the specified size
	 * of objects.
	 *
	 * @param theManager The memory manager instance for the block.
	 * @param theBlockSize The size of the block (the number of objects it can contain).
	 */
	ArenaBlock(
                MemoryManagerType&  theManager,
                size_type	        theBlockSize) :	
	    BaseClassType(theManager, theBlockSize)
	{
	}

	~ArenaBlock()
	{	
		assert( this->m_objectCount <= this->m_blockSize );
		
		for ( size_type i = 0; i < this->m_objectCount  ; ++i )
		{
			XalanDestroy(this->m_objectBlock[i]);
		}
	}

    static ThisType*
    create(
                MemoryManagerType&  theManager,
                size_type	        theBlockSize)
    {
        ThisType* theInstance;

        return XalanConstruct(
                    theManager,
                    theInstance,
                    theManager,
                    theBlockSize);
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

		++this->m_objectCount;
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
		return this->isInBorders(theObject, this->m_objectCount);
	}

private:

	// Not implemented...
	ArenaBlock(const ArenaBlock<ObjectType, SizeType>&);

	ArenaBlock<ObjectType, SizeType>&
	operator=(const ArenaBlock<ObjectType, SizeType>&);

	bool
	operator==(const ArenaBlock<ObjectType, SizeType>&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(ARENABLOCK_INCLUDE_GUARD_1357924680)
