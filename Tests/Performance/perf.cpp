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
#include <vector>

#include <util/PlatformUtils.hpp>

#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanFileOutputStream.hpp>
#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <XercesParserLiaison/XercesDOMSupport.hpp>

#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathSupportDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>

#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/StylesheetRoot.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>

#include <XercesParserLiaison/XercesParserLiaison.hpp>


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


const char* const	xslStylesheets[] =
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



// Used to hold compiled stylesheet, and source document.
XalanNode*		glbSourceXML[sizeof(xslStylesheets) / sizeof(const char*)];
StylesheetRoot*	glbStylesheetRoot[sizeof(xslStylesheets) / sizeof(const char*)];

void outputMessage(int iter)
{
	cout << "\n" << "Starting Iteration: " << iter << '\0';
}

int main( int argc,	const char* argv [])
{
	assert(sizeof(glbStylesheetRoot) == sizeof(glbSourceXML));

#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	if (argc > 2)
	{
		cerr << "Usage: perf" << endl  << endl;
	}
	else
	{
		int	iterCount = 1;
		if (argc == 2)
		{
			iterCount = atoi(argv[1]);
		}

		try
		{
			// Call the static initializers...
			XMLPlatformUtils::Initialize();

			{
				XSLTInit	theInit;

				const XalanDOMString  XSLSuffix(".xsl");
				const XalanDOMString  XMLSuffix(".xml");
				const XalanDOMString  outputSuffix(".out");

				// Create the necessary support objects to instantiate a processor.
				XercesDOMSupport			    csDOMSupport;
				XercesParserLiaison				csParserLiaison(csDOMSupport);
				XPathSupportDefault				csXPathSupport(csDOMSupport);
				XSLTProcessorEnvSupportDefault	csXSLTProcessorEnvSupport;
				XObjectFactoryDefault			csXObjectFactory;
				XPathFactoryDefault				csXPathFactory;

				// Create a processor for stylesheet compilation and connect to 
				// ProcessorEnvSupport object
				XSLTEngineImpl	csProcessor(
						csParserLiaison,
						csXPathSupport,
						csXSLTProcessorEnvSupport,
						csDOMSupport,
						csXObjectFactory,   
						csXPathFactory);
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

				for(int i = 0; xslStylesheets[i] != 0; i++)
				{
					const XalanDOMString  theXSLfile(XalanDOMString(xslStylesheets[i]) + XSLSuffix);
					const XalanDOMString  theXMLfile(XalanDOMString(xslStylesheets[i]) + XMLSuffix);

					cout << "Now compiling Stylesheet: " << xslStylesheets[i] << endl;

					//Generate the XML and XSL input objects.
					XSLTInputSource		csStylesheetSourceXSL(c_wstr(theXSLfile));
					XSLTInputSource		csSourceXML(c_wstr(theXMLfile));

					// Ask the processor to create a compiled stylesheet (StylesheetRoot) for the 
					// specified input XSL. We don't have to delete it, since it is owned by the 
					// StylesheetConstructionContext instance.
					glbStylesheetRoot[i] = csProcessor.processStylesheet(csStylesheetSourceXSL,
														   csConstructionContext);
					assert(glbStylesheetRoot[i] != 0);

					// Have the processor create a compiled SourceDocument for the specified
					// input XML. 
					glbSourceXML[i] = csProcessor.getSourceTreeFromInput(csSourceXML);
					assert(glbSourceXML[i] != 0);
				}

				for(int ii = 0; xslStylesheets[ii] != 0; ii++)
				{
					cout << endl << "Now running test: " << xslStylesheets[ii] << endl;

					// The execution context uses the same factory support objects as
					// the processor, since those objects have the same lifetime as
					// other objects created as a result of the execution.
					StylesheetExecutionContextDefault	psExecutionContext(
							csProcessor,
							csXSLTProcessorEnvSupport,
							csXPathSupport,
							csXObjectFactory);

					const XalanDOMString  outputfile(XalanDOMString(xslStylesheets[ii]) + outputSuffix);

					//Generate the XML input and output objects.
					XSLTInputSource		csSourceXML(glbSourceXML[ii]);
					XSLTResultTarget	theResultTarget(outputfile);

					// Set the stylesheet to be the compiled stylesheet. Then do the transform.
					const double startTime = clock();
					cout << "Clock before transforms: " << startTime << endl;
					for(int j = 0; j < iterCount; ++j)
					{	
						csProcessor.setStylesheetRoot(glbStylesheetRoot[ii]);
						csProcessor.process(csSourceXML, theResultTarget,psExecutionContext);
						psExecutionContext.reset();
					}
					const double endTime = clock();
					cout << "Clock after transforms: " << endTime << endl;
					cout << "Total clock ticks: " << endTime - startTime << endl;
					const double	millis = ((endTime - startTime) / CLOCKS_PER_SEC) * 1000.0;
					cout << "Milliseconds: " << millis << endl;
					cout << "Averaged: " << millis / iterCount << " per iteration" << endl;
				}
				
			}

			XMLPlatformUtils::Terminate();
		}
		catch(...)
		{
			cerr << "Exception caught!!!"
				 << endl
				 << endl;
		}
	}

	return 0;
}
