/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights 
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
#if !defined(XALANDOMSTRINGHASHTABLE_HEADER_GUARD_1357924680)
#define XALANDOMSTRINGHASHTABLE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <vector>



#include <Include/XalanAutoPtr.hpp>



#include <XalanDOM/XalanDOMString.hpp>



class XALAN_PLATFORMSUPPORT_EXPORT XalanDOMStringHashTable
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<const XalanDOMString*>	BucketType;
	typedef vector<unsigned int>			BucketCountsType;
#else
	typedef std::vector<const XalanDOMString*>	BucketType;
	typedef std::vector<unsigned int>			BucketCountsType;
#endif

	enum { eDefaultBucketCount = 101, eDefaultBucketSize = 15 };


	/**
	 * Create a hash table.
	 *
	 * @param theBucketCount The number of buckets to use for the hash table.  This should be a prime number for best results.
	 * @param theBucketSize The initial size of each bucket in the hash table.
	 */
	explicit
	XalanDOMStringHashTable(
			unsigned int	theBucketCount = eDefaultBucketCount,
			unsigned int	theBucketSize = eDefaultBucketSize);

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
	unsigned int
	size() const
	{
		return m_count;
	}

	/**
	 * Get the number of buckets in the table
	 *
	 * @return The number of buckets in the table
	 */
	unsigned int
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
	unsigned int
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
			unsigned int*			theBucketIndex = 0) const;

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
			const XalanDOMChar*		theString,
			unsigned int			theLength = unsigned(-1),
			unsigned int*			theBucketIndex = 0) const;

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
			unsigned int			theBucketIndex);

private:

	// Not implemented, for now...
	XalanDOMStringHashTable(const XalanDOMStringHashTable&);

	XalanDOMStringHashTable&
	operator=(const XalanDOMStringHashTable&);

	bool
	operator==(const XalanDOMStringHashTable&) const;


	// Data members...
	const unsigned int				m_bucketCount;

	const unsigned int				m_bucketSize;

	XalanArrayAutoPtr<BucketType>	m_buckets;

	unsigned int					m_count;

	unsigned int					m_collisions;		
};



#endif	// !defined(XALANDOMSTRINGPOOL_HEADER_GUARD_1357924680)
