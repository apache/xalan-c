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

#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#include <strstream.h>
#else
#include <iostream>
#include <strstream>
#endif

#include <stdio.h>

#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::cin;
	using std::endl;
	using std::ostrstream;
#endif


// XERCES HEADERS...
//	Most are included by HarnessInit.hpp
#include <parsers/DOMParser.hpp>

// XALAN HEADERS...
//	Most are included by FileUtility.hpp
#include <XalanTransformer/XercesDOMWrapperParsedSource.hpp>

// HARNESS HEADERS...
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


FileUtility		futil;



void
printArgOptions()
{
	cerr << endl
		 << "conf dir [-category -out -gold -source (XST | XPL | DOM)]"
		 << endl
		 << endl
		 << "dir		(base directory for testcases)"
		 << endl
		 << "-sub dir	(specific directory)"
		 << endl
		 << "-out dir	(base directory for output)"
		 << endl
		 << "-gold dir	(base directory for gold files)"
		 << endl
		 << "-source	(parsed source; XalanSourceTree(d), XercesParserLiasion, XercesDOM)"
		 << endl;
}

bool
getParams(int argc, 
		  const char*	argv[],
		  FileUtility& f,
		  XalanDOMString& baseDir,
		  XalanDOMString& outDir,
		  XalanDOMString& goldRoot,
		  XalanDOMString& category,
		  int& source)
{
	bool fSuccess = true;	// Used to continue argument loop
	bool fsetOut = true;	// Set default output directory, set to false if data is provided
	bool fsetGold = true;	// Set default gold directory, set to false if data is provided
	

	// Insure that required "-base" argument is there.
	if (argc == 1 || argv[1][0] == '-')
	{
		printArgOptions(); 
		return false;
	}
	else
	{
		if (f.checkDir(XalanDOMString(argv[1])))
		{
			assign(baseDir, XalanDOMString(argv[1]));
//			insert(baseDir, 0, pathSep);
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
				append(outDir, pathSep);
				f.checkAndCreateDir(outDir);
				fsetOut = false;
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
//				insert(goldRoot, 0, pathSep);
				fsetGold = false;
			}
			else
			{
				printArgOptions();
				fSuccess = false;
			}
		}
		else if(!stricmp("-source", argv[i]))
		{
			++i;
			if(i < argc && argv[i][0] != '-')
			{
				if (stricmp(argv[i],"XPL") == 0)
				{
					source = 1;
				}
				else if (stricmp(argv[i], "DOM") == 0)
				{
					source = 2;
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
		}
		else if(!stricmp("-sub", argv[i]))
		{
			++i;
			if(i < argc && argv[i][0] != '-')
			{
				assign(category, XalanDOMString(argv[i]));
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
	if (fsetOut)
	{
		unsigned int ii = lastIndexOf(baseDir,charAt(pathSep,0));
		outDir = substring(baseDir, 0, ii+1);
		append(outDir,XalanDOMString("CONF-results"));
		f.checkAndCreateDir(outDir);
		append(outDir,pathSep);
	}
	if (fsetGold)
	{
		goldRoot = baseDir;
		append(goldRoot,XalanDOMString("-gold"));
		f.checkAndCreateDir(goldRoot);
		//append(goldRoot,pathSep);
	}
	
	// Add the path seperator to the end of the base directory 
	// here after we've finished using it for all directory creation.

	append(baseDir,pathSep);
	append(goldRoot,pathSep);
	return fSuccess;
}



static const char* const 	excludeStylesheets[] =
{
	"output22.xsl",
	"entref01.xsl",
	"select73.xsl",
	"sort07.xsl",
	0
};




inline bool
checkForExclusion(const XalanDOMString&		currentFile)
{
	for (size_t i = 0; excludeStylesheets[i] != 0; ++i)
	{	
		if (equals(currentFile, excludeStylesheets[i]))
		{
			return true;
		}
	}

	return false;
}



void
parseWithTransformer(int sourceType, XalanTransformer &xalan, const XSLTInputSource &xmlInput, 
					 const XalanCompiledStylesheet* styleSheet, const XSLTResultTarget &output, 
					 XMLFileReporter &logFile)
{
	const XalanParsedSource* parsedSource = 0;

	if (sourceType != 0 )
	{
		xalan.parseSource(xmlInput, parsedSource, true);
		futil.data.xmlFormat = XalanDOMString("XercesParserLiasion");
	}
	else
	{
		xalan.parseSource(xmlInput, parsedSource, false);
		futil.data.xmlFormat = XalanDOMString("XalanSourceTree");
	}
					
	if (parsedSource == 0)
	{
		// Report the failure and be sure to increment fail count.
		cout << "Failed to PARSE source document for " << futil.data.testOrFile << endl;
		++futil.data.fail;
		logFile.logErrorResult(futil.data.testOrFile, XalanDOMString("Failed to PARSE source document."));
	}

	xalan.transform(*parsedSource, styleSheet, output);
	xalan.destroyParsedSource(parsedSource);
}



void
parseWithXerces(int sourceType, XalanTransformer &xalan, const XSLTInputSource &xmlInput, 
				const XalanCompiledStylesheet* styleSheet, const XSLTResultTarget &output, 
				XMLFileReporter &logFile)
{

	futil.data.xmlFormat = XalanDOMString("Xerces_DOM");

	DOMParser  theParser;
	theParser.setToCreateXMLDeclTypeNode(false);
	theParser.setDoValidation(true);
	theParser.setDoNamespaces(true);

	theParser.parse(xmlInput);
	DOM_Document theDOM = theParser.getDocument();

	theDOM.normalize();

	XercesDOMSupport	theDOMSupport;
	XercesParserLiaison theParserLiaison(theDOMSupport);

	try
	{
		 const XercesDOMWrapperParsedSource		parsedSource(
					theDOM, 
					theParserLiaison, 
					theDOMSupport, 
					XalanDOMString(xmlInput.getSystemId()));

		xalan.transform(parsedSource, styleSheet, output);
	}
	catch(...)
	{
		// Report the failure and be sure to increment fail count.
		cout << "Failed to PARSE source document for " << futil.data.testOrFile << endl;
		++futil.data.fail;
		logFile.logErrorResult(futil.data.testOrFile, XalanDOMString("Failed to PARSE source document."));
	}
}

int
main(
	 int			argc,
	 const char*	argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif


	Hashtable runAttrs;	// Attribute lists for conformance testing.


	XalanDOMString  category;	// Test all of base dir by default
	XalanDOMString  baseDir, outputRoot, goldRoot;
	int source = 0;				// Format the source xml will be. Default is XalanSourceTree 0.	

	if (getParams(argc, argv, futil, baseDir, outputRoot, goldRoot, category, source) == true)
	{
		// Call the static initializers for xerces and xalan, and create a transformer
		HarnessInit xmlPlatformUtils;
		XalanTransformer::initialize();
		XalanTransformer xalan;

		// Generate Unique Run id and processor info
		const XalanDOMString	UniqRunid = futil.generateUniqRunid();

		// Defined basic constants for file manipulation and open results file
		const XalanDOMString  drive(futil.getDrive());
		const XalanDOMString  resultFilePrefix("cpp");
		const XalanDOMString  resultsFile(drive + outputRoot + resultFilePrefix + UniqRunid + XMLSuffix);


		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("Conformance Testing:");


		// Get the list of Directories that are below conf
		const FileNameVectorType	dirs = futil.getDirectoryNames(baseDir);


		for(FileNameVectorType::size_type	j = 0; j < dirs.size(); ++j)
		{
			const XalanDOMString currentDir(dirs[j]);

			// Run specific category of files from given directory
			if (length(category) > 0 && !equals(currentDir, category))
			{
				continue;
			}					
					
			// Check that output directory is there.
			const XalanDOMString  theOutputDir = outputRoot + currentDir;
			futil.checkAndCreateDir(theOutputDir);
				
			// Get the files found in the test directory
			const FileNameVectorType files = futil.getTestFileNames(baseDir, currentDir, true);
				
			logFile.logTestCaseInit(currentDir);

			for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
			{
				Hashtable attrs;
				const XalanDOMString currentFile(files[i]);
				futil.data.testOrFile = currentFile;

				if (checkForExclusion(currentFile))
					continue;

				const XalanDOMString  theXSLFile = baseDir + currentDir + pathSep + currentFile;
				const XalanDOMString  theXMLFile = futil.generateFileName(theXSLFile,"xml");
				futil.data.xmlFileURL = theXMLFile;
				futil.data.xslFileURL = theXSLFile;
				futil.data.testBase = baseDir;

				XalanDOMString  theGoldFile = goldRoot + currentDir + pathSep + currentFile;
				theGoldFile = futil.generateFileName(theGoldFile, "out");

				const XalanDOMString  outbase =  outputRoot + currentDir + pathSep + currentFile; 
				const XalanDOMString  theOutputFile = futil.generateFileName(outbase, "out");

				const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
				const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));
				const XSLTResultTarget	resultFile(theOutputFile);

				//
				// Parsing(compile) the XSL stylesheet and report the results..
				//
				const XalanCompiledStylesheet*	compiledSS = 0;
				xalan.compileStylesheet(xslInputSource, compiledSS);
				if (compiledSS == 0 )
				{
					// Report the failure and be sure to increment fail count.
					cout << "Failed to PARSE stylesheet for " << currentFile << endl;
					futil.data.fail += 1;
					logFile.logErrorResult(currentFile, XalanDOMString("Failed to PARSE stylesheet."));
					continue;
				}

				//
				// Parsing the Source XML based on input params. Default is to use XalanParsedSource.
				//
				switch (source)
				{
					case 0:
					case 1:
						parseWithTransformer(source, xalan, xmlInputSource, compiledSS, resultFile, logFile);
						break;

					case 2:
						parseWithXerces(source, xalan, xmlInputSource, compiledSS, resultFile, logFile);
						break;
				}

				//
				// Perform One transform using parsed stylesheet and parsed xml source, report results...

				futil.checkResults(theOutputFile, 
								  theGoldFile, 
								  logFile);

				xalan.destroyStylesheet(compiledSS);

			}	//for files

			logFile.logTestCaseClose("Done", "Pass");

		}		//for directories

		futil.reportPassFail(logFile, UniqRunid);
		logFile.logTestFileClose("Conformance ", "Done");
		logFile.close();

		futil.analyzeResults(xalan, resultsFile);

	} //if getParams


	XalanTransformer::terminate();

	return 0;
}
