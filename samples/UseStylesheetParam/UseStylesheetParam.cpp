// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



int
main(
		  int			argc,
		  const char*	argv[])
{
	XALAN_USING_STD(cerr)
	XALAN_USING_STD(endl)

	int	theResult = 0;

	if (argc != 3)
	{
		cerr << "Usage: UseStylesheetParam key expression" << endl;

		theResult = -1;
	}
	else
	{
		XALAN_USING_XERCES(XMLPlatformUtils)

		XALAN_USING_XALAN(XalanTransformer)

		// Call the static initializer for Xerces.
		XMLPlatformUtils::Initialize();

		// Initialize Xalan.
		XalanTransformer::initialize();

		{
			XALAN_USING_XALAN(XalanDOMString)

			// Create a XalanTransformer.
			XalanTransformer	theXalanTransformer;

			// Set the stylesheet parameter name and
			// expression (a string expression).
			theXalanTransformer.setStylesheetParam(
					XalanDOMString(argv[1]),
					XalanDOMString(argv[2]));

			// Our input files...The assumption is that the executable will be run
			// from same directory as the input files.
			theResult = theXalanTransformer.transform("foo.xml", "foo.xsl", "foo.out");

			if(theResult != 0)
			{
				cerr << "UseStylesheetParam Error: \n" << theXalanTransformer.getLastError()
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

	return theResult;
}
