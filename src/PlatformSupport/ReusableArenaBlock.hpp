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


#if 0
template<class ObjectType,
		 class DestroyFunctionType = ArenaBlockDestroy<ObjectType> >
class ReusableArenaBlockDestroy
{
public:

	ReusableArenaBlockDestroy() :
		m_freeListSet()
	{
	}

	void
	operator()(ObjectType&	theObject) const
	{
		if (isOnFreeList(&theObject) == false)
		{
			m_destroyFunction(theObject);
		}
	}

	bool
	isOnFreeList(ObjectType*	theObject) const
	{
		return m_freeListSet.find(theObject) == m_freeListSet.end() ? false : true;
	}

	void
	destroyObject(ObjectType*	theObject)
	{
		assert(theObject != 0);

		m_destroyFunction(*theObject);

		m_freeListSet.insert(theObject);
	}

	ObjectType*
	getNextFromFreeList()
	{
		assert(m_freeListSet.size() != 0);

		const FreeListSetType::const_iterator	i = m_freeListSet.begin();

		return *i;
	}

	bool
	removeFromFreeList(ObjectType*	theObject)
	{
		assert(m_freeListSet.size() != 0);

		const FreeListSetType::iterator		i =
			m_freeListSet.find(theObject);

		if (i == m_freeListSet.end())
		{
			return false;
		}
		else
		{
			m_freeListSet.erase(i);

			return true;
		}
	}

#if defined(XALAN_NO_NAMESPACES)
	typedef set<ObjectType*>		FreeListSetType;
#else
	typedef std::set<ObjectType*>	FreeListSetType;
#endif

	FreeListSetType::size_type
	freeListSize() const
	{
		return m_freeListSet.size();
	}

private:

	// Destroy function to call.
	DestroyFunctionType		m_destroyFunction;

	// Bitmap which tracks which blocks are in use
	// that should not be destroyed.
	FreeListSetType			m_freeListSet;
};
#endif



template<class ObjectType,
		 class DestroyFunctionType = ArenaBlockDestroy<ObjectType>,
#if defined(XALAN_NO_NAMESPACES)
		 class AllocatorType = allocator<ObjectType> >
#else
		 class AllocatorType = std::allocator<ObjectType> >
#endif
class ReusableArenaBlock : public ArenaBlock<ObjectType,
											 DestroyFunctionType,
											 AllocatorType>
{
public:

	typedef ArenaBlock<ObjectType,
					   DestroyFunctionType,
					   AllocatorType>				BaseClassType;

	ReusableArenaBlock(size_type	theBlockCount) :
		BaseClassType(theBlockCount),
		m_freeList(theBlockCount),
		m_freeListSize(0)
	{
	}

	~ReusableArenaBlock()
	{
	}

	virtual ObjectType*
	allocateBlock()
	{
		if (m_freeListSize == 0)
		{
			return BaseClassType::allocateBlock();
		}
		else
		{
			return getNextFromFreeList();
		}
	}

	virtual void
	commitAllocation(ObjectType*	theBlock)
	{
		assert(theBlock != 0);
		assert(m_freeListSize == 0 ||
			   theBlock == getNextFromFreeList());

		if (m_freeListSize == 0)
		{
			BaseClassType::commitAllocation(theBlock);
		}
		else
		{
			removeFromFreeList(theBlock);
		}
	}

	virtual bool
	blockAvailable() const
	{
		return m_freeListSize != 0 ? true : BaseClassType::blockAvailable();
	}

	void
	destroyObject(ObjectType*	theObject)
	{
		assert(ownsObject(theObject) == true &&
			   isOnFreeList(theObject) == false);

		m_destroyFunction(*theObject);

		addToFreeList(theObject);
	}

protected:

	virtual bool
	shouldDestroyBlock(const ObjectType*	theObject) const
	{
		assert(ownsObject(theObject) == true);

		return !isOnFreeList(theObject);
	}

private:

	// Cannot and should not be implemented...
	ReusableArenaBlock(const ReusableArenaBlock&);

	ReusableArenaBlock&
	operator=(const ReusableArenaBlock&);

	bool
	operator==(const ReusableArenaBlock&) const;

	bool
	isOnFreeList(const ObjectType*	theObject) const
	{
		assert(ownsObject(theObject) == true);

		const size_type		theOffset =
				getBlockOffset(theObject);

		return m_freeList.isSet(theOffset);
	}

	void
	addToFreeList(const ObjectType*		theObject)
	{
		const size_type		theOffset =
				getBlockOffset(theObject);

		m_freeList.set(theOffset);

		++m_freeListSize;
	}

	void
	removeFromFreeList(const ObjectType*	theObject)
	{
		const size_type		theOffset =
				getBlockOffset(theObject);

		m_freeList.clear(theOffset);

		--m_freeListSize;
	}

	ObjectType*
	getNextFromFreeList()
	{
		ObjectType*		theResult = 0;

		const unsigned long	theFreeListSize = m_freeList.getSize();

		for(unsigned long i = 0; i < theFreeListSize; ++i)
		{
			if (m_freeList.isSet(i) == false)
			{
				theResult = getBlockAddress(i);
			}
		}

		return theResult;
	}

	// Bitmap which tracks which blocks are not in use
	// and that should not be destroyed.
	XalanBitmap		m_freeList;

	unsigned long	m_freeListSize;
};



#endif	// !defined(REUSABLEARENABLOCK_INCLUDE_GUARD_1357924680)
