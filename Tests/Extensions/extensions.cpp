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
 */


#include <iostream>
#include <strstream>
#include <stdio.h>
#include <direct.h>
#include <vector>

// This is here for memory leak testing. 
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

// XERCES HEADERS...
//	Are included by HarnessInit.hpp

// XALAN HEADERS...
//	Are included by FileUtility.hpp

// EXTENSION HEADERS...
#include <XalanExtensions/FunctionDifference.hpp>
#include <XalanExtensions/FunctionDistinct.hpp>
#include <XalanExtensions/FunctionEvaluate.hpp>
#include <XalanExtensions/FunctionHasSameNodes.hpp>
#include <XalanExtensions/FunctionIntersection.hpp>
#include <XalanExtensions/FunctionNodeSet.hpp>

// HARNESS HEADERS...
#include <XMLFileReporter.hpp>
#include <FileUtility.hpp>
#include <HarnessInit.hpp>


#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
#endif

// GLOBAL VARIABLES...
FileUtility				h;

const XalanDOMString	currentDir("library");
const XalanDOMString	theNamespace("http://xml.apache.org/xalan");
const char *resultString = "The specified function is not available: http://xml.apache.org/xalan:nodeset";


void
setHelp()
{
	h.args.help << endl
		 << "extensions dirname [-out]"
		 << endl
		 << endl
		 << "dirname		(base directory for xml-xalan\\test\\tests\\extensions)"
		 << endl
		 << "-out dirname	(base directory for output)"
		 << endl;
}

/*
bool
getParams(int argc, 
		  const char*	argv[],
		  XalanDOMString& baseDir,
		  XalanDOMString& outDir,
		  XalanDOMString& goldRoot)
{
	bool fSuccess = true;	// Used to continue argument loop
	bool fSetOut = true;	// Set default output directory, set to false if data is provided
	bool fsetGold = true;	// Set default gold directory, set to false if data is provided

	// Insure that required "-base" argument is there.
	if (argc == 1 || argv[1][0] == '-')
	{
		printArgOptions(); 
		return false;
	}
	else
	{
		if (h.checkDir(XalanDOMString(argv[1])))
		{
			assign(baseDir, XalanDOMString(argv[1]));
			if ( !endsWith(baseDir, XalanDOMString("extensions")) )
			{
				cout << endl << "Given base directory \"" << argv[1] << "\" not valid extensions directory" << endl;
				printArgOptions();
				return false;
			}
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
		if(!stricmp("-out", argv[i]))
		{
			++i;
			if(i < argc && argv[i][0] != '-')
			{
				assign(outDir, XalanDOMString(argv[i]));
//				insert(outDir, 0, XalanDOMString("\\"));
				append(outDir, XalanDOMString("\\"));
				h.checkAndCreateDir(outDir);
				fSetOut = false;
			}
			else
			{
				printArgOptions();
				fSuccess = false;
			}
		}
		else if(!stricmp("-gold", argv[i]))
		{
			++i;
			if(i < argc && argv[i][0] != '-')
			{
				assign(goldRoot, XalanDOMString(argv[i]));
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
		unsigned int ii = lastIndexOf(baseDir,charAt(pathSep,0));
		outDir = substring(baseDir, 0, ii+1);
		append(outDir,XalanDOMString("CEXTENSIONS-RESULTS\\"));
		h.checkAndCreateDir(outDir);
	}
	
	if (fsetGold)
	{
		goldRoot = baseDir;
		append(goldRoot,XalanDOMString("-gold\\"));
		h.checkAndCreateDir(goldRoot);
//		append(goldRoot,pathSep);
	}

	// Add the path seperator to the end of the base directory
	append(baseDir, pathSep);
	return fSuccess;
}
*/
// Generate the various filenames needed for testing.
void generateFiles(const XalanDOMString &fileName, 
				   XalanDOMString &xml, 
				   XalanDOMString &xsl,
				   XalanDOMString &out,
				   XalanDOMString &gold,
				   const char* test)
{
	// Set up the input/output files.

	const XalanDOMString testName(h.generateFileName(fileName,"out"));

	xsl = h.args.base + currentDir + pathSep + fileName;
	xml = h.generateFileName(xsl,"xml");
	h.data.xmlFileURL = xml;
	h.data.xslFileURL = xsl;

	out =  h.args.output + currentDir + pathSep + XalanDOMString(test) + testName; 

	gold = h.args.gold + currentDir + pathSep + testName;

}

