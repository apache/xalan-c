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
#include <vector>

// This is here for memory leak testing. 
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

// XERCES HEADERS...
//	Are included by HarnessInit.hpp
#include <parsers/DOMParser.hpp>

// XALAN HEADERS...
//	Are included by FileUtility.hpp
#include <XalanTransformer/XercesDOMWrapperParsedSource.hpp>

// HARNESS HEADERS...
#include <XMLFileReporter.hpp>
#include <FileUtility.hpp>
#include <HarnessInit.hpp>


#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
#endif

FileUtility		futil;

void
printArgOptions()
{
	cerr << endl
		 << "inputsource dirname [-out]"
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
		  XalanDOMString& baseDir,
		  XalanDOMString& outDir,
		  XalanDOMString& goldRoot)
{
	bool fSuccess = true;	// Used to continue argument loop
	bool fSetOut = true;	// Set default output directory, set to false if data is provided
	bool fsetGold = true;	// Set default gold directory, set to false if data is provided

	// Insure that required "-base" argument is there.
	if (argc == 1 || argv[1][0] == '-')
	{
		printArgOptions(); 
		return false;
	}
	else
	{
		if (futil.checkDir(pathSep + XalanDOMString(argv[1])))
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
				futil.checkAndCreateDir(outDir);
				fSetOut = false;
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
		unsigned int ii = lastIndexOf(baseDir,charAt(pathSep,0));
		outDir = substring(baseDir, 0, ii+1);
		append(outDir,XalanDOMString("ISOURCE-results\\"));
		futil.checkAndCreateDir(outDir);
	}
	
	if (fsetGold)
	{
		goldRoot = baseDir;
		append(goldRoot,XalanDOMString("-gold"));
		futil.checkAndCreateDir(goldRoot);
		append(goldRoot,pathSep);
	}

	// Add the path seperator to the end of the base directory
	append(baseDir, pathSep);
	return fSuccess;
}


// TestCase1 will use the following method of XSLTInputSource
//		- XSLTInputSource(const XMLCh* systemId)
//		- XSLTInputSource(const XMLCh* systemId,
//						  const XMLCh* publicId)
void testCase1(XalanTransformer &xalan, XMLFileReporter& logFile, 
			   const XalanDOMString &xml, const XalanDOMString &xsl, 
			   const XalanDOMString outBase, XalanDOMString &theGoldFile)
{
	const XalanDOMString  publicID("public-smublic");

	const XalanDOMString	theOutputFile = outBase + XalanDOMString("\\TestCase1.out");
	const XSLTResultTarget	theResultTarget(theOutputFile);

	futil.data.testOrFile = "TestCase1";

	// This code excersized the stated methods of XSLTInputSource
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl), c_wstr(publicID));

	// Do the transform and report the results.
	xalan.transform(xmlInputSource, xslInputSource, theResultTarget);
	futil.checkResults(theOutputFile, theGoldFile, logFile);
		
}

// TestCase2 will use the following methods of XSLTInputSource
//		- XSLTInputSource(const char* systemId)
//		- XSLTInputSource(const char* systemId,
//						  const char* publicId)
void testCase2(XalanTransformer &xalan, XMLFileReporter& logFile, 
			   const XalanDOMString outBase, XalanDOMString &theGoldFile)

{
	const XalanDOMString	theOutputFile = outBase + XalanDOMString("\\TestCase2.out");
	const XSLTResultTarget	theResultTarget2(theOutputFile);

	futil.data.testOrFile = "TestCase2";

	// This code excersized the stated methods of XSLTInputSource
	const XSLTInputSource	xslStringSource("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xsl","public-smublic");
	const XSLTInputSource	xmlStringSource("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xml");


	// Do the transform and report the results.
	xalan.transform(xmlStringSource, xslStringSource, theResultTarget2);
	futil.checkResults(theOutputFile, theGoldFile, logFile);

}

// TestCase3 will use the following methods of XSLTInputSource
//		- XSLTInputSource()
//		- XSLTInputSource(XalanNode* node)
//		- XSLTInputSource::setNode(XalanNode* node)
//		- XSLTInputSource::getNode()
//		- NOTE:  We can't use the XalanTransformer Class with this test. So we create the processor directly.

void testCase3(XalanTransformer &xalan, XMLFileReporter &logFile, 
			   const XalanDOMString &outBase, const XalanDOMString &theGoldFile)

