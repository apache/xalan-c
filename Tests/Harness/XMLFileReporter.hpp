/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 * XMLFileReporter.java
 */
#if !defined(HARNESS_HEADER_GUARD_1357924680)
#define HARNESS_HEADER_GUARD_1357924680


#include<string>
#include<stdio.h>
#include <time.h>



using namespace std;



/**
 * Reporter that saves output to a simple XML-format file.  
 * @author Shane_Curcuru@lotus.com
 * @version $Id$
 */

#if defined HARNESS_EXPORTS
#define HARNESS_API __declspec(dllexport)
#else
#define HARNESS_API __declspec(dllimport)
#endif

// This class is exported from the Harness.dll
class HARNESS_API XMLFileReporter 
{


public:

	/** Simple constructor, does not perform initialization.  */
	XMLFileReporter();   


	/** Construct and initialize this reporter with specified filename.  */
	XMLFileReporter(const string& fileName);

	/** Initialize this XMLFileReporter.  Must be called before attempting to log anything.  */
	bool initialize();

	/** Accessor for flushing; is set from properties.  */
	bool getFlushOnCaseClose();

	/** Accessor methods for our properties block.  */
	const string& getFileName() const;

	/** Accessor methods for our properties block.  */
	void setFileName(const string& fileName);

	/**
	* Is this Reporter still running OK?  
	* @returns status - true if an error has occoured, false if it's still working fine
	* <P>XMLFileReporter should just about never have errors.</P>
	*/
	bool checkError();


	/**
	* Is this Reporter ready to log results?  
	* @returns status - true if it's ready to report, false otherwise
	*/
	bool isReady();

	/** Flush this reporter - ensure our File is flushed.  */
	void flush();


	/** Close this reporter - ensure our File, etc. are closed.  */
	void close();

	//-----------------------------------------------------
	//-------- Testfile / Testcase start and stop routines --------
	//-----------------------------------------------------

	/**
	* Report that a testfile has started.  
	* @param msg message to log out
	*/
	void logTestFileInit(const string& msg);

	/**
	* Report that a testfile has finished, and report it's result.  
	* @param msg message to log out
	* @param result result of testfile
	*/
	void logTestFileClose(const string& msg, const string& result);

	void logTestCaseInit(const string& msg);

	/**
	* Report that a testcase has finished, and report it's result.  
	* @param msg message to log out
	* @param result result of testfile
	*/
	void logTestCaseClose(const string& msg, const string& result);

	//-----------------------------------------------------
	//-------- Test results reporting and logging routines --------
	//-----------------------------------------------------


	/**
	* Report a comment to result file with specified severity.  
	* <P>Record format: &lt;message level="##"&gt;msg&lt;/message&gt;</P>
	* @param level severity or class of message.
	* @param msg comment to log out.
	*/
	void logMessage(int level, const string& msg);

	/**
	* Logs out statistics to result file with specified severity.  
	* <P>Record format: &lt;statistic level="##" desc="msg"&gt;&lt;longval&gt;1234&lt;/longval&gt;&lt;doubleval&gt;1.234&lt;/doubleval&gt;&lt;/statistic&gt;</P>
	* @param level severity of message.
	* @param lVal statistic in long format.
	* @param dVal statistic in double format.
	* @param msg comment to log out.
	*/
	void logStatistic (int level, long lVal, double dVal, const string& msg);

	/**
	* Report an arbitrary string to result file with specified severity.  
	* <P>Appends and prepends \\n newline characters at the start and 
	* end of the message to separate it from the tags.</P>
	* <P>Record format: &lt;arbitrary level="##"&gt;<BR/>
	* msg<BR/>
	* &lt;/arbitrary&gt;
	* </P>
	* @param level severity or class of message.
	* @param msg arbitrary string to log out.
	*/
	void logArbitraryMessage (int level, const string& msg);

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
	void logHashtable (int level, Hashtable hash, const string& msg);
	*/

	/**
	* Writes out a Pass record with comment.  
	* <P>Record format: &lt;checkresult result="PASS" desc="comment"/&gt;</P>
	* @param comment comment to log with the pass record.
	*/
	void logCheckPass(const string& comment);

	/**
	* Writes out an ambiguous record with comment.  
	* <P>Record format: &lt;checkresult result="AMBG" desc="comment"/&gt;</P>
	* @param comment comment to log with the ambg record.
	*/
	void logCheckAmbiguous(const string& comment);

	/**
	* Writes out a Fail record with comment.  
	* <P>Record format: &lt;checkresult result="FAIL" desc="comment"/&gt;</P>
	* @param comment comment to log with the fail record.
	*/
	void logCheckFail(const string& comment);

	/**
	* Writes out a Error record with comment.  
	* <P>Record format: &lt;checkresult result="ERRR" desc="comment"/&gt;</P>
	* @param comment comment to log with the error record.
	*/
	void logCheckErr(const string& comment);

	/**
	* Escapes a string to remove < and > so it's valid XML.  
	* <P>Stolen mostly from Xalan applet sample.</P>
	* @param s string to escape.
	* @return string that has been escaped.
	*/
	string escapestring(const string& s);

private:

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
	bool printToFile(const string& output);

	/**
	* worker method to prints to the resultsfile.  
	*/
	string getDateTimeString();
	
	/** Key for Properties block that denotes our output filename.  */
	static const string OPT_FILENAME;

	/** XML tagnames for results file structure.  */
	static const string ELEM_RESULTSFILE;
	static const string ELEM_TESTFILE;
	static const string ELEM_FILERESULT;
	static const string ELEM_TESTCASE;
	static const string ELEM_CASERESULT;
	static const string ELEM_CHECKRESULT;
	static const string ELEM_STATISTIC;
	static const string ELEM_LONGVAL;
	static const string ELEM_DOUBLEVAL;
	static const string ELEM_MESSAGE;
	static const string ELEM_ARBITRARY;
	static const string ELEM_HASHTABLE;
	static const string ELEM_HASHITEM;

	/** XML attribute names for results file structure.  */
	static const string ATTR_LEVEL;
	static const string ATTR_DESC;
	static const string ATTR_TIME;
	static const string ATTR_RESULT;
	static const string ATTR_KEY;
	static const string ATTR_FILENAME;

	/** 
	* Optimization: for heavy use methods, form pre-defined constants to save on string concatenation.  
	* <P>Note the indent; must be updated if we ever switch to another indenting method.</P>
	*/
	static const string TESTCASEINIT_HDR;
	static const string TESTCASECLOSE_HDR;
	static const string MESSAGE_HDR;
	static const string STATISTIC_HDR;
	static const string ARBITRARY_HDR;
	static const string HASHTABLE_HDR;
	static const string HASHITEM_HDR;
	static const string CHECKPASS_HDR;
	static const string CHECKAMBG_HDR;
	static const string CHECKERRR_HDR;
	static const string CHECKFAIL_HDR;

	/** If we have output anything yet.  */
	bool m_anyOutput;

	/** Name of the file we're outputing to.  */
	string m_fileName;

	/** File reference and other internal convenience variables.  */
	FILE* m_fileHandle;

	/** If we're ready to start outputting yet.  */
	bool m_ready;

	/** If an error has occoured in this Reporter.  */
	bool m_error;

	/** If we should flush after every logTestCaseClose.  */
	bool m_flushOnCaseClose;

};        // end of class XMLFileReporter
#endif