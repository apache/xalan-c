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



#include <XalanTransformer/XalanTransformer.hpp>



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
#if defined(XALAN_STRICT_ANSI_HEADERS)
		using std::strlen;
#endif
#endif

	int	theResult = 0;

	if (argc != 1)
	{
		cerr << "Usage: StreamTransform"
			 << endl
			 << endl;
	}
	else
	{
		// Call the static initializer for Xerces.
		XMLPlatformUtils::Initialize();

		// Initialize Xalan.
		XalanTransformer::initialize();

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

		XSLTInputSource	inputSource(&theXSLStream);

		inputSource.setSystemId(c_wstr(XalanDOMString("foo")));

		// Do the transform.
		theResult = theXalanTransformer.transform(&theXMLStream, inputSource, cout);
    
		if(theResult != 0)
		{
			cerr << "StreamTransform Error: \n" << theXalanTransformer.getLastError()
				 << endl
				 << endl;
		}

		// Terminate Xalan.
		XalanTransformer::terminate();

		// Call the static terminator for Xerces.
		XMLPlatformUtils::Terminate();
	}

	return theResult;
}
