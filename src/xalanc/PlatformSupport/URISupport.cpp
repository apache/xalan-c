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
#if _XERCES_VERSION >= 20300
			const ArrayJanitor<XMLCh>	theFullPathGuard(XMLPlatformUtils::getFullPath(c_wstr(urlString)), XMLPlatformUtils::fgMemoryManager);
#else
			const ArrayJanitor<XMLCh>	theFullPathGuard(XMLPlatformUtils::getFullPath(c_wstr(urlString)));
#endif
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
