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

#if !defined(ARENABLOCK_INCLUDE_GUARD_1357924680)
#define ARENABLOCK_INCLUDE_GUARD_1357924680



#include <algorithm>
#include <cassert>
#include <set>
#include <memory>



template<class ObjectType>
class ArenaBlockDestroy
{
public:

	void
	operator()(ObjectType&	theObject) const
	{
		theObject.ObjectType::~ObjectType();
	}
};



template<class ObjectType,
		 class DestroyFunctionType = ArenaBlockDestroy<ObjectType>,
#if defined(XALAN_NO_NAMESPACES)
		 class AllocatorType = allocator<ObjectType> >
#else
		 class AllocatorType = std::allocator<ObjectType> >
#endif
class ArenaBlock
{
public:

	typedef AllocatorType::size_type	size_type;

	ArenaBlock(size_type	theBlockCount) :
		m_objectCount(0),
		m_blockCount(theBlockCount),
		m_objectBlock(0),
		m_allocator()
	{
		assert(theBlockCount > 0);
	}

	~ArenaBlock()
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::for_each;
#endif

		// Destroy all existing objects...
		for_each(m_objectBlock,
				 m_objectBlock + m_objectCount,
				 m_destroyFunction);

		// Release the memory...
		m_allocator.deallocate(m_objectBlock, 0);
	}

	virtual ObjectType*
	allocateBlock()
	{
		// If no memory has yet been allocated, then allocate it...
		if (m_objectBlock == 0)
		{
			m_objectBlock = m_allocator.allocate(m_blockCount, 0);
		}
		assert(m_objectBlock != 0);

		// Any space left?
		if (m_objectCount == m_blockCount)
		{
			return 0;
		}
		else
		{
			return m_objectBlock + m_objectCount;
		}
	}

	// $$$ ToDo: How much error checking, etc. do we do here?  Is
	// it worth trying to throw exceptions when things are not
	// what they should be?
	virtual void
#if defined (NDEBUG)
	commitAllocation(ObjectType*	/* theBlock */)
#else
	commitAllocation(ObjectType*	theBlock)
#endif
	{
		assert(theBlock == m_objectBlock + m_objectCount);
		assert(m_objectCount < m_blockCount);

		m_objectCount++;
	}

	virtual bool
	blockAvailable() const
	{
		return m_objectCount < m_blockCount ? true : false;
	}

	size_type
	getCountAllocated() const
	{
		return m_objectCount;
	}

	size_type
	getBlockCount() const
	{
		return m_blockCount;
	}

	bool
	ownsObject(const ObjectType*	theObject) const
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::less;
#endif

		// Use less<>, since it's guaranteed to do pointer
		// comparisons correctly...
		less<const ObjectType*>		functor;

		if (functor(theObject, m_objectBlock) == false &&
			functor(theObject, m_objectBlock + m_objectCount) == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

protected:

	DestroyFunctionType		m_destroyFunction;

private:

	// Cannot and should not be implemented...
	ArenaBlock(const ArenaBlock&);

	ArenaBlock&
	operator=(const ArenaBlock&);

	bool
	operator==(const ArenaBlock&) const;

	// data members...
	size_type			m_objectCount;

	const size_type		m_blockCount;

	ObjectType*			m_objectBlock;

	AllocatorType		m_allocator;
};



#endif	// !defined(ARENABLOCK_INCLUDE_GUARD_1357924680)
