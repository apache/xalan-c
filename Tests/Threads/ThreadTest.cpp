#include <cassert>
#include <fstream>
#include <iostream>
#include <strstream>

#include <util/PlatformUtils.hpp>

#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanFileOutputStream.hpp>
#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>

#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>

#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>

#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/StylesheetRoot.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>


//This is here for the threads.
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

// This is here for memory leak testing.
#if defined(_DEBUG)
#include <crtdbg.h>
#endif

// Used to hold compiled stylesheet
StylesheetRoot* glbStylesheetRoot;


THREADFUNCTIONRETURN theThread(LPVOID	param)
{
// This routine uses compiled stylesheet (glbStylesheetRoot), which is set using the 
// theProcessor.setStylesheetRoot method. The transform is done using the theProcessor's
// process() method.

	const int	number = reinterpret_cast<int>(param);
	const DWORD		theThreadID = GetCurrentThreadId();

	// Create the support objects that are necessary for running the processor...
	XalanSourceTreeDOMSupport		theDOMSupport;
	XalanSourceTreeParserLiaison	theParserLiaison(theDOMSupport);

	theDOMSupport.setParserLiaison(&theParserLiaison);
	// The default is that documents are not thread-safe.  Set this to
	// true so they are.
	//theParserLiaison.setThreadSafe(true);

	XSLTProcessorEnvSupportDefault	theXSLTProcessorEnvSupport;
	XObjectFactoryDefault			theXObjectFactory;
	XPathFactoryDefault				theXPathFactory;

	// Create a processor...and output start message.
	XSLTEngineImpl	theProcessor(
					theParserLiaison,
					theXSLTProcessorEnvSupport,
					theDOMSupport,
					theXObjectFactory,
					theXPathFactory);

	// Connect the processor to the support object...
	theXSLTProcessorEnvSupport.setProcessor(&theProcessor);

	// The execution context uses the same factory support objects as
	// the processor, since those objects have the same lifetime as
	// other objects created as a result of the execution.
	StylesheetExecutionContextDefault	ssExecutionContext(
						theProcessor,
						theXSLTProcessorEnvSupport,
						theDOMSupport,
						theXObjectFactory);

	// Our input files.  The assumption is that the executable will be run
	// from same directory as the input files.

	// Generate the input and output file names.
	char buffer[10];
	const XalanDOMString theXMLfile("birds.xml");
	const XalanDOMString outPutfile(XalanDOMString("birds") + XalanDOMString(itoa(number,buffer,10)) + XalanDOMString(".out"));


	//Generate the XML input and output objects.
	XSLTInputSource		theInputSource(c_wstr(theXMLfile));
	XSLTResultTarget	theResultTarget(outPutfile);

	// Set the stylesheet to be the compiled stylesheet. Then do the transform. 
	// Report both the start of the transform and end of the thread.
	theProcessor.setStylesheetRoot(glbStylesheetRoot);
	theProcessor.process(theInputSource,theResultTarget,ssExecutionContext);

	return (0);
}
void doThreads(int x)
{
	cout << endl << "Starting " << x << " threads." << endl;

	DWORD dwStackSize = 4096;              	// initial thread stack size
	LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)theThread;
	DWORD dwCreationFlags = 0;             	// creation flags
 	int nThreads = x;

	std::vector<HANDLE> hThreads;
	hThreads.reserve(nThreads);
 	int i=0;
	
	try
	{
		cout << endl << "Clock before starting threads: " << clock() << endl;

		for (i=0; i< nThreads; i++)
			{
				HANDLE hThread;
				DWORD  threadID;

				hThread = CreateThread(
					0, dwStackSize,
					lpStartAddress,					// pointer to thread function
					reinterpret_cast<LPVOID>(i),	// argument for new thread
					dwCreationFlags,				// creation flags
					&threadID);
				assert(hThread != 0);
				hThreads.push_back(hThread);
			}
		WaitForMultipleObjects(hThreads.size(), &hThreads[0], TRUE, INFINITE);

		cout << endl << "Clock after threads: " << clock() << endl;

		for (i=0; i< nThreads; i++)
			CloseHandle(hThreads[i]);
	}
	catch(...)
	{
		cerr << "Exception caught!!!"
			 << endl
			<< endl;
	}
}


int
main(
			int				argc,
			const char*		argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	if (argc > 2)
	{
		cerr << "Usage: ThreadTest"
			 << endl
			 << endl;
	}
	else
	{
		int	threadCount = 60;

		if (argc == 2)
		{
			threadCount = atoi(argv[1]);
		}

		try
		{
		  // Call the static initializers...
		  XMLPlatformUtils::Initialize();
          { 
			XSLTInit	theInit;

			// Create the necessary stuff of compile the stylesheet.
			XercesDOMSupport				ssDOMSupport;
			XercesParserLiaison				ssParserLiaison(ssDOMSupport);
			XSLTProcessorEnvSupportDefault	ssXSLTProcessorEnvSupport;
			XObjectFactoryDefault			ssXObjectFactory;
			XPathFactoryDefault				ssXPathFactory;

			// Create a processor to compile the stylesheet...
			XSLTEngineImpl	ssProcessor(
					ssParserLiaison,
					ssXSLTProcessorEnvSupport,
					ssDOMSupport,
					ssXObjectFactory,
					ssXPathFactory);

			// Create separate factory support objects so the stylesheet's
			// factory-created XObject and XPath instances are independent 
			// from processor's.
			XPathFactoryDefault		ssStylesheetXPathFactory;

			// Create a stylesheet construction context, using the
			// stylesheet's factory support objects.
			StylesheetConstructionContextDefault	ssConstructionContext(
													ssProcessor,
													ssXSLTProcessorEnvSupport,
													ssStylesheetXPathFactory);

			const XalanDOMString  theXSLFileName("birds.xsl");
			const XalanDOMString  theXMLFileName("birds.xml");

			// Our stylesheet input source...
			XSLTInputSource		ssStylesheetSourceXSL(c_wstr(theXSLFileName));
			XSLTInputSource		ssStylesheetSourceXML(c_wstr(theXMLFileName));

			// Ask the processor to create a StylesheetRoot for the specified
			// input XSL.  This is the compiled stylesheet.  We don't have to
			// delete it, since it is owned by the StylesheetConstructionContext
			// instance.

			glbStylesheetRoot = ssProcessor.processStylesheet(ssStylesheetSourceXSL,
													   ssConstructionContext);
			assert(glbStylesheetRoot != 0);
			

			doThreads(threadCount);
		  }

		XMLPlatformUtils::Terminate();

		}

		catch(...)
		{
			cerr << "Exception caught!!!"
				 << endl
				 << endl;
		}

	} 

	return 0;
}
