// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#include <strstream.h>
#include <fstream.h>
#else
#include <iostream>
#include <strstream>
#include <fstream>
#endif



#include <util/PlatformUtils.hpp>



#include <Include/XalanAutoPtr.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



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

				// Create some support objects that are necessary for running the processor...
				XalanSourceTreeDOMSupport		theDOMSupport;
				XalanSourceTreeParserLiaison	theParserLiaison(theDOMSupport);

				// Hook the two together...
				theDOMSupport.setParserLiaison(&theParserLiaison);

				// Create some more support objects...
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

				// A simple input document...
#if defined(XALAN_NON_ASCII_PLATFORM)
				const char* const  theInputDocument = "<?xml version='1.0' encoding='EBCDIC-CP-US' ?><doc>Hello world!</doc>";
#else
				const char* const  theInputDocument = "<?xml version='1.0' encoding='ISO-8859-1' ?><doc>Hello world!</doc>";
#endif

				// A "hello world" stylesheet.  Note that the encoding for the output is platform-dependent,
				// since we're writing to a string.  It could be any encoding, but "binary" encodings,
				// or encodings that could produce multi-byte characters would require transcoding on
				// some platforms.
				const char* const  theStylesheet =
#if defined(XALAN_NON_ASCII_PLATFORM)
"<?xml version='1.0' encoding='EBCDIC-CP-US'?>\
<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' version='1.0'>\
<xsl:output encoding='EBCDIC-CP-US'/>\
<xsl:template match='doc'>\
<out><xsl:value-of select='.'/></out>\
</xsl:template>\
</xsl:stylesheet>";
#else
"<?xml version='1.0'?>\
<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' version='1.0'>\
<xsl:output encoding='US-ASCII'/>\
<xsl:template match='doc'>\
<out><xsl:value-of select='.'/></out>\
</xsl:template>\
</xsl:stylesheet>";
#endif

				// Our input streams...
				istrstream	theInputDocumentStream(theInputDocument, strlen(theInputDocument));
				istrstream	theStylesheetStream(theStylesheet, strlen(theStylesheet));

				// Our input sources...
				XSLTInputSource		theInputSource(&theInputDocumentStream);
				XSLTInputSource		theStylesheetSource(&theStylesheetStream);

				// A fake system ID for the input sources...
				const XalanDOMString	theSystemID("Memory buffer");

				// Set the system IDs...
				theInputSource.setSystemId(c_wstr(theSystemID));
				theStylesheetSource.setSystemId(c_wstr(theSystemID));

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
				// Get a pointer to the string, and store it in a XalanArrayAutoPtr,
				// since it will need to be deleted.
				const XalanArrayAutoPtr<char>	theGuard(theOutputStream.str());

				const char* const	theData = theGuard.get();
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
