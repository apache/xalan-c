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

// Class header file...
#include "XalanDOMStringPool.hpp"



const XalanDOMString	XalanDOMStringPool::s_emptyString;



bool
XalanDOMStringPool::StringKey::operator<(const StringKey&	theRHS) const
{
	// Note that we don't really need lexicographical ordering, so this
	// is much cheaper.
	if (m_length < theRHS.m_length)
	{
		return true;
	}
	else if (m_length > theRHS.m_length)
	{
		return false;
	}
	else
	{
		unsigned int	i = 0;

		while(i < m_length)
		{
			const XalanDOMChar	theLHSChar = m_string[i];
			const XalanDOMChar	theRHSChar = theRHS.m_string[i];

			if (theLHSChar < theRHSChar)
			{
				return true;
			}
			else if (theLHSChar > theRHSChar)
			{
				return false;
			}
			else
			{
				++i;
			}
		}

		// They're equal, so return false...
		return false;
	}
}



XalanDOMStringPool::XalanDOMStringPool() :
	m_strings(),
	m_index()
{
}



XalanDOMStringPool::~XalanDOMStringPool()
{
}



void
XalanDOMStringPool::clear()
{
	// Clear by swapping things, which is
	// guaranteed to free up all allocated memory.
	XalanDOMStringCollectionType().swap(m_strings);

	IndexMapType().swap(m_index);
}



XalanDOMStringPool::size_type
XalanDOMStringPool::size() const
{
	assert(m_strings.size() == m_index.size());

	return m_strings.size();
}



const XalanDOMString&
XalanDOMStringPool::get(const XalanDOMString&	theString)
{
	return get(toCharArray(theString), length(theString));
}



const XalanDOMString&
XalanDOMStringPool::get(
			const XalanDOMChar*		theString,
			unsigned int			theLength)
{
	assert(m_strings.size() == m_index.size());

	if (theString == 0 || *theString == 0)
	{
		return s_emptyString;
	}
	else
	{
		const unsigned int	theActualLength = theLength == -1 ? length(theString) : theLength;

		// Find the string...
		const IndexMapType::const_iterator	i =
			m_index.find(IndexMapType::key_type(theString, theActualLength));

		if (i != m_index.end())
		{
			// Already there, so return it...
			return *(*i).second;
		}
		else
		{
			// Not found, so insert the string...
			const XalanDOMStringCollectionType::iterator	theIterator =
				m_strings.insert(m_strings.end(), XalanDOMString());

			XalanDOMString&		theNewString = *theIterator;

			assign(theNewString, theString, theActualLength);

			assert(theActualLength == length(theNewString));

			// Add an index entry...
			m_index.insert(
				IndexMapType::value_type(
					IndexMapType::key_type(toCharArray(theNewString), theActualLength),
					&theNewString));

			assert(m_strings.size() == m_index.size());

			return theNewString;
		}
	}
}
