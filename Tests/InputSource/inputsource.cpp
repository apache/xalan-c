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



#include <cstdio>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



// This is here for memory leak testing. 
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



// XERCES HEADERS...
#include <xercesc/util/PlatformUtils.hpp>



#include <xercesc/parsers/XercesDOMParser.hpp>



// XALAN HEADERS...
#include <xalanc/XPath/XPathExecutionContextDefault.hpp>
#include <xalanc/XPath/XObjectFactoryDefault.hpp>
#include <xalanc/XPath/XPathFactoryDefault.hpp>

#include <xalanc/XSLT/StylesheetConstructionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetExecutionContextDefault.hpp>
#include <xalanc/XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <xalanc/XSLT/XSLTEngineImpl.hpp>


#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>


#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>

#include <xalanc/Harness/XalanXMLFileReporter.hpp>
#include <xalanc/Harness/XalanFileUtility.hpp>



XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)


    
// Just hoist everything...
XALAN_CPP_NAMESPACE_USE



void
setHelp(XalanFileUtility&   h)
{
    h.args.getHelpStream() << endl
                << "inputsource dirname [-out]"
                << endl
                << endl
                << "dirname     (base directory for testcases)"
                << endl
                << "-out dirname    (base directory for output)"
                << endl;
}

// TestCase1 will use the following method of XSLTInputSource
//      - XSLTInputSource(const XMLCh* systemId)
//      - XSLTInputSource(const XMLCh* systemId,
//                        const XMLCh* publicId)
void
testCase1(
            XalanTransformer&       xalan,
            XalanXMLFileReporter&   logFile, 
            const XalanDOMString&   xml,
            const XalanDOMString&   xsl, 
            const XalanDOMString&   outBase,
            const XalanDOMString&   theGoldFile,
            XalanFileUtility&       h)
{
    const XalanDOMString    publicID("public-smublic");

    const XalanDOMString    theOutputFile = outBase + XalanDOMString("\\InputSource-TestCase1.out");
    const XSLTResultTarget  theResultTarget(theOutputFile);

    h.data.testOrFile = "InputSource-TestCase1";

    // This code exercised the stated methods of XSLTInputSource
    const XSLTInputSource   xmlInputSource(xml);
    const XSLTInputSource   xslInputSource(xsl, publicID);

    // Do the transform and report the results.
    xalan.transform(xmlInputSource, xslInputSource, theResultTarget);

    h.checkResults(theOutputFile, theGoldFile, logFile);
}


// TestCase2 will use the following methods of XSLTInputSource
//      - XSLTInputSource(const char* systemId)
//      - XSLTInputSource(const char* systemId,
//                        const char* publicId)
void
testCase2(
            XalanTransformer&       xalan,
            XalanXMLFileReporter&   logFile, 
            const XalanDOMString&   outBase,
            const XalanDOMString&   theGoldFile,
            XalanFileUtility&       h)

{
    const XalanDOMString    theOutputFile = outBase + XalanDOMString("\\InputSource-TestCase2.out");
    const XSLTResultTarget  theResultTarget2(theOutputFile);

    h.data.testOrFile = "InputSource-TestCase2";

    // This code excersized the stated methods of XSLTInputSource
    const XSLTInputSource   xslStringSource("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xsl","public-smublic");
    const XSLTInputSource   xmlStringSource("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xml");


    // Do the transform and report the results.
    xalan.transform(xmlStringSource, xslStringSource, theResultTarget2);
    h.checkResults(theOutputFile, theGoldFile, logFile);

}

// TestCase3 will use the following methods of XSLTInputSource
//      - XSLTInputSource()
//      - XSLTInputSource(XalanNode* node)
//      - XSLTInputSource::setNode(XalanNode* node)
//      - XSLTInputSource::getNode()
//      - NOTE:  We can't use the XalanTransformer Class with this test. So we create the processor directly.

