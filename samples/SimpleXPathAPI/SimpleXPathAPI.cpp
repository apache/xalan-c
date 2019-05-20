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



#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>



#include <xalanc/PlatformSupport/XSLException.hpp>



#include <xalanc/DOMSupport/XalanDocumentPrefixResolver.hpp>



#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XPathEvaluator.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeInit.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



int
main(
            int     argc,
            char*   argv[])
{
    using std::cerr;
    using std::cout;
    using std::endl;

    int     theResult = 0;

    if (argc != 4)
    {
        cerr << "Usage: SimpleXPathAPI XMLFilePath Context XPathExpression" << endl;

        theResult = -1;
    }
    else
    {
        using xalanc::XSLException;

        try
        {
            using xercesc::XMLPlatformUtils;

            using xalanc::XPathEvaluator;


            XMLPlatformUtils::Initialize();

            XPathEvaluator::initialize();

            {
                using xercesc::LocalFileInputSource;

                using xalanc::XalanDocument;
                using xalanc::XalanDocumentPrefixResolver;
                using xalanc::XalanDOMString;
                using xalanc::XalanNode;
                using xalanc::XalanSourceTreeInit;
                using xalanc::XalanSourceTreeDOMSupport;
                using xalanc::XalanSourceTreeParserLiaison;
                using xalanc::XObjectPtr;

                // Initialize the XalanSourceTree subsystem...
                XalanSourceTreeInit     theSourceTreeInit;

                // We'll use these to parse the XML file.
                XalanSourceTreeDOMSupport       theDOMSupport;
                XalanSourceTreeParserLiaison    theLiaison(theDOMSupport);

                // Hook the two together...
                theDOMSupport.setParserLiaison(&theLiaison);

                const XalanDOMString    theFileName(argv[1]);

                // Create an input source that represents a local file...
                const LocalFileInputSource  theInputSource(theFileName.c_str());

                // Parse the document...
                XalanDocument* const    theDocument =
                        theLiaison.parseXMLStream(theInputSource);
                assert(theDocument != 0);

                XalanDocumentPrefixResolver     thePrefixResolver(theDocument);

                XPathEvaluator  theEvaluator;

                // OK, let's find the context node...
                XalanNode* const    theContextNode =
                        theEvaluator.selectSingleNode(
                            theDOMSupport,
                            theDocument,
                            XalanDOMString(argv[2]).c_str(),
                            thePrefixResolver);

                if (theContextNode == 0)
                {
                    cerr << "Warning -- No nodes matched the location path \""
                         << argv[2]
                         << "\"."
                         << endl
                         << "Execution cannot continue..."
                         << endl
                         << endl;
                }
                else
                {
                    // OK, let's evaluate the expression...
                    const XObjectPtr    theResult(
                        theEvaluator.evaluate(
                                theDOMSupport,
                                theContextNode,
                                XalanDOMString(argv[3]).c_str(),
                                thePrefixResolver));

                    assert(theResult.null() == false);

                    cout << "The string value of the result is:"
                         << endl
                         << theResult->str(theEvaluator.getExecutionContext())
                         << endl
                         << endl;
                }
            }

            XPathEvaluator::terminate();

            XMLPlatformUtils::Terminate();
        }
        catch(const XSLException&   theException)
        {
            using xalanc::XalanDOMString;

            XalanDOMString  theError;

            cerr << "XSL exception: "
                 << theException.defaultFormat(theError)
                 << endl;

            theResult = -1;
        }
        catch(...)
        {
            cerr << "Generic exception caught!" << endl;

            theResult = -1;
        }
    }

    return theResult;
}
