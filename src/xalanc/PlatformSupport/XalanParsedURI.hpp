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

#if !defined(XALANPARSEDURI_HEADER_GUARD_1357924680)
#define XALANPARSEDURI_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>




XALAN_CPP_NAMESPACE_BEGIN



/**
 * URI handling (hopefully) according to RFC2396.
 */
class XALAN_PLATFORMSUPPORT_EXPORT XalanParsedURI
{
public:

	// Flags to say if a component is defined.  Note that each component may
	// be defined but empty, except for the path.
#if defined(XALAN_INLINE_INITIALIZATION)
	static const int d_scheme		= 1;
	static const int d_authority	= 2;
	static const int d_query		= 4;
	static const int d_fragment		= 8;
#else
	enum eComponent
	{
		d_scheme	= 1,
		d_authority	= 2,
		d_query		= 4,
		d_fragment	= 8
	};
#endif

	/** 
	 * Default constructor 
	 */
	XalanParsedURI(MemoryManagerType&      theManager) :
        m_scheme(theManager),
        m_authority(theManager),
        m_path(theManager),
        m_query(theManager),
        m_fragment(theManager),
		m_defined(0)
	{
	}

	/**
	 * Constructor which parses the passed in uri
	 *
	 * @param uriString		URI to parse
	 * @param uriStringLen	Length of the URI string
	 */
	XalanParsedURI(
		const XalanDOMChar*			uriString,
		XalanDOMString::size_type	uriStringLen,
        MemoryManagerType&          theManager) : 
        m_scheme(theManager),
        m_authority(theManager),
        m_path(theManager),
        m_query(theManager),
        m_fragment(theManager),
		m_defined(0)
	{
		parse(uriString, uriStringLen);
	}

	/**
	 * Constructor which parses the passed in uri
	 *
	 * @param uriString		URI to parse
	 */
	XalanParsedURI(
		const XalanDOMString		&uriString,
        MemoryManagerType&          theManager) :
        m_scheme(theManager),
        m_authority(theManager),
        m_path(theManager),
        m_query(theManager),
        m_fragment(theManager),
		m_defined(0)
	{
		parse(uriString.c_str(), uriString.length());
	}

    MemoryManagerType&
    getMemoryManager()
    {
        return m_scheme.getMemoryManager();
    }

	/**
	 * Parse the passed in uri
	 * 
	 * @param uriString		URI to parse
	 * @param uriStringLen	Length of the URI string
	 */	 
	void parse(
		const XalanDOMChar*			uriString,
		XalanDOMString::size_type	uriStringLen);

	/**
	 * Parse the passed in uri
	 * 
	 * @param uriString		URI to parse
	 * @param uriStringLen	Length of the URI string
	 */	 
	void parse(
		const XalanDOMString &uriString)
	{
		parse(uriString.c_str(), uriString.length());
	}

	/**
	 * Reassemble the uri components to make a complete URI
	 *
	 * @return	The reassembled URI
	 */
	XalanDOMString& make(XalanDOMString&		theResult) const;

	/**
	 * Resolve this URI relative to another, according to RFC2396.
	 *
	 * @param base The base URI to use during resolution.
	 */
	void resolve(const XalanParsedURI &base);

	/**
	 * Resolve this URI relative to another.
	 *
	 * @param base The base URI string
	 * @param baseLen The length of the base URI
     */
	void resolve(
		const XalanDOMChar *base,
		const XalanDOMString::size_type baseLen)
	{
		XalanParsedURI baseURI(base, baseLen,getMemoryManager());

		resolve(baseURI);
	}

	/**
	 * Resolve this URI relative to another.
	 *
	 * @param base The base URI string
     */
	void resolve(
		const XalanDOMString &base)
	{
		resolve(base.c_str(), base.length());
	}

	/**
	 * Resolve the one URI relative to another.
	 *
	 * @relative The URI string to resolve
	 * @relativeLen The lengh of the relative URI string
	 * @base The base URI string
	 * @baseLen The length of the base URI string
	 *
	 */
	static XalanDOMString& resolve(
		const XalanDOMChar			*relative,
		XalanDOMString::size_type	relativeLen,
		const XalanDOMChar			*base,
		XalanDOMString::size_type	baseLen,
        XalanDOMString&		        theResult
	);


