// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <fstream.h>
#include <iostream.h>
#else
#include <fstream>
#include <iostream>
#endif



#include <util/PlatformUtils.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <PlatformSupport/XalanStdOutputStream.hpp>



#include <DOMSupport/DOMSupportDefault.hpp>



#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>
#include <XPath/XPathProcessorImpl.hpp>



#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTResultTarget.hpp>
#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/TraceListenerDefault.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>



int
main(
			int				argc,
			const char*		argv[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::endl;
#endif

	// TraceListener flags...
	bool traceTemplates = false;
	bool traceTemplateChildren = false;
	bool traceGenerationEvent = false;
	bool traceSelectionEvent = false;

	if (argc < 2 || argc > 5)
	{
		cerr << "Usage: TraceListen [+ 1 or more of following] -tt -tg -ts -ttc" << endl;

		return -1;
	}

	// Set the TraceListener flags...
	for (int i = 1;	i < argc;	i ++)
	{
		if(!strcmp("-tt", argv[i]))
		{
			traceTemplates = true;
		}
		else if(!strcmp("-tg", argv[i]))
		{
			traceGenerationEvent = true;
		}
		else if(!strcmp("-ts", argv[i]))
		{
			traceSelectionEvent = true;
		}
		else if(!strcmp("-ttc", argv[i]))
		{
			traceTemplateChildren = true;
		}
		else
		{
  			cerr << "Usage: TraceListen [+ 1 or more of following] -tt -tg -ts -ttc" << endl;

			return -1;
		}
	} 
 
	try
	{
		// Call the static initializers...
		XMLPlatformUtils::Initialize();

		{
			// Initialize the Xalan XSLT subsystem...
			XSLTInit						theInit;

			// Create some support objects that are necessary for running the processor...
			XalanSourceTreeDOMSupport		theDOMSupport;
			XalanSourceTreeParserLiaison	theParserLiaison(theDOMSupport);

			// Hook the two together...
			theDOMSupport.setParserLiaison(&theParserLiaison);

			// Create some more support objects.
			XSLTProcessorEnvSupportDefault	theXSLTProcessorEnvSupport;
			XObjectFactoryDefault			theXObjectFactory;
			XPathFactoryDefault				theXPathFactory;

			// Create a processor...
			XSLTEngineImpl	theProcessor(
						theParserLiaison,
						theXSLTProcessorEnvSupport,
						theDOMSupport,
						theXObjectFactory,
						theXPathFactory);

			// Connect the processor to the support object...
			theXSLTProcessorEnvSupport.setProcessor(&theProcessor);

			// Create a stylesheet construction context, and a stylesheet
			// execution context...
			StylesheetConstructionContextDefault	theConstructionContext(
							theProcessor,
							theXSLTProcessorEnvSupport,
							theXPathFactory);

			StylesheetExecutionContextDefault		theExecutionContext(
							theProcessor,
							theXSLTProcessorEnvSupport,
							theDOMSupport,
							theXObjectFactory);

			// Our input files...The assumption is that the executable will be run
			// from same directory as the input files.
			const XalanDOMString	theXMLFileName("birds.xml");
			const XalanDOMString	theXSLFileName("birds.xsl");

			// Our input sources...
			const XSLTInputSource	theInputSource(c_wstr(theXMLFileName));
			const XSLTInputSource	theStylesheetSource(c_wstr(theXSLFileName));

			// Our output target...
			const XalanDOMString	theOutputFile("birds.out");
			XSLTResultTarget		theResultTarget(theOutputFile);

			// Set up a diagnostic writer to be used by the TraceListener...
			XalanStdOutputStream			theStdErr(cerr);
			XalanOutputStreamPrintWriter	diagnosticsWriter(theStdErr);

			// Make sure that error reporting, which includes any TraceListener output
			// does not throw exceptions when transcoding, since that could result in
			// an exception being thrown while another exception is active.  In particular,
			// characters that the TraceListener writes might not be representable in the
			// local code page.
			theStdErr.setThrowTranscodeException(false);

			// Set up the TraceListener... 
			TraceListenerDefault		theTraceListener(				
					diagnosticsWriter,
					traceTemplates,
					traceTemplateChildren,
					traceGenerationEvent,
					traceSelectionEvent);

			// Add the TraceListener to the XSLT processor...
			theProcessor.setTraceSelects(traceSelectionEvent);
			theProcessor.addTraceListener(&theTraceListener);

			// Perform the transformation...
			theProcessor.process(
							theInputSource,
							theStylesheetSource,
							theResultTarget,
							theConstructionContext,
							theExecutionContext);

		}

		// Call the static terminator for Xerces...
		XMLPlatformUtils::Terminate();
	}
	catch(...)
	{
		cerr << "Exception caught!  Exiting..." << endl;
	}

	return 0;
}
