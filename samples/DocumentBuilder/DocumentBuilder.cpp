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

#include <xalanc/Include/PlatformDefinitions.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/sax2/ContentHandler.hpp>
#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/PlatformSupport/AttributesImpl.hpp>



#include <xalanc/XalanTransformer/XalanDocumentBuilder.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>



XALAN_USING_XALAN(XalanDocumentBuilder)
XALAN_USING_XALAN(XalanTransformer)



void
BuildDocument(XalanDocumentBuilder*     theBuilder)
{
    XALAN_USING_XERCES(ContentHandler)
    XALAN_USING_XALAN(AttributesImpl)
    XALAN_USING_XALAN(XalanDOMChar)
    XALAN_USING_XALAN(XalanDOMString)

    // Get the SAX2 ContentHandler from the builder...
    ContentHandler* const   theContentHandler = theBuilder->getContentHandler();
    assert(theContentHandler != 0);

    // This will hold the attributes for the elements we create...
    AttributesImpl  theAttributes;

    // Some handy scratch strings for adding elements, attributes, and text nodes...
    XalanDOMString          theElementName;
    XalanDOMString          theAttributeName;
    XalanDOMString          theAttributeValue;
    const XalanDOMString    theAttributeType("CDATA");
    XalanDOMString          theTextValue;
    const XalanDOMChar      theEmptyString = 0;

    // start the document...
    theContentHandler->startDocument();

    // start the document element...
    theElementName = "foo";

    theContentHandler->startElement(
        &theEmptyString,
        &theEmptyString,
        theElementName.c_str(),
        theAttributes);

    // Create an element child...

    // Set the name of the element...
    theElementName = "foobar";

    // Add an attribute...
    theAttributeName = "attribute1";
    theAttributeValue = "value1";

    theAttributes.addAttribute(
        theAttributeName.c_str(),
        theAttributeType.c_str(),
        theAttributeValue.c_str());

    theContentHandler->startElement(
        &theEmptyString,
        &theEmptyString,
        theElementName.c_str(),
        theAttributes);

    // Add a text node...
    theTextValue = "The first foobar";

    theContentHandler->characters(
        theTextValue.c_str(),
        theTextValue.length());

    // End the element...
    theContentHandler->endElement(
        &theEmptyString,
        &theEmptyString,
        theElementName.c_str());

    theAttributes.clear();
    theAttributeName = "attribute2";
    theAttributeValue = "value2";
    theAttributes.addAttribute(
        theAttributeName.c_str(),
        theAttributeType.c_str(),
        theAttributeValue.c_str());

    theContentHandler->startElement(
        &theEmptyString,
        &theEmptyString,
        theElementName.c_str(),
        theAttributes);

    theTextValue = "The second foobar";
    theContentHandler->characters(
        theTextValue.c_str(),
        theTextValue.length());

    theContentHandler->endElement(
        &theEmptyString,
        &theEmptyString,
        theElementName.c_str());

    theAttributes.clear();
    theAttributeName = "attribute3";
    theAttributeValue = "value3";
    theAttributes.addAttribute(
        theAttributeName.c_str(),
        theAttributeType.c_str(),
        theAttributeValue.c_str());

    theContentHandler->startElement(
        &theEmptyString,
        &theEmptyString,
        theElementName.c_str(),
        theAttributes);

    theTextValue = "The third foobar";
    theContentHandler->characters(
        theTextValue.c_str(),
        theTextValue.length());

    theContentHandler->endElement(
        &theEmptyString,
        &theEmptyString,
        theElementName.c_str());

    // end the document element...
    theElementName = "foo";

    theContentHandler->endElement(
        &theEmptyString,
        &theEmptyString,
        theElementName.c_str());

    // end the document...
    theContentHandler->endDocument();
}



int
main(
          int       argc,
          char*     /* argv */ [])
{
    XALAN_USING_STD(cerr)
    XALAN_USING_STD(endl)

    int theResult = 0;

    if (argc != 1)
    {
        cerr << "Usage: DocumentBuilder" << endl;

        theResult = -1;
    }
    else
    {
        XALAN_USING_XERCES(XMLPlatformUtils)
        XALAN_USING_XERCES(XMLException)

        // Call the static initializer for Xerces.
        try
        {
             XMLPlatformUtils::Initialize();
        }
        catch (const XMLException& toCatch)
        {
             cerr << "Error during Xerces initialization.  Error code was "
                  << toCatch.getCode()
                  << "."
                  << endl;

             theResult = -1;
        }

        if (theResult == 0)
        {
            // Initialize Xalan.
            XalanTransformer::initialize();

            {
                // Create a XalanTransformer.
                XalanTransformer    theXalanTransformer;

                // Get a document builder from the transformer...
                XalanDocumentBuilder* const     theBuilder = theXalanTransformer.createDocumentBuilder();

                BuildDocument(theBuilder);

                // The assumption is that the executable will be run
                // from same directory as the stylesheet file.

                // Do the transform.
                theResult = theXalanTransformer.transform(*theBuilder, "foo.xsl", "foo.out");
    
                if(theResult != 0)
                {
                    cerr << "DocumentBuilder error: \n" << theXalanTransformer.getLastError()
                         << endl
                         << endl;
                }
            }

            // Terminate Xalan.
            XalanTransformer::terminate();
        }

        // Call the static terminator for Xerces.
        XMLPlatformUtils::Terminate();

        // Clean up the ICU, if it's integrated.
        XalanTransformer::ICUCleanUp();
    }

    return theResult;
}
