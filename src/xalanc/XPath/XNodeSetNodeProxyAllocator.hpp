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

#if !defined(XNODESETNODEPROXYALLOCATOR_INCLUDE_GUARD_12455133)
#define XNODESETNODEPROXYALLOCATOR_INCLUDE_GUARD_12455133



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XPath/XNodeSetNodeProxy.hpp>



#include <xalanc/PlatformSupport/ReusableArenaAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XNodeSetNodeProxyAllocator
{
public:

	typedef XNodeSetNodeProxy						nodeset_type;

	typedef ReusableArenaAllocator<nodeset_type>	ArenaAllocatorType;
	typedef ArenaAllocatorType::size_type			size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XNodeSetNodeProxyAllocator(MemoryManagerType&      theManager, size_type	theBlockCount);

	~XNodeSetNodeProxyAllocator();
	
	/**
	 * Create an object using the allocator.
	 * 
	 * @param value	source node
	 *
	 * @return pointer to instance
	 */
	nodeset_type*
	create(XalanNode*	value);

	/**
	 * Clone an XNodeSet object.
	 * 
	 * @param value	source instance
	 *
	 * @return pointer to a new instance
	 */
	nodeset_type*
	clone(const XNodeSetNodeProxy&	value);

	/**
	 * Delete an XNodeSet object from allocator.	 
	 */
	bool
	destroy(nodeset_type*	theNodeSet);
	
	/**
	 * Determine if an object is owned by the allocator...
	 */
	bool
	ownsObject(const nodeset_type*	theObject)
	{
		return m_allocator.ownsObject(theObject);
	}

	/**
	 * Delete all XNodeSet objects from allocator.	 
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
	XNodeSetNodeProxyAllocator(const XNodeSetNodeProxyAllocator&);

	XNodeSetNodeProxyAllocator&
	operator=(const XNodeSetNodeProxyAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XNODESETNODEPROXYALLOCATOR_INCLUDE_GUARD_12455133
