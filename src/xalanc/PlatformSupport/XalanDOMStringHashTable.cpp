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

// Class header file...
#include "XalanDOMStringHashTable.hpp"



#include <algorithm>



#include "DOMStringHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanDOMStringHashTable::XalanDOMStringHashTable(
			size_t		theBucketCount,
			size_t		theBucketSize) :

	m_bucketCount(theBucketCount),
	m_bucketSize(theBucketSize),
	m_buckets(new BucketType[theBucketCount]),
	m_count(0),
	m_collisions(0)
{
}



void
XalanDOMStringHashTable::clear()
{
	for(size_t i = 0; i < m_bucketCount; ++i)
	{
		m_buckets[i].clear();
	}

#if !defined(NDEBUG)
	m_collisions = 0;
#endif
	m_count = 0;
}



void
XalanDOMStringHashTable::getBucketCounts(BucketCountsType&	theVector) const
{
	for(size_t i = 0; i < m_bucketCount; ++i)
	{
		const bucket_size_type	size = m_buckets[i].size();

		theVector.push_back(size);
	}
}



#if defined(XALAN_NEEDS_EXPLICIT_TEMPLATE_INSTANTIATION)
bool
XalanDOMStringHashTable::equalsXalanDOMString::operator()(const XalanDOMString*		theString) const
{
	if (m_length != length(*theString))
	{
		return false;
	}
	else
	{
		return equals(m_string, c_wstr(*theString), m_length);
	}
}
#else
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
	operator()(const XalanDOMString*	theString) const
	{
		if (m_length != length(*theString))
		{
			return false;
		}
		else
		{
			return equals(m_string, c_wstr(*theString), m_length);
		}
	}

private:

	const XalanDOMChar* const			m_string;

	const XalanDOMString::size_type		m_length;
};
#endif



const XalanDOMString*
XalanDOMStringHashTable::find(
			const XalanDOMString&	theString,
			size_t*					theBucketIndex) const
{
	return find(c_wstr(theString), length(theString), theBucketIndex);
}



const XalanDOMString*
XalanDOMStringHashTable::find(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength,
			size_t*						theBucketIndex) const
{
	assert(theString != 0);

	const XalanDOMString::size_type		theActualLength =
		theLength == XalanDOMString::npos ? length(theString) : theLength;

	const XalanDOMString::size_type		theHash = XalanDOMString::hash(theString, theActualLength);

	const size_t	theLocalBucketIndex = theHash % m_bucketCount;

	assert(theLocalBucketIndex < m_bucketCount);

	const BucketType&	theBucket = m_buckets[theLocalBucketIndex];

	if (theBucketIndex != 0)
	{
		*theBucketIndex = theLocalBucketIndex;
	}

	XALAN_USING_STD(find_if)

	const BucketType::const_iterator	i =
		find_if(
				theBucket.begin(),
				theBucket.end(),
				equalsXalanDOMString(theString, theActualLength));

	if (i == theBucket.end())
	{
		return 0;
	}
	else
	{
		return *i;
	}
}



void
XalanDOMStringHashTable::insert(const XalanDOMString&	theString)
{
	const XalanDOMString::size_type		theHash = theString.hash();

	const size_t	theBucketIndex = theHash % m_bucketCount;

	assert(theBucketIndex < m_bucketCount);

	BucketType&	theBucket = m_buckets[theBucketIndex];

#if !defined(NDEBUG)
	if (theBucket.empty() == false)
	{
		++m_collisions;
	}
#endif

	theBucket.reserve(m_bucketSize);

	theBucket.push_back(&theString);

	++m_count;
}



void
XalanDOMStringHashTable::insert(
			const XalanDOMString&	theString,
			size_t					theBucketIndex)
{
	assert(theBucketIndex == theString.hash() % m_bucketCount);
	assert(theBucketIndex < m_bucketCount);

	BucketType&		theBucket = m_buckets[theBucketIndex];

#if !defined(NDEBUG)
	if (theBucket.empty() == false)
	{
		++m_collisions;
	}
#endif

	theBucket.reserve(m_bucketSize);

	theBucket.push_back(&theString);

	++m_count;
}



XALAN_CPP_NAMESPACE_END
