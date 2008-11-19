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
#if !defined(HARNESS_HEADER_GUARD_1357924680)
#define HARNESS_HEADER_GUARD_1357924680



#include "xalanc/Harness/XalanHarnessDefinitions.hpp"



#include <cstdio>



#include "xalanc/XalanDOM/XalanDOMString.hpp"



#include "xalanc/Include/XalanAutoPtr.hpp"
#include "xalanc/Include/XalanMap.hpp"
#include "xalanc/Include/XalanVector.hpp"



#include "xalanc/PlatformSupport/DOMStringHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class XalanOutputTranscoder;

class XALAN_HARNESS_EXPORT XalanXMLFileReporter
{
    
public:

    typedef XalanMap<XalanDOMString, XalanDOMString>  Hashtable;

#if defined(XALAN_STRICT_ANSI_HEADERS)
    typedef std::FILE   FileHandleType;
#else
    typedef FILE        FileHandleType;
#endif

public:


    // Construct and initialize this reporter with specified filename, if
    // the filename is not empty.
    XalanXMLFileReporter(MemoryManager& theManager, const XalanDOMString& fileName);

    ~XalanXMLFileReporter();

    // Initialize this XalanXMLFileReporter.  Must be called before attempting to log anything.
    bool initialize(MemoryManager& theManager);

    MemoryManager&
    getMemoryManager()
    {
        return m_fileName.getMemoryManager();
    }

    // Accessor for flushing; is set from properties.
    bool getFlushOnCaseClose();

    // Accessor methods for our properties block.
    const XalanDOMString& getFileName() const;

    // Accessor methods for our properties block.
    void setFileName(const XalanDOMString& fileName);

    // Accessor methods for our properties block.
    void setFileName(const char* fileName, MemoryManager& theManager)
    {
        setFileName(XalanDOMString(fileName, theManager));
    }

    //
    // Is this Reporter still running OK?  
    // @returns status - true if an error has occoured, false if it's still working fine
    bool checkError();

      
    //
    // Is this Reporter ready to log results?  
    // @returns status - true if it's ready to report, false otherwise
    bool isReady();

    // Flush this reporter - ensure our File is flushed.
    void flush();


    // Close this reporter - ensure our File, etc. are closed.
    void close();

    //-----------------------------------------------------
    //-------- Testfile / Testcase start and stop routines --------
    //-----------------------------------------------------

    /**
    * Report that a testfile has started.  
    * @param msg message to log out
    */
    void logTestFileInit(const XalanDOMString& msg);

    void logTestFileInit(const char*    msg)
    {
        logTestFileInit(XalanDOMString(msg,  getMemoryManager()));
    }

    /**
    * Report that a testfile has finished, and report it's result.  
    * @param msg message to log out
    * @param result result of testfile
    */
    void logTestFileClose(const XalanDOMString& msg, const XalanDOMString& result);

    void logTestFileClose(const char* msg, const char* result)
    {
        logTestFileClose(XalanDOMString(msg, getMemoryManager()), XalanDOMString(result, getMemoryManager()));  
    }

    void logTestCaseInit(const XalanDOMString& msg);

    void logTestCaseInit(const char*    msg)
    {
        logTestCaseInit(XalanDOMString(msg,  getMemoryManager()));
    }

    /**
    * Report that a testcase has finished, and report it's result.  
    * @param msg message to log out
    * @param result result of testfile
    */
    void logTestCaseClose(const XalanDOMString& msg, const XalanDOMString& result);

    void logTestCaseClose(const char* msg, const char* result)
    {
        logTestCaseClose(XalanDOMString(msg , getMemoryManager()), XalanDOMString(result , getMemoryManager()));
    }
    //-----------------------------------------------------
    //-------- Test results reporting and logging routines --------
    //-----------------------------------------------------


    /**
    * Report a comment to result file with specified severity.  
    * <P>Record format: &lt;message level="##"&gt;msg&lt;/message&gt;</P>
    * @param level severity or class of message.
    * @param msg comment to log out.
    */
    void logMessage(int level, const XalanDOMString& msg);

