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

FileUtility		h;

void
setHelp()
{
	h.args.help << endl
				<< "inputsource dirname [-out]"
				<< endl
				<< endl
				<< "dirname		(base directory for testcases)"
				<< endl
				<< "-out dirname	(base directory for output)"
				<< endl;
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

	const XalanDOMString	theOutputFile = outBase + XalanDOMString("\\InputSource-TestCase1.out");
	const XSLTResultTarget	theResultTarget(theOutputFile);

	h.data.testOrFile = "InputSource-TestCase1";

	// This code excersized the stated methods of XSLTInputSource
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl), c_wstr(publicID));

	// Do the transform and report the results.
	xalan.transform(xmlInputSource, xslInputSource, theResultTarget);
	h.checkResults(theOutputFile, theGoldFile, logFile);
		
}

// TestCase2 will use the following methods of XSLTInputSource
//		- XSLTInputSource(const char* systemId)
//		- XSLTInputSource(const char* systemId,
//						  const char* publicId)
void testCase2(XalanTransformer &xalan, XMLFileReporter& logFile, 
			   const XalanDOMString outBase, XalanDOMString &theGoldFile)

{
	const XalanDOMString	theOutputFile = outBase + XalanDOMString("\\InputSource-TestCase2.out");
	const XSLTResultTarget	theResultTarget2(theOutputFile);

	h.data.testOrFile = "InputSource-TestCase2";

	// This code excersized the stated methods of XSLTInputSource
	const XSLTInputSource	xslStringSource("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xsl","public-smublic");
	const XSLTInputSource	xmlStringSource("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xml");


	// Do the transform and report the results.
	xalan.transform(xmlStringSource, xslStringSource, theResultTarget2);
	h.checkResults(theOutputFile, theGoldFile, logFile);

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

	const XalanDOMString	theOutputFile = outBase + XalanDOMString("\\InputSource-TestCase3.out");
	XSLTResultTarget		theResultTarget3(theOutputFile);

	h.data.testOrFile = "InputSource-TestCase3a";

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

	h.checkAPIResults(value, XalanDOMString(" This is THE Smoke Test "), "XSLTInputSource.getNode()", logFile,
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
	h.data.testOrFile = "InputSource-TestCase3b";
	h.checkResults(theOutputFile, 
					  theGoldFile, 
					  logFile);
}

// TestCase4 will use the following API.  Default constructor of XSLTInputSource will take a string.
//		- XSLTInputSource(const char*)
//		- XSLTInputSource(const char*)
void testCase4(XalanTransformer &xalan, XMLFileReporter& logFile)
{

	const XalanDOMString theOutputFile("\\xml-xalan\\test\\tests\\ISOURCE-results\\smoke\\InputSource-TestCase4.out");
	const XalanDOMString theGoldFile("\\xml-xalan\\test\\tests\\capi-gold\\smoke\\smoke01.out");

	XSLTResultTarget theResultTarget(theOutputFile);

	h.data.testOrFile = "InputSource-TestCase4";

	// This code excersized the stated methods of XSLTInputSource
	// Do the transform and report the results.
	xalan.transform("\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xml", 
					"\\xml-xalan\\test\\tests\\capi\\smoke\\smoke01.xsl",
					theResultTarget);

	h.checkResults(theOutputFile, theGoldFile, logFile);
}

// TestCase5 uses XercesDOMWrapperParsedSource class to wrap a xerces generated dom. 
//
void testCase5(XalanTransformer &xalan, XMLFileReporter& logFile,
			   const XalanDOMString &xml, const XalanDOMString &xsl,
			   const XalanDOMString outBase, XalanDOMString &theGoldFile)
{
	h.data.testOrFile = "InputSource-TestCase5";
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
		 
	DOMParser  theParser;
	theParser.setToCreateXMLDeclTypeNode(false);
	theParser.parse(xmlInputSource);
	const DOM_Document theDOM = theParser.getDocument();

	XercesDOMSupport	theDOMSupport;
	XercesParserLiaison theParserLiaison(theDOMSupport);

	// This is the wrapper class for Xerces DOM. 
	const XercesDOMWrapperParsedSource* xmlDocWrapper = 0;
	xmlDocWrapper = new XercesDOMWrapperParsedSource(theDOM, 
													 theParserLiaison, 
													 theDOMSupport, 
													 xml);
/*	const XercesDOMWrapperParsedSource xmlDocWrapper(theDOM, 
													 theParserLiaison, 
													 theDOMSupport, 
													 xml);

*/
	const XalanDOMString theOutputFile(outBase + XalanDOMString("\\InputSource-TestCase5.out"));
	const XSLTInputSource	xslInputSource(c_wstr(xsl));
	const XSLTResultTarget	theResultTarget(theOutputFile);

	// Do the transform and report the results.
	xalan.transform(*xmlDocWrapper, xslInputSource, theResultTarget);
	h.checkResults(theOutputFile, theGoldFile, logFile);
	delete xmlDocWrapper;
}

int
main(int		  argc,
	 const char*  argv [])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	// Set the program help string,  then get the command line parameters.
	//
	setHelp();
	if (h.getParams(argc, argv, "INPUTSOURCE-RESULTS") == true)
	{
		// Generate and Initialize Unique result logfile, and get drive designation
		//
		const XalanDOMString UniqRunid = h.generateUniqRunid();
		const XalanDOMString drive(h.getDrive());
		const XalanDOMString  resultFilePrefix("isource");
		const XalanDOMString  resultsFile(drive + h.args.output + resultFilePrefix + UniqRunid + XMLSuffix);
		
		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("XSLTInputSource Testing: Give various types of allowable Inputs. ");

		try
		{
			// Call the static initializers...
			//
			HarnessInit xmlPlatformUtils;
			XalanTransformer::initialize();
			XalanTransformer xalan;
			{
				XalanDOMString		fileName;
				
				// Get testfiles from the capi\smoke directory, create output directory, .
				//
				const XalanDOMString  currentDir("smoke");
				const XalanDOMString  theOutputDir = h.args.output + currentDir;
				h.checkAndCreateDir(theOutputDir);

				// Get the single file found in the "smoke" directory, and run tests.
				//
				const FileNameVectorType	files = h.getTestFileNames(h.args.base, currentDir, true);
				logFile.logTestCaseInit(currentDir);
				for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
				{
					fileName = files[i];
					h.data.testOrFile = fileName;

					// Set up the input/output files.
					const XalanDOMString  theXSLFile= h.args.base + currentDir + pathSep + fileName;
					const XalanDOMString  theXMLFile = h.generateFileName(theXSLFile,"xml");
					h.data.xmlFileURL = theXMLFile;
					h.data.xslFileURL = theXSLFile;

					// Set the gold file.
					XalanDOMString  theGoldFile = h.args.gold + currentDir + pathSep + fileName;
					theGoldFile = h.generateFileName(theGoldFile, "out");
					
					// Execute the test cases. 
					//
					testCase1(xalan, logFile, theXMLFile, theXSLFile, theOutputDir, theGoldFile);
					testCase2(xalan, logFile, theOutputDir, theGoldFile);
					testCase3(xalan, logFile, theOutputDir, theGoldFile);
					testCase4(xalan, logFile);
					testCase5(xalan, logFile, theXMLFile, theXSLFile, theOutputDir, theGoldFile);
				}

				logFile.logTestCaseClose("Done", "Pass");	
			}

			h.reportPassFail(logFile, UniqRunid);
			logFile.logTestFileClose("ISource Testing: ", "Done");
			logFile.close();

			h.analyzeResults(xalan, resultsFile);
			XalanTransformer::terminate();
		}
		catch(...)
		{
			cerr << "Exception caught!!!" << endl << endl;
		}
	}

	return 0;

}
