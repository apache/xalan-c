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
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
#if !defined(XALAN_OBJECTCACHE_HEADER_GUARD)
#define XALAN_OBJECTCACHE_HEADER_GUARD



#include <algorithm>
#include <vector>



#include <Include/STLHelper.hpp>
#include <Include/XalanAutoPtr.hpp>



template<class ObjectType>
class XalanObjectCache
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<ObjectType*>			VectorType;
#else
	typedef std::vector<ObjectType*>	VectorType;
#endif

	explicit
	XalanObjectCache() :
		m_availableList(),
		m_busyList()
	{
	}

	virtual
	~XalanObjectCache()
	{
		reset();

#if !defined(XALAN_NO_NAMESPACES)
		using std::for_each;
#endif

		for_each(
			m_availableList.begin(),
			m_availableList.end(),
			DeleteFunctor<ObjectType>());
	}

	ObjectType*
	get()
	{
		// We'll always return the back of the free list, since
		// that's the cheapest thing.
		if (m_availableList.size() == 0)
		{
			XalanAutoPtr<ObjectType>	theNewObject(create());

			m_busyList.push_back(theNewObject.get());

			return theNewObject.release();
		}
		else
		{
			ObjectType* const	theObject = m_availableList.back();

			m_busyList.push_back(theObject);

			m_availableList.pop_back();

			return theObject;
		}
	}

	bool
	release(ObjectType*		theInstance)
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::find;
#endif

		typedef typename VectorType::iterator	IteratorType;

		const IteratorType	i =
			find(
				m_busyList.begin(),
				m_busyList.end(),
				theInstance);

		if (i == m_busyList.end())
		{
			return false;
		}
		else
		{
			m_availableList.push_back(theInstance);

			m_busyList.erase(i);

			return true;
		}
	}

	virtual void
	reset()
	{
		while (m_busyList.size() != 0)
		{
			ObjectType* const	theInstance = m_busyList.back();

			resetInstance(theInstance);

			m_availableList.push_back(theInstance);

			m_busyList.pop_back();
		}
	}

protected:

	virtual ObjectType*
	create() = 0;

	virtual void
	resetInstance(ObjectType*	/* theInstance */)
	{
	}

private:

	// There are not defined...
	XalanObjectCache(const XalanObjectCache<ObjectType>&	theRHS);

	XalanObjectCache<ObjectType>&
	operator=(const XalanObjectCache<ObjectType>&	theRHS);


	// Data members
	VectorType	m_availableList;

	VectorType	m_busyList;
};


template<class ObjectType>
class GetReleaseCachedObject
{
public:

	typedef XalanObjectCache<ObjectType>	CacheType;

	GetReleaseCachedObject(CacheType&	theCache) :
		m_cache(theCache),
		m_cachedObject(theCache.get())
	{
	}

	~GetReleaseCachedObject()
	{
		m_cache.release(m_cachedObject);
	}

	ObjectType*
	get() const
	{
		return m_cachedObject;
	}

private:

	// Not implemented...
	GetReleaseCachedObject(const GetReleaseCachedObject<ObjectType>&);


	// Data members...
	CacheType&			m_cache;

	ObjectType* const	m_cachedObject;
};


template<class ObjectType>
class XalanObjectCacheDefault : public XalanObjectCache<ObjectType>
{
public:

protected:

	virtual ObjectType*
	create()
	{
		return new ObjectType;
	}

private:
};



#endif
