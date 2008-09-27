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



#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <xalanc/Harness/XalanXMLFileReporter.hpp>
#include <xalanc/Harness/XalanFileUtility.hpp>



XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)



static const char* const    excludeStylesheets[] =
{
    0
};



XALAN_USING_XALAN(XalanFileUtility)
XALAN_USING_XALAN(XalanDOMString)



inline bool
checkForExclusion(const XalanDOMString&     currentFile)
{
    for (int i = 0; excludeStylesheets[i] != 0; ++i)
    {   
        if (currentFile == XalanDOMString(excludeStylesheets[i]))
        {
            return true;
        }
    }

    return false;
}



void
setHelp(XalanFileUtility&   h)
{
    h.args.getHelpStream() << endl
         << "stressmem dirname [-out -sub]"
         << endl
         << endl
         << "dirname        (base directory for testcases)"
         << endl
         << "-out dirname   (base directory for output)"
         << endl
         << "-sub dirname (run files only from a specific directory)"
         << endl;
}



int
runTests(
            int     argc,
            char*   argv[])
{
    int theResult = 0;

    try
    {
        XalanFileUtility    h;

        setHelp(h);

        bool setGold = false;

        // Set the program help string,  then get the command line parameters.
        //
        if (h.getParams(argc, argv, "MEM-RESULTS", setGold) == true)
        {
            //XALAN_USING_XALAN(XalanSourceTreeDOMSupport)
            //XALAN_USING_XALAN(XalanSourceTreeParserLiaison)
            XALAN_USING_XALAN(XalanXMLFileReporter)

            typedef XalanFileUtility::FileNameVectorType    FileNameVectorType;

            // Get the list of Directories that are below perf
            const FileNameVectorType    dirs = h.getDirectoryNames(h.args.base);

            // Generate Unique Run id. (Only used to name the result logfile.)
            const XalanDOMString        UniqRunid = h.generateUniqRunid();

            // Defined basic constants for file manipulation 

            const XalanDOMString    resultFilePrefix(XalanDOMString("cpp-mem"));
            const XalanDOMString    resultsFile(h.args.output + resultFilePrefix + UniqRunid + XalanFileUtility::s_xmlSuffix);

            XalanXMLFileReporter    logFile(resultsFile);

            logFile.logTestFileInit("Memory Testing - Memory leaks detected during ConformanceTests. ");

            try
            {
                XALAN_USING_XALAN(XalanTransformer)

                bool foundDir = false;

                XalanTransformer    transformEngine;

                for(FileNameVectorType::size_type   j = 0; j < dirs.size(); ++j)
                {
                    // Skip all but the specified directory if the -sub cmd-line option was used.
                    //
                    if (h.args.sub.length() > 0 && dirs[j] != h.args.sub)
                    {
                        continue;
                    }                   
                        
                    // Check that output directory is there.
                    const XalanDOMString  theOutputDir = h.args.output + dirs[j];
                    h.checkAndCreateDir(theOutputDir);

                    const FileNameVectorType    files = h.getTestFileNames(h.args.base, dirs[j],true);
                    foundDir = true;

                    for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
                    {
                        if (checkForExclusion(files[i]) == false)
                        {
                            XALAN_USING_XALAN(XSLTInputSource)
                            XALAN_USING_XALAN(XSLTResultTarget)

                            // Output file name to result log and console.
                            logFile.logTestCaseInit(files[i]);
                            cout << files[i] << endl;

                            const XalanDOMString  theXSLFile = h.args.base + dirs[j] + XalanFileUtility::s_pathSep + files[i];
                            const XalanDOMString  theXMLFile = h.generateFileName(theXSLFile,"xml");
                            const XalanDOMString  theOutput =  h.args.output + dirs[j] + XalanFileUtility::s_pathSep + files[i]; 
                            const XalanDOMString  theOutputFile = h.generateFileName(theOutput, "out");

                            // Do a total end to end transform with no pre parsing of either xsl or xml files.
                            const XSLTResultTarget  theResultTarget(theOutputFile);

                            const XSLTInputSource   xslInputSource(theXSLFile);
                            const XSLTInputSource   xmlInputSource(theXMLFile);

                            const int   theResult =
                                    transformEngine.transform(xmlInputSource, xslInputSource, theResultTarget);

                            if(theResult != 0)
                            {
                                logFile.logTestCaseClose("Done","Fail");
                                cerr << "XalanError: \n" << transformEngine.getLastError();
                            }
                            else
                            {
                                logFile.logTestCaseClose("Done","Pass");
                            }
                        }
                    }
                }

                // Check to see if -sub cmd-line directory was processed correctly.
                if (!foundDir)
                {
                    cout << "Specified test directory: \"" << h.args.sub << "\" not found" << endl;
                }

                logFile.logTestFileClose("Memory Testing: ", "Done");
                logFile.close();
            }
            catch(...)
            {
                cerr << "Exception caught!!!" << endl << endl;

                theResult  = -1;
            }
        }
    }
    catch(...)
    {
        cerr << "Initialization of testing harness failed!" << endl << endl;

        theResult  = -1;
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
