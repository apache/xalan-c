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

#if !defined(XNUMBERALLOCATOR_INCLUDE_GUARD_135792455)
#define XNUMBERALLOCATOR_INCLUDE_GUARD_135792455



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XPath/XNumber.hpp>



#include <xalanc/PlatformSupport/ReusableArenaAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XNumberAllocator
{
public:

	typedef XNumber			number_type;

	typedef ReusableArenaAllocator<number_type>		ArenaAllocatorType;
	typedef ArenaAllocatorType::size_type			size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XNumberAllocator(MemoryManagerType&      theManager, size_type	theBlockCount);

	~XNumberAllocator();

	/**
	 * Create an XNumber object using allocator from a string.
	 * 
	 * @param theString     source number
	 *
	 * @return a pointer to a number
	 */
	number_type*
	createNumber(double		theNumber);


	/**
	 * Delete an XNumber object from allocator.	 
	 */
	bool
	destroy(number_type*	theNumber);

	/**
	 * Determine if an object is owned by the allocator...
	 */
	bool
	ownsObject(const number_type*	theObject)
	{
		return m_allocator.ownsObject(theObject);
	}

	/**
	 * Delete all XNumber objects from allocator.	 
	 */	
	void
	reset();

	/**
	 * Get size of an ArenaBlock, that is, the number
	 * of objects in each block.
	 *
	 * @return The size of the block
	 */
	size_type
	getBlockCount() const
	{
		return m_allocator.getBlockCount();
	}

	/**
	 * Get the number of ArenaBlocks currently allocated.
	 *
	 * @return The number of blocks.
	 */
	size_type
	getBlockSize() const
	{
		return m_allocator.getBlockSize();
	}

private:

	// Not implemented...
	XNumberAllocator(const XNumberAllocator&);

	XNumberAllocator&
	operator=(const XNumberAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XNUMBERALLOCATOR_INCLUDE_GUARD_135792455
