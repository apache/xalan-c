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

#include "XalanFileUtility.hpp"



#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <climits>
#include <cstring>

#if defined(XALAN_WINDOWS)
#include <direct.h>
#define PATH_MAX _MAX_PATH
#define chdir _chdir
#define getcwd _getcwd
#define mkdir _mkdir
#else
#if !defined(PATH_MAX)
#define PATH_MAX 2000
#endif
#define DIR_MODE_BITS 509
#include <dirent.h>
#include <unistd.h>

#include <sys/stat.h>
#endif


#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#include <strstream.h>
#else
#include <iostream>
#include <strstream>
#endif

#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#if defined(XALAN_STRICT_ANSI_HEADERS)
using std::fclose;
using std::feof;
using std::ferror;
using std::fgets;
using std::fopen;
using std::localtime;
using std::sprintf;
using std::strftime;
using std::tm;
using std::time;
#endif



#include "xercesc/sax/ErrorHandler.hpp"
#include "xercesc/sax/SAXException.hpp"
#include "xercesc/sax/SAXParseException.hpp"



#include "xalanc/PlatformSupport/DirectoryEnumerator.hpp"
#include "xalanc/PlatformSupport/DOMStringHelper.hpp"
#include "xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp"
#include "xalanc/PlatformSupport/XalanFileOutputStream.hpp"
#include "xalanc/PlatformSupport/XalanUnicode.hpp"



#include "xalanc/XMLSupport/FormatterToXML.hpp"
#include "xalanc/XMLSupport/FormatterTreeWalker.hpp"



#include "xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp"
#include "xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp"
#include "xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp"



#include "xalanc/XSLT/StylesheetRoot.hpp"



#include "xalanc/XalanTransformer/XalanCompiledStylesheet.hpp"
#include "xalanc/XalanTransformer/XalanTransformer.hpp"



#include "XalanXMLFileReporter.hpp"



XALAN_CPP_NAMESPACE_BEGIN




const char* const   xalanNodeTypes[] =
{
    "UNKNOWN_NODE",
    "ELEMENT_NODE",
    "ATTRIBUTE_NODE",
    "TEXT_NODE",
    "CDATA_SECTION_NODE",
    "ENTITY_REFERENCE_NODE",
    "ENTITY_NODE",
    "PROCESSING_INSTRUCTION_NODE",
    "COMMENT_NODE",
    "DOCUMENT_NODE",
    "DOCUMENT_TYPE_NODE",
    "DOCUMENT_FRAGMENT_NODE",
    "NOTATION_NODE"
};



XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)


const XalanDOMString    XalanFileUtility::s_emptyString(XalanMemMgrs::getDummyMemMgr());



XalanFileUtility::reportStruct::reportStruct(MemoryManager&     theManager) :
    theDrive(theManager),
    testOrFile(theManager),
    xmlFileURL(theManager),
    xslFileURL(theManager),
    xmlFormat(theManager),
    msg(0),
    currentNode(theManager),
    actual(theManager),
    expected(theManager),
    pass(0),
    fail(0),
    nogold(0)
{
}



void
XalanFileUtility::reportStruct::reset()
{
    testOrFile.clear();
    msg = "";
    currentNode.clear();
    actual.clear();
    expected.clear();
}



XalanFileUtility::cmdParams::cmdParams(MemoryManager&   theManager) :
    help(),
    base(theManager),
    output(theManager),
    gold(theManager),
    sub(theManager),
    source(0),
    skip(false),
    iters(0)
{
}



const char*
XalanFileUtility::cmdParams::getHelpMessage()
{
    help << '\0';

    const char* const   data = help.str();

#if defined(HPUX)
   help.rdbuf() -> freeze(false);
#else
    help.freeze(false);
#endif

    return data;
}



XalanFileUtility::XalanFileUtility(MemoryManager&   theManager) :
    data(theManager),
    args(theManager),
    m_buffer(theManager),
    m_verbose(false)
{
    cout << endl
         << "Using Xalan version "
         << XALAN_FULLVERSIONDOT
         << endl
         << "Using Xerces version "
         << XERCES_FULLVERSIONDOT
         << endl
         << endl;
}



XalanFileUtility::~XalanFileUtility()
{
}



#if !defined(XALAN_WINDOWS)
XalanDOMString&
XalanFileUtility::getDrive(XalanDOMString&  theResult)
{
    theResult.erase();

    return theResult;
}
#else
XalanDOMString&
XalanFileUtility::getDrive(XalanDOMString&  theResult)
{
    const char temp[] =
    {
        char(_getdrive() + 'A' - 1),
        ':',
        '\0'
    };
    theResult.assign(temp, sizeof(temp) - 1);

    return theResult;
}
#endif


bool
XalanFileUtility::getParams(
            int             argc,
            char*           argv[],
            const char*     outDir,
            bool            fsetGold)
{
    bool fSuccess = true;   // Used to continue argument loop
    bool fsetOut = true;    // Set default output directory, set to false if data is provided

    args.skip = true;       // Default values for performance testing parameters.
    args.iters = 3;         

    // Insure that required "-base" argument is there.
    //
    if (argc == 1 || argv[1][0] == '-')
    {
        cout << args.getHelpMessage();  
        return false;
    }
    else
    {
        if (checkDir(XalanDOMString(argv[1], getMemoryManager())))
        {
            args.base.assign(XalanDOMString(argv[1], getMemoryManager()));
        }
        else
        {
            cout << endl << "Given base directory \"" << argv[1] << "\" does not exist" << endl;
            cout << args.getHelpMessage();
            return false;
        }
    }

    // Get the rest of the arguments.
    //
    for (int i = 2; i < argc && fSuccess == true; ++i)
    {
        if(!strcmp("-out", argv[i]))
        {
            ++i;
            if(i < argc && argv[i][0] != '-')
            {
                args.output.assign(XalanDOMString(argv[i], getMemoryManager()));
                args.output.append(s_pathSep);
                checkAndCreateDir(args.output);
                fsetOut = false;
            }
            else
            {
                cout << args.getHelpMessage();
                fSuccess = false;
            }
        }
        else if(!strcmp("-gold", argv[i]))
        {
            ++i;
            if(i < argc && argv[i][0] != '-')
            {
                args.gold.assign(XalanDOMString(argv[i], getMemoryManager()));

                if ( !checkDir(args.gold) )
                {   
                    TranscodeToLocalCodePage(args.gold, m_buffer, true);

                    cout << "Given Gold dir - " << &*m_buffer.begin() << " - does not exist" << endl;
                    fSuccess = false;
                }

                args.gold.append(s_pathSep);
                fsetGold = false;
            }
            else
            {
                cout << args.getHelpMessage();
                fSuccess = false;
            }
        }
        else if(!strcmp("-source", argv[i]))
        {
            ++i;
            if(i < argc && argv[i][0] != '-')
            {
                if (strcmp(argv[i],"XPL") == 0)
                {
                    args.source = 1;
                    outDir = "DOM-XALAN";
                }
                else if (strcmp(argv[i], "DOM") == 0)
                {
                    args.source = 2;
                    outDir = "DOM-XERCES";
                }
                else
                {
                    cout << args.getHelpMessage();
                    fSuccess = false;
                }
            }
            else
            {
                cout << args.getHelpMessage();
                fSuccess = false;
            }
        }
        else if(!strcmp("-sub", argv[i]))
        {
            ++i;
            if(i < argc && argv[i][0] != '-')
            {
                args.sub.assign(XalanDOMString(argv[i], getMemoryManager()));
            }
            else
            {
                cout << args.getHelpMessage();
                fSuccess = false;
            }
        }
        else if(!strcmp("-i", argv[i]))
        {
            args.skip = false;
        }
        else if(!strcmp("-iter", argv[i]))
        {
            ++i;
            
            // Make sure number is there and is greater then zero
            if(i < argc && atol(argv[i]) > 0)
            {
                args.iters = atol(argv[i]);
            }
            else
            {
                cout << args.getHelpMessage();
                fSuccess = false;
            }
        }
        else
        {
            cout << args.getHelpMessage();
            fSuccess = false;
        }

    } // End of for-loop

    // Do we need to set the default output directory??
    //
    if (fsetOut)
    { 
        const XalanDOMString::size_type     ii =
            lastIndexOf(args.base, s_pathSep[0]);

        if (ii < args.base.length())
        {
            args.output.assign(args.base, 0, ii + 1);
        }

        args.output.append(XalanDOMString(outDir, getMemoryManager()));
        checkAndCreateDir(args.output);
        args.output.append(s_pathSep); 

    }
    // Do we need to set the default gold directory??
    //
    if (fsetGold)
    {
        args.gold = args.base;
        args.gold.append(XalanDOMString("-gold", getMemoryManager()));
        if ( !checkDir(args.gold) )
        {   
            TranscodeToLocalCodePage(args.gold, m_buffer, true);
            cout << "Assumed Gold dir - " << &*m_buffer.begin() << " - does not exist" << endl;
            fSuccess = false;
        }
        args.gold.append(s_pathSep);
    }
    
    // Add the path seperator to the end of the base directory 
    // here after we've finished using it for all directory creation.
    //
    args.base.append(s_pathSep);
    
    return fSuccess;
}



