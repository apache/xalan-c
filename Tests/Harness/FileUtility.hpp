/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights 
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
 * FileUtility.hpp
 */
#if !defined(FILEUTILITY_HEADER_GUARD_1357924680)
#define FILEUTILITY_HEADER_GUARD_1357924680

#include<string>
#include<stdio.h>
#include <time.h>

#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#include <sstream.h>
#else
#include <iostream>
#include <sstream>
#endif

// XERCES HEADERS ... 
//	Are included in HarnessInit.hpp

// XALAN HEADERS...
#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <PlatformSupport/XalanFileOutputStream.hpp>
#include <PlatformSupport/DirectoryEnumerator.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>

#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>

#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>

#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>
#include <XalanSourceTree/XalanSourceTreeDocument.hpp>

#include <XalanTransformer/XalanCompiledStylesheetDefault.hpp>
#include <XalanTransformer/XalanTransformer.hpp>

#include "XMLFileReporter.hpp"

using namespace std;


/**
 * Utility call that extracts test file names from testsuite.  
 * @author Paul Dick@lotus.com
 * @version $Id$
 */

#if !defined(WIN32)
#define HARNESS_API
#else
#if defined(HARNESS_EXPORTS)
#define HARNESS_API __declspec(dllexport)
#else
#define HARNESS_API __declspec(dllimport)
#endif
#endif


// Misc typedefs and Global variables.
// These structures hold vectors of directory names and file names.
#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanDOMString>		FileNameVectorType;
#else
	typedef std::vector<XalanDOMString>	FileNameVectorType;
	using std::ostringstream;
#endif


// Basic Global variables used by many tests.
//const XalanDOMString	XSLSuffix(XALAN_STATIC_UCODE_STRING(".xsl"));
const XalanDOMString	XMLSuffix(XALAN_STATIC_UCODE_STRING(".xml"));

#if defined(WIN32)
const XalanDOMString	pathSep(XALAN_STATIC_UCODE_STRING("\\"));
#else
const XalanDOMString	pathSep(XALAN_STATIC_UCODE_STRING("/"));
#endif

// This class is exported from the Harness.dll
class HARNESS_API FileUtility 
{
public:

	struct reportStruct
	{
		XalanDOMString  theDrive;
		XalanDOMString	testOrFile;
		XalanDOMString  xmlFileURL;
		XalanDOMString  xslFileURL;
		XalanDOMString  xmlFormat;
		const char*		msg;
		XalanDOMString	currentNode;
		XalanDOMString	actual;
		XalanDOMString	expected;
		int				pass;
		int				fail;
		int				nogold;

		void reset()
		{
			clear(testOrFile);
			msg = "";
			clear(currentNode);
			clear(actual);
			clear(expected);
		}
	} data;

	struct cmdParams
	{
		ostringstream   help;
		XalanDOMString  base;
		XalanDOMString	output;
		XalanDOMString  gold;
		XalanDOMString  sub;
		int				source;
		bool			skip;
		long			iters;

	} args;

	/** Simple constructor, does not perform initialization.  */
	FileUtility()
	{
		cout << endl << "Using Xerces Version " << gXercesFullVersionStr << endl;
	}
	
	/** 
	* Utility method used to get test files from a specific directory.
	* @returns a vector containing test files.
	*/

	XalanDOMString
	getDrive();

	bool
	getParams(int argc, const char*	argv[]);


	FileNameVectorType
	getTestFileNames(
			const XalanDOMString&	baseDir,
			const XalanDOMString&	relDir,
			bool					useDirPrefix);

	/** 
	* Utility method used to get subdirectories from a specific directory.
	* @returns a vector containing directory files.
	*/	
	FileNameVectorType
	getDirectoryNames(const XalanDOMString&		rootDirectory);

	/** 
	* Utility method used to create default directories when neccessary
	*/
    void
	checkAndCreateDir(const XalanDOMString&		directory);

	/** 
	* Utility method determines if directory exists.
	*/
	bool
	checkDir(const XalanDOMString&	directory);

	/** 
	* Utility method used to get XSL file based on XML file.
	* @returns a XalanDOMString.
	*/
	XalanDOMString
	getXSLFileName(const XalanDOMString&	theXMLFileName);

