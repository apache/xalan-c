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

FileUtility		futil;

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
		append(goldRoot,XalanDOMString("\\xout"));
		futil.checkAndCreateDir(goldRoot);
		append(goldRoot,pathSep);
	}

	// Add the path seperator to the end of the base directory
	append(baseDir, pathSep);
	return fSuccess;
}


// TestCase1 will use the following method of XSLTInputSource
//		- XSLTextensions(const XMLCh* systemId)
//		- XSLTextensions(const XMLCh* systemId,
//						 const XMLCh* publicId)

// TestCase2 will use the following methods of XSLTInputSource
//		- XSLTInputSource(const char* systemId)
//		- XSLTInputSource(const char* systemId,
//						  const char* publicId)


// TestCase3 will use the following methods of XSLTInputSource
//		- XSLTInputSource()
//		- XSLTInputSource(XalanNode* node)
//		- XSLTInputSource::setNode(XalanNode* node)
//		- XSLTInputSource::getNode()
//		- NOTE:  We can't use the XalanTransformer Class with this test. So we create the processor directly.


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

//	FileUtility		futil;

	XalanDOMString  category;	// Test all of base dir by default
	XalanDOMString  baseDir, outputRoot, goldRoot;	 	


	if (getParams(argc, argv, baseDir, outputRoot, goldRoot) == true)
	{

		// Generate Unique Run id. (Only used to name the result logfile.)
		const XalanDOMString UniqRunid = futil.GenerateUniqRunid();

		// Defined basic constants for file manipulation 

		const XalanDOMString  resultFilePrefix("cextensions");
		const XalanDOMString  resultsFile(outputRoot + resultFilePrefix + UniqRunid + XMLSuffix);
		
		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("C++ Extension Testing. ");

		try
		{
			// Call the static initializers...
			HarnessInit xmlPlatformUtils;
			XalanTransformer::initialize();


			{
				XalanTransformer		transformEngine;
				
				// The namespace for our functions...
				const XalanDOMString	theNamespace("http://ExternalFunction.xalan-c.xml.apache.org");

				// Install the functions in the local space.  They will only
				// be installed in this instance, so no other instances
				// will know about them...
				transformEngine.installExternalFunction(
					theNamespace,
					XalanDOMString("nodeset"),
					FunctionNodeSet());
				

				// Check that output directory is there.
				XalanDOMString		  fileName;
				const XalanDOMString  xDir("cextension");
				const XalanDOMString  theOutputDir = outputRoot + xDir;
				futil.checkAndCreateDir(theOutputDir);

				// Get the files found in the "simpletests" directory
				const FileNameVectorType	files = futil.getTestFileNames(baseDir, xDir, false);

				for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
				{

					// Output file name to result log and console.
					//logFile.logTestCaseInit(files[i]);
					fileName = files[i];
					cout << fileName << endl;

					// Set up the input/output files.
					const XalanDOMString  theXSLFile= baseDir + xDir + pathSep + fileName;
					const XalanDOMString  theXMLFile = futil.GenerateFileName(theXSLFile,"xml");
					XalanDOMString  theOutput =  outputRoot + xDir + pathSep + fileName; 
					theOutput = futil.GenerateFileName(theOutput, "out");
					XalanDOMString  theGoldFile = goldRoot +xDir + pathSep + fileName;
					theGoldFile = futil.GenerateFileName(theGoldFile, "out");


					const XSLTResultTarget	theResultTarget(theOutput);

					// This code excersized the stated methods of XSLTInputSource
					const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));
					const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));

					int	theResult = transformEngine.transform(xmlInputSource, xslInputSource, theResultTarget);

//					testCase1(transformEngine, theXMLFile, theXSLFile, theOutput, theGoldFile, fileName);

//					testCase2(transformEngine, theOutput, theGoldFile, fileName);

//					testCase3(transformEngine, logFile, theXMLFile, theXSLFile, theOutput);
				}
				
			}

			XalanTransformer::terminate();

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
