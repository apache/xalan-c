/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <xalanc/Include/PlatformDefinitions.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/sax2/ContentHandler.hpp>
#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/PlatformSupport/AttributesImpl.hpp>



#include <xalanc/XalanTransformer/XalanDocumentBuilder.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>



XALAN_USING_XALAN(XalanDocumentBuilder)
XALAN_USING_XALAN(XalanTransformer)



void
BuildDocument(XalanDocumentBuilder*		theBuilder)
{
	XALAN_USING_XERCES(ContentHandler)
	XALAN_USING_XALAN(AttributesImpl)
	XALAN_USING_XALAN(XalanDOMChar)
	XALAN_USING_XALAN(XalanDOMString)
	XALAN_USING_XALAN(c_wstr)
	XALAN_USING_XALAN(length)

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
	theElementName = XALAN_STATIC_UCODE_STRING("foo");

	theContentHandler->startElement(&theEmptyString, &theEmptyString, c_wstr(theElementName), theAttributes);

	// Create an element child...

	// Set the name of the element...
	theElementName = XALAN_STATIC_UCODE_STRING("foobar");

	// Add an attribute...
	theAttributeName = XALAN_STATIC_UCODE_STRING("attribute1");
	theAttributeValue = XALAN_STATIC_UCODE_STRING("value1");

	theAttributes.addAttribute(c_wstr(theAttributeName), c_wstr(theAttributeType), c_wstr(theAttributeValue));

	theContentHandler->startElement(&theEmptyString, &theEmptyString, c_wstr(theElementName), theAttributes);

	// Add a text node...
	theTextValue = XALAN_STATIC_UCODE_STRING("The first foobar");

	theContentHandler->characters(c_wstr(theTextValue), length(theTextValue));

	// End the element...
	theContentHandler->endElement(&theEmptyString, &theEmptyString, c_wstr(theElementName));

	theAttributes.clear();
	theAttributeName = XALAN_STATIC_UCODE_STRING("attribute2");
	theAttributeValue = XALAN_STATIC_UCODE_STRING("value2");
	theAttributes.addAttribute(c_wstr(theAttributeName), c_wstr(theAttributeType), c_wstr(theAttributeValue));

	theContentHandler->startElement(&theEmptyString, &theEmptyString, c_wstr(theElementName), theAttributes);

	theTextValue = XALAN_STATIC_UCODE_STRING("The second foobar");
	theContentHandler->characters(c_wstr(theTextValue), length(theTextValue));

	theContentHandler->endElement(&theEmptyString, &theEmptyString, c_wstr(theElementName));

	theAttributes.clear();
	theAttributeName = XALAN_STATIC_UCODE_STRING("attribute3");
	theAttributeValue = XALAN_STATIC_UCODE_STRING("value3");
	theAttributes.addAttribute(c_wstr(theAttributeName), c_wstr(theAttributeType), c_wstr(theAttributeValue));

	theContentHandler->startElement(&theEmptyString, &theEmptyString, c_wstr(theElementName), theAttributes);

	theTextValue = XALAN_STATIC_UCODE_STRING("The third foobar");
	theContentHandler->characters(c_wstr(theTextValue), length(theTextValue));

	theContentHandler->endElement(&theEmptyString, &theEmptyString, c_wstr(theElementName));

	// end the document element...
	theElementName = XALAN_STATIC_UCODE_STRING("foo");

	theContentHandler->endElement(&theEmptyString, &theEmptyString, c_wstr(theElementName));

	// end the document...
	theContentHandler->endDocument();
}



int
main(
		  int		argc,
		  char*		/* argv */ [])
{
	XALAN_USING_STD(cerr)
	XALAN_USING_STD(endl)

	int	theResult = 0;

	if (argc != 1)
	{
		cerr << "Usage: DocumentBuilder" << endl;

		theResult = -1;
	}
	else
	{
		XALAN_USING_XERCES(XMLPlatformUtils)
		XALAN_USING_XERCES(XMLException)

		// Call the static initializer for Xerces.
		try
		{
			 XMLPlatformUtils::Initialize();
		}
		catch (const XMLException& toCatch)
		{
			 cerr << "Error during Xerces initialization.  Error code was "
                  << toCatch.getCode()
                  << "."
                  << endl;

			 theResult = -1;
		}

		if (theResult == 0)
		{
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
		}

		// Call the static terminator for Xerces.
		XMLPlatformUtils::Terminate();

		// Clean up the ICU, if it's integrated.
		XalanTransformer::ICUCleanUp();
	}

	return theResult;
}
