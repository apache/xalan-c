#include <cassert>
#include <fstream>
#include <iostream>
#include <strstream>

#include <util/PlatformUtils.hpp>

#include <PlatformSupport/DOMStringHelper.hpp>
#include <DOMSupport/DOMSupportDefault.hpp>

#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathSupportDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>

#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/StylesheetRoot.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>



#include <XercesParserLiaison/XercesDOMSupport.hpp>
#include <XercesParserLiaison/XercesParserLiaison.hpp>



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
StylesheetRoot* glbStylesheetRoot;
XalanNode* glbDocSource;

// Print messages tracking the progress of each thread, and the
// beginning and end of the entire operation.
void outputMessage(DWORD id, char msg[])
{
		ostrstream threadMsg;
	
		threadMsg << "\n" << msg << " Thread: " << id << '\0';
		cout << threadMsg.str();
		threadMsg.freeze(false);
}


THREADFUNCTIONRETURN theThread(LPVOID	param)
{
// This routine uses a compiled stylesheet (glbStylesheetRoot), 
// and a binary source tree (glbSourceDoc) to perform the 
// transformation.

	const int	number = reinterpret_cast<int>(param);
	const DWORD		theThreadID = GetCurrentThreadId();

	// Create the support objects that are necessary for running the processor...
	XercesDOMSupport				theDOMSupport;
	XercesParserLiaison				theParserLiaison(theDOMSupport);
	XPathSupportDefault				theXPathSupport(theDOMSupport);
	XSLTProcessorEnvSupportDefault	theXSLTProcessorEnvSupport;
	XObjectFactoryDefault			theXObjectFactory;
	XPathFactoryDefault				theXPathFactory;

	// Create a processor...and output the start message.
	XSLTEngineImpl	theProcessor(
					theParserLiaison,
					theXPathSupport,
					theXSLTProcessorEnvSupport,
					theDOMSupport,
					theXObjectFactory,
					theXPathFactory);
	outputMessage(theThreadID,"Starting ");

	// Connect the processor to the support object...
	theXSLTProcessorEnvSupport.setProcessor(&theProcessor);

	// The execution context uses the same factory support objects as
	// the processor, since those objects have the same lifetime as
	// other objects created as a result of the execution.
	StylesheetExecutionContextDefault	ssExecutionContext(
						theProcessor,
						theXSLTProcessorEnvSupport,
						theXPathSupport,
						theXObjectFactory);

  // Set the XSLTInputSource...
  XSLTInputSource xslIn(glbDocSource);

	// Generate the output file name for this thread.
    ostrstream theFormatterOut;
    theFormatterOut << "birds" << number << ".out" << '\0';

	//Generate the XML output object.
	XSLTResultTarget	theResultTarget(theFormatterOut.str());

	// Unfreeze the ostrstream, so memory is returned...
	theFormatterOut.freeze(false);

	// Set the processor to use the compiled stylesheet. Then do the transform
  // with the process() method that uses the compiled stylesheet.
	// Report the start of the transformation and the termination of the thread.
	theProcessor.setStylesheetRoot(glbStylesheetRoot);
	outputMessage(theThreadID,"Transforming");
  theProcessor.process(xslIn,theResultTarget,ssExecutionContext);
	outputMessage(theThreadID,"Finishing");
	return (0);
}

// Create and run the threads...
// Print messages tracking the progress of each thread and of the 
// overall operation...
void doThreads(int x)
{
	DWORD dwStackSize = 4096;              	// initial thread stack size
	LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)theThread;
	DWORD dwCreationFlags = 0;             	// creation flags
 	int nThreads = x;

	std::vector<HANDLE> hThreads;
	hThreads.reserve(nThreads);
 	int i = 0;
	
	try
	{
		cout << endl << "Clock before starting threads: " << clock() << endl;

		for (i=0; i< nThreads; i++)
		{
			HANDLE hThread;
			DWORD  threadID;

			hThread = CreateThread(
					0, dwStackSize,
					lpStartAddress,					      // pointer to thread function
					reinterpret_cast<LPVOID>(i),	// argument for new thread
					dwCreationFlags,				      // creation flags
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
	catch(...)
	{
		cerr << "Exception caught!!!"
			 << endl
			<< endl;
	}
}


int main(int argc, const char*	/* argv */[])
{

	if (argc != 1)
	{
		cerr << "Usage: ThreadTest"
			 << endl
			 << endl;
	}
	else
	{
		try
		{
			// Call the static initializer for Xerces...
			XMLPlatformUtils::Initialize();

			{
				// Initialize the Xalan XSLT subsystem...
				XSLTInit						theInit;

				// Create the support objects required to run the processor...
				XercesDOMSupport				ssDOMSupport;
				XercesParserLiaison				ssParserLiaison(ssDOMSupport);
				XPathSupportDefault				ssXPathSupport(ssDOMSupport);
				XSLTProcessorEnvSupportDefault	ssXSLTProcessorEnvSupport;
				XObjectFactoryDefault			ssXObjectFactory;
				XPathFactoryDefault				ssXPathFactory;

				// Create a processor...
				// This processor is used to compile the stylesheet and the source document. 
				// Each thread uses its own processor to perform a transformation.

				XSLTEngineImpl	ssProcessor(
						ssParserLiaison,
						ssXPathSupport,
						ssXSLTProcessorEnvSupport,
						ssDOMSupport,
						ssXObjectFactory,
						ssXPathFactory);

				// Create a stylesheet construction context, using the
				// stylesheet's factory support objects.
				StylesheetConstructionContextDefault	ssConstructionContext(
														ssProcessor,
														ssXSLTProcessorEnvSupport,
														ssXPathFactory);

				const XalanDOMString  theXSLFileName("birds.xsl");
				const XalanDOMString  theXMLFileName("birds.xml");

				// Our stylesheet XML input document and XSL stylesheet
				XSLTInputSource   xmlDocSource(c_wstr(theXMLFileName));
				XSLTInputSource		xslStylesheetSource(c_wstr(theXSLFileName));

				// Use the processor to create a StylesheetRoot for the specified
				// input XSL.  This is the compiled stylesheet.  We don't have to
				// delete it, since it is owned by the StylesheetConstructionContext
				// instance.
				glbStylesheetRoot = ssProcessor.processStylesheet(xslStylesheetSource,
														   ssConstructionContext);
				assert(glbStylesheetRoot != 0);
				
				// Compile the XML source document as well. All threads will use
				// this binary representation of the source tree.
				glbDocSource = ssProcessor.getSourceTreeFromInput(xmlDocSource);
				assert(glbDocSource != 0);

				// Create and run the threads...
				// Each thread uses the same XalanNode and 
				// StylesheetRoot to perform a transformation.
				doThreads(10);
			}

			// Call the static terminator for Xerces...
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
