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



const XalanDOMChar 	XalanTranscodingServices::s_utf8String[] =
{
	XalanUnicode::charLetter_U,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_F,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charDigit_8,
	0
};



const XalanDOMChar 	XalanTranscodingServices::s_utf16String[] =
{
	XalanUnicode::charLetter_U,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_F, 
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charDigit_1,
	XalanUnicode::charDigit_6,
	0
};



const XalanDOMChar 	XalanTranscodingServices::s_asciiString[] =
{
	XalanUnicode::charLetter_A,
	XalanUnicode::charLetter_S,
	XalanUnicode::charLetter_C, 
	XalanUnicode::charLetter_I,
	XalanUnicode::charLetter_I,
	0
};



const XalanDOMChar 	XalanTranscodingServices::s_usASCIIString[] =
{
	XalanUnicode::charLetter_U,
	XalanUnicode::charLetter_S,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_A,
	XalanUnicode::charLetter_S,
	XalanUnicode::charLetter_C, 
	XalanUnicode::charLetter_I,
	XalanUnicode::charLetter_I,
	0
};



const XalanDOMChar 	XalanTranscodingServices::s_windows1250String[] =
{
	XalanUnicode::charLetter_W,
	XalanUnicode::charLetter_I,
	XalanUnicode::charLetter_N,
	XalanUnicode::charLetter_D,
	XalanUnicode::charLetter_O, 
	XalanUnicode::charLetter_W,
	XalanUnicode::charLetter_S,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charDigit_1,
	XalanUnicode::charDigit_2,
	XalanUnicode::charDigit_5,
	XalanUnicode::charDigit_0,
	0
};



const XalanTranscodingServices::XalanXMLByte	XalanTranscodingServices::s_dummyByteOrderMark[] =
{
	XalanTranscodingServices::XalanXMLByte(0)
};



const XalanTranscodingServices::XalanXMLByte	XalanTranscodingServices::s_UTF8ByteOrderMark[] =
{
	XalanTranscodingServices::XalanXMLByte(0xEF),
	XalanTranscodingServices::XalanXMLByte(0xBB),
	XalanTranscodingServices::XalanXMLByte(0xBF),
	XalanTranscodingServices::XalanXMLByte(0)
};



