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



typedef XalanVector<UChar>  UCharVectorType;



// Use a stack-based buffer up to this size.
const XalanSize_t   theStackBufferSize = 200u;



const UnicodeString
ICUBridge::XalanDOMCharStringToUnicodeString(
            MemoryManager&      theManager,
            const XalanDOMChar*     theString)
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


const UnicodeString
ICUBridge::XalanDOMCharStringToUnicodeString(
            MemoryManager&        /*  theManager */,
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength)
{
    assert(theString != 0);

    if (theLength == 0)
    {
        return UnicodeString();
    }
    else
    {
        assert(theLength == static_cast<int32_t>(theLength));

        return UnicodeString(
                    theString,
                    static_cast<int32_t>(theLength));
    }
}



const UnicodeString
ICUBridge::XalanDOMStringToUnicodeString(
            MemoryManager&      theManager,
            const XalanDOMString&   theString)
{
    // Just call up to the XalanDOMChar* version...
    return XalanDOMCharStringToUnicodeString(theManager, theString.c_str(), theString.length());
}



XalanDOMString&
ICUBridge::UnicodeStringToXalanDOMString(
            const UnicodeString&    theString,
            XalanDOMString&         theResult)
{
    const int32_t   theLength = theString.length();

    if (theStackBufferSize > theLength)
    {
        UChar   theBuffer[theStackBufferSize];

        // Extract the data...
        theString.extract(0, theLength, theBuffer);

        theResult.assign(theBuffer, theLength);

        return theResult;
    }
    else
    {
        // Create a buffer to copy out the UnicodeString data...
        UCharVectorType     theBuffer(theResult.getMemoryManager());

        // Resize the buffer appropriately...
        theBuffer.resize(theLength);

        // Extract the data...
        theString.extract(0, theLength, &theBuffer[0]);

        assert(theLength == int32_t(theBuffer.size()));

        theResult.assign(&theBuffer[0], theLength);

        return theResult;
    }
}



XALAN_CPP_NAMESPACE_END
