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

#include "ICUBridge.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <unicode/coll.h>



XALAN_CPP_NAMESPACE_BEGIN



#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<UChar>					UCharVectorType;
#else
	typedef std::vector<UChar>				UCharVectorType;
#endif



#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
inline void
doCopyData(
			const XalanDOMChar*		theString,
			unsigned int			theStringLength,
			XalanDOMChar*			theBuffer)
{
	// Copy the data, truncating each character...
	for (unsigned int i = 0; i < theStringLength; ++i)
	{
		// There should be no truncation, since XalanDOMChars
		// hold UTF-16 code points, but assert, just in case...
		assert(theString[i] == UChar(theString[i]));

		theBuffer[i] = theString[i];
	}

}
#endif



// Use a stack-based buffer up to this size.
const unsigned int	theStackBufferSize = 200u;



const UnicodeString
ICUBridge::XalanDOMCharStringToUnicodeString(const XalanDOMChar*	theString)
{
	if (theString == 0)
	{
		return UnicodeString();
	}
	else
	{
		return XalanDOMCharStringToUnicodeString(theString, length(theString));
	}
}



const UnicodeString
ICUBridge::XalanDOMCharStringToUnicodeString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength)
{
	assert(theString != 0);

	if (theLength == 0)
	{
		return UnicodeString();
	}
	else
	{
#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)

		if (theStackBufferSize > theLength)
		{
			XalanDOMChar	theBuffer[theStackBufferSize];

			doCopyData(theString, theLength, theBuffer);

#if U_SIZEOF_WCHAR_T==2
			return UnicodeString((wchar_t*)&theBuffer[0], theLength);
#else
			return UnicodeString(&theBuffer[0], theLength);
#endif
		}
		else
		{
			// Create a buffer to copy out the UnicodeString data...
			UCharVectorType		theBuffer;

			// Resize the buffer appropriately...
			theBuffer.resize(theLength);		

#if U_SIZEOF_WCHAR_T==2
			doCopyData(theString, theLength, (XalanDOMChar*)&theBuffer[0]);
#else
			doCopyData(theString, theLength, &theBuffer[0]);
#endif

			assert(theLength == theBuffer.size());

			return UnicodeString(&theBuffer[0], theLength);
		}
#else
		return UnicodeString(theString, length(theString));
#endif
	}
}



const UnicodeString
ICUBridge::XalanDOMStringToUnicodeString(const XalanDOMString&	theString)
{
	// Just call up to the XalanDOMChar* version...
	return XalanDOMCharStringToUnicodeString(c_wstr(theString), length(theString));
}



const XalanDOMString
ICUBridge::UnicodeStringToXalanDOMString(const UnicodeString&	theString)
{
	const int32_t	theLength = theString.length();

#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)

	// If XalanDOMChar is larger than the ICU's UChar, we have to more work...
	// Create a buffer...
	XalanDOMCharVectorType	theBuffer;

	// Reserve the appropriate amount of space...
	theBuffer.reserve(theLength);

	// Copy the data...
	for (int32_t i = 0; i < theLength; ++i)
	{
		theBuffer.push_back(theString[i]);
	}

	return XalanDOMString(&theBuffer[0], theBuffer.size());

#else

	if (theStackBufferSize > theLength)
	{
		UChar	theBuffer[theStackBufferSize];

		// Extract the data...
		theString.extract(0, theLength, theBuffer);

		return XalanDOMString(theBuffer, theLength);
	}
	else
	{
		// Create a buffer to copy out the UnicodeString data...
		UCharVectorType		theBuffer;

		// Resize the buffer appropriately...
		theBuffer.resize(theLength);

		// Extract the data...
		theString.extract(0, theLength, &theBuffer[0]);

		assert(theLength == int32_t(theBuffer.size()));

		return XalanDOMString(&theBuffer[0], theLength);
	}
#endif
}



void
ICUBridge::UnicodeStringToXalanDOMString(
			const UnicodeString&	theString,
			XalanDOMString&			theResult)
{
#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
	
	// If XalanDOMChar is larger than the ICU's UChar, we have to more work.
	// Don't bother to provide the optimized version, just call to the
	// previous function.

	theResult = UnicodeStringToXalanDOMString(theString);

#else

	const int32_t	theLength = theString.length();

	if (theStackBufferSize > theLength)
	{
		UChar	theBuffer[theStackBufferSize];

		// Extract the data...
		theString.extract(0, theLength, theBuffer);

		theResult = XalanDOMString(theBuffer, theLength);
	}
	else
	{
#if defined(XALAN_NO_NAMESPACES)
		typedef vector<UChar>		UCharVectorType;
#else
		typedef std::vector<UChar>	UCharVectorType;
#endif

		// Create a buffer to copy out the UnicodeString data...
		UCharVectorType		theBuffer;

		// Resize the buffer appropriately...
		theBuffer.resize(theLength);

		// Extract the data...
		theString.extract(0, theLength, &theBuffer[0]);

		theResult = XalanDOMString(&theBuffer[0], theBuffer.size());
	}
#endif
}

XALAN_CPP_NAMESPACE_END