//	TestCase1
//  API Call: 
//		None. Verfies default behavior of XalanTransformer.
//	Comments:
//		This testcase is called once for each function that XalanC implements. It verfies that the function 
//		was properly installed and working correctly within the transformer.  All functions are preinstalled
//		globally. Currently XalanC supports the following extensions; difference, distinct, evaluate, 
//		hasSameNodes, intersection and nodeset. 
//		 
void TestCase1(XalanTransformer &xalan, const XalanDOMString &fileName, XMLFileReporter &logFile)
{
	
	XalanDOMString	xml, xsl, theOutputFile, theGoldFile;
		
	generateFiles(fileName, xml, xsl, theOutputFile, theGoldFile, "tc1-");

	h.data.testOrFile = XalanDOMString("TestCase1: ") + fileName;
	h.data.xmlFileURL = xml;
	h.data.xslFileURL = xsl;

	// Create the InputSources and ResultTraget.
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl));
	const XSLTResultTarget	theResultTarget(theOutputFile);

	// Perform the transform and check the results.
	xalan.transform(xmlInputSource, xslInputSource, theResultTarget);	
	h.checkResults(theOutputFile, theGoldFile, logFile);

}

//	TestCase2
//  API Call:
//		XalanTransformer::uninstallExternalFunctionGlobal
//	Comments:
//		This test verifies that the 'nodeset' function is properly uninstalled via the api.
//		Output file should NOT contain data. 
//
void TestCase2(XalanTransformer &xalan, const XalanDOMString &fileName, XMLFileReporter &logFile)
{	

	XalanDOMString	xml, xsl, theOutputFile, theGoldFile;
	
	generateFiles(fileName, xml, xsl, theOutputFile, theGoldFile, "tc2-");
	h.data.testOrFile = XalanDOMString("TestCase2");
	h.data.xmlFileURL = xml;
	h.data.xslFileURL = xsl;

	// Create the InputSources and ResultTraget.
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl));
	const XSLTResultTarget	theResultTarget(theOutputFile);

	// UnInstall the external Global function "nodeset"
	xalan.uninstallExternalFunctionGlobal(
					theNamespace,
					XalanDOMString("nodeset"));

	//Perform the transform and check the results.
	xalan.transform(xmlInputSource, xslInputSource, theResultTarget);

	h.checkAPIResults(xalan.getLastError(), 
						  resultString,
						  "transformer.uninstallExternalFunctionGlobal()",
						  logFile,
						  theOutputFile,
						  theGoldFile);
}
//	TestCase3:
//  API Call:	
//		XalanTransformer::installExternalFunction
//		XalanTransformer::uninstallExternalFunction
//  Comments:
//		This tests the installExternalFunction method of XalanTransformer using the nodeset function.
//		Output file should NOT contain data.
//
void TestCase3(XalanTransformer &xalan, const XalanDOMString &fileName, XMLFileReporter &logFile)
{
	
	XalanDOMString	xml, xsl, theOutputFile, theGoldFile;
	
	generateFiles(fileName, xml, xsl, theOutputFile, theGoldFile, "tc3-");
	h.data.testOrFile = XalanDOMString("TestCase3a");
	h.data.xmlFileURL = xml;
	h.data.xslFileURL = xsl;

	// Create the InputSources and ResultTraget.
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl));
	const XSLTResultTarget	theResultTarget(theOutputFile);

	// Install the external function "nodeset"
	xalan.installExternalFunction(theNamespace,
								  XalanDOMString("nodeset"),
								  FunctionNodeSet());

	// Perform the transform and check the results.
	xalan.transform(xmlInputSource, xslInputSource, theResultTarget);

	h.checkResults(theOutputFile, theGoldFile, logFile);

	// Because we install the function locally, this second instance of the transformer 
	// should _NOT_ run the test successfully.
	XalanTransformer newEngine;
	h.data.testOrFile = XalanDOMString("TestCase3b");

	//Perform the transform and check the results.
	newEngine.transform(xmlInputSource, xslInputSource, theResultTarget);
	h.checkAPIResults(newEngine.getLastError(), 
							   resultString,
						       "transformer.installExternalFunction()",
						       logFile,
							   theOutputFile,
							   theGoldFile);

	// Now unInstall the external function "nodeset". Once again the transform should
	// _NOT_ run the test successfully
	h.data.testOrFile = XalanDOMString("TestCase3c");
	xalan.uninstallExternalFunction(theNamespace, XalanDOMString("nodeset"));

	// Perform the transform and check the results.
	xalan.transform(xmlInputSource, xslInputSource, theResultTarget);
	h.checkAPIResults(xalan.getLastError(), 
							   resultString,
						       "transformer.uninstallExternalFunction()",
						       logFile,
							   theOutputFile,
							   theGoldFile);

}

