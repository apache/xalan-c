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
#if !defined(XALAN_FILEUTILITY_HEADER_GUARD_1357924680)
#define XALAN_FILEUTILITY_HEADER_GUARD_1357924680



#include "xalanc/Harness/XalanHarnessDefinitions.hpp"


#include "xalanc/Include/XalanVector.hpp"


#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <strstream.h>
#else
#include <strstream>
#endif



#include "xalanc/XalanDOM/XalanDOMString.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class FormatterListener;
class PrintWriter;
class StylesheetRoot;
class XalanCompiledStylesheet;
class XalanDocument;
class XalanNode;
class XalanSourceTreeDocument;
class XalanTransformer;
class XalanXMLFileReporter;
class XSLTInputSource;


XALAN_USING_XERCES(MemoryManager)



// This class is exported from the Harness.dll
class XALAN_HARNESS_EXPORT XalanFileUtility 
{
public:

// A vector to hold directory names and file names.

    typedef XalanVector<XalanDOMString>     FileNameVectorType;
    typedef XalanVector<char>               CharVectorType;

    struct XALAN_HARNESS_EXPORT reportStruct
    {
        XalanDOMString  theDrive;
        XalanDOMString  testOrFile;
        XalanDOMString  xmlFileURL;
        XalanDOMString  xslFileURL;
        XalanDOMString  xmlFormat;
        const char*     msg;
        XalanDOMString  currentNode;
        XalanDOMString  actual;
        XalanDOMString  expected;
        int             pass;
        int             fail;
        int             nogold;

        reportStruct(MemoryManager&     theManager);

        void
        reset();
    private:
        //Not implemented
        reportStruct();
        reportStruct(const reportStruct&);

    } data;

    struct XALAN_HARNESS_EXPORT cmdParams
    {
    private:

#if defined(XALAN_NO_STD_NAMESPACE)
        typedef ostrstream          StreamType;
#else
        typedef std::ostrstream     StreamType;
#endif

        StreamType  help;

    public:

        XalanDOMString  base;
        XalanDOMString  output;
        XalanDOMString  gold;
        XalanDOMString  sub;
        int             source;
        bool            skip;
        long            iters;


        cmdParams(MemoryManager& theManager);

        ~cmdParams()
        {
        }

        const char*
        getHelpMessage();

        StreamType&
        getHelpStream()
        {
            return help;
        }
    private:
        //Not implemented
        cmdParams();
        cmdParams(const cmdParams&);

    } args;

    /** Simple constructor, does not perform initialization.  */
    XalanFileUtility(MemoryManager& theManager);

    ~XalanFileUtility();

    bool
    getVerbose() const
    {
        return m_verbose;
    }

    void
    setVerbose(bool flag)
    {
        m_verbose = flag;
    }

    int
    getFailureCount() const
    {
        return data.fail;
    }

    /** 
    * Utility method used to get test files from a specific directory.
    * @returns a vector containing test files.
    */

    XalanDOMString&
    getDrive(XalanDOMString& theResult);

    bool
    getParams(
            int             argc,
            char*           argv[],
            const char*     outDir,
            bool            fsetGold = true);


    FileNameVectorType&
    getTestFileNames(
            const XalanDOMString&   baseDir,
            const XalanDOMString&   relDir,
            bool                    useDirPrefix,
            FileNameVectorType&     theFiles);

    /** 
    * Utility method used to get subdirectories from a specific directory.
    * @returns a vector containing directory files.
    */  
    FileNameVectorType&
    getDirectoryNames(const XalanDOMString&     rootDirectory,
                      FileNameVectorType&       theFiles);

    /** 
    * Utility method used to create default directories when neccessary
    */
    void
    checkAndCreateDir(const XalanDOMString&     directory);

    /** 
    * Utility method determines if directory exists.
    */
    bool
    checkDir(const XalanDOMString&  directory);

    /** 
    * Utility method used to get XSL file based on XML file.
    * @returns a XalanDOMString.
    */
    XalanDOMString&
    getXSLFileName(const XalanDOMString&    theXMLFileName,
                    XalanDOMString&         theResult);

    /** 
    * Utility method used to get OUT file based on XML file.
    * @returns a XalanDOMString.
    */
    XalanDOMString&
    generateFileName(
            const XalanDOMString&  theXMLFileName,
            const char*             suffix,
            XalanDOMString&         theResult,
            bool*                   status = 0);

    /** 
    * Utility method used to generate UniqRunid.
    * @returns a XalanDOMString.
    */
    XalanDOMString&
    generateUniqRunid(XalanDOMString& theResult);

    /** 
    * Utility methods used to get Xerces Version number.
    * @returns a XalanDOMString.
    */
    XalanDOMString&
    getXercesVersion(XalanDOMString& theResult);

    void
    checkResults(
            const XalanDOMString&   outputFile, 
            const XalanDOMString&   goldFile, 
            XalanXMLFileReporter&   logfile);

    void
    checkAPIResults(
            const XalanDOMString&   actual, 
            const XalanDOMString&   expected,
            const char*             msg,
            XalanXMLFileReporter&   logfile,
            const XalanDOMString&   outputFile,
            const XalanDOMString&   goldFile,
            bool                    containsOnly = false);

