// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <cassert>
#include <fstream>
#include <iostream>
#include <strstream>



#include <util/PlatformUtils.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <DOMSupport/DOMSupportDefault.hpp>



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
	using std::ifstream;
	using std::ostrstream;
#endif

	if (argc != 1)
	{
		cerr << "Usage: CompileStylesheet"
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

				// Create separate factory support objects so the stylesheet's
				// factory-created XPath instances are independent from the
				// processor's.
				XPathFactoryDefault				theStylesheetXPathFactory;

				// Create a stylesheet construction context, using the
				// stylesheet's factory support objects.
				StylesheetConstructionContextDefault	theConstructionContext(
							theProcessor,
							theXSLTProcessorEnvSupport,
							theStylesheetXPathFactory);

				// The execution context uses the same factory support objects as
				// the processor, since those objects have the same lifetime as
				// other objects created as a result of the execution.
				StylesheetExecutionContextDefault		theExecutionContext(
							theProcessor,
							theXSLTProcessorEnvSupport,
							theXPathSupport,
							theXObjectFactory);

				// Our input files.  The assumption is that the executable will be run
				// from same directory as the input files.
				const XalanDOMString		theXMLFileName("foo.xml");
				const XalanDOMString		theXSLFileName("foo.xsl");
				
				// Our stylesheet input source...
				XSLTInputSource			theStylesheetSource(c_wstr(theXSLFileName));

				// Ask the processor to create a StylesheetRoot for the specified
				// input XSL.  This is the compiled stylesheet.  We don't have to
				// delete it, since it is owned by the StylesheetConstructionContext
				// instance.
				StylesheetRoot* const	theStylesheetRoot =
					theProcessor.processStylesheet(
								theStylesheetSource,
								theConstructionContext);
				assert(theStylesheetRoot != 0);

				for (unsigned int i = 0; i < 10; i++)
				{
					theExecutionContext.setStylesheetRoot(theStylesheetRoot);

					// Buffers passed in to ostrstream.
					char		inBuffer[10];
					char		outBuffer[10];

					// Generate the input and output file names.
					ostrstream	theFormatterIn(inBuffer, sizeof(inBuffer));
					ostrstream	theFormatterOut(outBuffer, sizeof(outBuffer));
					
					theFormatterIn << "foo" << i + 1 << ".xml" << '\0';
					theFormatterOut << "foo" << i + 1 << ".out" << '\0';

					//Generate the XML input and output objects.
					XSLTInputSource		theInputSource(theFormatterIn.str());
					XSLTResultTarget	theResultTarget(theFormatterOut.str());

					// Do the tranformation...
					theProcessor.process(
						 theInputSource,
						 theResultTarget,
						 theExecutionContext);

					// Reset the processor and the execution context
					// so we can perform the next transformation.
					// Reset the parser liaison to clear out the
					// source document we just transformed.
					theProcessor.reset();
					theExecutionContext.reset();
					theParserLiaison.reset();
				}
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
