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

#if !defined(REUSABLEARENAALLOCATOR_INCLUDE_GUARD_1357924680)
#define REUSABLEARENAALLOCATOR_INCLUDE_GUARD_1357924680



#include <algorithm>



#include "ReusableArenaBlock.hpp"
#include "ArenaAllocator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



template<class ObjectType>
class ReusableArenaAllocator : public ArenaAllocator<ObjectType,
													 ReusableArenaBlock<ObjectType> >
{
public:

	typedef ReusableArenaBlock<ObjectType>				ReusableArenaBlockType;

	typedef typename ReusableArenaBlockType::size_type	size_type;

	typedef ArenaAllocator<ObjectType,
						   ReusableArenaBlockType>		BaseClassType;

	typedef ReusableArenaAllocator<ObjectType>          ThisType;

	typedef	XalanList<ReusableArenaBlockType*>			ArenaBlockListType;

	typedef	typename ArenaBlockListType::iterator				iterator;
	typedef	typename ArenaBlockListType::const_iterator			const_iterator;
	typedef	typename ArenaBlockListType::reverse_iterator		reverse_iterator;
	typedef	typename ArenaBlockListType::const_reverse_iterator const_reverse_iterator;


	/*
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	ReusableArenaAllocator(
                MemoryManagerType&  theManager,
                size_type	        theBlockSize, 
                bool                destroyBlocks = false) :
		BaseClassType(theManager, theBlockSize),
		m_destroyBlocks(destroyBlocks)
	{
	}

	virtual
	~ReusableArenaAllocator()
	{
	}

	/*
	 * Destroy the object, and free the block for re-use.
	 *
	 * @param theObject the address of the object.
	 * @return true if the object was deleted, false if not.
	 */
	bool
	destroyObject(ObjectType*	theObject)
	{
		bool bResult = false;

		assert ( theObject != 0 );

		if ( this->m_blocks.empty() )
			return bResult;

		iterator iTerator = this->m_blocks.begin();

		iterator iEnd = this->m_blocks.end();

		// first , run over unfull blocks ( that consentrated from the head )
		while( iTerator != iEnd 
					&& (*iTerator)->blockAvailable() )
		{
			if ((*iTerator)->ownsBlock(theObject) == true)
			{
				(*iTerator)->destroyObject(theObject);
				
				// move the block we have just deleted to the head of the list
				if (iTerator != this->m_blocks.begin())
				{
					// move the block to the beginning
					ReusableArenaBlockType* block = *iTerator;

					assert(block != 0);
					
					this->m_blocks.erase(iTerator);

					this->m_blocks.push_front(block);
				}

				if (m_destroyBlocks)
				{
					destroyBlock();
				}

				bResult = true;

				break;
			}

			++iTerator;
		}

		reverse_iterator rIterator = this->m_blocks.rbegin();

		reverse_iterator rEnd = this->m_blocks.rend();

		// if the block hasn't been found from the head , start with full blocks ( from the taile)
		while ( !bResult && rIterator != rEnd )
		{
			if ((*rIterator)->ownsBlock(theObject))
			{
				(*rIterator)->destroyObject(theObject);

				if (rIterator != this->m_blocks.rbegin())
				{
					// move the block to the beginning
					ReusableArenaBlockType* block = *iTerator;

					assert(block != 0);
					
					this->m_blocks.erase(iTerator);

					this->m_blocks.push_front(block);

				}

				if (m_destroyBlocks)
				{
					destroyBlock();
				}

				bResult = true;

				break; 
			}

			if ( *rIterator == *iTerator)
			{
				break;
			}
			else
			{
				++rIterator;
			}
		}

		return bResult;

		assert ( bResult );
	}

	/*
	 * Allocate a block of the appropriate size for an
	 * object.  Call commitAllocation() when after
	 * the object is successfully constructed.  You _must_
	 * commit an allocation before performing any other
	 * operation on the allocator.
	 * 
	 *
	 * @return A pointer to a block of memory
	 */
	virtual ObjectType*
	allocateBlock()
	{
		if( this->m_blocks.empty() 
			|| !this->m_blocks.front()->blockAvailable() )
		{
            this->m_blocks.push_front(
                ReusableArenaBlockType::create(
                    this->getMemoryManager(),
                    this->m_blockSize));
			
			assert( this->m_blocks.front() != 0 );
		}

		assert( this->m_blocks.front() != 0 );
		assert( this->m_blocks.front()->blockAvailable() );

		return this->m_blocks.front()->allocateBlock();
	}

	/*
	 * Commits the allocation of the previous
	 * allocateBlock() call.
	 *
	 * @param theObject A pointer to a block of memory
	 */
	virtual void
	commitAllocation(ObjectType*	theObject)
	{
		// Note that this-> is required by template lookup rules.
		assert( this->m_blocks.empty() == false );
		assert( this->m_blocks.front() != 0 );
		assert( this->m_blocks.front()->ownsBlock(theObject) == true );

		this->m_blocks.front()->commitAllocation(theObject);

		if( !this->m_blocks.front()->blockAvailable() )
		{
			ReusableArenaBlockType* fullBlock = this->m_blocks.front();

			assert ( fullBlock != 0 );

			this->m_blocks.pop_front();

			this->m_blocks.push_back( fullBlock );
		}
	}


	virtual bool
	ownsObject(const ObjectType*	theObject) const
	{
		if ( this->m_blocks.empty() )
			return false;

		const_iterator iTerator = this->m_blocks.begin();

		const_iterator iEnd = this->m_blocks.end();

		while( iTerator != iEnd 
					&& (*iTerator)->blockAvailable() )
		{
			if ((*iTerator)->ownsBlock(theObject) )
			{
				return true;
			}

			++iTerator;
		}

		const_reverse_iterator rIterator = this->m_blocks.rbegin();

		const_reverse_iterator rEnd = this->m_blocks.rend();

		while( rIterator != rEnd )
		{
			if ((*rIterator)->ownsBlock(theObject) )
			{
				return true;
			}

			if ( *iTerator == *rIterator )
			{
				break;
			}
			else
			{
				++rIterator;
			}
		}

		return false;
	}

protected:

	/*
	 * The method destroys an empty block from the head of the list.
	 * For eliminating multiple create/destroy operation, the block
     * is destroyed only if the second one is not full.
	 */
	void
	destroyBlock()
	{
        assert(m_destroyBlocks == true);

		if ( this->m_blocks.empty() == false)
		{
		    const_iterator iTerator = this->m_blocks.begin();

		    if ( (*iTerator)->isEmpty() )
		    {
			    ++iTerator;

			    if (iTerator == this->m_blocks.end() ||
				    (*iTerator)->blockAvailable() )
			    {
				    this->m_blocks.pop_front();
			    }
		    }
        }
	}

	// data members
	const bool  m_destroyBlocks;

private:

	// Not defined...
	ReusableArenaAllocator(const ReusableArenaAllocator<ObjectType>&);

	ReusableArenaAllocator<ObjectType>&
	operator=(const ReusableArenaAllocator<ObjectType>&);

	bool
	operator==(const ReusableArenaAllocator<ObjectType>&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(REUSABLEARENAALLOCATOR_INCLUDE_GUARD_1357924680)
