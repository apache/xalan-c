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

#if !defined(ICUBRIDGE_HEADER_GUARD_1357924680)
#define ICUBRIDGE_HEADER_GUARD_1357924680



#include <xalanc/ICUBridge/ICUBridgeDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <unicode/unistr.h>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_ICUBRIDGE_EXPORT ICUBridge
{
public:

	static const UnicodeString
	XalanDOMCharStringToUnicodeString(const XalanDOMChar*	theString);

	static const UnicodeString
	XalanDOMCharStringToUnicodeString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLHSLength);

	static const UnicodeString
	XalanDOMStringToUnicodeString(const XalanDOMString&		theString);

	static const XalanDOMString
	UnicodeStringToXalanDOMString(const UnicodeString&	theString);

	static void
	UnicodeStringToXalanDOMString(
			const UnicodeString&	theString,
			XalanDOMString&			theResult);
};



XALAN_CPP_NAMESPACE_END



#endif	// ICUBRIDGE_HEADER_GUARD_1357924680
