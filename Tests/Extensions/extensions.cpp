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
FileUtility				futil;
XalanDOMString			baseDir, outputRoot, goldRoot;  // These are set by the getParams routine.
const XalanDOMString	testDir("extensions");
const XalanDOMString	theNamespace("http://xml.apache.org/xalan");
const char *resultString = "The specified function is not available: http://xml.apache.org/xalan:nodeset";


void
printArgOptions()
{
	cerr << endl
		 << "extensions dirname [-out]"
		 << endl
		 << endl
		 << "dirname		(base directory for testcases)"
		 << endl
		 << "-out dirname	(base directory for output)"
		 << endl;
}

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
		if (futil.checkDir(pathSep + XalanDOMString(argv[1])))
		{
			assign(baseDir, XalanDOMString(argv[1]));
			insert(baseDir, 0, pathSep);
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
				insert(outDir, 0, XalanDOMString("\\"));
				append(outDir, XalanDOMString("\\"));
				futil.checkAndCreateDir(outDir);
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
		append(outDir,XalanDOMString("CEXTENSIONS-results\\"));
		futil.checkAndCreateDir(outDir);
	}
	
	if (fsetGold)
	{
		goldRoot = baseDir;
		append(goldRoot,XalanDOMString("-gold"));
		futil.checkAndCreateDir(goldRoot);
		append(goldRoot,pathSep);
	}

	// Add the path seperator to the end of the base directory
	append(baseDir, pathSep);
	return fSuccess;
}

// Generate the various filenames needed for testing.
void generateFiles(const XalanDOMString &fileName, 
				   XalanDOMString &xml, 
				   XalanDOMString &xsl,
				   XalanDOMString &out,
				   XalanDOMString &gold,
				   const char* test)
{
	// Set up the input/output files.
	const XalanDOMString testName(test + XalanDOMString(".out"));

	xsl = baseDir + testDir + pathSep + fileName;
	
	xml = futil.GenerateFileName(xsl,"xml");

	out =  outputRoot + testDir + pathSep + testName; 

	gold = goldRoot +testDir + pathSep + fileName;
	gold = futil.GenerateFileName(gold, "out");

}

//	TestCase1
//  API Call: 
//		None. Verfies default behavior of XalanTransformer.
//	Comments:
//		This test verifies that the Nodeset function was properly installed with the transformer.  All 
//		functions are preinstalled globally.  Output file should contain data. 
//		 
void TestCase1(XalanTransformer &transformEngine, const XalanDOMString &fileName, XMLFileReporter &logFile)
{
	
	XalanDOMString	xml, xsl, theOutputFile, theGoldFile;
		
	generateFiles(fileName, xml, xsl, theOutputFile, theGoldFile, "TestCase1");
	futil.data.testOrFile = XalanDOMString("TestCase1");

	// Create the InputSources and ResultTraget.
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl));
	const XSLTResultTarget	theResultTarget(theOutputFile);

	// Perform the transform and check the results.
	int	theResult = transformEngine.transform(xmlInputSource, xslInputSource, theResultTarget);	
	
	futil.checkResults(theOutputFile, theGoldFile, logFile);

}

//	TestCase2
//  API Call: 
//		None. Verfies default behavior of XalanTransformer.
//	Comments:
//		This test verifies that the following functions: 
//			difference, distinct, 
//			evaluate, hasSameNodes, 
//		and intersection are defined and working.  Output file should contain data.
//
void TestCase2(XalanTransformer &transformEngine, const XalanDOMString &fileName, XMLFileReporter &logFile)
{
	
	XalanDOMString	xml, xsl, theOutputFile, theGoldFile;

	generateFiles(fileName, xml, xsl, theOutputFile, theGoldFile, "TestCase2");
	futil.data.testOrFile = XalanDOMString("TestCase2");

	// Create the InputSources and ResultTraget.
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl));
	const XSLTResultTarget	theResultTarget(theOutputFile);


	// Perform the transform and check the results.
	transformEngine.transform(xmlInputSource, xslInputSource, theResultTarget);

	futil.checkResults(theOutputFile, theGoldFile, logFile);

}

//	TestCase3
//  API Call:
//		XalanTransformer::uninstallExternalFunctionGlobal
//	Comments:
//		This test verifies that the 'nodeset' function is properly uninstalled via the api.
//		Output file should NOT contain data. 
//
void TestCase3(XalanTransformer &transformEngine, const XalanDOMString &fileName, XMLFileReporter &logFile)
{	

	XalanDOMString	xml, xsl, theOutputFile, theGoldFile;
	
	generateFiles(fileName, xml, xsl, theOutputFile, theGoldFile, "TestCase3");
	futil.data.testOrFile = XalanDOMString("TestCase3");

	// Create the InputSources and ResultTraget.
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl));
	const XSLTResultTarget	theResultTarget(theOutputFile);

	// UnInstall the external Global function "nodeset"
	transformEngine.uninstallExternalFunctionGlobal(
					theNamespace,
					XalanDOMString("nodeset"));

	int theResult = transformEngine.transform(xmlInputSource, xslInputSource, theResultTarget);
	if (theResult == -1)
	{
		futil.checkAPIErrorResults(transformEngine.getLastError(), 
								   resultString,
							       "transformer.uninstallExternalFunctionGlobal()",
							       logFile);
	}
	else
	{
		cout << endl << "Failedxxx: TestCase3" ;
	}
	
}


