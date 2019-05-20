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



#include <cassert>
#include <iostream>



#include <xercesc/framework/URLInputSource.hpp>
#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/parsers/XercesDOMParser.hpp>
#else
#include <xercesc/parsers/DOMParser.hpp>
#endif
#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/PlatformSupport/URISupport.hpp>



#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>
#include <xalanc/XalanTransformer/XalanSourceTreeWrapperParsedSource.hpp>



using xalanc::XalanCompiledStylesheet;
using xalanc::XalanDOMString;
using xalanc::XalanTransformer;
using xalanc::XSLTInputSource;
using xalanc::XSLTResultTarget;



int
transformXercesDOM(
            XalanTransformer&               theTransformer,
            const XalanDOMString&           theURI,
            const XalanCompiledStylesheet*  theStylesheet,
            const XSLTResultTarget&         theResultTarget)
{
    using xercesc::URLInputSource;

    using xalanc::XercesParserLiaison;
    using xalanc::XercesDOMSupport;
    using xalanc::XercesDOMWrapperParsedSource;


    const URLInputSource    theInputSource(theURI.c_str());

    XercesParserLiaison::DOMParserType  theParser;

    theParser.parse(theInputSource);

    XercesParserLiaison     theParserLiaison;
    XercesDOMSupport        theDOMSupport(theParserLiaison);

    const XercesDOMWrapperParsedSource  theWrapper(
                theParser.getDocument(),
                theParserLiaison,
                theDOMSupport,
                theURI);

    return theTransformer.transform(
                        theWrapper,
                        theStylesheet,
                        theResultTarget);
}



int
transformXalanSourceTree(
            XalanTransformer&               theTransformer,
            const XalanDOMString&           theURI,
            const XalanCompiledStylesheet*  theStylesheet,
            const XSLTResultTarget&         theResultTarget)
{
    using xercesc::URLInputSource;

    using xalanc::XalanDocument;
    using xalanc::XalanSourceTreeDocument;
    using xalanc::XalanSourceTreeParserLiaison;
    using xalanc::XalanSourceTreeDOMSupport;
    using xalanc::XalanSourceTreeWrapperParsedSource;


    const URLInputSource            theInputSource(theURI.c_str());

    XalanSourceTreeParserLiaison    theParserLiaison;
    XalanSourceTreeDOMSupport       theDOMSupport(theParserLiaison);

    XalanDocument* const    theDocument = theParserLiaison.parseXMLStream(theInputSource, theURI);

    XalanSourceTreeDocument* const  theSourceTreeDocument = theParserLiaison.mapDocument(theDocument);
    assert(theSourceTreeDocument != 0);

    XalanSourceTreeWrapperParsedSource  theWrapper(theSourceTreeDocument, theParserLiaison, theDOMSupport, theURI);

    return theTransformer.transform(
                        theWrapper,
                        theStylesheet,
                        theResultTarget);
}



int
transform()
{
    using std::cerr;
    using std::cout;
    using std::endl;

    int     theResult = -1;

    try
    {
        // Create a XalanTransformer.
        XalanTransformer    theTransformer;

        const XSLTInputSource   theStylesheetInputSource("foo.xsl");

        // Let's compile the stylesheet and re-use it...
        const XalanCompiledStylesheet*      theStylesheet = 0;

        if (theTransformer.compileStylesheet(theStylesheetInputSource, theStylesheet) != 0)
        {
            cerr << "An error occurred compiling the stylesheet: "
                 << theTransformer.getLastError()
                 << endl;
        }
        else
        {
            using xalanc::URISupport;

            assert(theStylesheet != 0);

            const XalanDOMString    theInputFile("foo.xml");

            XalanDOMString  theURI;
            URISupport::getURLStringFromString(theInputFile, theURI);

            const XSLTResultTarget  theResultTarget(cout);

            theResult = transformXercesDOM(theTransformer, theURI, theStylesheet, theResultTarget);

            if (theResult == 0)
            {
                cout << endl;

                theResult = transformXalanSourceTree(theTransformer, theURI, theStylesheet, theResultTarget);
            }

            if (theResult != 0)
            {
                cerr << "Transformation failed: " << theTransformer.getLastError() << endl;
            }
        }
    }
    catch(...)
    {
        cerr << "An unknown error occurred!" << endl;
    }

    return theResult;
}



// This sample shows how existing Xerces DOMDocument instances and XalanSourceTreeDocument
// instances can be wrapped to use as input for to an instance of XalanTransformer.
int
main(
            int      argc,
            char*   /* argv */[])
{
    using std::cerr;
    using std::endl;

    int     theResult = -1;

    if (argc != 1)
    {
        cerr << "Usage: ParsedSourceWrappers" << endl;
    }
    else
    {
        try
        {
            using xercesc::XMLPlatformUtils;

            // Call the static initializer for Xerces.
            XMLPlatformUtils::Initialize();

            // Initialize Xalan.
            XalanTransformer::initialize();

            theResult = transform();

            // Terminate Xalan...
            XalanTransformer::terminate();

            // Terminate Xerces...
            XMLPlatformUtils::Terminate();

            // Clean up the ICU, if it's integrated...
            XalanTransformer::ICUCleanUp();
        }
        catch(...)
        {
            cerr << "Initialization failed!" << endl;
        }
    }

    return theResult;
}
