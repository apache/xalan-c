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
#if !defined(URISUPPORT_HEADER_GUARD_1357924680)
#define URISUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xercesc/util/XMLURL.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/Include/XalanAutoPtr.hpp>



#include <xalanc/PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER XMLURL	XMLURLType;



class XALAN_PLATFORMSUPPORT_EXPORT URISupport
{
public:

	typedef XalanAutoPtr<XMLURLType>	URLAutoPtrType;

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @return auto pointer to fully qualified URI
	 */
	static URLAutoPtrType
	getURLFromString(const XalanDOMString&	urlString,
                        MemoryManagerType&     theManager)
	{
		return getURLFromString(urlString.c_str(), theManager);
	}

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param url to update with the qualified string.
	 */
	static void
	getURLFromString(
			const XalanDOMString&	urlString,
			XMLURLType&				url,
            MemoryManagerType&      theManager)
	{
		getURLFromString(urlString.c_str(), url, theManager);
	}

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @return auto pointer to fully qualified URI
	 */
	static URLAutoPtrType
	getURLFromString(const XalanDOMChar*	    urlString,
                        MemoryManagerType&     theManager);

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param url to update with the qualified string.
	 */
	static void
	getURLFromString(
			const XalanDOMChar*		urlString,
			XMLURLType&				url,
            MemoryManagerType&      theManager)
	{
        XalanDOMString buffer(theManager);

        getURLStringFromString(urlString, buffer);

		url.setURL(buffer.c_str());
	}

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param base base location for URI
	 * @return auto pointer to fully qualified URI
	 */
	static URLAutoPtrType
	getURLFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base,
            MemoryManagerType&  theManager)
	{
        XalanDOMString theResult(theManager);

        getURLStringFromString(urlString, base, theResult);

		return getURLFromString(theResult, theManager);
	}

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param base base location for URI
	 * @return auto pointer to fully qualified URI
	 */
	static URLAutoPtrType
	getURLFromString(
			const XalanDOMChar*		urlString,
			const XalanDOMChar*		base);


	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @return string to fully qualified URI
	 */
	static void
	getURLStringFromString(
			const XalanDOMString&	urlString,
			XalanDOMString&			theNormalizedURI)
	{
		getURLStringFromString(urlString.c_str(), urlString.length(), theNormalizedURI);
	}


	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param theNormalizedURI fully qualified URI
	 */
	static void
	getURLStringFromString(
			const XalanDOMChar*			urlString,
			XalanDOMString&				theNormalizedURI)
	{
		assert(urlString != 0);

		getURLStringFromString(
			urlString,
			XalanDOMString::length(urlString),
			theNormalizedURI);
	}

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param len the length of urlString
	 * @param theNormalizedURI fully qualified URI
	 */
	static void
	getURLStringFromString(
			const XalanDOMChar*			urlString,
			XalanDOMString::size_type	len,
			XalanDOMString&				theNormalizedURI);

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param base base location for URI
	 * @param theNormalizedURI fully qualified URI
	 */
	static void
	getURLStringFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base,
			XalanDOMString&			theNormalizedURI)
	{
		getURLStringFromString(urlString.c_str(), base.c_str(), theNormalizedURI);
	}


	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param base base location for URI
	 * @param theNormalizedURI fully qualified URI
	 */
	static void
	getURLStringFromString(
			const XalanDOMChar*		urlString,
			const XalanDOMChar*		base,
			XalanDOMString&			theNormalizedURI)
	{
		assert(urlString != 0 && base != 0);

		getURLStringFromString(
			urlString,
			XalanDOMString::length(urlString),
			base,
			XalanDOMString::length(base),
			theNormalizedURI);
	}

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param base base location for URI
	 * @param theNormalizedURI fully qualified URI
	 */
	static void
	getURLStringFromString(
			const XalanDOMChar*			urlString,
			XalanDOMString::size_type	urlStringLen,
			const XalanDOMChar*			base,
			XalanDOMString::size_type	baseLen,
			XalanDOMString&				theNormalizedURI);

	/**
	 * Normalizes the string passed in, replacing
	 * \ with /.
	 *
	 * @param urlString string to normalize
	 * @return a reference to the passed parameter
	 */
	static XalanDOMString&
	NormalizeURIText(XalanDOMString&	uriString);

	/**
	 * Normalizes the string passed in, replacing
	 * \ with /.
	 *
	 * @param urlString string to normalize
	 * @return a copy of the normalized URI
	 */


	class InvalidURIException : public XSLException
	{
	public:

		/**
		 * Construct an InvalidURIException.
		 *
		 * @param theMessage the error message
		 */
		InvalidURIException(const XalanDOMString&	theMessage,
                            MemoryManagerType&      theManager);

		virtual
		~InvalidURIException();

		virtual const XalanDOMChar*
		getType() const
		{
			return m_type;
		}

	private:
		static const XalanDOMChar	m_type[];
	};


	static const XalanDOMChar	s_fileProtocolString1[];

	static const XalanDOMChar	s_fileProtocolString2[];
};



XALAN_CPP_NAMESPACE_END



#endif	// URISUPPORT_HEADER_GUARD_1357924680
