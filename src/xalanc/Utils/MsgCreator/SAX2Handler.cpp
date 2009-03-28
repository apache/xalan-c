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
#include "SAX2Handler.hpp"

#include <cassert>
#include <cstdio>

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

#include "IndexFileData.hpp"
#include "MsgCreator.hpp"



XALAN_CPP_NAMESPACE_BEGIN

#if defined(XALAN_STRICT_ANSI_HEADERS)
  using std::sprintf;
#endif


XALAN_USING_XERCES(Attributes)
XALAN_USING_XERCES(SAXParseException)
XALAN_USING_XERCES(XMLString)

// ---------------------------------------------------------------------------
//  SAX2Handler: Constructors and Destructor
// ---------------------------------------------------------------------------
SAX2Handler::SAX2Handler(const char*     indexFileName) :
    m_numberOfRecords(0),
    m_locale(0),
    m_startCollectingCharacters(false),
    m_indexOutputStream(indexFileName)
{
}



SAX2Handler::~SAX2Handler()
{
    if (m_locale != 0)
    {
        XMLString::release(&m_locale);
    }
}



void
SAX2Handler::createHeaderForIndexFile()
{
    printToIndexFile(szApacheLicense);

    printToIndexFile(szStartIndexFile);
}



void
SAX2Handler::printBeginOfIndexLine()
{
    printToIndexFile(szBeginIndexLine);
}


void
SAX2Handler::printEndOfIndexLine()
{
}



void
SAX2Handler::createBottomForIndexFile()
{
    printToIndexFile(szEndIndexFile);
}



void
SAX2Handler::printToIndexFile(const char*   sArrayOfStrings[])
{
    if (sArrayOfStrings != 0)
    {
        for (XalanSize_t i = 0; sArrayOfStrings[i] != 0; ++i)
        {
            m_indexOutputStream.writeAsASCII(
                sArrayOfStrings[i],
                XMLString::stringLen(sArrayOfStrings[i]));
        }
    }
}



void
SAX2Handler::startElement(
            const   XMLCh* const    /* uri */,
            const   XMLCh* const    /* localname */,
            const   XMLCh* const    qname,
            const   Attributes&     attributes)
{
    if(!XMLString::compareString(qname, s_transUnitXMLCh))
    {
        const XalanSize_t  len =
            attributes.getLength();

        ++m_numberOfRecords;

        for (XalanSize_t index = 0; index < len; index++)
        {
            const XMLCh* const  name =
                attributes.getQName(index);
            assert(name != 0);

            if (!XMLString::compareString(name, s_idXMLCh))
            {
                const XMLCh* const  val =
                    attributes.getValue(index);
                assert(val != 0);

                if (m_numberOfRecords !=  1)
                {
                    printBeginOfIndexLine();
                }

                m_indexOutputStream.writeAsASCII(
                    val,
                    XMLString::stringLen(val));

                char buff[100];

                sprintf(
                    buff,
                    "\t\t = %d \n",
                    m_numberOfRecords - 1);

                m_indexOutputStream.writeAsASCII(
                    buff,
                    XMLString::stringLen(buff));

                printEndOfIndexLine();
            }
        }
    }
}


XALAN_USING_STD(cerr)
XALAN_USING_STD(endl)


// ---------------------------------------------------------------------------
//  
// ---------------------------------------------------------------------------
void
SAX2Handler::error(const SAXParseException&     e)
{
    cerr << "\nError at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;

    throw e;
}



void
SAX2Handler::fatalError(const SAXParseException&    e)
{
    cerr << "\nFatal Error at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;

    throw e;
}



void
SAX2Handler::warning(const SAXParseException&   e)
{
    cerr << "\nWarning at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}



void
SAX2Handler::setLocale(const char*  localName)
{
    assert(localName != 0);

    if (m_locale != 0)
    {
        XMLString::release(&m_locale);
    }

    m_locale = XMLString::transcode(localName);
}



void
SAX2Handler::startDocument()
{
    createHeaderForIndexFile();
}



void
SAX2Handler::endDocument()
{
    createBottomForIndexFile();
}



XALAN_CPP_NAMESPACE_END