	/**
	 * Resolve the one URI relative to another.
	 *
	 * @relative The URI string to resolve
	 * @base The base URI string
	 *
	 */
	static XalanDOMString& resolve(
		const XalanDOMString    &relative,
		const XalanDOMString    &base,
        XalanDOMString&		    theResult
	)
	{
		return resolve(relative.c_str(), relative.length(), base.c_str(), base.length(), theResult);
	}

	/**
	 * Get the scheme component
	 */
	const XalanDOMString& getScheme() const 
	{ 
		return m_scheme; 
	}

	/**
	 * See if the scheme component is defined.
	 */
	bool isSchemeDefined() const 
	{ 
		return m_defined & d_scheme;
	}

	/**
	 * Set the scheme component.  Also sets the scheme defined flag.
	 */
	void setScheme(const XalanDOMChar *scheme) 
	{ 
		m_scheme = scheme;
		m_defined |= d_scheme;
	}

	/**
	 * Set the scheme component.  Also sets the scheme defined flag.
	 */
	void setScheme(const XalanDOMString &scheme) 
	{ 
		m_scheme = scheme;
		m_defined |= d_scheme;
	}

	/**
	 * Get the authority component
	 */
	const XalanDOMString& getAuthority() const 
	{ 
		return m_authority; 
	}

	/**
	 * See if the authority component is defined.
	 */
	bool isAuthorityDefined() const 
	{ 
        return m_defined & d_authority ? true : false; 
	}

	/**
	 * Set the authority component.  Also sets the authority defined flag.
	 */
	void setAuthority(const XalanDOMChar *authority) 
	{ 
		m_authority = authority;
		m_defined |= d_authority;
	}

	/**
	 * Set the authority component.  Also sets the authority defined flag.
	 */
	void setAuthority(const XalanDOMString &authority) 
	{ 
		m_authority = authority;
		m_defined |= d_authority;
	}

	/**
	 * Get the path component
	 */
	const XalanDOMString& getPath() const 
	{ 
		return m_path; 
	}

	/**
	 * Set the path component.
	 */
	void setPath(const XalanDOMChar *path) 
	{ 
		m_path = path;
	}

	/**
	 * Set the path component.
	 */
	void setPath(const XalanDOMString &path) 
	{ 
		m_path = path;
	}

	/**
	 * Get function to get the query component
	 */
	const XalanDOMString& getQuery() const 
	{ 
		return m_query; 
	}

	/**
	 * See if the query component is defined.
	 */
	bool isQueryDefined() const 
	{ 
		return m_defined & d_query ? true : false;
	}

	/**
	 * Set the query component.  Also sets the query defined flag.
	 */
	void setQuery(const XalanDOMChar *query) 
	{ 
		m_query = query;
		m_defined |= d_query;
	}

	/**
	 * Set the query component.  Also sets the query defined flag.
	 */
	void setQuery(const XalanDOMString &query) 
	{ 
		m_query = query;
		m_defined |= d_query;
	}

	/**
	 * Get the fragment component
	 */
	const XalanDOMString& getFragment() const 
	{ 
		return m_fragment; 
	}

	/**
	 * See if the fragment component is defined.
	 */
	bool isFragmentDefined() const 
	{ 
		return m_defined & d_fragment ? true : false;
	}

	/**
	 * Set the fragment component.  Also sets the fragment defined flag.
	 */
	void setFragment(const XalanDOMChar *fragment) 
	{ 
		m_fragment = fragment;
		m_defined |= d_fragment;
	}

	/**
	 * Set the fragment component.  Also sets the fragment defined flag.
	 */
	void setFragment(const XalanDOMString &fragment) 
	{ 
		m_fragment = fragment;
		m_defined |= d_fragment;
	}

	/**
	 * Get the defined components mask.
	 */
	unsigned int getDefined() const
	{
		return m_defined;
	}

	/**
	 * Set the defined components mask.
	 */
	void setDefined(unsigned int defined)
	{
		m_defined = defined;
	}

private:
    // not implemented
    XalanParsedURI();
    XalanParsedURI(const XalanParsedURI&);

	XalanDOMString	m_scheme;
	XalanDOMString	m_authority;
	XalanDOMString	m_path;
	XalanDOMString	m_query;
	XalanDOMString	m_fragment;

	unsigned int	m_defined;
};

XALAN_CPP_NAMESPACE_END

#endif // XALANPARSEDURI_HEADER_GUARD_1357924680
