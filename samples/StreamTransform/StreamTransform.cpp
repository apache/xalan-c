// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#include <strstream.h>
#include <fstream.h>
#else
#include <iostream>
#include <strstream>
#include <fstream>
#endif



#include <xercesc/util/PlatformUtils.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



int
main(
			int				argc,
			const char*		/* argv */[])
{
	XALAN_USING_STD(cerr)
	XALAN_USING_STD(cout)
	XALAN_USING_STD(endl)
	XALAN_USING_STD(istrstream)
	XALAN_USING_STD(ofstream)
	XALAN_USING_STD(ostrstream)

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::strlen;
#endif

	int	theResult = -1;

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
			XALAN_USING_XERCES(XMLPlatformUtils)

			XALAN_USING_XALAN(XalanTransformer)


			// Call the static initializer for Xerces.
			XMLPlatformUtils::Initialize();

			// Initialize Xalan.
			XalanTransformer::initialize();

			{
				// Create a XalanTransformer.
				XalanTransformer theXalanTransformer;

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
				istrstream	theXMLStream(theInputDocument, strlen(theInputDocument));
				istrstream	theXSLStream(theStylesheet, strlen(theStylesheet));

				XALAN_USING_XALAN(XalanDOMString)
				XALAN_USING_XALAN(XSLTInputSource)

				XSLTInputSource		inputSource(&theXSLStream);

				inputSource.setSystemId(XalanDOMString("foo").c_str());

				// Do the transform.
				theResult = theXalanTransformer.transform(&theXMLStream, inputSource, cout);

				if(theResult != 0)
				{
					cerr << "StreamTransform Error: \n" << theXalanTransformer.getLastError()
						 << endl
						 << endl;
				}
			}

			// Terminate Xalan...
			XalanTransformer::terminate();

			// Terminate Xerces...
			XMLPlatformUtils::Terminate();

			// Clean up the ICU, if it's integrated...
			XalanTransformer::ICUCleanUp();
		}
		catch(...)
		{
			cerr << "Initialization failed!" << endl;
		}
	}

	return theResult;
}
