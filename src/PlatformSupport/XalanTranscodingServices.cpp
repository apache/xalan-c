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
#include "XalanTranscodingServices.hpp"



#include <cassert>



#include <util/PlatformUtils.hpp>
#include <util/TransService.hpp>



#include "DOMStringHelper.hpp"
#include "XalanToXercesTranscoderWrapper.hpp"
#include "XalanUTF16Transcoder.hpp"



const XalanDOMChar 	XalanTranscodingServices::s_UTF8String[] =
{
	XalanUnicode::charLetter_U,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_F, 
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charDigit_8,
	0
};



const XalanDOMChar 	XalanTranscodingServices::s_UTF16String[] =
{
	XalanUnicode::charLetter_U,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_F, 
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charDigit_1,
	XalanUnicode::charDigit_6,
	0
};



static XalanTranscodingServices::eCode
translateCode(XMLTransService::Codes	theCode)
{
	if (theCode == XMLTransService::Ok)
	{
		return XalanTranscodingServices::OK;
	}
	else if (theCode == XMLTransService::UnsupportedEncoding)
	{
		return XalanTranscodingServices::UnsupportedEncoding;
	}
	else if (theCode == XMLTransService::InternalFailure)
	{
		return XalanTranscodingServices::InternalFailure;
	}
	else
	{
		assert(theCode == XMLTransService::SupportFilesNotFound);

		return XalanTranscodingServices::SupportFilesNotFound;
	}
}



XalanOutputTranscoder*
XalanTranscodingServices::makeNewTranscoder(
			const XalanDOMString&	theEncodingName,
			eCode&					theResult,
			unsigned int			theBlockSize)
{
	assert(XMLPlatformUtils::fgTransService != 0);

	XalanOutputTranscoder*	theTranscoder = 0;

	XMLTransService::Codes	theCode = XMLTransService::Ok;

	if (encodingIsUTF16(theEncodingName) == true)
	{
		theResult = OK;

		theTranscoder = new XalanUTF16Transcoder;
	}
	else
	{
		XMLTranscoder*	theXercesTranscoder = 
			XMLPlatformUtils::fgTransService->makeNewTranscoderFor(
					c_wstr(theEncodingName),
					theCode,
					theBlockSize);

		theResult = translateCode(theCode);
		assert(theResult == XMLTransService::Ok ||
			   theXercesTranscoder == 0);

		if (theResult == XMLTransService::Ok)
		{
			theTranscoder = new XalanToXercesTranscoderWrapper(theXercesTranscoder);
		}
	}

	return theTranscoder;
}



void
XalanTranscodingServices::destroyTranscoder(XalanOutputTranscoder*	theTranscoder)
{
	delete theTranscoder;
}



bool
XalanTranscodingServices::encodingIsUTF16(const XalanDOMChar*	theEncodingName)
{
	return compareIgnoreCase(c_wstr(theEncodingName), s_UTF16String) == 0 ? true : false;
}


bool
XalanTranscodingServices::encodingIsUTF16(const XalanDOMString&		theEncodingName)
{
	return encodingIsUTF16(c_wstr(theEncodingName));
}



// Some vectors for containing byte-order marks, or other leading bytes
// that would indicate the encoding.
static const XalanTranscodingServices::XalanXMLByteVectorType	s_dummyVector;
static XalanTranscodingServices::XalanXMLByteVectorType			s_UTF8Vector;
static XalanTranscodingServices::XalanXMLByteVectorType			s_UTF16Vector;



const XalanTranscodingServices::XalanXMLByteVectorType&
XalanTranscodingServices::getStreamProlog(const XalanDOMString&		theEncodingName)
{
	if (compareIgnoreCase(c_wstr(theEncodingName), s_UTF16String) == 0)
	{
		return s_UTF16Vector;
	}
#if 0
	// We won't do this for now...
	else if (compareIgnoreCase(c_wstr(theEncodingName), s_UTF8String) == 0)
	{
		return s_UTF8Vector;
	}
#endif
	else
	{
		return s_dummyVector;
	}
}



XalanOutputTranscoder::XalanOutputTranscoder()
{
}



XalanOutputTranscoder::~XalanOutputTranscoder()
{
}



void
XalanTranscodingServices::initialize()
{
	s_UTF8Vector.reserve(3);

	// These are the characters that comprise the
	// UTF-8 byte-order mark.
	s_UTF8Vector.push_back(0xEF);
	s_UTF8Vector.push_back(0xBB);
	s_UTF8Vector.push_back(0xBF);

	s_UTF16Vector.reserve(2);

	// The byte order mark varies, since we're writing
	// UTF-16 out in the platform's endian-ness.
#if defined(XALAN_LITLE_ENDIAN)
	s_UTF16Vector.push_back(0xFF);
	s_UTF16Vector.push_back(0xFE);
#elif defined(XALAN_BIG_ENDIAN)
	s_UTF16Vector.push_back(0xFE);
	s_UTF16Vector.push_back(0xFF);
#else
#error The platform must define the byte order!
#endif
}



void
XalanTranscodingServices::terminate()
{
	XalanXMLByteVectorType().swap(s_UTF16Vector);
	XalanXMLByteVectorType().swap(s_UTF8Vector);
}
