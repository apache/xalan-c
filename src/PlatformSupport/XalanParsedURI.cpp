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



#include "XalanParsedURI.hpp"



#include "XalanUnicode.hpp"



XALAN_CPP_NAMESPACE_BEGIN


#if defined(XALAN_INLINE_INITIALIZATION) && !defined(XALAN_INLINE_INITIALIZATION_IS_DEFINITION_BUG)
const int	XalanParsedURI::d_scheme;
const int	XalanParsedURI::d_authority;
const int	XalanParsedURI::d_query;
const int	XalanParsedURI::d_fragment;
#endif



/* Merge the components back into a complete URI string */
XalanDOMString XalanParsedURI::make() const
{
	XalanDOMString uri;
	if (m_defined & d_scheme)
	{
		uri += m_scheme;
		uri += XalanUnicode::charColon;
	}
	if (m_defined & d_authority)
	{
		uri += XalanUnicode::charSolidus;
		uri += XalanUnicode::charSolidus;
		uri += m_authority;
	}
	uri += m_path;
	if (m_defined & d_query)
	{
		uri += XalanUnicode::charQuestionMark;
		uri += m_query;
	}
	if (m_defined & d_fragment)
	{
		uri += XalanUnicode::charNumberSign;
		uri += m_fragment;
	}
	return uri;
}

/* Parse a URI into component parts.
   Essentially implements the regex ^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?
 */
void XalanParsedURI::parse(
	const XalanDOMChar*			uriString,
	XalanDOMString::size_type	uriStringLen
)
{
	XalanDOMString::size_type index = 0;
	
	// Clear the components present mask
	m_defined = 0;

	// Scheme portion
	while (index < uriStringLen && 
				uriString[index] != XalanUnicode::charColon && 
				uriString[index] != XalanUnicode::charSolidus && 
				uriString[index] != XalanUnicode::charQuestionMark && 
				uriString[index] != XalanUnicode::charNumberSign)
	{
		++index;
	}
	
	if (index > 0 && uriString[index] == XalanUnicode::charColon)
	{
		m_scheme = XalanDOMString(uriString, index);
		++index;
		m_defined |= d_scheme;
	}
	else
	{
		index = 0;
		m_scheme.clear();
	}

	// Authority portion
	if (index < uriStringLen - 1 &&
		uriString[index] == XalanUnicode::charSolidus && 
		uriString[index+1] == XalanUnicode::charSolidus) 
	{
		index += 2;
		XalanDOMString::size_type authority = index;

		while (index < uriStringLen &&
				uriString[index] != XalanUnicode::charSolidus && 
				uriString[index] != XalanUnicode::charQuestionMark && 
				uriString[index] != XalanUnicode::charNumberSign)
		{
			++index;
		}
		m_authority = XalanDOMString(uriString + authority, index - authority);
		m_defined |= d_authority;
	}
	else
	{
		m_authority.clear();
	}

	// Path portion
	XalanDOMString::size_type path = index;
	while (index < uriStringLen &&
			uriString[index] != XalanUnicode::charQuestionMark && 
			uriString[index] != XalanUnicode::charNumberSign)
	{
		++index;
	}
	m_path = XalanDOMString(uriString + path, index - path);

	// Query portion
	if (index < uriStringLen && uriString[index] == XalanUnicode::charQuestionMark)
	{
		++index;
		XalanDOMString::size_type query = index;

		while (index < uriStringLen &&
				uriString[index] != XalanUnicode::charNumberSign)
		{
			++index;
		}
		m_query = XalanDOMString(uriString + query, index - query);
		m_defined |= d_query;
	}
	else
	{
		m_query.clear();
	}

	// Fragment portion
	if (index < uriStringLen && uriString[index] == XalanUnicode::charNumberSign)
	{
		++index;
		m_fragment = XalanDOMString(uriString + index, uriStringLen - index);
		m_defined |= d_fragment;
	}
	else
	{
		m_fragment.clear();
	}
}

/* Case insensitive comparison for URIs.  Limited to A-Za-z */
static int ci_equals(const XalanDOMString &s1, const XalanDOMString &s2)
{
	if (s1.length() != s2.length())
		return false;

	const XalanDOMChar *p1 = s1.c_str(), *p2 = s2.c_str();
	for ( ; *p1 ; p1++, p2++)
	{
		XalanDOMChar c1 = *p1, c2 = *p2;
		if (c1 >= XalanUnicode::charLetter_A && c1 <= XalanUnicode::charLetter_Z)
			c1 = XalanUnicode::charLetter_a + (c1 - XalanUnicode::charLetter_A);
		if (c2 >= XalanUnicode::charLetter_A && c2 <= XalanUnicode::charLetter_Z)
			c2 = XalanUnicode::charLetter_a + (c2 - XalanUnicode::charLetter_A);
		if (c1 != c2)
			return false;
	}

	return true;
}

