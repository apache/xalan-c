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
//#include <strstream.h>
#include <sstream>
#else
#include <iostream>
//#include <strstream>
#include <sstream>
#endif

#include <stdio.h>

#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::cin;
	using std::endl;
	using std::ostringstream;
#endif


#include <parsers/DOMParser.hpp>



#include <XalanTransformer/XalanTransformer.hpp>
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



void
setHelp(FileUtility&	h)
{
	h.args.getHelpStream() << endl
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
		 << "-src type	(parsed source; XalanSourceTree(d), XercesParserLiasion, XercesDOM)"
		 << endl;
}



static const char* const 	excludeStylesheets[] =
{
	"output22.xsl",		// Excluded because it outputs EBCDIC
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
		cout << "ParseWTransformer - Failed to PARSE source document for " << h.data.testOrFile << endl;
		++h.data.fail;
		logFile.logErrorResult(h.data.testOrFile, XalanDOMString("Failed to PARSE source document."));
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

	h.data.xmlFormat = XalanDOMString("Xerces_DOM");

	DOMParser  theParser;
	theParser.setToCreateXMLDeclTypeNode(false);
	theParser.setDoValidation(true);
	theParser.setDoNamespaces(true);

	theParser.parse(xmlInput);

	DOM_Document theDOM = theParser.getDocument();

	theDOM.normalize();

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
		cout << "parseWXerces - Failed to PARSE source document for " << h.data.testOrFile << endl;
		++h.data.fail;
		logFile.logErrorResult(h.data.testOrFile, XalanDOMString("Failed to PARSE source document."));
	}
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

	try
	{
		HarnessInit		xmlPlatformUtils;

		FileUtility		h;

		// Set the program help string,  then get the command line parameters.
		//
		setHelp(h);

		if (h.getParams(argc, argv, "CONF-RESULTS") == true)
		{
			// Call the static initializers for xerces and xalan, and create a transformer
			//
			XalanTransformer::initialize();

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

				const FileNameVectorType	dirs = h.getDirectoryNames(h.args.base);

				for(FileNameVectorType::size_type	j = 0; j < dirs.size(); ++j)
				{
					// Skip all but the specified directory if the -sub cmd-line option was used.
					//
					const XalanDOMString	currentDir(dirs[j]);

					if (length(h.args.sub) > 0 && !equals(currentDir, h.args.sub))
					{
						continue;
					}					

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
						Hashtable attrs;
						const XalanDOMString	currentFile(files[i]);
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

						const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
						const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));
						const XSLTResultTarget	resultFile(theOutputFile);

						// Parsing(compile) the XSL stylesheet and report the results..
						//
						const XalanCompiledStylesheet*	compiledSS = 0;
						xalan.compileStylesheet(xslInputSource, compiledSS);
						if (compiledSS == 0 )
						{
							// Report the failure and be sure to increment fail count.
							//
							cout << "Failed to PARSE stylesheet for " << currentFile << endl;
							h.data.fail += 1;
							logFile.logErrorResult(currentFile, XalanDOMString("Failed to PARSE stylesheet."));
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

		XalanTransformer::terminate();
	}
	catch(...)
	{
		cerr << "Initialization failed!!!" << endl << endl;
	}

	return 0;
}
