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
#if !defined(XALANXMLCHAR_HEADER_GUARD_1357924680)
#define XALANXMLCHAR_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



XALAN_CPP_NAMESPACE_BEGIN



struct XALAN_PLATFORMSUPPORT_EXPORT XalanXMLChar
{

public:

	enum eType
	{
		XML_XX = 0,
		XML_BC = 1,
		XML_ID = 2,
		XML_EX = 3,
		XML_DI = 4,
		XML_CC = 5,
		XML_WS = 6
	};

    static const char theUnicodeTable[];

	static bool 
	isBaseChar(XalanDOMChar c)
	{
		return theUnicodeTable[c] == char(XML_BC);
	}

	static bool 
	isIdeographic(XalanDOMChar c)
	{
		return theUnicodeTable[c] == char(XML_ID);
	}

	static bool 
	isExtender(XalanDOMChar c) 
	{
		return theUnicodeTable[c] == char(XML_EX);
	}

	static bool 
	isDigit(XalanDOMChar c) 
	{
		return theUnicodeTable[c] == char(XML_DI);
	}

	static bool 
	isCombiningChar(XalanDOMChar c) 
	{
		return theUnicodeTable[c] == char(XML_CC);
	}

	static bool 
	isWhitespace(XalanDOMChar c) 
	{
		return theUnicodeTable[c] == char(XML_WS);
	}

	static bool 
	isLetter(XalanDOMChar c) 
	{
		return theUnicodeTable[c] == char(XML_BC) || theUnicodeTable[c] == char(XML_ID);
	}
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANXMLCHAR_HEADER_GUARD_1357924680

