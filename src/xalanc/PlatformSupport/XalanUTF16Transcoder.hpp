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
#if !defined(XALANUTF16TRANSCODER_HEADER_GUARD_1357924680)
#define XALANUTF16TRANSCODER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



// Base class header file...
#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT XalanUTF16Transcoder : public XalanOutputTranscoder
{
public:

	explicit
	XalanUTF16Transcoder(MemoryManagerType& theManager);

    static XalanUTF16Transcoder*
    create(MemoryManagerType& theManager);

	virtual
	~XalanUTF16Transcoder();

	/**
	 * Transcode data from UTF-16 to UTF-16.
	 *
	 * @param theSourceData The source data to transcode
	 * @param theSourceCount The length of the source data.
	 * @param theResult The error code, if any.
	 * @param theTarget The target array for storing the transcoded data.
	 * @param theTargetSize The number of characters that can be stored in the target.
	 * @param theSourceCharsTranscoded The count of the source characters that were transcoded.
	 * @param theTargetBytesUsed The number of characters stored in the target array
	 * @return The result code.
	 */
	virtual eCode
	transcode(
			const XalanDOMChar*		theSourceData,
			size_type					theSourceCount,
			XalanXMLByte*			theTarget,
			size_type					theTargetSize,
			size_type&					theSourceCharsTranscoded,
			size_type&					theTargetBytesUsed);

	/**
	 * Transcode data from UTF-16 to UTF-16.
	 *
	 * @param theSourceData The source data to transcode
	 * @param theSourceCount The length of the source data.
	 * @param theResult The error code, if any.
	 * @param theTarget The target array for storing the transcoded data.
	 * @param theTargetSize The number of characters that can be stored in the target.
	 * @param theSourceCharsTranscoded The count of the source characters that were transcoded.
	 * @param theTargetBytesUsed The number of characters stored in the target array
	 * @return The result code.
	 */
	virtual eCode
	transcode(
			const XalanXMLByte*		theSourceData,
			size_type					theSourceCount,
			XalanDOMChar*			theTarget,
			size_type					theTargetSize,
			size_type&					theSourceCharsTranscoded,
			size_type&					theTargetBytesUsed,
			unsigned char*			theCharSizes);

	virtual bool
	canTranscodeTo(UnicodeCharType	theChar) const;

private:

	// Not implemented...
	XalanUTF16Transcoder(const XalanUTF16Transcoder&);

	XalanUTF16Transcoder&
	operator=(const XalanUTF16Transcoder&);
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANUTF16TRANSCODER_HEADER_GUARD_1357924680
