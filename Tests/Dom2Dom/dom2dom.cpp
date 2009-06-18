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



#include <cstdio>
#include <direct.h>
#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#include <strstream.h>
#else
#include <iostream>
#include <strstream>
#endif
#include <vector>



// This is here for memory leak testing. 
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanFileOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>



#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XMLSupport/FormatterTreeWalker.hpp>



#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XSLT/XSLTResultTarget.hpp>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheetDefault.hpp>



#include <xalanc/Harness/XalanXMLFileReporter.hpp>
#include <xalanc/Harness/XalanFileUtility.hpp>



#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMFormatterWalker.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>



XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)



// Just hoist everything...
XALAN_CPP_NAMESPACE_USE



void
printArgOptions()
{
    cerr << endl
         << "dom2dom dirname [-out]"
         << endl
         << endl
         << "dirname        (base directory for testcases)"
         << endl
         << "-out dirname   (base directory for output)"
         << endl;
}



bool
getParams(
            int                 argc, 
            char*               argv[],
            XalanFileUtility&       f,
            XalanDOMString&     basedir,
            XalanDOMString&     outdir)
{
    MemoryManager& theManager = XalanMemMgrs::getDefaultXercesMemMgr();

    bool fSuccess = true;   // Used to continue argument loop
    bool fSetOut = true;    // Set default output directory

    // Insure that required "-base" argument is there.
    if (argc == 1 || argv[1][0] == '-')
    {
        printArgOptions(); 
        return false;
    }
    else
    {
        XalanDOMString dir(XalanFileUtility::s_pathSep, theManager);
        dir += XalanDOMString(argv[1], theManager);
        if (f.checkDir( dir ))
        {
            assign(basedir, XalanDOMString(argv[1], theManager));
            insert(basedir, 0, XalanFileUtility::s_pathSep);
        }
        else
        {
            cout << endl << "Given base directory \"" << argv[1] << "\" does not exist" << endl;
            printArgOptions();
            return false;
        }
    }

    // Get the rest of the arguments in any order.
    for (int i = 2; i < argc && fSuccess == true; ++i)
    {
        if(!strcmp("-out", argv[i]))
        {
            ++i;
            if(i < argc && argv[i][0] != '-')
            {
                assign(outdir, XalanDOMString(argv[i],theManager));
                insert(outdir, 0, XalanDOMString("\\",theManager));
                append(outdir, XalanDOMString("\\",theManager));
                f.checkAndCreateDir(outdir);
                fSetOut = false;
            }
            else
            {
                printArgOptions();
                fSuccess = false;
            }
        }
        else
        {
            printArgOptions();
            fSuccess = false;
        }

    } // End of for-loop

    // Do we need to set the default output directory??
    if (fSetOut)
    {
        const XalanDOMString::size_type     ii = lastIndexOf(basedir, XalanFileUtility::s_pathSep[0]);
        substring(basedir,outdir, 0, ii+1 );
        append(outdir, "Dom2Dom-RESULTS\\");
        f.checkAndCreateDir(outdir);
    }
    
    // Add the path seperator to the end of the base directory
    append(basedir, XalanFileUtility::s_pathSep);

    return fSuccess;
}



FormatterListener* 
getXMLFormatter(bool                    shouldWriteXMLHeader,
                bool                    stripCData,
                bool                    escapeCData,
                PrintWriter&            resultWriter,
                int                     indentAmount,
                const XalanDOMString&   mimeEncoding,
                const StylesheetRoot*   stylesheet)
{
    FormatterListener*  formatter = 0;

        XalanDOMString  version;
        bool            outputIndent = false;
        XalanDOMString  mediatype;
        XalanDOMString  doctypeSystem;
        XalanDOMString  doctypePublic;
        XalanDOMString  standalone;

        if (stylesheet != 0)
        {

            stylesheet->getOutputVersion(version);

            stylesheet->getOutputMediaType(mediatype);
            stylesheet->getOutputDoctypeSystem(doctypeSystem);
            stylesheet->getOutputDoctypePublic(doctypePublic);
            stylesheet->getOutputStandalone(standalone);
        }

        FormatterToXML* const   fToXML =
            new FormatterToXML(
                    resultWriter,
                    version,
                    outputIndent,
                    indentAmount,
                    mimeEncoding,
                    mediatype,
                    doctypeSystem,
                    doctypePublic,
                    true,   // xmlDecl
                    standalone);

        fToXML->setShouldWriteXMLHeader(shouldWriteXMLHeader);
        fToXML->setStripCData(stripCData);
        fToXML->setEscapeCData(escapeCData);

        formatter = fToXML;
        return formatter;
}