void testCase3(
            XalanXMLFileReporter&   logFile, 
            const XalanDOMString&   outBase,
            const XalanDOMString&   theGoldFile,
            XalanFileUtility&       h)
{
    XalanDOMString          name, value;    

    const XalanDOMString    theOutputFile = outBase + XalanDOMString("\\InputSource-TestCase3.out");
    XSLTResultTarget        theResultTarget3(theOutputFile);

    h.data.testOrFile = "InputSource-TestCase3a";

    // Create necessary Infrastructure to perform transform without the XalanTransformer Class.
    // Can't use it because it doesn't take a DOM.
    XalanSourceTreeDOMSupport       domSupport;
    XalanSourceTreeParserLiaison    parserLiaison(domSupport);
    domSupport.setParserLiaison(&parserLiaison);

    XSLTProcessorEnvSupportDefault  envSupport;
    XObjectFactoryDefault           xObjectFactory;
    XPathFactoryDefault             xPathFactory;
    
    // Create a processor and connect to ProcessorEnvSupport object
    XSLTEngineImpl  theProcessor(parserLiaison,
                            envSupport,
                            domSupport,
                            xObjectFactory,
                            xPathFactory);

    // Hook up the processor the the support object.
    envSupport.setProcessor(&theProcessor);

    // Create separate factory support object, so the stylesheet's
    // factory-created XPath instance are independent from processor's.
    XPathFactoryDefault     ssXPathFactory;

    // Create a stylesheet construction context, using the
    // stylesheet's factory support objects.
    StylesheetConstructionContextDefault constContext(  theProcessor,
                                                        ssXPathFactory);

    StylesheetExecutionContextDefault   execuContext(
                                            theProcessor,
                                            envSupport,
                                            domSupport,
                                            xObjectFactory);

    // Create the XSL Input Source
    const XSLTInputSource   xslStringSource("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xsl");

    XalanDocument* const    domXSL = parserLiaison.parseXMLStream(xslStringSource);

    // Here is one of the tests. It will be verified when we do the transform.
    XSLTInputSource     xslXalanNode(domXSL);

    // Set the system ID, so relative URIs are resolved properly...
    xslXalanNode.setSystemId(xslStringSource.getSystemId());

    XalanNode   *theNode, *firstChild;

    // Here is another test 
    theNode = xslXalanNode.getNode();

    firstChild = theNode->getFirstChild();
    value = firstChild->getNodeValue();

    h.checkAPIResults(value, XalanDOMString(" This is THE Smoke Test "), "XSLTInputSource.getNode()", logFile,
                            theOutputFile, theGoldFile);

    // Create the XML Input Source
    const XSLTInputSource   xmlStringSource("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xml");

    XalanDocument* const    domXML = parserLiaison.parseXMLStream(xmlStringSource);

    // Here is the final test. It too will be verified when we do the transform.
    XSLTInputSource xmlXalanNode;
    xmlXalanNode.setNode(domXML);

    // Do the transform and report the results. 
    theProcessor.process(xmlXalanNode,      //inputSource, 
                        xslXalanNode,       //stylesheetSource,
                        theResultTarget3,   //outputTarget,
                        constContext,
                        execuContext);

    // Reset testcase name
    h.data.testOrFile = "InputSource-TestCase3b";
    h.checkResults(theOutputFile, 
                      theGoldFile, 
                      logFile);
}

// TestCase4 will use the following API.  Default constructor of XSLTInputSource will take a string.
//      - XSLTInputSource(const char*)
//      - XSLTInputSource(const char*)
void testCase4(
            XalanTransformer&       xalan,
            XalanXMLFileReporter&   logFile, 
            const XalanDOMString&   outBase,
            const XalanDOMString&   theGoldFile,
            XalanFileUtility&       h)
{

    const XalanDOMString theOutputFile = outBase + XalanDOMString("\\InputSource-TestCase4.out");

    const XSLTResultTarget  theResultTarget(theOutputFile);

    h.data.testOrFile = "InputSource-TestCase4";

    // This code exercises the stated methods of XSLTInputSource
    // Do the transform and report the results.
    xalan.transform("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xml", 
                    "\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xsl",
                    theResultTarget);

    h.checkResults(theOutputFile, theGoldFile, logFile);
}

// TestCase5 uses XercesDOMWrapperParsedSource class to wrap a xerces generated dom. 
//
void 
testCase5(
            XalanTransformer&       xalan,
            XalanXMLFileReporter&   logFile,
            const XalanDOMString&   xml,
            const XalanDOMString&   xsl,
            const XalanDOMString&   outBase,
            const XalanDOMString&   theGoldFile,
            XalanFileUtility&       h)
{
    XALAN_USING_XERCES(XercesDOMParser)
    XALAN_USING_XERCES(DOMDocument)

    h.data.testOrFile = "InputSource-TestCase5";
    const XSLTInputSource   xmlInputSource(xml);
         
    XercesDOMParser     theParser;

    theParser.parse(xmlInputSource);
    const DOMDocument* const    theDOM = theParser.getDocument();

    XercesDOMSupport    theDOMSupport;
    XercesParserLiaison theParserLiaison(theDOMSupport);

    // This is the wrapper class for Xerces DOM. 
    const XercesDOMWrapperParsedSource  xmlDocWrapper(theDOM, 
                                                     theParserLiaison, 
                                                     theDOMSupport, 
                                                     xml);

    const XalanDOMString theOutputFile(outBase + XalanDOMString("\\InputSource-TestCase5.out"));

    const XSLTInputSource   xslInputSource(xsl);
    const XSLTResultTarget  theResultTarget(theOutputFile);

    // Do the transform and report the results.
    xalan.transform(xmlDocWrapper, xslInputSource, theResultTarget);

    h.checkResults(theOutputFile, theGoldFile, logFile);
}



