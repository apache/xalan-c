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

#if !defined(REUSABLEARENAALLOCATOR_INCLUDE_GUARD_1357924680)
#define REUSABLEARENAALLOCATOR_INCLUDE_GUARD_1357924680



#include <algorithm>
#include <vector>



#include "ReusableArenaBlock.hpp"
#include "ArenaAllocator.hpp"



template<class ObjectType>
class ReusableArenaAllocator : public ArenaAllocator<ObjectType,
													 ReusableArenaBlock<ObjectType> >
{
public:

	typedef ReusableArenaBlock<ObjectType>				ReusableArenaBlockType;

	typedef typename ReusableArenaBlockType::size_type	size_type;

	typedef ArenaAllocator<ObjectType,
						   ReusableArenaBlockType>		BaseClassType;

	// $$$ ToDo: This typedef is here because of a bug in gcc.
#if defined (XALAN_NO_NAMESPACES)
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
		assert(this->m_blocks.size() != 0 && m_lastBlockReferenced != 0 && m_lastBlockReferenced->ownsBlock(theObject) == true);

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



#endif	// !defined(REUSABLEARENAALLOCATOR_INCLUDE_GUARD_1357924680)
