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
#if !defined(XALANTRANSCODINGSERVICES_HEADER_GUARD_1357924680)
#define XALANTRANSCODINGSERVICES_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cassert>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanOutputTranscoder;



class XALAN_PLATFORMSUPPORT_EXPORT XalanTranscodingServices
{
public:

	/**
	 * Perform static initialization.  See class PlatformSupportInit.
	 */
	static void
	initialize(MemoryManagerType&      theManager);
 
	/**
	 * Perform static shut down.  See class PlatformSupportInit.
	 */
	static void
	terminate();

	typedef unsigned char	XalanXMLByte;
    typedef unsigned int    size_type;
    typedef unsigned int    UnicodeCharType;

	static size_type
	length(const XalanXMLByte*	theBytes)
	{
		assert(theBytes != 0);

        size_type   i = 0;

		while(theBytes[i] != 0)
		{
			++i;
		}

		return i;
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
            MemoryManagerType&      theManager,
			const XalanDOMString&	theEncodingName,
			eCode&					theResult,
			size_type				theBlockSize);

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
	canTranscodeToLocalCodePage(UnicodeCharType     theChar)
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
			UnicodeCharType			theCharacter,
			const XalanDOMString&	theEncoding,
            XalanDOMString&         theBuffer);

		virtual
		~UnrepresentableCharacterException();

		UnicodeCharType
		getCharacter() const
		{
			return m_badCharacter;
		}

		const XalanDOMString&
		getEncoding() const
		{
			return m_encoding;
		}

		virtual const XalanDOMChar*
		getType() const
		{
			return m_type;
		}

	private:
        // not implemented
        UnrepresentableCharacterException(const UnrepresentableCharacterException&);

		static const XalanDOMChar	m_type[];


		const UnicodeCharType	m_badCharacter;

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

	typedef XalanTranscodingServices::XalanXMLByte	    XalanXMLByte;
	typedef XalanTranscodingServices::eCode			    eCode;
    typedef XalanTranscodingServices::size_type         size_type;
    typedef XalanTranscodingServices::UnicodeCharType   UnicodeCharType;

	explicit
	XalanOutputTranscoder(MemoryManagerType& theManager);

	virtual
	~XalanOutputTranscoder();

    MemoryManagerType&
    getMemoryManager()
    {
        return m_memoryManager;
    }

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
			size_type				theSourceCount,
			XalanXMLByte*			theTarget,
			size_type			    theTargetSize,
			size_type&				theSourceCharsTranscoded,
			size_type&				theTargetBytesUsed) = 0;

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
			size_type				theSourceCount,
			XalanDOMChar*			theTarget,
			size_type				theTargetSize,
			size_type&				theSourceCharsTranscoded,
			size_type&				theTargetBytesUsed,
			unsigned char*			theCharSizes) = 0;

	virtual bool
	canTranscodeTo(UnicodeCharType	theChar) const = 0;

private:

    MemoryManagerType&              m_memoryManager;
	// Not implemented...
	XalanOutputTranscoder(const XalanOutputTranscoder&	theSource);

	bool
	operator==(const XalanOutputTranscoder&	theRHS) const;

	XalanOutputTranscoder&
	operator=(const XalanOutputTranscoder&	theRHS);
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANTRANSCODINGSERVICES_HEADER_GUARD_1357924680