int
runTests(
            int     argc,
            char*   argv[])
{
    int                 theResult = 0;

    XalanFileUtility    h;

    // Set the program help string,  then get the command line parameters.
    //
    setHelp(h);

    if (h.getParams(argc, argv, "INPUTSOURCE-RESULTS") == true)
    {
        // Generate and Initialize Unique result logfile, and get drive designation
        //
        const XalanDOMString UniqRunid = h.generateUniqRunid();
        const XalanDOMString drive(h.getDrive());
        const XalanDOMString  resultFilePrefix("isource");
        const XalanDOMString  resultsFile(drive + h.args.output + resultFilePrefix + UniqRunid + XalanFileUtility::s_xmlSuffix);

        XalanXMLFileReporter    logFile(resultsFile);
        logFile.logTestFileInit("XSLTInputSource Testing: Give various types of allowable Inputs. ");

        try
        {
            XalanTransformer    xalan;

            XalanDOMString      fileName;

            // Get testfiles from the capi\smoke directory, create output directory, .
            //
            const XalanDOMString  currentDir("smoke");
            const XalanDOMString  theOutputDir = h.args.output + currentDir;
                
            h.checkAndCreateDir(theOutputDir);

            typedef XalanFileUtility::FileNameVectorType        FileNameVectorType;

            // Get the single file found in the "smoke" directory, and run tests.
            //
            const FileNameVectorType    files = h.getTestFileNames(h.args.base, currentDir, true);
            logFile.logTestCaseInit(currentDir);

            for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
            {
                fileName = files[i];
                h.data.testOrFile = fileName;

                // Set up the input/output files.
                const XalanDOMString  theXSLFile= h.args.base + currentDir + XalanFileUtility::s_pathSep + fileName;
                const XalanDOMString  theXMLFile = h.generateFileName(theXSLFile,"xml");
                h.data.xmlFileURL = theXMLFile;
                h.data.xslFileURL = theXSLFile;

                // Set the gold file.
                XalanDOMString  theGoldFile = h.args.gold + currentDir + XalanFileUtility::s_pathSep + fileName;
                theGoldFile = h.generateFileName(theGoldFile, "out");
                        
                // Execute the test cases. 
                //
                testCase1(xalan, logFile, theXMLFile, theXSLFile, theOutputDir, theGoldFile, h);
                testCase2(xalan, logFile, theOutputDir, theGoldFile, h);
                testCase3(logFile, theOutputDir, theGoldFile, h);
                testCase4(xalan, logFile, theOutputDir, theGoldFile, h);
                testCase5(xalan, logFile, theXMLFile, theXSLFile, theOutputDir, theGoldFile, h);
            }

            logFile.logTestCaseClose("Done", "Pass");   

            h.reportPassFail(logFile, UniqRunid);
            logFile.logTestFileClose("ISource Testing: ", "Done");
            logFile.close();

            h.analyzeResults(xalan, resultsFile);
        }
        catch(...)
        {
            cerr << "Exception caught!!!" << endl << endl;

            theResult = -1;
        }
    }

    return theResult;
}



int
main(
            int     argc,
            char*   argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

    int theResult = 0;

    try
    {
        XALAN_USING_XERCES(XMLPlatformUtils)

        // Call the static initializers for xerces and xalan, and create a transformer
        //
        XMLPlatformUtils::Initialize();

        XalanTransformer::initialize();

        theResult = runTests(argc, argv);

        XalanTransformer::terminate();

        XMLPlatformUtils::Terminate();

        XalanTransformer::ICUCleanUp();
    }
    catch(...)
    {
        cerr << "Initialization failed!" << endl << endl;

        theResult = -1;
    }

    return theResult;
}
