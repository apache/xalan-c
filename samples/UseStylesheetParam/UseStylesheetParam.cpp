// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/util/PlatformUtils.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



int
main(
		  int			argc,
		  const char*	argv[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::endl;
#endif

	int	theResult = 0;

	if (argc != 3)
	{
		cerr << "Usage: UseStylesheetParam key expression" << endl;

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

		// Terminate Xalan.
		XalanTransformer::terminate();

		// Call the static terminator for Xerces.
		XMLPlatformUtils::Terminate();
	}

	return theResult;
}
