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
#if !defined(XALANUNICODE_HEADER_GUARD_1357924680)
#define XALANUNICODE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



XALAN_CPP_NAMESPACE_BEGIN



#if defined(XALAN_HAS_CPP_NAMESPACE)
namespace XalanUnicode
#else
struct XALAN_PLATFORMSUPPORT_EXPORT XalanUnicode
#endif
{
#if !defined(XALAN_HAS_CPP_NAMESPACE)

	enum
	{
		charHTab				= 0x09,
		charLF					= 0x0A,
		charFF					= 0x0C,
		charCR					= 0x0D,
		charSpace				= 0x20,

		charExclamationMark		= 0x21,

		charQuoteMark			= 0x22,

		charNumberSign			= 0x23,

		charDollarSign			= 0x24,

		charPercentSign			= 0x25,

		charAmpersand			= 0x26,

		charApostrophe			= 0x27,

		charLeftParenthesis		= 0x28,
		charRightParenthesis	= 0x29,

		charAsterisk			= 0x2A,

		charPlusSign			= 0x2B,

		charComma				= 0x2C,

		charHyphenMinus			= 0x2D,

		charFullStop			= 0x2E,

		charSolidus				= 0x2F,

		charDigit_0				= 0x30,
		charDigit_1				= 0x31,
		charDigit_2				= 0x32,
		charDigit_3				= 0x33,
		charDigit_4				= 0x34,
		charDigit_5				= 0x35,
		charDigit_6				= 0x36,
		charDigit_7				= 0x37,
		charDigit_8				= 0x38,
		charDigit_9				= 0x39,

		charColon				= 0x3A,
		charSemicolon			= 0x3B,

		charLessThanSign		= 0x3C,

		charEqualsSign			= 0x3D,

		charGreaterThanSign		= 0x3E,

		charQuestionMark		= 0x3F,

		charCommercialAt		= 0x40,

		charLetter_A			= 0x41,
		charLetter_B			= 0x42,
		charLetter_C			= 0x43,
		charLetter_D			= 0x44,
		charLetter_E			= 0x45,
		charLetter_F			= 0x46,
		charLetter_G			= 0x47,
		charLetter_H			= 0x48,
		charLetter_I			= 0x49,
		charLetter_J			= 0x4A,
		charLetter_K			= 0x4B,
		charLetter_L			= 0x4C,
		charLetter_M			= 0x4D,
		charLetter_N			= 0x4E,
		charLetter_O			= 0x4F,
		charLetter_P			= 0x50,
		charLetter_Q			= 0x51,
		charLetter_R			= 0x52,
		charLetter_S			= 0x53,
		charLetter_T			= 0x54,
		charLetter_U			= 0x55,
		charLetter_V			= 0x56,
		charLetter_W			= 0x57,
		charLetter_X			= 0x58,
		charLetter_Y			= 0x59,
		charLetter_Z			= 0x5A,

		charLeftSquareBracket	= 0x5B,

		charReverseSolidus		= 0x5C,

		charRightSquareBracket	= 0x5D,

		charCircumflexAccent	= 0x5E,

		charLowLine				= 0x5F,
		charGraveAccent			= 0x60,

		charLetter_a			= 0x61,
		charLetter_b			= 0x62,
		charLetter_c			= 0x63,
		charLetter_d			= 0x64,
		charLetter_e			= 0x65,
		charLetter_f			= 0x66,
		charLetter_g			= 0x67,
		charLetter_h			= 0x68,
		charLetter_i			= 0x69,
		charLetter_j			= 0x6A,
		charLetter_k			= 0x6B,
		charLetter_l			= 0x6C,
		charLetter_m			= 0x6D,
		charLetter_n			= 0x6E,
		charLetter_o			= 0x6F,
		charLetter_p			= 0x70,
		charLetter_q			= 0x71,
		charLetter_r			= 0x72,
		charLetter_s			= 0x73,
		charLetter_t			= 0x74,
		charLetter_u			= 0x75,
		charLetter_v			= 0x76,
		charLetter_w			= 0x77,
		charLetter_x			= 0x78,
		charLetter_y			= 0x79,
		charLetter_z			= 0x7A,

		charLeftCurlyBracket	= 0x7B,

		charVerticalLine		= 0x7C,

		charRightCurlyBracket	= 0x7D,

        charNEL                 = 0x85,

        charLSEP                = 0x2028,

		charPerMilleSign		= 0x2030
	};

#else

	static const XalanDOMChar	charHTab				= 0x09;
	static const XalanDOMChar	charLF					= 0x0A;
	static const XalanDOMChar	charCR					= 0x0D;
	static const XalanDOMChar	charFF					= 0x0C;
	static const XalanDOMChar	charSpace				= 0x20;

	static const XalanDOMChar	charExclamationMark		= 0x21;

	static const XalanDOMChar	charQuoteMark			= 0x22;

	static const XalanDOMChar	charNumberSign			= 0x23;

	static const XalanDOMChar	charDollarSign			= 0x24;

	static const XalanDOMChar	charPercentSign			= 0x25;

	static const XalanDOMChar	charAmpersand			= 0x26;

	static const XalanDOMChar	charApostrophe			= 0x27;

	static const XalanDOMChar	charLeftParenthesis		= 0x28;
	static const XalanDOMChar	charRightParenthesis	= 0x29;

	static const XalanDOMChar	charAsterisk			= 0x2A;

	static const XalanDOMChar	charPlusSign			= 0x2B;

	static const XalanDOMChar	charComma				= 0x2C;

	static const XalanDOMChar	charHyphenMinus			= 0x2D;

	static const XalanDOMChar	charFullStop			= 0x2E;

	static const XalanDOMChar	charSolidus				= 0x2F;

	static const XalanDOMChar	charDigit_0				= 0x30;
	static const XalanDOMChar	charDigit_1				= 0x31;
	static const XalanDOMChar	charDigit_2				= 0x32;
	static const XalanDOMChar	charDigit_3				= 0x33;
	static const XalanDOMChar	charDigit_4				= 0x34;
	static const XalanDOMChar	charDigit_5				= 0x35;
	static const XalanDOMChar	charDigit_6				= 0x36;
	static const XalanDOMChar	charDigit_7				= 0x37;
	static const XalanDOMChar	charDigit_8				= 0x38;
	static const XalanDOMChar	charDigit_9				= 0x39;

	static const XalanDOMChar	charColon				= 0x3A;
	static const XalanDOMChar	charSemicolon			= 0x3B;

	static const XalanDOMChar	charLessThanSign		= 0x3C;

	static const XalanDOMChar	charEqualsSign			= 0x3D;

	static const XalanDOMChar	charGreaterThanSign		= 0x3E;

	static const XalanDOMChar	charQuestionMark		= 0x3F;

	static const XalanDOMChar	charCommercialAt		= 0x40;

	static const XalanDOMChar	charLetter_A			= 0x41;
	static const XalanDOMChar	charLetter_B			= 0x42;
	static const XalanDOMChar	charLetter_C			= 0x43;
	static const XalanDOMChar	charLetter_D			= 0x44;
	static const XalanDOMChar	charLetter_E			= 0x45;
	static const XalanDOMChar	charLetter_F			= 0x46;
	static const XalanDOMChar	charLetter_G			= 0x47;
	static const XalanDOMChar	charLetter_H			= 0x48;
	static const XalanDOMChar	charLetter_I			= 0x49;
	static const XalanDOMChar	charLetter_J			= 0x4A;
	static const XalanDOMChar	charLetter_K			= 0x4B;
	static const XalanDOMChar	charLetter_L			= 0x4C;
	static const XalanDOMChar	charLetter_M			= 0x4D;
	static const XalanDOMChar	charLetter_N			= 0x4E;
	static const XalanDOMChar	charLetter_O			= 0x4F;
	static const XalanDOMChar	charLetter_P			= 0x50;
	static const XalanDOMChar	charLetter_Q			= 0x51;
	static const XalanDOMChar	charLetter_R			= 0x52;
	static const XalanDOMChar	charLetter_S			= 0x53;
	static const XalanDOMChar	charLetter_T			= 0x54;
	static const XalanDOMChar	charLetter_U			= 0x55;
	static const XalanDOMChar	charLetter_V			= 0x56;
	static const XalanDOMChar	charLetter_W			= 0x57;
	static const XalanDOMChar	charLetter_X			= 0x58;
	static const XalanDOMChar	charLetter_Y			= 0x59;
	static const XalanDOMChar	charLetter_Z			= 0x5A;

	static const XalanDOMChar	charLeftSquareBracket	= 0x5B;

	static const XalanDOMChar	charReverseSolidus		= 0x5C;

	static const XalanDOMChar	charRightSquareBracket	= 0x5D;

	static const XalanDOMChar	charCircumflexAccent	= 0x5E;

	static const XalanDOMChar	charLowLine				= 0x5F;
	static const XalanDOMChar   charGraveAccent			= 0x60;

	static const XalanDOMChar	charLetter_a			= 0x61;
	static const XalanDOMChar	charLetter_b			= 0x62;
	static const XalanDOMChar	charLetter_c			= 0x63;
	static const XalanDOMChar	charLetter_d			= 0x64;
	static const XalanDOMChar	charLetter_e			= 0x65;
	static const XalanDOMChar	charLetter_f			= 0x66;
	static const XalanDOMChar	charLetter_g			= 0x67;
	static const XalanDOMChar	charLetter_h			= 0x68;
	static const XalanDOMChar	charLetter_i			= 0x69;
	static const XalanDOMChar	charLetter_j			= 0x6A;
	static const XalanDOMChar	charLetter_k			= 0x6B;
	static const XalanDOMChar	charLetter_l			= 0x6C;
	static const XalanDOMChar	charLetter_m			= 0x6D;
	static const XalanDOMChar	charLetter_n			= 0x6E;
	static const XalanDOMChar	charLetter_o			= 0x6F;
	static const XalanDOMChar	charLetter_p			= 0x70;
	static const XalanDOMChar	charLetter_q			= 0x71;
	static const XalanDOMChar	charLetter_r			= 0x72;
	static const XalanDOMChar	charLetter_s			= 0x73;
	static const XalanDOMChar	charLetter_t			= 0x74;
	static const XalanDOMChar	charLetter_u			= 0x75;
	static const XalanDOMChar	charLetter_v			= 0x76;
	static const XalanDOMChar	charLetter_w			= 0x77;
	static const XalanDOMChar	charLetter_x			= 0x78;
	static const XalanDOMChar	charLetter_y			= 0x79;
	static const XalanDOMChar	charLetter_z			= 0x7A;

	static const XalanDOMChar	charLeftCurlyBracket	= 0x7B;

	static const XalanDOMChar	charVerticalLine		= 0x7C;

	static const XalanDOMChar	charRightCurlyBracket	= 0x7D;

	static const XalanDOMChar	charNEL                 = 0x85;

	static const XalanDOMChar	charLSEP                = 0x2028;

	static const XalanDOMChar	charPerMilleSign		= 0x2030;
#endif



#if !defined(XALAN_HAS_CPP_NAMESPACE)

private:

	// Not implemented...
	XalanUnicode();

	XalanUnicode(const XalanUnicode&);

	~XalanUnicode();

	XalanUnicode&
	operator=(const XalanUnicode&);

};
#else
}
#endif



XALAN_CPP_NAMESPACE_END



#endif	// XALANUNICODE_HEADER_GUARD_1357924680
