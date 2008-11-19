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
#include "XalanUTF16Transcoder.hpp"

#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>


#include <cassert>



XALAN_CPP_NAMESPACE_BEGIN



XalanUTF16Transcoder::XalanUTF16Transcoder(MemoryManager&   theManager) :
    XalanOutputTranscoder(theManager)
{
}



XalanUTF16Transcoder*
XalanUTF16Transcoder::create(MemoryManager& theManager)
{
    typedef XalanUTF16Transcoder    ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(theManager);

    theGuard.release();

    return theResult;
}



XalanUTF16Transcoder::~XalanUTF16Transcoder()
{
}



XalanUTF16Transcoder::eCode
XalanUTF16Transcoder::transcode(
            const XalanDOMChar*     theSourceData,
            size_type               theSourceCount,
            XalanXMLByte*           theTarget,
            size_type               theTargetSize,
            size_type&              theSourceCharsTranscoded,
            size_type&              theTargetBytesUsed)
{
    size_type   theSourceEaten = 0;
    size_type   theTargetPosition = 0;

    while(theSourceEaten < theSourceCount)
    {
        // Swap bytes to big endian...
        if (theTargetPosition + 1 >= theTargetSize)
        {
            break;
        }
        else
        {
            *reinterpret_cast<XalanDOMChar*>(theTarget + theTargetPosition) = theSourceData[theSourceEaten++];

            theTargetPosition += 2;
        }
    }

    theSourceCharsTranscoded = theSourceEaten;

    theTargetBytesUsed = theTargetPosition;

    return XalanTranscodingServices::OK;
}



XalanUTF16Transcoder::eCode
XalanUTF16Transcoder::transcode(
            const XalanXMLByte*     theSourceData,
            size_type               theSourceCount,
            XalanDOMChar*           theTarget,
            size_type               theTargetSize,
            size_type&              theSourceCharsTranscoded,
            size_type&              theTargetBytesUsed,
            unsigned char*          theCharSizes)
{
    size_type   theSourceEaten = 0;
    size_type   theTargetPosition = 0;

    while(theSourceEaten + 1 < theSourceCount)
    {
        // Swap bytes to big endian...
        if (theTargetPosition + 1 >= theTargetSize)
        {
            break;
        }
        else
        {
            theTarget[theTargetPosition++] = *reinterpret_cast<const XalanDOMChar*>(theSourceData + theSourceCount++);

            *theCharSizes++ = 2;
        }
    }

    theSourceCharsTranscoded = theSourceEaten;

    theTargetBytesUsed = theTargetPosition;

    return XalanTranscodingServices::OK;
}



bool
XalanUTF16Transcoder::canTranscodeTo(XalanUnicodeChar   /* theChar */) const
{
    return true;
}



XALAN_CPP_NAMESPACE_END
