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

#if !defined(XPATHALLOCATOR_INCLUDE_GUARD_135792455)
#define XPATHALLOCATOR_INCLUDE_GUARD_135792455



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/PlatformSupport/ArenaAllocator.hpp>



#include <xalanc/XPath/XPath.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XPathAllocator
{
public:

	typedef XPath							object_type;

#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
	typedef ArenaBlock<object_type>			ArenaBlockType;

	typedef ArenaAllocator<object_type,
						   ArenaBlockType>	ArenaAllocatorType;
#else
	typedef ArenaAllocator<object_type>		ArenaAllocatorType;
#endif

	typedef ArenaAllocatorType::size_type	size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XPathAllocator(MemoryManagerType&      theManager, size_type	theBlockCount);

	~XPathAllocator();

	/**
	 * Create an XPath object using the allocator.
	 * 
	 * @return a pointer to an XPath
	 */
	object_type*
	create();

	/**
	 * Determine if an object is owned by the allocator...
	 */
	bool
	ownsObject(const object_type*	theObject)
	{
		return m_allocator.ownsObject(theObject);
	}

	/**
	 * Delete all XPath objects allocated.	 
	 */	
	void
	reset();

	/**
	 * Get the number of objects in each block.
	 *
	 * @return The size of the block
	 */
	size_type
	getBlockCount() const
	{
		return m_allocator.getBlockCount();
	}

	/**
	 * Get the number of blocks currently allocated.
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
	XPathAllocator(const XPathAllocator&);

	XPathAllocator&
	operator=(const XPathAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XNUMBERALLOCATOR_INCLUDE_GUARD_135792455
