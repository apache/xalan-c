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
#include "XalanTranscodingServices.hpp"



#include <cassert>



#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>



#include "DOMStringHelper.hpp"
#include "XalanMessageLoader.hpp"
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
	XalanDOMChar(0)
};




XALAN_USING_XERCES(XMLTransService)


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
			size_type				theBlockSize)
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
// A glitch in Xerces 2.3 omits the default parameter, so
// we have to provide one.
#if XERCES_VERSION_MAJOR == 2 && XERCES_VERSION_MINOR == 3
					theBlockSize,
					XMLPlatformUtils::fgMemoryManager);
#else
					theBlockSize);
#endif

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
			UnicodeCharType			theCharacter,
			const XalanDOMString&	theEncoding) :
	XSLException(
		XalanMessageLoader::getMessage(
			XalanMessages::UnrepresentableCharacter_2Param,
			UnsignedLongToHexDOMString(theCharacter),
			theEncoding),
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
