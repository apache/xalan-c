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
// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>

#include <iostream>
#include <strstream>
#include <stdio.h>
#include <direct.h>
#include <vector>

// This is here for memory leak testing. 
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

#include <util/PlatformUtils.hpp>

#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTResultTarget.hpp>

#include <XalanTransformer/XalanTransformer.hpp>

#include <XMLFileReporter.hpp>
#include <FileUtility.hpp>
#include <HarnessInit.hpp>


#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
#endif


void
printArgOptions()
{
	cerr << endl
		 << "params dirname [-out]"
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
		  FileUtility& f,
		  XalanDOMString& basedir,
		  XalanDOMString& outdir)
{
	bool fSuccess = true;	// Used to continue argument loop
	bool fSetOut = true;	// Set default output directory

	// Insure that required "-base" argument is there.
	if (argc == 1 || argv[1][0] == '-')
	{
		printArgOptions(); 
		return false;
	}
	else
	{
		if (f.checkDir(pathSep + XalanDOMString(argv[1])))
		{
			assign(basedir, XalanDOMString(argv[1]));
			insert(basedir, 0, pathSep);
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
				assign(outdir, XalanDOMString(argv[i]));
				insert(outdir, 0, XalanDOMString("\\"));
				append(outdir, XalanDOMString("\\"));
				f.checkAndCreateDir(outdir);
				fSetOut = false;
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
		unsigned int ii = lastIndexOf(basedir,charAt(pathSep,0));
		outdir = substring(basedir, 0, ii+1);
		append(outdir,XalanDOMString("PARAM-RESULTS\\"));
		f.checkAndCreateDir(outdir);
	}
	
	// Add the path seperator to the end of the base directory
	append(basedir, pathSep);
	return fSuccess;
}

//	This function returns the testcase number.  All of these tests are called
//	variablemanXX, and there are only 6 of them,  so we can pick off the
//	second X to determine what test number we're dealing with.  We need to
//	know which test because each test gets different parameters. This code will
//  need modification if the number of tests changes.
unsigned short
getTestNumber(const XalanDOMString& theFile)
{
	assert(12 < length(theFile));

	return charAt(theFile, 12) - XalanUnicode::charDigit_0;
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

	FileUtility		f;

	XalanDOMString  category;	// Test all of base dir by default
	XalanDOMString  baseDir;	
	XalanDOMString  outputRoot;	


	if (getParams(argc, argv, f, baseDir, outputRoot) == true)
	{

		// Generate Unique Run id. (Only used to name the result logfile.)
		const XalanDOMString UniqRunid = f.GenerateUniqRunid();

		// Defined basic constants for file manipulation 

		const XalanDOMString  resultFilePrefix("params");
		const XalanDOMString  resultsFile(outputRoot + resultFilePrefix + UniqRunid + XMLSuffix);
		
		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("Param Testing: Transforms using variablexx in xmanual directory. ");

		try
		{
			// Call the static initializers...
			HarnessInit xmlPlatformUtils;
			XalanTransformer::initialize();

			{
				XalanTransformer		transformEngine;
					
				// Check that output directory is there.
				const XalanDOMString  xMan("xmanual");
				const XalanDOMString  theOutputDir = outputRoot + xMan;
				f.checkAndCreateDir(theOutputDir);

				// Get the files found in the "xmanual" directory
				const FileNameVectorType	files = f.getTestFileNames(baseDir, xMan,false);

				for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
				{

					// If the file starts with "variable" process it.
					if (startsWith(files[i],"variable"))
					{
						// Output file name to result log and console.
						logFile.logTestCaseInit(files[i]);
						cout << files[i] << endl;

						// Set up the input/output files.
						const XalanDOMString  theXSLFile= baseDir + xMan + pathSep + files[i];
						const XalanDOMString  theXMLFile = f.GenerateFileName(theXSLFile,"xml");
						const XalanDOMString  theOutput =  outputRoot + xMan + pathSep + files[i]; 
						const XalanDOMString  theOutputFile = f.GenerateFileName(theOutput, "out");

						XSLTResultTarget		theResultTarget(theOutputFile);
						const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
						const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));
						
						// Set the desired parameters
						switch (getTestNumber(files[i]))
						{	case 1:
								transformEngine.setStylesheetParam(XalanDOMString("input"), 
																   XalanDOMString("'testing 1 2 3'"));
								break;
							case 2:
								transformEngine.setStylesheetParam(XalanDOMString("in1"), 
																   XalanDOMString("'A '"));
								transformEngine.setStylesheetParam(XalanDOMString("in2"), 
																   XalanDOMString("'B '"));
								transformEngine.setStylesheetParam(XalanDOMString("in3"),
																   XalanDOMString("'C '"));
								transformEngine.setStylesheetParam(XalanDOMString("in4"),
																   XalanDOMString("'D '"));
								transformEngine.setStylesheetParam(XalanDOMString("in5"),
																   XalanDOMString("'E '"));
								break;
							case 3:
								transformEngine.setStylesheetParam(XalanDOMString("'xyz:in1'"),
															       XalanDOMString("'DATA'"));
								break;
							default:
								transformEngine.setStylesheetParam(XalanDOMString("input"),
																   XalanDOMString("'testing 1 2 3'"));
								break;
						}

						// Do a total end to end transform with no pre parsing of either xsl or xml files.
						int	theResult =
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

			XalanTransformer::terminate();

			logFile.logTestFileClose("Param Testing: ", "Done");
			logFile.close();

		}
		catch(...)
		{
			cerr << "Exception caught!!!" << endl << endl;
		}
	}

	return 0;

}
