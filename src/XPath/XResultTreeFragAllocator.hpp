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

#if !defined(XRESULTTREEFRAGALLOCATOR_INCLUDE_GUARD_12455133)
#define XRESULTTREEFRAGALLOCATOR_INCLUDE_GUARD_12455133



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <XPath/XResultTreeFrag.hpp>



#include <PlatformSupport/ReusableArenaAllocator.hpp>



class XALAN_XPATH_EXPORT XResultTreeFragAllocator
{
public:

	typedef XResultTreeFrag						data_type;

	typedef ReusableArenaAllocator<data_type>	ArenaAllocatorType;
	typedef ArenaAllocatorType::size_type		size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XResultTreeFragAllocator(size_type	theBlockCount);

	~XResultTreeFragAllocator();
	
	/**
	 * Create an XResultTreeFrag object using allocator from a string.
	 * 
	 * @param value			source NodeRefListBase
	 *
	 * @return pointer to a node
	 */
	data_type*
	create(ResultTreeFragBase*	value);

	/**
	 * Create an XResultTreeFrag object using allocator from a string.
	 * 
	 * @param theEnvSupport XPath environment support class instance
	 * @param theSupport    XPath support class instance
	 * @param value			source MutableNodeRefList
	 *
	 * @return pointer to a node
	 */
	data_type*
	create(const XResultTreeFrag&	value);

	/**
	 * Clone an XResultTreeFrag object.
	 * 
	 * @param value			source XResultTreeFrag
	 *
	 * @return pointer to an XResultTreeFrag
	 */
	data_type*
	clone(const XResultTreeFrag&	value);

	/**
	 * Delete an XResultTreeFrag object from allocator.	 
	 */
	bool
	destroy(data_type*	theObject);

	/**
	 * Determine if an object is owned by the allocator...
	 */
	bool
	ownsObject(const data_type*		theObject)
	{
		return m_allocator.ownsObject(theObject);
	}

	/**
	 * Delete all XResultTreeFrag objects from allocator.	 
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
	XResultTreeFragAllocator(const XResultTreeFragAllocator&);

	XResultTreeFragAllocator&
	operator=(const XResultTreeFragAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



#endif	// XRESULTTREEFRAGALLOCATOR_INCLUDE_GUARD_12455133
