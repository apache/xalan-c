// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <cassert>
#include <fstream>
#include <iostream>
#include <strstream>



#include <util/PlatformUtils.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



//This is here for the Windows threads.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
#define THREADFUNCTIONRETURN DWORD WINAPI



#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
	using std::ifstream;
	using std::ios_base;
	using std::ostrstream;
	using std::string;
#endif



// Used to hold compiled stylesheet and XML source document.
XalanCompiledStylesheet*	glbCompiledStylesheet = 0;
XalanParsedSource*			glbParsedSource = 0;
int							glbError = 0;	 



// Print messages tracking the progress of each thread, and the
// beginning and end of the entire operation.
void
outputMessage(
			DWORD		id,
			const char	msg[])
{
	ostrstream threadMsg;
	
	threadMsg << "\n" << msg << " Thread: " << id << '\0';

	cout << threadMsg.str();

	threadMsg.freeze(false);
}



THREADFUNCTIONRETURN
theThread(LPVOID	param)
{
// This routine uses a compiled stylesheet (glbCompiledStylesheet), 
// and a binary source tree (glbParsedSource) to perform the 
// transformation.

	int	theResult = 0;

	const int	number = reinterpret_cast<int>(param);

	const DWORD		theThreadID = GetCurrentThreadId();

	outputMessage(theThreadID, "Starting ");

	// Create a XalanTransformer.
	XalanTransformer	theXalanTransformer;

	// Generate the output file name for this thread.
    ostrstream	theFormatterOut;
    theFormatterOut << "birds" << number << ".out" << '\0';

	// Generate the XML output object.
	const XSLTResultTarget	theResultTarget(XalanDOMString(theFormatterOut.str()));

	// Unfreeze the ostrstream, so memory is returned...
	theFormatterOut.freeze(false);

	outputMessage(theThreadID, "Transforming");

 	// Do the transform.
	theResult = theXalanTransformer.transform(*glbParsedSource, glbCompiledStylesheet, theResultTarget);

	if(theResult != 0)
	{
		cerr << "ThreadSafe Error: \n" << theXalanTransformer.getLastError()
			 << endl
			 << endl;

		glbError = theResult;
	}

	outputMessage(theThreadID, "Finishing");

	return (theResult);
}



// Create and run the threads...
// Print messages tracking the progress of each thread and of the 
// overall operation...
void
doThreads(int	nThreads)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::vector;
#endif

	vector<HANDLE>	hThreads;

	hThreads.reserve(nThreads);

	cout << endl << "Clock before starting threads: " << clock() << endl;

	int		i = 0;	

	for (; i < nThreads; ++i)
	{
		DWORD  threadID;

		const HANDLE	hThread = CreateThread(
				0, 
				4096,							// Stack size for thread.
				theThread,						// pointer to thread function
				reinterpret_cast<LPVOID>(i),	// argument for new thread
				0,								// creation flags
				&threadID);

		assert(hThread != 0);

		hThreads.push_back(hThread);
	}

	WaitForMultipleObjects(hThreads.size(), &hThreads[0], TRUE, INFINITE);

	cout << endl << "Clock after threads: " << clock() << endl;

	for (i = 0; i < nThreads; ++i)
	{
		CloseHandle(hThreads[i]);
	}
}



int
main(
			int				argc,
			const char*		/* argv */[])
{
	if (argc != 1)
	{
		cerr << "Usage: ThreadTest"
			 << endl
			 << endl;
	}
	else
	{
		// Call the static initializer for Xerces.
		XMLPlatformUtils::Initialize();

		// Initialize Xalan.
		XalanTransformer::initialize();

		{
			// Create a XalanTransformer.  We won't actually use this to transform --
			// it's just acting likely a factory for the compiled stylesheet and
			// pre-parsed source.
			XalanTransformer	theXalanTransformer;

			glbCompiledStylesheet =	theXalanTransformer.compileStylesheet("birds.xsl");

			if (glbCompiledStylesheet == 0)
			{
				cerr << "ThreadSafe Error: \n" << theXalanTransformer.getLastError()
					 << endl
					 << endl;
			}
			else
			{
				// Compile the XML source document as well. All threads will use
				// this binary representation of the source tree.
				glbParsedSource = theXalanTransformer.parseSource("birds.xml");

				if (glbParsedSource == 0)
				{
					cerr << "ThreadSafe Error: \n" << theXalanTransformer.getLastError()
						 << endl
						 << endl;
				}
				else
				{
					// Create and run the threads...
					// Each thread uses the same document and 
					// stylesheet to perform a transformation.
					doThreads(10);
				}
			}
		}

		// Terminate Xalan.
		XalanTransformer::terminate();

		// Call the static terminator for Xerces.
		XMLPlatformUtils::Terminate();
	}

	return glbError;
}
