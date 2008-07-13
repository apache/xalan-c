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
#include "NLSData.hpp"
#include "NLSHandler.hpp"

#include <cstdio>



XALAN_CPP_NAMESPACE_BEGIN



// -----------------------------------------------------------------------
//  Constructors
// -----------------------------------------------------------------------
NLSHandler::NLSHandler(
            const char*     fileName,
            const char*     indexFileName,
            bool            createUnicode ):
    ICUResHandler(fileName, indexFileName),
    m_runningNumber(2),
    m_createUnicode(createUnicode)
{
}



void
NLSHandler::startDocument()
{
    if (m_createUnicode)
    {
        ICUResHandler::startDocument();
    }
    else
    {
        createHeaderForDataFile();
        
        SAX2Handler::startDocument();
    }
}



void
NLSHandler::characters(
            const XMLCh* const  chars,
            const XalanSize_t   length)
{
    if (m_startCollectingCharacters == true)
    {
        if (m_createUnicode)
        {
            m_stream.write(chars, length);
        }
        else
        {
            m_stream.writeAsASCII(chars, length);
        }
    }
}



void
NLSHandler::createHeaderForDataFile()
{
    if (m_createUnicode)
    {
        printToDataFile(szApacheLicense);
        printToDataFile(szStartDataFile);
    }
    else
    {
        printToDataFileAsASCII(szApacheLicense);
        printToDataFileAsASCII(szStartDataFile);
    }

}



void
NLSHandler::printToDataFileAsASCII(const char*  sArrayOfStrings[])
{
    if (sArrayOfStrings != 0)
    {
        for (XalanSize_t i = 0; sArrayOfStrings[i] != 0; ++i)
        {
            m_stream.writeAsASCII(
                sArrayOfStrings[i],
                XMLString::stringLen(sArrayOfStrings[i]));
        }
    }
}



void
NLSHandler::printBeginOfDataLine()
{
    char szNumb[20];

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::sprintf;
#endif

    sprintf(
        szNumb,
        "%d ^",
        m_runningNumber);

    ++m_runningNumber;

    if (m_createUnicode)
    {
        m_stream.write(
            szNumb,
            XMLString::stringLen(szNumb));
    }
    else
    {
        m_stream.writeAsASCII(
            szNumb,
            XMLString::stringLen(szNumb));
    }
}



void
NLSHandler::createBottomForDataFile()
{
}



void
NLSHandler::printEndOfDataLine()
{
    if (m_createUnicode)
    {
        m_stream.write(
            "^\n",
            2);
    }
    else
    {
        m_stream.writeAsASCII(
            "^\n",
            2);
    }
}



XALAN_CPP_NAMESPACE_END