//	TestCase4:
//  API Call: 
//		XalanTransformer::installExternalFunctionGlobal
//	Comments:
//		This tests uses transformer method installExternalFunctionGlobal to add the nodeset function. 
//		The output file should contain data.
//
void TestCase4(XalanTransformer &xalan, const XalanDOMString &fileName, XMLFileReporter &logFile)
{
	
	XalanDOMString	xml, xsl, theOutputFile, theGoldFile;

	generateFiles(fileName, xml, xsl, theOutputFile, theGoldFile, "tc4-");
	h.data.testOrFile = XalanDOMString("TestCase4a");
	h.data.xmlFileURL = xml;
	h.data.xslFileURL = xsl;

	// Create the InputSources and ResultTraget.
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl));
	const XSLTResultTarget	theResultTarget(theOutputFile);

	// Install the external function "nodeset" Globally
	xalan.installExternalFunctionGlobal(theNamespace,
										XalanDOMString("nodeset"),
										FunctionNodeSet());

	// Perform the transform and check the results.
	xalan.transform(xmlInputSource, xslInputSource, theResultTarget);
	h.checkResults(theOutputFile, theGoldFile, logFile);

	// Create a second transformer and verify that it can 'see' the extension as well...
	XalanTransformer newEngine;
	h.data.testOrFile = XalanDOMString("TestCase4b");

	newEngine.transform(xmlInputSource, xslInputSource, theResultTarget);
	h.checkResults(theOutputFile, theGoldFile, logFile);

}

int
main(int			argc,
	 const char*	argv [])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	// Set the program help string,  then get the command line parameters.
	//
	setHelp();	
	if (h.getParams(argc, argv) == true)
	{
		// Generate Unique Run id. (Only used to name the result logfile.)
		const XalanDOMString  UniqRunid = h.generateUniqRunid();

		// Defined basic constants for file manipulation 
		const XalanDOMString drive(h.getDrive());
		const XalanDOMString  resultsFile(drive + h.args.output + currentDir + UniqRunid + XMLSuffix);
		
		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("C++ Extension Testing. ");
		logFile.logTestCaseInit(currentDir);

		cout << "Performing Extension testing ..." << endl;

		// Call the static initializers...
		HarnessInit xmlPlatformUtils;
		XalanTransformer::initialize();

		XalanTransformer xalan;				
				
		// Check that output directory is there.
		XalanDOMString		  fileName;
				
		const XalanDOMString  theOutputDir = h.args.output + currentDir;
		h.checkAndCreateDir(theOutputDir);

		// Get the files found in the "cextension" directory
		const FileNameVectorType	files = h.getTestFileNames(h.args.base, currentDir, false);

		// TestCase1 is used to verify correct functioning of the default extension functions
		TestCase1(xalan, files[0], logFile);	// Difference function
		TestCase1(xalan, files[1], logFile);	// Distinct 
		TestCase1(xalan, files[2], logFile);	// Evaluate 
		TestCase1(xalan, files[3], logFile);	// HasSameNodes 
		TestCase1(xalan, files[4], logFile);	// Intersection 
		TestCase1(xalan, files[5], logFile);	// NodeSet01 - basic testing
		TestCase1(xalan, files[6], logFile);	// NodeSet02 - extensive RTF testing. 

		// These testcases are used to excerise the Install/Uninstall Function API's of the transformer.
		TestCase2(xalan, files[5], logFile);
		TestCase3(xalan, files[5], logFile);
		TestCase4(xalan, files[5], logFile);

		logFile.logTestCaseClose("Done", "Pass");
		h.reportPassFail(logFile, UniqRunid);
			
		logFile.logTestFileClose("C++ Extension Testing: ", "Done");
		logFile.close();

		h.analyzeResults(xalan, resultsFile);
		XalanTransformer::terminate();

	}

	return 0;

}
