// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <util/PlatformUtils.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



int
main(
			int				argc,
			const char*		/* argv */[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::endl;
#endif

	int	theResult = 0;

	if (argc != 1)
	{
		cerr << "Usage: SimpleTransform"
			 << endl
			 << endl;

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
			XalanTransformer theXalanTransformer;

			// The assumption is that the executable will be run
			// from same directory as the input files.
			theResult = theXalanTransformer.transform("foo.xml", "foo.xsl", "foo.out");

			if(theResult != 0)
			{
				cerr << "SimpleTransform Error: \n" << theXalanTransformer.getLastError()
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
