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

// XALAN HEADERS...
//	Are included by FileUtility.hpp

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

// This routine will output the results.
//
void reportResult(const char* name, int &results, XMLFileReporter &logFile, XalanTransformer &xalan)
{
	if(results != 0)
	{
		logFile.logTestCaseClose("Done","Fail");
		cerr << "XalanError: \n" << xalan.getLastError();
	}
	else
	{
		cerr << "Pass " << name << endl;
	}
}

// TestCase1 will use the following method of XSLTInputSource
//		- XSLTInputSource(const XMLCh* systemId)
//		- XSLTInputSource(const XMLCh* systemId,
//						  const XMLCh* publicId)
void testCase1(XalanTransformer &xalan, const XalanDOMString &xml, 
			   const XalanDOMString &xsl, const XalanDOMString outBase, 
			   XalanDOMString &goldInputFile, XalanDOMString &fileName)
{
	const XalanDOMString  publicID("public-smublic");

	const XalanDOMString	theOutputFile = futil.GenerateFileName(outBase, "out1");
	const XSLTResultTarget	theResultTarget(theOutputFile);

	// This code excersized the stated methods of XSLTInputSource
	const XSLTInputSource	xmlInputSource(c_wstr(xml));
	const XSLTInputSource	xslInputSource(c_wstr(xsl), c_wstr(publicID));

	// Do the transform and report the results.
	int	theResult = xalan.transform(xmlInputSource, xslInputSource, theResultTarget);
	if (!theResult)
	{
		const XSLTInputSource resultInputSource(c_wstr(theOutputFile));
		const XSLTInputSource goldInputSource(c_wstr(goldInputFile));
		futil.compareSerializedResults(resultInputSource, goldInputSource, fileName, "TestCase1"); 
	}

}

// TestCase2 will use the following methods of XSLTInputSource
//		- XSLTInputSource(const char* systemId)
//		- XSLTInputSource(const char* systemId,
//						  const char* publicId)
void testCase2(XalanTransformer &xalan, const XalanDOMString outBase, 
			   XalanDOMString &goldInputFile, XalanDOMString &fileName)

{
	const XalanDOMString	theOutputFile = futil.GenerateFileName(outBase, "one2");
	const XSLTResultTarget	theResultTarget2(theOutputFile);


	// This code excersized the stated methods of XSLTInputSource
	const XSLTInputSource	xslStringSource("\\xslt\\xsl-test\\conf\\capi\\capi01.xsl","public-smublic");
	const XSLTInputSource	xmlStringSource("\\xslt\\xsl-test\\conf\\capi\\capi01.xml");


	// Do the transform and report the results.
	int theResult = xalan.transform(xmlStringSource, xslStringSource, theResultTarget2);
	if (!theResult)
	{
		const XSLTInputSource resultInputSource(c_wstr(theOutputFile));
		const XSLTInputSource goldInputSource(c_wstr(goldInputFile));
		futil.compareSerializedResults(resultInputSource, goldInputSource, fileName, "TestCase2"); 
	}

}

// TestCase3 will use the following methods of XSLTInputSource
//		- XSLTInputSource()
//		- XSLTInputSource(XalanNode* node)
//		- XSLTInputSource::setNode(XalanNode* node)
//		- XSLTInputSource::getNode()
//		- NOTE:  We can't use the XalanTransformer Class with this test. So we create the processor directly.
void testCase3(XalanTransformer &xalan, XMLFileReporter &logFile,
			   const XalanDOMString &xml, const XalanDOMString &xsl, const XalanDOMString &out)
{

	const XalanDOMString	theOutputFile = futil.GenerateFileName(out, "out3");
	XSLTResultTarget		theResultTarget3(theOutputFile);

	// Create necessary Infrastructure to perform transform without the XalanTransformer Class.
	// Can't use it because ....
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
	const XSLTInputSource	xslStringSource("\\xslt\\xsl-test\\conf\\capi\\capi01.xsl","public-smublic");
	XalanDocument* domXSL = parserLiaison.createXalanSourceTreeDocument();
	domXSL = parserLiaison.parseXMLStream(xslStringSource);

	const XSLTInputSource	xslXalanNode(domXSL);
	XalanNode*				theNode;
	theNode = xslXalanNode.getNode();



	// Create the XML Input Source
	const XSLTInputSource	xmlStringSource("\\xslt\\xsl-test\\conf\\capi\\capi01.xml");
	XalanDocument* domXML = parserLiaison.createXalanSourceTreeDocument();
	domXML = parserLiaison.parseXMLStream(xmlStringSource);

	XSLTInputSource	xmlXalanNode;
	xmlXalanNode.setNode(domXML);
	
	

	// Do the transform and report the results. 
	// NOTE: Until a dom compairator is written I'm checking the results manually.
	//
	int theResult = 0;
	theProcessor.process(xmlXalanNode,		//inputSource, 
			        	xslXalanNode,		//stylesheetSource,
				    	theResultTarget3,	//outputTarget,
						constContext,
						execuContext);


	//int theResult = xalan.transform(xmlXalanNode, xslXalanNode, theResultTarget3);
	reportResult("testCase3", theResult, logFile, xalan);

}

