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
#include <vector>



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

#if defined (XALAN_NO_STD_NAMESPACE)
	typedef	vector<ReusableArenaBlockType*>				ArenaBlockListType;
#else
	typedef	std::vector<ReusableArenaBlockType*>		ArenaBlockListType;
#endif

	/*
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	ReusableArenaAllocator(size_type	theBlockSize) :
		BaseClassType(theBlockSize),
		m_lastBlockReferenced(0)
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
		bool	fSuccess = false;

		// Check this, just in case...
		if (m_lastBlockReferenced != 0 && m_lastBlockReferenced->ownsObject(theObject) == true)
		{
			m_lastBlockReferenced->destroyObject(theObject);

			fSuccess = true;
		}
		else
		{
			// Note that this-> is required by template lookup rules.
			const typename ArenaBlockListType::reverse_iterator	theEnd = this->m_blocks.rend();

			typename ArenaBlockListType::reverse_iterator	i = this->m_blocks.rbegin();

			while(i != theEnd)
			{
				if ((*i)->ownsObject(theObject) == true)
				{
					m_lastBlockReferenced = *i;

					m_lastBlockReferenced->destroyObject(theObject);

					fSuccess = true;

					break;
				}
				else
				{
					++i;
				}
			}
		}

		return fSuccess;
	}

	/*
	 * Allocate a block of the appropriate size for an
	 * object.  Call commitAllocation() when after
	 * the object is successfully constructed.  You _must_
	 * commit an allocation before performing any other
	 * operation on the allocator.
	 *
	 * @return A pointer to a block of memory
	 */
	virtual ObjectType*
	allocateBlock()
	{
		if (m_lastBlockReferenced == 0 ||
			m_lastBlockReferenced->blockAvailable() == false)
		{
			// Search back for a block with some space available...		
			const typename ArenaBlockListType::reverse_iterator	theEnd = this->m_blocks.rend();
			
			// Note that this-> is required by template lookup rules.
			typename ArenaBlockListType::reverse_iterator	i = this->m_blocks.rbegin();

			while(i != theEnd)
			{
				assert(*i != 0);

				if (*i != m_lastBlockReferenced && (*i)->blockAvailable() == true)
				{
					// Ahh, found one with free space.
					m_lastBlockReferenced = *i;

					break;
				}
				else
				{
					++i;
				}
			}

			if (i == theEnd)
			{
				// No blocks have free space available, so create a new block, and
				// push it on the list.
				// Note that this-> is required by template lookup rules.
				m_lastBlockReferenced = new ReusableArenaBlockType(this->m_blockSize);

				this->m_blocks.push_back(m_lastBlockReferenced);
			}
		}
		assert(m_lastBlockReferenced != 0 && m_lastBlockReferenced->blockAvailable() == true);

		return m_lastBlockReferenced->allocateBlock();
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
		assert(this->m_blocks.empty() == false && m_lastBlockReferenced != 0 && m_lastBlockReferenced->ownsBlock(theObject) == true);

		m_lastBlockReferenced->commitAllocation(theObject);
		assert(m_lastBlockReferenced->ownsObject(theObject) == true);
	}

	virtual void
	reset()
	{
		m_lastBlockReferenced = 0;

		BaseClassType::reset();
	}

	virtual bool
	ownsObject(const ObjectType*	theObject) const
	{
		bool	fResult = false;

		// If no block has ever been referenced, then we haven't allocated
		// any objects.
		if (m_lastBlockReferenced != 0)
		{
			// Check the last referenced block first.
			fResult = m_lastBlockReferenced->ownsObject(theObject);

			if (fResult == false)
			{
				fResult = BaseClassType::ownsObject(theObject);
			}
		}

		return fResult;
	}

private:

	// Not defined...
	ReusableArenaAllocator(const ReusableArenaAllocator<ObjectType>&);

	ReusableArenaAllocator<ObjectType>&
	operator=(const ReusableArenaAllocator<ObjectType>&);

	// Data members...
	ReusableArenaBlockType*		m_lastBlockReferenced;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(REUSABLEARENAALLOCATOR_INCLUDE_GUARD_1357924680)
