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
	"v:\\xsl-test\\prod\\misc\\misc-chess",
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
StylesheetRoot*		glbStylesheetRoot[sizeof(xslStylesheets) / sizeof(const char*)];

XalanNode*			glbSourceDoc[sizeof(xslStylesheets) / sizeof(const char*)];



void
outputMessage(int iter)
{
		cout << "\n" << "Starting Iteration: " << iter << '\0';
}



int
main(
			int				argc,
			const char*		argv [])
{
	assert(sizeof(glbStylesheetRoot) == sizeof(glbSourceDoc));

#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	if (argc > 2)
	{
		cerr << "Usage: perf"
			 << endl
			 << endl;
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

				// Create the necessary stuff to compile the stylesheet.
				XercesDOMSupport			    csDOMSupport;
				XercesParserLiaison				csParserLiaison(csDOMSupport);
				XPathSupportDefault				csXPathSupport(csDOMSupport);
				XSLTProcessorEnvSupportDefault	csXSLTProcessorEnvSupport;
				XObjectFactoryDefault			csXObjectFactory;
				XPathFactoryDefault				csXPathFactory;

				// Create a processor to compile the stylesheet...
				XSLTEngineImpl	csProcessor(
						csParserLiaison,
						csXPathSupport,
						csXSLTProcessorEnvSupport,
						csDOMSupport,
						csXObjectFactory,
						csXPathFactory);

				// Connect the processor to the support object...
				csXSLTProcessorEnvSupport.setProcessor(&csProcessor);

				// Create separate factory support objects so the stylesheet's
				// factory-created XObject and XPath instances are independent 
				// from processor's.
				XObjectFactoryDefault	csStylesheetXObjectFactory;
				XPathFactoryDefault		csStylesheetXPathFactory;

				// Create a stylesheet construction context, using the
				// stylesheet's factory support objects.
				StylesheetConstructionContextDefault	csConstructionContext(
														csProcessor,
														csXSLTProcessorEnvSupport,
														csStylesheetXPathFactory);

				const XalanDOMString  theXSLSuffix(".xsl");
				const XalanDOMString  theXMLSuffix(".xml");
				const XalanDOMString  theoutputSuffix(".out");

				for(int i = 0; xslStylesheets[i] != 0; i++)
				{
					const XalanDOMString  theXSLFilename(XalanDOMString(xslStylesheets[i]) + theXSLSuffix);
					const XalanDOMString  theXMLFilename(XalanDOMString(xslStylesheets[i]) + theXMLSuffix);

					cout << "Now compiling Stylesheet: " << xslStylesheets[i] << endl;

					//Generate the XML and XSL input objects.
					XSLTInputSource		csStylesheetSourceXSL(c_wstr(theXSLFilename));
					XSLTInputSource		csDocumentSource(c_wstr(theXMLFilename));

					// Ask the processor to create a StylesheetRoot for the specified
					// input XSL.  This is the compiled stylesheet.  We don't have to
					// delete it, since it is owned by the StylesheetConstructionContext
					// instance.
					glbStylesheetRoot[i] = csProcessor.processStylesheet(csStylesheetSourceXSL,
														   csConstructionContext);
					assert(glbStylesheetRoot[i] != 0);

					// Have the processor create a compiled SourceDocument for the specified
					// input XML. 
					glbSourceDoc[i] = csProcessor.getSourceTreeFromInput(csDocumentSource);
					assert(glbSourceDoc[i] != 0);
				}


					for(int ii = 0; xslStylesheets[ii] != 0; ii++)
					{
						cout << endl << "Now running test: " << xslStylesheets[ii] << endl;

						// Create the necessary stuff to run the processor.
						XercesDOMSupport				psDOMSupport;
						XercesParserLiaison				psParserLiaison(psDOMSupport);
						XPathSupportDefault				psXPathSupport(psDOMSupport);
						XSLTProcessorEnvSupportDefault	psXSLTProcessorEnvSupport;
						XObjectFactoryDefault			psXObjectFactory;
						XPathFactoryDefault				psXPathFactory;

						// Create a processor to perform the transform.
						XSLTEngineImpl	psProcessor(
							psParserLiaison,
							psXPathSupport,
							psXSLTProcessorEnvSupport,
							psDOMSupport,
							psXObjectFactory,
							psXPathFactory);

						// Connect the processor to the support object...
						psXSLTProcessorEnvSupport.setProcessor(&psProcessor);

						// The execution context uses the same factory support objects as
						// the processor, since those objects have the same lifetime as
						// other objects created as a result of the execution.
						StylesheetExecutionContextDefault		psExecutionContext(
								psProcessor,
								psXSLTProcessorEnvSupport,
								psXPathSupport,
								psXObjectFactory);

						const XalanDOMString  outputFileName(XalanDOMString(xslStylesheets[ii]) + theoutputSuffix);

						//Generate the XML input and output objects.
						XSLTInputSource		csDocumentSource(glbSourceDoc[ii]);
						XSLTResultTarget	theResultTarget(outputFileName);

						// Set the stylesheet to be the compiled stylesheet. Then do the transform.
						const double startTime = clock();
						cout << "Clock before transforms: " << startTime << endl;
						for(int j = 0; j < iterCount; ++j)
						{	
							psProcessor.setStylesheetRoot(glbStylesheetRoot[ii]);
							psProcessor.process(csDocumentSource, theResultTarget,psExecutionContext);
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
