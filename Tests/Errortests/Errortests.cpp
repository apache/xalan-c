/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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

#include <cstdio>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#include <strstream.h>
#else
#include <iostream>
#include <strstream>
#endif

#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
#endif



// XERCES HEADERS...
#include <xercesc/util/PlatformUtils.hpp>



// XALAN HEADERS...
#include <PlatformSupport/DOMStringHelper.hpp>



#include <XalanSourceTree/FormatterToSourceTree.hpp>
#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



#include <Harness/XMLFileReporter.hpp>
#include <Harness/FileUtility.hpp>
#include <Harness/HarnessInit.hpp>



#if defined(XALAN_NO_NAMESPACES)
typedef map<XalanDOMString, XalanDOMString, less<XalanDOMString> >	Hashtable;
#else
typedef std::map<XalanDOMString, XalanDOMString>  Hashtable;
#endif


// This is here for memory leak testing. 
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

const char* const 	excludeStylesheets[] =
{
	0
};



void
setHelp(FileUtility&	h)
{
	h.args.getHelpStream() << endl
		 << "errortests dir [-sub -out]"
		 << endl
		 << endl
		 << "dir		(location of conformance directory)"
		 << endl
		 << "-sub dir	(specific directory)"
		 << endl
		 << "-out dir	(base directory for output)"
		 << endl;
}



inline bool
checkForExclusion(XalanDOMString currentFile)
{
	for (int i = 0; excludeStylesheets[i] != 0; i++)
	{	
		if (equals(currentFile, XalanDOMString(excludeStylesheets[i])))
		{
			return true;
		}
	}

	return false;
}



int
runTests(
			int				argc,
			const char*		argv[])
{
	HarnessInit		xmlPlatformUtils;

	FileUtility		h;

	// Set the program help string,  then get the command line parameters.
	//
	setHelp(h);

	bool	setGold = false;

	if (h.getParams(argc, argv, "ERR-RESULTS", setGold) == true)
	{
		XalanTransformer				xalan;

		XalanSourceTreeDOMSupport		domSupport;
		XalanSourceTreeParserLiaison	parserLiaison(domSupport);

		domSupport.setParserLiaison(&parserLiaison);

		// Generate Unique Run id and processor info
		const XalanDOMString UniqRunid = h.generateUniqRunid();

		// Defined basic constants for file manipulation and open results file
		const XalanDOMString  resultFilePrefix("cpperr");
		const XalanDOMString  resultsFile(h.args.output + resultFilePrefix + UniqRunid + FileUtility::s_xmlSuffix);

		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("Error Testing:");

		// Get the list of Directories that are below conf
		bool foundDir = false;		// Flag indicates directory found. Used in conjunction with -sub cmd-line arg.
		const FileNameVectorType	dirs = h.getDirectoryNames(h.args.base);

		for(FileNameVectorType::size_type	j = 0; j < dirs.size(); ++j)
		{
			// If conformance directory structure does not exist, it needs to be created.
			const XalanDOMString  confSubdir = h.args.output + dirs[j];
			h.checkAndCreateDir(confSubdir);

				// Set up to get files from the associated error directories
				const XalanDOMString&	currentDir = dirs[j];

				// Run specific category of files from given directory
				if (length(h.args.sub) == 0 || equals(currentDir, h.args.sub) == true)
				{
					// Check that output directory is there.
					const XalanDOMString  theOutputDir = h.args.output + currentDir;
					h.checkAndCreateDir(theOutputDir);

					
					// Indicate that directory was processed and get test files from the directory
					foundDir = true;
					logFile.logTestCaseInit(currentDir);

					const FileNameVectorType files = h.getTestFileNames(h.args.base, currentDir, false);
					for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
					{
						Hashtable attrs;
						const XalanDOMString currentFile(files[i]);
						h.data.testOrFile = currentFile;

						if (checkForExclusion(currentFile))
							continue;

						const XalanDOMString  theXSLFile= h.args.base + currentDir + FileUtility::s_pathSep + currentFile;
						const XalanDOMString  theXMLFile = h.generateFileName(theXSLFile,"xml");
						XalanDOMString  theGoldFile = h.args.gold + currentDir + FileUtility::s_pathSep + currentFile;
						theGoldFile = h.generateFileName(theGoldFile, "out");

						const XalanDOMString  outbase =  h.args.output + currentDir + FileUtility::s_pathSep + currentFile; 
						const XalanDOMString  theOutputFile = h.generateFileName(outbase, "out");

						const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
						const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));
						const XSLTInputSource	goldInputSource(c_wstr(theGoldFile));
						const XSLTResultTarget	resultFile(theOutputFile);

						// Parsing(compile) the XSL stylesheet and report the results..
						//
						const XalanCompiledStylesheet*	compiledSS = 0;
						try 
						{
							cout << endl << "PARSING STYLESHEET FOR: " << currentFile << endl;

							xalan.compileStylesheet(xslInputSource, compiledSS);
							if (compiledSS == 0 )
							{
								cout << "FAILED to parse stylesheet for " << currentFile << endl;
								cout << "Reason: " << xalan.getLastError() << endl;
								logFile.logErrorResult(currentFile, XalanDOMString(xalan.getLastError()));
								continue;
							}
						}
						catch(...)
						{
							cerr << "Exception caught!!!" << endl << endl;
						}

						// Parsing the input XML and report the results..
						//
						cout << "PARSING SOURCE XML FOR: " << currentFile << endl;
						const XalanParsedSource*	parsedSource = 0;
						xalan.parseSource(xmlInputSource, parsedSource);
						if (parsedSource == 0)
						{
							cout << "Failed to PARSE source document for " << currentFile << endl;
							continue;
						}

						// Perform One transform using parsed stylesheet and parsed xml source, report results...
						// 
						cout << "TRANSFORMING: " << currentFile << endl;

						const int	theResult = xalan.transform(*parsedSource, compiledSS, resultFile);

						if (theResult != 0)
						{
							cout << "FAILED to transform stylesheet for " << currentFile << endl;
							cout << "Reason: " << xalan.getLastError() << endl;
							logFile.logErrorResult(currentFile, XalanDOMString(xalan.getLastError()));
						}
						else
						{
							logFile.logCheckPass(currentFile);
						}

						parserLiaison.reset();
						xalan.destroyParsedSource(parsedSource);
						xalan.destroyStylesheet(compiledSS);
					}
				}

				logFile.logTestCaseClose("Done", "Pass");
			}


		// Check to see if -sub cmd-line directory was processed correctly.
		if (!foundDir)
		{
			cout << "Specified test directory: \"" << c_str(TranscodeToLocalCodePage(h.args.sub)) << "\" not found" << endl;
		}

		h.reportPassFail(logFile, UniqRunid);

		logFile.logTestFileClose("Conformance ", "Done");
		logFile.close();
	}

	return 0;
}



int
main(
			int				argc,
			const char*		argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	int	theResult = 0;

	try
	{
		// Call the static initializers for xerces and xalan, and create a transformer
		//
		XMLPlatformUtils::Initialize();

		XalanTransformer::initialize();

		theResult = runTests(argc, argv);

		XalanTransformer::terminate();

		XMLPlatformUtils::Terminate();

		XalanTransformer::ICUCleanUp();
	}
	catch(...)
	{
		cerr << "Initialization failed!" << endl << endl;

		theResult = -1;
	}

	return theResult;
}
