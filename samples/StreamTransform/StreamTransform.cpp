// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <iostream>
#include <strstream>
#include <fstream>



#include <util/PlatformUtils.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



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
	using std::cout;
	using std::endl;
	using std::istrstream;
	using std::ofstream;
	using std::ostrstream;
#endif

	if (argc != 1)
	{
		cerr << "Usage: StreamTransform"
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

				// A simple input document...
				const char* const  theInputDocument = "<?xml version='1.0'?><doc>Hello world!</doc>";

				// A "hello world" stylesheet.  Note that the encoding for the output is US-ASCII,
				// since we're writing to a string.  It could be any encoding, but "binary" encodings,
				// or encodings that could produce multi-byte characters would require transcoding on
				// some platforms.
				const char* const  theStylesheet =
"<?xml version='1.0'?>\
<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' version='1.0'>\
<xsl:output encoding='US-ASCII'/>\
<xsl:template match='doc'>\
<out><xsl:value-of select='.'/></out>\
</xsl:template>\
</xsl:stylesheet>";

				// Our input streams...
				istrstream	theInputDocumentStream(theInputDocument, strlen(theInputDocument));
				istrstream	theStylesheetStream(theStylesheet, strlen(theStylesheet));

				// Our input sources...
				XSLTInputSource		theInputSource(&theInputDocumentStream);
				XSLTInputSource		theStylesheetSource(&theStylesheetStream);

				// Our output target.  By default, we'll use an ostrstream that will allocate
				// memory dynamically, although we could also use a static buffer since we 
				// know the size of the output.
#if defined(SIMPLE_STREAM_USE_STATIC_BUFFER)
				char				theData[100];

				ostrstream			theOutputStream(theData, sizeof(theData));
#else
				ostrstream			theOutputStream;
#endif

				XSLTResultTarget	theResultTarget(&theOutputStream);

				theProcessor.process(
							theInputSource,
							theStylesheetSource,
							theResultTarget,
							theConstructionContext,
							theExecutionContext);

				// OK, we'll now write the data from the string buffer to cout.

				// Null-terminate the data first, to make things easier...
				theOutputStream << '\0';

#if !defined(SIMPLE_STREAM_USE_STATIC_BUFFER)
				// Get a pointer to the string...
				const char* const	theData = theOutputStream.str();

				// Unfreeze the stream, so the memory is automatically
				// delete when the stream goes out of scope...
				theOutputStream.freeze(false);
#endif

				cout << theData;
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