//	TestCase4:
//  API Call:	
//		XalanTransformer::installExternalFunction
//		XalanTransformer::uninstallExternalFunction
//  Comments:
//		This tests the installExternalFunction method of XalanTransformer using the nodeset function.
//		Output file should NOT contain data.
//
void TestCase4(XalanTransformer &transformEngine, const XalanDOMString &fileName, XMLFileReporter &logFile)
{
	
	XalanDOMString	xml, xsl, theOutputFile, theGoldFile;
	
	generateFiles(fileName, xml, xsl, theOutputFile, theGoldFile, "TestCase4");
	futil.data.testOrFile = XalanDOMString("TestCase4a");

	// Create the InputSources and ResultTraget.
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl));
	const XSLTResultTarget	theResultTarget(theOutputFile);

	// Install the external function "nodeset"
	transformEngine.installExternalFunction(
		theNamespace,
		XalanDOMString("nodeset"),
		FunctionNodeSet());

	// Perform the transform and check the results.
	transformEngine.transform(xmlInputSource, xslInputSource, theResultTarget);

	futil.checkResults(theOutputFile, theGoldFile, logFile);

	// Because we install the function locally, this second instance of the transformer should not run the
	// test successfully.
	XalanTransformer newEngine;
	futil.data.testOrFile = XalanDOMString("TestCase4b");

	int theResult = newEngine.transform(xmlInputSource, xslInputSource, theResultTarget);
	if (theResult == -1)
	{
		futil.checkAPIErrorResults(newEngine.getLastError(), 
								   resultString,
							       "transformer.installExternalFunction()",
							       logFile);
	}
	else
	{
		cout << endl << "Failed: TestCase4b" ;
	}

	// Now unInstall the external function "nodeset"
	transformEngine.uninstallExternalFunction(
		theNamespace,
		XalanDOMString("nodeset"));

	// Perform the transform and check the results.
	futil.data.testOrFile = XalanDOMString("TestCase4c");
	theResult = transformEngine.transform(xmlInputSource, xslInputSource, theResultTarget);
	if (theResult == -1)
	{
		futil.checkAPIErrorResults(transformEngine.getLastError(), 
								   resultString,
							       "transformer.uninstallExternalFunction()",
							       logFile);
	}
	else
	{
		cout << endl << "Failed: TestCase4c" ;
	}

}

//	TestCase5:
//  API Call: 
//		XalanTransformer::installExternalFunctionGlobal
//	Comments:
//		This tests uses transformer method installExternalFunctionGlobal to add the nodeset function. 
//		The output file should contain data.
//
void TestCase5(XalanTransformer &transformEngine, const XalanDOMString &fileName, XMLFileReporter &logFile)
{
	
	XalanDOMString	xml, xsl, theOutputFile, theGoldFile;

	generateFiles(fileName, xml, xsl, theOutputFile, theGoldFile, "TestCase5");
	futil.data.testOrFile = XalanDOMString("TestCase5a");

	// Create the InputSources and ResultTraget.
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl));
	const XSLTResultTarget	theResultTarget(theOutputFile);

	// Install the external function "nodeset" Globally
	transformEngine.installExternalFunctionGlobal(
		theNamespace,
		XalanDOMString("nodeset"),
		FunctionNodeSet());

	// Perform the transform and check the results.
	transformEngine.transform(xmlInputSource, xslInputSource, theResultTarget);
	futil.checkResults(theOutputFile, theGoldFile, logFile);

	// Create a second transformer and verify that it can 'see' the extension as well...
	XalanTransformer newEngine;
	futil.data.testOrFile = XalanDOMString("TestCase5b");

	newEngine.transform(xmlInputSource, xslInputSource, theResultTarget);
	futil.checkResults(theOutputFile, theGoldFile, logFile);

}

int
main(
		  int			argc,
		  const char*	argv [])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif


	if (getParams(argc, argv, baseDir, outputRoot, goldRoot) == true)
	{
		// Generate Unique Run id. (Only used to name the result logfile.)
		const XalanDOMString UniqRunid = futil.GenerateUniqRunid();

		// Defined basic constants for file manipulation 
		const XalanDOMString  resultsFile(outputRoot + testDir + UniqRunid + XMLSuffix);
		
		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("C++ Extension Testing. ");
		cout << "Performing Extension testing ..." << endl;

		try
		{
			// Call the static initializers...
			HarnessInit xmlPlatformUtils;
			XalanTransformer::initialize();

			XalanTransformer transformEngine;				
				
			// Check that output directory is there.
			XalanDOMString		  fileName;
				
			const XalanDOMString  theOutputDir = outputRoot + testDir;
			futil.checkAndCreateDir(theOutputDir);

			// Get the files found in the "cextension" directory
			const FileNameVectorType	files = futil.getTestFileNames(baseDir, testDir, true);

			TestCase1(transformEngine, files[0], logFile);  // Nodeset function
			TestCase2(transformEngine, files[1], logFile);	// All others
			TestCase3(transformEngine, files[0], logFile);
			TestCase4(transformEngine, files[0], logFile);
			TestCase5(transformEngine, files[0], logFile);
				
			cout << endl;

			XalanTransformer::terminate();
			
			futil.reportPassFail(logFile);
			logFile.logTestFileClose("C++ Extension Testing: ", "Done");
			logFile.close();

		}
		catch(...)
		{
			cerr << "Exception caught!!!" << endl << endl;
		}
	}

	return 0;

}
