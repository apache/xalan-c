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

#if !defined(XALANELEMTEXTALLOCATOR_INCLUDE_GUARD_12455133)
#define XALANELEMTEXTALLOCATOR_INCLUDE_GUARD_12455133



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XSLT/ElemText.hpp>



#include <xalanc/PlatformSupport/ReusableArenaAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XSLT_EXPORT XalanElemTextAllocator
{
public:

	typedef ElemText							data_type;

	typedef ReusableArenaAllocator<data_type>	ArenaAllocatorType;

	typedef ArenaAllocatorType::size_type		size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XalanElemTextAllocator(MemoryManagerType&  theManager, size_type		theBlockCount);

	~XalanElemTextAllocator();
	
	/**
	 * Construct an instance
	 * 
	 * @param constructionContext The current construction context
	 * @param stylesheetTree The stylesheet containing element
	 * @param atts The list of attributes for the element
	 * @param lineNumber The line number in the document
	 * @param columnNumber The column number in the document
	 *
	 * @return A pointer to the new instance.
	 */
	data_type*
	create(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber);

	/**
	 * Destroy an instance previously created.
	 *
	 * @param theObject A pointer to the instance to destroy.
	 *
	 * @return true if the instance was destroyed, false if not.
	 */
	bool
	destroy(data_type*	theObject)
	{
		return m_allocator.destroyObject(theObject);
	}

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
	XalanElemTextAllocator(const XalanElemTextAllocator&);

	XalanElemTextAllocator&
	operator=(const XalanElemTextAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANELEMTEXTALLOCATOR_INCLUDE_GUARD_12455133
