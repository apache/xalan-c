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

// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#include <strstream.h>
#else
#include <iostream>
#include <strstream>
#endif

#include <cstdio>

#if defined(_MSC_VER)
#include <direct.h>
#endif



XALAN_USING_STD(cerr)
XALAN_USING_STD(endl)



// XERCES HEADERS...
#include <xercesc/util/PlatformUtils.hpp>



// XALAN HEADERS...
#include <xalanc/XalanSourceTree/FormatterToSourceTree.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



#include <xalanc/Harness/XalanXMLFileReporter.hpp>
#include <xalanc/Harness/XalanFileUtility.hpp>



// This is here for memory leak testing.
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



XALAN_USING_XALAN(XalanFileUtility)



void
setHelp(XalanFileUtility&   h)
{
    h.args.getHelpStream() << endl
         << "compare dirname [-out -gold]"
         << endl
         << endl
         << "dirname        (base directory for testcases)"
         << endl
         << "-out dirname   (base directory for output)"
         << endl
         << "-gold dirname  (base directory for gold files)"
         << endl;
}



int
runTests(
            int     argc,
            char*   argv[])
{
    XalanFileUtility    h;

    bool setGold = true;

    // Set the program help string,  then get the command line parameters.
    //
    setHelp(h);

    if (h.getParams(argc, argv, "DOMCOM-RESULTS", setGold) == true)
    {
        XALAN_USING_XALAN(FormatterToSourceTree)
        XALAN_USING_XALAN(XalanCompiledStylesheet)
        XALAN_USING_XALAN(XalanDOMString)
        XALAN_USING_XALAN(XalanParsedSource)
        XALAN_USING_XALAN(XalanSourceTreeDocument)
        XALAN_USING_XALAN(XalanSourceTreeDOMSupport)
        XALAN_USING_XALAN(XalanSourceTreeParserLiaison)
        XALAN_USING_XALAN(XalanTransformer)
        XALAN_USING_XALAN(XalanXMLFileReporter)
        XALAN_USING_XALAN(XSLTInputSource)
        XALAN_USING_XALAN(XSLTResultTarget)

        //
        // Call the static initializers for xerces and xalan, and create a transformer
        //
        XalanTransformer    xalan;

        XalanSourceTreeDOMSupport       domSupport;
        XalanSourceTreeParserLiaison    parserLiaison(domSupport);
        domSupport.setParserLiaison(&parserLiaison);


        // Generate Unique Run id and processor info
        const XalanDOMString UniqRunid = h.generateUniqRunid();
        const XalanDOMString  resultFilePrefix("cpp");
        const XalanDOMString  resultsFile(h.args.output + resultFilePrefix + UniqRunid + XalanFileUtility::s_xmlSuffix);

        XalanXMLFileReporter    logFile(resultsFile);
        logFile.logTestFileInit("Comparison Testing:");
                    
        // Specify the "test" directory for both input and output.
        //
        const XalanDOMString  currentDir("domcomtests");
        const XalanDOMString  theOutputDir = h.args.output + currentDir;
        h.checkAndCreateDir(theOutputDir);

        // Get the files found in the test directory
        //
        logFile.logTestCaseInit(currentDir);

        typedef XalanFileUtility::FileNameVectorType        FileNameVectorType;

        const FileNameVectorType    files = h.getTestFileNames(h.args.base, currentDir, true);

        for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
        {
            const XalanDOMString&   fileName = files[i];
            h.data.reset();
            h.data.testOrFile = fileName;

            const XalanDOMString  theXSLFile= h.args.base + currentDir + XalanFileUtility::s_pathSep + fileName;
            const XalanDOMString  theXMLFile = h.generateFileName(theXSLFile,"xml");
            XalanDOMString  theGoldFile = h.args.gold + currentDir + XalanFileUtility::s_pathSep + fileName;
            theGoldFile = h.generateFileName(theGoldFile, "out");

            const XalanDOMString  outbase =  h.args.output + currentDir + XalanFileUtility::s_pathSep + fileName; 
            const XalanDOMString  theOutputFile = h.generateFileName(outbase, "out");

            const XSLTInputSource   xslInputSource(theXSLFile);
            const XSLTInputSource   xmlInputSource(theXMLFile);
            const XSLTInputSource   goldInputSource(theGoldFile);

            // Use a XalanSourceTreeDocument to create the XSLTResultTarget. 
            //
            XalanSourceTreeDocument* dom = parserLiaison.createXalanSourceTreeDocument();
            FormatterToSourceTree   domOut(dom); 
            XSLTResultTarget        domResultTarget;
            domResultTarget.setFormatterListener(&domOut);

            // Parsing(compile) the XSL stylesheet and report the results..
            //
            const XalanCompiledStylesheet*  compiledSS = 0;
            xalan.compileStylesheet(xslInputSource, compiledSS);
            if (compiledSS == 0 )
            {  
                continue;
            }

            // Parsing the input XML and report the results..
            //
            const XalanParsedSource*  parsedSource = 0;
            xalan.parseSource(xmlInputSource, parsedSource);
            if (parsedSource == 0)
            {
                continue;
            }

            // Perform One transform using parsed stylesheet and unparsed xml source, report results...
            // 
            xalan.transform(*parsedSource, compiledSS, domResultTarget);
            h.checkDOMResults(theOutputFile, compiledSS, dom, goldInputSource, logFile);

            parserLiaison.reset();
            xalan.destroyParsedSource(parsedSource);
            xalan.destroyStylesheet(compiledSS);
        }

        logFile.logTestCaseClose("Done", "Pass");
        h.reportPassFail(logFile, UniqRunid);
        logFile.logTestFileClose("DomCom ", "Done");
        logFile.close();

        h.analyzeResults(xalan, resultsFile);
    }

    return 0;
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

        XALAN_USING_XALAN(XalanTransformer)

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
