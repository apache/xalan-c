/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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



#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <unicode/coll.h>



XALAN_CPP_NAMESPACE_BEGIN



typedef XalanVector<UChar>	UCharVectorType;



#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
inline void
doCopyData(
			const XalanDOMChar*		    theString,
			XalanDOMString::size_type   theStringLength,
			XalanDOMChar*			    theBuffer)
{
	// Copy the data, truncating each character...
	for (XalanDOMString::size_type i = 0; i < theStringLength; ++i)
	{
		// There should be no truncation, since XalanDOMChars
		// hold UTF-16 code points, but assert, just in case...
		assert(theString[i] == UChar(theString[i]));

		theBuffer[i] = theString[i];
	}

}
#endif



// Use a stack-based buffer up to this size.
const XalanSize_t	theStackBufferSize = 200u;



const UnicodeString
ICUBridge::XalanDOMCharStringToUnicodeString(
            MemoryManagerType&      theManager,
            const XalanDOMChar*	    theString)
{
	if (theString == 0)
	{
		return UnicodeString();
	}
	else
	{
		return XalanDOMCharStringToUnicodeString(theManager, theString, length(theString));
	}
}


#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)

const UnicodeString
ICUBridge::XalanDOMCharStringToUnicodeString(
            MemoryManagerType&          theManager,
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength)
#else
const UnicodeString
ICUBridge::XalanDOMCharStringToUnicodeString(
            MemoryManagerType&        /*  theManager */,
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength)
#endif
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
			UCharVectorType		theBuffer(theManager);

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
ICUBridge::XalanDOMStringToUnicodeString(
            MemoryManagerType&      theManager,
            const XalanDOMString&   theString)
{
	// Just call up to the XalanDOMChar* version...
	return XalanDOMCharStringToUnicodeString(theManager, c_wstr(theString), length(theString));
}



XalanDOMString&
ICUBridge::UnicodeStringToXalanDOMString(
            const UnicodeString&	theString,
            XalanDOMString&         theResult)
{
	const int32_t	theLength = theString.length();

#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)

	// If XalanDOMChar is larger than the ICU's UChar, we have to more work...
	// Create a buffer...
    XalanDOMCharVectorType	theBuffer(theResult.getMemoryManager());

	// Reserve the appropriate amount of space...
	theBuffer.reserve(theLength);

	// Copy the data...
	for (int32_t i = 0; i < theLength; ++i)
	{
		theBuffer.push_back(theString[i]);
	}

    theResult.assign(&theBuffer[0], theBuffer.size());

	return theResult;

#else

	if (theStackBufferSize > theLength)
	{
		UChar	theBuffer[theStackBufferSize];

		// Extract the data...
		theString.extract(0, theLength, theBuffer);

        theResult.assign(theBuffer, theLength);

		return theResult;
	}
	else
	{
		// Create a buffer to copy out the UnicodeString data...
        UCharVectorType		theBuffer(theResult.getMemoryManager());

		// Resize the buffer appropriately...
		theBuffer.resize(theLength);

		// Extract the data...
		theString.extract(0, theLength, &theBuffer[0]);

		assert(theLength == int32_t(theBuffer.size()));

        theResult.assign(&theBuffer[0], theLength);

		return theResult;
	}
#endif
}



XALAN_CPP_NAMESPACE_END
