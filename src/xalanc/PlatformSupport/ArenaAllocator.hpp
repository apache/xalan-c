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

#if !defined(ARENAALLOCATOR_INCLUDE_GUARD_1357924680)
#define ARENAALLOCATOR_INCLUDE_GUARD_1357924680



#include <algorithm>



#include <xalanc/Include/STLHelper.hpp>
#include <xalanc/Include/XalanList.hpp>



#include "ArenaBlock.hpp"



XALAN_CPP_NAMESPACE_BEGIN



template<class ObjectType,
#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
		 class ArenaBlockType>
#else
		 class ArenaBlockType = ArenaBlock<ObjectType> >
#endif
class ArenaAllocator
{
public:

    typedef ArenaAllocator<ObjectType, ArenaBlockType>  ThisType;

	typedef XalanList<ArenaBlockType*>	        ArenaBlockListType;

	typedef typename ArenaBlockType::size_type	size_type;

	/*
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	ArenaAllocator(
                MemoryManagerType&  theManager,
                size_type	        theBlockSize) :
		m_blockSize(theBlockSize),
		m_blocks(theManager)
	{
	}

	virtual
	~ArenaAllocator()
	{
		reset();
	}

    MemoryManagerType&
    getMemoryManager()
    {
        return m_blocks.getMemoryManager();
    }

    const MemoryManagerType&
    getMemoryManager() const
    {
        return m_blocks.getMemoryManager();
    }

    /*
	 * Get size of an ArenaBlock, that is, the number
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
	 * Set size of an ArenaBlock, that is, the number
	 * of objects in each block.  Only affects blocks
	 * allocated after the call.
	 *
	 * @param theSize The size of the block
	 */
	void
	setBlockSize(size_type	theSize)
	{
		m_blockSize = theSize;
	}

	/*
	 * Get the number of ArenaBlocks currently allocated.
	 *
	 * @return The number of blocks.
	 */
	size_type
	getBlockCount() const
	{
		return (size_type)m_blocks.size();
	}

	/*
	 * Allocate a block of the appropriate size for an
	 * object.  Call commitAllocation() when after
	 * the object is successfully constructed.
	 *
	 * @return A pointer to a block of memory
	 */
	virtual ObjectType*
	allocateBlock()
	{
		if (m_blocks.empty() == true ||
			m_blocks.back()->blockAvailable() == false)
		{
            m_blocks.push_back(
                ArenaBlockType::create(
                    getMemoryManager(),
                    m_blockSize));
		}
		assert(
            m_blocks.empty() == false &&
            m_blocks.back() != 0 &&
            m_blocks.back()->blockAvailable() == true);

		return m_blocks.back()->allocateBlock();
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
		assert(
            m_blocks.empty() == false &&
            m_blocks.back()->ownsBlock(theObject) == true);

		m_blocks.back()->commitAllocation(theObject);

        assert(m_blocks.back()->ownsObject(theObject) == true);
	}

	virtual bool
	ownsObject(const ObjectType*	theObject) const
	{
		bool	fResult = false;

        typedef typename ArenaBlockListType::const_reverse_iterator     const_reverse_iterator;

		// Search back for a block that may have allocated the object...
		const const_reverse_iterator    theEnd = this->m_blocks.rend();

		const_reverse_iterator  i = this->m_blocks.rbegin();

		while(i != theEnd)
		{
			assert(*i != 0);

			if ((*i)->ownsObject(theObject) == true)
			{
				fResult = true;

				break;
			}
			else
			{
				++i;
			}
		}

		return fResult;
	}

	virtual void
	reset()
	{
		XALAN_STD_QUALIFIER for_each(
			m_blocks.begin(),
			m_blocks.end(),
            DeleteFunctor<ArenaBlockType>(m_blocks.getMemoryManager()));

		m_blocks.clear();
	}

protected:

	// data members...
	size_type			m_blockSize;

	ArenaBlockListType	m_blocks;

private:

	// Not defined...
	ArenaAllocator(const ArenaAllocator<ObjectType, ArenaBlockType>&);

	ArenaAllocator<ObjectType, ArenaBlockType>&
	operator=(const ArenaAllocator<ObjectType, ArenaBlockType>&);

	bool
	operator==(const ArenaAllocator<ObjectType, ArenaBlockType>&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(ARENAALLOCATOR_INCLUDE_GUARD_1357924680)
