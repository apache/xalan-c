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
// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <iostream>
#include <strstream>
#include <stdio.h>
#include <direct.h>
#include <vector>



// This is here for memory leak testing. 
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanFileOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>



#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XMLSupport/FormatterTreeWalker.hpp>



#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XSLT/XSLTResultTarget.hpp>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheetDefault.hpp>



#include <Harness/XMLFileReporter.hpp>
#include <Harness/FileUtility.hpp>
#include <Harness/HarnessInit.hpp>



#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMFormatterWalker.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>



XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)



// Just hoist everything...
XALAN_CPP_NAMESPACE_USE



void
printArgOptions()
{
	cerr << endl
		 << "dom2dom dirname [-out]"
		 << endl
		 << endl
		 << "dirname		(base directory for testcases)"
		 << endl
		 << "-out dirname	(base directory for output)"
		 << endl;
}



bool
getParams(
			int					argc, 
			const char*			argv[],
			FileUtility&		f,
			XalanDOMString&		basedir,
			XalanDOMString&		outdir)
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
		if (f.checkDir(FileUtility::s_pathSep + XalanDOMString(argv[1])))
		{
			assign(basedir, XalanDOMString(argv[1]));
			insert(basedir, 0, FileUtility::s_pathSep);
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
		unsigned int ii = lastIndexOf(basedir,charAt(FileUtility::s_pathSep,0));
		outdir = substring(basedir, 0, ii+1);
		append(outdir, "Dom2Dom-RESULTS\\");
		f.checkAndCreateDir(outdir);
	}
	
	// Add the path seperator to the end of the base directory
	append(basedir, FileUtility::s_pathSep);

	return fSuccess;
}



FormatterListener* 
getXMLFormatter(bool					shouldWriteXMLHeader,
				bool					stripCData,
				bool					escapeCData,
				PrintWriter&			resultWriter,
				int						indentAmount,
				const XalanDOMString&	mimeEncoding,
				const StylesheetRoot*	stylesheet)
{
	FormatterListener*	formatter = 0;

		XalanDOMString	version;
		bool			outputIndent = false;
		XalanDOMString	mediatype;
		XalanDOMString	doctypeSystem;
		XalanDOMString	doctypePublic;
		XalanDOMString	standalone;

		if (stylesheet != 0)
		{
			version = stylesheet->getOutputVersion();

			mediatype = stylesheet->getOutputMediaType();
			doctypeSystem = stylesheet->getOutputDoctypeSystem();
			doctypePublic = stylesheet->getOutputDoctypePublic();
			standalone = stylesheet->getOutputStandalone();
		}

		FormatterToXML* const	fToXML =
			new FormatterToXML(
					resultWriter,
					version,
					outputIndent,
					indentAmount,
					mimeEncoding,
					mediatype,
					doctypeSystem,
					doctypePublic,
					true,	// xmlDecl
					standalone);

		fToXML->setShouldWriteXMLHeader(shouldWriteXMLHeader);
		fToXML->setStripCData(stripCData);
		fToXML->setEscapeCData(escapeCData);

		formatter = fToXML;
		return formatter;
}



int
runTests(
		  int			argc,
		  const char*	argv [])
{

	HarnessInit		xmlPlatformUtils;

	FileUtility		f;

	XalanDOMString  category;	// Test all of base dir by default
	XalanDOMString  baseDir;	
	XalanDOMString  outputRoot;
	
	if (getParams(argc, argv, f, baseDir, outputRoot) == true)
	{
		typedef FileUtility::FileNameVectorType		FileNameVectorType;

		// Get the list of Directories that are below perf
		const FileNameVectorType	dirs = f.getDirectoryNames(baseDir);

		// Generate Unique Run id. (Only used to name the result logfile.)
		const XalanDOMString UniqRunid = f.generateUniqRunid();

		// Defined basic constants for file manipulation 

		const XalanDOMString  resultFilePrefix("dom2dom");
		const XalanDOMString  resultsFile(outputRoot + resultFilePrefix + UniqRunid + FileUtility::s_xmlSuffix);
		
		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("Dom2Dom Testing: Treating all inputs and outputs as DOM's. ");

		try
		{
			XalanTransformer		transformEngine;
						
			XercesDOMSupport domSupport;
			XercesParserLiaison parserLiaison(domSupport);
					
			// Specify the "test" directory for both input and output.
			const XalanDOMString  xMan("dtod");
			const XalanDOMString  theOutputDir = outputRoot + xMan;
			f.checkAndCreateDir(theOutputDir);

			// Get the files found in the test directory
			const FileNameVectorType	files = f.getTestFileNames(baseDir, xMan,true);

			for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
			{
				// Output file name to result log and console.
				logFile.logTestCaseInit(files[i]);
				cout << files[i] << endl;

				// Set up the input/output files.
				const XalanDOMString  theXSLFile= baseDir + xMan + FileUtility::s_pathSep + files[i];
				const XalanDOMString  theXMLFile = f.generateFileName(theXSLFile,"xml");
				const XalanDOMString  theOutput =  outputRoot + xMan + FileUtility::s_pathSep + files[i]; 
				const XalanDOMString  theOutputFile = f.generateFileName(theOutput, "out");

				XALAN_USING_XERCES(DOMDocument)
				XALAN_USING_XERCES(DOMImplementation)

				// Use a Xerces Dom document to create the XSLTResultTarget. 
				const XalanAutoPtr<DOMDocument>		theDocument(DOMImplementation::getImplementation()->createDocument());
				assert(theDocument.get() != 0);

				FormatterToXercesDOM	theFormatter(theDocument.get(), 0);

				XSLTResultTarget	domResultTarget(theFormatter);

				const XSLTInputSource	xslInputSource(theXSLFile);
				const XSLTInputSource	xmlInputSource(theXMLFile);
					
				const XalanCompiledStylesheet*	compiledSS = 0;

				int	theResult = transformEngine.compileStylesheet(
						xslInputSource,
						compiledSS);

				if (theResult != 0)
				{
					logFile.logTestCaseClose("Done","Fail");
					cerr << "XalanError: \n" << transformEngine.getLastError();
				}
				else
				{
					// Transform using compiled stylesheet.
					theResult =
							transformEngine.transform(xmlInputSource, compiledSS, domResultTarget);

					if(theResult != 0)
					{
						logFile.logTestCaseClose("Done","Fail");
						cerr << "XalanError: \n" << transformEngine.getLastError();
					}
					else
					{
						const XalanDOMString	mimeEncoding(XALAN_STATIC_UCODE_STRING("UTF-8"));
						const XalanDOMString	encoding(XALAN_STATIC_UCODE_STRING("UTF-8"));

						XalanFileOutputStream myOutput(theOutputFile);
						XalanOutputStreamPrintWriter myResultWriter(myOutput);
						FormatterListener* theFormatter = getXMLFormatter(true,true,true,
																		myResultWriter,0,
																		mimeEncoding,
																		compiledSS->getStylesheetRoot());

						XercesDOMFormatterWalker theTreeWalker(*theFormatter);
						theTreeWalker.traverse(theDocument.get());

						delete theFormatter;
						logFile.logTestCaseClose("Done","Pass");
					}	
				}
			}

			logFile.logTestFileClose("Dom2Dom Testing: ", "Done");
			logFile.close();
		}
		catch(...)
		{
			cerr << "Exception caught!!!" << endl << endl;
		}
	}

	return 0;

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
