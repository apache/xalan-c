/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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



#include "XalanParsedURI.hpp"



#include "DOMStringHelper.hpp"
#include "XalanUnicode.hpp"



XALAN_CPP_NAMESPACE_BEGIN


#if defined(XALAN_INLINE_INITIALIZATION) && !defined(XALAN_INLINE_INITIALIZATION_IS_DEFINITION_BUG)
const int   XalanParsedURI::d_scheme;
const int   XalanParsedURI::d_authority;
const int   XalanParsedURI::d_query;
const int   XalanParsedURI::d_fragment;
#endif



/* Merge the components back into a complete URI string */
XalanDOMString& XalanParsedURI::make(XalanDOMString&        uri) const
{
    uri.erase();

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
    const XalanDOMChar*         uriString,
    XalanDOMString::size_type   uriStringLen
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
        m_scheme = XalanDOMString(uriString, getMemoryManager(), index);
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
        m_authority = XalanDOMString(uriString + authority, getMemoryManager(), index - authority);
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
    m_path = XalanDOMString(uriString + path,getMemoryManager(), index - path);

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
        m_query = XalanDOMString(uriString + query,getMemoryManager(), index - query);
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
        m_fragment = XalanDOMString(uriString + index, getMemoryManager(), uriStringLen - index);
        m_defined |= d_fragment;
    }
    else
    {
        m_fragment.clear();
    }
}

/* Resolve this URI relative to another according to RFC2396, section 5.2 */
void XalanParsedURI::resolve(
    const XalanParsedURI &base
)
{
    if (base.isSchemeDefined() == false)
    {
        // Protect against a base URI that is relative...
        // This might become an assert or an exception.
    }
    // Handle references to the current document (step 2)
    else if ((m_defined & (d_scheme | d_authority | d_query)) == 0 &&
        m_path.empty())
    {
        m_scheme    = base.m_scheme;
        m_authority = base.m_authority;
        m_path      = base.m_path;
        m_query     = base.m_query;

        // There is an error/unclarity in the specification in step 2 in that
        // it doesn't state that the fragment should be inherited; however
        // it is clear from the examples that it should be
        if (!(m_defined & d_fragment))
        {
            m_fragment = base.m_fragment;
        }

        m_defined |= base.m_defined;
    }
    // A defined scheme component implies that this is an absolute URI (step 3)
    // Also allow a scheme without authority that matches the base scheme to be 
    // interpreted as a relative URI
    else if (!(m_defined & d_scheme) || ( 
            (base.m_defined & d_scheme) && !(m_defined & d_authority) 
            && equalsIgnoreCaseASCII(m_scheme, base.m_scheme)))
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
                while (pathEnd > 0 && base.m_path[pathEnd - 1] != XalanUnicode::charSolidus)
                {
                    --pathEnd;
                }

                if (pathEnd > 0) 
                {
                    // b) append relative path
                    // This inserts the path portion from base...
                    m_path.insert(0, base.m_path, 0, pathEnd);
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
                            const XalanDOMString::size_type     end = index + 2;
                            if (index > 0) --index;
                            for ( ; index > 0 && m_path[index-1] != XalanUnicode::charSolidus; index--) 
                                ;
                            if (index > 0) --index;
                            m_path.erase(index, end - index);
                            continue;
                        } 
                        else if (index == m_path.length()-2 && 
                                    m_path[index+1] == XalanUnicode::charFullStop) // trailing /..
                        {
                            const XalanDOMString::size_type     end = index + 2;
                            if (index > 0) --index;
                            for ( ; index > 0 && m_path[index-1] != XalanUnicode::charSolidus; index--) 
                                ;
                            m_path.erase(index, end - index);
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
XalanDOMString& XalanParsedURI::resolve(
    const XalanDOMChar          *relative,
    XalanDOMString::size_type   relativeLen,
    const XalanDOMChar          *base,
    XalanDOMString::size_type   baseLen,
    XalanDOMString&             theResult
)
{
    XalanParsedURI relativeURI(relative, relativeLen, theResult.getMemoryManager());
    XalanParsedURI baseURI(base, baseLen, theResult.getMemoryManager());

    relativeURI.resolve(baseURI);
    return relativeURI.make(theResult);
}

XALAN_CPP_NAMESPACE_END
