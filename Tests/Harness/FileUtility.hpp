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
 * FileUtility.hpp
 */
#if !defined(FILEUTILITY_HEADER_GUARD_1357924680)
#define FILEUTILITY_HEADER_GUARD_1357924680

#include<string>
#include<stdio.h>
#include <time.h>

#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
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

#if defined HARNESS_EXPORTS
#define HARNESS_API __declspec(dllexport)
#else
#define HARNESS_API __declspec(dllimport)
#endif

// Misc typedefs and Global variables.
// These structures hold vectors of directory names and file names.
#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanDOMString>		FileNameVectorType;
#else
	typedef std::vector<XalanDOMString>	FileNameVectorType;
#endif

// Basic Global variables used by many tests.
const XalanDOMString	processorType(XALAN_STATIC_UCODE_STRING("XalanC"));
const XalanDOMString	XSLSuffix(XALAN_STATIC_UCODE_STRING(".xsl"));
const XalanDOMString	XMLSuffix(XALAN_STATIC_UCODE_STRING(".xml"));
const XalanDOMString	pathSep(XALAN_STATIC_UCODE_STRING("\\"));

// This class is exported from the Harness.dll
class HARNESS_API FileUtility 
{


public:

	struct reportStruct
	{
		XalanDOMString	testOrFile;
		char* msg;
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
	} data ;

	/** Simple constructor, does not perform initialization.  */
	FileUtility()
	{
		cout << endl << "Using Xerces Version " << gXercesFullVersionStr << endl;
	}
	
	/** 
	* Utility method used to get test files from a specific directory.
	* @returns a vector containing test files.
	*/
	FileNameVectorType FileUtility::getTestFileNames(XalanDOMString baseDir, XalanDOMString relDir, bool useDirPrefix);
	//FileNameVectorType getTestFileNames (char* theDirectory);

	/** 
	* Utility method used to get subdirectories from a specific directory.
	* @returns a vector containing directory files.
	*/	
	FileNameVectorType FileUtility::getDirectoryNames(XalanDOMString rootDirectory);

	/** 
	* Utility method used to create default directories when neccessary
	*/
    void FileUtility::checkAndCreateDir(XalanDOMString directory );

	/** 
	* Utility method determines if directory exists.
	*/
	bool FileUtility::checkDir(XalanDOMString directory );

	/** 
	* Utility method used to get XSL file based on XML file.
	* @returns a XalanDOMString.
	*/
	XalanDOMString FileUtility::GetXSLFileName(const XalanDOMString&	theXMLFileName);

	/** 
	* Utility method used to get OUT file based on XML file.
	* @returns a XalanDOMString.
	*/
	XalanDOMString FileUtility::GenerateFileName(const XalanDOMString&  theXMLFileName, char* suffix);

	/** 
	* Utility method used to generate UniqRunid.
	* @returns a XalanDOMString.
	*/
	XalanDOMString FileUtility::GenerateUniqRunid();

	/** 
	* Utility methods used to get Xerces Version number.
	* @returns a XalanDOMString.
	*/
	XalanDOMString FileUtility::getXercesVersion();


	void
	FileUtility::checkResults(const XalanDOMString& outputFile, 
							  const XalanDOMString& goldFile, 
							  XMLFileReporter& logfile);

	void
	FileUtility::checkAPIResults(const XalanDOMString& actual, 
								 const XalanDOMString& expected,
								 char* msg,
								 XMLFileReporter& logfile);

	void
	FileUtility::checkAPIErrorResults(const char* actual, 
									  const char* expected,
									  char* msg,
									  XMLFileReporter& logfile)
	{
		FileUtility::checkAPIResults(XalanDOMString(actual), 
									 XalanDOMString(expected),
									 msg,
									 logfile);
	}

	/**
	* Utility method used to compare the results. It inturn
	* call domCompare.  
	* @returns Void.
	*/
	bool
	FileUtility::compareDOMResults(const XalanDOMString& theOutputFile, 
								const XalanCompiledStylesheet* compiledSS, 
								XalanSourceTreeDocument* dom,
								const XSLTInputSource& goldInputSource);

	bool
	FileUtility::compareSerializedResults(const XalanDOMString& transformResult,
								const XalanDOMString& goldInputSource);
	/**
	* Utility method used to create a FormatterToXML FormatterListener.
	* This is required to DOM comparisions. 
	* @returns a pointer to a FormatterListener.
	*/
	FormatterListener* 
	FileUtility::getXMLFormatter(bool	shouldWriteXMLHeader,
				bool					stripCData,
				bool					escapeCData,
				PrintWriter&			resultWriter,
				int						indentAmount,
				const XalanDOMString&	mimeEncoding,
				const StylesheetRoot*	stylesheet);


	bool
	FileUtility::fileCompare(const char* goldFile,
						const char* outputFile);


	/** 
	* Utility methods used to perform a DOM Compare
	* @returns boolean
	*/
	bool 
	FileUtility::domCompare(const XalanNode& gold, const XalanNode& doc);

	/** 
	* Utility methods used to diff two Element nodes.
	* @returns boolean.
	*/
	bool 
	FileUtility::diffElement(const XalanNode& gold, const XalanNode& doc);

	/** 
	* Utility methods used to diff two attribute nodes.
	* @returns boolean.
	*/
	bool 
	FileUtility::diffAttr(const XalanNode* gAttr, const XalanNode* dAttr);

	/** 
	* Utility methods used to report Pass/Fail numbers.
	* @returns void.
	*/
	void
	FileUtility::reportPassFail(XMLFileReporter& logfile);

private:

	/** 
	* Utility methods used to collect information about compare failures.
	* @returns void.
	*/
	void 
	FileUtility::collectData(char* errmsg, 
						 XalanDOMString currentnode, 
						 XalanDOMString actdata, 
						 XalanDOMString expdata);

	/** 
	* Utility methods used to report DOM compare errors.
	* @returns void.
	*/
	void
	FileUtility::reportError();

};        // end of class FileUtility
#endif