//  This routine retrieves test file names from specified directories.
//  Inputs: baseDir:    typically "conf" or "perf"
//          relDir:     sub-directory to search.
//
//  Notes:  It builds the searchSpecification by concatenating all the 
//          necessary components.
//
XalanFileUtility::FileNameVectorType&
XalanFileUtility::getTestFileNames(
            const XalanDOMString&   baseDir,
            const XalanDOMString&   relDir,
            bool                    useDirPrefix,
            FileNameVectorType&     theFiles)
{
    char buffer3[PATH_MAX];
    getcwd(buffer3, PATH_MAX);

    const XalanDOMString    searchSuffix("*.xsl", getMemoryManager());
    XalanDOMString  searchSpecification(getMemoryManager());

    // Allow directory search w/o mandating files start with directory name. Required for files
    // garnered from XSLTMARK performance directory exm.
    if (useDirPrefix)
    {
        searchSpecification.assign(baseDir);
        searchSpecification += relDir;
        searchSpecification += s_pathSep;
        searchSpecification += relDir;
        searchSpecification += searchSuffix;
    }
    else
    {
        searchSpecification.assign(baseDir);
        searchSpecification += relDir;
        searchSpecification += s_pathSep;
        searchSpecification += searchSuffix;
    }


    DirectoryEnumeratorFunctor<
        FileNameVectorType,
        XalanDOMString>     theEnumerator(getMemoryManager());

    theEnumerator(searchSpecification, theFiles);

    chdir(buffer3);

    return theFiles;
}

/*  This routine retrieves all sub-directories from the specified directories.
//  Inputs: rootDirectory:  typically "conf" or "perf"
//
//  Notes:  The searchSpecification in this case is just "*". 
//                                                                          */  
XalanFileUtility::FileNameVectorType&
XalanFileUtility::getDirectoryNames(
            const XalanDOMString&                   rootDirectory,
            XalanFileUtility::FileNameVectorType&   theFiles)
{
    char buffer2[PATH_MAX];
    getcwd(buffer2, PATH_MAX);

    const XalanDOMString    dirSpec("*", getMemoryManager());

    DirectoryEnumeratorFunctor<
        FileNameVectorType,
        XalanDOMString,
        DirectoryFilterPredicate>   theEnumerator(getMemoryManager());

    theEnumerator(
        XalanDOMString(
            rootDirectory,
            getMemoryManager()),
        XalanDOMString(
            dirSpec,
            getMemoryManager()), 
        theFiles);
    
    chdir(buffer2);

    return theFiles;
}


bool XalanFileUtility::checkDir(const XalanDOMString&    directory )
{
    char buffer[PATH_MAX];

    getcwd(buffer, PATH_MAX);

    bool    fResult = false;

    TranscodeToLocalCodePage(directory, m_buffer, true);

    if ( !chdir(&*m_buffer.begin()) )
    {
        fResult = true;
    }

    chdir(buffer);

    return fResult;
}


void XalanFileUtility::checkAndCreateDir(const XalanDOMString&   directory)
{
    char buffer[PATH_MAX];

    getcwd(buffer, PATH_MAX);

    TranscodeToLocalCodePage(directory, m_buffer, true);

    const char* const   theDir = &*m_buffer.begin();
    assert(theDir != 0);

    if (chdir(theDir))
    {
        //cout << "Couldn't change to " << directory << ", will create it." << endl;
#if defined(XALAN_WINDOWS_DIR_FUNCTIONS)
        if (!mkdir(theDir))
#else
        if (!mkdir(theDir, DIR_MODE_BITS))
#endif
        {
            if (m_verbose == true)
            {
                cout << theDir << " created." << endl;
            }
        }
        else
        {
            cout << theDir << " NOT created." << endl;
        }
    }

    chdir(buffer);
}

/*  This routine generates file names based on the provide suffix
//  Inputs: theXMLFileName: typically "conf" or "perf"
//          suffix:         typically "xsl" or "out".
//
//  Notes:  
*/  

XalanDOMString&
XalanFileUtility::generateFileName(
            const XalanDOMString&   theXMLFileName,
            const char*             suffix,
            XalanDOMString&         targetFile,
            bool*                   status)
{
    const XalanDOMString::size_type     thePeriodIndex =
        lastIndexOf(theXMLFileName, XalanUnicode::charFullStop);

    if (thePeriodIndex != XalanDOMString::npos)
    {
        targetFile.assign(theXMLFileName, 0, thePeriodIndex + 1);

        targetFile += XalanDOMString(suffix, getMemoryManager());
    }

    // Check the .xml file exists.
    if (!strcmp(suffix, "xml"))
    {
        TranscodeToLocalCodePage(targetFile, m_buffer, true);
        const char* const   theFileName = &*m_buffer.begin();
        assert(theFileName != 0);

        FILE* fileHandle = fopen(theFileName, "r");
        if (fileHandle == 0)
        {
            cout << "TEST ERROR: File Missing: " << theFileName << endl;

            if (status != 0)
            {
                *status = false;
            }
        }
        else
        {
            fclose(fileHandle);
        }
    }

    return targetFile;
}