    /**
    * Logs out statistics to result file with specified severity.  
    * <P>Record format: &lt;statistic level="##" desc="msg"&gt;&lt;longval&gt;1234&lt;/longval&gt;&lt;doubleval&gt;1.234&lt;/doubleval&gt;&lt;/statistic&gt;</P>
    * @param level severity of message.
    * @param lVal statistic in long format.
    * @param dVal statistic in double format.
    * @param msg comment to log out.
    */
    void logStatistic (int level, long lVal, double dVal, const XalanDOMString& msg);

    void logStatistic (int level, long lVal, double dVal, const char*   msg)
    {
        logStatistic(level, lVal, dVal, XalanDOMString(msg, getMemoryManager()));
    }

    // This routine will add an attribute to the attribute list.
    void addMetricToAttrs(const char* desc, double theMetric, Hashtable& attrs);

    /**
    * Logs out a element to results with specified severity.
    * Uses user-supplied element name and attribute list.  Currently
    * attribute values and msg are forced .toString().  Also,
    * 'level' is forced to be the first attribute of the element.
    * @param level severity of message.
    * @param element name of enclosing element
    * @param attrs hash of name=value attributes; note that the
    * caller must ensure they're legal XML
    * @param msg comment to log out.
    */
    void logElementWAttrs(int level, const XalanDOMString& element, Hashtable& attrs, const XalanDOMString& msg);

    void logElementWAttrs(int level,  const char* element, Hashtable& attrs, const char* msg)
    {
        logElementWAttrs(level, XalanDOMString(element, getMemoryManager()), attrs, XalanDOMString(msg, getMemoryManager()));
    }

    void logElement(int level, const XalanDOMString& element, const XalanDOMString& msg);

    void logElement(const XalanDOMString& element, const XalanDOMString& msg);
    /**
    * Report an arbitrary XalanDOMString to result file with specified severity.  
    * <P>Appends and prepends \\n newline characters at the start and 
    * end of the message to separate it from the tags.</P>
    * <P>Record format: &lt;arbitrary level="##"&gt;<BR/>
    * msg<BR/>
    * &lt;/arbitrary&gt;
    * </P>
    * @param level severity or class of message.
    * @param msg arbitrary XalanDOMString to log out.
    */
    void logArbitraryMessage (int level, const XalanDOMString& msg);

    /**
    * Report a complete Hashtable to result file with specified severity.  
    * <P>Indents each hashitem within the table.</P>
    * <P>Record format: &lt;hashtable level="##" desc="msg"/&gt;<BR/>
    * &nbsp;&nbsp;&lt;hashitem key="key1"&gt;value1&lt;/hashitem&gt;<BR/>
    * &nbsp;&nbsp;&lt;hashitem key="key2"&gt;value2&lt;/hashitem&gt;<BR/>
    * &lt;/hashtable&gt;
    * </P>
    * @param level severity or class of message.
    * @param hash Hashtable to log the contents of. 
    * @param msg decription of the Hashtable.
    */
    /*
    void logHashtable (int level, Hashtable hash, const XalanDOMString& msg);
    */

    /**
    * Writes out a Pass record with comment.  
    * <P>Record format: &lt;checkresult result="PASS" desc="comment"/&gt;</P>
    * @param comment comment to log with the pass record.
    */
    void logCheckPass(const XalanDOMString& comment);

    /**
    * Writes out an ambiguous record with comment.  
    * <P>Record format: &lt;checkresult result="AMBG" desc="comment"/&gt;</P>
    * @param comment comment to log with the ambg record.
    */
    void logCheckAmbiguous(const XalanDOMString& comment);

    /**
    * Writes out a Fail record with comment.  
    * <P>Record format: &lt;checkresult result="FAIL" desc="comment"/&gt;</P>
    * @param comment comment to log with the fail record.
    */
    void logCheckFail(const XalanDOMString& comment);

    void logCheckFail(const XalanDOMString& test, const Hashtable& faildata, const Hashtable& actexp);

    void logCheckFail(const XalanDOMString& test, const Hashtable& actexp);

    void logErrorResult(const XalanDOMString& test, const XalanDOMString& reason);

    /**
    * Writes out a Error record with comment.  
    * <P>Record format: &lt;checkresult result="ERRR" desc="comment"/&gt;</P>
    * @param comment comment to log with the error record.
    */
    void logCheckErr(const XalanDOMString& comment);

