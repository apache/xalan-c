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

#if !defined(XALANSOURCETREEDOCUMENTALLOCATOR_INCLUDE_GUARD_12455133)
#define XALANSOURCETREEDOCUMENTALLOCATOR_INCLUDE_GUARD_12455133



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/PlatformSupport/ReusableArenaAllocator.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp>



XALAN_CPP_NAMESPACE_BEGIN



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
	XalanSourceTreeDocumentAllocator(MemoryManagerType&  theManager, size_type	theBlockCount);

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



XALAN_CPP_NAMESPACE_END



#endif	// XALANSOURCETREEDOCUMENTALLOCATOR_INCLUDE_GUARD_12455133
