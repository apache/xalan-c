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
#include <cassert>
#include <fstream>
#include <iostream>
#include <strstream>

#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

//These came from the debug test.
#include <cstdio>
#include <ctime>
#include <string>
#include <string.h>
#include <vector>

#include <util/PlatformUtils.hpp>

#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanFileOutputStream.hpp>
#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>

#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>

#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>

#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/StylesheetRoot.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>

#include <XMLFileReporter.hpp>


//This is here for the threads.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
#define THREADFUNCTIONRETURN DWORD WINAPI

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


// This is here for memory leak testing.
#if defined(_DEBUG)
#include <crtdbg.h>
#endif


const char* const 	xslStylesheets[] =
{
	"v:\\xsl-test\\perf\\basic\\basic-all_well",
	"v:\\xsl-test\\perf\\basic\\basic-datetranscode",
	"v:\\xsl-test\\perf\\basic\\basic-dict2", 
	"v:\\xsl-test\\perf\\basic\\basic-Fischer-Euwe",
	"v:\\xsl-test\\perf\\basic\\basic-queens", 
	"v:\\xsl-test\\perf\\large\\large-all_well",
	//"v:\\xsl-test\\perf\\large\\large-evans_large", 
	"v:\\xsl-test\\perf\\nodes\\nodes-fancy_xml_tree_viewer_34",
	"v:\\xsl-test\\perf\\nodes\\nodes-showtree-19991008",
	"v:\\xsl-test\\perf\\sort\\sort-big",
	"v:\\xsl-test\\perf\\xpath\\xpath-evans_small",
	"v:\\xsl-test\\perf\\xpath\\xpath-evans_tiny",
	0
};



const char* const	outputDir = "\\xslt-results\\perf\\test\\";


inline StylesheetRoot*
processStylesheet(
			const XalanDOMString&			theFileName,
			XSLTProcessor&					theProcessor,
			StylesheetConstructionContext&	theConstructionContext)
{
	const XSLTInputSource	theInputSource(c_wstr(theFileName));

	return theProcessor.processStylesheet(theInputSource, theConstructionContext);
}



inline XalanNode*
parseSourceDocument(
			const XalanDOMString&	theFileName,
			XSLTProcessor&			theProcessor)
{
	const XSLTInputSource	theInputSource(c_wstr(theFileName));

	return theProcessor.getSourceTreeFromInput(theInputSource);
}



inline double
calculateElapsedTime(
			clock_t		theStartTime,
			clock_t		theEndTime)
{
	return double(theEndTime - theStartTime) / CLOCKS_PER_SEC * 1000.0;
}



inline double
calculateAverageElapsedTime(
			clock_t			theStartTime,
			clock_t			theEndTime,
			long			theIterationCount)
{
	assert(theIterationCount > 0);

	return calculateElapsedTime(theStartTime, theEndTime) / theIterationCount;
}

inline clock_t
transformWUnparsedSource(const XalanDOMString&	theFileName,
				 XSLTProcessor&			theProcessor,
				 const StylesheetRoot*	theStylesheetRoot,
				 XSLTResultTarget&	theResults,
				 StylesheetExecutionContextDefault&  theExecutionContext)
{
	const XSLTInputSource	csSourceXML(c_wstr(theFileName));
	theProcessor.setStylesheetRoot(theStylesheetRoot);

	const clock_t startTime = clock();
	theProcessor.process(csSourceXML, theResults, theExecutionContext);
	const clock_t endTime = clock();

	return endTime - startTime;

}

inline clock_t
transformWParsedSource(XalanNode*		theParsedSource,
				 XSLTProcessor&			theProcessor,
				 const StylesheetRoot*	theStylesheetRoot,
				 XSLTResultTarget&		theResults,
				 StylesheetExecutionContextDefault&  theExecutionContext)
{
	// Put the parsed document into an XSLTInputSource, 
	// and set stylesheet root in the processor
	const XSLTInputSource	csSourceDocument(theParsedSource);
	theProcessor.setStylesheetRoot(theStylesheetRoot);

	const clock_t startTime = clock();
	theProcessor.process(csSourceDocument, theResults, theExecutionContext);
	const clock_t endTime = clock();
	
	return endTime - startTime;

}

