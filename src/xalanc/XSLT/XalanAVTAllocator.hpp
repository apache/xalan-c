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

#if !defined(XALANAVTALLOCATOR_INCLUDE_GUARD_12455133)
#define XALANAVTALLOCATOR_INCLUDE_GUARD_12455133



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XSLT/AVT.hpp>



#include <xalanc/PlatformSupport/ArenaAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XSLT_EXPORT XalanAVTAllocator
{
public:

	typedef AVT									data_type;

#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
	typedef ArenaBlock<data_type>				ArenaBlockType;
	typedef ArenaAllocator<data_type,
						   ArenaBlockType>		ArenaAllocatorType;
#else
	typedef ArenaAllocator<data_type>			ArenaAllocatorType;
#endif

	typedef ArenaAllocatorType::size_type		size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XalanAVTAllocator(MemoryManagerType&  theManager, size_type		theBlockCount);

	~XalanAVTAllocator();
	
	/**
	 * Create an instance.
	 *
	 * @param constructionContext context for construction of AVT
	 * @param locator the Locator for the AVT.  May be null.
	 * @param name name of AVT
	 * @param stringedValue string value to parse
	 * @param resolvervresolver for namespace resolution
	 *
	 * @return A pointer to the new instance.
	 */
	data_type*
	create(
			StylesheetConstructionContext&	constructionContext,
			const LocatorType*				locator,
			const XalanDOMChar*				name,
			const XalanDOMChar*				stringedValue,
			const PrefixResolver&			resolver);

	/**
	 * Determine if an object is owned by the allocator...
	 */
	bool
	ownsObject(const data_type*		theObject)
	{
		return m_allocator.ownsObject(theObject);
	}

	/**
	 * Delete all objects from the allocator.	 
	 */	
	void
	reset()
	{
		m_allocator.reset();
	}

	/**
	 * Get the number of ArenaBlocks currently allocated.
	 *
	 * @return The number of blocks.
	 */
	size_type
	getBlockCount() const
	{
		return m_allocator.getBlockCount();
	}

	/**
	 * Get size of an ArenaBlock, that is, the number
	 * of objects in each block.
	 *
	 * @return The size of the block
	 */
	size_type
	getBlockSize() const
	{
		return m_allocator.getBlockSize();
	}

private:

	// Not implemented...
	XalanAVTAllocator(const XalanAVTAllocator&);

	XalanAVTAllocator&
	operator=(const XalanAVTAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANAVTALLOCATOR_INCLUDE_GUARD_12455133
