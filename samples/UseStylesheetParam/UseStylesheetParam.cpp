// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <util/PlatformUtils.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



int
main(
		  int				     argc,
		  const char*		 argv [])
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

		// Create a XalanTransformer.
		XalanTransformer theXalanTransformer;

		// Our input files...The assumption is that the executable will be run
		// from same directory as the input files.
		const char*		theXMLFileName = "foo.xml";
		const char*		theXSLFileName = "foo.xsl";

		// Our output target...
		const char*	theOutputFileName = "foo.out";

		// Get the stylesheet parameter key (name) and
		// expression (a string expression).
		const char*	paramKey = argv[1];
		const char*	paramExpression = argv[2];

		// Set the stylesheet parameter.
		theXalanTransformer.setStylesheetParam(paramKey, paramExpression);

		// Do the transform.
		theResult = theXalanTransformer.transform(theXMLFileName, theXSLFileName, theOutputFileName);
    
		if(theResult != 0)
		{
			cerr << "UseStylesheetParam Error: \n" << theXalanTransformer.getLastError()
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
