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

// Class header file.
#include "XalanDOMStringCache.hpp"



#include <xalanc/Include/STLHelper.hpp>



#include "DOMStringHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



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
	XALAN_USING_STD(find)

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
			theString.erase();

			m_availableList.push_back(*i);
		}

		m_busyList.erase(i);

		return true;
	}
}



void
XalanDOMStringCache::clear()
{
	XALAN_USING_STD(for_each)

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
			m_busyList.back()->clear();

			m_availableList.push_back(m_busyList.back());
		}

		m_busyList.pop_back();
	}
}



XALAN_CPP_NAMESPACE_END
