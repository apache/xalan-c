/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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

// Class header file...
#include "XalanDOMStringPool.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString	XalanDOMStringPool::s_emptyString;



XalanDOMStringPool::XalanDOMStringPool(
			block_size_type		theBlockSize,
			bucket_count_type	theBucketCount,
			bucket_size_type	theBucketSize) :
	m_stringAllocator(theBlockSize),
	m_stringCount(0),
	m_hashTable(theBucketCount, theBucketSize)
{
}



XalanDOMStringPool::~XalanDOMStringPool()
{
}



void
XalanDOMStringPool::clear()
{
	m_stringAllocator.reset();

	m_hashTable.clear();

	m_stringCount = 0;
}



size_t
XalanDOMStringPool::size() const
{
	assert(m_stringCount == m_hashTable.size());

	return m_stringCount;
}



const XalanDOMString&
XalanDOMStringPool::get(const XalanDOMString&	theString)
{
	return get(toCharArray(theString), length(theString));
}



const XalanDOMString&
XalanDOMStringPool::get(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength)
{
	assert(m_stringCount == m_hashTable.size());

	if (theString == 0 || *theString == 0)
	{
		return s_emptyString;
	}
	else
	{
		const XalanDOMString::size_type		theActualLength = theLength == XalanDOMString::npos ? length(theString) : theLength;

		size_t	theBucketIndex;

		const XalanDOMString*	theTableString = m_hashTable.find(theString, theActualLength, &theBucketIndex);

		if (theTableString != 0)
		{
			return *theTableString;
		}
		else
		{
			// Not found, so insert the string...
			XalanDOMString* const	theNewString =
				m_stringAllocator.create(theString, theActualLength);
			assert(theNewString != 0);

			assert(theActualLength == length(*theNewString));

			++m_stringCount;

			// Insert the string into the hash table...
			m_hashTable.insert(*theNewString, theBucketIndex);

			assert(m_stringCount == m_hashTable.size());

			return *theNewString;
		}
	}
}



XALAN_CPP_NAMESPACE_END