{
	XalanSourceTreeDocument::NodeType	type;		// actual values used for testing.
	XalanDOMString						name, value;	

	const XalanDOMString	theOutputFile = outBase + XalanDOMString("\\TestCase3.out");
	XSLTResultTarget		theResultTarget3(theOutputFile);

	futil.data.testOrFile = "TestCase3a";

	// Create necessary Infrastructure to perform transform without the XalanTransformer Class.
	// Can't use it because it doesn't take a DOM.
	XalanSourceTreeDOMSupport		domSupport;
	XalanSourceTreeParserLiaison	parserLiaison(domSupport);
	domSupport.setParserLiaison(&parserLiaison);

	XSLTProcessorEnvSupportDefault	envSupport;
	XObjectFactoryDefault			xObjectFactory;
	XPathFactoryDefault				xPathFactory;
	
	// Create a processor and connect to ProcessorEnvSupport object
	XSLTEngineImpl	theProcessor(parserLiaison,
							envSupport,
							domSupport,
							xObjectFactory,
							xPathFactory);

	// Hook up the processor the the support object.
	envSupport.setProcessor(&theProcessor);

	// Create separate factory support object, so the stylesheet's
	// factory-created XPath instance are independent from processor's.
	XPathFactoryDefault		ssXPathFactory;

	// Create a stylesheet construction context, using the
	// stylesheet's factory support objects.
	StylesheetConstructionContextDefault constContext(	theProcessor,
														envSupport,
														ssXPathFactory);

	StylesheetExecutionContextDefault	execuContext(
											theProcessor,
											envSupport,
											domSupport,
											xObjectFactory);

	// Create the XSL Input Source
	const XSLTInputSource	xslStringSource("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xsl");

	XalanDocument* domXSL = parserLiaison.createXalanSourceTreeDocument();
	domXSL = parserLiaison.parseXMLStream(xslStringSource);

	// Here is one of the tests. It will be verified when we do the transform.
	XSLTInputSource	xslXalanNode(domXSL);

	// Set the system ID, so relative URIs are resolved properly...
	xslXalanNode.setSystemId(xslStringSource.getSystemId());

	XalanNode	*theNode, *firstChild;

	// Here is another test 
	theNode = xslXalanNode.getNode();

	firstChild = theNode->getFirstChild();
	value = firstChild->getNodeValue();

	futil.checkAPIResults(value, XalanDOMString(" This is THE Smoke Test "), "XSLTInputSource.getNode()", logFile,
							theOutputFile, theGoldFile);

	// Create the XML Input Source
	const XSLTInputSource	xmlStringSource("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xml");

	XalanDocument* domXML = parserLiaison.createXalanSourceTreeDocument();
	domXML = parserLiaison.parseXMLStream(xmlStringSource);

	// Here is the final test. It too will be verified when we do the transform.
	XSLTInputSource	xmlXalanNode;
	xmlXalanNode.setNode(domXML);

	// Do the transform and report the results. 
	theProcessor.process(xmlXalanNode,		//inputSource, 
			        	xslXalanNode,		//stylesheetSource,
				    	theResultTarget3,	//outputTarget,
						constContext,
						execuContext);

	// Reset testcase name
	futil.data.testOrFile = "TestCase3b";
	futil.checkResults(theOutputFile, 
					  theGoldFile, 
					  logFile);


}

// TestCase4 will use the following API.  Default constructor of XSLTInputSource will take a string.
//		- XSLTInputSource(const char*)
//		- XSLTInputSource(const char*)
void testCase4(XalanTransformer &xalan, XMLFileReporter& logFile)
{

	const XalanDOMString theOutputFile("\\xml-xalan\\test\\tests\\ISOURCE-results\\smoke\\testCase4.out");
	const XalanDOMString theGoldFile("\\xml-xalan\\test\\tests\\capi-gold\\smoke\\smoke01.out");

	futil.data.testOrFile = "TestCase4";

	// This code excersized the stated methods of XSLTInputSource
	// Do the transform and report the results.
	xalan.transform("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xml", 
					"\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xsl",
					"\\xml-xalan\\test\\tests\\ISOURCE-results\\smoke\\TestCase4.out");

	futil.checkResults(theOutputFile, theGoldFile, logFile);
}

