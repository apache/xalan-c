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
#include <Include/PlatformDefinitions.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#include <strstream.h>
#else
#include <iostream>
#include <strstream>
#endif

#include <cstdio>

#if defined(_MSC_VER)
#include <direct.h>
#endif



XALAN_USING_STD(cerr)
XALAN_USING_STD(endl)



// XERCES HEADERS...
#include <xercesc/util/PlatformUtils.hpp>



// XALAN HEADERS...
#include <XalanSourceTree/FormatterToSourceTree.hpp>
#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



#include <Harness/XMLFileReporter.hpp>
#include <Harness/FileUtility.hpp>
#include <Harness/HarnessInit.hpp>



// This is here for memory leak testing.
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



XALAN_USING_XALAN(FileUtility)



void
setHelp(FileUtility&	h)
{
	h.args.getHelpStream() << endl
		 << "compare dirname [-out -gold]"
		 << endl
		 << endl
		 << "dirname		(base directory for testcases)"
		 << endl
		 << "-out dirname	(base directory for output)"
		 << endl
		 << "-gold dirname	(base directory for gold files)"
		 << endl;
}



int
runTests(
			int				argc,
			const char*		argv[])
{
	XALAN_USING_XALAN(HarnessInit)

	HarnessInit		xmlPlatformUtils;

	FileUtility		h;

	bool setGold = true;

	// Set the program help string,  then get the command line parameters.
	//
	setHelp(h);

	if (h.getParams(argc, argv, "DOMCOM-RESULTS", setGold) == true)
	{
		XALAN_USING_XALAN(FormatterToSourceTree)
		XALAN_USING_XALAN(XalanCompiledStylesheet)
		XALAN_USING_XALAN(XalanDOMString)
		XALAN_USING_XALAN(XalanParsedSource)
		XALAN_USING_XALAN(XalanSourceTreeDocument)
		XALAN_USING_XALAN(XalanSourceTreeDOMSupport)
		XALAN_USING_XALAN(XalanSourceTreeParserLiaison)
		XALAN_USING_XALAN(XalanTransformer)
		XALAN_USING_XALAN(XMLFileReporter)
		XALAN_USING_XALAN(XSLTInputSource)
		XALAN_USING_XALAN(XSLTResultTarget)

		//
		// Call the static initializers for xerces and xalan, and create a transformer
		//
		XalanTransformer	xalan;

		XalanSourceTreeDOMSupport		domSupport;
		XalanSourceTreeParserLiaison	parserLiaison(domSupport);
		domSupport.setParserLiaison(&parserLiaison);


		// Generate Unique Run id and processor info
		const XalanDOMString UniqRunid = h.generateUniqRunid();
		const XalanDOMString  resultFilePrefix("cpp");
		const XalanDOMString  resultsFile(h.args.output + resultFilePrefix + UniqRunid + FileUtility::s_xmlSuffix);

		XMLFileReporter		logFile(resultsFile);
		logFile.logTestFileInit("Comparison Testing:");
					
		// Specify the "test" directory for both input and output.
		//
		const XalanDOMString  currentDir("domcomtests");
		const XalanDOMString  theOutputDir = h.args.output + currentDir;
		h.checkAndCreateDir(theOutputDir);

		// Get the files found in the test directory
		//
		logFile.logTestCaseInit(currentDir);

		typedef FileUtility::FileNameVectorType		FileNameVectorType;

		const FileNameVectorType	files = h.getTestFileNames(h.args.base, currentDir, true);

		for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
		{
			const XalanDOMString&	fileName = files[i];
			h.data.reset();
			h.data.testOrFile = fileName;

			const XalanDOMString  theXSLFile= h.args.base + currentDir + FileUtility::s_pathSep + fileName;
			const XalanDOMString  theXMLFile = h.generateFileName(theXSLFile,"xml");
			XalanDOMString  theGoldFile = h.args.gold + currentDir + FileUtility::s_pathSep + fileName;
			theGoldFile = h.generateFileName(theGoldFile, "out");

			const XalanDOMString  outbase =  h.args.output + currentDir + FileUtility::s_pathSep + fileName; 
			const XalanDOMString  theOutputFile = h.generateFileName(outbase, "out");

			const XSLTInputSource	xslInputSource(theXSLFile);
			const XSLTInputSource	xmlInputSource(theXMLFile);
			const XSLTInputSource	goldInputSource(theGoldFile);

			// Use a XalanSourceTreeDocument to create the XSLTResultTarget. 
			//
			XalanSourceTreeDocument* dom = parserLiaison.createXalanSourceTreeDocument();
			FormatterToSourceTree	domOut(dom); 
			XSLTResultTarget		domResultTarget;
			domResultTarget.setDocumentHandler(&domOut);

			// Parsing(compile) the XSL stylesheet and report the results..
			//
			const XalanCompiledStylesheet*	compiledSS = 0;
			xalan.compileStylesheet(xslInputSource, compiledSS);
			if (compiledSS == 0 )
			{  
				continue;
			}

			// Parsing the input XML and report the results..
			//
			const XalanParsedSource*  parsedSource = 0;
			xalan.parseSource(xmlInputSource, parsedSource);
			if (parsedSource == 0)
			{
				continue;
			}

			// Perform One transform using parsed stylesheet and unparsed xml source, report results...
			// 
			xalan.transform(*parsedSource, compiledSS, domResultTarget);
			h.checkDOMResults(theOutputFile, compiledSS, dom, goldInputSource, logFile);

			parserLiaison.reset();
			xalan.destroyParsedSource(parsedSource);
			xalan.destroyStylesheet(compiledSS);
		}

		logFile.logTestCaseClose("Done", "Pass");
		h.reportPassFail(logFile, UniqRunid);
		logFile.logTestFileClose("DomCom ", "Done");
		logFile.close();

		h.analyzeResults(xalan, resultsFile);
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
		XALAN_USING_XERCES(XMLPlatformUtils)

		XALAN_USING_XALAN(XalanTransformer)

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
