/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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

#if !defined(XALANSOURCETREEDOCUMENTALLOCATOR_INCLUDE_GUARD_12455133)
#define XALANSOURCETREEDOCUMENTALLOCATOR_INCLUDE_GUARD_12455133



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <PlatformSupport/ReusableArenaAllocator.hpp>



#include <XalanSourceTree/XalanSourceTreeDocument.hpp>



class XALAN_XSLT_EXPORT XalanSourceTreeDocumentAllocator
{
public:

	typedef XalanSourceTreeDocument				data_type;

	typedef ReusableArenaAllocator<data_type>				ArenaAllocatorType;
	typedef ArenaAllocatorType::size_type					size_type;

	enum { eDefaultAttributeAllocatorBlockSize = XalanSourceTreeDocument::eDefaultAttributeAllocatorBlockSize,
		   eDefaultAttributeNSAllocatorBlockSize = XalanSourceTreeDocument::eDefaultAttributeNSAllocatorBlockSize,
		   eDefaultCommentAllocatorBlockSize = XalanSourceTreeDocument::eDefaultCommentAllocatorBlockSize,
		   eDefaultElementAllocatorBlockSize = XalanSourceTreeDocument::eDefaultElementAllocatorBlockSize,
		   eDefaultElementNSAllocatorBlockSize = XalanSourceTreeDocument::eDefaultElementNSAllocatorBlockSize,
		   eDefaultPIAllocatorBlockSize = XalanSourceTreeDocument::eDefaultPIAllocatorBlockSize,
		   eDefaultTextAllocatorBlockSize = XalanSourceTreeDocument::eDefaultTextAllocatorBlockSize,
		   eDefaultTextIWSAllocatorBlockSize = XalanSourceTreeDocument::eDefaultTextIWSAllocatorBlockSize,
		   eDefaultNamesStringPoolBlockSize = XalanSourceTreeDocument::eDefaultNamesStringPoolBlockSize,
		   eDefaultNamesStringPoolBucketCount = XalanSourceTreeDocument::eDefaultNamesStringPoolBucketCount,
		   eDefaultNamesStringPoolBucketSize = XalanSourceTreeDocument::eDefaultNamesStringPoolBucketSize,
		   eDefaultValuesStringPoolBlockSize = XalanSourceTreeDocument::eDefaultValuesStringPoolBlockSize,
		   eDefaultValuesStringPoolBucketCount = XalanSourceTreeDocument::eDefaultValuesStringPoolBucketCount,
		   eDefaultValuesStringPoolBucketSize = XalanSourceTreeDocument::eDefaultValuesStringPoolBucketSize };

	typedef XalanSourceTreeDocument::block_size_type	block_size_type;
	typedef XalanSourceTreeDocument::bucket_count_type	bucket_count_type;
	typedef XalanSourceTreeDocument::bucket_size_type	bucket_size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XalanSourceTreeDocumentAllocator(size_type	theBlockCount);

	~XalanSourceTreeDocumentAllocator();

	/**
	 * Create a XalanSourceTreeDocument object.
	 * 
	 * @param fPoolAllText If false, text node data that is not whitespace will not be pooled.
	 * @param theNamesStringPoolBlockSize The block size for allocating strings in the name pool
	 * @param theNamesStringPoolBucketCount The number of buckets for allocating strings in the name pool
	 * @param theNamesStringPoolBucketSize The bucket size for allocating strings in the name pool
	 * @param theValuesStringPoolBlockSize The block size for allocating strings in the values pool
	 * @param theValuesStringPoolBucketCount The number of buckets for allocating strings in the values pool
	 * @param theValuesStringPoolBucketSize The bucket size for allocating strings in the values pool
	 *
	 * @return pointer to a node
	 */
	data_type*
	create(
			bool				fPoolAllText = true,
			block_size_type		theNamesStringPoolBlockSize = eDefaultNamesStringPoolBlockSize,
			bucket_count_type	theNamesStringPoolBucketCount = eDefaultNamesStringPoolBucketCount,
			bucket_size_type	theNamesStringPoolBucketSize = eDefaultNamesStringPoolBucketSize,
			block_size_type		theValuesStringPoolBlockSize = eDefaultValuesStringPoolBlockSize,
			bucket_count_type	theValuesStringPoolBucketCount = eDefaultValuesStringPoolBucketCount,
			bucket_size_type	theValuesStringPoolBucketSize = eDefaultValuesStringPoolBucketSize);

	/**
	 * Create a XalanSourceTreeDocument object.
	 * 
	 * @param fPoolAllText If false, text node data that is not whitespace will not be pooled.
	 * @param theAttributeBlockSize The block size for allocating attribute nodes
	 * @param theAttributeNSBlockSize The block size for allocating attribute NS nodes
	 * @param theCommentBlockSize The block size for allocating comment nodes
	 * @param theElementBlockSize The block size for allocating element nodes
	 * @param theElementNSBlockSize The block size for allocating element nodes
	 * @param theTextBlockSize The block size for allocating text nodes,
	 * @param theTextIWSBlockSize The block size for allocating text IWS nodes,
	 *
	 * @return pointer to a node
	 */
	data_type*
	create(
			size_type	theAttributeBlockSize,
			size_type	theAttributeNSBlockSize,
			size_type	theCommentBlockSize,
			size_type	theElementBlockSize,
			size_type	theElementNSBlockSize,
			size_type	thePIBlockSize,
			size_type	theTextBlockSize,
			size_type	theTextIWSBlockSize,
			bool		fPoolAllText = true);

	/**
	 * Delete a XalanSourceTreeDocument object from allocator.	 
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
	 * Delete all XalanSourceTreeDocument objects from allocator.	 
	 */	
	void
	reset();

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
	XalanSourceTreeDocumentAllocator(const XalanSourceTreeDocumentAllocator&);

	XalanSourceTreeDocumentAllocator&
	operator=(const XalanSourceTreeDocumentAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



#endif	// XALANSOURCETREEDOCUMENTALLOCATOR_INCLUDE_GUARD_12455133
