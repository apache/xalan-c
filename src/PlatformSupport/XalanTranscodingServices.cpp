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
#include "XalanTranscodingServices.hpp"



#include <cassert>



#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>



#include "DOMStringHelper.hpp"
#include "XalanToXercesTranscoderWrapper.hpp"
#include "XalanUTF16Transcoder.hpp"



XALAN_CPP_NAMESPACE_BEGIN



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



const XalanDOMChar 	XalanTranscodingServices::s_utf16LEString[] =
{
	XalanUnicode::charLetter_U,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_F, 
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charDigit_1,
	XalanUnicode::charDigit_6,
	XalanUnicode::charLetter_L,
	XalanUnicode::charLetter_E, 
	0
};



const XalanDOMChar 	XalanTranscodingServices::s_utf16BEString[] =
{
	XalanUnicode::charLetter_U,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_F, 
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charDigit_1,
	XalanUnicode::charDigit_6,
	XalanUnicode::charLetter_B,
	XalanUnicode::charLetter_E, 
	0
};



const XalanDOMChar 	XalanTranscodingServices::s_utf32String[] =
{
	XalanUnicode::charLetter_U,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_F, 
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charDigit_3,
	XalanUnicode::charDigit_2,
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



const XalanDOMChar 	XalanTranscodingServices::s_iso88591String[] =
{
	XalanUnicode::charLetter_I,
	XalanUnicode::charLetter_S,
	XalanUnicode::charLetter_O,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charDigit_8,
	XalanUnicode::charDigit_8,
	XalanUnicode::charDigit_5,
	XalanUnicode::charDigit_9,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charDigit_1,
	0
};



const XalanDOMChar	XalanTranscodingServices::s_shiftJISString[] =
{
	XalanUnicode::charLetter_S,
	XalanUnicode::charLetter_H,
	XalanUnicode::charLetter_I,
	XalanUnicode::charLetter_F,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLowLine,
	XalanUnicode::charLetter_J,
	XalanUnicode::charLetter_I,
	XalanUnicode::charLetter_S,
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



const XalanDOMChar	XalanTranscodingServices::s_UTF16ByteOrderMark[] =
{
	XalanDOMChar(0xFEFF),
	XalanTranscodingServices::XalanXMLByte(0)
};




XALAN_USING(xercesc, XMLTransService)


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
			size_t					theBlockSize)
{
	XALAN_USING_XERCES(XMLPlatformUtils)

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
		XALAN_USING_XERCES(XMLTranscoder)

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
			theTranscoder = new XalanToXercesTranscoderWrapper(*theXercesTranscoder);
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
	return compareIgnoreCaseASCII(c_wstr(theEncodingName), s_utf8String) == 0 ? true : false;
}


bool
XalanTranscodingServices::encodingIsUTF8(const XalanDOMString&	theEncodingName)
{
	return encodingIsUTF8(c_wstr(theEncodingName));
}



bool
XalanTranscodingServices::encodingIsUTF16(const XalanDOMChar*	theEncodingName)
{
	return compareIgnoreCaseASCII(c_wstr(theEncodingName), s_utf16String) == 0 ||
		   compareIgnoreCaseASCII(c_wstr(theEncodingName), s_utf16LEString) == 0 ||
		   compareIgnoreCaseASCII(c_wstr(theEncodingName), s_utf16BEString) == 0 ? true : false;
}


bool
XalanTranscodingServices::encodingIsUTF16(const XalanDOMString&		theEncodingName)
{
	return encodingIsUTF16(c_wstr(theEncodingName));
}



bool
XalanTranscodingServices::encodingIsUTF32(const XalanDOMChar*	theEncodingName)
{
	return compareIgnoreCaseASCII(c_wstr(theEncodingName), s_utf32String) == 0 ? true : false;
}


bool
XalanTranscodingServices::encodingIsUTF32(const XalanDOMString&		theEncodingName)
{
	return encodingIsUTF32(c_wstr(theEncodingName));
}



const XalanTranscodingServices::XalanXMLByte*
XalanTranscodingServices::getStreamProlog(const XalanDOMString&		theEncodingName)
{
	if (compareIgnoreCaseASCII(c_wstr(theEncodingName), s_utf16String) == 0)
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		return (const XalanXMLByte*)s_UTF16ByteOrderMark;
#else
		return reinterpret_cast<const XalanXMLByte*>(s_UTF16ByteOrderMark);
#endif
	}
#if 0
	// We won't do this for now...
	else if (compareIgnoreCaseASCII(c_wstr(theEncodingName), s_utf8String) == 0)
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
	if (compareIgnoreCaseASCII(c_wstr(theEncoding), s_utf8String) == 0 ||
		compareIgnoreCaseASCII(c_wstr(theEncoding), s_utf16String) == 0)
	{
		return XalanDOMChar(0xFFFFu);
	}
	else if (compareIgnoreCaseASCII(c_wstr(theEncoding), s_iso88591String) == 0)
	{
		return XalanDOMChar(0x00FFu);
	}
	else if (compareIgnoreCaseASCII(c_wstr(theEncoding), s_utf16LEString) == 0 ||
		compareIgnoreCaseASCII(c_wstr(theEncoding), s_utf16BEString) == 0 ||
		compareIgnoreCaseASCII(c_wstr(theEncoding), s_utf32String) == 0 ||
		compareIgnoreCaseASCII(c_wstr(theEncoding), s_shiftJISString) == 0)
	{
		return XalanDOMChar(0xFFFFu);
	}
	else
	{
		return XalanDOMChar(0x007fu);
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



void
XalanTranscodingServices::initialize()
{
}



void
XalanTranscodingServices::terminate()
{
}



XALAN_CPP_NAMESPACE_END
