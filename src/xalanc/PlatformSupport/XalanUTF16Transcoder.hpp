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
	XalanUTF16Transcoder();
   
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
			size_t					theSourceCount,
			XalanXMLByte*			theTarget,
			size_t					theTargetSize,
			size_t&					theSourceCharsTranscoded,
			size_t&					theTargetBytesUsed);

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
			size_t					theSourceCount,
			XalanDOMChar*			theTarget,
			size_t					theTargetSize,
			size_t&					theSourceCharsTranscoded,
			size_t&					theTargetBytesUsed,
			unsigned char*			theCharSizes);

	virtual bool
	canTranscodeTo(unsigned int		theChar) const;

private:

	// Not implemented...
	XalanUTF16Transcoder(const XalanUTF16Transcoder&);

	XalanUTF16Transcoder&
	operator=(const XalanUTF16Transcoder&);
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANUTF16TRANSCODER_HEADER_GUARD_1357924680
