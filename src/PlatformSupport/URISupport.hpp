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
#if !defined(URISUPPORT_HEADER_GUARD_1357924680)
#define URISUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xercesc/util/XMLURL.hpp>



#include <XalanDOM/XalanDOMString.hpp>



#include <Include/XalanAutoPtr.hpp>



#include <PlatformSupport/XSLException.hpp>



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
	getURLFromString(const XalanDOMString&	urlString)
	{
		return getURLFromString(urlString.c_str());
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
			XMLURLType&				url)
	{
		getURLFromString(urlString.c_str(), url);
	}

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @return auto pointer to fully qualified URI
	 */
	static URLAutoPtrType
	getURLFromString(const XalanDOMChar*	urlString);

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param url to update with the qualified string.
	 */
	static void
	getURLFromString(
			const XalanDOMChar*		urlString,
			XMLURLType&				url)
	{
		url.setURL(getURLStringFromString(urlString).c_str());
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
			const XalanDOMString&	base)
	{
		return getURLFromString(getURLStringFromString(urlString, base));
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
	static XalanDOMString
	getURLStringFromString(const XalanDOMString&	urlString)
	{
		XalanDOMString	result;

		getURLStringFromString(urlString.c_str(), urlString.length(), result);

		return result;
	}

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
	 * @return string to fully qualified URI
	 */
	static XalanDOMString
	getURLStringFromString(const XalanDOMChar*	urlString)
	{
		XalanDOMString	theNormalizedURI;

		getURLStringFromString(urlString, theNormalizedURI);

		return theNormalizedURI;
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
	 * @return string to fully qualified URI
	 */
	static XalanDOMString
	getURLStringFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base)
	{
		XalanDOMString	theNormalizedURI;

		getURLStringFromString(
			urlString.c_str(),
			urlString.length(),
			base.c_str(),
			base.length(),
			theNormalizedURI);

		return theNormalizedURI;
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
	 * @return string to fully qualified URI
	 */
	static XalanDOMString
	getURLStringFromString(
			const XalanDOMChar*		urlString,
			const XalanDOMChar*		base)
	{
		XalanDOMString	theNormalizedURI;

		getURLStringFromString(urlString, base, theNormalizedURI);

		return theNormalizedURI;
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
	static const XalanDOMString
	NormalizeURIText(const XalanDOMString&	uriString);


	class InvalidURIException : public XSLException
	{
	public:

		/**
		 * Construct an InvalidURIException.
		 *
		 * @param theMessage the error message
		 */
		InvalidURIException(const XalanDOMString&	theMessage);

		virtual
		~InvalidURIException();
	};


	static const XalanDOMChar	s_fileProtocolString1[];

	static const XalanDOMChar	s_fileProtocolString2[];
};



XALAN_CPP_NAMESPACE_END



#endif	// URISUPPORT_HEADER_GUARD_1357924680
