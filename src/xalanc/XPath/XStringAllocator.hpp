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

#if !defined(XSTRINGALLOCATOR_INCLUDE_GUARD_1357924680)
#define XSTRINGALLOCATOR_INCLUDE_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XPath/XString.hpp>



#include <xalanc/PlatformSupport/ReusableArenaAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XStringAllocator
{
public:

	typedef XString			string_type;

	typedef ReusableArenaAllocator<string_type>		ArenaAllocatorType;
	typedef ArenaAllocatorType::size_type			size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XStringAllocator(MemoryManagerType&  theManager, size_type	theBlockCount);

	~XStringAllocator();

	/**
	 * Create an XString object from a string.
	 * 
	 * @param theString     source string
	 *
	 * @return a pointer to string
	 */
	string_type*
	createString(const XalanDOMString&	theString);

	/**
	 * Create an XString object from a null-terminated array of characters.
	 * 
	 * @param theString     a pointer to the array
	 *
	 * @return a pointer to string
	 */
	string_type*
	createString(const XalanDOMChar*	theString);

	/**
	 * Create an XString object from an array of characters.
	 * 
	 * @param theString     a pointer to the array
	 * @param theLength     the length of the array.
	 *
	 * @return a pointer to string
	 */
	string_type*
	createString(
			const XalanDOMChar*		theString,
			unsigned int			theLength);

	/**
	 * Delete an XString object from allocator.	 
	 */
	bool
	destroy(string_type*	theString);

	/**
	 * Determine if an object is owned by the allocator...
	 */
	bool
	ownsObject(const string_type*	theObject)
	{
		return m_allocator.ownsObject(theObject);
	}

	/**
	 * Delete all XString objects from allocator.	 
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
	XStringAllocator(const XStringAllocator&);

	XStringAllocator&
	operator=(const XStringAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XSTRINGALLOCATOR_INCLUDE_GUARD_1357924680
