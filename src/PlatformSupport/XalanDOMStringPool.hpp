/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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
#if !defined(XALANDOMSTRINGPOOL_HEADER_GUARD_1357924680)
#define XALANDOMSTRINGPOOL_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <deque>
#include <map>



#include <PlatformSupport/ArenaAllocator.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanDOMStringAllocator.hpp>
#include <PlatformSupport/XalanDOMStringHashTable.hpp>



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



#endif	// !defined(XALANDOMSTRINGPOOL_HEADER_GUARD_1357924680)
