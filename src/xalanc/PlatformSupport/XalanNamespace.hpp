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
#if !defined(XALANNAMESPACE_HEADER_GUARD_1357924680)
#define XALANNAMESPACE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNamespace
{
public:

	explicit
	XalanNamespace(MemoryManagerType&      theManager) :
		m_prefix(theManager),
		m_uri(theManager)
	{
	}

	/**
	 * Construct a namespace for placement on the 
	 * result tree namespace stack.
	 *
	 * @param thePrefix namespace thePrefix
	 * @param theURI    URI of namespace
	 */
	XalanNamespace(
			const XalanDOMString&	thePrefix,
			const XalanDOMString&	theURI,
            MemoryManagerType&      theManager) :
		m_prefix(thePrefix,theManager),
		m_uri(theURI, theManager)
	{
	}

	XalanNamespace(
            const XalanNamespace&   other,
            MemoryManagerType&      theManager) :
		m_prefix(other.m_prefix,theManager),
		m_uri(other.m_uri, theManager)
	{
	}

	~XalanNamespace()
	{
	}

	/**
	 * Retrieve the thePrefix for namespace
	 * 
	 * @return thePrefix string
	 */
	const XalanDOMString&
	getPrefix() const
	{
		return m_prefix;
	}

	/**
	 * Set the thePrefix for namespace
	 * 
	 * @param thePrefix The new thePrefix value
	 */
	void
	setPrefix(const XalanDOMString&		thePrefix)
	{
		m_prefix = thePrefix;
	}

	/**
	 * Retrieve the URI for namespace
	 * 
	 * @return URI string
	 */
	const XalanDOMString&
	getURI() const
	{
		return m_uri;
	}

	/**
	 * Set the URI for namespace
	 * 
	 * @param theURI The new theURI value
	 */
	void
	setURI(const XalanDOMString&	theURI)
	{
		m_uri = theURI;
	}

	/**
	 * Set the URI for namespace
	 * 
	 * @param theURI The new theURI value
	 */
	void
	setURI(const XalanDOMChar*	theURI)
	{
		assert(theURI != 0);

		m_uri = theURI;
	}

	/**
	 * Set the URI for namespace
	 * 
	 * @param theURI The new theURI value
	 * @param len The length of the theURI
	 */
	void
	setURI(
			const XalanDOMChar*			theURI,
			XalanDOMString::size_type	len)
	{
		assert(theURI != 0);

		m_uri.assign(theURI, len);
	}

	bool
	empty() const
	{
		return m_prefix.empty() && m_uri.empty();
	}

	void
	clear()
	{
		m_prefix.clear();

		m_uri.clear();
	}

	/**
	 * Equality operator
	 *
	 * @param theRHS namespace to compare
	 */
	bool
	operator==(const XalanNamespace&	theRHS) const
	{
		return equals(m_prefix, theRHS.m_prefix) &&
			   equals(m_uri, theRHS.m_uri);
	}	

private:
    //Not implemented
    XalanNamespace();
    XalanNamespace(const XalanNamespace&);

	XalanDOMString	m_prefix;

	XalanDOMString	m_uri;
};

XALAN_USES_MEMORY_MANAGER(XalanNamespace)

XALAN_CPP_NAMESPACE_END



#endif	// ALANNAMESPACE_HEADER_GUARD_1357924680
