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

// Class header file.
#include "XalanDOMStringCache.hpp"



#include <Include/STLHelper.hpp>



#include "DOMStringHelper.hpp"



XalanDOMStringCache::XalanDOMStringCache(unsigned int	theMaximumSize) :
	m_availableList(),
	m_busyList(),
	m_maximumSize(theMaximumSize)
{
}



XalanDOMStringCache::~XalanDOMStringCache()
{
	clear();
}



XalanDOMString&
XalanDOMStringCache::get()
{
	if (m_availableList.empty() == true)
	{
		m_busyList.push_back(new XalanDOMString());
	}
	else
	{
		m_busyList.push_back(m_availableList.back());

		m_availableList.pop_back();
	}

	return *m_busyList.back();
}



bool
XalanDOMStringCache::release(XalanDOMString&	theString)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::find;
#endif

	StringListType::iterator	i =
		find(m_busyList.begin(),
			 m_busyList.end(),
			 &theString);

	if (i == m_busyList.end())
	{
		return false;
	}
	else
	{
		if (m_availableList.size() > m_maximumSize)
		{
			delete *i;
		}
		else
		{
			::clear(theString);

			m_availableList.push_back(*i);
		}

		m_busyList.erase(i);

		return true;
	}
}



void
XalanDOMStringCache::clear()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(m_busyList.begin(),
			 m_busyList.end(),
			 DeleteFunctor<XalanDOMString>());

	m_busyList.clear();

	for_each(m_availableList.begin(),
			 m_availableList.end(),
			 DeleteFunctor<XalanDOMString>());

	m_availableList.clear();
}



void
XalanDOMStringCache::reset()
{
	const StringListType::size_type		theSize =
		m_availableList.size();

	while(m_busyList.empty() == false)
	{
		assert(m_busyList.back() != 0);

		if (theSize > m_maximumSize)
		{
			delete m_busyList.back();
		}
		else
		{
			::clear(*m_busyList.back());

			m_availableList.push_back(m_busyList.back());
		}

		m_busyList.pop_back();
	}
}
