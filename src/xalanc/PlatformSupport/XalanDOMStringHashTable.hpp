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
#if !defined(XALANDOMSTRINGHASHTABLE_HEADER_GUARD_1357924680)
#define XALANDOMSTRINGHASHTABLE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>


#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>




#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT XalanDOMStringHashTable
{
public:

	typedef XalanVector<const XalanDOMString*>	BucketType;
	typedef BucketType::size_type				bucket_size_type;
	typedef XalanVector<bucket_size_type>		BucketCountsType;

	enum { eDefaultBucketCount = 101, eDefaultBucketSize = 15 };


	/**
	 * Create a hash table.
	 *
	 * @param theBucketCount The number of buckets to use for the hash table.  This should be a prime number for best results.
	 * @param theBucketSize The initial size of each bucket in the hash table.
	 */
	explicit
	XalanDOMStringHashTable(
            MemoryManagerType&  theManager,
			size_t				theBucketCount = eDefaultBucketCount,
			bucket_size_type	theBucketSize = eDefaultBucketSize);

	~XalanDOMStringHashTable() { }

	/**
	 * Clear the hash table.
	 */
	void
	clear();

	/**
	 * Get the number of strings in the table
	 *
	 * @return The number of strings in the table
	 */
	size_t
	size() const
	{
		return m_count;
	}

	/**
	 * Get the number of buckets in the table
	 *
	 * @return The number of buckets in the table
	 */
	size_t
	bucketCount() const
	{
		return m_bucketCount;
	}

	/**
	 * Get the size of each of the buckets in the table
	 *
	 * @param A vector to return the bucket counts.
	 */
	void
	getBucketCounts(BucketCountsType&	theVector) const;

	/**
	 * Get the collision count.  Release builds will always
	 * return 0.
	 *
	 * @return The number of collisions.  Valid only for non-release builds.
	 */
	size_t
	collisions() const
	{
		return m_collisions;
	}

	/**
	 * Find a string.  If the string is not found, return null.
	 *
	 * @param theString The string to find.
	 * @param theBucketIndex The index of the bucket for the string.
	 * @return a pointer to the string, or null if not found.
	 */
	const XalanDOMString*
	find(
			const XalanDOMString&	theString,
			size_t*					theBucketIndex = 0) const;

	/**
	 * Find a string.  If the string is not found, return null.
	 * If theBucketIndex is not null, the variable pointed to
	 * will be updated with the bucket index that was calculated
	 * for the string.  This index can be used in a later call to
	 * insert() to avoid recalculating the index.
	 *
	 * @param theString The string to find.
	 * @param theLength The number of characters in the string.
	 * @param theBucketIndex A pointer to an unsigned int to get the bucket index
	 * @return a pointer to the string, or null if not found.
	 */
	const XalanDOMString*
	find(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos,
			size_t*						theBucketIndex = 0) const;

	/**
	 * Insert a pointer to a string into the table.  If the string
	 * is already present, the string will still be added, but it
	 * will never be found, since it will be placed after the identical
	 * string.
	 *
	 * Note that this class only stores a _pointer_ to a XalanDOMString.
	 * It's expected that the string will be allocated and managed outside
	 * of the hash table.
	 *
	 * @param theString The string to insert.
	 */
	void
	insert(const XalanDOMString&	theString);

	/**
	 * Insert a pointer to a string into the table.  If the string
	 * is already present, the string will still be added, but it
	 * will never be found, since it will be placed after the identical
	 * string.  theBucketIndex _must_ be the index returned from a
	 * previous call to find.  If not, then the behavior is undefined.
	 * This is meant as an optimization to avoid re-hashing the string.
	 *
	 * Note that this class only stores a _pointer_ to a XalanDOMString.
	 * It's expected that the string will be allocated and managed outside
	 * of the hash table.
	 *
	 * @param theString The string to insert.
	 * @param theBucketIndex The index of the bucket for the string.
	 */
	void
	insert(
			const XalanDOMString&	theString,
			size_t					theBucketIndex);

#if defined(XALAN_NEEDS_EXPLICIT_TEMPLATE_INSTANTIATION)
	struct
	equalsXalanDOMString
	{
		equalsXalanDOMString(
				const XalanDOMChar*			theString,
				XalanDOMString::size_type	theLength) :
			m_string(theString),
			m_length(theLength)
		{
		}

		bool
		operator()(const XalanDOMString*	theString) const;

	private:

		const XalanDOMChar* const			m_string;

		const XalanDOMString::size_type		m_length;
	};
#endif

    MemoryManagerType&
    getMemoryManager()
    {
        assert(m_buckets.getMemoryManager() != 0);

        return *m_buckets.getMemoryManager();
    }

    const MemoryManagerType&
    getMemoryManager() const
    {
        assert(m_buckets.getMemoryManager() != 0);

        return *m_buckets.getMemoryManager();
    }

private:

	// Not implemented, for now...
	XalanDOMStringHashTable(const XalanDOMStringHashTable&);

	XalanDOMStringHashTable&
	operator=(const XalanDOMStringHashTable&);

	bool
	operator==(const XalanDOMStringHashTable&) const;


	// Data members...
	const size_t					m_bucketCount;

	const bucket_size_type			m_bucketSize;

	XalanMemMgrAutoPtrArray<BucketType>	m_buckets;

	size_t							m_count;

	unsigned int					m_collisions;		
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANDOMSTRINGPOOL_HEADER_GUARD_1357924680)
