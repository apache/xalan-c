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

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "ICUResData.hpp"
#include "ICUResHandler.hpp"

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

#include <cstdio>
#include <cassert>



XALAN_CPP_NAMESPACE_BEGIN



// -----------------------------------------------------------------------
//  Constructors
// -----------------------------------------------------------------------
ICUResHandler::ICUResHandler(
            const char*     fileName,
            const char*     indexFileName) :
    SAX2Handler(indexFileName),
    m_stream(fileName)
{
}



ICUResHandler::~ICUResHandler()
{

}



void
ICUResHandler::endDocument()
{
    createBottomForDataFile();

    SAX2Handler::endDocument();
}



void
ICUResHandler::characters(
            const XMLCh* const  chars,
            const XalanSize_t   length)
{
    if (m_startCollectingCharacters == true)
    {
        // We limit lines to 512 characters. Since
        // we need to acount for the terminating null,
        // and the possibility of escaping each character,
        // 1025 is the maximum buffer size.
        const XalanSize_t   theMaxChars = 512;
        assert(length <= theMaxChars); 

        XMLCh   buffer[(theMaxChars * 2) + 1];

        XalanSize_t    j = 0;

        // ICU genrb doesn't like " and {} chars
        // change tham with \", \{ and \}.
        for (XalanSize_t i = 0 ; i < length ; ++i , ++j)
        {
            if (chars[i] == chDoubleQuote ||
                chars[i] == chOpenCurly ||
                chars[i] == chCloseCurly)
            {
                buffer[j] = chBackSlash;

                ++j;

                buffer[j] = chars[i]; 
            }
            else
            {
                buffer[j] = chars[i];
            }
        }

        buffer[j] = chNull;

        m_stream.write(buffer, j);  
    }
}



void
ICUResHandler::startDocument()
{
    m_stream.writeUTFPrefix();

    createHeaderForDataFile();

    SAX2Handler::startDocument();
}



void
ICUResHandler::endElement(
            const XMLCh* const  /* uri */,
            const XMLCh* const  /* localname */,
            const XMLCh* const  qname)
{
    if (m_startCollectingCharacters == true)
    {
        if(!XMLString::compareString(qname, s_targetXMLCh))
        {
            m_startCollectingCharacters = false;

            printEndOfDataLine();
        }
    }
}



void
ICUResHandler::startElement(
            const XMLCh* const  uri,
            const XMLCh* const  localname,
            const XMLCh* const  qname,
            const Attributes&   attributes)
{
    if(!XMLString::compareString(qname, s_transUnitXMLCh))
    {
        // this is an elemente, SAX2Handler class is responsible to handle:
        // creating Index file, commom for all localization styles
        SAX2Handler::startElement(uri, localname, qname, attributes);
    }
    else if(!XMLString::compareString(qname, s_targetXMLCh))
    {
        if (m_locale != 0)
        {
            m_startCollectingCharacters = true; 

            printBeginOfDataLine();
        }
    }
}



void
ICUResHandler::createHeaderForDataFile()
{
    printToDataFile(szApacheLicense);

    if (m_locale != 0)
    {
        m_stream.write(
            m_locale,
            XMLString::stringLen(m_locale));
    }

    printToDataFile(szStartDataFile);
}



void
ICUResHandler::printBeginOfDataLine()
{
    printToDataFile(szBeginOfLineInDataFile);
}



void
ICUResHandler::createBottomForDataFile()
{
    printToDataFile(szEndDataFile);
}



void
ICUResHandler::printEndOfDataLine()
{
    printToDataFile(szEndOfLineInDataFile);
}



void
ICUResHandler::printToDataFile(const char*  sArrayOfStrings[])
{
    if ( sArrayOfStrings != 0)
    {
        for (int i = 0; sArrayOfStrings[i] != 0; ++i)
        {
            m_stream.write(
                sArrayOfStrings[i],
                XMLString::stringLen(sArrayOfStrings[i]));
        }
    }
}



XALAN_CPP_NAMESPACE_END
