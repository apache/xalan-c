// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <sax2/ContentHandler.hpp>
#include <util/PlatformUtils.hpp>



#include <PlatformSupport/AttributesImpl.hpp>



#include <XalanTransformer/XalanDocumentBuilder.hpp>
#include <XalanTransformer/XalanTransformer.hpp>



void
BuildDocument(XalanDocumentBuilder*		theBuilder)
{
	// Get the SAX2 ContentHandler from the builder...
	ContentHandler* const	theContentHandler = theBuilder->getContentHandler();
	assert(theContentHandler != 0);

	// This will hold the attributes for the elements we create...
	AttributesImpl	theAttributes;

	// Some handy scratch strings for adding elements, attributes, and text nodes...
	XalanDOMString			theElementName;
	XalanDOMString			theAttributeName;
	XalanDOMString			theAttributeValue;
	const XalanDOMString	theAttributeType("CDATA");
	XalanDOMString			theTextValue;
	const XalanDOMChar		theEmptyString = 0;

	// start the document...
	theContentHandler->startDocument();

	// start the document element...
	assign(theElementName, XALAN_STATIC_UCODE_STRING("foo"));

	theContentHandler->startElement(&theEmptyString, &theEmptyString, c_wstr(theElementName), theAttributes);

	// Create an element child...

	// Set the name of the element...
	assign(theElementName, XALAN_STATIC_UCODE_STRING("foobar"));

	// Add an attribute...
	assign(theAttributeName, XALAN_STATIC_UCODE_STRING("attribute1"));
	assign(theAttributeValue, XALAN_STATIC_UCODE_STRING("value1"));
	theAttributes.addAttribute(c_wstr(theAttributeName), c_wstr(theAttributeType), c_wstr(theAttributeValue));

	theContentHandler->startElement(&theEmptyString, &theEmptyString, c_wstr(theElementName), theAttributes);

	// Add a text node...
	assign(theTextValue, XALAN_STATIC_UCODE_STRING("The first foobar"));
	theContentHandler->characters(c_wstr(theTextValue), length(theTextValue));

	// End the element...
	theContentHandler->endElement(&theEmptyString, &theEmptyString, c_wstr(theElementName));

	theAttributes.clear();
	assign(theAttributeName, XALAN_STATIC_UCODE_STRING("attribute2"));
	assign(theAttributeValue, XALAN_STATIC_UCODE_STRING("value2"));
	theAttributes.addAttribute(c_wstr(theAttributeName), c_wstr(theAttributeType), c_wstr(theAttributeValue));

	theContentHandler->startElement(&theEmptyString, &theEmptyString, c_wstr(theElementName), theAttributes);

	assign(theTextValue, XALAN_STATIC_UCODE_STRING("The second foobar"));
	theContentHandler->characters(c_wstr(theTextValue), length(theTextValue));

	theContentHandler->endElement(&theEmptyString, &theEmptyString, c_wstr(theElementName));

	theAttributes.clear();
	assign(theAttributeName, XALAN_STATIC_UCODE_STRING("attribute3"));
	assign(theAttributeValue, XALAN_STATIC_UCODE_STRING("value3"));
	theAttributes.addAttribute(c_wstr(theAttributeName), c_wstr(theAttributeType), c_wstr(theAttributeValue));

	theContentHandler->startElement(&theEmptyString, &theEmptyString, c_wstr(theElementName), theAttributes);

	assign(theTextValue, XALAN_STATIC_UCODE_STRING("The third foobar"));
	theContentHandler->characters(c_wstr(theTextValue), length(theTextValue));

	theContentHandler->endElement(&theEmptyString, &theEmptyString, c_wstr(theElementName));

	// end the document element...
	assign(theElementName, XALAN_STATIC_UCODE_STRING("foo"));

	theContentHandler->endElement(&theEmptyString, &theEmptyString, c_wstr(theElementName));

	// end the document...
	theContentHandler->endDocument();
}



int
main(
		  int			argc,
		  const char*	/* argv */ [])
{
#if !defined(XALAN_NO_NAMESPACES)
  using std::cerr;
  using std::endl;
#endif

	int	theResult = 0;

	if (argc != 1)
	{
		cerr << "Usage: DocumentBuilder" << endl;

		theResult = -1;
	}
	else
	{
		// Call the static initializer for Xerces.
		XMLPlatformUtils::Initialize();

		// Initialize Xalan.
		XalanTransformer::initialize();

		{
			// Create a XalanTransformer.
			XalanTransformer	theXalanTransformer;

			// Get a document builder from the transformer...
			XalanDocumentBuilder* const		theBuilder = theXalanTransformer.createDocumentBuilder();

			BuildDocument(theBuilder);

			// The assumption is that the executable will be run
			// from same directory as the stylesheet file.

			// Do the transform.
			theResult = theXalanTransformer.transform(*theBuilder, "foo.xsl", "foo.out");
    
			if(theResult != 0)
			{
				cerr << "DocumentBuilder error: \n" << theXalanTransformer.getLastError()
					 << endl
					 << endl;
			}
		}

		// Terminate Xalan.
		XalanTransformer::terminate();

		// Call the static terminator for Xerces.
		XMLPlatformUtils::Terminate();
	}

	return theResult;
}