int
runTests(
          int       argc,
          char*     argv[])
{
    MemoryManager&  theManager = XalanMemMgrs::getDefaultXercesMemMgr();

    XalanFileUtility    f(theManager);

    XalanDOMString  category(theManager);   // Test all of base dir by default
    XalanDOMString  baseDir(theManager);    
    XalanDOMString  outputRoot(theManager);
    
    if (getParams(argc, argv, f, baseDir, outputRoot) == true)
    {
        typedef XalanFileUtility::FileNameVectorType        FileNameVectorType;

        // Get the list of Directories that are below perf
        FileNameVectorType  dirs(theManager);
        f.getDirectoryNames(baseDir, dirs);

        // Generate Unique Run id. (Only used to name the result logfile.)
        XalanDOMString UniqRunid(theManager);
        f.generateUniqRunid(UniqRunid);

        // Defined basic constants for file manipulation 

        const XalanDOMString  resultFilePrefix("dom2dom", theManager);
        XalanDOMString  resultsFile(outputRoot, theManager);
        resultsFile += resultFilePrefix;
        resultsFile += UniqRunid;
        resultsFile += XalanFileUtility::s_xmlSuffix;
        
        XalanXMLFileReporter    logFile(theManager, resultsFile);
        logFile.logTestFileInit("Dom2Dom Testing: Treating all inputs and outputs as DOM's. ");

        try
        {
            XalanTransformer        transformEngine(theManager);
                        
            XercesDOMSupport domSupport(theManager);
            XercesParserLiaison parserLiaison(theManager, domSupport);
                    
            // Specify the "test" directory for both input and output.
            const XalanDOMString  xMan("dtod");
            XalanDOMString  theOutputDir(outputRoot, theManager);
            theOutputDir += xMan;
            f.checkAndCreateDir(theOutputDir);

            // Get the files found in the test directory
            FileNameVectorType  files(theManager);
            f.getTestFileNames(baseDir, xMan,true, files);

            for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
            {
                // Output file name to result log and console.
                logFile.logTestCaseInit(files[i]);
                cout << files[i] << endl;

                // Set up the input/output files.
                XalanDOMString  theXSLFile( baseDir, theManager);
                theXSLFile += xMan;
                theXSLFile += XalanFileUtility::s_pathSep;
                theXSLFile += files[i];
                XalanDOMString  theXMLFile(theManager);
                f.generateFileName(theXSLFile,"xml", theXMLFile);
                XalanDOMString  theOutput (outputRoot, theManager);
                theOutput += xMan ;
                theOutput += XalanFileUtility::s_pathSep;
                theOutput += files[i]; 
                XalanDOMString  theOutputFile(theManager);
                f.generateFileName(theOutput, "out", theOutputFile);

                XALAN_USING_XERCES(DOMDocument)
                XALAN_USING_XERCES(DOMImplementation)

                // Use a Xerces Dom document to create the XSLTResultTarget. 
                const XalanAutoPtr<DOMDocument>     theDocument(DOMImplementation::getImplementation()->createDocument());
                assert(theDocument.get() != 0);

                FormatterToXercesDOM    theFormatter(theDocument.get(), 0);

                XSLTResultTarget    domResultTarget(theFormatter);

                const XSLTInputSource   xslInputSource(theXSLFile);
                const XSLTInputSource   xmlInputSource(theXMLFile);
                    
                const XalanCompiledStylesheet*  compiledSS = 0;

                int theResult = transformEngine.compileStylesheet(
                        xslInputSource,
                        compiledSS);

                if (theResult != 0)
                {
                    logFile.logTestCaseClose("Done","Fail");
                    cerr << "XalanError: \n" << transformEngine.getLastError();
                }
                else
                {
                    // Transform using compiled stylesheet.
                    theResult =
                            transformEngine.transform(xmlInputSource, compiledSS, domResultTarget);

                    if(theResult != 0)
                    {
                        logFile.logTestCaseClose("Done","Fail");
                        cerr << "XalanError: \n" << transformEngine.getLastError();
                    }
                    else
                    {
                        const XalanDOMString    mimeEncoding("UTF-8", theManager);
                        const XalanDOMString    encoding("UTF-8", theManager);

                        XalanFileOutputStream myOutput(theOutputFile, theManager);
                        XalanOutputStreamPrintWriter myResultWriter(myOutput);
                        FormatterListener* theFormatter = getXMLFormatter(true,true,true,
                                                                        myResultWriter,0,
                                                                        mimeEncoding,
                                                                        compiledSS->getStylesheetRoot());

                        XercesDOMFormatterWalker theTreeWalker(*theFormatter);
                        theTreeWalker.traverse(theDocument.get());

                        delete theFormatter;
                        logFile.logTestCaseClose("Done","Pass");
                    }   
                }
            }

            logFile.logTestFileClose("Dom2Dom Testing: ", "Done");
            logFile.close();
        }
        catch(...)
        {
            cerr << "Exception caught!!!" << endl << endl;
        }
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
