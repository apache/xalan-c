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
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>



#include <XercesParserLiaison/XercesParserLiaison.hpp>



#include <XercesPlatformSupport/TextFileOutputStream.hpp>
#include <XercesPlatformSupport/XercesDOMPrintWriter.hpp>



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
			const XalanDOMString		theXMLFileName("foo.xml");
			const XalanDOMString		theXSLFileName("foo.xsl");

			// Our input sources...
			XSLTInputSource		theInputSource(c_wstr(theXMLFileName));
			XSLTInputSource		theStylesheetSource(c_wstr(theXSLFileName));

			// Our output target...
			TextFileOutputStream	theOutputStream("foo.out");
			XercesDOMPrintWriter	theResultWriter(theOutputStream);
			XSLTResultTarget		theResultTarget(&theResultWriter);

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
			cerr << "Exception caught!!!"
				 << endl
				 << endl;
		}
	}

	return 0;
}