const XalanTranscodingServices::XalanXMLByte	XalanTranscodingServices::s_UTF16ByteOrderMark[] =
{
#if defined(XALAN_LITLE_ENDIAN)
	XalanTranscodingServices::XalanXMLByte(0xFF),
	XalanTranscodingServices::XalanXMLByte(0xFE),
#elif defined(XALAN_BIG_ENDIAN)
	XalanTranscodingServices::XalanXMLByte(0xFE),
	XalanTranscodingServices::XalanXMLByte(0xFF),
#else
#error The platform must define the byte order!
#endif
	XalanTranscodingServices::XalanXMLByte(0)
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
		assert(theResult == XalanTranscodingServices::OK ||
			   theXercesTranscoder == 0);

		if (theResult == XalanTranscodingServices::OK)
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
XalanTranscodingServices::encodingIsUTF8(const XalanDOMChar*	theEncodingName)
{
	return compareIgnoreCase(c_wstr(theEncodingName), s_utf8String) == 0 ? true : false;
}


bool
XalanTranscodingServices::encodingIsUTF8(const XalanDOMString&		theEncodingName)
{
	return encodingIsUTF8(c_wstr(theEncodingName));
}



bool
XalanTranscodingServices::encodingIsUTF16(const XalanDOMChar*	theEncodingName)
{
	return compareIgnoreCase(c_wstr(theEncodingName), s_utf16String) == 0 ? true : false;
}


bool
XalanTranscodingServices::encodingIsUTF16(const XalanDOMString&		theEncodingName)
{
	return encodingIsUTF16(c_wstr(theEncodingName));
}



const XalanTranscodingServices::XalanXMLByte*
XalanTranscodingServices::getStreamProlog(const XalanDOMString&		theEncodingName)
{
	if (compareIgnoreCase(c_wstr(theEncodingName), s_utf16String) == 0)
	{
		return s_UTF16ByteOrderMark;
	}
#if 0
	// We won't do this for now...
	else if (compareIgnoreCase(c_wstr(theEncodingName), s_utf8String) == 0)
	{
		return s_UTF8ByteOrderMark;
	}
#endif
	else
	{
		return s_dummyByteOrderMark;
	}
}



XalanDOMChar
XalanTranscodingServices::getMaximumCharacterValue(const XalanDOMString&	theEncoding)
{
	const MaximumCharacterValueMapType::const_iterator	i =
		s_maximumCharacterValues.find(toUpperCaseASCII(theEncoding));

	if (i == s_maximumCharacterValues.end())
	{
		return XalanDOMChar(0x7fu);
	}
	else
	{
		return (*i).second;
	}
}



XalanDOMChar
XalanTranscodingServices::getMaximumCharacterValue()
{
	// $$$ ToDo: We need to set this according to the local
	// code page, but how?  Perhaps try to transcode various
	// strings with increasingly larger character values, using
	// DOMStringHelper's TranscodeToLocalCodePage()?  Not exactly
	// what we need, but it may do.
	return XalanDOMChar(0x7fu);
}



bool
XalanTranscodingServices::getBytesEqualChars(const XalanDOMString&	theEncoding)
{
	return equals(theEncoding, s_asciiString) ||
		   equals(theEncoding, s_usASCIIString) ||
		   equals(theEncoding, s_windows1250String);
}



XalanTranscodingServices::UnrepresentableCharacterException::UnrepresentableCharacterException(
			XalanDOMChar			theCharacter,
			const XalanDOMString&	theEncoding) :
	XSLException(TranscodeFromLocalCodePage("Unable to represent a character in the specified encoding"),
				 TranscodeFromLocalCodePage("UnrepresentableCharacterException")),
	m_badCharacter(theCharacter),
	m_encoding(theEncoding)
{
}



XalanTranscodingServices::UnrepresentableCharacterException::~UnrepresentableCharacterException()
{
}



XalanOutputTranscoder::XalanOutputTranscoder()
{
}



XalanOutputTranscoder::~XalanOutputTranscoder()
{
}



static XalanTranscodingServices::MaximumCharacterValueMapType	s_maximumCharacterValues;

const XalanTranscodingServices::MaximumCharacterValueMapType&	XalanTranscodingServices::s_maximumCharacterValues =
			::s_maximumCharacterValues;



static void
initMaximumCharacterValueMap(XalanTranscodingServices::MaximumCharacterValueMapType&	theMap)
{
	typedef XalanTranscodingServices::MaximumCharacterValueMapType::value_type	value_type;

	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("WINDOWS-1250")),	 0xFF)); // Windows 1250 Peter Smolik
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("UTF-8")),			 0xFFFF)); // Universal Transformation Format 8
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("UTF-16")),			 0xFFFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("US-ASCII")),		 0x7F));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1")), 	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-2")), 	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-3")), 	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-4")), 	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-5")), 	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-6")), 	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-7")), 	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-8")), 	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-9")), 	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-JP")),	 0xFFFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("SHIFT_JIS")),		 0xFFFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EUC-JP")), 		 0xFFFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("GB2312")), 		 0xFFFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("BIG5")),			 0xFFFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EUC-KR")), 		 0xFFFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-KR")),	 0xFFFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("KOI8-R")), 		 0xFFFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-US")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CA")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NL")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-DK")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NO")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FI")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-SE")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IT")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ES")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-GB")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FR")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR1")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-HE")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CH")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ROECE")), 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-YU")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IS")),	 0xFF));
	theMap.insert(value_type(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR2")),	 0xFF));
}



void
XalanTranscodingServices::initialize()
{
	initMaximumCharacterValueMap(::s_maximumCharacterValues);
}



void
XalanTranscodingServices::terminate()
{
	MaximumCharacterValueMapType().swap(::s_maximumCharacterValues);
}
