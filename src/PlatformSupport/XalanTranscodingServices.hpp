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
#if !defined(XALANTRANSCODINGSERVICES_HEADER_GUARD_1357924680)
#define XALANTRANSCODINGSERVICES_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cassert>
#include <map>
#include <vector>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanOutputTranscoder;



class XALAN_PLATFORMSUPPORT_EXPORT XalanTranscodingServices
{
public:

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

	typedef unsigned char	XalanXMLByte;

	static size_t
	length(const XalanXMLByte*	theBytes)
	{
		assert(theBytes != 0);

		const XalanXMLByte*		theCurrentByte = theBytes;

		while(*theCurrentByte != 0)
		{
			++theCurrentByte;
		}

		return theCurrentByte - theBytes;
	}

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
			size_t					theBlockSize);

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
	 * @param theEncodingName The name of the output encoding.
	 * @return true or false
	 */
	static bool
	encodingIsUTF8(const XalanDOMChar*		theEncodingName);

	/**
	 * Determine if the encoding name supplied is equivalent to UTF-8.
	 * 
	 * @param theEncodingName The name of the output encoding.
	 * @return true or false
	 */
	static bool
	encodingIsUTF8(const XalanDOMString&	theEncodingName);

	/**
	 * Determine if the encoding name supplied is equivalent to UTF-16.
	 * 
	 * @param theEncodingName The name of the output encoding.
	 * @return true or false
	 */
	static bool
	encodingIsUTF16(const XalanDOMChar*		theEncodingName);

	/**
	 * Determine if the encoding name supplied is equivalent to UTF-16.
	 * 
	 * @param theEncodingName The name of the output encoding.
	 * @return true or false
	 */
	static bool
	encodingIsUTF16(const XalanDOMString&	theEncodingName);

	/**
	 * Determine if the encoding name supplied is equivalent to UTF-32.
	 * 
	 * @param theEncodingName The name of the output encoding.
	 * @return true or false
	 */
	static bool
	encodingIsUTF32(const XalanDOMChar*		theEncodingName);

	/**
	 * Determine if the encoding name supplied is equivalent to UTF-32.
	 * 
	 * @param theEncodingName The name of the output encoding.
	 * @return true or false
	 */
	static bool
	encodingIsUTF32(const XalanDOMString&	theEncodingName);
	/**
	 * Get an array that contains any leading bytes that should be
	 * written to an XML stream for the specified encoding.  For example,
	 * if the encoding is UTF-16, the vector will contain the appropriate
	 * byte order mark for the current platform.  If there is no
	 * prolog for the encoding, or the encoding is unknown, an empty
	 * array is returned.
	 * 
	 * @param theEncodingName The name of the desired output encoding.
	 * @return An array containing the appropriate bytes.
	 */
	static const XalanXMLByte*
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
	 * This is now deprecated, since it's impossibly broken.
	 *
	 * @deprecated
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

	static bool
	canTranscodeToLocalCodePage(unsigned int	theChar)
	{
		// Yuck!! See getMaximumCharacterValue() for more details.
		return theChar <= 0x7fu ? true : false;
	}

	static const XalanDOMChar 	s_utf8String[];

	static const XalanDOMChar 	s_utf16String[];

	static const XalanDOMChar 	s_utf16LEString[];

	static const XalanDOMChar 	s_utf16BEString[];

	static const XalanDOMChar 	s_utf32String[];

	static const XalanDOMChar 	s_asciiString[];

	static const XalanDOMChar 	s_usASCIIString[];

	static const XalanDOMChar 	s_windows1250String[];

	static const XalanDOMChar 	s_iso88591String[];

	static const XalanDOMChar	s_shiftJISString[];


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

	static const XalanXMLByte	s_dummyByteOrderMark[];
	static const XalanXMLByte	s_UTF8ByteOrderMark[];
	static const XalanDOMChar	s_UTF16ByteOrderMark[];
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
			size_t					theSourceCount,
			XalanXMLByte*			theTarget,
			size_t					theTargetSize,
			size_t&					theSourceCharsTranscoded,
			size_t&					theTargetBytesUsed) = 0;

	/**
	 * Transcode data from the transcoder's encoding to UTF-16.  If successfull,
	 * theResult will be set to OK.  If not, it will be set to one of the other
	 * codes.
	 *
	 * @param theSourceData The source data to transcode
	 * @param theSourceCount The length of the source data.
	 * @param theTarget The target array for storing the transcoded data.
	 * @param theTargetSize The number of characters that can be stored in the target.
	 * @param theSourceCharsTranscoded The count of the source characters that were transcoded.
	 * @param theTargetBytesUsed The number of characters stored in the target array
	 * @param theCharSizes For each character in theTarget, the number of characters consumed from theSourceData.
	 * @return The result code.
	 */
	virtual eCode
	transcode(
			const XalanXMLByte*		theSourceData,
			size_t					theSourceCount,
			XalanDOMChar*			theTarget,
			size_t					theTargetSize,
			size_t&					theSourceCharsTranscoded,
			size_t&					theTargetBytesUsed,
			unsigned char*			theCharSizes) = 0;

	virtual bool
	canTranscodeTo(unsigned int		theChar) const = 0;

private:

	// Not implemented...
	XalanOutputTranscoder(const XalanOutputTranscoder&	theSource);

	bool
	operator==(const XalanOutputTranscoder&	theRHS) const;

	XalanOutputTranscoder&
	operator=(const XalanOutputTranscoder&	theRHS);
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANTRANSCODINGSERVICES_HEADER_GUARD_1357924680
