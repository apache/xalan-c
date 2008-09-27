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

#include "XPathConstructionContextDefault.hpp"



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include "xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp"
#include "xalanc/PlatformSupport/XalanStdOutputStream.hpp"



#include "XPathParserException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XPathConstructionContextDefault::XPathConstructionContextDefault(MemoryManager&     theManager) :
    XPathConstructionContext(theManager),
    m_stringPool(theManager),
    m_stringCache(theManager)
{
}


    
XPathConstructionContextDefault*
XPathConstructionContextDefault::create(MemoryManager&  theManager)
{
    typedef XPathConstructionContextDefault ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* theResult = new (theGuard.get()) ThisType(theManager);

    theGuard.release();

    return theResult;
}



XPathConstructionContextDefault::~XPathConstructionContextDefault()
{
    reset();
}



void
XPathConstructionContextDefault::reset()
{
    m_stringCache.reset();
}



const XalanDOMString&
XPathConstructionContextDefault::getPooledString(const XalanDOMString&  theString)
{
    return m_stringPool.get(theString);
}



const XalanDOMString&
XPathConstructionContextDefault::getPooledString(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength)
{
    return m_stringPool.get(theString, theLength);
}



XalanDOMString&
XPathConstructionContextDefault::getCachedString()
{
    return m_stringCache.get();
}



bool
XPathConstructionContextDefault::releaseCachedString(XalanDOMString&    theString)
{
    return m_stringCache.release(theString);
}



void
XPathConstructionContextDefault::problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode)
{
    MemoryManager&  theManager = getMemoryManager();

    XALAN_USING_STD(cerr)

    XalanStdOutputStream            theStream(cerr, theManager);
    XalanOutputStreamPrintWriter    thePrintWriter(theStream);

    defaultFormat(
        thePrintWriter,
        source,
        classification,
        msg,
        locator,
        sourceNode);

    if (classification == eError)
    {
        throw XPathParserException(msg, theManager, locator);
    }
}



void
XPathConstructionContextDefault::problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode)
{
    MemoryManager&  theManager = getMemoryManager();

    XALAN_USING_STD(cerr)

    XalanStdOutputStream            theStream(cerr, theManager);
    XalanOutputStreamPrintWriter    thePrintWriter(theStream);

    defaultFormat(
        thePrintWriter,
        source,
        classification,
        msg,
        sourceNode);

    if (classification == eError)
    {
        throw XPathParserException(msg, theManager);
    }
}



XALAN_CPP_NAMESPACE_END