    /**
    * Escapes a XalanDOMString to remove <, >, ', &, and " so it's valid XML.  
    * <P>Stolen mostly from Xalan applet sample.</P>
    * @param s XalanDOMString to escape.
    * @return XalanDOMString that has been escaped.
    */
    XalanDOMString& escapestring(const XalanDOMString& s, XalanDOMString&      buffer);


private:
    // not implemented
    XalanXMLFileReporter(const XalanXMLFileReporter&);
    /**
    * worker method to dump the xml header and open the resultsfile element.  
    */
    bool startResultsFile();

    /**
    * worker method to close the resultsfile element.  
    */
    bool closeResultsFile();

    /**
    * worker method to prints to the resultsfile.  
    */
    bool printToFile(const XalanDOMString& output);

    void
    initStrings();

    /**
    * worker method to prints to the resultsfile.  
    */
    XalanDOMString&
    getDateTimeString(XalanDOMString& result);
    
    /** Key for Properties block that denotes our output filename.  */
    XalanDOMString  OPT_FILENAME;

    /** XML tagnames for results file structure.  */
    XalanDOMString  ELEM_RESULTSFILE;
    XalanDOMString  ELEM_TESTFILE;
    XalanDOMString  ELEM_FILERESULT;
    XalanDOMString  ELEM_TESTCASE;
    XalanDOMString  ELEM_CASERESULT;
    XalanDOMString  ELEM_CHECKRESULT;
    XalanDOMString  ELEM_STATISTIC;
    XalanDOMString  ELEM_LONGVAL;
    XalanDOMString  ELEM_DOUBLEVAL;
    XalanDOMString  ELEM_MESSAGE;
    XalanDOMString  ELEM_ARBITRARY;
    XalanDOMString  ELEM_HASHTABLE;
    XalanDOMString  ELEM_HASHITEM;

    /** XML attribute names for results file structure.  */
    XalanDOMString  ATTR_LEVEL;
    XalanDOMString  ATTR_DESC;
    XalanDOMString  ATTR_TIME;
    XalanDOMString  ATTR_RESULT;
    XalanDOMString  ATTR_KEY;
    XalanDOMString  ATTR_FILENAME;
    XalanDOMString  LESS_THAN;
    XalanDOMString  GREATER_THAN;
    XalanDOMString  EQUALS_QUOTE;
    XalanDOMString  SPACE;
    XalanDOMString  QUOTE;
    XalanDOMString  QUOTE_SPACE;
    XalanDOMString  QUOTE_GREATER_THAN;
    XalanDOMString  QUOTE_SOLIDUS_GREATER_THAN;
    XalanDOMString  PASS;
    XalanDOMString  AMBG;
    XalanDOMString  ERRR;
    XalanDOMString  FAIL;
    XalanDOMString  LESS_THAN_SOLIDUS;
    XalanDOMString  XML_HEADER;
    XalanDOMString  REASON_EQUALS_QUOTE;

    /** 
    * Optimization: for heavy use methods, form pre-defined constants to save on XalanDOMString concatenation.  
    * <P>Note the indent; must be updated if we ever switch to another indenting method.</P>
    */
    XalanDOMString  TESTCASEINIT_HDR;
    XalanDOMString  TESTCASECLOSE_HDR;
    XalanDOMString  MESSAGE_HDR;
    XalanDOMString  STATISTIC_HDR;
    XalanDOMString  ARBITRARY_HDR;
    XalanDOMString  HASHTABLE_HDR;
    XalanDOMString  HASHITEM_HDR;
    XalanDOMString  CHECKPASS_HDR;
    XalanDOMString  CHECKAMBG_HDR;
    XalanDOMString  CHECKERRR_HDR;
    XalanDOMString  CHECKFAIL_HDR;
    XalanDOMString  CHECKFAIL_FTR;

    /** If we have output anything yet.  */
    bool m_anyOutput;

    /** Name of the file we're outputing to.  */
    XalanDOMString m_fileName;

    /** File reference and other internal convenience variables.  */
    FileHandleType* m_fileHandle;

    /** If we're ready to start outputting yet.  */
    bool m_ready;

    /** If an error has occoured in this Reporter.  */
    bool m_error;

    /** If we should flush after every logTestCaseClose.  */
    bool m_flushOnCaseClose;

    const XalanAutoPtr<XalanOutputTranscoder>   m_transcoder;

    CharVectorType                  m_buffer;
};        // end of class XalanXMLFileReporter



XALAN_CPP_NAMESPACE_END



#endif
