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


// Class header file.
#include "FormatterToXMLUnicode.hpp"


// temprorary
#include "XalanUTF8Writer.hpp"

XALAN_CPP_NAMESPACE_BEGIN





#if 0
XalanDOMChar	s_specialChars[kSpecialsSize] =
{
	kNotSpecial,		// 0
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kAttributeSpecial,	// 0x9 -- horizontal tab.  Write as a numeric character reference in attribute values.
	kBothSpecial,		// 0xA -- linefeed  Normalize as requested, and write as a numeric character reference in attribute values.
	kBothSpecial,
	kBothSpecial,
	kAttributeSpecial,	// 0xD -- carriage return.  Write as a numeric character reference in attribute values.
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,		// 0x10
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kBothSpecial,
	kNotSpecial,		// 0x20
	kNotSpecial,
	kAttributeSpecial,	// 0x22 '"'
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kBothSpecial,		// 0x26 -- '&'
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,		// 0x30
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kNotSpecial,
	kBothSpecial,		// 0x3C '<'
	kNotSpecial,
	kBothSpecial		// 0x3E '>'
};
#endif


XALAN_CPP_NAMESPACE_END
