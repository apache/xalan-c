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
#include "xalanc/Harness/XalanXMLFileReporter.hpp"
#include "xalanc/Harness/XalanFileUtility.hpp"



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
setHelp(XalanFileUtility&	h)
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
    MemoryManagerType& mgr = XalanMemMgrs::getDefaultXercesMemMgr();

	for (size_t i = 0; excludeStylesheets[i] != 0; ++i)
	{	
		if (equals(currentFile, XalanDOMString(excludeStylesheets[i], mgr)))
		{
			return true;
		}
	}

	return false;
}



int
parseWithTransformer(
			int								sourceType,
			XalanTransformer&				xalan,
			const XSLTInputSource&			xmlInput,
			const XalanCompiledStylesheet*	styleSheet,
			const XSLTResultTarget&			output, 
			XalanXMLFileReporter&				logFile,
			XalanFileUtility&					h)
{
	const XalanParsedSource* parsedSource = 0;

    MemoryManagerType& mgr = XalanMemMgrs::getDefaultXercesMemMgr();

    int theResult = 0;

	// Parse the XML source accordingly.
	//
	if (sourceType != 0 )
	{
		theResult = xalan.parseSource(xmlInput, parsedSource, true);
		h.data.xmlFormat = XalanDOMString("XercesParserLiasion", mgr);
	}
	else
	{
		theResult = xalan.parseSource(xmlInput, parsedSource, false);
		h.data.xmlFormat = XalanDOMString("XalanSourceTree", mgr);
	}
				
	// If the source was parsed correctly then perform the transform else report the failure.
	//
	if (parsedSource == 0)
	{
		// Report the failure and be sure to increment fail count.
		//
		cout << "ParseWTransformer - Failed to parse source document for " << h.data.testOrFile << endl;
		++h.data.fail;
        XalanDOMString tmp("Failed to parse source document.  ", mgr);

        tmp.append(xalan.getLastError());

		logFile.logErrorResult(h.data.testOrFile,  tmp );
	}
	else 
	{
		theResult = xalan.transform(*parsedSource, styleSheet, output);
		xalan.destroyParsedSource(parsedSource);
	}

    return theResult;
}



int
parseWithXerces(
			XalanTransformer&				xalan,
			const XSLTInputSource&			xmlInput, 
			const XalanCompiledStylesheet*	styleSheet,
			const XSLTResultTarget&			output,
			XalanXMLFileReporter&			logFile,
			XalanFileUtility&				h)
{
	XALAN_USING_XERCES(XercesDOMParser)
	XALAN_USING_XERCES(DOMDocument)

    MemoryManagerType& mgr = XalanMemMgrs::getDefaultXercesMemMgr();

	h.data.xmlFormat = XalanDOMString("Xerces_DOM", mgr);

	XercesDOMParser  theParser;

	theParser.setDoValidation(true);
	theParser.setDoNamespaces(true);

	theParser.parse(xmlInput);

	DOMDocument* const theDOM = theParser.getDocument();

	theDOM->normalize();

	XercesDOMSupport	theDOMSupport(mgr);
	XercesParserLiaison theParserLiaison(mgr);

    int theResult = 0;

	try
	{
		const XercesDOMWrapperParsedSource	parsedSource(
					theDOM, 
					theParserLiaison, 
					theDOMSupport, 
					XalanDOMString(xmlInput.getSystemId(), mgr),
                    mgr);

		theResult = xalan.transform(parsedSource, styleSheet, output);
	}
	catch(...)
	{
		// Report the failure and be sure to increment fail count.
		//
		cout << "parseWXerces - Failed to parse source document for " << h.data.testOrFile << endl;
		++h.data.fail;
        XalanDOMString resultString("Failed to parse source document.  ", mgr);
        resultString.append( xalan.getLastError());
		logFile.logErrorResult(h.data.testOrFile,  resultString);
	}

    return theResult;
}