int
main(
		  int			argc,
		  const char*	argv [])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

//	FileUtility		futil;

	XalanDOMString  category;	// Test all of base dir by default
	XalanDOMString  baseDir, outputRoot, goldRoot;	 	


	if (getParams(argc, argv, baseDir, outputRoot, goldRoot) == true)
	{

		// Generate Unique Run id. (Only used to name the result logfile.)
		const XalanDOMString UniqRunid = futil.GenerateUniqRunid();

		// Defined basic constants for file manipulation 

		const XalanDOMString  resultFilePrefix("isource");
		const XalanDOMString  resultsFile(outputRoot + resultFilePrefix + UniqRunid + XMLSuffix);
		
		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("XSLTInputSource Testing: Give various types of allowable Inputs. ");

		try
		{
			// Call the static initializers...
			HarnessInit xmlPlatformUtils;
			XalanTransformer::initialize();

			XalanSourceTreeDOMSupport domSupport;
			XalanSourceTreeParserLiaison parserLiaison(domSupport);
			domSupport.setParserLiaison(&parserLiaison);

			{
				XalanTransformer		transformEngine;
					
				// Check that output directory is there.
				XalanDOMString		  fileName;
				const XalanDOMString  xDir("smoke");
				const XalanDOMString  theOutputDir = outputRoot + xDir;
				futil.checkAndCreateDir(theOutputDir);

				// Get the files found in the "simpletests" directory
				const FileNameVectorType	files = futil.getTestFileNames(baseDir, xDir, true);

				for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
				{

					// Output file name to result log and console.
					//logFile.logTestCaseInit(files[i]);
					fileName = files[i];
					cout << fileName << endl;

					// Set up the input/output files.
					const XalanDOMString  theXSLFile= baseDir + xDir + pathSep + fileName;
					const XalanDOMString  theXMLFile = futil.GenerateFileName(theXSLFile,"xml");
					const XalanDOMString  theOutput =  outputRoot + xDir + pathSep + fileName; 
					XalanDOMString  theGoldFile = goldRoot +xDir + pathSep + fileName;
					theGoldFile = futil.GenerateFileName(theGoldFile, "out");


					// Use a XalanSourceTreeDocument to create the XSLTResultTarget. 
					XalanSourceTreeDocument* dom = parserLiaison.createXalanSourceTreeDocument();
					FormatterToSourceTree domOut(dom); 
					XSLTResultTarget domResultTarget;
					domResultTarget.setDocumentHandler(&domOut);

					testCase1(transformEngine, theXMLFile, theXSLFile, theOutput, theGoldFile, fileName);

					testCase2(transformEngine, theOutput, theGoldFile, fileName);

					//testCase3(transformEngine, logFile, theXMLFile, theXSLFile, theOutput);
				}
				
			}

			XalanTransformer::terminate();

			logFile.logTestFileClose("ISource Testing: ", "Done");
			logFile.close();

		}
		catch(...)
		{
			cerr << "Exception caught!!!" << endl << endl;
		}
	}

	return 0;

}
