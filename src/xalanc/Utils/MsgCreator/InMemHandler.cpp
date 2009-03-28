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
#include "InMemHandler.hpp"
#include <cstdio>
#include <cassert>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include "MsgCreator.hpp"
#include "InMemData.hpp"



XALAN_CPP_NAMESPACE_BEGIN

#if defined(XALAN_STRICT_ANSI_HEADERS)
  using std::sprintf;
#endif

// -----------------------------------------------------------------------
//  Constructors
// -----------------------------------------------------------------------
InMemHandler::InMemHandler(
            const char*     fileName,
            const char*     indexFileName) :
    SAX2Handler(indexFileName),
    m_stream(fileName),
    m_isTheFirstLineInArray(true)
{
}



InMemHandler::~InMemHandler()
{

}



void
InMemHandler::printToDataFile(const char*   sArrayOfStrings[])
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
InMemHandler::endDocument()
{
    createBottomForDataFile();

    SAX2Handler::endDocument();
}



void
InMemHandler::characters(
            const XMLCh* const  chars,
            const XalanSize_t   length)
{
    if (m_startCollectingCharacters == true)
    {
        char buffer[20];

        for (XalanSize_t i = 0; i < length ; ++i)
        {
            sprintf(
                buffer,
                " %#4x, ",
                chars[i]);

            m_stream.writeAsASCII(
                buffer,
                XMLString::stringLen(buffer));
        }
    }
}



void
InMemHandler::startDocument()
{
    createHeaderForDataFile();

    SAX2Handler::startDocument();
}



void
InMemHandler::endElement(
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
InMemHandler::startElement(
            const XMLCh* const  uri,
            const XMLCh* const  localname,
            const XMLCh* const  qname,
            const Attributes&   attributes)
{
    if(!XMLString::compareString(qname, s_transUnitXMLCh))
    {
        // This is an element, SAX2Handler class is responsible to handle:
        // creating Index file, commom for all localization styles
        SAX2Handler::startElement(uri, localname, qname, attributes);
    }
    else if(!XMLString::compareString(qname, s_targetXMLCh))
    {
        m_startCollectingCharacters = true; 

        printBeginOfDataLine();
    }
}



void
InMemHandler::createHeaderForDataFile()
{
    printToDataFile(szApacheLicense);
    printToDataFile(szStartDataFile);
}



void
InMemHandler::printBeginOfDataLine()
{
    char    buff[500];

    assert(XMLString::stringLen(s_szVariableName) < 400);

    sprintf(
        buff,
        "%.400s%d[] = {",
        s_szVariableName,
        m_numberOfRecords);

    m_stream.writeAsASCII(
        buff,
        XMLString::stringLen(buff));
}



void
InMemHandler::printEndOfDataLine()
{
    printToDataFile(szEndOfLineInDataFile);
}



void
InMemHandler::createBottomForDataFile()
{
    printToDataFile(szArraySizeVar);

    char    buff[100];

    sprintf(
        buff,
        " %d ;",
        m_numberOfRecords);

    m_stream.writeAsASCII(
        buff,
        XMLString::stringLen(buff));

    const char* const   theString =
        "\nstatic const XalanDOMChar* msgArray[]={";

    m_stream.writeAsASCII(
        theString,
        XMLString::stringLen(theString));

    for(int i = 0; i < m_numberOfRecords; ++i)
    {
        sprintf(
            buff,
            "%s%d ",
            s_szSimpleVariableName,
            i + 1);

        m_stream.writeAsASCII(
            buff,
            XMLString::stringLen(buff));

        if(i != m_numberOfRecords - 1)
        {
            m_stream.writeAsASCII(
                ",",
                1);
        }
        else
        {
            m_stream.writeAsASCII(
                "};",
                2);
        }
    }

    printToDataFile(szEndDataFile);
}



XALAN_CPP_NAMESPACE_END
