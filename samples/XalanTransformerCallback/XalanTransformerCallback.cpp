#include <util/PlatformUtils.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



#include <cstdio>

#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif



// This is a simple class that illustrates how XalanTransformer's "callback" API
// is used.  This example just abstracts writing data to a FILE*, but other
// actions are possible.
class CallbackHandler
{
public:

	CallbackHandler(FILE*	theFile) :
		m_file(theFile)
	{
		assert(m_file != 0);
	}

	unsigned long
	write(
			const char*		theData,
			unsigned long	theLength)
	{
		return fwrite(theData, sizeof(char), theLength, m_file);
	}

	void
	flush()
	{
		fflush(m_file);
	}

private:

	FILE* const		m_file;
};



extern "C"
{

// This is the write callback function, which casts the handle
// to the appropriate type, then calls the write() member function
// on the CallbackHandler class.
unsigned long
writeCallback(
			const char*		theData,
			unsigned long	theLength,
			void*			theHandle)
{
#if defined(XALAN_OLD_STYLE_CASTS)
	return ((CallbackHandler*)theHandle)->write(theData, theLength);
#else
	return reinterpret_cast<CallbackHandler*>(theHandle)->write(theData, theLength);
#endif
}

// This is the fluxh callback function, which casts the handle
// to the appropriate type, then calls the flush() member function
// on the CallbackHandler class.
void
flushCallback(void*	theHandle)
{
#if defined(XALAN_OLD_STYLE_CASTS)
	((CallbackHandler*)theHandle)->flush();
#else
	reinterpret_cast<CallbackHandler*>(theHandle)->flush();
#endif
}

};



int
doTransform(
			const char*		theXMLFile,
			const char*		theXSLFile,
			FILE*			theOutputFile)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
    using std::endl;
#endif

    // Create a XalanTransformer.
	XalanTransformer	theXalanTransformer;

	// Do the transform.
	CallbackHandler		theHandler(theOutputFile);

	// Do the transform.
	const int	theResult = theXalanTransformer.transform(
					theXMLFile,
					theXSLFile,
					&theHandler,
					writeCallback,
					flushCallback);
        
	if(theResult != 0)
	{
		cerr << "XalanError: " << theXalanTransformer.getLastError() << endl;
	}

	return theResult;
}



int
main(
			int				 argc,
			const char*		 argv[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
    using std::endl;
#endif

    if (argc < 3 || argc > 4)
	{
		cerr << "Usage: XalanTransformerCallback XMLFileName XSLFileName [OutFileName]" << endl;

		return -1;
	}

	// Call the static initializer for Xerces.
	XMLPlatformUtils::Initialize();

    // Initialize Xalan.
    XalanTransformer::initialize();

    int				theResult = 0;

    if (argc == 3)
	{
		// No output file, so use stdout...
		theResult = doTransform(argv[1], argv[2], stdout);
    }
    else
    {
		// Ooutput file specified, so try to open it...
		FILE* const	theOutputFile = fopen(argv[3], "w");

		if (theOutputFile == 0)
		{
			cerr << "Error: " << "Unable to open output file " << argv[3] << endl;
		}
		else
		{
			theResult = doTransform(argv[1], argv[2], theOutputFile);

			fclose(theOutputFile);
		}
    }

    // Terminate Xalan.
	XalanTransformer::terminate();

	// Call the static terminator for Xerces.
	XMLPlatformUtils::Terminate();

	return theResult;
}
