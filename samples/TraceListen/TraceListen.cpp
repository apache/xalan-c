#include <iostream>
#include <fstream>

#include <util/PlatformUtils.hpp>

#include <PlatformSupport/DOMStringHelper.hpp>

#include <DOMSupport/DOMSupportDefault.hpp>

#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathSupportDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>
#include <XPath/XPathProcessorImpl.hpp>

#include <XercesPlatformSupport/XercesDOMPrintWriter.hpp>
#include <XercesPlatformSupport/TextFileOutputStream.hpp>
#include <XercesPlatformSupport/XercesStdTextOutputStream.hpp>

#include <XercesParserLiaison/XercesParserLiaison.hpp>

#include <XSLT/XSLTEngineImpl.hpp>
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
	using std::auto_ptr;
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
		cerr << "Usage: TraceListen [+ 1 or more of following] -TT -TG -TS -TTC" << endl;
		return -1;
	}

	// Set the TraceListener flags...
	for (int i = 1;	i < argc;	i ++)
	{
		if(!stricmp("-TT", argv[i]))
		{
			traceTemplates = true;
		}
		else if(!stricmp("-TG", argv[i]))
		{
			traceGenerationEvent = true;
		}
		else if(!stricmp("-TS", argv[i]))
		{
			traceSelectionEvent = true;
		}
		else if(!stricmp("-TTC", argv[i]))
		{
			traceTemplateChildren = true;
		}
		else
		{
  			cerr << "Usage: TraceListen [+ 1 or more of following] -TT -TG -TS -TTC" << endl;
	  		return -1;
		}
	} 
 
	try
	{
		// Call the static initializers...
		XMLPlatformUtils::Initialize();
		XSLTEngineImpl::Initialize();	  

		// Create the support objects that are necessary for running the processor...
		DOMSupportDefault				theDOMSupport;
		XercesParserLiaison				theParserLiaison(theDOMSupport);
		XPathSupportDefault				theXPathSupport(theDOMSupport);
		XSLTProcessorEnvSupportDefault	theXSLTProcessorEnvSupport;
		XObjectFactoryDefault			theXObjectFactory;
		XPathFactoryDefault				theXPathFactory;

		// Create a processor...
		XSLTEngineImpl	theProcessor(
					theParserLiaison,
					theXPathSupport,
					theXSLTProcessorEnvSupport,
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
						theXPathSupport,
						theXObjectFactory);

	  // Our input files...The assumption is that the executable will be run
		// from same directory as the input files.
		const XalanDOMString		theXMLFileName("birds.xml");
		const XalanDOMString		theXSLFileName("birds.xsl");

		// Our input sources...
		XSLTInputSource		theInputSource(c_wstr(theXMLFileName));
		XSLTInputSource		theStylesheetSource(c_wstr(theXSLFileName));

		// Our output target...
		TextFileOutputStream	theOutputStream("birds.out");
		XercesDOMPrintWriter	theResultWriter(theOutputStream);
		XSLTResultTarget		theResultTarget(&theResultWriter);

		// Set up a diagnostic writer to be used by the TraceListener...
		XercesStdTextOutputStream				theStdErr(cerr);
		XercesDOMPrintWriter					diagnosticsWriter(theStdErr);

	  // Set up the TraceListener... 
		auto_ptr<TraceListener>		theTraceListener(
			new TraceListenerDefault(
				diagnosticsWriter,
				traceTemplates,
				traceTemplateChildren,
				traceGenerationEvent,
				traceSelectionEvent));

		// Add the TraceListener to the XSLT processor...
		theProcessor.setTraceSelects(traceSelectionEvent);
		theProcessor.addTraceListener(theTraceListener.get());

		// Perform the transformation...
		theProcessor.process(
						theInputSource,
						theStylesheetSource,
						theResultTarget,
						theConstructionContext,
						theExecutionContext);

		// Call the static terminators...
		XMLPlatformUtils::Terminate();
		XSLTEngineImpl::Terminate();
	}
	catch(...)
	{
		cerr << "Exception caught!  Exiting..." << endl;
	}

	return 0;
}