// TestCase5 uses XercesDOMWrapperParsedSource class to wrap a xerces generated dom. 
//
void testCase5(XalanTransformer &xalan, XMLFileReporter& logFile)
{

	const XalanDOMString theOutputFile("\\xml-xalan\\test\\tests\\ISOURCE-results\\smoke\\TestCase5.out");
	const XalanDOMString theGoldFile("\\xml-xalan\\test\\tests\\capi-gold\\smoke\\smoke01.out");

	futil.data.testOrFile = "TestCase5";
	const char* const theURI = "/xml-xalan/test/tests/capi/smoke/smoke01.xml";
		 
	DOMParser  theParser;
	theParser.setToCreateXMLDeclTypeNode(false);
	theParser.parse(theURI);
	const DOM_Document theDOM = theParser.getDocument();

	XercesDOMSupport	theDOMSupport;
	XercesParserLiaison theParserLiaison(theDOMSupport);

	// This is the new class...
	const XercesDOMWrapperParsedSource xmlDocWrapper(theDOM, 
													 theParserLiaison, 
													 theDOMSupport, 
													 XalanDOMString(theURI));

	const XSLTInputSource	xslInputSource("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xsl");
	const XSLTResultTarget	theResultTarget(theOutputFile);

	// Do the transform and report the results.
	xalan.transform(xmlDocWrapper, xslInputSource, theResultTarget);
	futil.checkResults(theOutputFile, theGoldFile, logFile);
}

int
main(int			argc,
		const char*	argv [])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	XalanDOMString  baseDir, outputRoot, goldRoot;	 	


	if (getParams(argc, argv, baseDir, outputRoot, goldRoot) == true)
	{

		// Generate Unique Run id. (Only used to name the result logfile.)
		const XalanDOMString UniqRunid = futil.generateUniqRunid();

		// Defined basic constants for file manipulation
		const XalanDOMString drive(futil.getDrive());
		futil.data.testBase = baseDir;
		
		const XalanDOMString  resultFilePrefix("isource");
		const XalanDOMString  resultsFile(drive + outputRoot + resultFilePrefix + UniqRunid + XMLSuffix);
		
		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("XSLTInputSource Testing: Give various types of allowable Inputs. ");

		try
		{
			// Call the static initializers...
			HarnessInit xmlPlatformUtils;
			XalanTransformer::initialize();
			XalanTransformer xalan;
			{
				XalanDOMString		fileName;
				
				// Set the test directory.
				const XalanDOMString  currentDir("smoke");
				logFile.logTestCaseInit(currentDir);

				// Create the output directory.
				const XalanDOMString  theOutputDir = outputRoot + currentDir;
				futil.checkAndCreateDir(theOutputDir);

				// Get the files found in the "smoke" directory
				const FileNameVectorType	files = futil.getTestFileNames(baseDir, currentDir, true);

				// This only processes a single file, smoke01.xsl
				for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
				{

					fileName = files[i];
					futil.data.testOrFile = fileName;

					// Set up the input/output files.
					const XalanDOMString  theXSLFile= baseDir + currentDir + pathSep + fileName;
					const XalanDOMString  theXMLFile = futil.generateFileName(theXSLFile,"xml");
					futil.data.xmlFileURL = theXMLFile;
					futil.data.xslFileURL = theXSLFile;

					// Set the gold file.
					XalanDOMString  theGoldFile = goldRoot +currentDir + pathSep + fileName;
					theGoldFile = futil.generateFileName(theGoldFile, "out");
					
					// Execute the test cases. 
					testCase1(xalan, logFile, theXMLFile, theXSLFile, theOutputDir, theGoldFile);
					testCase2(xalan, logFile, theOutputDir, theGoldFile);
					testCase3(xalan, logFile, theOutputDir, theGoldFile);
					testCase4(xalan, logFile);
					testCase5(xalan, logFile);
				}

				logFile.logTestCaseClose("Done", "Pass");	
			}

			futil.reportPassFail(logFile, UniqRunid);
			logFile.logTestFileClose("ISource Testing: ", "Done");
			logFile.close();

			futil.analyzeResults(xalan, resultsFile);
			XalanTransformer::terminate();
		}
		catch(...)
		{
			cerr << "Exception caught!!!" << endl << endl;
		}
	}

	return 0;

}
