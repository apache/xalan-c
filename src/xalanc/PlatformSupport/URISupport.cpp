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
#include "URISupport.hpp"



#include <algorithm>



#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include "XalanUnicode.hpp"
#include "XalanParsedURI.hpp"


XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar  URISupport::s_fileProtocolString1[] =
{
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    XalanUnicode::charColon,
    XalanUnicode::charSolidus,
    XalanUnicode::charSolidus,
    0
};



const XalanDOMChar  URISupport::s_fileProtocolString2[] =
{
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    XalanUnicode::charColon,
    XalanUnicode::charSolidus,
    XalanUnicode::charSolidus,
    XalanUnicode::charSolidus,
    0
};



URISupport::URLAutoPtrType
URISupport::getURLFromString(
            const XalanDOMChar*     urlString,
            MemoryManager&          theManager)
{
    URLAutoPtrType  url(new (&theManager) XMLURLType(&theManager));

    XalanDOMString normalizedURL(theManager);

    getURLStringFromString(urlString, normalizedURL);

    url->setURL(normalizedURL.c_str());

    return url;
}



void
URISupport::getURLStringFromString(
            const XalanDOMChar*         urlString,
            XalanDOMString::size_type   len,
            XalanDOMString&             theNormalizedURI)
{
    assert(urlString != 0);

    // Let's see what sort of URI we have...
    if (len != 0)
    {
        const XalanDOMString::size_type     index = indexOf(urlString, XalanUnicode::charColon);

        bool    protocolPresent = false;

        if (index != len)
        {
            const XalanDOMString    theProtocolString(urlString, theNormalizedURI.getMemoryManager(), index );

            // $$$ ToDo: XMLURL::lookupByName() is supposed to be static, but is not.
            const XMLURLType::Protocols     theProtocol =
                XMLURLType::lookupByName(theProtocolString.c_str());

            if (theProtocol != XMLURLType::Unknown)
            {
                protocolPresent = true;
            }
        }

        if (protocolPresent == true)
        {
            theNormalizedURI = urlString;

            NormalizeURIText(theNormalizedURI);
        }
        else
        {
            XALAN_USING_XERCES(ArrayJanitor)
            XALAN_USING_XERCES(XMLPlatformUtils)

            // Assume it's a file specification...
            XALAN_USING_XERCES(MemoryManager)

            MemoryManager&  theMemoryManager = theNormalizedURI.getMemoryManager();

            const ArrayJanitor<XMLCh>   theFullPathGuard(
                        XMLPlatformUtils::getFullPath(
                            urlString,
                            &theMemoryManager),
                        &theMemoryManager);

            const XalanDOMChar* const   theFullPath = theFullPathGuard.get();
            assert(theFullPath != 0);

            const XalanDOMString::size_type     theFullPathLength =
                XalanDOMString::length(theFullPath);

            assert(theFullPathLength > 0);

            if (theFullPath[0] == XalanDOMChar(XalanUnicode::charSolidus))
            {
                const size_t    theSize = sizeof(s_fileProtocolString1) / sizeof(s_fileProtocolString1[0]) - 1;

                theNormalizedURI.reserve(theFullPathLength + theSize);

                theNormalizedURI.assign(s_fileProtocolString1, theSize);
            }
            else
            {
                const size_t    theSize = sizeof(s_fileProtocolString2) / sizeof(s_fileProtocolString2[0]) - 1;

                theNormalizedURI.reserve(theFullPathLength + theSize);

                theNormalizedURI.assign(s_fileProtocolString2, theSize);
            }

            theNormalizedURI.append(theFullPath, theFullPathLength);

            NormalizeURIText(theNormalizedURI);
        }
    }
}



void
URISupport::getURLStringFromString(
            const XalanDOMChar*         urlString,
            XalanDOMString::size_type   urlStringLen,
            const XalanDOMChar*         base,
            XalanDOMString::size_type   baseLen,
            XalanDOMString&             theNormalizedURI)
{
    MemoryManager&  theMemoryManager = theNormalizedURI.getMemoryManager();

    XalanDOMString  context(base, theMemoryManager, baseLen);
    XalanDOMString  url(urlString, theMemoryManager, urlStringLen);

    // Flip slashes
    NormalizeURIText(context);
    NormalizeURIText(url);

    // Resolve the URI
    XalanParsedURI::resolve(url, context, url);

    // Do platform specific stuff
    getURLStringFromString(url, theNormalizedURI);
}



XalanDOMString&
URISupport::NormalizeURIText(XalanDOMString&    uriString)
{
    // OK, look for a quick, cheap exit...
    const XalanDOMString::size_type     len = uriString.length();
    const XalanDOMString::size_type     index = indexOf(uriString, XalanUnicode::charReverseSolidus);

    if (index != len)
    {
        // Start replacing at the index point, since that's the
        // first one...
        XALAN_STD_QUALIFIER replace(
                uriString.begin() + index,
                uriString.end(),
                XalanUnicode::charReverseSolidus,
                XalanUnicode::charSolidus);
    }

    return uriString;
}



URISupport::InvalidURIException::InvalidURIException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager,
            const Locator*          theLocator) :
    XSLException(
        theMessage,
        theManager,
        theLocator)
{
}



URISupport::InvalidURIException::InvalidURIException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager) :
    XSLException(
        theMessage,
        theManager)
{
}



URISupport::InvalidURIException::~InvalidURIException()
{
}



static const XalanDOMChar   s_type[] = 
{   
    XalanUnicode::charLetter_I,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_v,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_U,
    XalanUnicode::charLetter_R,
    XalanUnicode::charLetter_I,
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    0
};



const XalanDOMChar*
URISupport::InvalidURIException::getType() const
{
    return s_type;
}



XALAN_CPP_NAMESPACE_END
