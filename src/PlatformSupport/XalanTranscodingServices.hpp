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
#if !defined(XALANTRANSCODINGSERVICES_HEADER_GUARD_1357924680)
#define XALANTRANSCODINGSERVICES_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <map>
#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/XSLException.hpp>



class XalanOutputTranscoder;



class XALAN_PLATFORMSUPPORT_EXPORT XalanTranscodingServices
{
public:

	typedef unsigned char	XalanXMLByte;

#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString,
				XalanDOMChar,
				less<XalanDOMString> >	MaximumCharacterValueMapType;
	typedef vector<XalanXMLByte>		XalanXMLByteVectorType;
#else
	typedef std::map<XalanDOMString,
					 XalanDOMChar>		MaximumCharacterValueMapType;
	typedef std::vector<XalanXMLByte>	XalanXMLByteVectorType;
#endif

	/**
	 * Perform static initialization.  See class PlatformSupportInit.
	 */
	static void
	initialize();

	/**
	 * Perform static shut down.  See class PlatformSupportInit.
	 */
	static void
	terminate();

    enum eCode
    {
		OK,
		UnsupportedEncoding,
		InternalFailure,
		SupportFilesNotFound
    };


	/**
	 * Create a transcoder instance for the specified encoding.
	 * 
	 * @param theEncodingName The name of the desired output encoding.
	 * @param theResult The error code.
	 * @param theTarget The target array for storing the transcoded data.
	 * @param theBlockSize The block size the transcoder should use.
	 * @return The new transcoder, or 0 if there was an error.
	 */
	static XalanOutputTranscoder*
	makeNewTranscoder(
			const XalanDOMString&	theEncodingName,
			eCode&					theResult,
			unsigned int			theBlockSize);

	/**
	 * Destroy a transcoder instance.
	 * 
	 * @theTranscoder The transcoder to destroy.
	 */
	static void
	destroyTranscoder(XalanOutputTranscoder*	theTranscoder);

	/**
	 * Determine if the encoding name supplied is equivalent to UTF-8.
	 * 
	 * @param theEncodingName The name of the desired output encoding.
	 * @return true or false
	 */
	static bool
	encodingIsUTF8(const XalanDOMChar*		theEncodingName);

	/**
	 * Determine if the encoding name supplied is equivalent to UTF-8.
	 * 
	 * @param theEncodingName The name of the desired output encoding.
	 * @return true or false
	 */
	static bool
	encodingIsUTF8(const XalanDOMString&	theEncodingName);

	/**
	 * Determine if the encoding name supplied is equivalent to UTF-16.
	 * 
	 * @param theEncodingName The name of the desired output encoding.
	 * @return true or false
	 */
	static bool
	encodingIsUTF16(const XalanDOMChar*		theEncodingName);

	/**
	 * Determine if the encoding name supplied is equivalent to UTF-16.
	 * 
	 * @param theEncodingName The name of the desired output encoding.
	 * @return true or false
	 */
	static bool
	encodingIsUTF16(const XalanDOMString&	theEncodingName);

	/**
	 * Get a vector that contains any leading bytes that should be
	 * written to an XML stream for the specified encoding.  For example,
	 * if the encoding is UTF-16, the vector will contain the appropriate
	 * byte order mark for the current platform.  If there is no
	 * prolog for the encoding, or the encoding is unknown, an empty
	 * vector is returned.
	 * 
	 * @param theEncodingName The name of the desired output encoding.
	 * @return A reference to a vector containing the appropriate bytes.
	 */
	static const XalanXMLByteVectorType&
	getStreamProlog(const XalanDOMString&	theEncodingName);

	/**
	 * Get the maximum character value for the encoding.
	 *
	 * @param theEncoding The encoding name.
	 * @return The maximum character value the encoding supports.
	 */
	static XalanDOMChar
	getMaximumCharacterValue(const XalanDOMString&	theEncoding);

	/**
	 * Get the maximum character value for the local code page.
	 *
	 * @return The maximum character value the local code page supports.
	 */
	static XalanDOMChar
	getMaximumCharacterValue();

	/**
	 * Determine if the output stage can safely skip transcoding
	 * by truncating.
	 *
	 * @param theEncoding The encoding name.
	 * @return true if truncation is possible, false if not.
	 */
	static bool
	getBytesEqualChars(const XalanDOMString&	theEncoding);

	static const XalanDOMChar 	s_utf8String[];

	static const XalanDOMChar 	s_utf16String[];

	static const XalanDOMChar 	s_asciiString[];

	static const XalanDOMChar 	s_usASCIIString[];

	static const XalanDOMChar 	s_windows1250String[];

	class XALAN_PLATFORMSUPPORT_EXPORT UnrepresentableCharacterException : public XSLException
	{
	public:

		UnrepresentableCharacterException(
			XalanDOMChar			theCharacter,
			const XalanDOMString&	theEncoding);

		virtual
		~UnrepresentableCharacterException();

		XalanDOMChar
		getCharacter() const
		{
			return m_badCharacter;
		}

		const XalanDOMString&
		getEncoding() const
		{
			return m_encoding;
		}

	private:

		const XalanDOMChar		m_badCharacter;

		const XalanDOMString	m_encoding;
	};

private:

	static const MaximumCharacterValueMapType&	s_maximumCharacterValues;
};



class XALAN_PLATFORMSUPPORT_EXPORT XalanOutputTranscoder
{
public:

	typedef XalanTranscodingServices::XalanXMLByte	XalanXMLByte;
	typedef XalanTranscodingServices::eCode			eCode;

	explicit
	XalanOutputTranscoder();

	virtual
	~XalanOutputTranscoder();

	/**
	 * Transcode data from UTF-16 to the transcoder's encoding.  If successfull,
	 * theResult will be set to OK.  If not, it will be set to one of the other
	 * codes.
	 *
	 * @param theSourceData The source data to transcode
	 * @param theSourceCount The length of the source data.
	 * @param theTarget The target array for storing the transcoded data.
	 * @param theTargetSize The number of characters that can be stored in the target.
	 * @param theSourceCharsTranscoded The count of the source characters that were transcoded.
	 * @param theTargetBytesUsed The number of characters stored in the target array
	 * @return The result code.
	 */
	virtual eCode
	transcode(
			const XalanDOMChar*		theSourceData,
			unsigned int			theSourceCount,
			XalanXMLByte*			theTarget,
			unsigned int			theTargetSize,
			unsigned int&			theSourceCharsTranscoded,
			unsigned int&			theTargetBytesUsed) = 0;

private:

	// Not implemented...
	XalanOutputTranscoder(const XalanOutputTranscoder&	theSource);

	bool
	operator==(const XalanOutputTranscoder&	theRHS) const;

	XalanOutputTranscoder&
	operator=(const XalanOutputTranscoder&	theRHS);
};



#endif	// NULLTEXTOUTPUTSTREAM_HEADER_GUARD_1357924680
