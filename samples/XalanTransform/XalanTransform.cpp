#include <xercesc/util/PlatformUtils.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#include <strstream.h>
#else
#include <iostream>
#include <strstream>
#endif



int
main(
			int				 argc,
			const char*		 argv[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cout;
	using std::cerr;
    using std::endl;
#endif

    if (argc < 3 || argc > 4)
	{
		cerr << "Usage: XalanTransform XMLFileName XSLFileName [OutFileName]" << endl;
		return -1;
	}

	// Call the static initializer for Xerces.
	XMLPlatformUtils::Initialize();

    // Initialize Xalan.
    XalanTransformer::initialize();

	int		theResult = 0;

	{
		// Create a XalanTransformer.
		XalanTransformer theXalanTransformer;

		if (argc == 4)
		{
			// Do the transform.
			theResult = theXalanTransformer.transform(argv[1], argv[2], argv[3]);
        
			if(theResult != 0)
			{
				cerr << "XalanError: \n" << theXalanTransformer.getLastError();
			}
		}
		else
		{
			// Do the transform.
			theResult = theXalanTransformer.transform(argv[1], argv[2], cout);
        
			if(theResult != 0)
			{
				cerr << "XalanError: \n" << theXalanTransformer.getLastError();
			}
		}
	}

    // Terminate Xalan.
	XalanTransformer::terminate();

	// Call the static terminator for Xerces.
	XMLPlatformUtils::Terminate();

	return theResult;
}
