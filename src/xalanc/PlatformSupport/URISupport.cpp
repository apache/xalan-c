/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000-2002 The Apache Software Foundation.  All rights 
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
#include "URISupport.hpp"



#include <algorithm>
#include <vector>



#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include "XalanUnicode.hpp"
#include "XalanParsedURI.hpp"


XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar	URISupport::s_fileProtocolString1[] =
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



const XalanDOMChar	URISupport::s_fileProtocolString2[] =
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
URISupport::getURLFromString(const XalanDOMChar*	urlString)
{
	URLAutoPtrType	url(new XMLURLType);

	url->setURL(getURLStringFromString(urlString).c_str());

	return url;
}



void
URISupport::getURLStringFromString(
			const XalanDOMChar*			urlString,
			XalanDOMString::size_type	len,
			XalanDOMString&				theNormalizedURI)
{
	assert(urlString != 0);

	// Let's see what sort of URI we have...
	if (len != 0)
	{
		const XalanDOMString::size_type		index = indexOf(urlString, XalanUnicode::charColon);

		bool	protocolPresent = false;

		if (index != len)
		{
			const XalanDOMString	theProtocolString(urlString, index);

			// $$$ ToDo: XMLURL::lookupByName() is supposed to be static, but is not.
			const XMLURLType::Protocols		theProtocol =
				XMLURLType().lookupByName(c_wstr(theProtocolString));

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
			const ArrayJanitor<XMLCh>	theFullPathGuard(XMLPlatformUtils::getFullPath(c_wstr(urlString)), XMLPlatformUtils::fgMemoryManager);

			const XalanDOMChar* const	theFullPath = theFullPathGuard.get();
			assert(theFullPath != 0);

			const XalanDOMString::size_type		theFullPathLength =
				XalanDOMString::length(theFullPath);

			assert(theFullPathLength > 0);

			if (theFullPath[0] == XalanDOMChar(XalanUnicode::charSolidus))
			{
				const size_t	theSize = sizeof(s_fileProtocolString1) / sizeof(s_fileProtocolString1[0]) - 1;

				theNormalizedURI.reserve(theFullPathLength + theSize);

				theNormalizedURI.assign(s_fileProtocolString1, theSize);
			}
			else
			{
				const size_t	theSize = sizeof(s_fileProtocolString2) / sizeof(s_fileProtocolString2[0]) - 1;

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
			const XalanDOMChar*			urlString,
			XalanDOMString::size_type	urlStringLen,
			const XalanDOMChar*			base,
			XalanDOMString::size_type	baseLen,
			XalanDOMString&				theNormalizedURI)
{
	XalanDOMString	context(base, baseLen);
	XalanDOMString	url(urlString, urlStringLen);

	// Flip slashes
	NormalizeURIText(context);
	NormalizeURIText(url);

	// Resolve the URI
	url = XalanParsedURI::resolve(url, context);

	// Do platform specific stuff
	getURLStringFromString(url, theNormalizedURI);
}



XalanDOMString&
URISupport::NormalizeURIText(XalanDOMString&	uriString)
{
	// OK, look for a quick, cheap exit...
	const XalanDOMString::size_type		len = length(uriString);
	const XalanDOMString::size_type		index = indexOf(uriString, XalanUnicode::charReverseSolidus);

	if (index != len)
	{
		// Start replacing at the index point, since that's the
		// first one...
		XALAN_STD_QUALIFIER replace(
				uriString.begin() + index,
				uriString.end(),
				XalanDOMChar(XalanUnicode::charReverseSolidus),
				XalanDOMChar(XalanUnicode::charSolidus));
	}

	return uriString;
}



const XalanDOMString
URISupport::NormalizeURIText(const XalanDOMString&	uriString)
{
	XalanDOMString	theCopy(uriString);

	NormalizeURIText(theCopy);

	return theCopy;
}



URISupport::InvalidURIException::InvalidURIException(const XalanDOMString&	theMessage) :
	XSLException(theMessage,
				 TranscodeFromLocalCodePage("InvalidURIException"))
{
}



URISupport::InvalidURIException::~InvalidURIException()
{
}



XALAN_CPP_NAMESPACE_END