int
runTests(
			int		argc,
			char*	argv[])
{
	int	theResult = 0;

    MemoryManagerType& mgr = XalanMemMgrs::getDefaultXercesMemMgr();

	try
	{
		XalanFileUtility	h(mgr);

		// Set the program help string,  then get the command line parameters.
		//
		setHelp(h);

		if (h.getParams(argc, argv, "CONF-RESULTS") == true)
		{
			XalanTransformer	xalan;

			// Get drive designation for final analysis and generate Unique name for results log.
			//
			XalanDOMString  drive( mgr);			// This is used to get stylesheet for final analysis
            h.getDrive(drive);
			const XalanDOMString  resultFilePrefix("conf", mgr);		// This & UniqRunid used for log file name.
			XalanDOMString  UniqRunid(  mgr); 
            h.generateUniqRunid(UniqRunid);
			XalanDOMString  resultsFile(drive, mgr);
            resultsFile += h.args.output;
            resultsFile += resultFilePrefix;
            resultsFile += UniqRunid;
            resultsFile += XalanFileUtility::s_xmlSuffix;

			// Open results log, and do some initialization of result data.
			//
			XalanXMLFileReporter	logFile(mgr, resultsFile);
			logFile.logTestFileInit("Conformance Testing:");
			h.data.xmlFormat = XalanDOMString("NotSet", mgr);

			// Get the list of Directories that are below conf and iterate through them
			//

			// Flag indicates directory found. Used in conjunction with -sub cmd-line arg.
			bool	foundDir = false;

			typedef XalanFileUtility::FileNameVectorType		FileNameVectorType;

			FileNameVectorType	dirs(mgr);
            h.getDirectoryNames(h.args.base, dirs);

            int theResult = 0;

			for(FileNameVectorType::size_type j = 0;
                    j < dirs.size() && theResult == 0;
                        ++j)
			{
				// Skip all but the specified directory if the -sub cmd-line option was used.
				//
				const XalanDOMString&	currentDir = dirs[j];

				if (length(h.args.sub) == 0 || equals(currentDir, h.args.sub) == true)
				{
					// Check that output directory is there.
					//
					XalanDOMString  theOutputDir(mgr);
                    theOutputDir = h.args.output;
                    theOutputDir += currentDir;

					h.checkAndCreateDir(theOutputDir);

					// Indicate that directory was processed and get test files from the directory
					//
					foundDir = true;
					FileNameVectorType files(mgr);
                    h.getTestFileNames(h.args.base, currentDir, true, files);

					// Log directory in results log and process it's files.
					//
					logFile.logTestCaseInit(currentDir);

					for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
					{
						XalanXMLFileReporter::Hashtable	attrs(mgr);

						const XalanDOMString&	currentFile = files[i];
						if (checkForExclusion(currentFile))
							continue;

						h.data.testOrFile = currentFile;
						XalanDOMString  theXSLFile( h.args.base, mgr);
                        theXSLFile += currentDir;
                        theXSLFile += XalanFileUtility::s_pathSep;
                        theXSLFile += currentFile;

						// Check and see if the .xml file exists. If not skip this .xsl file and continue.
						bool fileStatus = true;
						XalanDOMString  theXMLFile(mgr); 
                        h.generateFileName(theXSLFile, "xml", theXMLFile, &fileStatus);
						if (!fileStatus)
							continue;

						h.data.xmlFileURL = theXMLFile;
						h.data.xslFileURL = theXSLFile;


						XalanDOMString  theGoldFile(h.args.gold, mgr);
                        theGoldFile += currentDir;
                        theGoldFile += XalanFileUtility::s_pathSep;
                        theGoldFile += currentFile;
						 
                        h.generateFileName(theGoldFile, "out", theGoldFile);

						XalanDOMString  outbase (h.args.output, mgr);
                        outbase += currentDir;
                        outbase += XalanFileUtility::s_pathSep;
                        outbase += currentFile; 
						XalanDOMString  theOutputFile(mgr);
                        h.generateFileName(outbase, "out", theOutputFile);

						const XSLTInputSource	xslInputSource(theXSLFile, mgr);
						const XSLTInputSource	xmlInputSource(theXMLFile, mgr);
						const XSLTResultTarget	resultFile(theOutputFile, mgr);

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
                            XalanDOMString tmp("Failed to parse stylesheet.  ", mgr);
                            tmp += XalanDOMString(xalan.getLastError(), mgr);
							logFile.logErrorResult(currentFile, mgr);
							continue;
						}

						// Parse the Source XML based on input parameters, and then perform transform.
						//
						switch (h.args.source)
						{
							case 0:
							case 1:
								theResult = parseWithTransformer(h.args.source, xalan, xmlInputSource, compiledSS, resultFile, logFile, h);
								break;

							case 2:
								theResult = parseWithXerces(xalan, xmlInputSource, compiledSS, resultFile, logFile, h);
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
                CharVectorType vect(mgr);
                TranscodeToLocalCodePage(h.args.sub, vect);

				cout << "Specified test directory: \"" << c_str(vect) << "\" not found" << endl;
			}
            else if (theResult != 0)
            {
				cout << "An unexpected tranformer error occurred.  The error code is "
                     << theResult
                     << "\n"
                     << "The error message is \""
                     << xalan.getLastError()
                     << endl;
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
