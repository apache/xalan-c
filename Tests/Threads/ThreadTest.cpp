#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <strstream>

#include <process.h>

#include <util/PlatformUtils.hpp>
#include <util/Mutexes.hpp>

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

extern "C" void theThreadRoutine(void* param);


class SynchronizedCounter
{
public:

	SynchronizedCounter();

	~SynchronizedCounter();

	void
	increment();

	void
	decrement();

	unsigned long
	getCounter() const;

private:

	mutable XMLMutex	m_mutex;

	unsigned long		m_counter;
};



SynchronizedCounter::SynchronizedCounter() :
	m_mutex(),
	m_counter(0)
{
}



SynchronizedCounter::~SynchronizedCounter()
{
}



void
SynchronizedCounter::increment()
{
	XMLMutexLock	theLock(&m_mutex);

	if (m_counter < ULONG_MAX)
	{
		++m_counter;
	}
}



void
SynchronizedCounter::decrement()
{
	XMLMutexLock	theLock(&m_mutex);

	if (m_counter > 0)
	{
		--m_counter;
	}
}



unsigned long
SynchronizedCounter::getCounter() const
{
	XMLMutexLock	theLock(&m_mutex);

	return m_counter;
}



struct
ThreadInfo
{
	ThreadInfo(
			unsigned int			theThreadNumber,
			SynchronizedCounter*	theCounter) :
		m_threadNumber(theThreadNumber),
		m_counter(theCounter)
	{
	}

	unsigned int			m_threadNumber;

	SynchronizedCounter*	m_counter;
};



void
theThreadRoutine(void*		param)
{
// This routine uses compiled stylesheet (glbStylesheetRoot), which is set using the 
// theProcessor.setStylesheetRoot method. The transform is done using the theProcessor's
// process() method.

	const ThreadInfo*	theInfo = reinterpret_cast<const ThreadInfo*>(param);

	assert(theInfo != 0);

	theInfo->m_counter->increment();

	try
	{
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
		const XalanDOMString	theXMLfile("birds.xml");

		const XalanDOMString	theOutputFile(
				XalanDOMString("birds") +
				UnsignedLongToDOMString(theInfo->m_threadNumber) +
				XalanDOMString(".out"));

		//Generate the XML input and output objects.
		XSLTInputSource		theInputSource(c_wstr(theXMLfile));
		XSLTResultTarget	theResultTarget(theOutputFile);

		// Set the stylesheet to be the compiled stylesheet. Then do the transform. 
		// Report both the start of the transform and end of the thread.
		theProcessor.setStylesheetRoot(glbStylesheetRoot);
		theProcessor.process(theInputSource,theResultTarget,ssExecutionContext);
	}
	catch(...)
	{
	}

	// Decrement the counter because we're done...
	theInfo->m_counter->decrement();
}



void
doThreads(long	theThreadCount)
{
	cout << endl << "Starting " << theThreadCount << " threads." << endl;

	typedef std::vector<ThreadInfo>		ThreadInfoVectorType;

	ThreadInfoVectorType	theThreadInfo;

	theThreadInfo.reserve(theThreadCount);

	try
	{
		cout << endl << "Clock before starting threads: " << clock() << endl;

		SynchronizedCounter		theCounter;

		for (long i = 0; i < theThreadCount; i++)
		{
			theThreadInfo.push_back(ThreadInfoVectorType::value_type(i, &theCounter));

			const unsigned long		theThreadID =
					_beginthread(theThreadRoutine, 4096, reinterpret_cast<LPVOID>(&theThreadInfo.back()));

			if (theThreadID == unsigned(-1))
			{
				cerr << endl << "Unable to create thread number " << i + 1 << "." << endl;
			}
		}

		if (theThreadInfo.size() == 0)
		{
			cerr << endl << "No threads were created!" << endl;
		}
		else
		{
			do
			{
				Sleep(2000);
			}
			while(theCounter.getCounter() != 0);

			Sleep(2000);
		}

		cout << endl << "Clock after threads: " << clock() << endl;
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
