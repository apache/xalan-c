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



#include <DOMSupport/DOMSupportDefault.hpp>



#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>



#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>



#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



int
main(
		  int				     argc,
		  const char*		 argv [])
{
#if !defined(XALAN_NO_NAMESPACES)
  using std::cerr;
  using std::endl;
  using std::ofstream;
#endif

	if (argc != 3)
	{
		cerr << "Usage: UseStylesheetParam key expression" << endl;

		return -1;
	}

	try
	{
		// Call the static initializer for Xerces...
		XMLPlatformUtils::Initialize();

		{
			// Initialize the Xalan XSLT subsystem...
			XSLTInit			theInit;

			// Get the stylesheet parameter key (name) and
			// expression (a string expression).
			const XalanDOMString	paramKey(argv[1]);
			const XalanDOMString	paramExpression(argv[2]);

			// Set up input and output objects for the transformation.
			// Assumption: the executable is run from the directory
			// containing the input files. 
			const XalanDOMString	theXMLFileName("foo.xml");
			const XalanDOMString	theXSLFileName("foo.xsl");

			const XSLTInputSource	theInputSource(c_wstr(theXMLFileName));
			const XSLTInputSource	theStylesheetSource(c_wstr(theXSLFileName));
 
			// The output target...
			const XalanDOMString	theOutputFile("foo.out");
			XSLTResultTarget		theResultTarget(theOutputFile);

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

			// Set the stylesheet parameter.
			theProcessor.setStylesheetParam(paramKey,  paramExpression);
 
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
		cerr << "UseStylesheetParam Exception caught!!!"
			 << endl
			 << endl;
	}

	return 0;
}
