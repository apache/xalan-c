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

#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::cin;
	using std::endl;
	using std::ifstream;
	using std::ios_base;
	using std::ostrstream;
	using std::string;
#endif

// XERCES HEADERS...
//	Are included by HarnessInit.hpp

// XALAN HEADERS...
//	Are included by FileUtility.hpp

// HARNESS HEADERS...
#include <XMLFileReporter.hpp>
#include <FileUtility.hpp>
#include <HarnessInit.hpp>

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
		 << "Perf dirname [-out -gold]"
		 << endl
		 << endl
		 << "dirname		(base directory for testcases)"
		 << endl
		 << "-out dirname	(base directory for output)"
		 << endl
		 << "-gold dirname	(base directory for gold files)"
		 << endl;
}

bool
getParams(int argc, 
		  const char*	argv[],
		  FileUtility& f,
		  XalanDOMString& baseDir,
		  XalanDOMString& outDir,
		  XalanDOMString& goldRoot)
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
		if (f.checkDir(pathSep + XalanDOMString(argv[1])))
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
				fsetGold = false;
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
		append(outDir,XalanDOMString("DOMCOM-results"));
		f.checkAndCreateDir(outDir);
		append(outDir,pathSep);
	}
	if (fsetGold)
	{
		goldRoot = baseDir;
		append(goldRoot,XalanDOMString("-gold"));
		f.checkAndCreateDir(goldRoot);
		append(goldRoot,pathSep);
	}
	
	// Add the path seperator to the end of the base directory 
	// here after we've finished using it for all directory creation.

	append(baseDir,pathSep);
	return fSuccess;
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

	HarnessInit xmlPlatformUtils;
	XalanTransformer::initialize();

	{
		Hashtable runAttrs;		// Attribute list for perfdata element
		int transResult = 0;

		XalanDOMString  category;	// Test all of base dir by default
		XalanDOMString  baseDir, outputRoot, goldRoot, fileName;	


		//FileUtility futil;

		if (getParams(argc, argv, futil, baseDir, outputRoot, goldRoot) == true)
		{
			//
			// Call the static initializers for xerces and xalan, and create a transformer
			//
			XalanTransformer xalan;

			XalanSourceTreeDOMSupport domSupport;
			XalanSourceTreeParserLiaison parserLiaison(domSupport);
			domSupport.setParserLiaison(&parserLiaison);


			// Generate Unique Run id and processor info
			const XalanDOMString UniqRunid = futil.GenerateUniqRunid();


			// Defined basic constants for file manipulation and open results file
			const XalanDOMString  resultFilePrefix("cpp");
			const XalanDOMString  resultsFile(outputRoot + resultFilePrefix + UniqRunid + XMLSuffix);


			XMLFileReporter	logFile(resultsFile);
			logFile.logTestFileInit("Comparison Testing:");


			// Create run entry that contains runid and number of iterations used for averages.
			runAttrs.insert(Hashtable::value_type(XalanDOMString("UniqRunid"), UniqRunid));
			runAttrs.insert(Hashtable::value_type(XalanDOMString("Xerces-Version "), futil.getXercesVersion()));
			logFile.logElement(10, "perfdata", runAttrs, "xxx");
				
			// Specify the "test" directory for both input and output.
			const XalanDOMString  xDir("domcomtests");
			const XalanDOMString  theOutputDir = outputRoot + xDir;
			futil.checkAndCreateDir(theOutputDir);

			// Get the files found in the test directory
			const FileNameVectorType files = futil.getTestFileNames(baseDir, xDir, true);

				for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
				{

					Hashtable attrs;
					fileName = files[i];

					attrs.insert(Hashtable::value_type(XalanDOMString("idref"), fileName));
					attrs.insert(Hashtable::value_type(XalanDOMString("UniqRunid"),UniqRunid));
					attrs.insert(Hashtable::value_type(XalanDOMString("processor"),processorType));

					const XalanDOMString  theXSLFile= baseDir + xDir + pathSep + fileName;
					const XalanDOMString  theXMLFile = futil.GenerateFileName(theXSLFile,"xml");
					XalanDOMString  theGoldFile = goldRoot +xDir + pathSep + fileName;
					theGoldFile = futil.GenerateFileName(theGoldFile, "out");

					const XalanDOMString  outbase =  outputRoot + xDir + pathSep + fileName; 
					const XalanDOMString  theOutputFile = futil.GenerateFileName(outbase, "out");
					cout << endl << endl << "Processing: " << fileName << endl;

					const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
					const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));
					const XSLTInputSource	goldInputSource(c_wstr(theGoldFile));

					// Use a XalanSourceTreeDocument to create the XSLTResultTarget. 
					XalanSourceTreeDocument* dom = parserLiaison.createXalanSourceTreeDocument();
					FormatterToSourceTree domOut(dom); 
					XSLTResultTarget domResultTarget;
					domResultTarget.setDocumentHandler(&domOut);

					//
					// Parsing(compile) the XSL stylesheet and report the results..
					//
					const XalanCompiledStylesheet*	compiledSS = 0;
					xalan.compileStylesheet(xslInputSource, compiledSS);
					if (compiledSS == 0 )
					{
						continue;
					}

					//
					// Parsing the input XML and report the results..
					//
					const XalanParsedSource*	parsedSource = 0;
					xalan.parseSource(xmlInputSource, parsedSource);
					if (parsedSource == 0)
					{
						continue;
					}

					//
					// Perform One transform using parsed stylesheet and unparsed xml source, report results...
					// 
					transResult = xalan.transform(*parsedSource, compiledSS, domResultTarget);

					if(!transResult)
					{
						futil.compareResults(theOutputFile, compiledSS, dom, fileName, goldInputSource);
					}
					else
					{
						cout << xalan.getLastError();
						return 0;
					}

					parserLiaison.reset();
					xalan.destroyParsedSource(parsedSource);
					xalan.destroyStylesheet(compiledSS);

				}//for files

		logFile.logTestFileClose("Performance", "Done");
		logFile.close();

		} //if getParams
	}

	XalanTransformer::terminate();


	return 0;
}
