// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <iostream>
#include <fstream>



#include <util/PlatformUtils.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <DOMSupport/DOMSupportDefault.hpp>



#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathSupportDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>



#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>



#include <XercesParserLiaison/XercesDOMSupport.hpp>
#include <XercesParserLiaison/XercesParserLiaison.hpp>



int
main(
			int				argc,
			const char*		/* argv */[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::endl;
	using std::ofstream;
#endif

	if (argc != 1)
	{
		cerr << "Usage: SimpleTransform"
			 << endl
			 << endl;
	}
	else
	{
		try
		{
			// Call the static initializer for Xerces...
			XMLPlatformUtils::Initialize();

			{
				// Initialize the Xalan XSLT subsystem...
				XSLTInit						theInit;

				// Create the support objects that are necessary for running the processor...
				XercesDOMSupport				theDOMSupport;
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
							theXPathSupport,
							theXObjectFactory);

				// Our input files...The assumption is that the executable will be run
				// from same directory as the input files.
				const XalanDOMString		theXMLFileName("foo.xml");
				const XalanDOMString		theXSLFileName("foo.xsl");

				// Our input sources...
				XSLTInputSource		theInputSource(c_wstr(theXMLFileName));
				XSLTInputSource		theStylesheetSource(c_wstr(theXSLFileName));

				// Our output target...
				const XalanDOMString	theOutputFileName("foo.out");
				XSLTResultTarget		theResultTarget(theOutputFileName);

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
			cerr << "Exception caught!!!"
				 << endl
				 << endl;
		}
	}

	return 0;
}