/*  This routine generates a Unique Runid. 
//  Inputs: None
//          
//  Notes: The format is mmddhhmm. For example
//         03151046 is "Mar 15 10:46"   
*/

XalanDOMString&
XalanFileUtility::generateUniqRunid(XalanDOMString& theResult)
{
    struct tm *newtime;
    time_t long_time;
    char tmpbuf[10];

    time( &long_time );                /* Get time as long integer. */
    newtime = localtime( &long_time ); /* Convert to local time. */

    strftime( tmpbuf, 10,"%m%d%H%M",newtime );

    theResult.assign(tmpbuf);

    return theResult;
}


//  This routine gets Xerces Version number. It's used to put the Xerces Version
//  into the output xml results file as an attribute of 'PerfData' element.
//  Inputs: None
//              

XalanDOMString&
XalanFileUtility::getXercesVersion(XalanDOMString& theResult)
{
    theResult.assign(gXercesFullVersionStr);

    return theResult;
}

/*  This routine creates a FormatterToXML FormatterListener. This is used to format
//  the output DOM so a comparision can be done with the expected GOLD file. 
//  Inputs: None
//              
*/


FormatterListener* 
XalanFileUtility::getXMLFormatter(
            PrintWriter&            resultWriter,
            int                     indentAmount,
            const XalanDOMString&   mimeEncoding,
            const StylesheetRoot*   stylesheet)
{
    XalanDOMString  version(getMemoryManager());
    bool            outputIndent= 0;
    XalanDOMString  mediatype(getMemoryManager());
    XalanDOMString  doctypeSystem(getMemoryManager());
    XalanDOMString  doctypePublic(getMemoryManager());
    XalanDOMString  standalone(getMemoryManager());

    if (stylesheet != 0)
    {
        XalanDOMString  theBuffer(getMemoryManager());

        version = stylesheet->getOutputVersion(theBuffer);

        mediatype = stylesheet->getOutputMediaType(theBuffer);
        doctypeSystem = stylesheet->getOutputDoctypeSystem(theBuffer);
        doctypePublic = stylesheet->getOutputDoctypePublic(theBuffer);
        standalone = stylesheet->getOutputStandalone(theBuffer);
        outputIndent = stylesheet->getOutputIndent();
    }

    return FormatterToXML::create(
                    resultWriter.getMemoryManager(),
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
}


/*  This routine is used to compares the results of a transform and report the results.
//  When a failure is detected the 'data' structure used to report detailed info about 
//  a failure is filled in.
//  Inputs: 
//      goldFile    - Name of gold file
//      outputFile  - Name of result file.
//      logfile     - Name of log file reporter.
//      
//  Returns: 
//      Void
*/
void
XalanFileUtility::checkResults(
            const XalanDOMString&   outputFile, 
            const XalanDOMString&   goldFile, 
            XalanXMLFileReporter&   logfile)
{
    int ambgFlag = data.nogold; // get the current number of tests w/o gold files.

    // Compare the results, report success if compareSerializedResults returns true.
    if(compareSerializedResults(outputFile, goldFile))
    {
        cout << "Passed: " << data.testOrFile << endl;
        logfile.logCheckPass(data.testOrFile);
        data.pass += 1;
    }
    else
    {
        typedef XalanXMLFileReporter::Hashtable  Hashtable;

        // if the compairson fails gather up the failure data and determine if it failed 
        // due to bad output or missing Gold file. Lastly, log the failure.
        Hashtable   attrs(getMemoryManager());
        Hashtable   actexp(getMemoryManager());

        reportError();

        attrs.insert(XalanDOMString("reason", getMemoryManager()), XalanDOMString(data.msg, getMemoryManager()));
        attrs.insert(XalanDOMString("atNode", getMemoryManager()), data.currentNode);
        actexp.insert(XalanDOMString("exp", getMemoryManager()),data.expected);
        actexp.insert(XalanDOMString("act", getMemoryManager()), data.actual);

        actexp.insert(XalanDOMString("xsl", getMemoryManager()), data.xslFileURL);
        actexp.insert( XalanDOMString("xml", getMemoryManager()),  data.xmlFileURL);
        actexp.insert( XalanDOMString("result", getMemoryManager()),  outputFile );
        actexp.insert( XalanDOMString("gold", getMemoryManager()),  goldFile);

        if (ambgFlag < data.nogold)
        {
            logfile.logCheckAmbiguous(data.testOrFile);
        }
        else
        {
            logfile.logCheckFail(data.testOrFile, attrs, actexp);
        }

    }
}

void
XalanFileUtility::checkAPIResults(
            const XalanDOMString&   actual, 
            const XalanDOMString&   expected, 
            const char*             msg,
            XalanXMLFileReporter&        logfile,
            const XalanDOMString&   outputFile, 
            const XalanDOMString&   goldFile,
            bool                    containsOnly)
{
    if(actual == expected ||
       (containsOnly == true && indexOf(actual, expected) != XalanDOMString::npos))
    {
        data.pass += 1;
        cout << "Passed: " << data.testOrFile << endl;
        logfile.logCheckPass(data.testOrFile);
    }
    else
    {   data.actual = actual;
        data.expected = expected;
        data.currentNode = "API Test";
        data.msg = msg;
        data.fail += 1;

        reportError();

        typedef XalanXMLFileReporter::Hashtable  Hashtable;

        Hashtable   actexp(getMemoryManager());

        actexp.insert( XalanDOMString("exp", getMemoryManager()), expected);
        actexp.insert( XalanDOMString("act", getMemoryManager()), actual);
        actexp.insert( XalanDOMString("xsl", getMemoryManager()), data.xslFileURL);
        actexp.insert( XalanDOMString("xml", getMemoryManager()), data.xmlFileURL);
        actexp.insert( XalanDOMString("result", getMemoryManager()), outputFile);
        actexp.insert( XalanDOMString("gold", getMemoryManager()), goldFile);

        // Todo: Need to determine if I should check for missing gold in these cases.
        logfile.logCheckFail(data.testOrFile, actexp);
    }
}



/*  This routine compares the results of a transform with the gold file.
//  It in turn call the domCompare routine to do the actual comparision. 
//  Inputs: 
//      gold - Dom tree for the expected results
//      doc  - Dom tree created during transformation
//      filename - Current filename
//      
//  Returns: 
//      Void
//      
*/
void
XalanFileUtility::checkDOMResults(
            const XalanDOMString&           theOutputFile, 
            const XalanCompiledStylesheet*  compiledSS, 
            const XalanSourceTreeDocument*  dom,
            const XSLTInputSource&          goldInputSource,
            XalanXMLFileReporter&           logfile)
{
    const int   ambgFlag = data.nogold;

    const XalanDOMString    mimeEncoding("", getMemoryManager());

    XalanFileOutputStream           myOutput(theOutputFile, getMemoryManager());
    XalanOutputStreamPrintWriter    myResultWriter(myOutput);

    FormatterListener* const    theFormatter =
        getXMLFormatter(
            myResultWriter,
            0,
            mimeEncoding,
            compiledSS->getStylesheetRoot());

    FormatterTreeWalker     theTreeWalker(*theFormatter, getMemoryManager());

    theTreeWalker.traverse(dom);

    XalanDestroy(
        getMemoryManager(),
        *theFormatter);

    XalanSourceTreeDOMSupport       domSupport;
    XalanSourceTreeParserLiaison    parserLiaison(domSupport, getMemoryManager());
    
    domSupport.setParserLiaison(&parserLiaison);

    const XalanDocument* const  goldDom =
        parserLiaison.parseXMLStream(goldInputSource);

    if (domCompare(*goldDom, *dom))
    {
        cout << "Passed: " << data.testOrFile << endl;
        logfile.logCheckPass(data.testOrFile);
        data.pass += 1;
    }
    else
    {
        typedef XalanXMLFileReporter::Hashtable  Hashtable;

        // if the compairson fails gather up the failure data and determine if it failed 
        // due to bad output or missing Gold file. Lastly, log the failure.
        Hashtable attrs(getMemoryManager());
        Hashtable actexp(getMemoryManager());

        reportError();

        attrs.insert(
            XalanDOMString("reason", getMemoryManager()), 
            XalanDOMString(data.msg, getMemoryManager()));

        attrs.insert(
            XalanDOMString("atNode", getMemoryManager()),
            data.currentNode);

        actexp.insert(
            XalanDOMString("exp", getMemoryManager()),
            data.expected);

        actexp.insert(
            XalanDOMString("act", getMemoryManager()),
            data.actual);

        if (ambgFlag < data.nogold)
        {
            logfile.logCheckAmbiguous(data.testOrFile);
        }
        else
        {
            logfile.logCheckFail(data.testOrFile, attrs, actexp);
        }

    }
}



class DummyErrorHandler : public XERCES_CPP_NAMESPACE_QUALIFIER ErrorHandler
{
    virtual void
    warning(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& exc)
    {
        throw exc;
    }

    virtual void
    error(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& exc)
    {
        throw exc;
    }

    virtual void
    fatalError(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& exc)
    {
        throw exc;
    }

    virtual void
    resetErrors()
    {
    }
};



/*  This routine takes the result file and gold file and parses them.
//  If either of the files fails to parse and a SAXException is throw,
//  then the files are compared using a char by char file compare,
//  otherwise the domCompare routine is used.
//  Inputs: 
//      outputFile:  Name of result file
//      goldFile:    Name of gold file
//      
//  Returns: 
//      True or False
//      
*/
bool
XalanFileUtility::compareSerializedResults(
            const XalanDOMString&   outputFile,
            const XalanDOMString&   goldFile)
{
    const XSLTInputSource   resultInputSource(outputFile, getMemoryManager());
    const XSLTInputSource   goldInputSource(goldFile, getMemoryManager());

    XalanSourceTreeDOMSupport       domSupport;
    XalanSourceTreeParserLiaison    parserLiaison(domSupport, getMemoryManager());

    domSupport.setParserLiaison(&parserLiaison);

    DummyErrorHandler   theErrorHandler;

    parserLiaison.setErrorHandler(&theErrorHandler);

    try
    {
        const XalanDocument* const  transformDom =
            parserLiaison.parseXMLStream(resultInputSource);
        assert(transformDom != 0);

        const XalanDocument* const  goldDom =
            parserLiaison.parseXMLStream(goldInputSource);
        assert(goldDom != 0);

        return domCompare(*goldDom, *transformDom);
    }
    // This exception is being reported prior to this Catch, however, however, I clarify that it's a SAX exception.
    // It's a good indication that the Gold file is not a valid XML.  When this happens the transform result needs
    // to be compared with the Gold,  with a character by character basis,  not via the DOM compair. 
    catch (const XERCES_CPP_NAMESPACE_QUALIFIER SAXException&)
    {
        if (m_verbose == true)
        {
            cout << "SAXException: Using fileCompare to check output.\n";
        }

        CharVectorType     goldFileVec(getMemoryManager());
        TranscodeToLocalCodePage(goldFile, goldFileVec, true);

        CharVectorType     outputFileVec(getMemoryManager());
        TranscodeToLocalCodePage(outputFile, outputFileVec, true);
        return fileCompare(c_str(goldFileVec), c_str(outputFileVec));
    }
}



/*  This routine is used to compare the results against the gold when one or both of 
//  fails to parse without throwing a SAXException. When a failure is detected the 'data' 
//  structure used to report detailed info about a failure is filled in.
//  Inputs: 
//      outputFile:  Name of result file
//      goldFile:    Name of gold file
//      
//  Returns: 
//      True or False
//      
*/
bool
XalanFileUtility::fileCompare(
            const char*     goldFile,
            const char*     outputFile)
{
    bool    retValue = true;

    const unsigned long     maxBuffer = 132;

    char rline[maxBuffer];  // declare buffers to hold single line from file
    char gline[maxBuffer];
    char temp[20];              // buffer to hold line number
    char lineNum = 1;

    // Set fail data incase there are i/o problems with the files to compare.
    data.expected = XalanDOMString(" ", getMemoryManager());
    data.actual = XalanDOMString(" ", getMemoryManager());
    data.currentNode = XalanDOMString("Line: 0", getMemoryManager());

    // Attempt to open the files. 
    FILE* const     result = fopen(outputFile, "r");
    FILE* const     gold = fopen(goldFile, "r");

    if (!result)
    {
        // If the result file fails to open report this as a failure.
        data.msg = "No Result (Transform failed)";
        data.fail += 1;
        retValue = false;
    }
    else if (!gold)
    {
        // If the gold file fails to open report this as ambiguous.
        data.msg = "No Gold file";
        data.nogold += 1;
        retValue = false;
    }
    else
    {
        // Start file comparison,  line by line..
        while(!feof(result) && !feof(gold) && retValue == true)
        {
		    gline[0] = '\0';
		    rline[0] = '\0';

		    fgets(gline, sizeof(gline), gold );
            fgets(rline, sizeof(rline), result );
            sprintf(temp,"%d",lineNum);

            if (ferror(gold) || ferror(result))
            {
                data.msg = "Read Error - Gold/Result file";
                data.currentNode = XalanDOMString("Line: ", getMemoryManager());
                data.currentNode += XalanDOMString(temp, getMemoryManager());
                retValue = false;
            }
            else
            {
                // Compare the lines character by charcter ....
                XalanSize_t     i = 0;
                while(i < strlen(gline) && retValue == true) 
                {
                    if (gline[i] == rline[i]) 
                    {
                        ++i;
                    }
                    else
                    {   // If there is a mismatch collect up the fail data and return false.  To ensure that 
                        // the results can be seen in the browser enclose the actual/expected in CDATA Sections.

                        data.msg = "Text based comparison failure";

                        try
                        {
                            data.expected += XalanDOMString(gline, getMemoryManager());
                        }
                        catch(const XalanDOMString::TranscodingError&)
                        {
                            data.expected +=
                                XalanDOMString(
                                    "Unable to transcode expected data.",
                                    getMemoryManager());
                        }

                        try
                        {
                            data.actual += XalanDOMString(rline, getMemoryManager());
                        }
                        catch(const XalanDOMString::TranscodingError&)
                        {
                            data.actual +=
                                XalanDOMString(
                                    "Unable to transcode actual data.",
                                    getMemoryManager());
                        }

                        data.currentNode = XalanDOMString("Line: ", getMemoryManager());
                        data.currentNode += XalanDOMString(temp, getMemoryManager());
                        data.fail += 1;

                        retValue = false;
                    }
                }

                ++lineNum;
            }
        }
    }

    fclose(result);
    fclose(gold);

    return retValue;
}



/*  This routine performs a DOM Comparision. 
//  Inputs: 
//      gold - Dom tree for the expected results
//      doc  - Dom tree created during transformation
//      filename - Current filename
//      
//  Returns: 
//      True or False
//      
*/
bool 
XalanFileUtility::domCompare(
            const XalanNode&    gold,
            const XalanNode&    doc)
{
    const XalanNode::NodeType   docNodeType  = doc.getNodeType();
    const XalanNode::NodeType   goldNodeType = gold.getNodeType();

    const XalanDOMString&  docNodeName  = doc.getNodeName();    

    if (goldNodeType != docNodeType)
    {
        collectData(
            "NodeType mismatch.",
            docNodeName,
            XalanDOMString(xalanNodeTypes[docNodeType], getMemoryManager()),
            XalanDOMString(xalanNodeTypes[goldNodeType], getMemoryManager()));

        return false;
    }

    switch (goldNodeType)
    {
    case XalanNode::ELEMENT_NODE:   // ATTRIBUTE_NODEs are processed with diffElement().
        { 
            if (diffElement(gold, doc) == false) 
            {
                return false;
            }
        }
        break;

    case XalanNode::CDATA_SECTION_NODE:
    case XalanNode::TEXT_NODE:  
        {
            const XalanDOMString&   docNodeValue  = doc.getNodeValue();
            const XalanDOMString&   goldNodeValue = gold.getNodeValue();
            
            //debugNodeData(docNodeName, docNodeValue);
            
            if(goldNodeValue != docNodeValue)
            {
                collectData(
                    "Text node mismatch. ", 
                    docNodeName,
                    goldNodeValue,
                    docNodeValue);

                return false;
            }
        }
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        {
            const XalanDOMString&  goldNodeName = gold.getNodeName();

            if (goldNodeName != docNodeName)
            {
                collectData(
                    "processing-instruction target mismatch. ", 
                    docNodeName,
                    goldNodeName,
                    docNodeName);

                return false;
            }
            else
            {
                const XalanDOMString&   docNodeValue = doc.getNodeValue();
                const XalanDOMString&   goldNodeValue = gold.getNodeValue();

                if (goldNodeValue != docNodeValue)
                {
                    collectData(
                        "processing-instruction data mismatch. ", 
                        docNodeName,
                        goldNodeValue,
                        docNodeValue);

                    return false;
                }
            }
        }
        break;

    case XalanNode::COMMENT_NODE:
        {
            const XalanDOMString&   docNodeValue = doc.getNodeValue();
            const XalanDOMString&   goldNodeValue = gold.getNodeValue();

            if (goldNodeValue != docNodeValue)
            {
                collectData(
                    "comment data mismatch. ", 
                    docNodeName,
                    goldNodeValue,
                    docNodeValue);

                return false;
            }
        }
        break;

    case XalanNode::DOCUMENT_NODE:
        {
            //debugNodeData(docNodeName);

            const XalanNode *goldNextNode;
            const XalanNode *domNextNode;

            goldNextNode = gold.getFirstChild();
            domNextNode = doc.getFirstChild();

            if (0 != goldNextNode)
            {
                if(domCompare(*goldNextNode,*domNextNode) == false)
                {
                    return false;
                }
            }
        }
        break;

    case XalanNode::ENTITY_REFERENCE_NODE:
    case XalanNode::ENTITY_NODE:
    case XalanNode::DOCUMENT_TYPE_NODE:
    case XalanNode::DOCUMENT_FRAGMENT_NODE:
    case XalanNode::NOTATION_NODE:
    default:
        cerr << "Unexpected node type: " << goldNodeType << endl;

        return false;
    }

    // Need to process siblings.  Children are processed in diffElement, since
    // only they can have children in the XPath data model.
    const XalanNode* const  goldNextNode = gold.getNextSibling();
    const XalanNode* const  domNextNode = doc.getNextSibling();

    if (0 != goldNextNode)
    {
        if (0 != domNextNode)
        {
            if (domCompare(*goldNextNode, *domNextNode) == false)
            {
                return false;
            }
        }
        else
        {
            collectData(
                "Missing sibling node. ", 
                docNodeName,
                goldNextNode->getNodeName(),
                goldNextNode->getNodeName());

            return false;
        }
    }
    else if (0 != domNextNode)
    {
        collectData(
            "Extra sibling node. ", 
            docNodeName,
            domNextNode->getNodeName(),
            domNextNode->getNodeName());

        return false;
    }

    return true;
}



bool 
XalanFileUtility::domCompare(
            const XalanDocument&    gold,
            const XalanDocument&    doc)
{
    const XalanNode*    theGoldPos = &gold;
    const XalanNode*    theDocPos = &doc;

    bool    fEqual = true;

    do
    {
        fEqual = diffNode(theGoldPos, theDocPos);

        if (fEqual == true)
        {
            assert(theGoldPos != 0 && theDocPos != 0);

            const XalanNode*    nextGoldNode = theGoldPos->getFirstChild();
            const XalanNode*    nextDocNode = theDocPos->getFirstChild();

            bool    fBreak = false;

            while(
                nextGoldNode == 0 &&
                nextDocNode == 0 &&
                fBreak == false)
            {
                // Move to the next sibling of each node,
                // since we would get here only if both have
                // no children.
                nextGoldNode = theGoldPos->getNextSibling();
                nextDocNode = theDocPos->getNextSibling();

                // If there is no next sibling, move up to the
                // parent.  If one, but not both, has a sibling,
                // we'll end up back at the top of the do/while
                // loop and the difference will be reported.
                if(0 == nextGoldNode && 0 == nextDocNode)
                {
                    theGoldPos = theGoldPos->getParentNode();
                    theDocPos = theDocPos->getParentNode();

                    // If the parent is null, then we've reached
                    // the end of the document.  Note that if we
                    // got here, then there must also be a parent
                    // node in the document we're verifying, so we
                    // could simply assert that theDocPos is either
                    // null if theGoldPos is null, or it is not-null
                    // if theGoldPos is not-null.
                    if(0 == theGoldPos)
                    {
                        nextGoldNode = theGoldPos;

                        fBreak = true;
                    }

                    if(0 == theDocPos)
                    {
                        nextDocNode = theDocPos;

                        fBreak = true;
                    }
                }
            }

            theGoldPos = nextGoldNode;
            theDocPos = nextDocNode;
        }
    } while((theGoldPos != 0 || theDocPos != 0) && fEqual == true);


    return fEqual;
}



bool
XalanFileUtility::diffNode(
            const XalanNode&    gold,
            const XalanNode&    doc)
{
    const XalanNode::NodeType   docNodeType  = doc.getNodeType();
    const XalanNode::NodeType   goldNodeType = gold.getNodeType();

    const XalanDOMString&  docNodeName  = doc.getNodeName();    

    if (goldNodeType != docNodeType)
    {
        collectData(
            "NodeType mismatch.",
            docNodeName,
            XalanDOMString(xalanNodeTypes[docNodeType], getMemoryManager()),
            XalanDOMString(xalanNodeTypes[goldNodeType], getMemoryManager()));

        return false;
    }

    switch (goldNodeType)
    {
    case XalanNode::ELEMENT_NODE:   // ATTRIBUTE_NODEs are processed with diffElement().
        return diffElement2(gold, doc);
        break;

    case XalanNode::CDATA_SECTION_NODE:
    case XalanNode::TEXT_NODE:  
        {
            const XalanDOMString&   docNodeValue  = doc.getNodeValue();
            const XalanDOMString&   goldNodeValue = gold.getNodeValue();
            
            //debugNodeData(docNodeName, docNodeValue);
            
            if(goldNodeValue != docNodeValue)
            {
                collectData(
                    "Text node mismatch. ",
                    docNodeName,
                    goldNodeValue,
                    docNodeValue);

                return false;
            }
        }
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        {
            const XalanDOMString&  goldNodeName  = gold.getNodeName();

            if (goldNodeName != docNodeName)
            {
                collectData(
                    "processing-instruction target mismatch. ", 
                    docNodeName,
                    goldNodeName,
                    docNodeName);

                return false;
            }
            else
            {
                const XalanDOMString&   docNodeValue  = doc.getNodeValue();
                const XalanDOMString&   goldNodeValue = gold.getNodeValue();

                if (goldNodeValue != docNodeValue)
                {
                    collectData(
                        "processing-instruction data mismatch. ", 
                        docNodeName,
                        goldNodeValue,
                        docNodeValue);

                    return false;
                }
            }
        }
        break;

    case XalanNode::COMMENT_NODE:
        {
            const XalanDOMString&   docNodeValue  = doc.getNodeValue();
            const XalanDOMString&   goldNodeValue = gold.getNodeValue();

            if (goldNodeValue != docNodeValue)
            {
                collectData(
                    "comment data mismatch. ", 
                    docNodeName,
                    goldNodeValue,
                    docNodeValue);

                return false;
            }
        }
        break;

    case XalanNode::DOCUMENT_NODE:
        break;

    case XalanNode::ENTITY_REFERENCE_NODE:
    case XalanNode::ENTITY_NODE:
    case XalanNode::DOCUMENT_TYPE_NODE:
    case XalanNode::DOCUMENT_FRAGMENT_NODE:
    case XalanNode::NOTATION_NODE:
    default:
        cerr << "Unexpected node type: " << goldNodeType << endl;

        return false;
    }

    return true;
}



bool
XalanFileUtility::diffNode(
            const XalanNode*    gold,
            const XalanNode*    doc)
{
    if (gold != 0 && doc != 0)
    {
        return diffNode(*gold, *doc);
    }
    else if (gold != 0)
    {
        const XalanNode* const  parent =
            gold->getParentNode();

        collectData(
            "Missing sibling node. ",
            parent == 0 ? s_emptyString : parent->getNodeName(),
            s_emptyString,
            gold->getNodeName());

        return false;
    }
    else
    {
        assert(doc != 0 && gold == 0);

        const XalanNode* const  parent =
            doc->getParentNode();

        collectData(
            "Extra sibling node. ", 
            parent == 0 ? s_emptyString : parent->getNodeName(),
            doc->getNodeName(),
            s_emptyString);

        return false;
    }
}



/*  This routine compares two element nodes. 
//  Inputs: 
//      gold - Dom tree for the expected results
//      doc  - Dom tree created during transformation
//      filename - Current filenam
//      
//  Returns: 
//      True or False
//                      
*/

bool
XalanFileUtility::diffElement(
            const XalanNode&    gold,
            const XalanNode&    doc)
{
    assert(gold.getNodeType() == XalanNode::ELEMENT_NODE);
    assert(gold.getNodeType() == XalanNode::ELEMENT_NODE);

    const XalanDOMString&  docNodeName  = doc.getNodeName();    
    const XalanDOMString&  goldNodeName = gold.getNodeName();

    const XalanDOMString&  docNsUri  = doc.getNamespaceURI();
    const XalanDOMString&  goldNsUri = gold.getNamespaceURI();

    //debugNodeData(docNodeName);

    // This essentially checks 2 things, that the prefix and localname are the
    // same.  So specific checks of these items are not necessary.
    if (goldNodeName != docNodeName)
    {
        collectData(
            "Element mismatch. ", 
            docNodeName,
            goldNodeName,
            docNodeName);

        return false;
    }

    if ( goldNsUri != docNsUri)
    {
        collectData(
            "Element NamespaceURI mismatch. ",
            docNodeName,
            goldNsUri,
            docNsUri);

        return false;
    }

    // Get Attributes for each Element Node. 
    const XalanNamedNodeMap* const  goldAttrs = gold.getAttributes();
    const XalanNamedNodeMap* const  docAttrs  = doc.getAttributes();

    // Get number of Attributes
    const XalanSize_t  numGoldAttr = goldAttrs->getLength();
    const XalanSize_t  numDomAttr  = docAttrs ->getLength();

    /*
    // This needs to be uncommented if 'compare.exe' is to work. 
    // If this is the 'root' element strip off the xmlns:xml namespace attribute,
    // that is lurking around on the gold file, but not the dom.  This is necessary
    // only for the 'compare' test, that uses a pure DOM, that has not been serialized.
    //if (goldNodeName == XalanDOMString("root"))
    {
        numGoldAttr -= 1;
        XalanNode *gXMLAttr = goldAttrs->item(1);
    }
    */
    // Check that each Element has same number of Attributes. If they don't report error  
    if ( numGoldAttr == numDomAttr )
    {
        // Compare Attributes one at a time.
        for (XalanSize_t i = 0; i < numGoldAttr; ++i)
        {
            // Attribute order is irrelvant, so comparision is base on Attribute name.
            const XalanNode* const  gAttr = goldAttrs->item(i);
            const XalanDOMString&   goldAttrName = gAttr->getNodeName();

            const XalanNode* const  dAttr = docAttrs->getNamedItem(goldAttrName);

            if (dAttr != 0)
            {
                if( ! (diffAttr(gAttr, dAttr)) )
                    return false;
            }
            else
            {
                collectData(
                    "Element missing named Attribute. ",
                    docNodeName,
                    goldAttrName,
                    XalanDOMString("NOTHING", getMemoryManager()));

                return false;
            }
        }
    }
    else
    {
        XalanDOMString  numGoldStr(getMemoryManager());
        XalanDOMString  numDOMStr(getMemoryManager());

        collectData(
            "Wrong number of attributes. ",
            docNodeName,
            NumberToDOMString(numGoldAttr, numGoldStr),
            NumberToDOMString(numDomAttr, numDOMStr));

        return false;
    }

    const XalanNode*    goldNextNode = gold.getFirstChild();
    const XalanNode*    domNextNode = doc.getFirstChild();

    if (0 != goldNextNode)
    {
        if (0 != domNextNode)
        {
            if ( ! domCompare(*goldNextNode, *domNextNode) )
                return false;
        }
        else
        {
            collectData(
                "Element missing ChildNode. ", 
                docNodeName,
                XalanDOMString(goldNextNode->getNodeName(), getMemoryManager()),
                XalanDOMString("NOTHING", getMemoryManager()));

            return false;
        }
    }
    else if (domNextNode != 0)
    {
        // The result doc has additional Children. If the additional node is a text node
        // then gather up the text and print it out.
        if ( domNextNode->getNodeType() == XalanNode::TEXT_NODE)
        {
            collectData(
                "Result has additional Child node: ", 
                docNodeName,
                XalanDOMString("NOTHING", getMemoryManager()),       
                XalanDOMString(domNextNode->getNodeName(), getMemoryManager()).append(XalanDOMString("  \"", getMemoryManager()).append(
                XalanDOMString(domNextNode->getNodeValue(), getMemoryManager()).append(XalanDOMString("\"", getMemoryManager())))));
        }
        // Additional node is NOT text, so just print it's Name.
        else
        {
            collectData(
                "Result has additional Child node: ",
                docNodeName,
                XalanDOMString("NOTHING", getMemoryManager()),
                XalanDOMString(domNextNode->getNodeName(), getMemoryManager()));

        }

        return false;
    }

    return true;
}


bool
XalanFileUtility::diffElement2(
            const XalanNode&    gold,
            const XalanNode&    doc)
{
    assert(gold.getNodeType() == XalanNode::ELEMENT_NODE);
    assert(gold.getNodeType() == XalanNode::ELEMENT_NODE);

    const XalanDOMString&  docNodeName  = doc.getNodeName();    
    const XalanDOMString&  goldNodeName = gold.getNodeName();

    const XalanDOMString&  docNsUri  = doc.getNamespaceURI();
    const XalanDOMString&  goldNsUri = gold.getNamespaceURI();

    //debugNodeData(docNodeName);

    // This essentially checks 2 things, that the prefix and localname are the
    // same.  So specific checks of these items are not necessary.
    if (goldNodeName != docNodeName)
    {
        collectData(
            "Element mismatch. ", 
            docNodeName,
            goldNodeName,
            docNodeName);

        return false;
    }

    if ( goldNsUri != docNsUri)
    {
        collectData(
            "Element NamespaceURI mismatch. ",
            docNodeName,
            goldNsUri,
            docNsUri);

        return false;
    }

    // Get Attributes for each Element Node. 
    const XalanNamedNodeMap* const  goldAttrs = gold.getAttributes();
    assert(goldAttrs != 0);

    const XalanNamedNodeMap* const  docAttrs  = doc.getAttributes();
    assert(docAttrs != 0);

    // Get number of Attributes
    const XalanSize_t  numGoldAttr = goldAttrs->getLength();
    const XalanSize_t  numDomAttr  = docAttrs ->getLength();

    // Check that each Element has same number of Attributes. If they don't report error  
    if ( numGoldAttr == numDomAttr )
    {
        // Compare Attributes one at a time.
        for (XalanSize_t i = 0; i < numGoldAttr; ++i)
        {
            // Attribute order is irrelevant, so comparision is base on Attribute name.
            const XalanNode* const  gAttr = goldAttrs->item(i);
            const XalanDOMString&   goldAttrName = gAttr->getNodeName();

            const XalanNode* const  dAttr = docAttrs->getNamedItem(goldAttrName);

            if (dAttr != 0)
            {
                if( ! (diffAttr(gAttr, dAttr)) )
                    return false;
            }
            else
            {
                collectData(
                    "Element missing named Attribute. ",
                    docNodeName,
                    goldAttrName,
                    XalanDOMString("NOTHING", getMemoryManager()));

                return false;
            }
        }
    }
    else
    {
        XalanDOMString  numGoldStr(getMemoryManager());
        XalanDOMString  numDOMStr(getMemoryManager());

        collectData(
            "Wrong number of attributes. ",
            docNodeName,
            NumberToDOMString(numGoldAttr, numGoldStr),
            NumberToDOMString(numDomAttr, numDOMStr));

        return false;
    }

    return true;
}


/*  This routine compares two attribute nodes. 
//  Inputs: 
//      gAttr - attribute from Gold dom tree 
//      dAttr - attribute from Dom tree created during transformation
//      fileName - Current filenam
//      
//  Returns: 
//      True or False
//              
*/

bool XalanFileUtility::diffAttr(const XalanNode* gAttr, const XalanNode* dAttr)
{

    const XalanDOMString&   docAttrName  = dAttr->getNodeName();

    //debugAttributeData(goldAttrName);

    const XalanDOMString&   goldAttrValue = gAttr->getNodeValue();
    const XalanDOMString&   docAttrValue    = dAttr->getNodeValue();

    if (goldAttrValue != docAttrValue)
    {
        collectData(
            "Attribute Value mismatch. ",
            docAttrName,
            goldAttrValue,
            docAttrValue);

        return false;
    }

    const XalanDOMString&   goldAttrNsUri = gAttr->getNamespaceURI();
    const XalanDOMString&   docAttrNsUri    = dAttr->getNamespaceURI();

    if (goldAttrNsUri != docAttrNsUri)
    {
        collectData(
            "Attribute NamespaceURI mismatch. ", 
            docAttrName,
            goldAttrNsUri,
            docAttrNsUri);

        return false;
    }

    return true;
}

/*  This routine reports DOM comparison errors. 
//  Inputs: 
//      file    -   Name of current file
//      node    -   Current node that fails
//      msg     -   Failure message
//
*/
void
XalanFileUtility::reportError()
{

    cout << endl
         << "* Failed "
         << data.testOrFile
         << "  Error: "
         << data.msg
         << endl
         << "   "
         << "Processing Node: "
         << data.currentNode
         << endl
         << "   Expected:   "
         << data.expected
         << endl
         << "   Actual:     "
         << data.actual
         << endl
         << endl;
}


void
XalanFileUtility::debugNodeData(const XalanDOMString&    value)
{
    CharVectorType     valueVec(getMemoryManager());

    TranscodeToLocalCodePage(value, valueVec, true);

    cout << "Node is: " << c_str(valueVec) << endl;
}



void
XalanFileUtility::debugNodeData(
            const XalanDOMString&   node,
            const XalanDOMString&   value)
{
    CharVectorType     valueVec(getMemoryManager());
    TranscodeToLocalCodePage(value, valueVec, true);

    CharVectorType     nodeVec(getMemoryManager());
    TranscodeToLocalCodePage(node, nodeVec, true);

    cout << "Node is: " << c_str(nodeVec) << "   "
         << "Value is: \"" << c_str(valueVec) << "\"\n";
}



void
XalanFileUtility::debugAttributeData(const XalanDOMString&   value)
{
    CharVectorType      theTargetVector(getMemoryManager());

    TranscodeToLocalCodePage(value, theTargetVector, true);

    cout << "Attribute is: " << c_str(theTargetVector) << endl;
}


/*  This routine collects up data pertinent to a dom comparison failure. 
//  Inputs: 
//      errmsg:         Reason for the failure.
//      currentnode:    Node in the dom tree where the mismatch occured
//      expdata:        Expected data based on the Gold file.
//      actdata:        Actual data returned in the result file.
//  Returns: Void                       
*/
void 
XalanFileUtility::collectData(
            const char*             errmsg,
            const XalanDOMString&   currentnode,
            const XalanDOMString&   expdata,
            const XalanDOMString&   actdata)
{
    data.msg = errmsg;
    data.currentNode = currentnode;
    data.expected = expdata;
    data.actual = actdata;
    data.fail += 1;
}


/*  Routine prints the result to the console, as well as adds summary info into the logfile. 
//  Inputs: 
//      logfile:    Current log file
//      runid:      Unique runid
//  Returns: Void                       
*/
void
XalanFileUtility::reportPassFail(
            XalanXMLFileReporter&   logfile,
            const XalanDOMString&   runid)
{
    typedef XalanXMLFileReporter::Hashtable  Hashtable;

    Hashtable   runResults(getMemoryManager());

    char temp[10];

    // Create entrys that contain runid, xerces version, and numbers for Pass, Fail and No Gold.

    XalanDOMString theBuffer(getMemoryManager());

    runResults.insert(
        XalanDOMString("UniqRunid", getMemoryManager()),
        runid);

    runResults.insert(
        XalanDOMString("Xerces-Version ", getMemoryManager()),
        getXercesVersion(theBuffer));

    runResults.insert(
        XalanDOMString("BaseDrive ", getMemoryManager()), 
        XalanDOMString(getDrive(theBuffer), getMemoryManager()));

    runResults.insert(
        XalanDOMString("TestBase ", getMemoryManager()),
        XalanDOMString(args.base, getMemoryManager()));

    runResults.insert(
        XalanDOMString("xmlFormat ", getMemoryManager()),
        data.xmlFormat);

    sprintf(temp, "%ld", args.iters);

    runResults.insert(
        XalanDOMString("Iters ", getMemoryManager()),
        XalanDOMString(temp, getMemoryManager()));

    sprintf(temp, "%d", data.pass);

    runResults.insert(
        XalanDOMString("Passed", getMemoryManager()),
        XalanDOMString(temp, getMemoryManager()));
    
    sprintf(temp, "%d", data.fail);

    runResults.insert(
        XalanDOMString("Failed", getMemoryManager()),
        XalanDOMString(temp, getMemoryManager()));

    sprintf(temp, "%d", data.nogold);

    runResults.insert(
        XalanDOMString("No_Gold_Files", getMemoryManager()),
        XalanDOMString(temp, getMemoryManager()));

    logfile.logElementWAttrs(10, "RunResults", runResults, "xxx");  

    cout << "\nPassed " << data.pass;
    cout << "\nFailed " << data.fail;
    cout << "\nMissing Gold " << data.nogold << endl;

}

/*  Routine runs a stylesheet on the log file and displays the results in HTML. 
//  Inputs: 
//      xalan:          An instance of the transformer
//      resultsFile:    logfile 
//  Returns: Void                       
*/
void
XalanFileUtility::analyzeResults(XalanTransformer& xalan, const XalanDOMString& resultsFile)
{
    XalanDOMString paramValue(getMemoryManager());

    bool    fileStatus;

#if defined(AIX) || defined(SOLARIS) || defined(LINUX) || defined(HPUX)

    bool    pathStatus;
    CharVectorType     withPath(getMemoryManager());
    TranscodeToLocalCodePage(resultsFile, withPath, false);
    if (withPath[0] == '/')
        pathStatus=true;
    else
        pathStatus=false;
    
    char buffer5[PATH_MAX];
    XalanDOMString resultPath(getcwd(buffer5, PATH_MAX), getMemoryManager());
    append(resultPath, s_pathSep);
#endif
    

    // Pass the results .xml file as a parameter to the stylesheet.  It must be wrapped in single
    // quotes so that it is not considered an expression.
    //
  #if defined (AIX) || defined(SOLARIS) || defined(LINUX) || defined(HPUX)
    paramValue.assign(XalanDOMString("\'", getMemoryManager()));
    if ( !pathStatus )
        paramValue.append(resultPath);
    paramValue.append(resultsFile);
    paramValue.append(XalanDOMString("\'", getMemoryManager()));
  #else 
    paramValue.assign(XalanDOMString("'", getMemoryManager()));
    paramValue.append(resultsFile);
    paramValue.append(XalanDOMString("'", getMemoryManager()));
  #endif

    // Set the parameter
    //
    xalan.setStylesheetParam(
        XalanDOMString("testfile", getMemoryManager()),
        paramValue);

    // Generate the input and output file names.
    //
    XalanDOMString  theHTMLFile(getMemoryManager());
    generateFileName(resultsFile,"html", theHTMLFile, &fileStatus);
    
    XalanDOMString  theStylesheet(getMemoryManager());
    theStylesheet += args.base;
    theStylesheet += XalanDOMString("cconf.xsl", getMemoryManager());

    XalanDOMString  theXMLSource(getMemoryManager());
    theXMLSource += args.base;
    theXMLSource += XalanDOMString("cconf.xml", getMemoryManager());

    // Check that we can find the stylesheet to analyze the results.
    //
    CharVectorType theBuffer(getMemoryManager());
    TranscodeToLocalCodePage(theStylesheet, theBuffer, true);
    FILE* fileHandle = fopen(c_str(theBuffer), "r");
    if (fileHandle == 0)
    {
        cout << "ANALYSIS ERROR: File Missing: " << c_str(theBuffer) << endl;
        return;
    }
    else
    {
        fclose(fileHandle);
    }

    // Create the InputSources and ResultTarget.
    const XSLTInputSource   xslInputSource(theStylesheet, getMemoryManager());
    const XSLTInputSource   xmlInputSource(theXMLSource, getMemoryManager());
    const XSLTResultTarget  resultFile(theHTMLFile, getMemoryManager());

    // Do the transform, display the output HTML, or report any failure.
    const int   result = xalan.transform(xmlInputSource, xslInputSource, resultFile);

    if (result == 0)
    {
#if defined(_MSC_VER)
        CharVectorType theBuffer(getMemoryManager());
        TranscodeToLocalCodePage(theHTMLFile, theBuffer, true);

        // system(c_str(theBuffer));
#else
        cout << "The HTML output: " << theHTMLFile << " was created" << endl;
#endif
    }
    else 
    {
        cout << "Analysis failed due to following error: "
             << xalan.getLastError()
             << endl;
    }   
}



const XalanDOMChar  XalanFileUtility::s_xmlSuffix[] =
{
    XalanUnicode::charFullStop,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_l,
    0
};

const XalanDOMChar  XalanFileUtility::s_pathSep[] =
{
#if defined(XALAN_WINDOWS)
    XalanUnicode::charReverseSolidus,
#else
    XalanUnicode::charSolidus,
#endif
    0
};



XALAN_CPP_NAMESPACE_END