/* Resolve this URI relative to another according to RFC2396, section 5.2 */
void XalanParsedURI::resolve(
	const XalanParsedURI &base
)
{
	// Handle references to the current document (step 2)
	if ((m_defined & (d_scheme | d_authority | d_query)) == 0 &&
		m_path.empty())
	{
		m_scheme	= base.m_scheme;
		m_authority = base.m_authority;
		m_path		= base.m_path;
		m_query		= base.m_query;

		// There is an error/unclarity in the specification in step 2 in that
		// it doesn't state that the fragment should be inherited; however
		// it is clear from the examples that it should be
		if (!(m_defined & d_fragment))
		{
			m_fragment = base.m_fragment;
		}

		m_defined |= base.m_defined;
		return;
	}

	// A defined scheme component implies that this is an absolute URI (step 3)
	// Also allow a scheme without authority that matches the base scheme to be 
	// interpreted as a relative URI
	if (!(m_defined & d_scheme) || ( 
			(base.m_defined & d_scheme) && !(m_defined & d_authority) 
			&& ci_equals(m_scheme, base.m_scheme)))
	{
		// Inherit the base scheme
		m_scheme = base.m_scheme;
		m_defined |= d_scheme;

		// Step 4: If the authority is unm_defined then inherit it, otherwise skip to step 7
		if (!(m_defined & d_authority))
		{
			// Inherit the base authority
			m_authority = base.m_authority;
			m_defined |= d_authority;

			// Step 5: if the path starts with a / then it is absolute
			if (!(m_path.length() > 0 && m_path[0] == XalanUnicode::charSolidus))
			{
				// Step 6: merge relative path components

				// a) strip off characters after the right most slash in the base path
				XalanDOMString::size_type pathEnd = base.m_path.length();
				while (pathEnd >= 0 && base.m_path[pathEnd] != XalanUnicode::charSolidus)
				{
					--pathEnd;
				}

				if (pathEnd >= 0) 
				{
					// b) append relative path
					m_path = base.m_path.substr(0, pathEnd + 1) + m_path;
				}
				else
				{
					// TODO, maybe raise an error here as this
					// is a severely wonky looking URI
				}

				// c)->g remove various "./" and "../" segments
				for (XalanDOMString::size_type index = 0; index < m_path.length(); ) 
				{
					// remove '<segment>/../' and ./
					if (m_path[index] == XalanUnicode::charFullStop) 
					{
						if (index < m_path.length()-1 && 
							m_path[index+1] == XalanUnicode::charSolidus) // ./
						{
							m_path.erase(index,2);
							continue;
						} 
						else if (index == m_path.length()-1) // trailing /.
						{
							m_path.erase(index,1);
							continue;
						} 
						// Note: also strips leading ../ in an attempt to get 
						// something out of a bad m_path
						else if (index < m_path.length()-2 && 
									m_path[index+1] == XalanUnicode::charFullStop && 
									m_path[index+2] == XalanUnicode::charSolidus) // ../
						{ 
							int end = index+2;
							if (index > 0) --index;
							for ( ; index > 0 && m_path[index-1] != XalanUnicode::charSolidus; index--) 
								;
							if (index > 0) --index;
							m_path.erase(index,end-index);
							continue;
						} 
						else if (index == m_path.length()-2 && 
									m_path[index+1] == XalanUnicode::charFullStop) // trailing /..
						{
							int end = index+2;
							if (index > 0) --index;
							for ( ; index > 0 && m_path[index-1] != XalanUnicode::charSolidus; index--) 
								;
							m_path.erase(index,end-index);
							continue;
						}
					}
					for ( ; index < m_path.length() && m_path[index] != XalanUnicode::charSolidus ; ++index)
					{
					}
					++index;
				}
			}
		}
	}
}

/* Static helper function to perform a resolve without mucking about with this class */
XalanDOMString XalanParsedURI::resolve(
	const XalanDOMChar			*relative,
	XalanDOMString::size_type	relativeLen,
	const XalanDOMChar			*base,
	XalanDOMString::size_type	baseLen
)
{
	XalanParsedURI relativeURI(relative, relativeLen);
	XalanParsedURI baseURI(base, baseLen);

	relativeURI.resolve(baseURI);
	return relativeURI.make();
}

XALAN_CPP_NAMESPACE_END
