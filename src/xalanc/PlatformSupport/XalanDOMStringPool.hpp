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
#if !defined(XALANDOMSTRINGPOOL_HEADER_GUARD_1357924680)
#define XALANDOMSTRINGPOOL_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/PlatformSupport/ArenaAllocator.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanDOMStringAllocator.hpp>
#include <xalanc/PlatformSupport/XalanDOMStringHashTable.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT XalanDOMStringPool
{
public:

	enum { eDefaultBlockSize = 32,
		   eDefaultBucketCount = XalanDOMStringHashTable::eDefaultBucketCount,
		   eDefaultBucketSize = XalanDOMStringHashTable::eDefaultBucketSize };

	typedef XalanDOMStringAllocator			AllocatorType;

	typedef AllocatorType::size_type					block_size_type;
	typedef size_t										bucket_count_type;
	typedef XalanDOMStringHashTable::bucket_size_type	bucket_size_type;

	/**
	 * Create a string pool.
	 *
	 * @param theBlockSize The block size for the allocator.
	 * @param theBucketCount The number of buckets to use for the hash table.  This should be a prime number for best results.
	 * @param theBucketSize The initial size of each bucket in the hash table.
	 */
	explicit
	XalanDOMStringPool(
            MemoryManagerType&  theManager,
			block_size_type		theBlockSize = eDefaultBlockSize,
			bucket_count_type	theBucketCount = eDefaultBucketCount,
			bucket_size_type	theBucketSize = eDefaultBucketSize);

    static XalanDOMStringPool*
    create(
            MemoryManagerType&  theManager,
			block_size_type		theBlockSize = eDefaultBlockSize,
			bucket_count_type	theBucketCount = eDefaultBucketCount,
			bucket_size_type	theBucketSize = eDefaultBucketSize);


	virtual
	~XalanDOMStringPool();

	/**
	 * Clear the pool.
	 *
	 */
	virtual void
	clear();

	/**
	 * Get the number of strings in the pool
	 *
	 * @return the size of the pool.
	 */
	virtual size_t
	size() const;

	/**
	 * Get a pooled string.  If the string is not pooled, it is added.
	 *
	 * @param theString The string to pool.
	 * @return a const reference to the pooled string.
	 */
	virtual const XalanDOMString&
	get(const XalanDOMString&	theString);

	/**
	 * Get a pooled string.  If the string is not pooled, it is added.
	 *
	 * @param theString The string to pool.
	 * @param theLength The length of the string.  If XalanDOMString::npos, the string is assumed to be null-terminated.
	 * @return a const reference to the pooled string.
	 */
	virtual const XalanDOMString&
	get(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

	/**
	 * Get a reference to the pool's hash table.  Useful for diagnostic
	 * purposes.
	 *
	 * @return a const reference to the hash table.
	 */
	const XalanDOMStringHashTable&
	getHashTable() const
	{
		return m_hashTable;
	}

    MemoryManagerType&
    getMemoryManager()
    {
        return m_hashTable.getMemoryManager();
    }

    const MemoryManagerType&
    getMemoryManager() const
    {
        return m_hashTable.getMemoryManager();
    }

private:

	// Not implemented, for now...
	XalanDOMStringPool(const XalanDOMStringPool&);

	XalanDOMStringPool&
	operator=(const XalanDOMStringPool&);

	bool
	operator==(const XalanDOMStringPool&) const;

	// Data members...
	AllocatorType					m_stringAllocator;

	size_t							m_stringCount;

	XalanDOMStringHashTable			m_hashTable;

	static const XalanDOMString		s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANDOMSTRINGPOOL_HEADER_GUARD_1357924680)