int
main(
	 int			argc,
	 const char*	argv[])
{
	XMLFileReporter	theXMLFileReporter;

	theXMLFileReporter.setFileName("cpp.xml");
	theXMLFileReporter.initialize();
	theXMLFileReporter.logTestFileInit("Performance Testing - Reports performance times for single transform, and average for multiple transforms using compiled stylesheet");
	
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	if (argc > 3)
	{
		cerr << "Usage: perf <count>" << endl  << endl;
	}
	else
	{
		long iterCount = 1;
		if (argc == 2)
		{
			iterCount = atol(argv[1]);
		}

		if (iterCount <= 0)
		{
			cerr << "Usage: perf <count>" << endl  << endl;
		}
		else
		{
			try
			{
				// Call the static initializers... and define file suffixes
				XMLPlatformUtils::Initialize();

				{
					XSLTInit	theInit;
					
					// Define some constants for file suffixes ...
					const XalanDOMString  XSLSuffix(".xsl");
					const XalanDOMString  XMLSuffix(".xml");
					const XalanDOMString  outputSuffix(".out");

					// Define some variables used for timing...
					clock_t startTime, endTime, accmTime;
                    double timeinMilliseconds, theAverage;

					for(long i = 0; xslStylesheets[i] != 0; i++)
					{
						// Output file name to result log.
						theXMLFileReporter.logTestCaseInit(xslStylesheets[i]);

						// Create the necessary support objects to instantiate a processor.
						XalanSourceTreeDOMSupport		csDOMSupport;
						XalanSourceTreeParserLiaison	csParserLiaison(csDOMSupport);

						csDOMSupport.setParserLiaison(&csParserLiaison);

						XSLTProcessorEnvSupportDefault	csXSLTProcessorEnvSupport;
						XObjectFactoryDefault			csXObjectFactory;
						XPathFactoryDefault				csXPathFactory;

						// Create a processor and connect to ProcessorEnvSupport object
						XSLTEngineImpl	csProcessor(
								csParserLiaison,
								csXSLTProcessorEnvSupport,
								csDOMSupport,
								csXObjectFactory,
								csXPathFactory);

						// Hook up the processor the the support object.
						csXSLTProcessorEnvSupport.setProcessor(&csProcessor);

						// Create separate factory support object, so the stylesheet's
						// factory-created XPath instance are independent from processor's.
						XPathFactoryDefault			ssXPathFactory;

						// Create a stylesheet construction context, using the
						// stylesheet's factory support objects.
						StylesheetConstructionContextDefault	csConstructionContext(
																csProcessor,
																csXSLTProcessorEnvSupport,
																ssXPathFactory);

						const XalanDOMString  theXSLFile(XalanDOMString(xslStylesheets[i]) + XSLSuffix);
						cout << endl << "Compiling: " << xslStylesheets[i] << endl;


						// Create a parsed stylesheet (StylesheetRoot) for the
						// specified input XSL. We don't have to delete it, since
						// it is owned by the StylesheetConstructionContextDefault
						// instance. Time it as well...

						startTime = clock();
						const StylesheetRoot* const glbStylesheetRoot =
									processStylesheet(
										theXSLFile,
										csProcessor,
										csConstructionContext);
						endTime = clock();
						assert(glbStylesheetRoot != 0);

						// Calculate & report performance on stylesheet parse to console and log file.
						timeinMilliseconds = calculateElapsedTime(startTime, endTime);
						cout << "Compilation of the stylesheet took " << timeinMilliseconds << " milliseconds." << endl;
						theXMLFileReporter.logStatistic(
								60,
								timeinMilliseconds,
								timeinMilliseconds,
								"Compilation of stylesheet took: ");


						// Parse the input XML and report how long it took...                             
						const XalanDOMString  theXMLFile(XalanDOMString(xslStylesheets[i]) + XMLSuffix);

						startTime = clock();
						XalanNode* const	glbSourceXML = parseSourceDocument(theXMLFile, csProcessor);
						endTime = clock();

						// Calculate & report performance on source document parse to console and log file.
						timeinMilliseconds = calculateElapsedTime(startTime, endTime);
						cout << "Compilation of the XML source took " << timeinMilliseconds << " milliseconds." << endl;
						theXMLFileReporter.logStatistic(
								60,
								timeinMilliseconds,
								timeinMilliseconds,
								"Compilation of XML source took: ");

						assert(glbSourceXML != 0);



						// The execution context uses the same factory support objects as
						// the processor, since those objects have the same lifetime as
						// other objects created as a result of the execution.
						StylesheetExecutionContextDefault	psExecutionContext(
									csProcessor,
									csXSLTProcessorEnvSupport,
									csDOMSupport,
									csXObjectFactory);

						const XalanDOMString theOutputFile(XalanDOMString(xslStylesheets[i]) + outputSuffix);

						// The result will go in the
						XSLTResultTarget	theResultTarget(theOutputFile);

						// Perform a single transform using compiled stylesheet and report results...
						csProcessor.setStylesheetRoot(glbStylesheetRoot);
						const XSLTInputSource	csSourceDocument(glbSourceXML);

						startTime = clock();
						csProcessor.process(csSourceDocument, theResultTarget, psExecutionContext);
						endTime = clock();

						psExecutionContext.reset();	// Reset the execution context...
						timeinMilliseconds = calculateElapsedTime(startTime, endTime);

						// Output single transform time to console and result log
						cout << "Single transform took " << timeinMilliseconds << " milliseconds." << endl;

						theXMLFileReporter.logStatistic(
								60,
								timeinMilliseconds,
								timeinMilliseconds,
								"Single transform took: ");

						// Perform multiple transforms and calculate the average time ..
						accmTime = 0;
						for(long j = 0; j < iterCount; ++j)
						{	
							accmTime += transformWParsedSource(glbSourceXML,
														 csProcessor,
														 glbStylesheetRoot,
														 theResultTarget,
														 psExecutionContext);
							psExecutionContext.reset();
							
						}
						csParserLiaison.reset();
						theAverage = calculateAverageElapsedTime(startTime, endTime, iterCount);

						// Output average transform time to console and result log
						cout << "Avg: " << theAverage << " for " << iterCount << " iterations w/Parsed XML" << endl;

						char tmp[100];
						sprintf(tmp, "%s%d%s","Average transform time based on ", iterCount, " iterations.");

						theXMLFileReporter.logStatistic(
								60, 
								theAverage, 
								theAverage, 
								tmp);

						//	This is currently how the XalanJ 2.0 is performing transforms,
						//	i.e. with the unparsed XML Source.
						
						accmTime = 0;
						for(long k = 0; k < iterCount; ++k)
						{
							accmTime += transformWUnparsedSource(theXMLFile,
														 csProcessor,
														 glbStylesheetRoot,
														 theResultTarget,
														 psExecutionContext);
							psExecutionContext.reset();
							csParserLiaison.reset();
						}
						theAverage = accmTime / iterCount;
						cout << "Avg: " << theAverage << " for " << iterCount << " iterations w/Source XML" << endl;


						theXMLFileReporter.logTestCaseClose(xslStylesheets[i], "Done");
					}
				}

				theXMLFileReporter.logTestFileClose("Performance", "Done");
				theXMLFileReporter.close();

				XMLPlatformUtils::Terminate();
			}
			catch(...)
			{
				cerr << "Exception caught!!!"
					 << endl
					 << endl;
			}
		}
	}


	return 0;
}
