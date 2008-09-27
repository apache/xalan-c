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

#include "XSLTInputSource.hpp"



#include <cassert>

#if !defined(XALAN_CLASSIC_IOSTREAMS)
#include <istream>
#endif



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/StdBinInputStream.hpp>
#include <xalanc/PlatformSupport/URISupport.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XSLTInputSource::XSLTInputSource(MemoryManager&     theMemoryManager) :
    InputSource(&theMemoryManager),
    m_stream(0),
    m_node(0)
{
}



// $$$ ToDo:  Xerces' InputSource class does not yet have a copy
// constructor or assignment operator.  See bug #7944.
XSLTInputSource::XSLTInputSource(
            const XSLTInputSource&  theSource,
            MemoryManager&          theMemoryManager) :
    InputSource(&theMemoryManager),
    m_stream(theSource.m_stream),
    m_node(theSource.m_node)
{
    setIssueFatalErrorIfNotFound(theSource.getIssueFatalErrorIfNotFound());

    const XMLCh*    theValue = theSource.getSystemId();

    if (theValue != 0)
    {
        setSystemId(theValue);
    }

    theValue = theSource.getPublicId();

    if (theValue != 0)
    {
        setPublicId(theValue);
    }

    theValue = theSource.getEncoding();

    if (theValue != 0)
    {
        setEncoding(theValue);
    }
}



XSLTInputSource&
XSLTInputSource::operator=(const XSLTInputSource&   theRHS)
{
    if (this != &theRHS)
    {
        m_stream = theRHS.m_stream;
        m_node = theRHS.m_node;

        setIssueFatalErrorIfNotFound(theRHS.getIssueFatalErrorIfNotFound());

        const XMLCh*    theValue = theRHS.getSystemId();

        if (theValue != 0)
        {
            setSystemId(theValue);
        }

        theValue = theRHS.getPublicId();

        if (theValue != 0)
        {
            setPublicId(theValue);
        }

        theValue = theRHS.getEncoding();

        if (theValue != 0)
        {
            setEncoding(theValue);
        }

    }

    return *this;
}



XSLTInputSource::XSLTInputSource(
            const XMLCh*    systemId,
            MemoryManager&  theMemoryManager) :
    InputSource(
        systemId,
        &theMemoryManager),
    m_stream(0),
    m_node(0)
{
}



XSLTInputSource::XSLTInputSource(
            const XalanDOMString&   systemId,
            MemoryManager&          theMemoryManager) :
    InputSource(
        systemId.c_str(),
        &theMemoryManager),
    m_stream(0),
    m_node(0)
{
}



XSLTInputSource::XSLTInputSource(
            const XMLCh*    systemId,
            const XMLCh*    publicId,
            MemoryManager&  theMemoryManager) :
    InputSource(
        systemId,
        publicId,
        &theMemoryManager),
    m_stream(0),
    m_node(0)
{
}



XSLTInputSource::XSLTInputSource(
            const XalanDOMString&   systemId,
            const XalanDOMString&   publicId,
            MemoryManager&          theMemoryManager) :
    InputSource(
        systemId.c_str(),
        publicId.c_str(),
        &theMemoryManager),
    m_stream(0),
    m_node(0)
{
}



XSLTInputSource::XSLTInputSource(
            const char*     systemId,
            MemoryManager&  theMemoryManager) :
    InputSource(
        systemId,
        &theMemoryManager),
    m_stream(0),
    m_node(0)
{
}



XSLTInputSource::XSLTInputSource(
            const char*     systemId,
            const char*     publicId,
            MemoryManager&  theMemoryManager) :
    InputSource(
        systemId,
        publicId,
        &theMemoryManager),
    m_stream(0),
    m_node(0)
{
}



XSLTInputSource::XSLTInputSource(
            XalanNode*      node,
            MemoryManager&  theMemoryManager) :
    InputSource(&theMemoryManager),
    m_stream(0),
    m_node(node)
{
}



XSLTInputSource::XSLTInputSource(
            StreamType*     stream,
            MemoryManager&  theMemoryManager) :
    InputSource(&theMemoryManager),
    m_stream(stream),
    m_node(0)
{
}



XSLTInputSource::XSLTInputSource(
            StreamType&     stream,
            MemoryManager&  theMemoryManager) :
    InputSource(&theMemoryManager),
    m_stream(&stream),
    m_node(0)
{
}



BinInputStreamType*
XSLTInputSource::makeStream() const
{
    BinInputStreamType*     theResult = 0;

    MemoryManager*  theManager = getMemoryManager();

    assert(theManager != 0 );

    if (m_stream != 0)
    {
        
        theResult = new (theManager) StdBinInputStream(*m_stream);
    }
    else if (m_node == 0)
    {
        const XalanDOMChar* const   theSystemId = getSystemId();

        if (theSystemId != 0)
        {
            XALAN_USING_XERCES(XMLURL)

            XMLURL  theURL(theManager);

            URISupport::getURLFromString(theSystemId, theURL, *theManager);

            theResult = theURL.makeNewStream();
        }
    }

    return theResult;
}



XALAN_CPP_NAMESPACE_END
