#include <cassert>
#include <iostream>



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



#if defined(WIN32)
//This is here for the threads.
#include <process.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(XALAN_POSIX2_AVAILABLE)
#include <pthread.h>
#include <unistd.h>
#else
#error Unsupported platform!
#endif



#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
#endif


	
// This is here for memory leak testing.
#if defined(_DEBUG)
#include <crtdbg.h>
#endif

// Used to hold compiled stylesheet
StylesheetRoot* glbStylesheetRoot;


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



#if defined(WIN32)

extern "C" void theThreadRoutine(void* param);

void
#elif defined(XALAN_POSIX2_AVAILABLE)

extern "C" void* theThreadRoutine(void* param);

void*
#else
#error Unsupported platform!
#endif
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

#if defined(XALAN_POSIX2_AVAILABLE)
	return 0;
#endif
}



inline void
doSleep(unsigned int	theMilliseconds)
{
#if defined(WIN32)
	Sleep(theMilliseconds);
#elif defined(XALAN_POSIX2_AVAILABLE)
	usleep(theMilliseconds * 10);
#else
#error Unsupported platform!
#endif
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

#if defined(WIN32)

			const unsigned long		theThreadID =
					_beginthread(theThreadRoutine, 4096, reinterpret_cast<LPVOID>(&theThreadInfo.back()));

			if (theThreadID == unsigned(-1))
			{
				cerr << endl << "Unable to create thread number " << i + 1 << "." << endl;
			}

#elif defined(XALAN_POSIX2_AVAILABLE)

			pthread_t	theThread;

			const int	theResult = pthread_create(&theThread, 0, theThreadRoutine, (void*)&theThreadInfo.back());

			if (theResult != 0)
			{
				cerr << endl << "Unable to create thread number " << i + 1 << "." << endl;
			}
			else
			{
				pthread_detach(theThread);
			}
#else
#error Unsupported platform!
#endif
		}

		clock_t		theClock = 0;

		if (theThreadInfo.size() == 0)
		{
			cerr << endl << "No threads were created!" << endl;
		}
		else
		{
			unsigned int	theCheckCount = 0;

			do
			{
				doSleep(2000);

				// Check a couple of times, just in case, since
				// getCounter() is not synchronized...
				if (theCounter.getCounter() == 0)
				{
					if (theCheckCount == 0)
					{
						theClock = clock();
					}

					++theCheckCount;
				}
			}
			while(theCheckCount < 2);
		}

		cout << endl << "Clock after threads: " << theClock << endl;
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
