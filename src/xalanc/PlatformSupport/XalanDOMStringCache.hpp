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
#if !defined(XALAN_DOMSTRINGCACHE_HEADER_GUARD)
#define XALAN_DOMSTRINGCACHE_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/XalanDOMStringReusableAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT XalanDOMStringCache
{
public:

	enum { eDefaultMaximumSize = 100 };

	typedef XalanVector<XalanDOMString*>	StringListType;

	explicit
	XalanDOMStringCache(MemoryManagerType&     theManager,
                        unsigned int	theMaximumSize = eDefaultMaximumSize);

	~XalanDOMStringCache();

	unsigned int
	getMaximumSize() const
	{
		return m_maximumSize;
	}

	void
	setMaximumSize(unsigned int		theSize)
	{
		m_maximumSize = theSize;
	}

	XalanDOMString&
	get();

	bool
	release(XalanDOMString&		theString);

	/*
	 * Clear all of the strings in the cache.  This
	 * destroys all of the strings.
	 *
	 */
	void
	clear();

	/*
	 * Reset the cache so that all strings that are
	 * currently in use are available.
	 */
	void
	reset();

	class GetAndRelease
	{
	public:

		GetAndRelease(XalanDOMStringCache&	theCache) :
			m_cache(theCache),
			m_string(&theCache.get())
		{
		}

		~GetAndRelease()
		{
			m_cache.release(*m_string);
		}

		XalanDOMString&
		get() const
		{
			return *m_string;
		}

	private:

		XalanDOMStringCache&	m_cache;

		XalanDOMString* const	m_string;
	};

private:

	// not implemented
	XalanDOMStringCache(const XalanDOMStringCache&);

	bool
	operator==(const XalanDOMStringCache&) const;

	XalanDOMStringCache&
	operator=(const XalanDOMStringCache&);

	/**
	 * A list to hold the strings that are available...
	 */
	StringListType	                    m_availableList;

	/**
	 * A list to hold the strings that are busy...
	 */
	StringListType	                    m_busyList;

	unsigned int	                    m_maximumSize;

    XalanDOMStringReusableAllocator     m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_RESULTNAMESPACESSTACK_HEADER_GUARD
