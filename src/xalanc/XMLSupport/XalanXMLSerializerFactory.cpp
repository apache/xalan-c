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
#include "XalanXMLSerializerFactory.hpp"



#include "xalanc/PlatformSupport/XalanTranscodingServices.hpp"



#include "xalanc/XMLSupport/FormatterToXML.hpp"
#include "xalanc/XMLSupport/FormatterToXMLUnicode.hpp"

#include "xalanc/XMLSupport/XalanDummyIndentWriter.hpp"
#include "xalanc/XMLSupport/XalanIndentWriter.hpp"

#include "xalanc/XMLSupport/XalanOtherEncodingWriter.hpp"

#include "xalanc/XMLSupport/XalanUTF8Writer.hpp"
#include "xalanc/XMLSupport/XalanUTF16Writer.hpp"


XALAN_CPP_NAMESPACE_BEGIN



FormatterListener*
XalanXMLSerializerFactory::create(
            MemoryManager&          theManager,
            Writer&                 theWriter,
            const XalanDOMString&   theVersion,
            bool                    doIndent,
            int                     theIndentAmount,
            const XalanDOMString&   theEncoding,
            const XalanDOMString&   /*theMediaType*/,
            const XalanDOMString&   theDoctypeSystem,
            const XalanDOMString&   theDoctypePublic,
            bool                    generateXMLDeclaration,
            const XalanDOMString&   theStandaloneString)
{
    bool    isVersion1_1 = false;

    if (theVersion == XalanXMLSerializerBase::s_1_1String)
    {
        isVersion1_1 = true;
    }

    XalanDOMString fixedEncoding(theEncoding, theManager);

    setEncoding(theManager, theWriter, fixedEncoding);

    FormatterListener*  theFormatter = 0;

    if (XalanTranscodingServices::encodingIsUTF8(fixedEncoding))
    {
        if (isVersion1_1 == true)
        {
            if(doIndent == true)
            {
                typedef XalanFormatterWriter::NewLineWriterFunctor<XalanUTF8Writer> NewLineWriter;
                typedef XalanFormatterWriter::WhiteSpaceWriterFunctor<XalanUTF8Writer> WhiteSpaceWriter;

                typedef XalanIndentWriter<WhiteSpaceWriter,NewLineWriter> IndentWriter;

                typedef FormatterToXMLUnicode<
                    XalanUTF8Writer,
                    XalanXMLSerializerBase::UTF8,
                    XalanXMLSerializerBase::CharFunctor1_1,
                    IndentWriter,
                    FormatterListener::XML_VERSION_1_1>  Type;

                theFormatter =
                    Type::create(
                    theManager,
                    theWriter,
                    fixedEncoding,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString,
                    theIndentAmount);
            }
            else // doIndent == false
            {      
                typedef XalanDummyIndentWriter<XalanUTF8Writer> IndentWriter;

                typedef FormatterToXMLUnicode<
                    XalanUTF8Writer,
                    XalanXMLSerializerBase::UTF8,
                    XalanXMLSerializerBase::CharFunctor1_1,
                    IndentWriter,
                    FormatterListener::XML_VERSION_1_1>  Type;

                theFormatter =
                    Type::create(
                    theManager,    
                    theWriter,
                    fixedEncoding,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString,
                    theIndentAmount);
            }
        }
        else // XML 1.0 section
        {
            if(doIndent == true)
            {
                typedef XalanFormatterWriter::NewLineWriterFunctor<XalanUTF8Writer> NewLineWriter;
                typedef XalanFormatterWriter::WhiteSpaceWriterFunctor<XalanUTF8Writer> WhiteSpaceWriter;

                typedef XalanIndentWriter<WhiteSpaceWriter,NewLineWriter> IndentWriter;

                typedef FormatterToXMLUnicode<
                    XalanUTF8Writer,
                    XalanXMLSerializerBase::UTF8,
                    XalanXMLSerializerBase::CharFunctor1_0,
                    IndentWriter,
                    FormatterListener::XML_VERSION_1_0>  Type;

                theFormatter =
                    Type::create(
                    theManager,
                    theWriter,
                    fixedEncoding,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString,
                    theIndentAmount);
            }
            else // doIndent == false
            {      
                typedef XalanDummyIndentWriter<XalanUTF8Writer> IndentWriter;

                typedef FormatterToXMLUnicode<
                    XalanUTF8Writer,
                    XalanXMLSerializerBase::UTF8,
                    XalanXMLSerializerBase::CharFunctor1_0,
                    IndentWriter,
                    FormatterListener::XML_VERSION_1_0>  Type;

                theFormatter =
                    Type::create(
                    theManager,
                    theWriter,
                    fixedEncoding,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString,
                    theIndentAmount);
            }
        }
    }
    else if (XalanTranscodingServices::encodingIsUTF16(fixedEncoding))
    {
        if (isVersion1_1 == true)
        {
            if(doIndent == true)
            {
                typedef XalanFormatterWriter::NewLineWriterFunctor<XalanUTF16Writer> NewLineWriter;
                typedef XalanFormatterWriter::WhiteSpaceWriterFunctor<XalanUTF16Writer> WhiteSpaceWriter;

                typedef XalanIndentWriter<WhiteSpaceWriter, NewLineWriter> IndentWriter;

                typedef FormatterToXMLUnicode<
                            XalanUTF16Writer,
                            XalanXMLSerializerBase::UTF16,
                            XalanXMLSerializerBase::CharFunctor1_1,
                            IndentWriter,
                            FormatterListener::XML_VERSION_1_1>  Type;

                theFormatter =
                    Type::create(
                        theManager,
                        theWriter,
                        fixedEncoding,
                        theDoctypeSystem,
                        theDoctypePublic,
                        generateXMLDeclaration,
                        theStandaloneString,
                        theIndentAmount);
            }
            else //doIndent == false
            {
                typedef XalanDummyIndentWriter<XalanUTF16Writer> IndentWriter;

                typedef FormatterToXMLUnicode<
                            XalanUTF16Writer,
                            XalanXMLSerializerBase::UTF16,
                            XalanXMLSerializerBase::CharFunctor1_1,
                            IndentWriter,
                            FormatterListener::XML_VERSION_1_1>  Type;

                theFormatter =
                    Type::create(
                        theManager,
                        theWriter,
                        fixedEncoding,
                        theDoctypeSystem,
                        theDoctypePublic,
                        generateXMLDeclaration,
                        theStandaloneString,
                        theIndentAmount);
            }
        }
        else // XML 1.0 section
        {
            if(doIndent == true)
            {
                typedef XalanFormatterWriter::NewLineWriterFunctor<XalanUTF16Writer> NewLineWriter;
                typedef XalanFormatterWriter::WhiteSpaceWriterFunctor<XalanUTF16Writer> WhiteSpaceWriter;

                typedef XalanIndentWriter<WhiteSpaceWriter,NewLineWriter> IndentWriter;

                typedef FormatterToXMLUnicode<
                    XalanUTF16Writer,
                    XalanXMLSerializerBase::UTF16,
                    XalanXMLSerializerBase::CharFunctor1_0,
                    IndentWriter,
                    FormatterListener::XML_VERSION_1_0>  Type;

                theFormatter =
                    Type::create(
                        theManager,
                        theWriter,
                        fixedEncoding,
                        theDoctypeSystem,
                        theDoctypePublic,
                        generateXMLDeclaration,
                        theStandaloneString,
                        theIndentAmount);
            }
            else // doIndent == false
            {
                typedef XalanDummyIndentWriter<XalanUTF16Writer> IndentWriter;

                typedef FormatterToXMLUnicode<
                    XalanUTF16Writer,
                    XalanXMLSerializerBase::UTF16,
                    XalanXMLSerializerBase::CharFunctor1_0,
                    IndentWriter,
                    FormatterListener::XML_VERSION_1_0>  Type;

                theFormatter =
                    Type::create(
                        theManager,
                        theWriter,
                        fixedEncoding,
                        theDoctypeSystem,
                        theDoctypePublic,
                        generateXMLDeclaration,
                        theStandaloneString,
                        theIndentAmount);
            }
        }
    }
    else // all other encodings
    {
        typedef XalanOtherEncodingWriter<   XalanFormatterWriter::CommonRepresentableCharFunctor,
                                            XalanXMLSerializerBase::UTF16> WriterType ;

        if( isVersion1_1 == true)
        {
            if (doIndent == true)
            {
                typedef XalanFormatterWriter::NewLineWriterFunctor<WriterType> NewLineWriter;
                typedef XalanFormatterWriter::WhiteSpaceWriterFunctor<WriterType> WhiteSpaceWriter;

                typedef XalanIndentWriter<WhiteSpaceWriter,NewLineWriter> IndentWriter;


                typedef FormatterToXMLUnicode<
                    WriterType,
                    XalanXMLSerializerBase::UTF16,
                    XalanXMLSerializerBase::CharFunctor1_1,
                    IndentWriter,
                    FormatterListener::XML_VERSION_1_1>  Type;

                theFormatter =
                    Type::create(
                    theManager,
                    theWriter,
                    fixedEncoding,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString,
                    theIndentAmount);
            }
            else
            {
                typedef XalanDummyIndentWriter<WriterType> IndentWriter;

                typedef FormatterToXMLUnicode<
                    WriterType,
                    XalanXMLSerializerBase::UTF16,
                    XalanXMLSerializerBase::CharFunctor1_1,
                    IndentWriter,
                    FormatterListener::XML_VERSION_1_1>  Type;

                theFormatter =
                    Type::create(
                    theManager,
                    theWriter,
                    fixedEncoding,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString,
                    theIndentAmount);
            }
        }
        else // XML 1.0 section
        {
            if(doIndent == true)
            {
                typedef XalanFormatterWriter::NewLineWriterFunctor<WriterType> NewLineWriter;
                typedef XalanFormatterWriter::WhiteSpaceWriterFunctor<WriterType> WhiteSpaceWriter;

                typedef XalanIndentWriter<WhiteSpaceWriter,NewLineWriter> IndentWriter;

                typedef FormatterToXMLUnicode<
                    WriterType,
                    XalanXMLSerializerBase::UTF16,
                    XalanXMLSerializerBase::CharFunctor1_0,
                    IndentWriter,
                    FormatterListener::XML_VERSION_1_0>  Type;

                theFormatter =
                    Type::create(
                    theManager,
                    theWriter,
                    fixedEncoding,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString,
                    theIndentAmount);
            }
            else //doIndent == false
            {
                typedef XalanDummyIndentWriter<WriterType> IndentWriter;

                typedef FormatterToXMLUnicode<
                    WriterType,
                    XalanXMLSerializerBase::UTF16,
                    XalanXMLSerializerBase::CharFunctor1_0,
                    IndentWriter,
                    FormatterListener::XML_VERSION_1_0>  Type;

                theFormatter =
                    Type::create(
                    theManager,
                    theWriter,
                    fixedEncoding,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString,
                    theIndentAmount);
            }
        }
    }

    assert (theFormatter != 0);

    return theFormatter;
}



void
XalanXMLSerializerFactory::setEncoding(
            MemoryManager&     theManager,
            Writer&            theWriter,
            XalanDOMString&    theEncoding)
{
    XalanOutputStream* const    stream = theWriter.getStream();

    if(stream != 0)
    {
        if(theEncoding.empty())
        {
            // Default to UTF-8 if the requested encoding is not supported...
            stream->setOutputEncoding(XalanDOMString(XalanTranscodingServices::s_utf8String, theManager));

            theEncoding = XalanTranscodingServices::s_utf8String;
        }
        else
        {
            try
            {
                stream->setOutputEncoding(theEncoding);
            }
            catch(const XalanOutputStream::UnsupportedEncodingException&)
            {
                // Default to UTF-8 if the requested encoding is not supported...
                stream->setOutputEncoding(XalanDOMString(XalanTranscodingServices::s_utf8String, theManager));

                theEncoding = XalanTranscodingServices::s_utf8String;
            }
        }
    }
}



XALAN_CPP_NAMESPACE_END
