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
//#include <direct.h>

// This is here for memory leak testing. 
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

#include <vector>



#include <xercesc/util/PlatformUtils.hpp>



#include <XalanTransformer/XalanTransformer.hpp>

#include <Harness/XMLFileReporter.hpp>
#include <Harness/FileUtility.hpp>
#include <Harness/HarnessInit.hpp>



#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
#endif



static const char* const	excludeStylesheets[] =
{
//	"impincl16.xml",
	0
};



inline bool
checkForExclusion(const XalanDOMString&		currentFile)
{
	for (int i = 0; excludeStylesheets[i] != 0; ++i)
	{	
		if (equals(currentFile, XalanDOMString(excludeStylesheets[i])))
		{
			return true;
		}
	}

	return false;
}



void
setHelp(FileUtility&	h)
{
	h.args.getHelpStream() << endl
		 << "stressmem dirname [-out -sub]"
		 << endl
		 << endl
		 << "dirname		(base directory for testcases)"
		 << endl
		 << "-out dirname	(base directory for output)"
		 << endl
		 << "-sub dirname (run files only from a specific directory)"
		 << endl;
}



int
main(int			argc,
	 const char*	argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	// Set the program help string,  then get the command line parameters.
	//
	try
	{
		HarnessInit		xmlPlatformUtils;

		FileUtility		h;

		setHelp(h);

		bool setGold = false;

		if (h.getParams(argc, argv, "MEM-RESULTS", setGold) == true)
		{
			// Get the list of Directories that are below perf
			const FileNameVectorType	dirs = h.getDirectoryNames(h.args.base);

			// Generate Unique Run id. (Only used to name the result logfile.)
			const XalanDOMString		UniqRunid = h.generateUniqRunid();

			// Defined basic constants for file manipulation 

			const XalanDOMString  resultFilePrefix(XalanDOMString("cpp-mem"));
			const XalanDOMString  resultsFile(h.args.output + resultFilePrefix + UniqRunid + FileUtility::s_xmlSuffix);

			XMLFileReporter		logFile(resultsFile);

			logFile.logTestFileInit("Memory Testing - Memory leaks detected during ConformanceTests. ");

			// Call the static initializers...
			XalanTransformer::initialize();

			try
			{
				bool foundDir = false;

				XalanTransformer		transformEngine;

				for(FileNameVectorType::size_type	j = 0; j < dirs.size(); ++j)
				{
					// Skip all but the specified directory if the -sub cmd-line option was used.
					//
					if (length(h.args.sub) > 0 && !equals(dirs[j], h.args.sub))
					{
						continue;
					}					
						
					// Check that output directory is there.
					const XalanDOMString  theOutputDir = h.args.output + dirs[j];
					h.checkAndCreateDir(theOutputDir);

					const FileNameVectorType	files = h.getTestFileNames(h.args.base, dirs[j],true);
					foundDir = true;

					for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
					{
						if (checkForExclusion(files[i]) == false)
						{
							// Output file name to result log and console.
							logFile.logTestCaseInit(files[i]);
							cout << files[i] << endl;

							const XalanDOMString  theXSLFile = h.args.base + dirs[j] + FileUtility::s_pathSep + files[i];
							const XalanDOMString  theXMLFile = h.generateFileName(theXSLFile,"xml");
							const XalanDOMString  theOutput =  h.args.output + dirs[j] + FileUtility::s_pathSep + files[i]; 
							const XalanDOMString  theOutputFile = h.generateFileName(theOutput, "out");

							// Do a total end to end transform with no pre parsing of either xsl or xml files.
							const XSLTResultTarget	theResultTarget(theOutputFile);

							const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
							const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));

							const int	theResult =
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

				// Check to see if -sub cmd-line directory was processed correctly.
				if (!foundDir)
				{
					cout << "Specified test directory: \"" << c_str(TranscodeToLocalCodePage(h.args.sub)) << "\" not found" << endl;
				}

				logFile.logTestFileClose("Memory Testing: ", "Done");
				logFile.close();
			}
			catch(...)
			{
				cerr << "Exception caught!!!" << endl << endl;
			}
		}

		XalanTransformer::terminate();
	}
	catch(...)
	{
		cerr << "Initialization failed!!!" << endl << endl;
	}

	return 0;
}