	/** 
	* Utility method used to get OUT file based on XML file.
	* @returns a XalanDOMString.
	*/
	XalanDOMString
	generateFileName(
			const XalanDOMString&  theXMLFileName,
			const char*				suffix);

	/** 
	* Utility method used to generate UniqRunid.
	* @returns a XalanDOMString.
	*/
	XalanDOMString
	generateUniqRunid();

	/** 
	* Utility methods used to get Xerces Version number.
	* @returns a XalanDOMString.
	*/
	XalanDOMString
	getXercesVersion();


	void
	checkResults(
			const XalanDOMString&	outputFile, 
			const XalanDOMString&	goldFile, 
			XMLFileReporter&		logfile);

	void
	checkAPIResults(
			const XalanDOMString&	actual, 
			const XalanDOMString&	expected,
			const char*				msg,
			XMLFileReporter&		logfile,
			const XalanDOMString&	outputFile,
			const XalanDOMString&	goldFile);

	void
	checkAPIResults(
			const char*				actual,
			const char*				expected,
			const char*				msg,
			XMLFileReporter&		logfile,
			const XalanDOMString&	outputFile,
			const XalanDOMString&	goldFile)
	{
		checkAPIResults(
			XalanDOMString(actual), 
			XalanDOMString(expected),
			msg,
			logfile,
			outputFile,
			goldFile);
	}

	/**
	* Utility method used to compare the results. It inturn
	* call domCompare.  
	* @returns Void.
	*/
	void
	checkDOMResults(
			const XalanDOMString&			theOutputFile, 
			const XalanCompiledStylesheet*	compiledSS,
			const XalanSourceTreeDocument*	dom,
			const XSLTInputSource&			goldInputSource,
			XMLFileReporter&				logfile);

	bool
	compareSerializedResults(
			const XalanDOMString&	transformResult,
			const XalanDOMString&	goldInputSource);
	/**
	* Utility method used to create a FormatterToXML FormatterListener.
	* This is required to DOM comparisions. 
	* @returns a pointer to a FormatterListener.
	*/
	FormatterListener* 
	getXMLFormatter(
			bool					shouldWriteXMLHeader,
			bool					stripCData,
			bool					escapeCData,
			PrintWriter&			resultWriter,
			int						indentAmount,
			const XalanDOMString&	mimeEncoding,
			const StylesheetRoot*	stylesheet);


	bool
	fileCompare(
			const char*		goldFile,
			const char*		outputFile);

	/** 
	* Utility methods used to perform a DOM Compare
	* @returns boolean
	*/
	bool 
	domCompare(const XalanNode& gold, const XalanNode& doc);

	/** 
	* Utility methods used to diff two Element nodes.
	* @returns boolean.
	*/
	bool 
	diffElement(const XalanNode& gold, const XalanNode& doc);

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
	reportPassFail(XMLFileReporter& logfile);

	void
	reportPassFail(XMLFileReporter& logfile, const XalanDOMString& runid);

	void
	analyzeResults(XalanTransformer& xalan, const XalanDOMString& resultsFile);

private:

	XalanDOMString
	getProgramName(const char* fullName);

	/** 
	* Utility methods used to collect information about compare failures.
	* @returns void.
	*/
	void 
	collectData(
			const char*				errmsg,
			const XalanDOMString&	currentnode,
			const XalanDOMString&	actdata,
			const XalanDOMString&	expdata);

	/** 
	* Utility methods used to report DOM compare errors.
	* @returns void.
	*/
	void
	reportError();

	void
	debugNodeData(const XalanDOMString& value)
	{
#if !defined(NDEBUG) && defined(_MSC_VER)
		cout << "Node is: " << c_str(TranscodeToLocalCodePage(value)) << endl;
#endif
	}

	void
	debugNodeData(
			const XalanDOMString&	node,
			const XalanDOMString&	value)
	{
#if !defined(NDEBUG) && defined(_MSC_VER)
		cout << "Node is: " << c_str(TranscodeToLocalCodePage(node)) << "	"
			 << "Value is: \"" << c_str(TranscodeToLocalCodePage(value)) << "\"\n";
#endif
	}

	void
	debugAttributeData(const XalanDOMString&	value)
	{
#if !defined(NDEBUG) && defined(_MSC_VER)
		cout << "Attribute is: " << c_str(TranscodeToLocalCodePage(value)) << endl;
#endif
	}
};        // end of class FileUtility



#endif

