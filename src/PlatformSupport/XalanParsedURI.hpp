/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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

#if !defined(XALANPARSEDURI_HEADER_GUARD_1357924680)
#define XALANPARSEDURI_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <XalanDOM/XalanDOMString.hpp>




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
	XalanParsedURI() :
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
		XalanDOMString::size_type	uriStringLen) : 
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
		const XalanDOMString		&uriString) :
		m_defined(0)
	{
		parse(uriString.c_str(), uriString.length());
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
	XalanDOMString make() const;

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
		XalanParsedURI baseURI(base, baseLen);

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
	static XalanDOMString resolve(
		const XalanDOMChar			*relative,
		XalanDOMString::size_type	relativeLen,
		const XalanDOMChar			*base,
		XalanDOMString::size_type	baseLen
	);


	/**
	 * Resolve the one URI relative to another.
	 *
	 * @relative The URI string to resolve
	 * @base The base URI string
	 *
	 */
	static XalanDOMString resolve(
		const XalanDOMString &relative,
		const XalanDOMString &base
	)
	{
		return resolve(relative.c_str(), relative.length(), base.c_str(), base.length());
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
		return !!(m_defined & d_scheme); 
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
		return !!(m_defined & d_authority); 
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
		return !!(m_defined & d_query); 
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
		return !!(m_defined & d_fragment); 
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
	XalanDOMString	m_scheme;
	XalanDOMString	m_authority;
	XalanDOMString	m_path;
	XalanDOMString	m_query;
	XalanDOMString	m_fragment;

	unsigned int	m_defined;
};

XALAN_CPP_NAMESPACE_END

#endif // XALANPARSEDURI_HEADER_GUARD_1357924680