    // This API is deprecated.  Please use the following
    // one.
    void
    checkAPIResults(
            const char*             actual,
            const char*             expected,
            const char*             msg,
            XalanXMLFileReporter&   logfile,
            const XalanDOMString&   outputFile,
            const XalanDOMString&   goldFile,
            MemoryManager&          /* theManager */,
            bool                    containsOnly = false)
    {
        checkAPIResults(
            actual,
            expected,
            msg,
            logfile,
            outputFile,
            goldFile,
            containsOnly);
    }

    void
    checkAPIResults(
            const char*             actual,
            const char*             expected,
            const char*             msg,
            XalanXMLFileReporter&   logfile,
            const XalanDOMString&   outputFile,
            const XalanDOMString&   goldFile,
            bool                    containsOnly = false)
    {
        checkAPIResults(
            XalanDOMString(actual, getMemoryManager()), 
            XalanDOMString(expected, getMemoryManager()),
            msg,
            logfile,
            outputFile,
            goldFile,
            containsOnly);
    }

    /**
    * Utility method used to compare the results. It inturn
    * call domCompare.  
    * @returns Void.
    */
    void
    checkDOMResults(
            const XalanDOMString&           theOutputFile, 
            const XalanCompiledStylesheet*  compiledSS,
            const XalanSourceTreeDocument*  dom,
            const XSLTInputSource&          goldInputSource,
            XalanXMLFileReporter&           logfile);

    bool
    compareSerializedResults(
            const XalanDOMString&   transformResult,
            const XalanDOMString&   goldInputSource);

    /**
     * Utility method used to create a FormatterToXML FormatterListener.
     * This is required to DOM comparisions. 
     * @returns a pointer to a FormatterListener.
     */
    FormatterListener* 
    getXMLFormatter(
            PrintWriter&            resultWriter,
            int                     indentAmount,
            const XalanDOMString&   mimeEncoding,
            const StylesheetRoot*   stylesheet);


    bool
    fileCompare(
            const char*     goldFile,
            const char*     outputFile);

    /** 
    * Utility methods used to perform a DOM Compare
    * @returns boolean
    */
    bool 
    domCompare(const XalanNode& gold, const XalanNode& doc);

    /** 
    * Utility methods used to perform a DOM Compare
    * @returns boolean
    */
    bool 
    domCompare(
            const XalanDocument&    gold,
            const XalanDocument&    doc);

    /** 
    * Utility methods used to diff two Element nodes.
    * @returns boolean.
    */
    bool 
    diffElement(const XalanNode& gold, const XalanNode& doc);

    /** 
    * Utility methods used to diff two nodes.
    * @returns true if the nodes are equal, and false if not.
    */
    bool
    diffNode(
            const XalanNode&    gold,
            const XalanNode&    doc);

    /** 
    * Utility methods used to diff two nodes.
    * @returns true if the nodes are equal, and false if not.
    */
    bool
    diffNode(
            const XalanNode*    gold,
            const XalanNode*    doc);

    /** 
    * Utility methods used to diff two Element nodes.
    * @returns boolean.
    */
    bool 
    diffElement2(
            const XalanNode&    gold,
            const XalanNode&    doc);

    /** 
    * Utility methods used to diff two attribute nodes.
    * @returns boolean.
    */
    bool 
    diffAttr(const XalanNode* gAttr, const XalanNode* dAttr);

    /** 
    * Utility methods used to report Pass/Fail numbers.
    * @returns void.
    */
    void
    reportPassFail(XalanXMLFileReporter& logfile);

    void
    reportPassFail(XalanXMLFileReporter& logfile, const XalanDOMString& runid);

    void
    analyzeResults(XalanTransformer& xalan, const XalanDOMString& resultsFile);

    static const XalanDOMChar   s_xmlSuffix[];

    static const XalanDOMChar   s_pathSep[];

    const MemoryManager&
    getMemoryManager() const
    {
        return m_buffer.getMemoryManager();
    }

    MemoryManager&
    getMemoryManager()
    {
        return m_buffer.getMemoryManager();
    }

private:

    static const XalanDOMString     s_emptyString;

    XalanDOMString&
    getProgramName(const char* fullName, XalanDOMString& theBuffer);

    /** 
    * Utility methods used to collect information about compare failures.
    * @returns void.
    */
    void 
    collectData(
            const char*             errmsg,
            const XalanDOMString&   currentnode,
            const XalanDOMString&   actdata,
            const XalanDOMString&   expdata);

    /** 
    * Utility methods used to report DOM compare errors.
    * @returns void.
    */
    void
    reportError();

    void
    debugNodeData(const XalanDOMString&     /* value */);

    void
    debugNodeData(
            const XalanDOMString&   /* node */,
            const XalanDOMString&   /* value */);

    void
    debugAttributeData(const XalanDOMString&    /* value */);

private:

    CharVectorType  m_buffer;

    bool            m_verbose;

    //Not implemented
    XalanFileUtility();
    XalanFileUtility(const XalanFileUtility&);
};



XALAN_CPP_NAMESPACE_END



#endif  // XALAN_FILEUTILITY_HEADER_GUARD_1357924680
