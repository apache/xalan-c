/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif

#include <cstdio>



XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)



#include <xercesc/util/PlatformUtils.hpp>



#include <xercesc/parsers/XercesDOMParser.hpp>



#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>



// HARNESS HEADERS...
#include <Harness/XMLFileReporter.hpp>
#include <Harness/FileUtility.hpp>
#include <Harness/HarnessInit.hpp>



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



// Just hoist everything...
XALAN_CPP_NAMESPACE_USE



// This is here for memory leak testing. 
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



void
setHelp(FileUtility&	h)
{
	h.args.getHelpStream() << endl
		 << "conf dir [-sub -out -gold -source (XST | XPL | DOM)]"
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
		 << "-src type	(parsed source; XalanSourceTree(d), XercesParserLiasion, XercesDOM)"
		 << endl;
}



static const char* const 	excludeStylesheets[] =
{
//	"output22.xsl",		// Excluded because it outputs EBCDIC
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
parseWithTransformer(
			int								sourceType,
			XalanTransformer&				xalan,
			const XSLTInputSource&			xmlInput,
			const XalanCompiledStylesheet*	styleSheet,
			const XSLTResultTarget&			output, 
			XMLFileReporter&				logFile,
			FileUtility&					h)
{
	const XalanParsedSource* parsedSource = 0;

	// Parse the XML source accordingly.
	//
	if (sourceType != 0 )
	{
		xalan.parseSource(xmlInput, parsedSource, true);
		h.data.xmlFormat = XalanDOMString("XercesParserLiasion");
	}
	else
	{
		xalan.parseSource(xmlInput, parsedSource, false);
		h.data.xmlFormat = XalanDOMString("XalanSourceTree");
	}
				
	// If the source was parsed correctly then perform the transform else report the failure.
	//
	if (parsedSource == 0)
	{
		// Report the failure and be sure to increment fail count.
		//
		cout << "ParseWTransformer - Failed to parse source document for " << h.data.testOrFile << endl;
		++h.data.fail;
		logFile.logErrorResult(h.data.testOrFile, XalanDOMString("Failed to parse source document.  ") + xalan.getLastError());
	}
	else 
	{
		xalan.transform(*parsedSource, styleSheet, output);
		xalan.destroyParsedSource(parsedSource);
	}
}



void
parseWithXerces(
			XalanTransformer&				xalan,
			const XSLTInputSource&			xmlInput, 
			const XalanCompiledStylesheet*	styleSheet,
			const XSLTResultTarget&			output,
			XMLFileReporter&				logFile,
			FileUtility&					h)
{
	XALAN_USING_XERCES(XercesDOMParser)
	XALAN_USING_XERCES(DOMDocument)

	h.data.xmlFormat = XalanDOMString("Xerces_DOM");

	XercesDOMParser  theParser;

	theParser.setDoValidation(true);
	theParser.setDoNamespaces(true);

	theParser.parse(xmlInput);

	DOMDocument* const theDOM = theParser.getDocument();

	theDOM->normalize();

	XercesDOMSupport	theDOMSupport;
	XercesParserLiaison theParserLiaison;

	try
	{
		const XercesDOMWrapperParsedSource	parsedSource(
					theDOM, 
					theParserLiaison, 
					theDOMSupport, 
					XalanDOMString(xmlInput.getSystemId()));

		xalan.transform(parsedSource, styleSheet, output);
	}
	catch(...)
	{
		// Report the failure and be sure to increment fail count.
		//
		cout << "parseWXerces - Failed to parse source document for " << h.data.testOrFile << endl;
		++h.data.fail;
		logFile.logErrorResult(h.data.testOrFile, XalanDOMString("Failed to parse source document.  ") + xalan.getLastError());
	}
}



int
runTests(
			int		argc,
			char*	argv[])
{
	int	theResult = 0;

	try
	{
		HarnessInit		xmlPlatformUtils;

		FileUtility		h;

		// Set the program help string,  then get the command line parameters.
		//
		setHelp(h);

		if (h.getParams(argc, argv, "CONF-RESULTS") == true)
		{
			XalanTransformer	xalan;

			// Get drive designation for final analysis and generate Unique name for results log.
			//
			const XalanDOMString  drive(h.getDrive());			// This is used to get stylesheet for final analysis
			const XalanDOMString  resultFilePrefix("conf");		// This & UniqRunid used for log file name.
			const XalanDOMString  UniqRunid = h.generateUniqRunid(); 
			const XalanDOMString  resultsFile(drive + h.args.output + resultFilePrefix + UniqRunid + FileUtility::s_xmlSuffix);

			// Open results log, and do some initialization of result data.
			//
			XMLFileReporter	logFile(resultsFile);
			logFile.logTestFileInit("Conformance Testing:");
			h.data.xmlFormat = XalanDOMString("NotSet");

			// Get the list of Directories that are below conf and iterate through them
			//

			// Flag indicates directory found. Used in conjunction with -sub cmd-line arg.
			bool	foundDir = false;

			typedef FileUtility::FileNameVectorType		FileNameVectorType;

			const FileNameVectorType	dirs = h.getDirectoryNames(h.args.base);

			for(FileNameVectorType::size_type	j = 0; j < dirs.size(); ++j)
			{
				// Skip all but the specified directory if the -sub cmd-line option was used.
				//
				const XalanDOMString&	currentDir = dirs[j];

				if (length(h.args.sub) == 0 || equals(currentDir, h.args.sub) == true)
				{
					// Check that output directory is there.
					//
					const XalanDOMString  theOutputDir = h.args.output + currentDir;
					h.checkAndCreateDir(theOutputDir);

					// Indicate that directory was processed and get test files from the directory
					//
					foundDir = true;
					const FileNameVectorType files = h.getTestFileNames(h.args.base, currentDir, true);

					// Log directory in results log and process it's files.
					//
					logFile.logTestCaseInit(currentDir);

					for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
					{
						XMLFileReporter::Hashtable	attrs;

						const XalanDOMString&	currentFile = files[i];
						if (checkForExclusion(currentFile))
							continue;

						h.data.testOrFile = currentFile;
						const XalanDOMString  theXSLFile = h.args.base + currentDir + FileUtility::s_pathSep + currentFile;

						// Check and see if the .xml file exists. If not skip this .xsl file and continue.
						bool fileStatus = true;
						const XalanDOMString  theXMLFile = h.generateFileName(theXSLFile, "xml", &fileStatus);
						if (!fileStatus)
							continue;

						h.data.xmlFileURL = theXMLFile;
						h.data.xslFileURL = theXSLFile;


						XalanDOMString  theGoldFile = h.args.gold + currentDir + FileUtility::s_pathSep + currentFile;
						theGoldFile = h.generateFileName(theGoldFile, "out");

						const XalanDOMString  outbase =  h.args.output + currentDir + FileUtility::s_pathSep + currentFile; 
						const XalanDOMString  theOutputFile = h.generateFileName(outbase, "out");

						const XSLTInputSource	xslInputSource(theXSLFile);
						const XSLTInputSource	xmlInputSource(theXMLFile);
						const XSLTResultTarget	resultFile(theOutputFile);

						// Parsing(compile) the XSL stylesheet and report the results..
						//
						const XalanCompiledStylesheet*	compiledSS = 0;
						xalan.compileStylesheet(xslInputSource, compiledSS);
						if (compiledSS == 0 )
						{
							// Report the failure and be sure to increment fail count.
							//
							cout << "Failed to parse stylesheet for " << currentFile << endl;
							h.data.fail += 1;
							logFile.logErrorResult(currentFile, XalanDOMString("Failed to parse stylesheet.  ")  + xalan.getLastError());
							continue;
						}

						// Parse the Source XML based on input parameters, and then perform transform.
						//
						switch (h.args.source)
						{
							case 0:
							case 1:
								parseWithTransformer(h.args.source, xalan, xmlInputSource, compiledSS, resultFile, logFile, h);
								break;

							case 2:
								parseWithXerces(xalan, xmlInputSource, compiledSS, resultFile, logFile, h);
								break;
						}

						// Check and report results...  Then delete compiled stylesheet.
						//
						h.checkResults(theOutputFile, theGoldFile, logFile);
						xalan.destroyStylesheet(compiledSS);
					}

					logFile.logTestCaseClose("Done", "Pass");
				}
			}

			// Check to see if -sub cmd-line directory was processed correctly.
			//
			if (!foundDir)
			{
				cout << "Specified test directory: \"" << c_str(TranscodeToLocalCodePage(h.args.sub)) << "\" not found" << endl;
			}

			h.reportPassFail(logFile, UniqRunid);
			logFile.logTestFileClose("Conformance ", "Done");
			logFile.close();

			h.analyzeResults(xalan, resultsFile);
		}
	}
	catch(...)
	{
		cerr << "Initialization of testing harness failed!" << endl << endl;
	}

	return theResult;
}



int
main(
			int		argc,
			char*	argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

#if defined(XALAN_VQ_SPECIAL_TRACE)
	QuantifyStopRecordingData();
	QuantifyClearData();
#endif

	int	theResult = 0;

	try
	{
		XALAN_USING_XERCES(XMLPlatformUtils)

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
